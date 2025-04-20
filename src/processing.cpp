// Author: Luca Pellegrini

#include "processing.h"
#include "enumobjtype.h"

#include <iostream>
#include <stdexcept>

#include <opencv2/core.hpp>

namespace fs = std::filesystem;
using std::cout;
using std::cerr;
using std::endl;

void process_images(int obj_type, const fs::path &dataset_dir)
{
    std::string obj_type_str;
    switch (obj_type)
    {
    case OBJTYPE::BOX:
        obj_type_str = "004_sugar_box";
        break;
    case OBJTYPE::BOTTLE:
        obj_type_str = "006_mustard_bottle";
        break;
    case OBJTYPE::DRILL:
        obj_type_str = "035_power_drill";
        break;
    default:
        throw std::invalid_argument("Unknown object type");
    }

    std::vector<cv::Mat> cropped_models = load_cropped_models(dataset_dir);
    if (cropped_models.empty())
    {
        cerr << "No cropped models\n";
//        throw std::exception();
    }

    std::vector<TestImage> test_images = load_test_images(dataset_dir);
    if (test_images.empty())
    {
        cerr << "No test images\n";
//        throw std::exception();
    }

    if (!mk_output_dir(dataset_dir))
    {
        cerr << "Unable to create output directory\n";
//        throw std::exception();
    }

    for (TestImage image : test_images)
    {
        std::vector<cv::Point2i> bound_box_coord = detect(cropped_models, image);
        // bound_box_coord either is empty (no object detected) or contains to points
        // (coordinates of the bounding box)

        write_image_output(dataset_dir, image, obj_type_str, bound_box_coord);
    }
}

/* --------------------------------------------------------------- */

std::vector<cv::Mat> load_cropped_models(const fs::path &dataset_dir)
{
    cerr << "load_cropped_models: not implemented yet!\n";
    return std::vector<cv::Mat>();
}

std::vector<TestImage> load_test_images(const fs::path &dataset_dir)
{
    cerr << "load_test_images: not implemented yet!\n";
    return std::vector<TestImage>();
}

bool mk_output_dir(const std::filesystem::__cxx11::path &dataset_dir)
{
    cerr << "mk_output_dir: not implemented yet!\n";
    return false;
}

std::vector<cv::Point2i> detect(const std::vector<cv::Mat> &cropped_models, const TestImage &image)
{
    cerr << "detect: not implemented yet!\n";
    return std::vector<cv::Point2i>();
}

void write_image_output(const fs::path &dataset_dir, const TestImage &image,
                        std::string obj_type_str, const std::vector<cv::Point2i> &bound_box_coord)
{
    cerr << "write_image_output: not implemented yet!\n";
}
