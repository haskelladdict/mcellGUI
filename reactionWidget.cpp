// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <cassert>
#include <QDebug>

#include <set>
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
  //reactListView->setSortingEnabled(true);

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
void ReactionWidget::initModel(ReactTreeModel* reactModel, MolModel* molModel) {
  reactModel_ = reactModel;
  molModel_ = molModel;

  //auto proxyModel = new QSortFilterProxyModel(this);
  //proxyModel->setSourceModel(reactModel_);
  reactTreeView->setModel(reactModel_);
  //reactTableView->setColumnHidden(0,true);

  //ReactionModelDelegate* del = new ReactModelDelegate(molModel, this);
  reactTreeView->setItemDelegate(new ReactModelDelegate(molModel, this));
}


// deleteReactions deletes all currently selected molecules from the model
void ReactionWidget::deleteReactions() {
  /*
  auto selModel = reactTableView->selectionModel();
  auto selIDs = selModel->selectedIndexes();
  if (selIDs.size() == 0) {
    return;
  }

  std::set<int> uniqueRows;
  for (auto& i : selIDs) {
    uniqueRows.insert(i.row());
  }
  std::set<int> reactIDs;
  for (auto& r : uniqueRows) {
    auto ql = reactModel_->index(r, ReactCol::ID).data().toLongLong();
    reactIDs.insert(ql);
  }
  for (auto& i : reactIDs) {
    reactModel_->delReaction(i);
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
  QString reactName = "reac_" + id.setNum(reactCount_++);
  Molecule* mol = molModel_->getMols()[0].get();
  std::vector<const Molecule*> products;
  products.push_back(mol);
  reactModel_->addReaction(reactName, "0.0", mol, mol, mol);
}


// ReactModelDelegate constructor
// NOTE: The delegate needs access to the molModel in order to populate
// properly populate the molecule selectors with the available molecule names
ReactModelDelegate::ReactModelDelegate(const MolModel* molModel,
  QWidget *parent) : QItemDelegate(parent), molModel_(molModel) {};


// createEditor creates the appropriate editor for each of the columns
QWidget* ReactModelDelegate::createEditor(QWidget *parent,
  const QStyleOptionViewItem &option, const QModelIndex &index) const {

  Q_UNUSED(option)

  QLineEdit* edit;
  QComboBox* comb;
  ReactItem* item = static_cast<ReactItem*>(index.internalPointer());
  switch (item->type()) {
    case ReactItemType::Name:
    case ReactItemType::Rate:
      edit = new QLineEdit(parent);
      return edit;
    case ReactItemType::Reactant:
      comb = new QComboBox(parent);
      comb->addItems(molModel_->getMolNames());
      return comb;
    case ReactItemType::Product:
      comb = new QComboBox(parent);
      comb->addItem("NULL");
      comb->addItems(molModel_->getMolNames());
      return comb;
    default:
      break;
  }
  return nullptr;
}


// setEditorData reads the data and writes it to the editor
void ReactModelDelegate::setEditorData(QWidget* editor,
  const QModelIndex& index) const {

  QVariant v = index.model()->data(index, Qt::EditRole);
  ReactItem* item = static_cast<ReactItem*>(index.internalPointer());
  QLineEdit* edit;
  QComboBox* combo;
  qlonglong id;
  switch (item->type()) {
    case ReactItemType::Name:
      edit = qobject_cast<QLineEdit*>(editor);
      Q_ASSERT(edit);
      edit->setText(v.toString());
      break;
    case ReactItemType::Rate:
      edit = qobject_cast<QLineEdit*>(editor);
      Q_ASSERT(edit);
      edit->setValidator(new QDoubleValidator);
      edit->setText(v.toString());
      break;
    case ReactItemType::Reactant:
    case ReactItemType::Product:
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
// NOTE: Currently we send the molecule info to the reaction model as void
// pointers within a QVariant. This seems a bit hackish and could perhaps be
// improved.
void ReactModelDelegate::setModelData(QWidget *editor, QAbstractItemModel* model,
  const QModelIndex& index) const {

  QLineEdit* edit;
  QComboBox* combo;
  QVariant v;
  ReactItem* item = static_cast<ReactItem*>(index.internalPointer());
  const Molecule* mol;
  switch (item->type()) {
    case ReactItemType::Name:
    case ReactItemType::Rate:
      edit = qobject_cast<QLineEdit*>(editor);
      Q_ASSERT(edit);
      model->setData(index, edit->text());
      break;
    case ReactItemType::Reactant:
    case ReactItemType::Product:
      combo = qobject_cast<QComboBox*>(editor);
      mol = molModel_->getMolecule(combo->currentText());
      v = qVariantFromValue((void *)mol);
      model->setData(index, v);
      break;
    default:
      QItemDelegate::setModelData(editor, model, index);
      break;
  }
}
