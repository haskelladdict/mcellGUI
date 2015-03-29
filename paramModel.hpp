// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef PARAM_MODEL_HPP
#define PARAM_MODEL_HPP

#include <QStandardItemModel>
#include <QStringList>

class ParamModel : public QStandardItemModel {

public:

  ParamModel(QObject* parent = 0);

  // list of main keywords
  const QStringList keyWords = QStringList({"ITERATIONS", "TIME_STEP"});

  // list of advanced keywords and their possible values
  const QStringList advKeyWords = QStringList({"TIME_STEP_MAX", "SPACE_STEP",
    "SURFACE_GRID_DENSITY", "INTERACTION_RADIUS", "ACCURATE_3D_REACTIONS",
    "CENTER_MOLECULES_ON_GRID", "VACANCY_SEARCH_DISTANCE",
    "MICROSCOPIC_REVERSIBILITY"});
  QList<QStringList> values;
};

#endif
