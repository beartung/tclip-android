//from http://code.taobao.org/p/tclip/

#ifndef __TCLIP__
#define __TCLIP__

#include "cv.h"  
#include "opencv2/core/core.hpp"

#ifdef __cplusplus
extern "C" {
#endif

int clip(cv::Mat & image, cv::Mat & dest_image, const char * config_path, int dest_width, int dest_height);

#ifdef __cplusplus
}
#endif

#endif
