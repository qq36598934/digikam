/* ============================================================
 * Authors: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 *          Caulier Gilles <caulier dot gilles at kdemail dot net>
 *          Marcel Wiesweg <marcel.wiesweg@gmx.de>
 * Date  : 2003-03-09
 * Description : Comments, Tags, and Rating properties editor
 *
 * Copyright 2003-2005 by Renchi Raju & Gilles Caulier
 * Copyright 2006-2007 by Gilles Caulier & Marcel Wiesweg
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
 
#ifndef IMAGEDESCEDITTAB_H
#define IMAGEDESCEDITTAB_H

// Qt includes.

#include <qwidget.h>
#include <qpixmap.h>

// Local includes.

#include "digikam_export.h"

class QListViewItem;

namespace Digikam
{
class AlbumIconItem;
class Album;
class TAlbum;
class ImageInfo;
class ImageDescEditTabPriv;


class DIGIKAM_EXPORT ImageDescEditTab : public QWidget
{
    Q_OBJECT

public:

    ImageDescEditTab(QWidget *parent, bool navBar=true);
    ~ImageDescEditTab();

    void assignRating(int rating);
    void setItem(ImageInfo *info=0, int itemType=0);
    void populateTags();
    void setFocusToComments();

signals:

    void signalFirstItem(void);
    void signalPrevItem(void);
    void signalNextItem(void);
    void signalLastItem(void);

protected:

    bool eventFilter(QObject *o, QEvent *e);

private:

    void setInfo(ImageInfo *info, int itemType);

    void updateTagsView();
    void updateComments();
    void updateRating();
    void updateDate();
    void updateRecentTags();

    void tagNew(TAlbum* parAlbum, const QString& _title=QString(),
                const QString& _icon=QString());
    void tagEdit(TAlbum* album);
    void tagDelete(TAlbum *album);

    void toggleChildTags(TAlbum *album, bool b);
    void toggleParentTags(TAlbum *album, bool b);

    void setTagThumbnail(TAlbum *album);

private slots:

    void slotApplyAllChanges();
    void slotRevertAllChanges();
    void slotModified();
    void slotRightButtonClicked(QListViewItem *, const QPoint &, int);
    void slotTagsSearchChanged();

    void slotAlbumAdded(Album* a);
    void slotAlbumDeleted(Album* a);
    void slotAlbumIconChanged(Album* a);
    void slotAlbumRenamed(Album* a);
    void slotAlbumsCleared();
    void slotAlbumMoved(TAlbum* tag, TAlbum* newParent);
    
    void slotABCContextMenu();
    void slotGotThumbnailFromIcon(Album *album, const QPixmap& thumbnail);
    void slotThumbnailLost(Album *album);

    void slotImageTagsChanged(Q_LLONG imageId);
    void slotImagesChanged(int albumId);
    void slotImageRatingChanged(Q_LLONG imageId);
    void slotImageDateChanged(Q_LLONG imageId);
    void slotImageCaptionChanged(Q_LLONG imageId);

    void slotRecentTagsMenuActivated(int);
    void slotAssignedTagsToggled(bool);

private:

    ImageDescEditTabPriv* d;

};

}  // NameSpace Digikam
 
#endif  // IMAGEDESCEDITTAB_H
