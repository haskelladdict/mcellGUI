// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <iostream>

#include "molModel.hpp"

// constructor
MolModel::MolModel(QObject* parent) : QAbstractTableModel(parent) {

  // add some fake molecules
  Molecule m1 = {"A", "1e-3", MolType::VOL};
  Molecule m2 = {"B", "33e-6", MolType::SURF};
  Molecule m3 = {"C", "1e-3", MolType::VOL};
  mols_.push_back(m1);
  mols_.push_back(m2);
  mols_.push_back(m3);
}

// rowCount returns the number of rows in the model
int MolModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent)
  return mols_.size();
}

// columnCount returns the number of columns in the model
int MolModel::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent)
  return labels_.size();
}

// headerData prints the table headers
QVariant MolModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      return labels_[section];
    } else if (orientation == Qt::Vertical) {
      return section;
    }
  }
  return QVariant();
}

// data returns the data contained at index
QVariant MolModel::data(const QModelIndex& index, int role) const {
  if (role == Qt::DisplayRole || role == Qt::EditRole) {
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


// setData enables editing of model properties via model views
bool MolModel::setData(const QModelIndex& index, const QVariant& value, int role) {
  if (role == Qt::EditRole) {
    Molecule& m = mols_[index.row()];
    QString type;
    QString name;
    QString D;
    switch (index.column()) {
      case 0:
        name = value.toString();
        if (name.isEmpty()) {
          return false;
        }
        m.name = name;
        break;
      case 1:
        D = value.toString();
        if (D.isEmpty()) {
          return false;
        }
        m.D = D;
        break;
      case 2:
        type = value.toString();
        if (type == "2D") {
          m.type = MolType::SURF;
        } else if (type == "3D") {
          m.type = MolType::VOL;
        } else {
          return false;
        }
    }
  }
  return true;
}

// flags signals the model is read and writable
Qt::ItemFlags MolModel::flags(const QModelIndex& index) const {
  Q_UNUSED(index);
  return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}


// getMol returns a reference to the molecule in row
const Molecule& MolModel::getMol(int row) const {
  return mols_[row];
}


// delMol deletes the molecule in the given row from the model
// FIXME: calling beginRemoveRows with a temporary seems like a hack
void MolModel::delMol(int rowID) {
  QModelIndex mod;
  beginRemoveRows(mod, rowID, rowID);
  auto it = mols_.begin() + rowID;
  mols_.erase(it);
  endRemoveRows();
}
