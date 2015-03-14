// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef MOL_WIDGET_HPP
#define MOL_WIDGET_HPP

#include <QWidget>

#include "molModel.hpp"
#include "ui_molWidget.h"


class MolWidget : public QWidget, Ui::MolWidget {

  Q_OBJECT

public:
  MolWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);


private:

  MolModel model_;

private slots:
  void printIt();
};





























#endif
