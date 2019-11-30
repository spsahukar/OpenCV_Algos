//  Copyright © 2019 Surya Sahukar. All rights reserved.

#include "cct_lut.hpp"
#include "cct.hpp"
#include "cct_lut_xy.hpp"
using namespace cv;
using namespace std;
/*References
 https://www.usna.edu/Users/oceano/raylee/papers/RLee_AO_CCTpaper.pdf
 https://dsp.stackexchange.com/questions/8949/how-do-i-calculate-the-color-temperature-of-the-light-source-illuminating-an-ima
 https://andi-siess.de/rgb-to-color-temperature/
 */
float calc_cct(cv::Mat img)
{
    //Convert to XYX color space
    cv::Mat xyz_img;
    uint32_t rows = 0, cols = 0;
    float X = 0, Y = 0, Z = 0;
    float B = 0, G = 0, R = 0;
    float x = 0.0, y = 0.0, n = 0.0;
    float CCT = 0.0;
    
    cvtColor(img, xyz_img, COLOR_BGR2XYZ, 1);
    rows = xyz_img.rows;
    cols = xyz_img.cols;

    printf("rows = %d, cols = %d\n", rows, cols);
    
    for (int i = 0; i < xyz_img.rows; i++)
    {
        for (int j =0; j < xyz_img.cols; j++)
        {
            X += xyz_img.at<Vec3b>(i,j)[0];
            Y += xyz_img.at<Vec3b>(i,j)[1];
            Z += xyz_img.at<Vec3b>(i,j)[2];
            
            B += img.at<Vec3b>(i,j)[0];
            G += img.at<Vec3b>(i,j)[1];
            R += img.at<Vec3b>(i,j)[2];
        }
    }
    
    X = X / (rows * cols);
    Y = Y / (rows * cols);
    Z = Z / (rows * cols);
    
    B = B / (rows * cols);
    G = G / (rows * cols);
    R = R / (rows * cols);
    
    printf("X = %f, Y = %f, Z = %f\n", X, Y, Z);
    printf("B = %f, G = %f, R = %f\n", B, G, R);
    // NOrmalized values
    x = X / (X + Y + Z);
    y = Y / (X + Y + Z);
    
    n = (x - 0.3320) / (0.1858 - y);
    
    CCT = 449 * n * n * n + 3525 * n * n + 6823.3 * n + 5520.33;
    
    return CCT;
}

void get_cct(string filename)
{
	cout << "File Name " << filename << endl;
    std::string input_image_name = filename;
    cv::Mat i_img = cv::imread( input_image_name, 1);
    float cct = calc_cct(i_img);
    printf("Color Temp = %f\n", cct);
}

uint32_t nearest100(uint32_t temp)
{
    uint8_t rem  = temp % 100;
    if (rem < 50)
        return temp - rem;
    else
        return temp - rem + 100;
}

void split(string str, string delim, vector<string>& words)
{
    char *token;
    
    token = strtok( &str[0], &delim[0] );
    while( token != NULL )
    {
        words.push_back(token);
        token = strtok( NULL, &delim[0] );
        
    }
}

string create_filename(string filename, uint32_t temp)
{
	vector <string> path;
	vector <string> fname;
	int len;
	
	split(filename, "/", path);
	len = path.size();
	
	split(path[len -1], ".", fname);
	string o_filename = path[0] + "/" + path[1] + "/" + fname[0] + "_" + to_string(temp) + "." + fname[1];
	
	cout << "o_filename " << o_filename << endl;
	return o_filename;
}


void set_cct_rgb(string filename, uint32_t temp)
{
	cout << "File Name " << filename << endl;
	string o_filename;
	uint32_t cct100;
	struct rgb rgb_cct;
	std::string input_image_name;
	Mat i_img;
	uint32_t rows = 0, cols = 0;
    float B = 0, G = 0, R = 0;
    float B_gain = 0, G_gain = 0, R_gain = 0;
	vector<int> compression_params;

	o_filename = create_filename(filename, temp);
	
	cct100 = nearest100(temp);
	cout << "nearest 100 = " << cct100 << endl;;
	
	rgb_cct = kelvin_table[cct100];
	//cout << "r = " << rgb_cct.r << "g = " << rgb_cct.g << "b = " << rgb_cct.b << endl;
	printf("r = %d g = %d b = %d\n ", rgb_cct.r, rgb_cct.g, rgb_cct.b);

	//Mat Z = Mat::zeros(3, 3, CV_32F);
		
	//Mat Z = (Mat_<CV_32F>(3,3) << rgb_cct.r/255, 0, 0, 0, rgb_cct.g/255, 0, 0, 0, rgb_cct.b/255);
	
	input_image_name = filename;
	i_img = cv::imread(input_image_name, 1);
	 
	rows = i_img.rows; cols = i_img.cols;
	printf("rows = %d, cols = %d\n", rows, cols); 
	
	//Mat o_img = (Mat_<double>(rows,cols));
	//o_img = i_img.convertTo(i_img, CV_32F) * Z;
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
        {
            B += i_img.at<Vec3b>(i,j)[0];
            G += i_img.at<Vec3b>(i,j)[1];
            R += i_img.at<Vec3b>(i,j)[2];
        }
    }

    B = B / (rows * cols);
    G = G / (rows * cols);
    R = R / (rows * cols);

	printf("B = %f, G = %f, R = %f\n", B, G, R);
	B_gain = rgb_cct.b / B;
	G_gain = rgb_cct.g / G;
	R_gain = rgb_cct.r / R;

	printf("________________________________________B_gain = %f, G_gain = %f, R_gain = %f\n____________________________________", B_gain, G_gain, R_gain);
	for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
			//printf("Before converstion = %d, %d, %d\n", i_img.at<Vec3b>(i,j)[0], i_img.at<Vec3b>(i,j)[1], i_img.at<Vec3b>(i,j)[2]);
            i_img.at<Vec3b>(i,j)[0] =  i_img.at<Vec3b>(i,j)[0] * B_gain;
			i_img.at<Vec3b>(i,j)[1] = i_img.at<Vec3b>(i,j)[1] * 1;
			i_img.at<Vec3b>(i,j)[2] = i_img.at<Vec3b>(i,j)[2] * R_gain;
			//printf("after converstion = %d, %d, %d\n", i_img.at<Vec3b>(i,j)[0], i_img.at<Vec3b>(i,j)[1], i_img.at<Vec3b>(i,j)[2]);
        }
    }

	//compression_params.push_back(IMWRITE_JPEG_QUALITY);
	//compression_params.push_back(100);
	cv::imwrite(o_filename, i_img);
}



void set_cct_cie_xy(string filename, uint32_t temp)
{
    cout << "File Name " << filename << endl;
    string o_filename;
    uint32_t cct100;
	struct cie_xy xy_cct;
    std::string input_image_name;
    Mat i_img;
    uint32_t rows = 0, cols = 0;
	float X = 0, Y = 0, Z = 0, Xp = 0 , Yp = 0;
    float B = 0, G = 0, R = 0;
    float xp = 0.0, yp = 0.0;
	float X_gain = 0.0, Y_gain = 0.0;

    vector<int> compression_params;
	cv::Mat xyz_img;

    o_filename = create_filename(filename, temp);

    cct100 = nearest100(temp);
    cout << "nearest 100 = " << cct100 << endl;;

    xy_cct = kelvin_table_xy[cct100];
    //cout << "r = " << rgb_cct.r << "g = " << rgb_cct.g << "b = " << rgb_cct.b << endl;
    printf("x = %f y = %f\n ", xy_cct.x, xy_cct.y);
	xp = xy_cct.x;
	yp = xy_cct.y;
    //Mat Z = Mat::zeros(3, 3, CV_32F);

    //Mat Z = (Mat_<CV_32F>(3,3) << rgb_cct.r/255, 0, 0, 0, rgb_cct.g/255, 0, 0, 0, rgb_cct.b/255);

    input_image_name = filename;
    i_img = cv::imread(input_image_name, 1);

    rows = i_img.rows; cols = i_img.cols;
    printf("rows = %d, cols = %d\n", rows, cols);
	cvtColor(i_img, xyz_img, COLOR_BGR2XYZ, 1);
	
	for (int i = 0; i < xyz_img.rows; i++)
    {   
        for (int j =0; j < xyz_img.cols; j++)
        {   
            X += xyz_img.at<Vec3b>(i,j)[0];
            Y += xyz_img.at<Vec3b>(i,j)[1];
            Z += xyz_img.at<Vec3b>(i,j)[2];
            
        }
    }
    
    X = X / (rows * cols);
    Y = Y / (rows * cols);
    Z = Z / (rows * cols);

	Xp =  (xp * (Y + Z)) / (1 - xp);
	Yp =  (yp * (X + Z)) / (1 - yp);
	
	X_gain = Xp / X;
	Y_gain = Yp / Y;

	for (int i = 0; i < xyz_img.rows; i++)
    {
        for (int j =0; j < xyz_img.cols; j++)
        {
            xyz_img.at<Vec3b>(i,j)[0] *= X_gain;;
            xyz_img.at<Vec3b>(i,j)[1] *= Y_gain;
        }
    }
	cvtColor(xyz_img, i_img, COLOR_XYZ2BGR, 1);
    //compression_params.push_back(IMWRITE_JPEG_QUALITY);
    //compression_params.push_back(100);
    cv::imwrite(o_filename, i_img);
}

void help()
{
	printf("please check input\n\
	\t ./cct <get/set> <filename> <color_temp (K)\n\
	\t to get color temp:\n\
	\t\t ./cct get monalisa.jpg\n\
	\t to set color temp:\n\
	\t\t ./cct set monalisa.jpg 1000K\n\
	\t for help:\n\
	\t\t ./cct help\n");
	
	exit(1);
}

int main(int argc, char *argv[])
{
    if ((argc ==  3) && !strcmp(argv[1] ,"get"))
	    get_cct(argv[2]);
    else if ((argc == 4) && !strcmp(argv[1], "set"))
		set_cct_rgb(argv[2], atoi(argv[3]));
    else if (argv[1] == "help")
	    help();
    else
	    help();
	return 0;
}

