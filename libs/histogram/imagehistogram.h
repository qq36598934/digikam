/* ============================================================
 * File  : imagehistogram.h
 * Author: Gilles Caulier <caulier dot gilles at free.fr>
 * Date  : 2004-07-21
 * Description : image histogram manipulation methods.
 * 
 * Copyright 2004 by Gilles Caulier
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
 
 
#ifndef IMAGEHISTOGRAM_H
#define IMAGEHISTOGRAM_H

namespace Digikam
{

class ImageHistogram
{

public:

enum HistogramChannelType
{
    ValueChannel = 0,    
    RedChannel,  
    GreenChannel,
    BlueChannel, 
    AlphaChannel
};

private:

// Using a structure instead a class is more fast (access with memset() and bytes manipulation).
struct double_packet
{
    double value;
    double red;
    double green;
    double blue;
    double alpha;
};

public:
    
    ImageHistogram(uint *i_data, uint i_w, uint i_h);
    ~ImageHistogram();

    // Methods for to manipulate the histogram data.        
    double getCount(int channel, int start, int end);
    double getMean(int channel, int start, int end);
    double getPixels();
    int    getMedian(int channel, int start, int end);
    double getStdDev(int channel, int start, int end);
    double getValue(int channel, int bin);
    double getMaximum(int channel);

private:

    // The histogram data.
    struct double_packet *m_histogram;

    // Image informations.
    uint  *m_imageData;
    uint   m_imageWidth;
    uint   m_imageHeight;
    
    bool calcHistogramValues();       
};

}  // NameSpace Digikam

#endif /* IMAGEHISTOGRAM_H */
