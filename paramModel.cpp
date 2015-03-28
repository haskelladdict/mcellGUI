// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <QDebug>
#include <QList>

#include "paramModel.hpp"


// constructor for ParamModel
ParamModel::ParamModel(QObject* parent) :
  QStandardItemModel(parent) {

  keyWords << "ITERATIONS" << "TIME_STEP";
  QStringList defaults;
  defaults << "0" << "0.0";
  for (int i=0; i < keyWords.size(); ++i) {
    QStandardItem *key = new QStandardItem(keyWords[i]);
    QStandardItem *val = new QStandardItem(defaults[i]);
    appendRow(QList<QStandardItem*>{key, val});
  }

  advKeyWords << "SURFACE_GRID_DENSITY" << "ACCURATE_3D_REACTIONS"
    << "CENTER_MOLECULES_ON_GRID" << "MICROSCOPIC_REVERSIBILITY";
  QStringList advDefaults;
  advDefaults << "10000" << "TRUE" << "FALSE" << "OFF";
  for (int i=0; i < advKeyWords.size(); ++i) {
    QStandardItem *key = new QStandardItem(advKeyWords[i]);
    QStandardItem *val = new QStandardItem(advDefaults[i]);
    appendRow(QList<QStandardItem*>{key, val});
  }

  QStringList of;
  of << "ON" << "OFF";

  QStringList tf;
  tf << "TRUE" << "FALSE";

  values << QStringList("10000") << tf << tf << of;
}
