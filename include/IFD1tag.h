/* 
 * File:   IFDtag.h
 * Author: Cam
 *
 * Created on 30 marzo 2011, 11.11
 */
#include <fstream>

#ifndef IFD1TAG_H
#define	IFD1TAG_H

using namespace std;
class IFD1tag {
public:
    IFD1tag();
    IFD1tag(fstream &fs, int tiffStart, bool isLittle);
    void printData();
    unsigned int GetJpegInterchangeFormat() const {
        return jpegInterchangeFormat;
    }

    unsigned int GetJpegInterchangeFormatLength() const {
        return jpegInterchangeFormatLength;
    }



private:
        // 1th IFD Tag
    unsigned short int compression; //M*


    unsigned int xResolution[2]; // M*
    unsigned int yResolution[2]; // M*
    unsigned short int resolutionUnit; // M*

    unsigned int jpegInterchangeFormat; // M
    unsigned int jpegInterchangeFormatLength; // M

};

#endif	/* IFDTAG_H */

