// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef PARAM_WIDGET_HPP
#define PARAM_WIDGET_HPP

#include <QItemDelegate>
#include <QWidget>

#include "ui_paramWidget.h"

class ParamModel;

// ParamModelDelegate defines a custom delegate between combo-boxes and the model
class ComboBoxModelDelegate : public QItemDelegate {

    Q_OBJECT

public:
    ComboBoxModelDelegate(QObject* parent = 0);

    void setEditorData(QWidget* editor, const QModelIndex &index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
      const QModelIndex & index ) const;

private slots:
    void onCurrentIndexChanged(int i);
};


// MolWidget is the main widget for handling molecule data
class ParamWidget : public QWidget, Ui::ParamWidget {

  Q_OBJECT

public:
  ParamWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);

  void initModel(ParamModel* model);
};


#endif
