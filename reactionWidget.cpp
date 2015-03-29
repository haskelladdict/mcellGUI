// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <QComboBox>
#include <QMessageBox>
#include <QShortcut>

#include "reactionWidget.hpp"

// constructor
ReactionWidget::ReactionWidget(QWidget* parent, Qt::WindowFlags flags) :
  QWidget(parent, flags) {

  setupUi(this);
  reactTableView->setSortingEnabled(true);
//  reactTableView->setItemDelegate(&delegate_);

  connect(addReactionButton, SIGNAL(clicked()), this, SLOT(addReaction()));
  connect(deleteReactionsButton, SIGNAL(clicked()), this, SLOT(deleteReactions()));

  // add shortcuts for adding and deleting
  QShortcut *addShortCut = new QShortcut(QKeySequence("Ctrl+A"), this);
  connect(addShortCut, SIGNAL(activated()), this, SLOT(addReaction()));
  QShortcut *delShortCut = new QShortcut(QKeySequence("Ctrl+X"), this);
  connect(delShortCut, SIGNAL(activated()), this, SLOT(deleteReactions()));
}


// initModel initializes the widget's underlying molecule model
void ReactionWidget::initModel(ReactionModel* model) {
  model_ = model;
  auto proxyModel = new QSortFilterProxyModel(this);
  proxyModel->setSourceModel(model);
  reactTableView->setModel(proxyModel);
}

// deleteReactions deletes all currently selected molecules from the model
void ReactionWidget::deleteReactions() {
  auto selModel = reactTableView->selectionModel();
  auto selIDs = selModel->selectedIndexes();
  if (selIDs.size() == 0) {
    return;
  }
/*
  std::set<int> uniqueRows;
  for (auto& i : selIDs) {
    uniqueRows.insert(i.row());
  }
  for (auto& r : uniqueRows) {
    model_->delMol(r);
  }
  */
}


// addReaction adds the molecule defined in the define molecule grouper to the
// model after checking that it is complete and valid
void ReactionWidget::addReaction() {
  // construct a default molecule
  QString id;
  QString molName = "newMol_" + id.setNum(reactCount_++);
//  model_->addMol(Molecule{molName, "0.0", MolType::VOL});
}

#if 0
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
    case Col::Name:
    case Col::D:
      edit = new QLineEdit(parent);
      return edit;
    case Col::Type:
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
    case Col::Name:
      edit = qobject_cast<QLineEdit*>(editor);
      Q_ASSERT(edit);
      edit->setText(v.toString());
      break;
    case Col::D:
      edit = qobject_cast<QLineEdit*>(editor);
      Q_ASSERT(edit);
      edit->setValidator(new QDoubleValidator);
      edit->setText(v.toString());
      break;
    case Col::Type:
      combo = qobject_cast<QComboBox*>(editor);
      Q_ASSERT(combo);
      combo->setCurrentText(v.toString());
      break;
    default:
      QItemDelegate::setEditorData(editor, index);
      break;
  }
}


// setModelData writes the data to the model based on the editor setting
void MolModelDelegate::setModelData(QWidget *editor, QAbstractItemModel* model,
  const QModelIndex& index) const {

  QLineEdit* edit;
  QComboBox* combo;
  switch (index.column()) {
    case Col::Name:
    case Col::D:
      edit = qobject_cast<QLineEdit*>(editor);
      Q_ASSERT(edit);
      model->setData(index, edit->text());
      break;
    case Col::Type:
      combo = qobject_cast<QComboBox*>(editor);
      Q_ASSERT(edit);
      model->setData(index, combo->currentText());
      break;
    default:
      QItemDelegate::setModelData(editor, model, index);
      break;
  }
}
#endif


