// Author: Marco Carraro

#include "enumobjtype.h"
#include "measurement.h"

#include <iostream>
#include <fstream>  // Necessary to operate with text files
#include <string>
#include <sstream>  // Necessary to use istringstream
#include <vector>

namespace fs = std::filesystem;
using namespace std;
using std::cout;
using std::cerr;
using std::endl;

vector<fs::path> list_files(const fs::path& directory) {
    vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path());
        }
    }
    sort(files.begin(), files.end()); // Important: sort alphabetically
    return files;
}

void performance_evaluation(OBJTYPE obj_type, const fs::path &dataset_dir)
{
    string obj_type_str;

    switch (obj_type) {
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

    const fs::path prediction_dir = dataset_dir / obj_type_str / "output";
    const fs::path dataset_path = dataset_dir / obj_type_str / "labels";

    vector<fs::path> label_files = list_files(dataset_path);
    vector<fs::path> prediction_files = list_files(prediction_dir);

    if (label_files.size() != prediction_files.size()) {
        cerr << "Mismatch between number of label and prediction files!" << endl;
        return;
    }

    int number_of_files = label_files.size();
    vector<float> iou(number_of_files);

    int temp_acc = 0;
    float miou, acc;

    for (int i = 0; i < number_of_files; i++) {
        iou[i] = IoU(obj_type_str, label_files[i], prediction_files[i]);
        temp_acc += accuracy(iou[i]);
    }

    miou = mIoU(iou, number_of_files);
    acc = (float)temp_acc / number_of_files;  // Cast to float for correct result

    write_performance(obj_type_str, dataset_dir, miou, acc);

    cout << "mean Intersection over Union: " << miou << endl;
    cout << "Accuracy: " << acc << endl;
}
int box_area(std::string obj_type_str, const fs::path &text_file)
{
    string obj_name, line;      // obj_name to compare the name of the object we are evaluating, line to read one string at the time if there are more than one object in the text file
    int x_min, y_min, x_max, y_max;

    ifstream input_file(text_file);

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

int intersection_area(string obj_type_str, const fs::path &dataset_box, const fs::path &predicted_box)
{
    string obj_name, line;
    int x_min_gt, y_min_gt, x_max_gt, y_max_gt;
    int x_min_pred, y_min_pred, x_max_pred, y_max_pred;

    ifstream dataset_file(dataset_box);
    ifstream pred_file(predicted_box);

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

    int x_inters_min = max(x_min_gt, x_min_pred);
    int y_inters_min = max(y_min_gt, y_min_pred);
    int x_inters_max = min(x_max_gt, x_max_pred);
    int y_inters_max = min(y_max_gt, y_max_pred);

    int width_inters = max(0, x_inters_max - x_inters_min);
    int height_inters = max(0, y_inters_max - y_inters_min);

    return width_inters * height_inters;
}

int union_area(string obj_type_str, const fs::path &dataset_box, const fs::path &predicted_box)
{
    int area_gt, area_pred, area_inters;

    area_gt = box_area(obj_type_str, dataset_box);
    area_pred = box_area(obj_type_str, predicted_box);
    area_inters = intersection_area(obj_type_str, dataset_box, predicted_box);

    int area = area_gt + area_pred - area_inters;

    return area;
}

float IoU(string obj_type_str, const fs::path &dataset_box, const fs::path &predicted_box)
{
    int area_inters = intersection_area(obj_type_str, dataset_box, predicted_box);
    int area_union = union_area(obj_type_str, dataset_box, predicted_box);

    if(area_union == 0) return 0.0f; // avoid division by zero
    return (float)area_inters / (float)area_union;
}

int number_images(string obj_type_str, const fs::path &dataset_dir)
{
    int file_count= 0;

    const fs::path count_items = dataset_dir / obj_type_str / "test_images";    // Set the path where to count the number of files

    for(const auto &entry : fs::directory_iterator(count_items)){       // Count the number of files tested
        if(entry.is_regular_file()){
            file_count++;
        }
    }

    return file_count;
}

float mIoU(vector<float> iou, int number_of_files)
{
    float total_IoU = 0;
    
    for(int i = 0; i < number_of_files; i++){
        if(iou[i] > 0){
            total_IoU = total_IoU + iou[i];
        }
    }

    float mean_IoU = total_IoU / number_of_files;
    
    return mean_IoU;
}

int accuracy(float current_iou)
{
    int acc = 0;
    
    if(current_iou > 0.5){
        acc++;
    }
    
    return acc;
}

void write_performance(string obj_type_str, const fs::path &output_dir, const float mIoU, const float accuracy)
{
    const std::string output_filename = obj_type_str + "-performance.txt";
    const fs::path output_filepath = output_dir / output_filename;
    std::fstream fs {output_filepath, std::ios::out};
    
    fs << obj_type_str << endl;
    fs << "mean Intersection over Union" << " " << mIoU << endl;
    fs << "Accuracy" << " " << accuracy;
    fs << endl;
}