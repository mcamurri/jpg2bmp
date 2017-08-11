/* 
 * File:   EXIFextractor.cpp
 * Author: Cam
 * 
 * Created on 24 marzo 2011, 22.11
 */

#include <EXIFextractor.h>
#include <ByteHandler.h>
#include <IFD0tag.h>
#include <IFD1tag.h>
#include <EXIFtag.h>

EXIFextractor::EXIFextractor() {
}

EXIFextractor::EXIFextractor(fstream &fs) {
    unsigned short int counter;

    ByteHandler bh;
    int pos = fs.tellg();
    // Recupero la lunghezza dell'header e la salvo.
    bh.getBig(fs, counter);



    // Decremento di due in quanto il counter tiene conto anche della sua stessa
    // lunghezza

    unsigned int exifID;
    bh.getBig(fs, exifID);

    if (exifID != 0x45786966) {
        cout << "Invalid Exif marker. Skipping..." << endl;
        fs.seekg(pos + counter, ios::beg);
        return;
    }
    unsigned char stuff;
    // Byte di stuffing
    bh.get(fs, stuff);
    if (stuff != 0x00) {
        fs.seekg(pos + counter, ios::beg);
        return;
    }
    bh.get(fs, stuff);

    // Byte di padding
    if (stuff != 0x00) {
        fs.seekg(pos + counter, ios::beg);
        return;
    }
    int tiffStart = fs.tellg();
    unsigned short int endianess;
    bh.getLittle(fs, endianess);
    switch (endianess) {
            // Little endian
        case 0x4949:
        {
            cout << "Codifica: little endian" << endl;
            unsigned short int fourtyTwo;
            bh.getLittle(fs, fourtyTwo);
            if (fourtyTwo != 0x2A) {
                return;
            }
            cout << "Campo 42: corretto" << endl;
            unsigned int offsetIFD0;
            bh.getLittle(fs, offsetIFD0);
            cout << "Offset IFD0: 0x" << uppercase << setw(8) << setfill('0') << hex << offsetIFD0 << endl;

            cout << "IFD 0 info: " << endl;
            IFD0tag ifd0(fs, tiffStart, true);

            unsigned int nextIFDpointer;
            bh.getLittle(fs, nextIFDpointer);
            ifd0.printData();
            fs.seekg(tiffStart + ifd0.getExifIFDpointer(), ios::beg);
            cout << endl << "EXIF info:" << endl;
            EXIFtag exif(fs, tiffStart, true);
            exif.printData();
            cout << endl << "IFD1" << endl;
            fs.seekg(nextIFDpointer + tiffStart, ios::beg);
            IFD1tag ifd1(fs, tiffStart, true);
            ifd1.printData();

            //fs.seekg(tiffStart + ifd1.GetJpegInterchangeFormat() + ifd1.GetJpegInterchangeFormatLength());


            //fs.seekg(tiffStart + exifIFDpointer, ios::beg);


            fs.seekg(pos + counter, ios::beg);
        }
            break;

        case 0x4D4D:
        {
            cout << "Codifica: big endian" << endl;
            unsigned short int fourtyTwo;
            bh.getBig(fs, fourtyTwo);
            if (fourtyTwo != 0x2A) {
                return;
            }
            cout << "Campo 42: corretto" << endl;
            unsigned int offsetIFD0;
            bh.getBig(fs, offsetIFD0);
            cout << "Offset IFD0: 0x" << uppercase << setw(8) << setfill('0') << hex << offsetIFD0 << endl;

            cout << "IFD 0 info: " << endl;
            IFD0tag ifd0(fs, tiffStart, false);

            unsigned int nextIFDpointer;
            bh.getBig(fs, nextIFDpointer);
            ifd0.printData();
            fs.seekg(tiffStart + ifd0.getExifIFDpointer(), ios::beg);
            cout << endl << "EXIF info:" << endl;
            EXIFtag exif(fs, tiffStart, false);
            exif.printData();
            cout << endl << "IFD1" << endl;
            fs.seekg(nextIFDpointer + tiffStart, ios::beg);
            IFD1tag ifd1(fs, tiffStart, false);
            ifd1.printData();

            //fs.seekg(tiffStart + ifd1.GetJpegInterchangeFormat() + ifd1.GetJpegInterchangeFormatLength());


            //fs.seekg(tiffStart + exifIFDpointer, ios::beg);


            fs.seekg(pos + counter, ios::beg);
        }
            break;
        default:
            fs.seekg(pos + counter, ios::beg);



    }
}

