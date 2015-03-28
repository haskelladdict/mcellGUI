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
  QStringList keyWords;

  // list of advanced keywords and their possible values
  QStringList advKeyWords;
  QList<QStringList> values;
};

#endif
