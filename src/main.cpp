#include "enumobjtype.h"
#include "processing.h"

#include <iostream>

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

    // Processing
    process_images(OBJTYPE::BOX, "object_detection_dataset");

    // Performance measurements

    return 0;
}
