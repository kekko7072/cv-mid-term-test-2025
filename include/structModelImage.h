#ifndef STRUCTMODELIMAGE_H
#define STRUCTMODELIMAGE_H

//#include "enumobjtype.h"

#include <string>
#include <opencv2/core.hpp>

struct ModelImage
{
    std::string filename;
//    OBJTYPE obj_type;
    std::string obj_type_str;
    cv::Mat img;
    int alpha;  // first angle (either 0, 30 or 60)
    int beta;  // second angle (between 0 and 9)
};

#endif // STRUCTMODELIMAGE_H
