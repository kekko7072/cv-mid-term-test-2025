#include "enumobjtype.h"
#include "structuserreturn.h"

#include "preprocessing.h"
#include "processing.h"

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
    USER_RETURN result = ask_user();
    cout << "\nSelected object type: " << static_cast<int>(result.type) << endl;
    cout << "Directory: " << result.directory << endl;
    
    // Processing
    process_images(object_type.type, object_type.directory);

    // Performance measurements

    return 0;
}
