/* ============================================================
 * File  : imagelevels.h
 * Author: Gilles Caulier <caulier dot gilles at free.fr>
 * Date  : 2004-07-29
 * Description : image levels manipulation methods.
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
 
#ifndef IMAGELEVELS_H
#define IMAGELEVELS_H

#define CLAMP(x,l,u) ((x)<(l)?(l):((x)>(u)?(u):(x)))

/*  Map RGB to intensity  */

#define GIMP_RGB_INTENSITY_RED    0.30
#define GIMP_RGB_INTENSITY_GREEN  0.59
#define GIMP_RGB_INTENSITY_BLUE   0.11
#define GIMP_RGB_INTENSITY(r,g,b) ((r) * GIMP_RGB_INTENSITY_RED   + \
                                   (g) * GIMP_RGB_INTENSITY_GREEN + \
                                   (b) * GIMP_RGB_INTENSITY_BLUE)
                                   
namespace Digikam
{

class ImageHistogram;

class ImageLevels
{

private:

enum PixelType
{
    RedPixel = 0,  
    GreenPixel,
    BluePixel, 
    AlphaPixel
};

struct _Levels
{
    double gamma[5];
    
    int    low_input[5];
    int    high_input[5];
    
    int    low_output[5];
    int    high_output[5];
    
    uchar  input[5][256]; // this is used only by the gui (Renchi, what's that ???).
};

struct _Lut
{
    uchar **luts;
    int     nchannels;
};

public:
    
    ImageLevels();
    ~ImageLevels();

    // Methods for to manipulate the levels data.        
    void  levelsAuto(Digikam::ImageHistogram *hist);
    void  levelsChannelAuto(Digikam::ImageHistogram *hist, int channel);
    int   levelsInputFromColor(int channel, uchar *color);    
    void  levelsAdjustByColors(int channel, uchar *black, uchar *gray, uchar *white);
    void  levelsCalculateTransfers();
    float levelsLutFunc(int n_channels, int channel, float value);
    void  levelsLutSetup(int nchannels);
    
private:

    // Levels data.
    struct _Levels *m_levels;
    
    // Lut data.
    struct _Lut *m_lut;
     
    void levelsChannelReset(int channel);
};

}  // NameSpace Digikam


#endif /* IMAGELEVELS_H */
