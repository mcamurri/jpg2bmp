/* 
 * File:   RgbQuad.cpp
 * Author: cam
 * 
 * Created on March 31, 2010, 2:29 PM
 */

#include <RgbQuad.h>

RgbQuad::RgbQuad() {
    rgbRed = 0;
    rgbGreen = 0;
    rgbBlue = 0;
    rgbReserved = 0;
}

RgbQuad::~RgbQuad() {
}

bool RgbQuad::get(istream & stream) {
    if (!ByteHandler::get(stream, rgbBlue)) return false;
    if (!ByteHandler::get(stream, rgbGreen)) return false;
    if (!ByteHandler::get(stream, rgbRed)) return false;
    if (!ByteHandler::get(stream, rgbReserved)) return false;
    return true;
}

BYTE RgbQuad::getBlue() const{
    return rgbBlue;
}

BYTE RgbQuad::getGreen() const{
    return rgbGreen;
}

BYTE RgbQuad::getRed() const{
    return rgbRed;
}

BYTE RgbQuad::getReserved() const{
    return rgbReserved;
}

void RgbQuad::setColor(int type, int value) {
    switch (type) {
        case 0:
            rgbRed = value;
            break;
        case 1:
            rgbGreen = value;
            break;
        case 2:
            rgbBlue = value;
            break;
    }
}
