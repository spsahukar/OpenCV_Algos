//  cct.hpp

#ifndef DBR_hpp
#define DBR_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <string>
void split(std::string, std::string , std::vector<std::string>&);
std::string create_filename(std::string, std::string);
void help(void);
void bayerit(std::string);
void debayer(std::string);
#endif /* DBR_hpp */

