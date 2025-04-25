// Author: Francesco Vezzani

#include <enumobjtype.h>
#include <iostream>
#include <preprocessing.h>
#include "structuserreturn.h"

using namespace std;

USER_RETURN ask_user() {
    cout << "\nAvailable object types:" << endl;
    cout << "0 - Box (004_sugar_box)" << endl;
    cout << "1 - Bottle (006_mustard_bottle)" << endl;
    cout << "2 - Drill (035_power_drill)" << endl;
    
    int choice;
    cout << "\nSelect an object type (0-2): ";
    cin >> choice;
    
    USER_RETURN result;
    switch (choice) {
        case 0:
            result.type = OBJTYPE::BOX;
            result.directory = "004_sugar_box";
            break;
        case 1:
            result.type = OBJTYPE::BOTTLE;
            result.directory = "006_mustard_bottle";
            break;
        case 2:
            result.type = OBJTYPE::DRILL;
            result.directory = "035_power_drill";
            break;
        default:
            cout << "Invalid selection. Defaulting to Box." << endl;
            result.type = OBJTYPE::BOX;
            result.directory = "004_sugar_box";
    }
    
    return result;
}