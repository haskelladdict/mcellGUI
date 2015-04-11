// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <algorithm>

#include "reactionModel.hpp"

// constructor
ReactionModel::ReactionModel(QObject* parent) :
  QAbstractTableModel(parent), numCols_(headerLabels_.size()) {}


// rowCount returns the number of rows in the model
int ReactionModel::rowCount(const QModelIndex& parent) const {
  return parent.isValid() ? 0 : reactions_.size();
}


// columnCount returns the number of columns in the model
int ReactionModel::columnCount(const QModelIndex& parent) const {
  return parent.isValid() ? 0 : numCols_;
}

// headerData prints the table headers
QVariant ReactionModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  int headerSize = headerLabels_.size();
  if (orientation == Qt::Horizontal) {
    if (section < headerSize) {
      return headerLabels_[section];
    } else {
      return "product";
    }
  } else if (orientation == Qt::Vertical) {
      return section+1;
  }
  return QVariant();
}

// data returns the data contained at index
QVariant ReactionModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  int row = index.row();
  int col = index.column();
  int numRows = reactions_.size();
  if (row < 0 || row >= numRows || col < 0 || col >= numCols_) {
    return QVariant();
  }

  const Reaction* r = reactions_[row].get();
  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    int col = index.column();
    if (col == ReactCol::Name) {
      return r->name;
    } else if (col == ReactCol::Rate) {
      return r->rate;
    } else if (col == ReactCol::React1) {
      return r->reactant1->name;
    } else if (col == ReactCol::React2) {
      if (r->reactant2 != nullptr) {
        return r->reactant2->name;
      } else {
       return QString("");
      }
    } else if (col == ReactCol::Type) {
      if (r->type == ReactType::UNI) {
        return "->";
      } else {
        return "<->";
      }
    } else {
      size_t p = col - ReactCol::Type - 1;
      if (p < r->products.size()) {
        return r->products[p]->name;
      } else {
        return QString("");
      }
    }
  }
  return QVariant();
}

// setData enables editing of model properties via model views
bool ReactionModel::setData(const QModelIndex& index, const QVariant& v, int role) {
  if (!index.isValid() || role != Qt::EditRole) {
    return false;
  }
  int row = index.row();
  int col = index.column();
  int numRows = reactions_.size();
  if (row < 0 || row >= numRows || col < 0 || col >= numCols_) {
    return false;
  }

  Reaction* r = reactions_[row].get();
  if (col == ReactCol::Name) {
    r->name = v.toString();
  } else if (col == ReactCol::Rate) {
    r->rate = v.toString();
  } else if (col == ReactCol::React1) {
    r->reactant1 = static_cast<const Molecule*>(v.value<void *>());
  } else if (col == ReactCol::React2) {
     r->reactant2 = static_cast<const Molecule*>(v.value<void *>());
  } else if (col == ReactCol::Type) {
    if (v.toString() == "->") {
      r->type = ReactType::UNI;
    } else {
      r->type = ReactType::BI;
    }
  } else {
    //int p = col - ReactCol::Type - 1;
    // retrieve product and store
  }
  emit dataChanged(index, index);
  return true;
}

// flags signals the model is read and writable
Qt::ItemFlags ReactionModel::flags(const QModelIndex& index) const {
  Qt::ItemFlags flags = QAbstractTableModel::flags(index);
  if (index.isValid()) {
    flags |= Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
  }
  return flags;
}


// addReaction adds a new reaction to the model
// NOTE: addReaction assumes that the given reaction is distinct from all
// existing reactions
void ReactionModel::addReaction(const QString& reactName, const QString& rate,
    const Molecule* react1, const Molecule* react2, const ReactType& type,
    std::vector<const Molecule*>&& products) {
  auto r = std::unique_ptr<Reaction>(new Reaction);
  r->name = reactName;
  r->rate = rate;
  r->reactant1 = react1;
  r->reactant2 = react2;
  r->type = type;
  r->products = std::move(products);
  beginResetModel();
  reactions_.push_back(std::move(r));
  endResetModel();
  //molNames_[m.name] = 1;
}


#if 0
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



// getMol returns a read only reference to the underlying molecule map.
// NOTE: This could probably be encapsulated a bit better without exposing
// the internals of how molecules are stored within the model. However,
// returning a const MolMap& seems more efficient for the time being.
const MolList& MolModel::getMols() const {
  return mols_;
}
#endif
