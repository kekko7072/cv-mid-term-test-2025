// Author: Marco Carraro

#include "measurement.h"
#include "enumobjtype.h"

#include <iostream>
#include <fstream>  // Necessary to operate with text files
#include <string>
#include <sstream>  // Necessary to use istringstream

namespace fs = std::filesystem;
using namespace std;
using std::cout;
using std::cerr;
using std::endl;

int box_area(int obj_type, const fs::path &text_file)
{
    string obj_type_str, obj_name, line;      // obj_name to compare the name of the object we are evaluating, line to read one string at the time if there are more than one object in the text file
    int x_min, y_min, x_max, y_max;

    ifstream input_file(text_file);
    
    switch (obj_type){
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
            throw invalid_argument("Unknown object type");
    }

    if(input_file.is_open()){      // Check if the file opens correctly
        while(getline(input_file, line)){
            if(line.find(obj_type_str) != string::npos){    // Check to pick the correct object
                istringstream str_split(line);  // Convert the line into a stream in order to save the values

                str_split >> obj_name >> x_min >> y_min >> x_max >> y_max;
            }
        }

        input_file.close();
    } else{
        cerr << "Unable to open the file: " << text_file << endl;
        return -1;
    }

    int area = (x_max - x_min) * (y_max - y_min);

    return area;
}

int intersection_area(int obj_type, const fs::path &dataset_box, const fs::path &predicted_box)
{
    string obj_type_str, obj_name, line;
    int x_min_gt, y_min_gt, x_max_gt, y_max_gt;
    int x_min_pred, y_min_pred, x_max_pred, y_max_pred;

    ifstream dataset_file(dataset_box);
    ifstream pred_file(predicted_box);
    
    switch (obj_type){
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
            throw invalid_argument("Unknown object type");
    }

    if(dataset_file.is_open()){      // Check if the file opens correctly
        while(getline(dataset_file, line)){
            if(line.find(obj_type_str) != string::npos){    // Check to pick the correct object
                istringstream str_split(line);  // Convert the line into a stream in order to save the values

                str_split >> obj_name >> x_min_gt >> y_min_gt >> x_max_gt >> y_max_gt;
            }
        }

        dataset_file.close();
    } else{
        cerr << "Unable to open the file: " << dataset_box << endl;
        return -1;
    }

    if(pred_file.is_open()){      // Check if the file opens correctly
        while(getline(pred_file, line)){
            if(line.find(obj_type_str) != string::npos){    // Check to pick the correct object
                istringstream str_split(line);  // Convert the line into a stream in order to save the values

                str_split >> obj_name >> x_min_pred >> y_min_pred >> x_max_pred >> y_max_pred;
            }
        }

        pred_file.close();
    } else{
        cerr << "Unable to open the file: " << predicted_box << endl;
        return -1;
    }
    
    int x_inters_min, y_inters_min, x_inters_max, y_inters_max;
    int width_inters, height_inters;

    x_inters_min = min(x_min_gt, x_min_pred);
    y_inters_min = min(y_min_gt, y_min_pred);
    x_inters_max = max(x_max_gt, x_max_pred);
    y_inters_max = max(y_max_gt, y_max_pred);

    width_inters = x_inters_max - x_inters_min;
    height_inters = y_inters_max - y_inters_min;

    int area_inters = width_inters * height_inters;

    return area_inters;
}

int union_area(int obj_type, const fs::path &dataset_box, const fs::path &predicted_box)
{
    int area_gt, area_pred, area_inters;

    area_gt = box_area(obj_type, dataset_box);
    area_pred = box_area(obj_type, predicted_box);
    area_inters = intersection_area(obj_type, dataset_box, predicted_box);

    int area = area_gt + area_pred - area_inters;

    return area;
}

float IoU(int obj_type, const fs::path &dataset_box, const fs::path &predicted_box)
{
    int area_inters, area_union;

    area_inters = intersection_area(obj_type, dataset_box, predicted_box);
    area_union = union_area(obj_type, dataset_box, predicted_box);

    float iou_value = area_inters / area_union;

    return iou_value;
}

float mIoU(int obj_type, const fs::path &dataset_box, const fs::path &predicted_box)
{
    int file_count= 0;
    float total_IoU = 0;
    string obj_type_str;

    switch (obj_type){
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
            throw invalid_argument("Unknown object type");
    }

    const fs::path count_items = dataset_box / obj_type_str / "test_images";    // Set the path where to count the number of files

    for(const auto &entry : fs::directory_iterator(count_items)){       // Count the number of files tested
        if(entry.is_regular_file()){
            total_IoU = IoU(obj_type, dataset_box, predicted_box);
            
            file_count++;
        }
    }

    float mean_IoU = total_IoU / file_count;
    
    return mean_IoU;
}

float accuracy(int obj_type, const fs::path &dataset_box, const fs::path &predicted_box)
{
    int file_count= 0;
    float acc = 0;
    string obj_type_str;

    switch (obj_type){
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
            throw invalid_argument("Unknown object type");
    }

    const fs::path count_items = dataset_box / obj_type_str / "test_images";    // Set the path where to count the number of files

    for(const auto &entry : fs::directory_iterator(count_items)){       // Count the number of files tested
        if(entry.is_regular_file()){
            if(IoU(obj_type, dataset_box, predicted_box) > 0.5){
                acc++;
            }

            file_count++;
        }
    }

    acc = acc / file_count;
    
    return acc;
}