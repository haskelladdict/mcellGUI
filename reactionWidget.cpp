// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <utility>

#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QShortcut>

#include "reactionWidget.hpp"

// constructor
ReactionWidget::ReactionWidget(QWidget* parent, Qt::WindowFlags flags) :
  QWidget(parent, flags) {

  setupUi(this);
  reactTableView->setSortingEnabled(true);
  reactTableView->setItemDelegate(&delegate_);

  connect(addReactionButton, SIGNAL(clicked()), this, SLOT(addReaction()));
  connect(deleteReactionsButton, SIGNAL(clicked()), this, SLOT(deleteReactions()));

  // add shortcuts for adding and deleting
  QShortcut *addShortCut = new QShortcut(QKeySequence("Ctrl+A"), this);
  connect(addShortCut, SIGNAL(activated()), this, SLOT(addReaction()));
  QShortcut *delShortCut = new QShortcut(QKeySequence("Ctrl+X"), this);
  connect(delShortCut, SIGNAL(activated()), this, SLOT(deleteReactions()));
}


// initModel initializes the widget's underlying reaction model. In addition
// to the ReactionModel, the widget also keeps track of the MoleculeModel
void ReactionWidget::initModel(ReactionModel* reactModel, const MolModel* molModel) {
  reactModel_ = reactModel;
  molModel_ = molModel;
  auto proxyModel = new QSortFilterProxyModel(this);
  proxyModel->setSourceModel(reactModel_);
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
 if (molModel_->numMols() == 0) {
    QMessageBox::critical(this, tr("No Molecule Defined"),
      tr("Please define at least one molecule before adding reactions"),
      QMessageBox::Close);
    return;
  }
  // construct a default reaction
  QString id;
  QString reactName = "newReaction_" + id.setNum(reactCount_++);
  Molecule* mol = molModel_->getMols()[0].get();
  std::vector<const Molecule*> products;
  reactModel_->addReaction(reactName, "0.0", mol, mol, ReactType::UNI,
    std::move(products));
}


// ReactionModelDelegate constructor
ReactionModelDelegate::ReactionModelDelegate(QWidget *parent) :
  QItemDelegate(parent) {};


// createEditor creates the appropriate editor for each of the columns
QWidget* ReactionModelDelegate::createEditor(QWidget *parent,
  const QStyleOptionViewItem &option, const QModelIndex &index) const {

  Q_UNUSED(option)

  QLineEdit* edit;
  QComboBox* comb;
  switch (index.column()) {
    case ReactCol::Name:
    case ReactCol::Rate:
    case ReactCol::Type:
      edit = new QLineEdit(parent);
      return edit;
    case ReactCol::React1:
    case ReactCol::React2:
    default:
      comb = new QComboBox(parent);
      return comb;
  }
  return nullptr;
}


// setEditorData reads the data and writes it to the editor
void ReactionModelDelegate::setEditorData(QWidget* editor,
  const QModelIndex& index) const {

  QVariant v = index.model()->data(index, Qt::EditRole);
  QLineEdit* edit;
  QComboBox* combo;
  switch (index.column()) {
    case ReactCol::Name:
      edit = qobject_cast<QLineEdit*>(editor);
      Q_ASSERT(edit);
      edit->setText(v.toString());
      break;
    case ReactCol::Rate:
      edit = qobject_cast<QLineEdit*>(editor);
      Q_ASSERT(edit);
      edit->setValidator(new QDoubleValidator);
      edit->setText(v.toString());
      break;
    case ReactCol::React1:
    case ReactCol::React2:
      combo = qobject_cast<QComboBox*>(editor);
      Q_ASSERT(combo);
      combo->setCurrentText(v.toString());
      break;
    default:
      QItemDelegate::setEditorData(editor, index);
      break;
  }
}

#if 0
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
