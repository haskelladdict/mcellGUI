// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <QDebug>

#include <QDataWidgetMapper>
#include <QMetaProperty>
#include <QStringListModel>
#include <QStandardItemModel>

#include "paramWidget.hpp"


// constructor
ParamWidget::ParamWidget(QWidget* parent, Qt::WindowFlags flags) :
  QWidget(parent, flags) {

  setupUi(this);
  iterEntry->setValidator(new QIntValidator);
  timeStepEntry->setValidator(new QDoubleValidator);
  surfGridEdit->setValidator(new QIntValidator);
}


// setModel connects the widget elements to the underlying model
void ParamWidget::setModel(QStandardItemModel* paramModel) {

  QDataWidgetMapper* iterMapper = new QDataWidgetMapper;
  iterMapper->setModel(paramModel);
  iterMapper->addMapping(iterEntry, 1);
  iterMapper->toFirst();

  QDataWidgetMapper* dtMapper = new QDataWidgetMapper;
  dtMapper->setModel(paramModel);
  dtMapper->addMapping(timeStepEntry, 1);
  dtMapper->setCurrentIndex(1);

  QDataWidgetMapper* surfGridMapper = new QDataWidgetMapper;
  surfGridMapper->setModel(paramModel);
  surfGridMapper->addMapping(surfGridEdit, 1);
  surfGridMapper->setCurrentIndex(2);

  QDataWidgetMapper* acc3DMapper = new QDataWidgetMapper;
  acc3DMapper->setModel(paramModel);
  auto acc3DDel = new ParamModelDelegate(this);
  acc3DMapper->setItemDelegate(acc3DDel);
  acc3DMapper->addMapping(acc3DCombo, 1);
  acc3DMapper->setCurrentIndex(3);
  connect(acc3DCombo, SIGNAL(currentIndexChanged(int)), acc3DDel,
    SLOT(onCurrentIndexChanged(int)));

  QDataWidgetMapper* centerMolMapper = new QDataWidgetMapper;
  centerMolMapper->setModel(paramModel);
  auto centerMolDel = new ParamModelDelegate(this);
  centerMolMapper->setItemDelegate(centerMolDel);
  centerMolMapper->addMapping(centerMolCombo, 1);
  centerMolMapper->setCurrentIndex(4);
  connect(centerMolCombo, SIGNAL(currentIndexChanged(int)), centerMolDel,
    SLOT(onCurrentIndexChanged(int)));

  QDataWidgetMapper* microRevMapper = new QDataWidgetMapper;
  microRevMapper->setModel(paramModel);
  auto microRevDel = new ParamModelDelegate(this);
  microRevMapper->setItemDelegate(microRevDel);
  microRevMapper->addMapping(microRevCombo, 1);
  microRevMapper->setCurrentIndex(5);
  connect(microRevCombo, SIGNAL(currentIndexChanged(int)), microRevDel,
    SLOT(onCurrentIndexChanged(int)));
}


// ParamModelDelegate constructor
ParamModelDelegate::ParamModelDelegate(QObject *parent) :
  QItemDelegate(parent) {}


// setEditorData reads the data and writes it to the editor
void ParamModelDelegate::setEditorData(QWidget* editor,
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
void ParamModelDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
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
void ParamModelDelegate::onCurrentIndexChanged(int i) {
  Q_UNUSED(i);
  QComboBox* cb = static_cast<QComboBox*>(sender());
  if(cb) {
    emit commitData(cb);
  }
}






