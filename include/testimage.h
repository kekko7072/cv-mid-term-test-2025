// Author: Luca Pellegrini

#ifndef TESTIMAGE_H
#define TESTIMAGE_H

#include <string>

#include <opencv2/core.hpp>

class TestImage
{
public:
    TestImage(std::string img_id, cv::Mat img);
    ~TestImage();
    std::string id() const;
    cv::Mat *mat();

private:
    std::string m_img_id;
    cv::Mat m_img;
};

#endif // TESTIMAGE_H
