#include "enumobjtype.h"
#include "structuserreturn.h"

#include "preprocessing.h"
#include "processing.h"
#include "measurements.h"

#include <iostream>
#include <filesystem>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

namespace fs = std::filesystem;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[])
{
    // Preprocessing
    USER_RETURN object_type = ask_user();
    crop_model_image(object_type);
    
    // Processing
    process_images(object_type.type, object_type.directory);

    // Performance measurements
    performance_evaluation(object_type.directory, "");
    
    return 0;
}
