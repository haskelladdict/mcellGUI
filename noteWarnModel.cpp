// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include "noteWarnModel.hpp"


// constructor for NotificationsModel
NotificationsModel::NotificationsModel(QObject* parent) :
  QStandardItemModel(parent) {

  QStringList of;
  of << "ON" << "OFF";

  QStringList ofAll(of);
  ofAll << "UNSET";

  QStringList ob;
  ob << "ON" << "BRIEF";

  noteKeyWords << "BOX_TRIANGULATION_REPORT" << "DIFFUSION_CONSTANT_REPORT"
    << "FILE_OUTPUT_REPORT" << "FINAL_SUMMARY" << "ITERATION_REPORT"
    << "PARTITION_LOCATION_REPORT" << "PROBABILITY_REPORT"
    << "PROBABILITY_REPORT_THRESHOLD" << "VARYING_PROBABILITY_REPORT"
    << "PROGRESS_REPORT" << "RELEASE_EVENT_REPORT"
    << "MOLECULE_COLLISION_REPORT" << "ALL_NOTIFICATIONS";

  noteValues << of << ob << of << of << of << of << of << QStringList("1")
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

  QStringList warn;
  warn << "IGNORED" << "WARNING" << "ERROR";
  QStringList warnAll(warn);
  warnAll << "UNSET";

  warnKeyWords << "DEGENERATE_POLYGONS" << "HIGH_REACTION_PROBABILITY"
    << "HIGH_PROBABILITY_THRESHOLD" << "LIFETIME_TOO_SHORT"
    << "LIFETIME_THRESHOLD" << "MISSED_REACTIONS" << "MISSED_REACTION_THRESHOLD"
    << "NEGATIVE_DIFFUSION_CONSTANT" << "MISSING_SURFACE_ORIENTATION"
    << "NEGATIVE_REACTION_RATE" << "USELESS_VOLUME_ORIENTATION"
    << "ALL_WARNINGS";

  warnValues << warn << warn << QStringList("1.0") << warn << QStringList("50")
    << warn << QStringList("1e-3") << warn << warn << warn << warn << warnAll;

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
