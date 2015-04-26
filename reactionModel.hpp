// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef REACTION_MODEL_HPP
#define REACTION_MODEL_HPP

#include <map>
#include <memory>
#include <vector>

#include <QAbstractListModel>
#include <QList>
#include <QString>

#include "molModel.hpp"


// this enum describes the type of ReactItem
enum class ReactItemType {Repr, ReactantTag, Reactant, ProductTag, Product,
  TypeTag, Type, RateTag, Rate, NameTag, Name
};



// ReactItem constitutes a single row entry in the ReactTreeModel and describes
// both editable properties (like reactants, products, name, rate) as well as
// non-editable headers.
class ReactItem {

public:

  explicit ReactItem(const ReactItemType& type, const QString& name,
    const Molecule* mol = nullptr, ReactItem* parent = nullptr);
  ~ReactItem();

  QString name() const;
  ReactItemType type() const;
  bool isEditable() const;

  ReactItem* parent() const;
  ReactItem* childAt(int row) const;
  const QList<ReactItem*> children() const;
  const Molecule* mol() const;
  int rowOfChild(ReactItem* child) const;
  int childCount() const;

  void setName(const QString& name);
  void setMol(const Molecule* mol);

  void insertChild(int row, ReactItem* item);
  void addChild(ReactItem* item);
  ReactItem* takeChild(int row);


private:

  QString makeReactionString_() const;

  ReactItemType type_;
  QString name_;
  const Molecule* mol_;

  ReactItem* parent_;
  QList<ReactItem*> children_;
};



// ReactTreeModel encapsulates the currently defined reactions as a tree model
class ReactTreeModel : public QAbstractItemModel {

 Q_OBJECT

public:

  explicit ReactTreeModel(QObject* parent = nullptr);
  ~ReactTreeModel();

  Qt::ItemFlags flags(const QModelIndex& index) const;
  QVariant data(const QModelIndex& index, int role) const;
  QVariant headerData(int section, Qt::Orientation orient, int role) const;
  QModelIndex index(int row, int column, const QModelIndex& parent) const;
  QModelIndex parent(const QModelIndex& index) const;
  int rowCount(const QModelIndex& index) const;
  int columnCount(const QModelIndex& index) const;

  bool setHeaderData(int section, Qt::Orientation orient, const QVariant& value,
    int role = Qt::EditRole);
  bool setData(const QModelIndex& index, const QVariant& value, int role);
  bool insertRows(int row, int count, const QModelIndex& parent);
  bool removeRows(int row, int count, const QModelIndex& parent);

  void addReaction(const QString& reactName, const QString& rate, const Molecule* react1,
    const Molecule* react2, const Molecule* prod1);


signals:

  void useMol(long id);
  void unuseMol(long id);


private:

  ReactItem* itemForIndex_(const QModelIndex& index) const;

  const int columnCount_ = 1;
  ReactItem* root_;

};


#endif
