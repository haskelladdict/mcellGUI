// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef MOL_WIDGET_HPP
#define MOL_WIDGET_HPP

#include <QItemDelegate>
#include <QSortFilterProxyModel>
#include <QWidget>

#include "molModel.hpp"
#include "ui_molWidget.h"


// MolModelDelegate defines custom delegates for the MolModel view
class MolModelDelegate : public QItemDelegate {

    Q_OBJECT

public:
    MolModelDelegate(QWidget *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel* model,
      const QModelIndex& index) const;
};


// MolWidget is the main widget for handling molecule data
class MolWidget : public QWidget, Ui::MolWidget {

  Q_OBJECT

public:
  MolWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);

  void initModel(MolModel* model);

private:

  MolModel* model_;
  MolModelDelegate delegate_;

private slots:
  void addMol();
  void clearSelection();
  void deleteMols();
};






























#endif
