#ifndef STRUCTTESTIMAGE_H
#define STRUCTTESTIMAGE_H

#include <string>
#include <opencv2/core.hpp>

struct TestImage
{
    std::string id;
    cv::Mat img;
};

#endif // STRUCTTESTIMAGE_H
