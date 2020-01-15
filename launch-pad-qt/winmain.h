#pragma once

#include <memory>
#include <QMainWindow>

namespace Ui { class TheWinMain; }

class TheWinMain : public QMainWindow
{
    Q_OBJECT
public:
    explicit TheWinMain(QWidget *parent = nullptr);
    ~TheWinMain();
private:
    std::unique_ptr<Ui::TheWinMain> ui_;
};
