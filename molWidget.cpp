// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <iostream>

#include "molWidget.hpp"

// constructor
MolWidget::MolWidget(QWidget* parent, Qt::WindowFlags flags) :
  QWidget(parent, flags),
  model_(nullptr) {

  setupUi(this);
  molTableView->setModel(&model_);

  connect(addEditMolButton, SIGNAL(clicked()), this, SLOT(printIt()));
}


// printIt is a helper function for testing
void MolWidget::printIt() {
  std::cout << "pressed the button " << std::endl;
}
