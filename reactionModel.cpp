// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <QDebug>

#include <algorithm>
#include <cassert>

#include "reactionModel.hpp"

// constructor
ReactionModel::ReactionModel(QObject* parent) :
  QAbstractTableModel(parent), reactCount_(0), numCols_(headerLabels_.size()) {}


// rowCount returns the number of rows in the model
int ReactionModel::rowCount(const QModelIndex& parent) const {
  return parent.isValid() ? 0 : reactions_.size();
}


// columnCount returns the number of columns in the model
int ReactionModel::columnCount(const QModelIndex& parent) const {
  return parent.isValid() ? 0 : numCols_;
}

// isOriented returns if the reaction with index i is oriented, i.e. contains
// 2D molecules or surfaces
bool ReactionModel::isOriented(qlonglong reactID) {
  auto it = std::find_if(reactions_.begin(), reactions_.end(),
    [reactID] (const std::unique_ptr<Reaction> &p) { return p->id == reactID; });
  assert(it != reactions_.end());
  return (*it)->isOriented;
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


// getReactions returns the underlying list of reactions
const ReactList& ReactionModel::getReactions() const {
  return reactions_;
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
    if (col == ReactCol::ID) {
      return r->id;
    } else if (col == ReactCol::Name) {
      // we return both the name as well as the reaction id since only the
      // latter allows to uniquely identify a reaction (the name is not unique)
      return r->name;
    } else if (col == ReactCol::Orient1) {
      return r->orient1;
    } else if (col == ReactCol::Orient2) {
      return r->orient2;
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

  const auto& r = reactions_[row];
  if (col == ReactCol::ID) {
    r->id = v.toLongLong();
  } else if (col == ReactCol::Name) {
    r->name = v.toString();
  } else if (col == ReactCol::Rate) {
    r->rate = v.toString();
  } else if (col == ReactCol::React1) {
    if (r->reactant1 != nullptr) {
      emit(unuseMol(r->reactant1->id));
    }
    r->reactant1 = static_cast<const Molecule*>(v.value<void *>());
    if (r->reactant1 != nullptr) {
      emit(useMol(r->reactant1->id));
      setOriented(r);
    }
  } else if (col == ReactCol::React2) {
    if (r->reactant2 != nullptr) {
      emit(unuseMol(r->reactant2->id));
    }
    r->reactant2 = static_cast<const Molecule*>(v.value<void *>());
    if (r->reactant2 != nullptr) {
      emit(useMol(r->reactant2->id));
      setOriented(r);
    }
  } else if (col == ReactCol::Orient1) {
    QString orient = v.toString();
    if ((r->isOriented && orient == "") || (!r->isOriented && orient != "")) {
      return false;
    }
    r->orient1 = v.toString();
  } else if (col == ReactCol::Orient2) {
    QString orient = v.toString();
    if ((r->isOriented && orient == "") || (!r->isOriented && orient != "")) {
      return false;
    }
    r->orient2 = v.toString();
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
  if (index.isValid() && (index.column() != ReactCol::ID)) {
    flags |= Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
  }
  return flags;
}


// addReaction adds a new reaction to the model
// NOTE: addReaction assumes that the given reaction is distinct from all
// existing reactions
void ReactionModel::addReaction(const QString& reactName, const QString& rate,
    const Molecule* react1, const Molecule* react2, const ReactType& type,
    const Molecule* prod1) {
  auto r = ReactPtr(new Reaction);
  r->id = reactCount_++;
  r->name = reactName;
  r->rate = rate;
  r->type = type;
  r->isOriented = (react1->type == MolType::SURF) ||
    (react2->type == MolType::SURF) || (prod1->type == MolType::SURF);

  r->reactant1 = react1;
  emit(useMol(react1->id));
  r->orient1 = "";

  r->reactant2 = react2;
  emit(useMol(react2->id));
  r->orient2 = "";

  r->products.push_back(prod1);
  emit(useMol(prod1->id));
  r->prodOrients.push_back("");
  setOriented(r);

  beginResetModel();
  reactions_.push_back(std::move(r));
  endResetModel();
}


// deleteReaction deletes the selected reaction from the model
void ReactionModel::delReaction(qlonglong reactID) {
  auto it = std::find_if(reactions_.begin(), reactions_.end(),
    [reactID] (const std::unique_ptr<Reaction> &p) { return p->id == reactID; });
  assert(it != reactions_.end());

  if ((*it)->reactant1 != nullptr) {
    emit(unuseMol((*it)->reactant1->id));
  }
  if ((*it)->reactant2 != nullptr) {
    emit(unuseMol((*it)->reactant2->id));
  }
  for (auto *m : (*it)->products) {
    if (m != nullptr) {
      emit(unuseMol(m->id));
    }
  }

  beginResetModel();
  reactions_.erase(it);
  endResetModel();
}


// setOriented checks if a reaction should be oriented and then sets isOriented
// appropriately. It also adjust the content of the orient properties to
// properly reflect the orientation.
void setOriented(const ReactPtr& p) {
  if (p->reactant1->type == MolType::SURF || p->reactant2->type == MolType::SURF) {
    if (!p->isOriented) {
      p->orient1 = ";";
      p->orient2 = ";";
      p->isOriented = true;
    }
  } else if (p->isOriented) {
    p->orient1 = "";
    p->orient2 = "";
    p->isOriented = false;
  }
}





