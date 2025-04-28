# Processing step

Input:
- object type: one of "004_sugar_box", "006_mustard_bottle" or "035_power_drill"
- dataset directory: for each object type we have following subdirectories
    - `models` directory
        - `cropped` directory
            - one directory for each model, which contains: cropped color image + cropped mask (black/white) image
    - `labels` directory
    - `test_images` directory

Output:
- inside dataset directory/object-specific directory:
    - `output` directory: new directory inside dataset directory (`std::filesystem::create_directory()`)
        - for each test image, a txt file with coordinates of bounding box of detected object

## Pseudocode:

Struct **ModelImage**
```cpp
struct ModelImage
{
    std::string name;
    std::string obj_type_str;
    cv::Mat img;
    cv::Mat mask;
    int alpha;  // first angle (either 0, 30 or 60)
    int beta;  // second angle (between 0 and 9)
};
```


Struct **TestImage**
```cpp
class TestImage {
public:
    TestImage(std::string img_id, cv::Mat img);
    std::string id();
    cv::Mat* getMat();
    
private:
    std::string img_id;
    // We need the image ID in order to write its associated txt file with the output of the detector
    // e.g. image filename: "4_0001_000121-color.jpg" -> ID: "4_0001_000121"
    // -> txt file: "4_0001_000121-detect.txt" (in the output directory)
    cv::Mat img;
}
```


```cpp
namespace fs = std::filesystem;

enum OBJ
{
    BOX = 0,
    BOTTLE = 1,
    DRILL = 2,
};

void process_images(int obj_type, const fs::path& dataset_dir)
{
    std::string obj_type_str;
    switch (obj_type)
    {
        case OBJ::BOX:
            obj_type_str = "004_sugar_box";
        case OBJ::BOTTLE:
            obj_type_str = "006_mustard_bottle";
        case OBJ::DRILL:
            obj_type_str = "035_power_drill";
    }
    
    std::vector<cv::Mat> cropped_models = load_cropped_models(dataset_dir);
    if (cropped_models.empty())
    {
        throw exception;
    }
    
    std::vector<TestImage> test_images = load_test_images(dataset_dir);
    if (test_images.empty())
    {
        cerr << "No test images";
        throw exception
    }
    
    if (!mk_output_dir(dataset_dir))
    {
        cerr << "Unable to create output directory";
        throw exception;
    }
    
    for (TestImage image : test_images)
    {
        std::vector<cv::Point2i> bound_box_coord = detect(cropped_models, image);
        // bound_box_coord either is empty (no object detected) or contains to points
        // (coordinates of the bounding box)
        
        write_image_output(dataset_dir, image, obj_type_str, bound_box_coord);
    }
}
```

To be implemented:
```cpp
std::vector<cv::Mat> load_cropped_models(const fs::path& dataset_dir)
{
    // Load cropped models from the default location: dataset_dir/models/cropped/
}

std::vector<TestImage> load_test_images(const fs::path& dataset_dir)
{
    // Load test images from the default location: dataset_dir/test_images/
}

bool mk_output_dir(const fs::path& dataset_dir)
{
    // Create the output directory; return true if successful, otherwise return false
}

std::vector<cv::Point2i> detect(const std::vector<cv::Mat>& cropped_models, const TestImage& image)
{
    // Detect the object of the specificied type in the given image, using the provided models.
    // If the object is detected, return a vector of two points (coordinates of the bounding box);
    // otherwise (object not detected) return an empty vector.
}

void write_image_output(const fs::path& dataset_dir, const TestImage& image, std::string obj_type_str, const std::vector<cv::Point2i>& bound_box_coord)
```
