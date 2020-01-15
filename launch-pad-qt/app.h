#pragma once

#include "winmain.h"
#include <memory>
#include <QApplication>

class TheApp : public QApplication
{
    Q_OBJECT
public:
    TheApp(int &argc, char **argv);
    ~TheApp();
    int run();
private:
    std::unique_ptr<TheWinMain> winMain_;
};
