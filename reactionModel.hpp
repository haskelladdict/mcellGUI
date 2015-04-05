// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef REACTION_MODEL_HPP
#define REACTION_MODEL_HPP

#include <map>
#include <vector>

#include <QAbstractTableModel>
#include <QList>
#include <QString>

#include "molModel.hpp"

// ReactType classifies the type of reaction. Currently only uni and
// bidirectional are supported
enum class ReactType {UNI, BI};

// Reaction class stores the data for a single reaction
struct Reaction {
  QString name;
  QString rate;
  const Molecule* reactant1;
  const Molecule* reactant2;
  ReactType type;
  QList<const Molecule*> products;
};
using ReactList = QList<Reaction>;

// ReactCol::col lists column names (one per data element in Reaction with
// exception of products since the number of products is not fixed and unknown)
namespace ReactCol {
  enum col {Name, Rate, React1, React2, Type};
}

// ReactModel describes the QT MVC data model for reactions
class ReactionModel : public QAbstractTableModel {

  Q_OBJECT

public:
  ReactionModel(QObject* parent = 0);

  int rowCount(const QModelIndex& parent = QModelIndex()) const ;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;

  // read methods
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex& index) const;

//  bool hasReact(const QString& molName) const;
//  const ReactList& getReactions() const;

  // write methods
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
  void addReaction(const Reaction& data);
//  void delReaction(int rowID);


private:
  ReactList reactions_;
  //std::map<QString, int> reactNames_;

  std::vector<QString> headerLabels_ = {"reaction name", "rate", "reactant 1",
    "reactant2", "type", "product 1"};
  int numCols_;
};

#endif
