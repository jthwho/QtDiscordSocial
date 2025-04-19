/**
 * @file      discordobject.h
 * @author    Jason Howard <jth@howardlogic.com>
 * @copyright Howard Logic.  All rights reserved.
 * 
 * See LICENSE file in the project root folder for license information
 */

#pragma once

#include <QObject>
#include "discordpp.h"

class QTimer;

class DiscordObject : public QObject {
    Q_OBJECT
    public:
        discordpp::Client client;

        DiscordObject(QObject *parent = nullptr);

        bool auth();

    signals:
        void ready();

    private slots:
        void housekeeping();

    private:
        QTimer  *_housekeepingTimer = nullptr;
        bool setup();
};

