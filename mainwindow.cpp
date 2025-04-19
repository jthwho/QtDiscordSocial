/**
 * @file      mainwindow.cpp
 * @author    Jason Howard <jth@howardlogic.com>
 * @copyright Howard Logic.  All rights reserved.
 * 
 * See LICENSE file in the project root folder for license information
 */

#include <QListWidget>
#include <QPixmap>

#include "mainwindow.h"
#include "discordobject.h"

#define ROLE_ID static_cast<Qt::ItemDataRole>(Qt::UserRole + 0)

MainWindow::MainWindow(QWidget *p) :
    QMainWindow(p)
{
    _friendList = new QListWidget(this);
    setCentralWidget(_friendList);

    _discord = new DiscordObject(this);
    connect(_discord, &DiscordObject::userAdded, this, &MainWindow::friendUpdate);
    connect(_discord, &DiscordObject::userAvitarChanged, this, &MainWindow::friendUpdateAvitar);
    _discord->auth();
}

QListWidgetItem *MainWindow::findById(quint64 id) const {
    for(int i = 0; i < _friendList->count(); i++) {
        QListWidgetItem *item = _friendList->item(i);
        if(id == item->data(ROLE_ID).toULongLong()) return item;
    }
    return nullptr;
}

void MainWindow::friendUpdate(quint64 id, const QString &name) {
    QListWidgetItem *item = findById(id);
    if(item == nullptr) {
        item = new QListWidgetItem;
        item->setData(ROLE_ID, id);
        _friendList->addItem(item);
    }
    item->setData(Qt::DisplayRole, name);
    return;
}

void MainWindow::friendUpdateAvitar(quint64 id, const QImage &img) {
    QListWidgetItem *item = findById(id);
    if(item == nullptr) {
        qWarning("Got avitar update for %llu, but user not in friend list", (unsigned long long)id);
        return;
    }
    QImage scaled = img.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    item->setData(Qt::DecorationRole, QPixmap::fromImage(scaled));
    return;
}

