// Author: Luca Pellegrini

#include "processing.h"
#include "enumobjtype.h"
#include "structModelImage.h"
#include "structTestImage.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

namespace fs = std::filesystem;
using std::cout;
using std::cerr;
using std::endl;

void process_images(OBJTYPE obj_type, const fs::path &data_dir)
{
    // Ensure we work with absolute paths
    const fs::path dataset_dir = fs::absolute(data_dir);

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

    const fs::path output_dir = dataset_dir / obj_type_str / "output";
    if (!mk_output_dir(output_dir))
    {
        cerr << "Unable to create output directory\n";
        throw std::exception();
    }

    for (TestImage image : test_images)
    {
        std::vector<cv::Point2i> bbox_coord = detect(cropped_models, image, output_dir);
        // bbox_coord contains two points (coordinates of the bounding box)
        write_image_output(output_dir, image, obj_type_str, bbox_coord);
    }
}

/* --------------------------------------------------------------- */

std::vector<ModelImage> load_cropped_models(std::string obj_type_str, const fs::path &dataset_dir)
{
    std::vector<ModelImage> cropped_models;
    const fs::path cropped_models_dir = dataset_dir / obj_type_str / "models" / "cropped";

    for (const auto &entry : fs::directory_iterator(cropped_models_dir))
    {
        if (entry.is_directory())
        {
            fs::path path = entry.path();
            std::string path_str = path.string();
            std::string::size_type pos = path_str.rfind("/");
            std::string model_name = path_str.substr(pos+1);

            ModelImage model;
            model.name = model_name;
            model.obj_type_str = obj_type_str;
            std::string color_path = path.string() + "/" + "color.png";
            std::string mask_path = path.string() + "/" + "mask.png";
            if (!fs::exists(color_path) || !fs::exists(mask_path))
                continue;
            model.color = cv::imread(color_path);
            model.mask = cv::imread(mask_path);
            cropped_models.push_back(model);
        }
    }
    return cropped_models;
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

bool mk_output_dir(const fs::path &output_dir)
{
    if (fs::exists(output_dir))
    {
        if (fs::is_directory(output_dir))
        {
            return true;
        }
        else  // exists, but not a directory
        {
            return false;
        }
    }
    return fs::create_directory(output_dir);
}

std::vector<cv::Point2i> detect(const std::vector<ModelImage> &cropped_models, const TestImage &image,
                                const fs::path &output_dir)
{
    cout << "Processing test image: " << image.id << endl;

    size_t num_good_matches = 0;
    std::vector<cv::Point2i> bbox_corners(2);

    for (ModelImage model : cropped_models)
    {
//        cout << "model image = " << model.name << endl;
        cv::Mat obj_color = model.color;
        cv::Mat obj_mask = model.mask;
        cv::Mat scene_img = image.img;

        // Detect keypoints in both images (model and scene) and compute descriptors for each of the keypoints
        cv::Ptr<cv::Feature2D> kaze = cv::KAZE::create();
        std::vector<cv::KeyPoint> keypoints_obj, keypoints_scene;
        cv::Mat descriptors_obj, descriptors_scene;
        kaze->detectAndCompute(obj_color, obj_mask, keypoints_obj, descriptors_obj);
        kaze->detectAndCompute(scene_img, cv::noArray(), keypoints_scene, descriptors_scene);

        // Find the closest matches between descriptors from the model image to the scene image
        cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
        std::vector<cv::DMatch> matches;
        matcher->match(descriptors_obj, descriptors_scene, matches);

//        cout << "num_good_matches " << num_good_matches << endl;
//        cout << "matches " << matches.size() << endl;
        if (matches.size() >= num_good_matches)
        {
            num_good_matches = matches.size();

            // Localize the object
            std::vector<cv::Point2f> obj;
            std::vector<cv::Point2f> scene;

            for( size_t i = 0; i < matches.size(); i++ )
            {
                // Get the keypoints from the good matches
                obj.push_back( keypoints_obj[ matches[i].queryIdx ].pt );
                scene.push_back( keypoints_scene[ matches[i].trainIdx ].pt );
            }

            // Get the corners from the object image
            std::vector<cv::Point2f> obj_corners(4);
            obj_corners[0] = cv::Point2f(0, 0);
            obj_corners[1] = cv::Point2f(static_cast<float>(obj_color.cols), 0);
            obj_corners[2] = cv::Point2f(static_cast<float>(obj_color.cols), static_cast<float>(obj_color.rows));
            obj_corners[3] = cv::Point2f(0, static_cast<float>(obj_color.rows));
            std::vector<cv::Point2f> scene_corners(4);

            cv::Mat T = estimateAffine2D( obj, scene, cv::noArray(), cv::RANSAC);
            transform( obj_corners, scene_corners, T);

            int xmin, ymin, xmax, ymax;
            xmin = scene_corners[0].x;
            ymin = scene_corners[0].y;
            xmax = scene_corners[2].x;
            ymax = scene_corners[2].y;
            bbox_corners[0] = cv::Point2i(xmin, ymin);
            bbox_corners[1] = cv::Point2i(xmax, ymax);
        }
    }
    // Print image
    cv::Mat bbox = image.img.clone();
    cv::rectangle(bbox, bbox_corners[0], bbox_corners[1], /*color=*/ cv::Scalar(0, 255, 0), /*thickness=*/ 4);
    std::string text = cropped_models[0].obj_type_str;
    cv::putText(bbox, text, cv::Point2i(0, 40), cv::HersheyFonts::FONT_HERSHEY_SIMPLEX,
                1.5, cv::Scalar(0, 0, 255), 4);
//    cv::namedWindow("bbox", cv::WINDOW_NORMAL);
//    cv::imshow("bbox", bbox);
//    cv::waitKey(0);
    const fs::path out_images_dir = output_dir / "images";
    if (mk_output_dir(out_images_dir))
    {
        std::string out_image_filename = image.id + "_bbox.png";
        const fs::path out_image_path = out_images_dir / out_image_filename;
        cv::imwrite(out_image_path.string(), bbox);
    }

    return bbox_corners;
}

void write_image_output(const fs::path &output_dir, const TestImage &image,
                        std::string obj_type_str, const std::vector<cv::Point2i> &bbox_coord)
{
    const std::string output_filename = image.id + "-detected.txt";
    const fs::path output_filepath = output_dir / output_filename;
    std::fstream fs {output_filepath, std::ios::out};
    if (!bbox_coord.empty())
    {
        fs << obj_type_str << " ";
        fs << bbox_coord[0].x << " " << bbox_coord[0].y << " ";
        fs << bbox_coord[1].x << " " << bbox_coord[1].y;
    }
    fs << endl;
}
