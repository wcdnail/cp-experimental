#include "stdafx.h"
#include "logbox.h"
#include <QFileDialog>
#include <QTextStream>

LogBox::LogBox(QWidget *parent) 
    : QPlainTextEdit(parent)
{
	setupUi(this);
    connect(this->actionClear, SIGNAL(triggered()), this, SLOT(onClearConsole()));
    connect(this->actionSaveAs, SIGNAL(triggered()), this, SLOT(onSaveAs()));
}

LogBox::~LogBox()
{
}

void LogBox::onClearConsole()
{
    this->document()->clear();
}

void LogBox::onSaveAs()
{
    const auto& text = this->document()->toPlainText();
    const auto fileName = QFileDialog::getSaveFileName(this, "Save log box content");
    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << text;
        stream.flush();
        //PRINTB("LogBox content saved to '%s'.", fileName.toStdString());
    }
}
