#include "app.h"
#ifdef Q_OS_MAC
#include "EventFilter.h"
#endif

TheApp::TheApp(int &argc, char **argv)
    : QApplication(argc, argv)
    , winMain_(std::make_unique<TheWinMain>(nullptr))
{
#ifdef Q_OS_MAC
    this->installEventFilter(new SCADEventFilter(this));
#endif
}

TheApp::~TheApp()
{
}

int TheApp::run()
{
    winMain_->show();
    return exec();
}