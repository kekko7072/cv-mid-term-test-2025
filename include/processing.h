// Author: Luca Pellegrini

#ifndef PROCESSING_H
#define PROCESSING_H

#include "testimage.h"

#include <filesystem>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

/**
 * @brief Perform the processing phase
 * @param obj_type One of the possible value of enum `OBJTYPE`
 * @param dataset_dir Directory of the dataset being processed
 */
void process_images(int obj_type, const std::filesystem::path &dataset_dir);

/**
 * @brief Load cropped models from default location: dataset_dir/models/cropped/
 * @param dataset_dir Directory of the dataset being processed
 * @return Vector of cv::Mat objects, each containing a model
 */
std::vector<cv::Mat> load_cropped_models(const std::filesystem::path &dataset_dir);

/**
 * @brief Load test images from default location: dataset_dir/test_images/
 * @param dataset_dir Directory of the dataset being processed
 * @return Vector of TestImage objects
 */
std::vector<TestImage> load_test_images(const std::filesystem::path &dataset_dir);

/**
 * @brief Create the output directory
 * @param dataset_dir Directory of the dataset being processed
 * @return true if successful
 */
bool mk_output_dir(const std::filesystem::path &dataset_dir);

/**
 * @brief Detect object of the specificied type in the given image, using the provided models
 *
 * If the object is detected, return a vector of two points (coordinates of the bounding box);
 * otherwise (object not detected) return an empty vector.
 *
 * @param cropped_models Vector of cv::Mat objects, each containing a model
 * @param image Test image to be processed
 * @return Vector of two points (if object detected), or an empty vector
 */
std::vector<cv::Point2i> detect(const std::vector<cv::Mat> &cropped_models, const TestImage &image);

/**
 * @brief write_image_output
 * @param dataset_dir
 * @param image
 * @param obj_type_str
 * @param bound_box_coord
 */
void write_image_output(const std::filesystem::path &dataset_dir, const TestImage &image,
                        std::string obj_type_str, const std::vector<cv::Point2i> &bound_box_coord);

#endif // PROCESSING_H
