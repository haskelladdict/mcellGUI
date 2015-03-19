// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <QDebug>
#include <QList>

#include "paramModel.hpp"


// init initializes a default model for mdl parameter keywords
void paramModel::init(QStandardItemModel& model) {

  QStringList keyWords;
  keyWords << "ITERATIONS" << "TIME_STEP" << "SURFACE_GRID_DENSITY"
    << "ACCURATE_3D_REACTIONS" << "CENTER_MOLECULES_ON_GRID"
    << "MICROSCOPIC_REVERSIBILITY";

  QStringList defaults;
  defaults << "1" << "0.0" << "10000" << "TRUE" << "FALSE" << "OFF";

  for (int i=0; i < keyWords.size(); ++i) {
    QStandardItem *key = new QStandardItem(keyWords[i]);
    QStandardItem *val = new QStandardItem(defaults[i]);
    model.appendRow(QList<QStandardItem*>{key, val});
  }
}
