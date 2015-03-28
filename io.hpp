// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef IO_HPP
#define IO_HPP

#include <QStandardItemModel>

#include "molModel.hpp"

class QTextStream;

bool writeMDL(QString fileName, const MolList& molecules,
  const QStandardItemModel& paramModel);

void writeParams(QTextStream& out, const QStandardItemModel& paramModel);
void writeMolecules(QTextStream& out, const MolList& molecules);

#endif
