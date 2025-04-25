// Author: Francesco Vezzani

#include <enumobjtype.h>
#include <iostream>
#include <preprocessing.h>
#include "structuserreturn.h"

using namespace std;

USER_RETURN ask_user() {

    USER_RETURN result;
    
    // Default object directory dataset
    result.directory = "object_detection_dataset";

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