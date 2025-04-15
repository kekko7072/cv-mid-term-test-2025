# Computer Vision Project 2025
Project for Computer Vision by Professor Ghidoni S. for class of 2025 in master degree in Computer Enineering 2025.

## Structure
The project it's writte in **C++** (stdandard 17), it uses only **OpenCV** library version 4.5.X and other basic standard libraries.

We create a programm structured as follows:
- **preprocessing**:
    - input of cli command for choosing the folder of the desired object
    - preprocessing of images inside *models folder* using the mask to obtain the rectangle that encapsulate the object
- **processing** of each image inside *test folder*:
    - we compute features using **surf** and we do *feature maching* using **flann matcher** 
    - we compute the bounding box using **homography** approach
- **measurements** of performances:
    - mIoU 
    - accuracy

| MARCO | LUCA | FRANCESCO |
| :----: | :---: | :---: |
| measurements | processing | preprocessing

## Hours worked
| MARCO | LUCA | FRANCESCO |
| :----: | :---: | :---: |
| XXX  | XXX | XXX