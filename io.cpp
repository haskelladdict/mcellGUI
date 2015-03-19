// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <QFile>
#include <QTextStream>

#include "io.hpp"

const QString TAB("  ");

// writeMDL is responsible for writing model MDL files based on the data model.
// This function returns true if it succeeds and false otherwise.
bool writeMDL(QString fileName, const MolMap& molecules,
  const QStandardItemModel& paramModel) {
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    return false;
  }

  QTextStream out(&file);
  writeParams(out, paramModel);
  writeMolecules(out, molecules);

  return true;
}


// writeParams writes the model parameters to the QTextStream
void writeParams(QTextStream& out, const QStandardItemModel& paramModel) {
  for (int i = 0; i < paramModel.rowCount(); ++i) {
    out << paramModel.item(i, 0)->text() << " = "
        << paramModel.item(i, 1)->text() << "\n";
  }
  out << "\n";
}


// writeMolecules writes the molecule info to the QTextStream.
void writeMolecules(QTextStream& out, const MolMap& molecules) {

  out << "DEFINE_MOLECULES {\n";

  for (const auto& m : molecules) {
    MolData data = m.second;
    out << TAB << m.first << " {\n";
    if (data.type == MolType::VOL) {
      out << TAB << TAB << "DIFFUSION_CONSTANT_3D";
    } else {
      out << TAB << TAB << "DIFFUSION_CONSTANT_2D";
    }
    out << " = " << data.D << "\n";
    out << TAB << "}\n";
  }
  out << "}\n";
}

