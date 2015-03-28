// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef IO_HPP
#define IO_HPP

#include <QStandardItemModel>

#include "molModel.hpp"
#include "noteWarnModel.hpp"
#include "paramModel.hpp"

class QTextStream;

bool writeMDL(QString fileName, const MolList& molecules,
  const ParamModel& paramModel, const NotificationsModel& noteModel,
  const WarningsModel& warnModel);

void writeParams(QTextStream& out, const ParamModel& paramModel);
void writeNotifications(QTextStream& out, const NotificationsModel& noteModel);
void writeWarnings(QTextStream& out, const WarningsModel& noteModel);
void writeMolecules(QTextStream& out, const MolList& molecules);

#endif
