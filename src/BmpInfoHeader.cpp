/* 
 * File:   BmpInfoHeader.cpp
 * Author: cam
 * 
 * Created on March 31, 2010, 12:16 PM
 */

#include <iostream>

#include <BmpInfoHeader.h>

BmpInfoHeader::BmpInfoHeader() {

}

BmpInfoHeader::BmpInfoHeader(int width, int height) {
    //L'infoheader è sempre grande 40 = 0x28
    biSize = 0x28;
    biWidth = width;
    biHeight = height;
    biPlanes = 1;
    //Un byte per ogni pixel
    biBitCount = 8;
    biCompression = 0;
    biSizeImage = 0;
    biXPelsPerMeter = 0;
    biYPelsPerMeter = 0;
    //256 colori usati
    biClrUsed = 256;
    biClrImportant = 256;

}
BmpInfoHeader::BmpInfoHeader(int width, int height, unsigned char bpp) {
    //L'infoheader è sempre grande 40 = 0x28
    biSize = 0x28;
    biWidth = width;
    biHeight = height;
    biPlanes = 1;
    //Un byte per ogni pixel
    biBitCount = bpp;
    biCompression = 0;
    biSizeImage = 0;
    biXPelsPerMeter = 0;
    biYPelsPerMeter = 0;
    //256 colori usati
    biClrUsed = 0;
    biClrImportant = 0;

}


bool BmpInfoHeader::get(istream &stream) {
    
    fstream in("matrix.txt", ios::in | ios::binary);
   
    
    
    if (!ByteHandler::getLittle(stream, biSize)) {
        return false;
    }
    if (!ByteHandler::getLittle(stream, biWidth)) {
        return false;
    }
    if (!ByteHandler::getLittle(stream, biHeight)) {
        return false;
    }
    if (!ByteHandler::getLittle(stream, biPlanes)) {
        return false;
    }
    if (!ByteHandler::getLittle(stream, biBitCount)) {
        return false;
    }
    if (!ByteHandler::getLittle(stream, biCompression)) {
        return false;
    }
    if (!ByteHandler::getLittle(stream, biSizeImage)) {
        return false;
    }
    if (!ByteHandler::getLittle(stream, biXPelsPerMeter)) {
        return false;
    }
    if (!ByteHandler::getLittle(stream, biYPelsPerMeter)) {
        return false;
    }
    if (!ByteHandler::getLittle(stream, biClrUsed)) {
        return false;
    }
    if (!ByteHandler::getLittle(stream, biClrImportant)) {
        return false;
    }
    return true;
}

void BmpInfoHeader::printInfoHeader() {
    cout << "BITMAPINFOHEADER" << endl;
    cout << "biSize: " << dec << biSize << endl;
    cout << "biWidth: " << dec << biWidth << endl;
    cout << "biHeight: " << dec << biHeight << endl;
    cout << "biPlanes: " << dec << biPlanes << endl;
    cout << "biBitCount: " << dec << biBitCount << endl;
    cout << "biCompression: " << dec << biCompression << endl;
    cout << "biSizeImage: " << dec << biSizeImage << endl;
    cout << "biXPelsPerMeter: " << dec << biXPelsPerMeter << endl;
    cout << "biYPelsPerMeter: " << dec << biYPelsPerMeter << endl;
    cout << "biClrUsed: " << dec << biClrUsed << endl;
    cout << "biClrImportant: " << dec << biClrImportant << endl << endl;
}

bool BmpInfoHeader::writeHeader(ostream &stream) {
    char word[2];

    //Creo una doppia parola
    char dword[4];
    //Ci salvo dentro bfSize
    ByteHandler::dwordBuffer(biSize, dword);
    
    //Scrivo sul file
    stream.write(dword, 4);

    char l0ng[sizeof (LONG)];
    //e così via...
    ByteHandler::longBuffer(biWidth, l0ng);
    stream.write(l0ng, sizeof (LONG));
    ByteHandler::longBuffer(biHeight, l0ng);
    stream.write(l0ng, sizeof (LONG));
    ByteHandler::wordBuffer(biPlanes, word);
    stream.write(word, 2);
    ByteHandler::wordBuffer(biBitCount, word);
    stream.write(word, 2);
    ByteHandler::dwordBuffer(biCompression, dword);
    stream.write(dword, 4);
    ByteHandler::dwordBuffer(biSizeImage, dword);
    stream.write(dword, 4);
    ByteHandler::longBuffer(biXPelsPerMeter, l0ng);
    stream.write(l0ng, sizeof (LONG));
    ByteHandler::longBuffer(biYPelsPerMeter, l0ng);
    stream.write(l0ng, sizeof (LONG));
    ByteHandler::dwordBuffer(biClrUsed, dword);
    stream.write(dword, 4);
    ByteHandler::dwordBuffer(biClrImportant, dword);
    stream.write(dword, 4);
    return true;
}
