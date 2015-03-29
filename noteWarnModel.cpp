// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include "noteWarnModel.hpp"


// constructor for NotificationsModel
NotificationsModel::NotificationsModel(QObject* parent) :
  QStandardItemModel(parent) {

  QStringList of({"ON", "OFF"});
  QStringList ofAll(of);
  ofAll << "UNSET";
  QStringList ob({"ON", "BRIEF"});

  noteValues << of << ob << of << of << of << of << of << QStringList("")
    << of << of << of << of << ofAll;

  QStringList noteDefaults;
  noteDefaults << "OFF" << "BRIEF" << "OFF" << "ON" << "ON" << "OFF" << "ON"
    << "1" << "ON" << "ON" << "ON" << "OFF" << "UNSET";

  for (int i=0; i < noteKeyWords.size(); ++i) {
    QStandardItem *key = new QStandardItem(noteKeyWords[i]);
    QStandardItem *val = new QStandardItem(noteDefaults[i]);
    appendRow(QList<QStandardItem*>{key, val});
  }
}


// constructor for WarningsModel
WarningsModel::WarningsModel(QObject* parent) :
  QStandardItemModel(parent) {

  QStringList warn({"IGNORED", "WARNING", "ERROR"});
  QStringList warnAll(warn);
  warnAll << "UNSET";
  QStringList empty("");

  warnValues << warn << warn << empty << warn << empty << warn << empty
    << warn << warn << warn << warn << warnAll;

  QStringList warnDefaults;
  warnDefaults << "WARNING" << "IGNORED" << "1.0" << "WARNING" << "50"
    << "WARNING" << "1e-3" << "WARNING" << "ERROR" << "WARNING" << "WARNING"
    << "UNSET";

  for (int i=0; i < warnKeyWords.size(); ++i) {
    QStandardItem *key = new QStandardItem(warnKeyWords[i]);
    QStandardItem *val = new QStandardItem(warnDefaults[i]);
    appendRow(QList<QStandardItem*>{key, val});
  }
}
