// Author: Luca Pellegrini

#include "processing.h"
#include "enumobjtype.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

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

    std::vector<cv::Mat> cropped_models = load_cropped_models(obj_type_str, dataset_dir);
    if (cropped_models.empty())
    {
        cerr << "No cropped models\n";
        throw std::exception();
    }

    std::vector<TestImage> test_images = load_test_images(obj_type_str, dataset_dir);
    if (test_images.empty())
    {
        cerr << "No test images\n";
        throw std::exception();
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

std::vector<cv::Mat> load_cropped_models(std::string obj_type_str, const fs::path &dataset_dir)
{
    std::vector<cv::Mat> cropped_models;
    const fs::path cropped_models_dir = dataset_dir / obj_type_str / "models" / "cropped";

    for (const auto &entry : fs::directory_iterator(cropped_models_dir))
        if (entry.is_regular_file())
        {
            fs::path path = entry.path();
            if (path.extension().string() == ".png")
            {
                cv::Mat img = cv::imread(path.string());
                cropped_models.push_back(img);
            }
        }

    return cropped_models;
}

std::vector<TestImage> load_test_images(std::string obj_type_str, const fs::path &dataset_dir)
{
//    cerr << "load_test_images: not implemented yet!\n";

    std::vector<TestImage> test_images;
    const fs::path test_images_dir = dataset_dir / obj_type_str / "test_images";

    for (const auto &entry : fs::directory_iterator(test_images_dir))
    {
        if (entry.is_regular_file())
        {
            fs::path path = entry.path();
            if (path.extension().string() == ".jpg" || path.extension().string() == ".png")
            {
                std::string img_id = get_img_id(path.filename());
                cv::Mat img = cv::imread(path.string());
                TestImage image(img_id, img);
                test_images.push_back(image);
            }
        }
    }

    return test_images;
}

std::string get_img_id(const fs::path &filename)
{
    std::string str = filename.string();
    std::string::size_type ext_pos = str.rfind("-");
    std::string r = str.substr(0, ext_pos);
    return r;
}

bool mk_output_dir(const fs::path &dataset_dir)
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
