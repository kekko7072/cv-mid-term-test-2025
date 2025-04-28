#ifndef STRUCTMODELIMAGE_H
#define STRUCTMODELIMAGE_H

//#include "enumobjtype.h"

#include <string>
#include <opencv2/core.hpp>

struct ModelImage
{
    std::string name;
//    OBJTYPE obj_type;
    std::string obj_type_str;
    cv::Mat color;
    cv::Mat mask;
};

#endif // STRUCTMODELIMAGE_H
