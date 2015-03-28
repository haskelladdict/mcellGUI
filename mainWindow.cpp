// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <QFileDialog>

#include "io.hpp"
#include "mainWindow.hpp"

// constructor
MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags) :
  QMainWindow(parent, flags) {

  setupUi(this);

  // initialize and propagate models to view widgets
  molTab->initModel(&moleculeModel_);
  paramTab->initModel(&paramModel_);
  noteWarnTab->initModel(&noteModel_, &warnModel_);

  // add some fake molecule data
  moleculeModel_.addMol(Molecule{"A", "1e-3", MolType::VOL});
  moleculeModel_.addMol(Molecule{"B", "33e-6", MolType::SURF});
  moleculeModel_.addMol(Molecule{"C", "1e-3", MolType::VOL});

  // signals and slots
  connect(exportMDLAction, SIGNAL(triggered(bool)), this, SLOT(exportMDL_()));
}


// exportMDL asks the user for the export path and then forwards path and model
// info to the relevant io routine.
void MainWindow::exportMDL_() {
  QString mdlFileName = QFileDialog::getSaveFileName(this, tr("Export MDL"),
    QDir::homePath(), tr("MCell Model Files (*.mdl)"));
  if (mdlFileName.isEmpty()) {
    return;
  }
  writeMDL(mdlFileName, moleculeModel_.getMols(), paramModel_, noteModel_,
    warnModel_);
}

