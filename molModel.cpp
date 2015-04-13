// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <algorithm>
#include <cassert>
#include <utility>

#include "molModel.hpp"

// constructor
MolModel::MolModel(QObject* parent) : QAbstractTableModel(parent),
  molID_(0)  {}


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
  int numRows = mols_.size();
  if (row < 0 || row >= numRows || col < 0 || col >= numCols) {
    return QVariant();
  }

  const Molecule* m = mols_[row].get();
  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    switch (index.column()) {
      case Col::Name:
        return m->name;
      case Col::D:
        return m->D;
      case Col::Type:
        if (m->type == MolType::VOL) {
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
  int numRows = mols_.size();
  if (row < 0 || row >= numRows || col < 0 || col >= numCols) {
    return false;
  }

  Molecule* m = mols_[row].get();
  QString newName, D, type;
  switch (col) {
    case Col::Name:
      newName = value.toString();
      if (newName.isEmpty() || haveMol(newName)) {
        return false;
      }
      m->name = newName;
      break;
    case Col::D:
      D = value.toString();
      if (D.isEmpty()) {
        return false;
      }
      m->D = D;
      break;
    case Col::Type:
      type = value.toString();
      if (type == "2D") {
        m->type = MolType::SURF;
      } else if (type == "3D") {
        m->type = MolType::VOL;
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
void MolModel::delMol(const QString& name) {
  auto it = std::find_if(mols_.begin(), mols_.end(),
    [&name](std::unique_ptr<Molecule> const& p) { return p->name == name; });
  assert(it != mols_.end());
  beginResetModel();
  mols_.erase(it);
  endResetModel();
}


// haveMol returns true if a molecule with the provided name exists and false
// otherwise
bool MolModel::haveMol(const QString& name) const {
  auto it = std::find_if(mols_.begin(), mols_.end(),
    [&name](std::unique_ptr<Molecule> const& p) { return p->name == name; });
  return it != mols_.end();
}


// numMol returns the number of molecules available in the model
int MolModel::numMols() const {
  return mols_.size();
}


// addMol adds a new molecule of the given data to the model
// NOTE: addMol assumes that the molecule of name molName does not yet exist
void MolModel::addMol(const QString& name, const QString& D, const MolType& type) {
  // create new Molecule
  auto m = std::unique_ptr<Molecule>(new Molecule());
  m->name = name;
  m->D = D;
  m->type = type;
  m->id = molID_++;

  beginResetModel();
  mols_.push_back(std::move(m));
  endResetModel();
}


// getMol returns a read only reference to the underlying molecule map.
// NOTE: This could probably be encapsulated a bit better without exposing
// the internals of how molecules are stored within the model. However,
// returning a const MolMap& seems more efficient for the time being.
const MolList& MolModel::getMols() const {
  return mols_;
}

// getMolecule returns a pointer to the molecule of given name
const Molecule* MolModel::getMolecule(QString name) const {
  auto it = std::find_if(mols_.begin(), mols_.end(),
    [&name](std::unique_ptr<Molecule> const& p) { return p->name == name; });
  if (it == mols_.end()) {
    return nullptr;
  }
  return it->get();
}


// getMolNames returns the list of current molecule names
QStringList MolModel::getMolNames() const {
  QStringList names;
  for (auto &i : mols_) {
    names << i->name;
  }
  return names;
}








