// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <QComboBox>
#include <QDataWidgetMapper>
#include <QLineEdit>

#include "paramWidget.hpp"
#include "noteWarnModel.hpp"
#include "noteWarnWidget.hpp"


// constructor
NoteWarnWidget::NoteWarnWidget(QWidget* parent, Qt::WindowFlags flags) :
  QWidget(parent, flags) {

  setupUi(this);
}

// initModel connects the widget elements to the underlying model
// NOTE: To make the code a bit more compact the interface is created
// programmatically based on the NotificationsModel and WarningsModel
void NoteWarnWidget::initModel(NotificationsModel* noteModel,
  WarningsModel* warnModel) {

  auto keys = noteModel->noteKeyWords;
  auto vals = noteModel->noteValues;
  for (int i=0; i<keys.size(); ++i) {
    QDataWidgetMapper* mapper = new QDataWidgetMapper;
    mapper->setModel(noteModel);
    noteGrid->addWidget(new QLabel(keys[i]), i, 0);

    if (keys[i] == "PROBABILITY_REPORT_THRESHOLD") {
      auto l = new QLineEdit(vals[i][0], this);
      l->setValidator(new QIntValidator);
      noteGrid->addWidget(l, i, 1);
      mapper->addMapping(l, 1);
    } else {
      auto c = new QComboBox(this);
      c->insertItems(0,vals[i]);
      noteGrid->addWidget(c, i, 1);

      auto del = new ComboBoxModelDelegate(this);
      mapper->setItemDelegate(del);
      mapper->addMapping(c, 1);
      connect(c, SIGNAL(currentIndexChanged(int)), del,
        SLOT(onCurrentIndexChanged(int)));
    }
    mapper->setCurrentIndex(i);
  }

  keys = warnModel->warnKeyWords;
  vals = warnModel->warnValues;
  for (int i=0; i<keys.size(); ++i) {
    QDataWidgetMapper* mapper = new QDataWidgetMapper;
    mapper->setModel(warnModel);
    warnGrid->addWidget(new QLabel(keys[i]), i, 0);

    if (keys[i] == "HIGH_PROBABILITY_THRESHOLD" ||
        keys[i] == "LIFETIME_THRESHOLD" ||
        keys[i] == "MISSED_REACTION_THRESHOLD") {
      auto l = new QLineEdit(vals[i][0], this);
      if (keys[i] == "LIFETIME_THRESHOLD") {
        l->setValidator(new QIntValidator);
      } else {
        l->setValidator(new QDoubleValidator);
      }
      warnGrid->addWidget(l, i, 1);
      mapper->addMapping(l, 1);
    } else {
      auto c = new QComboBox(this);
      c->insertItems(0,vals[i]);
      warnGrid->addWidget(c, i, 1);

      auto del = new ComboBoxModelDelegate(this);
      mapper->setItemDelegate(del);
      mapper->addMapping(c, 1);
      connect(c, SIGNAL(currentIndexChanged(int)), del,
        SLOT(onCurrentIndexChanged(int)));
    }
    mapper->setCurrentIndex(i);
  }
}


