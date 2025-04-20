// Author: Luca Pellegrini

#include "testimage.h"

#include <string>
#include <opencv2/core.hpp>

TestImage::TestImage(std::string img_id, cv::Mat img)
    : m_img_id{img_id}, m_img{img}
{}

TestImage::~TestImage()
{}

std::string TestImage::id() const
{
    return m_img_id;
}

cv::Mat *TestImage::mat()
{
    return &m_img;
}
