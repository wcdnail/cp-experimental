#include "stdafx.h"
#include "winmain.h"
#include "app.h"
#include "ui_winmain.h"

TheWinMain::TheWinMain(QWidget *parent /* = nullptr */)
    : QMainWindow(parent)
    , ui_(std::make_unique<Ui::TheWinMain>())
{
    ui_->setupUi(this);
}

TheWinMain::~TheWinMain()
{
}
