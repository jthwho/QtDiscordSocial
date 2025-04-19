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
class QListWidgetItem;
class DiscordObject;

class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);

    public slots:
        void friendUpdate(quint64 id, const QString &name);
        void friendUpdateAvitar(quint64 id, const QImage &image);

    private:
        DiscordObject   *_discord = nullptr;
        QListWidget     *_friendList = nullptr;

        QListWidgetItem *findById(quint64 id) const;
};

