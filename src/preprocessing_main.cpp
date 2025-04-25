#include "preprocessing.h"
#include <iostream>

int main() {
    USER_RETURN result = ask_user();
    std::cout << "\nSelected object type: " << static_cast<int>(result.type) << std::endl;
    std::cout << "Directory: " << result.directory << std::endl;
    return 0;
} 