// Author: Marco Carraro

#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <filesystem>   // To retrieve the path of the files

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
 * @brief Compute the mean Intersection over Union
 * @param Text file where the coordinates of the two box are saved
 * @return Value of the mean Intersection over Union
 */
float mIoU (int obj_type, const std::filesystem::path &dataset_box, const std::filesystem::path &predicted_box);

/**
 * @brief Compute the accuracy
 * @param Text file where the coordinates of the two box are saved
 * @return Value of the accuracy
 */
float accuracy (int obj_type, const std::filesystem::path &dataset_box, const std::filesystem::path &predicted_box);

#endif // MEASUREMENT_H