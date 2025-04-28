// Author: Marco Carraro

#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "enumobjtype.h"

#include <filesystem>   // To retrieve the path of the files
#include <vector>

/**
 * @brief Perform the measurement for the performance of the program, print on the terminal and save the performance on a text file
 * @param Value of the evaluating category
 * @param Path of the dataset category
 * @param Path of the predected items
 */
void performance_evaluation (OBJTYPE obj_type, const std::filesystem::path &dataset_dir, const std::filesystem::path &predicted_dir);
/**
 * @brief Compute the area of the box
 * @param type of object we are evaluating 
 * @param Text file where the coordinates of the box are saved
 * @return Value of the area
 */
int box_area (int obj_type, const std::filesystem::path &text_file);

/**
 * @brief Find the intersection coordinates of the two boxes
 * @param Text file where the coordinates of the two box are saved
 * @return Coordinates of the intersection of the two boxes
 */
int intersection_area (int obj_type, const std::filesystem::path &dataset_box, const std::filesystem::path &predicted_box);

/**
 * @brief Compute the union of the two boxes
 * @param Text file where the coordinates of the two box are saved
 * @return Value of the union of the two boxes
 */
int union_area (int obj_type, const std::filesystem::path &dataset_box, const std::filesystem::path &predicted_box);

/**
 * @brief Compute the intersection over Union
 * @param Text file where the coordinates of the two box are saved
 * @return Value of the Intersection over Union
 */
float IoU (int obj_type, const std::filesystem::path &dataset_box, const std::filesystem::path &predicted_box);

/**
 * @brief Count the number of images on that we are evaluating
 * @param Value of the wvaluating category
 * @param Path of the dataset directory
 * @return Number of images in the test directory
 */
int number_images (OBJTYPE obj_type, const std::filesystem::path &dataset_dir);

/**
 * @brief Compute the mean Intersection over Union
 * @param Vector containing all the values of the IoU for that category
 * @param Number of files of the evaluated category
 * @return Value of the mean Intersection over Union
 */
float mIoU (std::vector<float> iou, int number_of_files);

/**
 * @brief Compute the accuracy
 * @param Value of the Intersection over Union just performed
 * @return Value of the accuracy
 */
int accuracy (float current_iou);

/**
 * @brief Write on a text file the results of the performance evaluation
 * @param Value of the category for the corrent name of the file
 * @param Path of the destination where to save the file
 * @param Value of the mean Intersection over Union
 * @param Value of the accuracy
 */
void write_performance (OBJTYPE obj_type, const std::filesystem::path &output_dir, const float mIoU, const float accuracy);

#endif // MEASUREMENT_H