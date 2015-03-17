// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <iostream>

#include "mainWindow.hpp"

// constructor
MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags) :
  QMainWindow(parent, flags) {
  setupUi(this);

  // propagate models to view widgets
  molTab->initModel(&moleculeModel_);

  // add some fake molecule data
  moleculeModel_.addMol("A", {"1e-3", MolType::VOL});
  moleculeModel_.addMol("B", {"33e-6", MolType::SURF});
  moleculeModel_.addMol("C", {"1e-3", MolType::VOL});

  // signals and slots
  connect(exportMDLAction, SIGNAL(triggered(bool)), this, SLOT(exportMDL_()));
}


// exportMDL is responsible for exporting model MDL files based on the data model
void MainWindow::exportMDL_() {
  std::cout << "exporting MDL file .... " << std::endl;
}

