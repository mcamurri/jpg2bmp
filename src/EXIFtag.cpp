/* 
 * File:   EXIFtag.cpp
 * Author: Cam
 * 
 * Created on 30 marzo 2011, 11.18
 */

#include <EXIFtag.h>
#include <EXIFextractor.h>
#include <malloc.h>

EXIFtag::EXIFtag() {
}

EXIFtag::EXIFtag(fstream& fs, int tiffStart, bool isLittle) {

    ByteHandler bh;
    if (isLittle) {
        bh.getLittle(fs, fieldNumber);
        cout << "Trovati " << dec << fieldNumber << " campi per il tag Exif IFD" << endl;

        fieldsToShow = (unsigned short int*)malloc(fieldNumber * sizeof (unsigned short int));

        for (int i = 0; i < fieldNumber; i++) {
            unsigned short int identifier;
            bh.getLittle(fs, identifier);

            switch (identifier) {
                case EXPOSURETIME_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag ExposureTime: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    unsigned int valueOffset;
                    bh.getLittle(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);

                    bh.getLittle(fs, exposureTime[0]);
                    bh.getLittle(fs, exposureTime[1]);

                    fieldsToShow[i] = EXPOSURETIME_ID;
                    fs.seekg(currPos);
                }
                    break;
                case EXIFVERSION_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != UNDEFINED_TYPE) {
                        cout << "Errore sul tag ExifVersion: UNDEFINED type"
                                " expected, " << type << " found." << endl;
                        return;
                    }
                    unsigned count;
                    bh.getLittle(fs, count);
                    if (count < 4) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(4): " << count << endl;
                    } else if (count > 4) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(4), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 4;
                    }
                    for (int j = 0; j < count; j++) {
                        fs.get(exifVersion[j]);
                    }
                    exifVersion[4] = '\0';
                    fieldsToShow[i] = EXIFVERSION_ID;
                }
                    break;
                case FLASHPIXVERSION_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != UNDEFINED_TYPE) {
                        cout << "Errore sul tag FlaxpixVersion: UNDEFINED type"
                                " expected, " << type << " found." << endl;
                        return;
                    }
                    unsigned count;
                    bh.getLittle(fs, count);
                    if (count < 4) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(4): " << count << endl;
                    } else if (count > 4) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(4), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 4;
                    }
                    for (int j = 0; j < count; j++) {
                        fs.get(flashpixVersion[j]);
                    }
                    flashpixVersion[4] = '\0';
                    fieldsToShow[i] = FLASHPIXVERSION_ID;
                }
                    break;
                case COMPONENTSCONFIGURATION_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != UNDEFINED_TYPE) {
                        cout << "Errore sul tag ComponentsConfiguration: UNDEFINED type"
                                " expected, " << type << " found." << endl;
                        return;
                    }
                    unsigned count;
                    bh.getLittle(fs, count);
                    if (count < 4) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(4): " << count << endl;
                    } else if (count > 4) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(4), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 4;
                    }
                    for (int j = 0; j < count; j++) {
                        bh.get(fs, componentsConfiguration[j]);
                    }
                    fieldsToShow[i] = COMPONENTSCONFIGURATION_ID;
                }
                    break;
                case COLORSPACE_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag ColorSpace: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int j = 0; j < count; j++) {
                        bh.getLittle(fs, colorSpace);
                    }
                    fs.seekg(2, ios::cur);

                    fieldsToShow[i] = COLORSPACE_ID;
                }
                    break;

                case PIXELXDIMENSION_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != SHORT_TYPE && type != LONG_TYPE) {
                        cout << "Errore sul tag PixelXDimension: SHORT or LONG type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);
                    switch (type) {
                        case SHORT_TYPE:
                        {
                            unsigned short int littlepx;
                            bh.getLittle(fs, littlepx);
                            pixelXdimension = littlepx;
                            fs.seekg(2, ios::cur);
                        }
                            break;
                        case LONG_TYPE:
                        {
                            bh.getLittle(fs, pixelXdimension);
                        }
                            break;
                        default:
                            fs.seekg(4, ios::cur);

                    }
                    fieldsToShow[i] = PIXELXDIMENSION_ID;
                }
                    break;
                case PIXELYDIMENSION_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != SHORT_TYPE && type != LONG_TYPE) {
                        cout << "Errore sul tag PixelYDimension: SHORT or LONG type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);
                    switch (type) {
                        case SHORT_TYPE:
                        {
                            unsigned short int littlepx;
                            bh.getLittle(fs, littlepx);
                            pixelYdimension = littlepx;
                            fs.seekg(2, ios::cur);
                        }
                            break;
                        case LONG_TYPE:
                        {
                            bh.getLittle(fs, pixelYdimension);
                        }
                            break;
                        default:
                            fs.seekg(4, ios::cur);

                    }
                    fieldsToShow[i] = PIXELYDIMENSION_ID;
                }
                    break;
                case EXPOSUREMODE_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag ExposureMode: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int j = 0; j < count; j++) {
                        bh.getLittle(fs, exposureMode);
                    }
                    fs.seekg(2, ios::cur);
                    fieldsToShow[i] = EXPOSUREMODE_ID;
                }
                    break;
                case WHITEBALANCE_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag WhiteBalance: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int j = 0; j < count; j++) {
                        bh.getLittle(fs, whiteBalance);
                    }
                    fs.seekg(2, ios::cur);
                    fieldsToShow[i] = WHITEBALANCE_ID;

                }
                    break;
                case SCENECAPTURETYPE_ID:
                {
                    unsigned short int type;
                    bh.getLittle(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag SceneCaptureType: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getLittle(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int j = 0; j < count; j++) {
                        bh.getLittle(fs, sceneCaptureType);
                    }
                    fs.seekg(2, ios::cur);

                    fieldsToShow[i] = SCENECAPTURETYPE_ID;
                }
                    break;
                default:
                {
                    fs.seekg(10, ios::cur);
                }
            }
        }
    } else {
        bh.getBig(fs, fieldNumber);
        cout << "Trovati " << dec << fieldNumber << " campi per il tag Exif IFD" << endl;

        fieldsToShow = (unsigned short int*)malloc(fieldNumber * sizeof (unsigned short int));

        for (int i = 0; i < fieldNumber; i++) {
            unsigned short int identifier;
            bh.getBig(fs, identifier);

            switch (identifier) {
                case EXPOSURETIME_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != RATIONAL_TYPE) {
                        cout << "Errore sul tag ExposureTime: RATIONAL type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    unsigned int valueOffset;
                    bh.getBig(fs, valueOffset);
                    int currPos = fs.tellg();
                    fs.seekg(tiffStart + valueOffset, ios::beg);

                    bh.getBig(fs, exposureTime[0]);
                    bh.getBig(fs, exposureTime[1]);

                    fieldsToShow[i] = EXPOSURETIME_ID;
                    fs.seekg(currPos);
                }
                    break;
                case EXIFVERSION_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != UNDEFINED_TYPE) {
                        cout << "Errore sul tag ExifVersion: UNDEFINED type"
                                " expected, " << type << " found." << endl;
                        return;
                    }
                    unsigned count;
                    bh.getBig(fs, count);
                    if (count < 4) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(4): " << count << endl;
                    } else if (count > 4) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(4), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 4;
                    }
                    for (int j = 0; j < count; j++) {
                        fs.get(exifVersion[j]);
                    }
                    exifVersion[4] = '\0';
                    fieldsToShow[i] = EXIFVERSION_ID;
                }
                    break;
                case FLASHPIXVERSION_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != UNDEFINED_TYPE) {
                        cout << "Errore sul tag FlaxpixVersion: UNDEFINED type"
                                " expected, " << type << " found." << endl;
                        return;
                    }
                    unsigned count;
                    bh.getBig(fs, count);
                    if (count < 4) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(4): " << count << endl;
                    } else if (count > 4) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(4), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 4;
                    }
                    for (int j = 0; j < count; j++) {
                        fs.get(flashpixVersion[j]);
                    }
                    flashpixVersion[4] = '\0';
                    fieldsToShow[i] = FLASHPIXVERSION_ID;
                }
                    break;
                case COMPONENTSCONFIGURATION_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != UNDEFINED_TYPE) {
                        cout << "Errore sul tag ComponentsConfiguration: UNDEFINED type"
                                " expected, " << type << " found." << endl;
                        return;
                    }
                    unsigned count;
                    bh.getBig(fs, count);
                    if (count < 4) {
                        cout << "Attenzione: valore count minore di quello"
                                "atteso(4): " << count << endl;
                    } else if (count > 4) {
                        cout << "Errore: valore di count maggiore di"
                                " quello atteso(4), i valori in eccesso"
                                " saranno troncati" << endl;
                        count = 4;
                    }
                    for (int j = 0; j < count; j++) {
                        bh.get(fs, componentsConfiguration[j]);
                    }
                    fieldsToShow[i] = COMPONENTSCONFIGURATION_ID;
                }
                    break;
                case COLORSPACE_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag ColorSpace: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int j = 0; j < count; j++) {
                        bh.getBig(fs, colorSpace);
                    }
                    fs.seekg(2, ios::cur);

                    fieldsToShow[i] = COLORSPACE_ID;
                }
                    break;

                case PIXELXDIMENSION_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != SHORT_TYPE && type != LONG_TYPE) {
                        cout << "Errore sul tag PixelXDimension: SHORT or LONG type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);
                    switch (type) {
                        case SHORT_TYPE:
                        {
                            unsigned short int littlepx;
                            bh.getBig(fs, littlepx);
                            pixelXdimension = littlepx;
                            fs.seekg(2, ios::cur);
                        }
                            break;
                        case LONG_TYPE:
                        {
                            bh.getBig(fs, pixelXdimension);
                        }
                            break;
                        default:
                            fs.seekg(4, ios::cur);

                    }
                    fieldsToShow[i] = PIXELXDIMENSION_ID;
                }
                    break;
                case PIXELYDIMENSION_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != SHORT_TYPE && type != LONG_TYPE) {
                        cout << "Errore sul tag PixelYDimension: SHORT or LONG type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);
                    switch (type) {
                        case SHORT_TYPE:
                        {
                            unsigned short int littlepx;
                            bh.getBig(fs, littlepx);
                            pixelYdimension = littlepx;
                            fs.seekg(2, ios::cur);
                        }
                            break;
                        case LONG_TYPE:
                        {
                            bh.getBig(fs, pixelYdimension);
                        }
                            break;
                        default:
                            fs.seekg(4, ios::cur);

                    }
                    fieldsToShow[i] = PIXELYDIMENSION_ID;
                }
                    break;
                case EXPOSUREMODE_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag ExposureMode: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int j = 0; j < count; j++) {
                        bh.getBig(fs, exposureMode);
                    }
                    fs.seekg(2, ios::cur);
                    fieldsToShow[i] = EXPOSUREMODE_ID;
                }
                    break;
                case WHITEBALANCE_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag WhiteBalance: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int j = 0; j < count; j++) {
                        bh.getBig(fs, whiteBalance);
                    }
                    fs.seekg(2, ios::cur);
                    fieldsToShow[i] = WHITEBALANCE_ID;

                }
                    break;
                case SCENECAPTURETYPE_ID:
                {
                    unsigned short int type;
                    bh.getBig(fs, type);
                    if (type != SHORT_TYPE) {
                        cout << "Errore sul tag SceneCaptureType: SHORT type expected, " << type << " found" << endl;
                        return;
                    }
                    unsigned int count;
                    bh.getBig(fs, count);

                    // Essendo un dato SHORT, viene salvato nei primi due
                    // byte, gli altri due li salto.
                    for (int j = 0; j < count; j++) {
                        bh.getBig(fs, sceneCaptureType);
                    }
                    fs.seekg(2, ios::cur);

                    fieldsToShow[i] = SCENECAPTURETYPE_ID;
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

void EXIFtag::printData() {

    for (int i = 0; i < fieldNumber; i++) {

        switch (fieldsToShow[i]) {
            case EXPOSURETIME_ID:
                cout << "Tempo di esposizione:\t" << exposureTime[0] << "/" << exposureTime[1] << " s" << endl;
                break;
            case EXIFVERSION_ID:
                cout << "Exif Version: " << exifVersion << endl;
                break;
            case COMPONENTSCONFIGURATION_ID:
            {
                cout << "Configurazione componenti: [";
                for (int i = 0; i < 4; i++) {
                    switch (componentsConfiguration[i]) {
                        case 1:
                            cout << " Y";
                            break;
                        case 2:
                            cout << " Cb";
                            break;
                        case 3:
                            cout << " Cr";
                            break;
                        case 4:
                            cout << " R";
                            break;
                        case 5:
                            cout << " G";
                            break;
                        case 6:
                            cout << " B";
                            break;
                        default:
                            cout << " .";
                    }
                }
                cout << " ]" << endl;
            }
                break;
            case FLASHPIXVERSION_ID:
                cout << "Flaspix Version: " << flashpixVersion << endl;
                break;
            case COLORSPACE_ID:
            {
                cout << "Spazio colore: ";
                switch (colorSpace) {
                    case 0x0001:
                        cout << "sRGB" << endl;
                        break;
                    case 0xFFFF:
                        cout << "uncalibrated" << endl;
                        break;
                    default:
                        cout << "non definito" << endl;
                }
            }
                break;

            case EXPOSUREMODE_ID:
            {
                switch (exposureMode) {
                    case 0:
                        cout << "Modalita' di esposizione:\t Auto exposure" << endl;
                        break;
                    case 1:
                        cout << "Modalita' di esposizione:\t Manual exposure" << endl;
                        break;
                    case 2:
                        cout << "Modalita' di esposizione:\t Auto bracket" << endl;
                        break;
                    default:
                        ;
                }
            }
                break;
            case WHITEBALANCE_ID:
            {
                switch (whiteBalance) {
                    case 0:
                        cout << "Modalita' di bilanciamento bianco:\t Auto bilanciamento" << endl;
                        break;
                    case 1:
                        cout << "Modalita' di bilanciamento bianco:\t Bilanciamento manuale" << endl;
                        break;
                    default:
                        ;
                }
            }
                break;
            case SCENECAPTURETYPE_ID:
            {
                switch (sceneCaptureType) {
                    case 0:
                        cout << "Modalita' di cattura:\t Standard" << endl;
                        break;
                    case 1:
                        cout << "Modalita' di cattura:\t Paesaggio" << endl;
                        break;
                    case 2:
                        cout << "Modalita' di cattura:\t Ritratto" << endl;
                        break;
                    case 3:
                        cout << "Modalita' di cattura:\t Notturna" << endl;
                        break;
                    default:
                        ;
                }
            }
            break;
            case PIXELXDIMENSION_ID:
            {
                cout << "Dimensione orizzontale: " << pixelXdimension << " px" <<  endl;
            }
            break;
            case PIXELYDIMENSION_ID:
            {
                cout << "Dimensione verticale: " << pixelYdimension << " px" << endl;
            }
            break;
        }
    }
}

