// Copyright 2015 Markus Dittrich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// mcellGUI is a simulation GUI for MCell (www.mcell.org)

#ifndef REACTION_WIDGET_HPP
#define REACTION_WIDGET_HPP

#include <QItemDelegate>
#include <QSortFilterProxyModel>
#include <QWidget>

#include "reactionModel.hpp"
#include "ui_reactionWidget.h"

#if 0
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
#endif

// MolWidget is the main widget for handling molecule data
class ReactionWidget : public QWidget, Ui::ReactionWidget {

  Q_OBJECT

public:
  ReactionWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);

  void initModel(ReactionModel* model);

private:

  int reactCount_ = 0;
  ReactionModel* model_;
//  ReactModelDelegate delegate_;

private slots:
  void addReaction();
  void deleteReactions();
};






























#endif
