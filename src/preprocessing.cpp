// Author: Francesco Vezzani

#include <enumobjtype.h>
#include <structuserreturn.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <filesystem>  
#include <vector>
#include <string>
#include <sstream>    
#include <iomanip>   
#include <iostream>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

// Ask user which object to detect and which directory to use as directory for objects
USER_RETURN ask_user() {

    USER_RETURN result;

    // Select folder
    string path;
    cout << "\nInsert folder path (leave empty to use default):" << endl;
    getline(cin, path);  // Allow empty input
      if (path.empty()) {
        // Default folder: relative to project root, NOT build
        path = "../object_detection_dataset";
    }
    result.directory = fs::absolute(path).string();

    cout << "\nAvailable object types:" << endl;
    cout << "0 - Box (004_sugar_box)" << endl;
    cout << "1 - Bottle (006_mustard_bottle)" << endl;
    cout << "2 - Drill (035_power_drill)" << endl;
    
    int choice;
    cout << "\nSelect an object type (0-2): ";
    cin >> choice;
    
    
    switch (choice) {
        case 0:
            result.type = OBJTYPE::BOX;
            break;
        case 1:
            result.type = OBJTYPE::BOTTLE;
            break;
        case 2:
            result.type = OBJTYPE::DRILL;
            break;
        default:
            cout << "Invalid selection. Defaulting to Box." << endl;
            result.type = OBJTYPE::BOX;
    }
    
    return result;
}

void crop_model_image(USER_RETURN user_return) {
    // Detect object type
    std::string obj_type_str;
    switch (user_return.type) {
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

    // Object folder directory
    string folder_base = user_return.directory + "/" + obj_type_str + "/models/";
    string folder_cropped = folder_base + "cropped/";

    // Create folder for cropped images
    fs::create_directories(folder_cropped);

    // Find all PNG images
    vector<String> fn;
    glob(folder_base + "*.png", fn, false);

    size_t count = fn.size(); // number of png files
    if (count == 0) {
        cerr << "No PNG images found in " << folder_base << endl;
        return;
    }

    for (size_t i = 0; i < count; i++)
    {
        Mat img = imread(fn[i]);

        if (img.empty()) {
            cerr << "Failed to load image: " << fn[i] << endl;
            continue;
        }

        // Extract filename without extension
        string filename = fs::path(fn[i]).filename().string(); // e.g., view_0_001_color.png

        // Find the prefix (common name before _color or _mask)
        size_t pos = filename.find("_color");
        if (pos == string::npos) {
            pos = filename.find("_mask");
        }
        if (pos == string::npos) {
            cerr << "Skipping file with unexpected name format: " << filename << endl;
            continue;
        }

        string base_name = filename.substr(0, pos); // e.g., "view_0_001"

        // Build output directory
        string view_folder = folder_cropped + base_name + "/";
        fs::create_directories(view_folder);

        // Initialize the bounding box with extreme values
        Mat cropped;
        int left_most_px = img.cols;
        int right_most_px = 0;
        int top_most_px = img.rows;
        int bottom_most_px = 0;

        uint8_t* pixelPtr = (uint8_t*)img.data;
        int cn = img.channels(); // Number of channels (e.g., 3 for BGR)

        // Loop over all pixels
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                uint8_t b = pixelPtr[i * img.cols * cn + j * cn + 0]; // B
                uint8_t g = pixelPtr[i * img.cols * cn + j * cn + 1]; // G
                uint8_t r = pixelPtr[i * img.cols * cn + j * cn + 2]; // R

                // Check if the pixel is "white" (all channels are 255) for mask crop
                if (filename.find("mask") != string::npos) {
                    if (r == 255 && g == 255 && b == 255) {
                        // Update bounding box
                        if (j < left_most_px) left_most_px = j;
                        if (j > right_most_px) right_most_px = j;
                        if (i < top_most_px) top_most_px = i;
                        if (i > bottom_most_px) bottom_most_px = i;
                    }
                
                // Check if the pixel is not "white" (all channels are 255) for color crop
               }else{
                    if (r != 255 && g != 255 && b != 255) {
                         // Update bounding box
                        if (j < left_most_px) left_most_px = j;
                        if (j > right_most_px) right_most_px = j;
                        if (i < top_most_px) top_most_px = i;
                        if (i > bottom_most_px) bottom_most_px = i;
                    }
                }
            }
        }

        // After the loop, create the ROI
        if (left_most_px <= right_most_px && top_most_px <= bottom_most_px) {
            int roi_width = right_most_px - left_most_px + 1;
            int roi_height = bottom_most_px - top_most_px + 1;
            Rect roi(left_most_px, top_most_px, roi_width, roi_height);

            // Crop the image
            cropped = img(roi);
        }

        // Save cropped image
        if (filename.find("mask") != string::npos) {
            string output_path = view_folder + "mask.png";
            imwrite(output_path, cropped);  
        }
        else if (filename.find("color") != string::npos) {
            string output_path = view_folder + "color.png";
            imwrite(output_path, cropped);  
        }
    }
}