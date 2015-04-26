// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)


#include <QFile>
#include <QTextStream>

#include "molModel.hpp"
#include "noteWarnModel.hpp"
#include "paramModel.hpp"
#include "reactionModel.hpp"

#include "io.hpp"

const QString TAB("  ");

// writeMDL is responsible for writing model MDL files based on the data model.
// This function returns true if it succeeds and false otherwise.
bool writeMDL(QString fileName, const MolModel* molModel,
  const ParamModel* paramModel, const NotificationsModel* noteModel,
  const WarningsModel* warnModel, const ReactTreeModel* reactModel) {

  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    return false;
  }

  QTextStream out(&file);
  writeParams(out, paramModel);
  writeNotifications(out, noteModel);
  out << "\n";
  writeWarnings(out, warnModel);
  out << "\n";
  writeMolecules(out, molModel);
  out << "\n";
  //writeReactions(out, reactModel);

  return true;
}


// writeParams writes the model parameters to the QTextStream
void writeParams(QTextStream& out, const ParamModel* paramModel) {
  for (int i = 0; i < paramModel->rowCount(); ++i) {
    if (paramModel->item(i, 1)->text() == "") {
      continue;
    }
    out << paramModel->item(i, 0)->text() << " = "
        << paramModel->item(i, 1)->text() << "\n";
  }
  out << "\n";
}


// writeNotifications writes the model notifications to the QTextStream
void writeNotifications(QTextStream& out, const NotificationsModel* noteModel) {
  out << "NOTIFICATIONS {\n";
  for (int i = 0; i < noteModel->rowCount(); ++i) {
    if (noteModel->item(i, 1)->text() == "UNSET") {
      continue;
    }
    out << noteModel->item(i, 0)->text() << " = "
        << noteModel->item(i, 1)->text() << "\n";
  }
  out << "}\n";
}

// writeWarnings writes the model warnings to the QTextStream
void writeWarnings(QTextStream& out, const WarningsModel* warnModel) {
  out << "WARNINGS {\n";
  for (int i = 0; i < warnModel->rowCount(); ++i) {
    if (warnModel->item(i, 1)->text() == "UNSET") {
      continue;
    }
    out << warnModel->item(i, 0)->text() << " = "
        << warnModel->item(i, 1)->text() << "\n";
  }
  out << "}\n";
}

// writeMolecules writes the molecule info to the QTextStream.
void writeMolecules(QTextStream& out, const MolModel* molModel) {

  const MolList& molecules = molModel->getMols();
  out << "DEFINE_MOLECULES {\n";

  for (const auto& m : molecules) {
    out << TAB << m->name << " {\n";
    if (m->type == MolType::VOL) {
      out << TAB << TAB << "DIFFUSION_CONSTANT_3D";
    } else {
      out << TAB << TAB << "DIFFUSION_CONSTANT_2D";
    }
    out << " = " << m->D << "\n";
    out << TAB << "}\n";
  }
  out << "}\n";
}

#if 0
// writeReactinos writes the molecule info to the QTextStream.
void writeReactions(QTextStream& out, const ReactionModel* reactModel) {

  const ReactList& reacts = reactModel->getReactions();
  out << "DEFINE_REACTIONS {\n";

  for (const auto& r : reacts) {
    out << TAB << r->reactant1->name << "\n";
  }
  out << "}\n";
}
#endif
