/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2005-24-01
 * Description : normalize image filter.
 *
 * Copyright (C) 2005-2018 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef NORMALIZEFILTER_H
#define NORMALIZEFILTER_H

// Local includes

#include "digikam_export.h"
#include "dimgthreadedfilter.h"
#include "digikam_globals.h"

namespace Digikam
{

class DImg;

class DIGIKAM_EXPORT NormalizeFilter : public DImgThreadedFilter
{

public:
    explicit NormalizeFilter(QObject* const parent = 0);
    NormalizeFilter(DImg* const orgImage, const DImg* const refImage, QObject* const parent=0);
    virtual ~NormalizeFilter();

    static QString          FilterIdentifier()
    {
        return QLatin1String("digikam:NormalizeFilter");
    }

    static QList<int>       SupportedVersions()
    {
        return QList<int>() << 1;
    }

    static int              CurrentVersion()
    {
        return 1;
    }

    static QString          DisplayableName()
    {
        return QString::fromUtf8(I18N_NOOP("Auto Normalize"));
    }

    virtual QString         filterIdentifier() const
    {
        return FilterIdentifier();
    }

    virtual FilterAction    filterAction();

    void                    readParameters(const FilterAction& action);

private:

    void filterImage();
    void normalizeImage();

private:

    struct NormalizeParam
    {
        unsigned short* lut;
        double          min;
        double          max;
    };

    DImg m_refImage;
};

} // namespace Digikam

#endif // NORMALIZEFILTER_H
