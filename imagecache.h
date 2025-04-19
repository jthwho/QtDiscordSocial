/**
 * @file      imagecache.h
 * @author    Jason Howard <jth@aja.com>
 * @copyright AJA Video.  All rights reserved.
 * 
 * See LICENSE file in the project root folder for license information
 */

#pragma once

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QImage>
#include <QUrl>

class QNetworkAccessManager;

class ImageCache : public QObject {
    Q_OBJECT
    public:
        using KeyType = uint64_t;
        ImageCache(QObject *parent = nullptr);

        QImage getImage(const KeyType &key) const {
            return _hash[key];
        }

    public slots:
        void loadImage(const KeyType &key, const QUrl &url);

    signals:
        void imageLoaded(const KeyType &key, const QImage &image);

    private:
        using ImageHash = QHash<KeyType, QImage>;

        QNetworkAccessManager   *_nam = nullptr;
        ImageHash               _hash;
};


