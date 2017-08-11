/* 
 * File:   EXIFtag.h
 * Author: Cam
 *
 * Created on 30 marzo 2011, 11.18
 */
#include <fstream>

#ifndef EXIFTAG_H
#define	EXIFTAG_H

using namespace std;

class EXIFtag {
public:
    EXIFtag();
    EXIFtag(fstream &fs, int tiffStart, bool isLittle);
    void printData();
private:
    unsigned short int* fieldsToShow;
    unsigned short int fieldNumber;


    unsigned int exposureTime[2];
    unsigned int fNumber[2];
    unsigned short int exposureProgram;
    string spectralSensivity;
    unsigned short int isoSpeedRatings;
    string oecf;
    char exifVersion[5];
    string dateTimeOriginal;
    string dateTimeDigitized;
    unsigned char componentsConfiguration[4];
    unsigned int compressedBitsPerPixel[2];
    int shutterSpeedValue[2];
    unsigned int apertureValue[2];
    int brightnessValue[2];
    int exposureBiasValue[2];
    unsigned int maxApertureValue[2];
    unsigned int subjectDistance[2];
    unsigned short int meteringMode;
    unsigned short int lightSource;
    unsigned short int flash;
    unsigned int focalLength[2];
    unsigned short int subjectArea;
    unsigned char makerNote[];
    string userComment;
    string subSecTime;
    string subSecTimeOriginal;
    string subSecTimeDigitized;
    char flashpixVersion[5];
    unsigned short int colorSpace;
    unsigned int pixelXdimension;
    unsigned int pixelYdimension;
    string relatedSoundFile;
    // interoperabilityIDFpointer
    unsigned int flashEnergy[2];
    string spatialFrequencyResponse;
    unsigned int focalPlaneXresolution[2];
    unsigned int focalPlaneYresolution[2];
    unsigned short int focalPlaneResolutionUnit;
    unsigned short int subjectLocation;
    unsigned int exposureIndex[2];
    unsigned short int sensingMethod;
    string fileSource;
    string sceneType;
    string CFApattern;
    unsigned short int customRendered;
    unsigned short int exposureMode;
    unsigned short int whiteBalance;
    unsigned int digitalZoomRatio[2];
    unsigned short int focalLengthIn35mmFilm;
    unsigned short int sceneCaptureType;
    unsigned int gainControl[2];
    unsigned short int contrast;
    unsigned short int saturation;
    unsigned short int sharpness;
    string deviceSettingDescription;
    unsigned short int subjectDistanceRange;
    string imageUniqueID;
};

#endif	/* EXIFTAG_H */

