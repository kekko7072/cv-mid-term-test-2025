# Computer Vision Project 2025
Project for Computer Vision by Professor Ghidoni S. for class of 2025 in master degree in Computer Enineering 2025.

## Structure
The project it's writte in **C++** (stdandard 17), it uses only **OpenCV** library version 4.5.X and other basic standard libraries.

We create two executables:
- **Trainer** for training the [Cascade Classifier](https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html) and generating the model.
- **Detector** for running the model loaded from file and locating known objects in an input image.

| Trainer | Detector |
| --  | --  |
| Reading Dataset | Cli execution selector |
| Cascade Classifier module  | Execution of model |
| Write module  | Calculation of MIOU and accuracy |
||Print of result|