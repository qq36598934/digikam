/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2005-05-06
 * Description : Albums folder view.
 *
 * Copyright (C) 2005-2006 by Joern Ahrens <joern dot ahrens at kdemail dot net>
 * Copyright (C) 2006-2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2009 by Andi Clemens <andi dot clemens at gmx dot net>
 * Copyright (C) 2009 by Johannes Wienke <languitar at semipol dot de>
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

#ifndef ALBUMSELECTIONTREEVIEW_H
#define ALBUMSELECTIONTREEVIEW_H

// QT includes
#include <qtreeview.h>

// Local includes
#include "albummodel.h"
#include "albumtreeview.h"
#include "albummodificationhelper.h"

namespace Digikam {

class AlbumSelectionTreeViewPriv;

/**
 * Album tree view used in the left sidebar to select PAlbums and perform
 * operations on them via a context menu.
 *
 * @author jwienke
 */
class AlbumSelectionTreeView: public AlbumTreeView
{
Q_OBJECT
public:
    AlbumSelectionTreeView(QWidget *parent, AlbumModel *model, AlbumModificationHelper *albumModificationHelper);
    ~AlbumSelectionTreeView();

    /**
     * Sets whether this widget shall display tool tips or not.
     */
    void setEnableToolTips(bool enable);

Q_SIGNALS:

    /**
     * Emitted if a find duplicates search shall be invoked on the given album.
     *
     * @param album the album to find duplicates in
     */
    void signalFindDuplicatesInAlbum(Album *album);

protected:

    virtual QString contextMenuTitle() const;
    virtual void addCustomContextMenuActions(ContextMenuHelper &cmh, Album *album);
    virtual void handleCustomContextMenuAction(QAction *action, AlbumPointer<Album> album);

private:

    /**
     * Re-implemented to handle custom tool tips.
     *
     * @param event event to process.
     */
    bool viewportEvent(QEvent *event);

private:
    AlbumSelectionTreeViewPriv *d;

};

}  // namespace Digikam

#endif // ALBUMSELECTIONTREEVIEW_H
