// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

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
  reactTableView->setSortingEnabled(true);

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
void ReactionWidget::initModel(ReactionModel* reactModel, MolModel* molModel) {
  reactModel_ = reactModel;
  molModel_ = molModel;

  auto proxyModel = new QSortFilterProxyModel(this);
  proxyModel->setSourceModel(reactModel_);
  reactTableView->setModel(proxyModel);

  ReactionModelDelegate* del = new ReactionModelDelegate(molModel, this);
  reactTableView->setItemDelegate(del);
}


// deleteReactions deletes all currently selected molecules from the model
void ReactionWidget::deleteReactions() {
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
    QList<QVariant> ql = reactModel_->index(r, Col::Name).data().toList();
    reactIDs.insert(ql[1].toLongLong());
  }
  for (auto& i : reactIDs) {
    reactModel_->delReaction(i);
  }
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
  Molecule* mol = molModel_->getMols()[1].get();
  std::vector<const Molecule*> products;
  products.push_back(mol);
  reactModel_->addReaction(reactName, "0.0", mol, mol, ReactType::UNI, mol);
}


// ReactionModelDelegate constructor
// NOTE: The delegate needs access to the molModel in order to populate
// properly populate the molecule selectors with the available molecule names
ReactionModelDelegate::ReactionModelDelegate(const MolModel* molModel,
  QWidget *parent) : QItemDelegate(parent), molModel_(molModel) {};


// createEditor creates the appropriate editor for each of the columns
QWidget* ReactionModelDelegate::createEditor(QWidget *parent,
  const QStyleOptionViewItem &option, const QModelIndex &index) const {

  Q_UNUSED(option)

  QLineEdit* edit;
  QComboBox* comb;
  switch (index.column()) {
    case ReactCol::Name:
    case ReactCol::Rate:
      edit = new QLineEdit(parent);
      return edit;
    case ReactCol::Type:
      comb = new QComboBox(parent);
      comb->addItem("->");
      comb->addItem("<->");
      return comb;
    case ReactCol::React1:
      comb = new QComboBox(parent);
      comb->addItems(molModel_->getMolNames());
      return comb;
    case ReactCol::React2:
      comb = new QComboBox(parent);
      comb->addItem("---");
      comb->addItems(molModel_->getMolNames());
      return comb;
    case ReactCol::Prod1:
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
void ReactionModelDelegate::setEditorData(QWidget* editor,
  const QModelIndex& index) const {

  QVariant v = index.model()->data(index, Qt::EditRole);
  QLineEdit* edit;
  QComboBox* combo;
  QList<QVariant> ql;
  switch (index.column()) {
    case ReactCol::Name:
      edit = qobject_cast<QLineEdit*>(editor);
      Q_ASSERT(edit);
      // the name property returns both the non-unique reaction name
      // as well as the unique reaction id
      ql = v.toList();
      edit->setText(ql[0].toString());
      break;
    case ReactCol::Rate:
      edit = qobject_cast<QLineEdit*>(editor);
      Q_ASSERT(edit);
      edit->setValidator(new QDoubleValidator);
      edit->setText(v.toString());
      break;
    case ReactCol::Type:
    case ReactCol::React1:
    case ReactCol::React2:
    case ReactCol::Prod1:
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
void ReactionModelDelegate::setModelData(QWidget *editor, QAbstractItemModel* model,
  const QModelIndex& index) const {

  QLineEdit* edit;
  QComboBox* combo;
  QVariant v;
  const Molecule* mol;
  switch (index.column()) {
    case ReactCol::Name:
    case ReactCol::Rate:
      edit = qobject_cast<QLineEdit*>(editor);
      Q_ASSERT(edit);
      model->setData(index, edit->text());
      break;
    case ReactCol::Type:
      combo = qobject_cast<QComboBox*>(editor);
      Q_ASSERT(edit);
      model->setData(index, combo->currentText());
      break;
    case ReactCol::React1:
    case ReactCol::React2:
    case ReactCol::Prod1:
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


// we need to re-implement the paint function to properly display the
// reaction name since the model returns a QList<QVariant> containing
// both the name and the unique reaction ID.
void ReactionModelDelegate::paint(QPainter* painter,
  const QStyleOptionViewItem &option, const QModelIndex &index) const {

  if (index.column() == ReactCol::Name) {
    QVariant v = index.model()->data(index, Qt::EditRole);
    QList<QVariant> qv = v.toList();
    drawDisplay(painter, option, option.rect, qv[0].toString());
    drawFocus(painter, option, option.rect);
  } else{
    QItemDelegate::paint(painter, option, index);
  }
}













