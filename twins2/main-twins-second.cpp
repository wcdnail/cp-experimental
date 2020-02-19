#include "stdafx.h"
#include "main-twins-second.h"
#include <iostream>
#include <iomanip>
#include <filesystem>

int main(int argc, const char *argv[], const char *envp[])
{
    try {
        std::filesystem::directory_iterator it;
    }
    catch(std::exception &ex) {
        std::cerr << "ERROR: " << ex.what() << std::endl;
    }
    return 0;
}

