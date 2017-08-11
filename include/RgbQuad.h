/* 
 * File:   RgbQuad.h
 * Author: cam
 *
 * Created on March 31, 2010, 2:29 PM
 */
#include <ByteHandler.h>

#ifndef _RGBQUAD_H
#define	_RGBQUAD_H

#define RED = 0
#define GREEN = 1
#define BLUE = 2

class RgbQuad : public ByteHandler {
public:
    RgbQuad();
    virtual ~RgbQuad();
private:
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;

public:
    BYTE getBlue() const;
    BYTE getGreen() const;

    BYTE getRed() const;
    BYTE getReserved() const;
    bool get(istream & stream);

    //Metodi setter
    void setColor(int type, int value);
};

#endif	/* _RGBQUAD_H */

