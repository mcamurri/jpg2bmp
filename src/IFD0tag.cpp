/* 
 * File:   IFDtag.cpp
 * Author: Cam
 * 
 * Created on 30 marzo 2011, 11.11
 */

#include <IFD0tag.h>
#include <EXIFextractor.h>

IFD0tag::IFD0tag() {
}

IFD0tag::IFD0tag(fstream& fs, int tiffStart, bool isLittle) {
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
                case IMAGEDESCRIPTION_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag imagedescription: ascii type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getLittle(fs, count);
                    if (count > 4) {

                        unsigned int valueOffset;
                        bh.getLittle(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            imageDescription += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            imageDescription += c;
                        }
                    }
                }
                    break;
                case MAKE_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag make: ascii type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getLittle(fs, count);
                    if (count > 4) {
                        unsigned int valueOffset;
                        bh.getLittle(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            make += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            make += c;
                        }
                    }
                }
                    break;
                case MODEL_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag model: ascii type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getLittle(fs, count);
                    if (count > 4) {
                        unsigned int valueOffset;
                        bh.getLittle(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            model += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            model += c;
                        }
                    }
                }
                    break;
                case ORIENTATION_ID:
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
                        bh.getLittle(fs, orientation);
                    }
                    fs.seekg(2, ios::cur);


                }
                    break;
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
                case SOFTWARE_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag model: ascii type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getLittle(fs, count);
                    if (count > 4) {
                        unsigned int valueOffset;
                        bh.getLittle(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            software += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            software += c;
                        }
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
                case DATEIME_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag dateTime: ascii type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getLittle(fs, count);
                    if (count != 20) {
                        cout << "Attenzione: lunghezza della data diversa"
                                " da come atteso(20):" << count << endl;
                    }
                    if (count > 4) {

                        unsigned int valueOffset;
                        bh.getLittle(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            dateTime += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            dateTime += c;
                        }
                    }
                }
                    break;
                case YCBCRPOSITIONING_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag YCbCrPositioning: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int i = 0; i < count; i++) {
                        bh.getLittle(fs, yCbCrPositioning);
                    }
                    fs.seekg(2, ios::cur);


                }
                    break;
                case EXIFIFDPOINTER_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != LONG_TYPE) {
                        cout << "Errore sul tag orientation: LONG type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    for (int i = 0; i < count; i++) {
                        bh.getLittle(fs, exifIFDpointer);
                    }
                }
                    break;
                case GPSINFOIFDPOINTER_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != LONG_TYPE) {
                        cout << "Errore sul tag orientation: LONG type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    for (int i = 0; i < count; i++) {
                        bh.getLittle(fs, gpsInfoIFDpointer);
                    }
                }
                    break;
                case COPYRIGHT_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag model: ascii type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getLittle(fs, count);
                    if (count > 4) {
                        unsigned int valueOffset;
                        bh.getLittle(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            software += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            copyright += c;
                        }
                    }
                }
                    break;
                case TRANSFERFUNCTION_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag YCbCrPositioning: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);
                    if (count < 3 * 256) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(3*256): " << count << endl;
                    } else if (count > 3 * 256) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(3*256), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 3 * 256;
                    }
                    unsigned int valueOffset;
                    bh.getLittle(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);
                    for (int i = 0; i < count; i++) {
                        bh.getLittle(fs, transferFunction[i]);
                    }
                    fs.seekg(currPos);
                }
                    break;
                case ARTIST_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag artist: ASCII type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getLittle(fs, count);
                    if (count > 4) {
                        unsigned int valueOffset;
                        bh.getLittle(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            artist += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            artist += c;
                        }
                    }
                }
                    break;
                case PRIMARYCHROMATICIES_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag WhitePoint: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);
                    if (count < 6) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(6): " << count << endl;
                    } else if (count > 6) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(6), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 6;
                    }

                    unsigned int valueOffset;
                    bh.getLittle(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);
                    for (int i = 0; i < count; i++) {
                        bh.getLittle(fs, whitePoint[i][0]);
                        bh.getLittle(fs, whitePoint[i][1]);
                    }

                    fs.seekg(currPos);

                }
                    break;
                case WHITEPOINT_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag WhitePoint: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);
                    if (count < 2) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(6): " << count << endl;
                    } else if (count > 2) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(6), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 2;
                    }

                    unsigned int valueOffset;
                    bh.getLittle(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);
                    for (int i = 0; i < count; i++) {
                        bh.getLittle(fs, whitePoint[i][0]);
                        bh.getLittle(fs, whitePoint[i][1]);
                    }

                    fs.seekg(currPos);

                }
                    break;
                case YCBCRCOEFFICIENTS_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag WhitePoint: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);
                    if (count < 3) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(6): " << count << endl;
                    } else if (count > 3) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(6), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 3;
                    }

                    unsigned int valueOffset;
                    bh.getLittle(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);
                    for (int i = 0; i < count; i++) {
                        bh.getLittle(fs, yCbCrCoefficients[i][0]);
                        bh.getLittle(fs, yCbCrCoefficients[i][1]);
                    }

                    fs.seekg(currPos);

                }
                    break;
                case REFERENCEBLACKWHITE_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag WhitePoint: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);
                    if (count < 6) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(6): " << count << endl;
                    } else if (count > 6) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(6), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 6;
                    }

                    unsigned int valueOffset;
                    bh.getLittle(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);
                    for (int i = 0; i < count; i++) {
                        bh.getLittle(fs, referenceBlackWhite[i][0]);
                        bh.getLittle(fs, referenceBlackWhite[i][1]);
                    }

                    fs.seekg(currPos);

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
                case IMAGEDESCRIPTION_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag imagedescription: ascii type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getBig(fs, count);
                    if (count > 4) {

                        unsigned int valueOffset;
                        bh.getBig(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            imageDescription += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            imageDescription += c;
                        }
                    }
                }
                    break;
                case MAKE_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag make: ascii type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getBig(fs, count);
                    if (count > 4) {
                        unsigned int valueOffset;
                        bh.getBig(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            make += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            make += c;
                        }
                    }
                }
                    break;
                case MODEL_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag model: ascii type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getBig(fs, count);
                    if (count > 4) {
                        unsigned int valueOffset;
                        bh.getBig(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            model += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            model += c;
                        }
                    }
                }
                    break;
                case ORIENTATION_ID:
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
                        bh.getBig(fs, orientation);
                    }
                    fs.seekg(2, ios::cur);


                }
                    break;
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
                case SOFTWARE_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag model: ascii type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getBig(fs, count);
                    if (count > 4) {
                        unsigned int valueOffset;
                        bh.getBig(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            software += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            software += c;
                        }
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
                case DATEIME_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag dateTime: ascii type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getBig(fs, count);
                    if (count != 20) {
                        cout << "Attenzione: lunghezza della data diversa"
                                " da come atteso(20):" << count << endl;
                    }
                    if (count > 4) {

                        unsigned int valueOffset;
                        bh.getBig(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            dateTime += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            dateTime += c;
                        }
                    }
                }
                    break;
                case YCBCRPOSITIONING_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag YCbCrPositioning: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int i = 0; i < count; i++) {
                        bh.getBig(fs, yCbCrPositioning);
                    }
                    fs.seekg(2, ios::cur);


                }
                    break;
                case EXIFIFDPOINTER_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != LONG_TYPE) {
                        cout << "Errore sul tag orientation: LONG type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    for (int i = 0; i < count; i++) {
                        bh.getBig(fs, exifIFDpointer);
                    }
                }
                    break;
                case GPSINFOIFDPOINTER_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != LONG_TYPE) {
                        cout << "Errore sul tag orientation: LONG type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    for (int i = 0; i < count; i++) {
                        bh.getBig(fs, gpsInfoIFDpointer);
                    }
                }
                    break;
                case COPYRIGHT_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag model: ascii type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getBig(fs, count);
                    if (count > 4) {
                        unsigned int valueOffset;
                        bh.getBig(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            software += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            copyright += c;
                        }
                    }
                }
                    break;
                case TRANSFERFUNCTION_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag YCbCrPositioning: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);
                    if (count < 3 * 256) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(3*256): " << count << endl;
                    } else if (count > 3 * 256) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(3*256), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 3 * 256;
                    }
                    unsigned int valueOffset;
                    bh.getBig(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);
                    for (int i = 0; i < count; i++) {
                        bh.getBig(fs, transferFunction[i]);
                    }
                    fs.seekg(currPos);
                }
                    break;
                case ARTIST_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != ASCII_TYPE) {
                        cout << "errore sul tag artist: ASCII type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;

                    bh.getBig(fs, count);
                    if (count > 4) {
                        unsigned int valueOffset;
                        bh.getBig(fs, valueOffset);
                        int currPos = fs.tellg();
                        fs.seekg(tiffStart + valueOffset, ios::beg);
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            artist += c;
                        }
                        fs.seekg(currPos);

                    } else {
                        for (int i = 1; i < count; i++) {
                            char c;
                            fs.get(c);
                            if (c == 0x00) {
                                break;
                            }
                            artist += c;
                        }
                    }
                }
                    break;
                case PRIMARYCHROMATICIES_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag WhitePoint: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);
                    if (count < 6) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(6): " << count << endl;
                    } else if (count > 6) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(6), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 6;
                    }

                    unsigned int valueOffset;
                    bh.getBig(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);
                    for (int i = 0; i < count; i++) {
                        bh.getBig(fs, whitePoint[i][0]);
                        bh.getBig(fs, whitePoint[i][1]);
                    }

                    fs.seekg(currPos);

                }
                    break;
                case WHITEPOINT_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag WhitePoint: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);
                    if (count < 2) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(6): " << count << endl;
                    } else if (count > 2) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(6), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 2;
                    }

                    unsigned int valueOffset;
                    bh.getBig(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);
                    for (int i = 0; i < count; i++) {
                        bh.getBig(fs, whitePoint[i][0]);
                        bh.getBig(fs, whitePoint[i][1]);
                    }

                    fs.seekg(currPos);

                }
                    break;
                case YCBCRCOEFFICIENTS_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag WhitePoint: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);
                    if (count < 3) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(6): " << count << endl;
                    } else if (count > 3) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(6), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 3;
                    }

                    unsigned int valueOffset;
                    bh.getBig(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);
                    for (int i = 0; i < count; i++) {
                        bh.getBig(fs, yCbCrCoefficients[i][0]);
                        bh.getBig(fs, yCbCrCoefficients[i][1]);
                    }

                    fs.seekg(currPos);

                }
                    break;
                case REFERENCEBLACKWHITE_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag WhitePoint: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);
                    if (count < 6) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(6): " << count << endl;
                    } else if (count > 6) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(6), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 6;
                    }

                    unsigned int valueOffset;
                    bh.getBig(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);
                    for (int i = 0; i < count; i++) {
                        bh.getBig(fs, referenceBlackWhite[i][0]);
                        bh.getBig(fs, referenceBlackWhite[i][1]);
                    }

                    fs.seekg(currPos);

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

void IFD0tag::printData() {
    if (imageDescription != "") {
        cout << "Descrizione immagine: \t\"" << imageDescription << "\"" << endl;
    }
    if (make != "") {
        cout << "Marca macchina fotografica: \t\"" << make << "\"" << endl;
    }
    if (model != "") {
        cout << "Modello macchina fotografica: \t\"" << model << "\"" << endl;
    }

    switch (orientation) {
        case 1:
            cout << "Orientazione:\t Riga 0 in alto, colonna 0 a sinistra" << endl;
            break;
        case 2:
            cout << "Orientazione:\t Riga 0 in alto, colonna 0 a destra" << endl;
            break;
        case 3:
            cout << "Orientazione:\t Riga 0 in basso, colonna 0 a destra" << endl;
            break;
        case 4:
            cout << "Orientazione:\t Riga 0 in basso, colonna 0 a sinistra" << endl;
            break;
        case 5:
            cout << "Orientazione:\t Riga 0 a sinistra, colonna 0 in alto" << endl;
            break;
        case 6:
            cout << "Orientazione:\t Riga 0 a destra, colonna 0 in alto" << endl;
            break;
        case 7:
            cout << "Orientazione:\t Riga 0 a destra, colonna zero in basso" << endl;
            break;
        case 8:
            cout << "Orientazione:\t Riga 0 a sinistra, colonna zero in basso" << endl;
            break;
        default:
            break;

    }
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
    if (software != "") {
        cout << "Software: " << software << endl;
    }
    if (dateTime != "") {
        cout << "Data e ora dello scatto: " << dateTime << endl;
    }

    switch (yCbCrPositioning) {
        case 1:
            cout << "Posizionamento Y Cb Cr:\t" << "centered" << endl;
            break;
        case 2:
            cout << "Posizionamento Y Cb Cr:\t" << "co-sited" << endl;
            break;
        default:
            ;
    }

    cout << "Exif Tag: 0x" << hex << uppercase << exifIFDpointer << endl;
}

