// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <algorithm>

#include "molModel.hpp"

// constructor
MolModel::MolModel(QObject* parent) : QAbstractTableModel(parent) {}


// rowCount returns the number of rows in the model
int MolModel::rowCount(const QModelIndex& parent) const {
  return parent.isValid() ? 0 : mols_.size();
}


// columnCount returns the number of columns in the model
int MolModel::columnCount(const QModelIndex& parent) const {
  return parent.isValid() ? 0 : headerLabels_.size();
}


// headerData prints the table headers
QVariant MolModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (orientation == Qt::Horizontal) {
      return headerLabels_[section];
  } else if (orientation == Qt::Vertical) {
      return section+1;
  }
  return QVariant();
}


// data returns the data contained at index
QVariant MolModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  int row = index.row();
  int col = index.column();
  int numCols = headerLabels_.size();
  if (row < 0 || row >= mols_.size() || col < 0 || col >= numCols) {
    return QVariant();
  }

  const Molecule& m = mols_.at(row);
  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    switch (index.column()) {
      case Col::Name:
        return m.name;
      case Col::D:
        return m.D;
      case Col::Type:
        if (m.type == MolType::VOL) {
          return QString("3D");
        } else {
          return QString("2D");
        }
      default:
        Q_ASSERT(false);
    }
  }
  return QVariant();
}

// setData enables editing of model properties via model views
bool MolModel::setData(const QModelIndex& index, const QVariant& value, int role) {
  if (!index.isValid() || role != Qt::EditRole) {
    return false;
  }
  int row = index.row();
  int col = index.column();
  int numCols = headerLabels_.size();
  if (row < 0 || row >= mols_.size() || col < 0 || col >= numCols) {
    return false;
  }

  Molecule& m = mols_[row];
  QString newName, D, type;
  switch (col) {
    case Col::Name:
      newName = value.toString();
      if (newName.isEmpty() || haveMol(newName)) {
        return false;
      }
      m.name = newName;
      break;
    case Col::D:
      D = value.toString();
      if (D.isEmpty()) {
        return false;
      }
      m.D = D;
      break;
    case Col::Type:
      type = value.toString();
      if (type == "2D") {
        m.type = MolType::SURF;
      } else if (type == "3D") {
        m.type = MolType::VOL;
      } else {
        return false;
      }
  }
  emit dataChanged(index, index);
  return true;
}

// flags signals the model is read and writable
Qt::ItemFlags MolModel::flags(const QModelIndex& index) const {
  Qt::ItemFlags flags = QAbstractTableModel::flags(index);
  if (index.isValid()) {
    flags |= Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
  }
  return flags;
}


// delMol deletes the molecule in the given row from the model
// NOTE: since generateRowMapping_ updates the model views it appears that
// we don't need to call beginRemoveRows and endRemoveRows
void MolModel::delMol(int rowID) {
  molNames_.erase(mols_[rowID].name);
  beginResetModel();
  mols_.removeAt(rowID);
  endResetModel();
}


// haveMol returns true if a molecule with the provided name exists and false
// otherwise
bool MolModel::haveMol(const QString& name) const {
  return molNames_.find(name) != molNames_.end();
}


// addMol adds a new molecule of the given data to the model
// NOTE: addMol assumes that the molecule of name molName does not yet exist
void MolModel::addMol(const Molecule& m) {
  beginResetModel();
  mols_.insert(mols_.size(), m);
  endResetModel();
  molNames_[m.name] = 1;
}


// getMol returns a read only reference to the underlying molecule map.
// NOTE: This could probably be encapsulated a bit better without exposing
// the internals of how molecules are stored within the model. However,
// returning a const MolMap& seems more efficient for the time being.
const MolList& MolModel::getMols() const {
  return mols_;
}
