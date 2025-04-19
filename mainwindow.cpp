/**
 * @file      mainwindow.cpp
 * @author    Jason Howard <jth@howardlogic.com>
 * @copyright Howard Logic.  All rights reserved.
 * 
 * See LICENSE file in the project root folder for license information
 */

#include <QListWidget>

#include "mainwindow.h"
#include "discordobject.h"

MainWindow::MainWindow(QWidget *p) :
    QMainWindow(p)
{
    _friendList = new QListWidget(this);
    setCentralWidget(_friendList);

    _discord = new DiscordObject(this);
    connect(_discord, &DiscordObject::ready, this, &MainWindow::updateFriendList);
    _discord->auth();
}

void MainWindow::updateFriendList() {
    auto list = _discord->client.GetRelationships();
    for(const auto &i : list) {
        _friendList->addItem(QString::fromStdString(i.User().value().DisplayName()));
    }
    return;
}

