//  cct.hpp

#ifndef CCT_hpp
#define CCT_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <string>
float calc_cct(cv::Mat);
void get_cct(std::string);
uint32_t nearest100(uint32_t);
void split(std::string, std::string, std::vector<std::string>&);
void set_cct_rgb(std::string, uint32_t);
void set_cct_cie_xy(std::string, uint32_t);
void help(void);
#endif /* CCT_hpp */

