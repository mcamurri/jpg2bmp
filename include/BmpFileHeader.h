/* 
 * File:   bmpFileHeader.h
 * Author: cam
 *
 * Created on March 31, 2010, 9:12 AM
 */
#include <fstream>
#include <iostream>
#include <iomanip>
#include "ByteHandler.h"
using namespace std;

#ifndef _BMPFILEHEADER_H
#define	_BMPFILEHEADER_H

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;

class BmpFileHeader : public ByteHandler {
public:
    BmpFileHeader();
    BmpFileHeader(int width, int height);
    BmpFileHeader(int width, int height, unsigned short int bpp);
private:
    WORD bfType;
    DWORD bfSize;
    DWORD bfReserved;
    DWORD bfOffBits;
public:
    bool get(istream & stream);
    void printFileHeader();
    bool writeHeader(ostream &stream);


    //Metodi getter
    WORD getBfType() const;
    DWORD getBfSize() const;

    DWORD getBfOffBits() const {
        return bfOffBits;
    }

};

#endif	/* _BMPFILEHEADER_H */

