//  Copyright © 2019 Surya Sahukar. All rights reserved.

#include "debayer.hpp"
using namespace cv;
using namespace std;

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

string create_filename(string filename, string str)
{
    vector <string> path;
    vector <string> fname;
    int len;

    split(filename, "/", path);
    len = path.size();

    split(path[len -1], ".", fname);
    string o_filename = path[0] + "/" + path[1] + "/" + fname[0] + "_" + str + "." + fname[1];

    cout << "o_filename " << o_filename << endl;
    return o_filename;
}

int help()
{
	printf("\t\tplease check input\n\
			\t./debayer <bayer/debayer> <filename>\n\
			to change jpeg to bayer:\n\
			\t./debayer bayer monalisa.jpg\n\
			to debayer image from bayer (jpeg):\n\
			\t./debayer debayer monalisa_debayer.jpg\n\
			for help:\n\
			\t./debayer help\n");
	exit(1);
}

void bayerit(string filename)
{
  	uint32_t rows = 0, cols = 0;
 	Mat i_img, B, G, R, o_img;
	string o_filename;
	
	i_img = imread(filename, 1);
	rows = i_img.rows;
	cols = i_img.cols;
	cout << "rows = " << rows << " cols = " << cols << endl;

	Vec3b bgr;	

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			bgr = i_img.at<Vec3b>(i,j);
			
			if ( (i % 2) ||  (j % 2) )
				bgr.val[0] = 0;
			if ( !(i % 2) ||  !(j % 2))
				bgr.val[2] = 0;
  			if ( ( !(i % 2) &&  !(j % 2) ) || ( (i % 2) &&  (j % 2) ) )
				 bgr.val[1] = 0;

			i_img.at<Vec3b>(i,j) = bgr;
		}
	}
	printf("[%s %d]\n", __func__, __LINE__);	
	o_filename = create_filename(filename, "bayer");
	imwrite(o_filename,  i_img);

}
void debayer(string filename)
{
	uint32_t rows = 0, cols = 0;
    Mat i_img, B, G, R, o_img;
    string o_filename;
	uint8_t r1 = 0, r2 = 0, r3 = 0, r4 = 0;
	uint8_t b1 = 0, b2 = 0, b3 = 0, b4 = 0;
	uint8_t g1 = 0, g2 = 0, g3 = 0, g4 = 0;

	cout << "filename " << filename << endl;	
	i_img = imread(filename, 1);
    rows = i_img.rows;
    cols = i_img.cols;
    cout << "rows = " << rows << " cols = " << cols << endl;

   	Vec3b bgr;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            bgr = i_img.at<Vec3b>(i,j);

            if ( !(i % 2) &&  !(j % 2) )
				{
					if(((i - 1) >= 0) && ((j - 1) >= 0))
						r1 = i_img.at<Vec3b>(i - 1, j - 1)[2];
					else
						r1 = 0;
					if(((i - 1) >= 0) && ((j + 1) <= cols))
						r2 = i_img.at<Vec3b>(i - 1, j + 1)[2];
					else
						r2 = 0;
					if(((i + 1) <= rows) && ((j - 1) >= 0))
						r3 = i_img.at<Vec3b>(i + 1, j - 1)[2];
					else
						r3 = 0;
                    if(((i + 1) <= rows) && ((j -+ 1) <= cols))
                        r4 = i_img.at<Vec3b>(i + 1, j + 1)[2];
                    else
                        r4 = 0;

					if(j - 1 >= 0)
						g1 = i_img.at<Vec3b>(i, j - 1)[1];
					else 
						g1 = 0;
					if(i - 1 >= 0)
						g2 = i_img.at<Vec3b>(i - 1, j)[1];
					else 
						g2 = 0;
					 if(j + 1 <= cols)
                        g3 = i_img.at<Vec3b>(i, j + 1)[1];
                    else
                        g3 = 0;
 					if(i + 1 <= rows)
                        g4 = i_img.at<Vec3b>(i + 1, j)[1];
                    else
                        g4 = 0;
					

					i_img.at<Vec3b>(i,j)[1] = (g1 + g2 + g3 + g4)/4;
					i_img.at<Vec3b>(i,j)[2] = (r1 + r2 + r3 + r4)/4;
				}

			if ( (i % 2) && (j %2))
				{
					if(((i - 1) >= 0) && ((j - 1) >= 0))
                        b1 = i_img.at<Vec3b>(i - 1, j - 1)[0];
                    else
                        b1 = 0;
                    if(((i - 1) >= 0) && ((j + 1) <= cols))
                        b2 = i_img.at<Vec3b>(i - 1, j + 1)[0];
                    else
                        b2 = 0;
                    if(((i + 1) <= rows) && ((j - 1) >= 0))
                        b3 = i_img.at<Vec3b>(i + 1, j - 1)[0];
                    else
                        b3 = 0;
                    if(((i + 1) <= rows) && ((j -+ 1) <= cols))
                        b4 = i_img.at<Vec3b>(i + 1, j + 1)[0];
                    else
                        b4 = 0;

                    if(j - 1 >= 0)
                        g1 = i_img.at<Vec3b>(i, j - 1)[1];
                    else 
                        g1 = 0;
                    if(i - 1 >= 0)
                        g2 = i_img.at<Vec3b>(i - 1, j)[1];
                    else
                        g2 = 0;
                     if(j + 1 <= cols)
                        g3 = i_img.at<Vec3b>(i, j + 1)[1];
                    else
                        g3 = 0;
                    if(i + 1 <= rows)
                        g4 = i_img.at<Vec3b>(i + 1, j)[1];
                    else
                        g4 = 0;

				    i_img.at<Vec3b>(i,j)[1] = (g1 + g2 + g3 + g4)/4;
                    i_img.at<Vec3b>(i,j)[0] = (b1 + b2 + b3 + b4)/4;
				}

			if ( !(i % 2) && (j % 2)) 
				{
					if ( j - 1 >= 0)
						b1 = i_img.at<Vec3b>(i, j - 1)[0];
					else
						b1 = 0;
					if ( j + 1 <= cols)
                        b2 = i_img.at<Vec3b>(i, j + 1)[0];
                    else
                        b2 = 0;
					if ( i - 1 >= 0)
						r1 = i_img.at<Vec3b>(i - 1, j)[2];
					else
						r1 = 0;
				  	if ( i + 1 <= 0)
                        r2 = i_img.at<Vec3b>(i + 1, j)[2];
                    else
                        r2 = 0;

					i_img.at<Vec3b>(i,j)[2] = (r1 + r2 )/2;
					i_img.at<Vec3b>(i,j)[0] = (b1 + b2)/2;
				}
				
            if ( (i % 2) && !(j % 2)) 
                {
                    if ( j - 1 >= 0)
                        r1 = i_img.at<Vec3b>(i, j - 1)[2];
                    else
                        r1 = 0;
                    if ( j + 1 <= cols)
                        r2 = i_img.at<Vec3b>(i, j + 1)[2];
                    else
                        b2 = 0;
                    if ( i - 1 >= 0)
                        b1 = i_img.at<Vec3b>(i - 1, j)[0];
                    else
                        b1 = 0;
                    if ( i + 1 <= 0)
                        b2 = i_img.at<Vec3b>(i + 1, j)[0];
                    else
                        b2 = 0;

                    i_img.at<Vec3b>(i,j)[2] = (r1 + r2 )/2;
                    i_img.at<Vec3b>(i,j)[0] = (b1 + b2)/2;
                }
				
        }
    }	
	o_filename = create_filename(filename, "debayer");
	imwrite(o_filename,  i_img);
	
}

int main(int argc, char *argv[])
{
	//string filename = "../images/monalisa_bayer.jpg";
	//bayerit(filename);
   //debayer(filename); 
	if (argc == 3)
	{
		printf("[%s %d argv[1] = %s\n", __func__, __LINE__, argv[1]);
    	if (!(strcmp(argv[1], "bayer")))
	     	bayerit(argv[2]);
    	if (!(strcmp(argv[1], "debayer")))
	    	debayer(argv[2]);
	}
	else
		help();
}

