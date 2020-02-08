#include "stdafx.h"
#include "app.h"

TheApp::TheApp(int &argc, char **argv)
    : QApplication(argc, argv)
    , winMain_(std::make_unique<TheWinMain>(nullptr))
{
}

TheApp::~TheApp()
{
}

int TheApp::run()
{
    winMain_->show();
    return exec();
}