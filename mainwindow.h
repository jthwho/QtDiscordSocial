/**
 * @file      mainwindow.h
 * @author    Jason Howard <jth@howardlogic.com>
 * @copyright Howard Logic.  All rights reserved.
 * 
 * See LICENSE file in the project root folder for license information
 */

#pragma once

#include <QMainWindow>

class QListWidget;
class DiscordObject;

class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);

    public slots:
        void updateFriendList();

    private:
        DiscordObject   *_discord = nullptr;
        QListWidget     *_friendList = nullptr;
};

