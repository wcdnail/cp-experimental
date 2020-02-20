#include "stdafx.h"
#include "main-twins-second.h"
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <cstdlib>

int main(int argc, const char *argv[], const char *envp[])
{
    namespace fs = std::filesystem;

    try {
        fs::path begPath =
#ifdef _WIN32
            //"C:\\"
            //"C:\\ProgramData"
            //fs::temp_directory_path()
            std::getenv("USERPROFILE")
#else
            "~/"
#endif
            ;
        for (auto& dirEntry : fs::directory_iterator(begPath, fs::directory_options::follow_directory_symlink)) {
            auto&& dirEntryPath = dirEntry.path();
#ifdef _WIN32
            OutputDebugStringW(dirEntryPath.native().c_str());
            OutputDebugStringW(L"\n");
#else
            std::cout << dirEntryPath << '\n';
#endif
        }
    }
    catch (std::exception &ex) {
#ifdef _WIN32
        OutputDebugStringA("ERROR: ");
        OutputDebugStringA(ex.what());
        OutputDebugStringA("\n");
#else
        std::cerr << "ERROR: " << ex.what() << std::endl;
#endif
    }
    return 0;
}
