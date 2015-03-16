// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef MOL_MODEL_HPP
#define MOL_MODEL_HPP

#include <vector>

#include <QString>

#include <QAbstractTableModel>

// MolType classifies 2D (SURF) or 3D (VOL) molecules
enum class MolType {SURF, VOL};

// Molecule is a simple storage class containing molecule properties
struct Molecule {
  QString name;
  QString D;
  MolType type;
};

// MolModel describes the QT MVC data model for molecules
class MolModel : public QAbstractTableModel {

  Q_OBJECT

public:
  MolModel(QObject* parent = 0);

  int rowCount(const QModelIndex& parent = QModelIndex()) const ;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;

  // read methods
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  const Molecule& getMol(int row) const;
  void delMol(int rowID);

  // write methods
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
  Qt::ItemFlags flags(const QModelIndex& index) const;

private:
  std::vector<Molecule> mols_;
  std::vector<QString> labels_ = {"molecule name", "D", "type"};
};
























#endif
