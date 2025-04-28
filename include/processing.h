// Author: Luca Pellegrini

#ifndef PROCESSING_H
#define PROCESSING_H

#include "enumobjtype.h"
#include "structModelImage.h"
#include "structTestImage.h"

#include <filesystem>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

/**
 * @brief Perform the processing phase
 * @param obj_type One of the possible value of enum `OBJTYPE`
 * @param dataset_dir Directory of the dataset being processed
 */
void process_images(OBJTYPE obj_type, const std::filesystem::path &dataset_dir);

/**
 * @brief Load cropped models from default location: dataset_dir/obj_type_str/models/cropped/
 * @param obj_type_str String ID of the object type we want to detect
 * @param dataset_dir Directory of the dataset being processed
 * @return Vector of ModelImage objects
 */
std::vector<ModelImage> load_cropped_models(std::string obj_type_str, const std::filesystem::path &dataset_dir);

/**
 * @brief Get viewangles (alpha and beta) from model's filename, and save their values into the model
 *
 * alpha = first angle (either 0, 30 or 60)
 * beta = second angle (between 0 and 9)
 * @param model Model image being loaded
 */
void get_model_viewangles(ModelImage &model);

/**
 * @brief Load test images from default location: dataset_dir/obj_type_str/test_images/
 * @param obj_type_str String ID of the object type we want to detect
 * @param dataset_dir Directory of the dataset being processed
 * @return Vector of TestImage objects
 */
std::vector<TestImage> load_test_images(std::string obj_type_str, const std::filesystem::path &dataset_dir);

/**
 * @brief Get string ID of the test image being processed from its filename
 * @param filename
 * @return
 */
std::string get_img_id(const std::filesystem::path &path);

/**
 * @brief Create the output directory
 * @param output_dir Directory in which to write the output
 * @return true if successful, or if directory already exists
 */
bool mk_output_dir(const std::filesystem::path &output_dir);

/**
 * @brief Detect object of the specificied type in the given image, using the provided models
 *
 * If the object is detected, return a vector of two points (coordinates of the bounding box);
 * otherwise (object not detected) return an empty vector.
 *
 * @param cropped_models Vector of ModelImage objects
 * @param image Test image to be processed
 * @return Vector of two points (if object detected), or an empty vector
 */
std::vector<cv::Point2i> detect(const std::vector<ModelImage> &cropped_models, const TestImage &image);

/**
 * @brief write_image_output
 * @param output_dir
 * @param image
 * @param obj_type_str
 * @param bbox_coord
 */
void write_image_output(const std::filesystem::path &output_dir, const TestImage &image,
                        std::string obj_type_str, const std::vector<cv::Point2i> &bbox_coord);

#endif // PROCESSING_H
