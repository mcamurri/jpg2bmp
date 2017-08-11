/* 
 * File:   BmpInfoHeader.h
 * Author: cam
 *
 * Created on March 31, 2010, 12:16 PM
 */
#include "BmpFileHeader.h"

#ifndef _BMPINFOHEADER_H
#define	_BMPINFOHEADER_H

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;

class BmpInfoHeader : public ByteHandler {
public:
    BmpInfoHeader();
    BmpInfoHeader(int width, int height);
    BmpInfoHeader(int width, int height, unsigned char bpp);

private:
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
public:
    bool get(istream & stream);
    void printInfoHeader();
    bool writeHeader(ostream & stream);

    WORD getBiBitCount() const {
        return biBitCount;
    }

    DWORD getBiClrImportant() const {
        return biClrImportant;
    }

    DWORD getBiClrUsed() const {
        return biClrUsed;
    }

    DWORD getBiCompression() const {
        return biCompression;
    }

    LONG getBiHeight() const {
        return biHeight;
    }

    WORD getBiPlanes() const {
        return biPlanes;
    }

    DWORD getBiSize() const {
        return biSize;
    }

    DWORD getBiSizeImage() const {
        return biSizeImage;
    }

    LONG getBiWidth() const {
        return biWidth;
    }

    LONG getBiXPelsPerMeter() const {
        return biXPelsPerMeter;
    }

    LONG getBiYPelsPerMeter() const {
        return biYPelsPerMeter;
    }

    void setBiBitCount(WORD biBitCount) {
        this->biBitCount = biBitCount;
    }

    void setBiHeight(LONG biHeight) {
        this->biHeight = biHeight;
    }

    void setBiSize(DWORD biSize) {
        this->biSize = biSize;
    }

    void setBiSizeImage(DWORD biSizeImage) {
        this->biSizeImage = biSizeImage;
    }

    void setBiWidth(LONG biWidth) {
        this->biWidth = biWidth;
    }

    void setBiXPelsPerMeter(LONG biXPelsPerMeter) {
        this->biXPelsPerMeter = biXPelsPerMeter;
    }

    void setBiYPelsPerMeter(LONG biYPelsPerMeter) {
        this->biYPelsPerMeter = biYPelsPerMeter;
    }

};

#endif	/* _BMPINFOHEADER_H */

