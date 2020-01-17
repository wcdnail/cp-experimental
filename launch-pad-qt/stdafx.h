#pragma once

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <exception>
#include <stdexcept>

#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QtGui/QIcon>
#include <QtCore/QVariant>
#include <QApplication>
#include <QDockWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QOpenGLWidget>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QAction>
#include <QPlainTextEdit>
#ifdef USE_QOPENGLWIDGET
#include <QOpenGLWidget>
#else
#include <QGLWidget>
#endif

#include <boost/exception/diagnostic_information.hpp>
