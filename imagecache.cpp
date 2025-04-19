/**
 * @file      imagecache.cpp
 * @author    Jason Howard <jth@aja.com>
 * @copyright AJA Video.  All rights reserved.
 * 
 * See LICENSE file in the project root folder for license information
 */

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImageReader>
#include "imagecache.h"


ImageCache::ImageCache(QObject *p) :
    QObject(p),
    _nam(new QNetworkAccessManager(this))
{

}


void ImageCache::loadImage(const KeyType &key, const QUrl &url) {
    QNetworkRequest request(url);
    QNetworkReply* reply = _nam->get(request);
    connect(reply, &QNetworkReply::finished, this, 
        [this, reply, key]() {
            if(reply->error() == QNetworkReply::NoError) {
                QByteArray imageData = reply->readAll();
                QImage image;
                image.loadFromData(imageData);
                if(!image.isNull()) {
                    _hash[key] = image;
                    emit imageLoaded(key, image);
                } else {
                    qWarning("Failed to decode image");
                }
            } else {
                qWarning("Request failed: %s", qPrintable(reply->errorString()));
            }
            reply->deleteLater();
    });    
    return;
}


