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

  advKeyWords << "TIME_STEP_MAX" << "SPACE_STEP"
    << "SURFACE_GRID_DENSITY" << "INTERACTION_RADIUS"
    << "ACCURATE_3D_REACTIONS" << "CENTER_MOLECULES_ON_GRID"
    << "VACANCY_SEARCH_DISTANCE" << "MICROSCOPIC_REVERSIBILITY";
  QStringList advDefaults;
  advDefaults << "" << "" << "10000" << "" << "TRUE" << "FALSE" << "0.1" << "OFF";
  for (int i=0; i < advKeyWords.size(); ++i) {
    QStandardItem *key = new QStandardItem(advKeyWords[i]);
    QStandardItem *val = new QStandardItem(advDefaults[i]);
    appendRow(QList<QStandardItem*>{key, val});
  }

  QStringList of;
  of << "ON" << "OFF";

  QStringList tf;
  tf << "TRUE" << "FALSE";

  QStringList empty("");

  values << empty << empty << QStringList("10000") << empty << tf << tf
    << QStringList("0.1") << of;
}
