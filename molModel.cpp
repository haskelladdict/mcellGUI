// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include "molModel.hpp"

// constructor
MolModel::MolModel(QObject* parent) : QAbstractTableModel(parent) {

  // add some fake molecules
  Molecule m1 = {"A", 1e-3, MolType::VOL};
  Molecule m2 = {"B", 33e-6, MolType::SURF};
  mols_.push_back(m1);
  mols_.push_back(m2);
}

// rowCount returns the number of rows in the model
int MolModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return mols_.size();
}

// columnCount returns the number of columns in the model
int MolModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return labels_.size();
}

// headerData prints the table headers
QVariant MolModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      return labels_[section];
    }
  }
  return QVariant();
}

// data returns the data contained at index
QVariant MolModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    auto row = index.row();
    switch (index.column()) {
      case 0:
        return mols_[row].name;
      case 1:
        return mols_[row].D;
      case 2:
        if (mols_[row].type == MolType::SURF) {
          return QString("2D");
        } else {
          return QString("3D");
        }
    }
  }
  return QVariant();
}
