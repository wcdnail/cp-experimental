#include "stdafx.h"
#include "main-twins-second.h"
#include <iostream>
#include <iomanip>
#include <filesystem>

int main(int argc, const char *argv[], const char *envp[])
{
    namespace fs = std::filesystem;

    try {
        for (auto& p : fs::directory_iterator("/")) {
            std::cout << p.path() << '\n';
        }
    }
    catch(std::exception &ex) {
        std::cerr << "ERROR: " << ex.what() << std::endl;
    }
    return 0;
}

