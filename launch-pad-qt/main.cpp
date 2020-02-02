#include "stdafx.h"
#include "app.h"
#include <QMessageBox>
#include <exception>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/stacktrace.hpp>

static std::unique_ptr<TheApp> TheAppPtr;

int main(int argc, char **argv)
{
    int res = 0;
    try {
        TheAppPtr = std::make_unique<TheApp>(argc, argv);
        res = TheAppPtr->run();
    }
    catch (...) {
        std::string          diagnostics = boost::current_exception_diagnostic_information(true);
        std::ostringstream           stm;
        boost::stacktrace::stacktrace bt;
        stm << "System failure :\n"
            << diagnostics
            << "\n\n";
        
        diagnostics = stm.str();
        fputs(diagnostics.c_str(), stderr);
        QMessageBox::critical(nullptr, "Error", diagnostics.c_str());
    }
    TheAppPtr.release();
    return res;
}
