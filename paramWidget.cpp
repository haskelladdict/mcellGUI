// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <QDebug>

#include <QDataWidgetMapper>

#include "paramWidget.hpp"


// constructor
ParamWidget::ParamWidget(QWidget* parent, Qt::WindowFlags flags) :
  QWidget(parent, flags) {

  setupUi(this);
  //molTableView->setItemDelegate(&delegate_);
  iterEntry->setValidator(new QIntValidator);
  timeStepEntry->setValidator(new QDoubleValidator);
  surfGridEdit->setValidator(new QIntValidator);
}


// initModel initializes the widget's underlying molecule model
void ParamWidget::setModel(QStandardItemModel* paramModel) {
  paramModel_ = paramModel;

  QDataWidgetMapper* iterMapper = new QDataWidgetMapper;
  iterMapper->setModel(paramModel_);
  iterMapper->addMapping(iterEntry,1);
  iterMapper->toFirst();

  QDataWidgetMapper* dtMapper = new QDataWidgetMapper;
  dtMapper->setModel(paramModel_);
  dtMapper->addMapping(timeStepEntry, 1);
  dtMapper->setCurrentIndex(1);

  QDataWidgetMapper* surfGridMapper = new QDataWidgetMapper;
  surfGridMapper->setModel(paramModel_);
  dtMapper->addMapping(surfGridEdit, 1);
  dtMapper->setCurrentIndex(2);

  QDataWidgetMapper* acc3DMapper = new QDataWidgetMapper;
  acc3DMapper->setModel(paramModel_);
  acc3DMapper->addMapping(acc3DCombo, 1);
  acc3DMapper->setCurrentIndex(3);

  QDataWidgetMapper* centerMolMapper = new QDataWidgetMapper;
  centerMolMapper->setModel(paramModel_);
  centerMolMapper->addMapping(centerMolCombo, 1);
  centerMolMapper->setCurrentIndex(4);

  QDataWidgetMapper* microRevMapper = new QDataWidgetMapper;
  microRevMapper->setModel(paramModel_);
  microRevMapper->addMapping(microRevCombo, 1);
  microRevMapper->setCurrentIndex(5);
}
