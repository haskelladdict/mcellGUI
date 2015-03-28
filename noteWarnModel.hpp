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
  QStringList noteKeyWords;
  QList<QStringList> noteValues;
};


// WarningsModel keeps track of options to adjust warnings status
class WarningsModel : public QStandardItemModel {

public:

  WarningsModel(QObject* parent = 0);

  // list of keywords and their possible values
  QStringList warnKeyWords;
  QList<QStringList> warnValues;
};

#endif
