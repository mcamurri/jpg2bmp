/* 
 * File:   IFDtag.h
 * Author: Cam
 *
 * Created on 30 marzo 2011, 11.11
 */
#include <fstream>

#ifndef IFD0TAG_H
#define	IFD0TAG_H

using namespace std;
class IFD0tag {
public:
    IFD0tag();
    IFD0tag(fstream &fs, int tiffStart, bool isLittle);
    void printData();

    unsigned int getExifIFDpointer() const {
        return exifIFDpointer;
    }

    unsigned int getGpsInfoIFDpointer() const {
        return gpsInfoIFDpointer;
    }


private:
        // 0th IFD Tag
    string imageDescription; // R*
    string make; // R*
    string model; // R*
    unsigned short int orientation; // R*
    unsigned int xResolution[2]; // M*
    unsigned int yResolution[2]; // M*
    unsigned short int resolutionUnit; // M*
    unsigned short int transferFunction[3*256]; // R
    string software; // O
    string dateTime; // R
    string artist; // O
    unsigned int whitePoint[2][2]; // O
    unsigned int primaryChromaticities[6][2]; // O
    unsigned int yCbCrCoefficients[3][2]; // O
    unsigned short int yCbCrPositioning; // M
    unsigned int referenceBlackWhite[6][2]; // O
    string copyright; // O
    unsigned int exifIFDpointer; // M
    unsigned int gpsInfoIFDpointer; // O

};

#endif	/* IFDTAG_H */

