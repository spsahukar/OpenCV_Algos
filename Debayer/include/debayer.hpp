//  cct.hpp

#ifndef CCT_hpp
#define CCT_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <string>
float calc_cct(cv::Mat);
void get_cct(std::string);
#endif /* CCT_hpp */

