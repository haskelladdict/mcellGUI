// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#include <QApplication>

#include "mainWindow.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  MainWindow *mainWindow = new MainWindow;

  mainWindow->show();
  return app.exec();
}
