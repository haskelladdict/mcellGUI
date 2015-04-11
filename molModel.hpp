// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef MOL_MODEL_HPP
#define MOL_MODEL_HPP

#include <map>
#include <memory>
#include <vector>

#include <QAbstractTableModel>
#include <QString>

// MolType classifies 2D (SURF) or 3D (VOL) molecules
enum class MolType {SURF, VOL};

// Molecule class stores the data for a single molecule
struct Molecule {
  QString name;
  QString D;
  MolType type;
};
using MolList = std::vector<std::unique_ptr<Molecule>>;

// Col names column types (one per data element in Molecule)
namespace Col {
  enum col {Name, D, Type};
}

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
  Qt::ItemFlags flags(const QModelIndex& index) const;

  bool haveMol(const QString& molName) const;
  int numMols() const;
  const MolList& getMols() const;
  const Molecule* getMolecule(QString name) const;
  QStringList getMolNames() const;

  // write methods
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
  void addMol(const QString& name, const QString& D, const MolType& type);
  void delMol(const QString& name);


private:
  MolList mols_;

  std::vector<QString> headerLabels_ = {"molecule name", "D", "type"};
};

#endif
