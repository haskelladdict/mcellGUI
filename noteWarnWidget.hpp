// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef NOTE_WARN_WIDGET_HPP
#define NOTE_WARN_WIDGET_HPP

#include <QWidget>

#include "ui_noteWarnWidget.h"

// NoteWarnWidget is responsible for setting notification and error
// reporting options
class NoteWarnWidget : public QWidget, Ui::NoteWarnWidget {

public:
  NoteWarnWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);

  void initModel(NotificationsModel* note, WarningsModel* warn);
};


#endif
