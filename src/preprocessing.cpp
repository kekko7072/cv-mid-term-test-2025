// Author: Francesco Vezzani

#include <enumobjtype.h>
#include <iostream>
#include <string>
#include <preprocessing.h>
#include "structuserreturn.h"

using namespace std;


// Ask user which object to detect and which directory to use as directory for objects
USER_RETURN ask_user() {

    USER_RETURN result;

    // Select folder
    string path;
    cout << "\nInsert folder path (leave empty to use default):" << endl;
    getline(cin, path);  // Allow empty input
    result.directory = !path.empty() ? path : "object_detection_dataset";


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