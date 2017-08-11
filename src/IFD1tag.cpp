/* 
 * File:   IFDtag.cpp
 * Author: Cam
 * 
 * Created on 30 marzo 2011, 11.11
 */

#include <IFD1tag.h>
#include <EXIFextractor.h>

IFD1tag::IFD1tag() {
}

IFD1tag::IFD1tag(fstream& fs, int tiffStart, bool isLittle) {
    ByteHandler bh;
    if (isLittle) {
        unsigned short int fieldNumber;
        bh.getLittle(fs, fieldNumber);
        cout << "Trovati " << dec << fieldNumber << " campi per il tag IFD" << endl;
        // Lettura dei fieldNumber campi
        for (int i = 0; i < fieldNumber; i++) {
            unsigned short int identifier;
            bh.getLittle(fs, identifier);

            switch (identifier) {
                
                case XRESOLUTION_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag orientation: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    unsigned int valueOffset;
                    bh.getLittle(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);

                    bh.getLittle(fs, xResolution[0]);
                    bh.getLittle(fs, xResolution[1]);

                    fs.seekg(currPos);

                }
                    break;
                case YRESOLUTION_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag orientation: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    unsigned int valueOffset;
                    bh.getLittle(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);

                    bh.getLittle(fs, yResolution[0]);
                    bh.getLittle(fs, yResolution[1]);

                    fs.seekg(currPos);

                }
                    break;
                
                case JPEGINTERCHANGEFORMAT_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != LONG_TYPE) {
                        cout << "Errore sul tag JPEGInterchangeFormat: LONG type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int i = 0; i < count; i++) {
                        bh.getLittle(fs, jpegInterchangeFormat);
                    }
                    


                }
                    break;

                    case JPEGINTERCHANGEFORMATLENGTH_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != LONG_TYPE) {
                        cout << "Errore sul tag JPEGInterchangeFormatLength: LONG type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int i = 0; i < count; i++) {
                        bh.getLittle(fs, jpegInterchangeFormatLength);
                    }
                    


                }
                    break;

                    case RESOLUTIONUNIT_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag orientation: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int i = 0; i < count; i++) {
                        bh.getLittle(fs, resolutionUnit);
                    }
                    fs.seekg(2, ios::cur);


                }
                    break;

                
                default:
                {
                    fs.seekg(10, ios::cur);
                }
            }
        }
    } else {
                unsigned short int fieldNumber;
        bh.getBig(fs, fieldNumber);
        cout << "Trovati " << dec << fieldNumber << " campi per il tag IFD" << endl;
        // Lettura dei fieldNumber campi
        for (int i = 0; i < fieldNumber; i++) {
            unsigned short int identifier;
            bh.getBig(fs, identifier);

            switch (identifier) {

                case XRESOLUTION_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag orientation: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    unsigned int valueOffset;
                    bh.getBig(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);

                    bh.getBig(fs, xResolution[0]);
                    bh.getBig(fs, xResolution[1]);

                    fs.seekg(currPos);

                }
                    break;
                case YRESOLUTION_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag orientation: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    unsigned int valueOffset;
                    bh.getBig(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);

                    bh.getBig(fs, yResolution[0]);
                    bh.getBig(fs, yResolution[1]);

                    fs.seekg(currPos);

                }
                    break;

                case JPEGINTERCHANGEFORMAT_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != LONG_TYPE) {
                        cout << "Errore sul tag JPEGInterchangeFormat: LONG type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int i = 0; i < count; i++) {
                        bh.getBig(fs, jpegInterchangeFormat);
                    }



                }
                    break;

                    case JPEGINTERCHANGEFORMATLENGTH_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != LONG_TYPE) {
                        cout << "Errore sul tag JPEGInterchangeFormatLength: LONG type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int i = 0; i < count; i++) {
                        bh.getBig(fs, jpegInterchangeFormatLength);
                    }



                }
                    break;

                    case RESOLUTIONUNIT_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag orientation: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int i = 0; i < count; i++) {
                        bh.getBig(fs, resolutionUnit);
                    }
                    fs.seekg(2, ios::cur);


                }
                    break;


                default:
                {
                    fs.seekg(10, ios::cur);
                }
            }
        }

    }
}

void IFD1tag::printData() {
   
    // Campo obbligatorio nessun controllo sull'esistenza
    double res = (xResolution[0] + 0.0) / (xResolution[1] + 0.0);
    cout << "Risoluzione orizzontale: " << setprecision(2) << fixed << res;
    switch (resolutionUnit) {
        case 2:
            cout << " px/inch" << endl;
            break;
        case 3:
            cout << " px/cm" << endl;
            break;
        default:
            cout << endl;
            break;



    }
    res = (yResolution[0] + 0.0) / (yResolution[1] + 0.0);
    cout << "Risoluzione verticale: " << setprecision(2) << fixed << res;


    switch (resolutionUnit) {
        case 2:
            cout << " px/inch" << endl;
            break;
        case 3:
            cout << " px/cm" << endl;
            break;
        default:
            cout << endl;
            break;

    }

    cout << "JPEGinterchangeFormat:\t 0x" << setw(8) << setfill('0') << hex << uppercase << jpegInterchangeFormat << endl;
    cout << "JPEGinterchangeFormatLength:\t " << dec << uppercase << jpegInterchangeFormatLength << endl;

}

