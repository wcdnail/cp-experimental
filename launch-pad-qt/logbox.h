#pragma once

#include <QPlainTextEdit>
#include "ui_logbox.h"

class LogBox : public QPlainTextEdit, Ui_LogBox
{
	Q_OBJECT

public:
    LogBox(QWidget *parent = nullptr);
    virtual ~LogBox() override;

public slots:
    void onClearConsole();
    void onSaveAs();
};
