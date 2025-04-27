// Author: Luca Pellegrini

#include "processing.h"
#include "enumobjtype.h"
#include "structModelImage.h"
#include "structTestImage.h"

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

    std::vector<ModelImage> cropped_models = load_cropped_models(obj_type_str, dataset_dir);
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

std::vector<ModelImage> load_cropped_models(std::string obj_type_str, const fs::path &dataset_dir)
{
    std::vector<ModelImage> cropped_models;
    const fs::path cropped_models_dir = dataset_dir / obj_type_str / "models" / "cropped";

    for (const auto &entry : fs::directory_iterator(cropped_models_dir))
        if (entry.is_regular_file())
        {
            fs::path path = entry.path();
            if (path.extension().string() == ".png")
            {
                ModelImage model;
                model.filename = path.filename();
                model.obj_type_str = obj_type_str;
                model.img = cv::imread(path.string());
                get_model_viewangles(model);
                cropped_models.push_back(model);
            }
        }

    return cropped_models;
}

void get_model_viewangles(ModelImage &model)
{
    std::string str = model.filename;
    // e.g. str = "view_0_001_color_cropped.png"
    int alpha, beta;
    model.alpha = std::stoi(str.substr(5, 6));
    model.beta = std::stoi(str.substr(7, 10));
}

std::vector<TestImage> load_test_images(std::string obj_type_str, const fs::path &dataset_dir)
{
    std::vector<TestImage> test_images;
    const fs::path test_images_dir = dataset_dir / obj_type_str / "test_images";

    for (const auto &entry : fs::directory_iterator(test_images_dir))
    {
        if (entry.is_regular_file())
        {
            fs::path path = entry.path();
            if (path.extension().string() == ".jpg" || path.extension().string() == ".png")
            {
                TestImage image;
                image.id = get_img_id(path);
                image.img = cv::imread(path.string());
                test_images.push_back(image);
            }
        }
    }

    return test_images;
}

std::string get_img_id(const fs::path &path)
{
    std::string filename = path.filename().string();
    // e.g. filename = "4_0001_000121-color.jpg"
    std::string::size_type pos = filename.find("-");
    std::string id = filename.substr(0, pos);
    return id;
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
