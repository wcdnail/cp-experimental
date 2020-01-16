#include "stdafx.h"
#include "app.h"
#include <QMessageBox>
#include <exception>
#include <boost/exception/diagnostic_information.hpp>

int main(int argc, char **argv)
{
    int res = 0;
    try {
        TheApp app(argc, argv);
        res = app.run();
    }
    catch (...) {
        std::string diagnostics = boost::current_exception_diagnostic_information(true);
        std::ostringstream  stm;
        stm << "System failure :\n" << diagnostics << std::endl;
        diagnostics = stm.str();
        fputs(diagnostics.c_str(), stderr);
        QMessageBox::critical(nullptr, "Error", diagnostics.c_str());
    }
    return res;
}
