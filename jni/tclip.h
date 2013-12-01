#ifndef __TCLIP__
#define __TCLIP__

#include "cv.h"  
#include "opencv2/core/core.hpp"

#ifdef __cplusplus
extern "C" {
#endif

int detectFace(cv::Mat img , std::string face_cascade_name);
int detectCharacter(cv::Mat img);
int clip(cv::Mat image, cv::Mat dest_image, int dest_width, int dest_height);

#ifdef __cplusplus
}
#endif

#endif
