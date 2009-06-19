/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2003-02-03
 * Description : Cameras list container
 *
 * Copyright (C) 2003-2005 by Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Copyright (C) 2006-2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "cameralist.h"
#include "cameralist.moc"

// Qt includes

#include <QDateTime>
#include <QString>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>
#include <QTextCodec>

// KDE includes

#include <kmessagebox.h>
#include <klocale.h>

// Local includes

#include "gpcamera.h"
#include "cameratype.h"

namespace Digikam
{

CameraList* CameraList::m_defaultList = 0;

CameraList* CameraList::defaultList()
{
    return m_defaultList;
}

class CameraListPrivate
{
public:

    CameraListPrivate()
    {
        modified = false;
    }

    bool               modified;

    QList<CameraType*> clist;
    QString            file;
};

CameraList::CameraList(QObject *parent, const QString& file)
          : QObject(parent), d(new CameraListPrivate)
{
    d->file = file;
    if (!m_defaultList)
        m_defaultList = this;
}

CameraList::~CameraList()
{
    save();
    clear();
    delete d;

    if (m_defaultList == this)
        m_defaultList = 0;
}

bool CameraList::load()
{
    d->modified = false;

    QFile cfile(d->file);

    if (!cfile.open(QIODevice::ReadOnly))
        return false;

    QDomDocument doc("cameralist");
    if (!doc.setContent(&cfile))
        return false;

    QDomElement docElem = doc.documentElement();
    if (docElem.tagName()!="cameralist")
        return false;

    for (QDomNode n = docElem.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement e = n.toElement();
        if (e.isNull()) continue;
        if (e.tagName() != "item") continue;

        QString title     = e.attribute("title");
        QString model     = e.attribute("model");
        QString port      = e.attribute("port");
        QString path      = e.attribute("path");
        int sn            = e.attribute("startingnumber").toInt();
        CameraType *ctype = new CameraType(title, model, port, path, sn);
        insertPrivate(ctype);
    }

    return true;
}

bool CameraList::save()
{
    // If not modified don't save the file
    if (!d->modified)
        return true;

    QDomDocument doc("cameralist");
    doc.setContent(QString("<!DOCTYPE XMLCameraList><cameralist version=\"1.2\" client=\"digikam\"/>"));

    QDomElement docElem=doc.documentElement();

    foreach (CameraType *ctype, d->clist)
    {
       QDomElement elem = doc.createElement("item");
       elem.setAttribute("title",          ctype->title());
       elem.setAttribute("model",          ctype->model());
       elem.setAttribute("port",           ctype->port());
       elem.setAttribute("path",           ctype->path());
       elem.setAttribute("startingnumber", QString::number(ctype->startingNumber()));
       docElem.appendChild(elem);
    }

    QFile cfile(d->file);
    if (!cfile.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(&cfile);
    stream.setCodec(QTextCodec::codecForName("UTF-8"));
    stream.setAutoDetectUnicode(true);
    stream << doc.toString();
    cfile.close();

    return true;
}

void CameraList::insert(CameraType* ctype)
{
    if (!ctype) return;

    d->modified = true;
    insertPrivate(ctype);
}

void CameraList::remove(CameraType* ctype)
{
    if (!ctype) return;

    d->modified = true;
    removePrivate(ctype);
}

void CameraList::insertPrivate(CameraType* ctype)
{
    if (!ctype) return;
    d->clist.append(ctype);
    emit signalCameraAdded(ctype);
}

void CameraList::removePrivate(CameraType* ctype)
{
    if (!ctype) return;

    emit signalCameraRemoved(ctype->action());

    int i = d->clist.indexOf(ctype);
    if (i != -1)
        delete d->clist.takeAt(i);
}

void CameraList::clear()
{
    while (!d->clist.isEmpty())
        removePrivate( d->clist.first());
}

QList<CameraType*>* CameraList::cameraList()
{
    return &d->clist;
}

CameraType* CameraList::find(const QString& title)
{
    foreach (CameraType *ctype, d->clist)
    {
        if (ctype->title() == title)
            return ctype;
    }
    return 0;
}

CameraType* CameraList::autoDetect(bool& retry)
{
    retry = false;

    QString model, port;
    if (GPCamera::autoDetect(model, port) != 0)
    {
        retry = ( KMessageBox::warningYesNo(0, i18n("Failed to auto-detect camera; "
                                                    "please make sure it is connected "
                                                    "properly and is turned on. "
                                                    "Would you like to try again?"))
                 == KMessageBox::Yes );
        return 0;
    }

    // Check if the camera is already in the list
    foreach (CameraType *ctype, d->clist)
    {
        // We can get away with checking only the model, as the auto-detection
        // works only for usb cameras. so the port is always usb:
        if (ctype->model() == model)
            return ctype;
    }

    // Looks like a new camera

    // NOTE: libgphoto2 now (2.1.4+) expects port names to be
    // something like "usb:001,012". but on linux these port numbers
    // will change every time camera is reconnected. gphoto port funcs
    // also allow regexp match, so the safe bet is to just pass in
    // "usb:" and cross your fingers that user doesn't have multiple cameras
    // connected at the same time (whack them if they do).

    if (port.startsWith(QLatin1String("usb:")))
        port = "usb:";

    CameraType* ctype = new CameraType(model, model, port, "/", 1);
    insert(ctype);

    return ctype;
}

bool CameraList::findConnectedCamera(int vendorId, int productId, QString& model, QString& port)
{
    return GPCamera::findConnectedUsbCamera(vendorId, productId, model, port);
}

bool CameraList::changeCameraStartIndex(const QString& cameraTitle, int startIndex)
{
    CameraType* cam = find(cameraTitle);
    if (cam)
    {
        cam->setStartingNumber(startIndex);
        d->modified = true;
        save();
        return true;
    }
    return false;
}

}  // namespace Digikam
