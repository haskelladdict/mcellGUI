// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <QDebug>

#include <QComboBox>
#include <QDataWidgetMapper>
#include <QMetaProperty>
#include <QStringListModel>
#include <QStandardItemModel>

#include "paramModel.hpp"
#include "paramWidget.hpp"


// constructor
ParamWidget::ParamWidget(QWidget* parent, Qt::WindowFlags flags) :
  QWidget(parent, flags) {

  setupUi(this);
  advancedGrouper->setHidden(true);
  timeStepEntry->setValidator(new QDoubleValidator);
  //surfGridEdit->setValidator(new QIntValidator);
}


// setModel connects the widget elements to the underlying model
void ParamWidget::setModel(ParamModel* paramModel) {

  QDataWidgetMapper* iterMapper = new QDataWidgetMapper;
  iterMapper->setModel(paramModel);
  iterMapper->addMapping(iterEntry, 1);
  iterMapper->toFirst();

  QDataWidgetMapper* dtMapper = new QDataWidgetMapper;
  dtMapper->setModel(paramModel);
  dtMapper->addMapping(timeStepEntry, 1);
  dtMapper->setCurrentIndex(1);

  auto keys = paramModel->advKeyWords;
  auto vals = paramModel->values;
  int id = 2;
  for (int i=0; i<keys.size(); ++i) {

    QDataWidgetMapper* mapper = new QDataWidgetMapper;
    mapper->setModel(paramModel);
    paramGrid->addWidget(new QLabel(keys[i]), i, 0);

    if (keys[i] == "SURFACE_GRID_DENSITY") {
      auto l = new QLineEdit(vals[i][0], this);
      l->setValidator(new QIntValidator);
      paramGrid->addWidget(l, i, 1);
      mapper->addMapping(l, 1);
    } else {
      auto c = new QComboBox(this);
      c->insertItems(0,vals[i]);
      paramGrid->addWidget(c, i, 1);

      auto del = new ComboBoxModelDelegate(this);
      mapper->setItemDelegate(del);
      mapper->addMapping(c, 1);
      connect(c, SIGNAL(currentIndexChanged(int)), del,
        SLOT(onCurrentIndexChanged(int)));
    }
    mapper->setCurrentIndex(id++);
  }
}


// ComboBoxModelDelegate constructor
ComboBoxModelDelegate::ComboBoxModelDelegate(QObject *parent) :
  QItemDelegate(parent) {}


// setEditorData reads the data and writes it to the editor
void ComboBoxModelDelegate::setEditorData(QWidget* editor,
  const QModelIndex& index) const {
  if (!editor->metaObject()->userProperty().isValid()) {
    if (editor->property("currentText").isValid()) {
      editor->setProperty("currentText", index.data());
      return;
    }
  }
  QItemDelegate::setEditorData(editor, index);
}

// setModelData writes the data contained in the editor to the model
void ComboBoxModelDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
  const QModelIndex& index ) const {
  if (!editor->metaObject()->userProperty().isValid()) {
    QVariant value = editor->property("currentText");
    if (value.isValid()) {
      model->setData(index, value);
      return;
    }
  }
  QItemDelegate::setModelData(editor, model, index);
}


// onCurrentIndexChanged triggers a commitData signal if the sender was
// a combo-box
void ComboBoxModelDelegate::onCurrentIndexChanged(int i) {
  Q_UNUSED(i);
  QComboBox* cb = static_cast<QComboBox*>(sender());
  if(cb) {
    emit commitData(cb);
  }
}






