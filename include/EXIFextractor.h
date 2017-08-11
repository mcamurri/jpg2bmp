#include "ByteHandler.h"
#include <string>

#ifndef EXIFEXTRACTOR_H
#define	EXIFEXTRACTOR_H

#define IMAGEWIDTH_ID  0x100
#define IMAGELENGTH_ID  0x101
#define BITSPERSAMPLE_ID  0x102
#define COMPRESSION_ID  0x103
#define PHOTOMETRICINTERPRETATION_ID  0x106
#define ORIENTATION_ID  0x112
#define SAMPLESPERPIXEL_ID  0x115
#define PLANARCONFIGURATION_ID  0x11C
#define YCBCRSUBSAMPLING_ID  0x212
#define YCBCRPOSITIONING_ID  0x213
#define XRESOLUTION_ID  0x11A
#define YRESOLUTION_ID  0x11B
#define RESOLUTIONUNIT_ID  0x128

#define STRIPOFFSETS_ID  0x111
#define ROWSPERSTRIP_ID  0x116
#define STRIPBYTECOUNTS_ID  0x117
#define JPEGINTERCHANGEFORMAT_ID  0x201
#define JPEGINTERCHANGEFORMATLENGTH_ID  0x202

#define TRANSFERFUNCTION_ID  0x12D
#define WHITEPOINT_ID  0x13E
#define PRIMARYCHROMATICIES_ID  0x13F
#define YCBCRCOEFFICIENTS_ID  0x211
#define REFERENCEBLACKWHITE_ID  0x214

#define DATEIME_ID  0x132
#define IMAGEDESCRIPTION_ID  0x10E
#define MAKE_ID  0x10F
#define MODEL_ID  0x110
#define SOFTWARE_ID  0x131
#define ARTIST_ID  0x13B
#define COPYRIGHT_ID  0x8298

#define EXIFVERSION_ID 0x9000
#define FLASHPIXVERSION_ID 0xA000
#define COLORSPACE_ID 0xA001
#define PIXELXDIMENSION_ID 0xA002
#define PIXELYDIMENSION_ID 0xA003
#define COMPONENTSCONFIGURATION_ID 0x9101
#define COMPRESSEDBITSPERPIXEL_ID 0x9102
#define MAKERNOTE_ID 0x927C
#define USERCOMMENT_ID 0x9286
#define RELATEDSOUNDFILE_ID 0xA004
#define DATETIMEORIGINAL_ID 0x9003
#define DATETIMEDIGITIZED_ID 0x9004
#define SUBSECTIME_ID 0x9290
#define SUBSECTIMEORIGINAL_ID 0x9291
#define SUBSECTIMEDIGITIZED_ID 0x9292
#define EXPOSURETIME_ID 0x829A
#define FNUMBER_ID 0x829D
#define EXPOSUREPROGRAM_ID 0x8822
#define SPECTRALSENSIVITY_ID 0x8824
#define ISOSPEEDRATINGS_ID 0x8827
#define OECF_ID 0x8828
#define SHUTTERSPEEDVALUE_ID 0x9201
#define APERTUREVALUE_ID 0x9202
#define BRIGHTNESSVALUE_ID 0x9203
#define EXPOSUREBIASVALUE_ID 0x9204
#define MAXAPERTUREVALUE_ID 0x9205
#define SUBJECTDISTANCE_ID 0x9206
#define METERINGMODE_ID 0x9207
#define LIGHTSOURCE_ID 0x9208
#define FLASH_ID 0x9209
#define SUBJECTAREA_ID 0x9214
#define FOCALLENGTH_ID 0x920A
#define FLASHENERGY_ID 0xA20B
#define SPACIALFREQUENCYRESPONSE_ID 0xA20C
#define FOCALPLANEXRESOLUTION_ID 0xA20E
#define FOCALPLANEYRESOLUTION_ID 0xA20F
#define FOCALPLANERESOLUTIONUNIT_ID 0xA210
#define SUBJECTLOCATION_ID 0xA214
#define EXPOSUREINDEX_ID 0xA215
#define SENSINGMETHOD_ID 0xA217
#define FILESOURCE_ID 0xA300
#define SCENETYPE_ID 0xA301
#define CFAPATTERN_ID 0xA302
#define CUSTOMRENDERED_ID 0xA401
#define EXPOSUREMODE_ID 0xA402
#define WHITEBALANCE_ID 0xA403
#define DIGITALZOOMRATIO_ID 0xA404
#define FOCALLENGTHIN35MMFILM_ID 0xA405
#define SCENECAPTURETYPE_ID 0xA406
#define SCENECONTROL_ID 0xA407
#define CONTRAST_ID 0xA408
#define SATURATION_ID 0xA409
#define SHARPNESS_ID 0xA40A
#define DEVICESETTINGDESCRIPTION_ID 0xA40B
#define SUBJECTDISTANCERANGE_ID 0xA40C
#define IMAGEUNIQUEID_ID 0xA420


#define EXIFIFDPOINTER_ID 0x8769
#define GPSINFOIFDPOINTER_ID 0x8825

#define WORD_TYPE  1
#define ASCII_TYPE 2
#define SHORT_TYPE 3
#define LONG_TYPE 4
#define RATIONAL_TYPE 5
#define UNDEFINED_TYPE 7
#define SLONG_TYPE 9
#define SRATIONAL 10

#define FLASHPIXFORMAT "0100"
#define SRGB 0x1
#define UNCALIBRATED 0xFFFF

using namespace std;

class EXIFextractor {
public:
    EXIFextractor();
    EXIFextractor(fstream &fs);
private:




    // Exif Tag





    // gpsVersionID
    // gpsLatitudeRef
    // gpsLatitude
    // gpsLongitudeRef
    // gpsLongitude
    // gpsAltitudeRef
    // gpsAltitude
    // gpsTimeStamp
    // gpsSatellites
    // gpsStatus
    // gpsMeasureMode
    // gpsDOP
    // gpsSpeedRef
    // gpsSpeed
    // gpsTrackRef
    // gpsTrack
    // gpsImgDirectionRef
    // gpsImgDirection
    // gpsMapDatum
    // gpsDestLatitudeRef
    // gpsDestLatitude
    // gpsDestLongitudeRef
    // gpsDestLongitude
    // gpsDestBearingRef
    // gpsDestBearing
    // gpsDestDistanceRef
    // gpsDestDistance
    // gpsProcessingMethod
    // gpsAreaInformation
    // gpsDateStamp
    // gpsDIfferential


    // interoperabilityIndex
    // interoperabilityVersion




};

#endif	/* EXIFEXTRACTOR_H */

