// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

//#include <memory>

#include <QMainWindow>

#include "ui_mainWindow.h"


class MainWindow : public QMainWindow, Ui::MainWindow {

  Q_OBJECT

public:

  MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);

private:
};





























#endif
