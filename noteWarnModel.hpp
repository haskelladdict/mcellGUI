// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef NOTE_WARN_MODEL_HPP
#define NOTE_WARN_MODEL_HPP

#include <QList>
#include <QStandardItemModel>
#include <QStringList>

// NotificationsModel keeps track of options to adjust notification status
class NotificationsModel : public QStandardItemModel {

public:

  NotificationsModel(QObject* parent = 0);

  // list of keywords and their possible values
  const QStringList noteKeyWords = QStringList({"BOX_TRIANGULATION_REPORT",
    "DIFFUSION_CONSTANT_REPORT", "FILE_OUTPUT_REPORT", "FINAL_SUMMARY",
    "ITERATION_REPORT", "PARTITION_LOCATION_REPORT", "PROBABILITY_REPORT",
    "PROBABILITY_REPORT_THRESHOLD", "VARYING_PROBABILITY_REPORT",
    "PROGRESS_REPORT", "RELEASE_EVENT_REPORT", "MOLECULE_COLLISION_REPORT",
    "ALL_NOTIFICATIONS"});
  QList<QStringList> noteValues;
};


// WarningsModel keeps track of options to adjust warnings status
class WarningsModel : public QStandardItemModel {

public:

  WarningsModel(QObject* parent = 0);

  // list of keywords and their possible values
  const QStringList warnKeyWords = QStringList({"DEGENERATE_POLYGONS",
    "HIGH_REACTION_PROBABILITY", "HIGH_PROBABILITY_THRESHOLD",
    "LIFETIME_TOO_SHORT", "LIFETIME_THRESHOLD", "MISSED_REACTIONS",
     "MISSED_REACTION_THRESHOLD", "NEGATIVE_DIFFUSION_CONSTANT",
     "MISSING_SURFACE_ORIENTATION", "NEGATIVE_REACTION_RATE",
     "USELESS_VOLUME_ORIENTATION", "ALL_WARNINGS"});
  QList<QStringList> warnValues;
};

#endif
