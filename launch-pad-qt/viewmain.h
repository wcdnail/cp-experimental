#pragma once

#ifdef USE_QOPENGLWIDGET
#include <QOpenGLWidget>
using QGLWidgetBase = QOpenGLWidget;
#else
#include <QGLWidget>
using QGLWidgetBase = QGLWidget;
#endif

class MainView : public QGLWidgetBase
{
    Q_OBJECT

public:
    MainView(QWidget *parent = nullptr);
    virtual ~MainView();
};
