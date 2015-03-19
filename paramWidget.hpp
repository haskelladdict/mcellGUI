// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef PARAM_WIDGET_HPP
#define PARAM_WIDGET_HPP

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QWidget>

#include "ui_paramWidget.h"


/*
// MolModelDelegate defines custom delegates for the MolModel view
class MolModelDelegate : public QItemDelegate {

    Q_OBJECT

public:
    MolModelDelegate(QWidget *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
};
*/

// MolWidget is the main widget for handling molecule data
class ParamWidget : public QWidget, Ui::ParamWidget {

  Q_OBJECT

public:
  ParamWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);

  void setModel(QStandardItemModel* model);


private:

  QStandardItemModel* paramModel_;
  //MolModelDelegate delegate_;

private slots:
};


#endif
