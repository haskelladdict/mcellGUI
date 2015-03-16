// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <iostream>
#include <set>

#include <QComboBox>

#include "molWidget.hpp"


// constructor
MolWidget::MolWidget(QWidget* parent, Qt::WindowFlags flags) :
  QWidget(parent, flags) {

  setupUi(this);
  molTableView->setModel(&model_);
  molTableView->setItemDelegate(&delegate_);

  connect(addMolButton, SIGNAL(clicked()), this, SLOT(printIt()));
  connect(deleteMolButton, SIGNAL(clicked()), this, SLOT(deleteMols()));
}


// deleteMols deletes all currently selected molecules from the model
void MolWidget::deleteMols() {
  auto selModel = molTableView->selectionModel();
  auto selIDs = selModel->selectedIndexes();
  if (selIDs.size() != 1) {
    return;
  }
  int rowID = selIDs[0].row();
  model_.delMol(rowID);
}


// printIt is a helper function for testing
void MolWidget::printIt() {
  std::cout << "pressed the button " << std::endl;
}


// MolModelDelegate constructor
MolModelDelegate::MolModelDelegate(QWidget *parent) :
  QItemDelegate(parent) {};


// createEditor creates the appropriate editor for each of the columns
QWidget* MolModelDelegate::createEditor(QWidget *parent,
  const QStyleOptionViewItem &option, const QModelIndex &index) const {

  Q_UNUSED(option)

  QLineEdit* edit;
  QComboBox* comb;
  switch (index.column()) {
    case 0:
    case 1:
      edit = new QLineEdit(parent);
      return edit;
    case 2:
      comb = new QComboBox(parent);
      comb->addItem("3D");
      comb->addItem("2D");
      return comb;
  }
  return nullptr;
}

// setEditorData reads the data and writes it to the editor
void MolModelDelegate::setEditorData(QWidget* editor,
  const QModelIndex& index) const {

  QVariant v = index.model()->data(index, Qt::EditRole);
  QLineEdit* edit;
  QComboBox* combo;
  switch (index.column()) {
    case 0:
      edit = static_cast<QLineEdit*>(editor);
      edit->setText(v.toString());
      break;
    case 1:
      edit = static_cast<QLineEdit*>(editor);
      edit->setValidator(new QDoubleValidator);
      edit->setText(v.toString());
      break;
    case 2:
      combo = static_cast<QComboBox*>(editor);
      combo->setCurrentText(v.toString());
      break;
  }
}


