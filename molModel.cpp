// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <algorithm>
#include <iostream>

#include "molModel.hpp"

// constructor
MolModel::MolModel(QObject* parent) : QAbstractTableModel(parent) {

  // add some fake molecules
  Molecule m1 = {"A", "1e-3", MolType::VOL};
  Molecule m2 = {"B", "33e-6", MolType::SURF};
  Molecule m3 = {"C", "1e-3", MolType::VOL};
  molMap_[m1.name] = m1;
  molMap_[m2.name] = m2;
  molMap_[m3.name] = m3;
  generateRowMapping_();
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
    int row = index.row();
    QString molName = mols_[row];
    const Molecule& mol = molMap_.at(molName);
    switch (index.column()) {
      case 0:
        return mol.name;
      case 1:
        return mol.D;
      case 2:
        if (mol.type == MolType::SURF) {
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
    QString molName = mols_[index.row()];
    Molecule mol = molMap_[molName];
    QString type;
    QString newName;
    QString D;
    switch (index.column()) {
      case 0:
        newName = value.toString();
        if (newName.isEmpty()) {
          return false;
        }
        mol.name = newName;
        molMap_.erase(molName);
        molMap_[newName] = mol;
        break;
      case 1:
        D = value.toString();
        if (D.isEmpty()) {
          return false;
        }
        mol.D = D;
        break;
      case 2:
        type = value.toString();
        if (type == "2D") {
          mol.type = MolType::SURF;
        } else if (type == "3D") {
          mol.type = MolType::VOL;
        } else {
          return false;
        }
    }
  }
  generateRowMapping_();
  return true;
}

// flags signals the model is read and writable
Qt::ItemFlags MolModel::flags(const QModelIndex& index) const {
  Q_UNUSED(index);
  return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}


// getMol returns a reference to the molecule in row
const Molecule& MolModel::getMol(int row) const {
  QString molName = mols_[row];
  return molMap_.at(molName);
}


// delMol deletes the molecule in the given row from the model
// NOTE: since generateRowMapping_ updates the model views it appears that
// we don't need to call beginRemoveRows and endRemoveRows
void MolModel::delMol(int rowID) {
  QModelIndex mod;
  QString molName = mols_[rowID];
  molMap_.erase(molName);
  generateRowMapping_();
}


// generateRowMapping creates the model's mapping of molecules to row indices
// based on the molecule data stored in molMap_
void MolModel::generateRowMapping_() {
  mols_.clear();
  beginResetModel();
  for (auto& m : molMap_) {
    mols_.push_back(m.first);
  }
  std::sort(mols_.begin(), mols_.end());
  endResetModel();
}


