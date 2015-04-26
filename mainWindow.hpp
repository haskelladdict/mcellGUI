// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>

#include "molModel.hpp"
#include "noteWarnModel.hpp"
#include "paramModel.hpp"
#include "reactionModel.hpp"

#include "ui_mainWindow.h"


class MainWindow : public QMainWindow, Ui::MainWindow {

  Q_OBJECT

public:

  MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);

private:

  // data models
  MolModel* moleculeModel_;
  ParamModel* paramModel_;
  NotificationsModel* noteModel_;
  WarningsModel* warnModel_;
  ReactTreeModel* reactTreeModel_;

private slots:

  void exportMDL_();
};

#endif
