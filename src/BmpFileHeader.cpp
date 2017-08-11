#include <fstream>
#include <iostream>
#include <iomanip>
#include <BmpFileHeader.h>
#include <math.h>

using namespace std;

BmpFileHeader::BmpFileHeader() {
}

BmpFileHeader::BmpFileHeader(int width, int height) {
    //Stringa itentificativa del file bmp
    bfType = 0x4D42;
    //Calcolo della dimensione del file
    bfSize = 54 + 4 * (((1 << 8) & 0xffff) + height * ((width * 8 + 31) / 32));
    bfReserved = 0;
    //Con una tavolozza da 2 colori l'offsett diventa 62 = 54 + 2*4
    bfOffBits = 54 + 4 * 256;
}

BmpFileHeader::BmpFileHeader(int width, int height, unsigned short int bpp) {
    //Stringa itentificativa del file bmp
    bfType = 0x4D42;
    //Calcolo della dimensione del file
    bfSize = 54 + 4 * (((1 << bpp) & 0xffff) + height * ((width * bpp + 31) / 32));
    bfReserved = 0;
    //Con una tavolozza da 2 colori l'offsett diventa 62 = 54 + 2*4

    if (bpp > 8) {
        bfOffBits = 54;
    } else {

        bfOffBits = 54 + pow(2,bpp) * 4;
    }
}

void BmpFileHeader::printFileHeader() {
    cout << "BITMAPFILEHEADER" << endl;
    cout << "bfType: " << dec << bfType << endl;
    cout << "bfSize: " << dec << bfSize << endl;
    cout << "bfReserved: " << dec << bfReserved << endl;
    cout << "bfOffBits: " << dec << bfOffBits << endl << endl;
}

bool BmpFileHeader::get(istream & stream) {
    if (!ByteHandler::getLittle(stream, bfType)) return false;
    if (!ByteHandler::getLittle(stream, bfSize)) return false;
    if (!ByteHandler::getLittle(stream, bfReserved)) return false;
    if (!ByteHandler::getLittle(stream, bfOffBits)) return false;
    return true;
}

bool BmpFileHeader::writeHeader(ostream& stream) {
    //Creo una parola
    char word[2];
    //Ci salvo dentro il bfType
    ByteHandler::wordBuffer(bfType, word);
    //Scrivo sul file
    stream.write(word, 2);
    //Creo una doppia parola
    char dword[4];
    //Ci salvo dentro bfSize
    ByteHandler::dwordBuffer(bfSize, dword);
    //Scrivo sul file
    stream.write(dword, 4);
    //e così via...
    ByteHandler::dwordBuffer(bfReserved, dword);
    stream.write(dword, 4);
    ByteHandler::dwordBuffer(bfOffBits, dword);
    stream.write(dword, 4);
    return true;
}

WORD BmpFileHeader::getBfType() const {
    return bfType;
}

DWORD BmpFileHeader::getBfSize() const {
    return bfSize;
}
