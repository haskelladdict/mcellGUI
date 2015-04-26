// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <QDebug>

#include <algorithm>
#include <cassert>

#include "reactionModel.hpp"


ReactItem::ReactItem(const ReactItemType& type, const QString& name,
  const Molecule* mol, ReactItem* parent) : type_(type), name_(name), mol_(mol),
  parent_(parent) {
  if (parent) {
    parent->addChild(this);
  }
}


// the destructor recursively deletes a children underneath this item
ReactItem::~ReactItem() {
  qDeleteAll(children_);
}


// add child adds a single child item
void ReactItem::addChild(ReactItem* item) {
  item->parent_ = this;
  children_ << item;
}


// name returns the proper name for the ReactItem
QString ReactItem::name() const {
  QString reactString;
  switch (type_) {
    case ReactItemType::Repr:
      return makeReactionString_();
    case ReactItemType::Reactant:
      Q_ASSERT(mol_ != nullptr);
      return mol_->name;
    case ReactItemType::Product:
      if (mol_ == nullptr) {
        return "NULL";
      } else {
        return mol_->name;
      }
    default:
      return name_;
  }
}



// makeReactionString generates a human readable string of the reaction based
// on the child items corresponding to reactants, products ... etc.
QString ReactItem::makeReactionString_() const {
  Q_ASSERT(type_ == ReactItemType::Repr);
  QStringList reacts;
  QStringList prods;
  QString name;
  QString rate;
  for (const auto c : children_) {
    switch (c->type()) {
      case ReactItemType::ReactantTag:
        for (const auto i : c->children()) {
          Q_ASSERT(i->type() == ReactItemType::Reactant);
          if (i != nullptr) {
            reacts << i->name();
          }
        }
        break;
      case ReactItemType::ProductTag:
        for (const auto i : c->children()) {
          Q_ASSERT(i->type() == ReactItemType::Product);
          prods << i->name();
        }
        break;
      case ReactItemType::NameTag:
        name = c->childAt(0)->name();
        break;
      case ReactItemType::RateTag:
        rate = c->childAt(0)->name();
        break;
      default:
        break;
    }
  }
  return reacts.join(" + ") + " -> " + prods.join(" + ")
    + QString("     [%1]  : %2").arg(rate, name);
}


ReactItemType ReactItem::type() const {
  return type_;
}


const Molecule* ReactItem::mol() const {
  return mol_;
}


bool ReactItem::isEditable() const {
  return type_ == ReactItemType::Reactant || type_ == ReactItemType::Product ||
    type_ == ReactItemType::Rate || type_ == ReactItemType::Name;
}


ReactItem* ReactItem::parent() const {
  return parent_;
}


ReactItem* ReactItem::childAt(int row) const {
  return children_.value(row);
}


const QList<ReactItem*> ReactItem::children() const {
  return children_;
}


int ReactItem::childCount() const {
  return children_.count();
}


int ReactItem::rowOfChild(ReactItem* child) const {
  return children_.indexOf(child);
}


void ReactItem::setName(const QString& name) {
  name_ = name;
}


void ReactItem::setMol(const Molecule* mol) {
  mol_ = mol;
}


void ReactItem::insertChild(int row, ReactItem *item) {
  item->parent_ = this;
  children_.insert(row, item);
}


ReactItem* ReactItem::takeChild(int row) {
  ReactItem* item = children_.takeAt(row);
  Q_ASSERT(item);
  item->parent_ = nullptr;
  return item;
}



// ReactTreeModel encapsulates the currently defined reactions as a tree model
ReactTreeModel::ReactTreeModel(QObject* parent) :
  QAbstractItemModel(parent), root_(nullptr)  {}


ReactTreeModel::~ReactTreeModel() {
  delete root_;
}


// flags returns the proper flags for the tracked ReactItems some of which
// are editable and some of which are not.
Qt::ItemFlags ReactTreeModel::flags(const QModelIndex& index) const {
  Qt::ItemFlags flags = QAbstractItemModel::flags(index);
  if (index.isValid()) {
    flags |= Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    ReactItem* item = itemForIndex_(index);
    if (item->isEditable()) {
      flags |= Qt::ItemIsEditable;
    }
  }
  return flags;
}


QVariant ReactTreeModel::data(const QModelIndex& index, int role) const {
  if (!root_ || !index.isValid() || index.column() < 0 ||
    index.column() >= columnCount_) {
    return QVariant();
  }
  if (ReactItem* item = itemForIndex_(index)) {
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
      return item->name();
    }
  }
  return QVariant();
}


bool ReactTreeModel::setData(const QModelIndex& index, const QVariant& v,
  int role) {
  if (!index.isValid() || index.column() != 0) {
    return false;
  }
  if (ReactItem* item = itemForIndex_(index)) {
    if (role == Qt::EditRole) {
      switch(item->type()) {
        case ReactItemType::Reactant:
        case ReactItemType::Product:
          if (item->mol() != nullptr) {
            emit(unuseMol(item->mol()->id));
          }
          item->setMol(static_cast<const Molecule*>(v.value<void *>()));
          if (item->mol() != nullptr) {    // will happen for NULL product
            emit(useMol(item->mol()->id));
          }
          break;
      default:
        item->setName(v.toString());
        break;
      }
      return true;
    } else {
      return false;
    }
  }
  return true;
}


// itemForIndex returns a pointer to the ReactItem corresponding to index
ReactItem* ReactTreeModel::itemForIndex_(const QModelIndex& index) const {
  if (index.isValid()) {
    if (ReactItem* item = static_cast<ReactItem*>(index.internalPointer())) {
      return item;
    }
  }
  return root_;
}


QVariant ReactTreeModel::headerData(int section, Qt::Orientation orient,
  int role) const {

  if (orient == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
    return tr("Reactions");
  }
  return QVariant();
}


// index returns the QModelIndex for an item at row and column and parent.
QModelIndex ReactTreeModel::index(int row, int column, const QModelIndex& parent) const {
  if (!root_ || row < 0 || column < 0 || column >= columnCount_ ||
    (parent.isValid() && parent.column() != 0)) {
    return QModelIndex();
  }
  ReactItem* item = itemForIndex_(parent);
  Q_ASSERT(item);
  if (ReactItem* r = item->childAt(row)) {
    return createIndex(row, column, r);
  }
  return QModelIndex();
}


// parent returns a pointer the the parent item of index
QModelIndex ReactTreeModel::parent(const QModelIndex& index) const {
  if (!index.isValid()) {
    return QModelIndex();
  }
  if (ReactItem* childItem = itemForIndex_(index)) {
    if (ReactItem* parentItem = childItem->parent()) {
      if (parentItem == root_) {
        return QModelIndex();
      }
      if (ReactItem* grandParentItem = parentItem->parent()) {
        int row = grandParentItem->rowOfChild(parentItem);
        return createIndex(row, 0, parentItem);
      }
    }
  }
  return QModelIndex();
}


// rowCount returns the number of rows underneath parent
int ReactTreeModel::rowCount(const QModelIndex& parent) const {
  if (parent.isValid() && parent.column() != 0) {
    return 0;
  }
  ReactItem* parentItem = itemForIndex_(parent);
  return parentItem ? parentItem->childCount() : 0;
}


// columnCount only returns the fixed column count for column 0 only.
int ReactTreeModel::columnCount(const QModelIndex& parent) const {
  return (parent.isValid() && parent.column() != 0) ? 0 : columnCount_;
}


bool ReactTreeModel::setHeaderData(int section, Qt::Orientation orient,
  const QVariant& value, int role) {
  Q_UNUSED(section);
  Q_UNUSED(value);
  Q_UNUSED(orient);
  Q_UNUSED(role);
  return false;
}



bool ReactTreeModel::insertRows(int row, int count, const QModelIndex& parent) {
  if (!root_) {
    root_ = new ReactItem(ReactItemType::Repr, "");
  }
  ReactItem* parentItem = parent.isValid() ? itemForIndex_(parent) : root_;
  beginInsertRows(parent, row, row+count-1);
  for (int i=0; i < count; ++i) {
    ReactItem* item = new ReactItem(ReactItemType::Repr, tr("NewItem"));
    parentItem->insertChild(row, item);
  }
  endInsertRows();
  return true;
}


bool ReactTreeModel::removeRows(int row, int count, const QModelIndex& parent) {
  if (!root_) {
    return false;
  }
  ReactItem* parentItem = parent.isValid() ? itemForIndex_(parent) : root_;
  beginRemoveRows(parent, row, row+count-1);
  for (int i=0; i<count; ++i) {
    delete parentItem->takeChild(row);
  }
  endRemoveRows();
  return true;
}


// addReaction adds a new default reaction to the model which can then be
// edited by the user.
void ReactTreeModel::addReaction(const QString& reactName, const QString& rate,
  const Molecule* react1, const Molecule* react2, const Molecule* prod1) {
  ReactItem* parentItem;
  if (!root_) {
    root_ = new ReactItem(ReactItemType::Repr, "");
  }
  beginInsertRows(QModelIndex(), 0, 4);
  ReactItem* reaction = new ReactItem(ReactItemType::Repr, tr(""));
  root_->insertChild(0, reaction);

  ReactItem* reactItem = new ReactItem(ReactItemType::ReactantTag, tr("reactants"));
  reaction->insertChild(0, reactItem);
  ReactItem* react1Item = new ReactItem(ReactItemType::Reactant, "", react1);
  reactItem->insertChild(0, react1Item);
  emit(useMol(react1->id));
  ReactItem* react2Item = new ReactItem(ReactItemType::Reactant, "", react2);
  reactItem->insertChild(1, react2Item);
  emit(useMol(react2->id));

  ReactItem* prodItem = new ReactItem(ReactItemType::ProductTag, tr("products"));
  reaction->insertChild(1, prodItem);
  ReactItem* prod1Item = new ReactItem(ReactItemType::Product, "", prod1);
  prodItem->insertChild(0, prod1Item);
  emit(useMol(prod1->id));

  ReactItem* rateItem = new ReactItem(ReactItemType::RateTag, tr("rate"));
  reaction->insertChild(2, rateItem);
  ReactItem* rate1Item = new ReactItem(ReactItemType::Rate, tr("0.0"));
  rateItem->insertChild(0, rate1Item);

  ReactItem* nameItem = new ReactItem(ReactItemType::NameTag, tr("name"));
  reaction->insertChild(3, nameItem);
  ReactItem* name1Item = new ReactItem(ReactItemType::Name, tr("reaction"));
  nameItem->insertChild(0, name1Item);

  endInsertRows();
}











