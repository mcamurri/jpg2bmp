/* 
 * File:   BmpImage.cpp
 * Author: cam
 * 
 * Created on April 3, 2010, 7:28 PM
 */
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <BmpFileHeader.h>
#include <BmpInfoHeader.h>
#include <RgbQuad.h>
#include <BmpImage.h>

using namespace std;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;

BmpImage::BmpImage() {
    width = 0;
    height = 0;
    linewidth = 0;
    data = NULL;
    imagePalette = NULL;
}

BmpImage::BmpImage(unsigned w, unsigned h) :

width(w),
height(h),
linewidth(((w + 3) >> 2) << 2),
imageFileHeader(w, h, 8),
imageInfoHeader(w, h) {

    data = (unsigned char*) calloc(linewidth * height, 1);
    if (data == NULL) {
        cout << "Errore di allocazione della memoria." << endl;
        cerr << "Errore di allocazione della memoria. Allocazione"
                " di BmpImage.data non riuscita." << endl;
    }
    imagePalette = (RgbQuad*) malloc(256 * sizeof (RgbQuad));

    if (imagePalette == NULL) {
        cout << "Errore di allocazione della memoria." << endl;
        cerr << "Errore di allocazione della memoria. Allocazione"
                " di BmpImage.imagePalette non riuscita." << endl;

    }
    for (int i = 0; i < 256; i++) {
        RgbQuad q;
        q.setColor(0, i);
        q.setColor(1, i);
        q.setColor(2, i);
        imagePalette[i] = q;
    }
}

void BmpImage::setPaletteColor(unsigned char index, RgbQuad quad) {
    imagePalette[index] = quad;
}

BmpImage::BmpImage(unsigned int w, unsigned int h, unsigned char bpp) :
width(w),
height(h),
linewidth(((w*(bpp / 8) + 3) >> 2) << 2),



imageFileHeader(w, h, bpp),
imageInfoHeader(w, h, (unsigned short int) bpp) {
    unsigned int colors = pow(2, bpp);

    data = (unsigned char*) calloc(linewidth * height, 1);
    if (data == NULL) {
        cout << "Errore di allocazione della memoria." << endl;
        cerr << "Errore di allocazione della memoria. Allocazione"
                " di BmpImage.data non riuscita." << endl;
    }

    if (bpp < 9) {
        imagePalette = (RgbQuad*) malloc(colors * sizeof (RgbQuad));
        if (imagePalette == NULL) {
            cout << "Errore di allocazione della memoria." << endl;
            cerr << "Errore di allocazione della memoria. Allocazione"
                    " di BmpImage.imagePalette non riuscita." << endl;
        }

        switch (colors) {
            case 2:
            {
                for (int j = 0; j < 2; j++) {
                    RgbQuad q;
                    for (int i = 0; i < 3; i++) {
                        q.setColor(i, (j % 2)*255);
                    }
                }
            }
                break;
            default:
            {
                int step = (256 / colors);
                for (int i = 0; i < colors; i++) {
                    RgbQuad q;
                    q.setColor(0, i * step);
                    q.setColor(1, i * step);
                    q.setColor(2, i * step);
                    imagePalette[i] = q;
                }
            }
        }

    } else {
        imagePalette = NULL;
    }
}

BmpImage::BmpImage(const char *path) {
    fstream in(path, ios::in | ios::binary);
    // Controllo se ci sono errori
    if (!in) {
        cout << "Impossibile aprire il file " << path << endl;
    }
    imageFileHeader.get(in);
    imageInfoHeader.get(in);

    if ((14 + imageInfoHeader.getBiSize() != imageFileHeader.getBfOffBits())) {
        if (imageInfoHeader.getBiClrUsed() == 0) {
            unsigned int colors = pow(2, imageInfoHeader.getBiBitCount());
            imagePalette = (RgbQuad*) malloc(colors * sizeof (RgbQuad));
            for (unsigned i = 0; i < colors; i++) {
                imagePalette[i].get(in);
            }
        } else {
            if (((int) (imageInfoHeader.getBiClrUsed())*4) == (imageFileHeader.getBfOffBits() - (14 + imageInfoHeader.getBiSize()))) {
                imagePalette = (RgbQuad*) malloc(imageInfoHeader.getBiClrUsed() * sizeof (RgbQuad));
                for (unsigned i = 0; i < imageInfoHeader.getBiClrUsed(); i++) {
                    imagePalette[i].get(in);
                }
            } else {
                cout << "Errore: header mal composti.";
            }
        }
    } else {
        imagePalette = NULL;
    }

    height = imageInfoHeader.getBiHeight() > 0 ? imageInfoHeader.getBiHeight() : -imageInfoHeader.getBiHeight();
    width = imageInfoHeader.getBiWidth();
    linewidth = ((width + 3) >> 2) << 2;

    switch ((int) imageInfoHeader.getBiBitCount()) {
        case 1:
        {
            unsigned int bytes = (unsigned int) ceil(linewidth * height / 8.0);
            data = (unsigned char*) calloc(bytes, 1);
        }
            break;

        case 4:
        {
            unsigned int bytes = (unsigned int) ceil(linewidth * height / 2.0);
            data = (unsigned char*) calloc(bytes, 1);
        }
            break;

        case 8:
            data = (unsigned char*) calloc(linewidth * height, 1);

            break;
        case 16:
            data = (unsigned char*) calloc(linewidth * height, 2);
            break;

        case 24:
            data = (unsigned char*) calloc(linewidth * height, 3);
            break;

        case 32:
            data = (unsigned char*) calloc(linewidth*height, 4);
            break;

        default:
            cout << "Errore di lettura, numero di bit per pixel errato." << endl;
    }
    if (imageInfoHeader.getBiBitCount() > 7) {
        if (imageInfoHeader.getBiHeight() > 0) {
            for (int y = getHeight() - 1; y >= 0; y--)
                in.read((char *) getLineptr(y), getLinewidth()*(imageInfoHeader.getBiBitCount() / 8));
        } else {
            for (int y = 0; y < getHeight(); y++)
                in.read((char *) getLineptr(y), getLinewidth()*(imageInfoHeader.getBiBitCount() / 8));
        }
    } else {
        cout << "Errore. Valore di bit per pixel non ancora supportato" << endl;
    }
    in.close();
}

BmpImage::~BmpImage() {
    free(data);
    free(imagePalette);
}

BmpImage::BmpImage(const BmpImage& orig) {
    width = orig.width;
    height = orig.height;
    linewidth = orig.linewidth;
    imageFileHeader = orig.imageFileHeader;
    imageInfoHeader = orig.imageInfoHeader;

    if (orig.imagePalette != NULL) {
        unsigned int colors;
        if (imageInfoHeader.getBiClrUsed() == 0) {
            colors = pow(2, (int) imageInfoHeader.getBiBitCount());
        } else {
            colors = imageInfoHeader.getBiClrUsed();
        }
        imagePalette = (RgbQuad*) malloc(colors * sizeof (RgbQuad));
        for (int i = 0; i < colors; i++) {
            imagePalette[i] = orig.imagePalette[i];
        }
    } else {
        imagePalette = NULL;
    }

    if (orig.data == NULL) {
        return;
    }

    switch ((int) imageInfoHeader.getBiBitCount()) {
        case 1:
        {
            unsigned int bytes = (unsigned int) ceil(linewidth * height / 8.0);
            data = (unsigned char*) calloc(bytes, 1);

            for (int i = 0; i < bytes; i++) {
                *(data + i) = *(orig.data + i);
            }
        }
            break;

        case 4:
        {
            unsigned int bytes = (unsigned int) ceil(linewidth * height / 2.0);
            data = (unsigned char*) calloc(bytes, 1);
            for (int i = 0; i < bytes; i++) {
                *(data + i) = *(orig.data + i);
            }
        }
            break;
        case 8:
            data = (unsigned char*) calloc(linewidth * height, 1);
            for (int i = 0; i < linewidth * height; i++) {
                *(data + i) = *(orig.data + i);
            }
            break;

        case 16:
            data = (unsigned char*) calloc(linewidth * height, 2);
            for (int i = 0; i < linewidth * height * 2; i++) {
                *(data + i) = *(orig.data + i);
            }
            break;

        case 24:
            data = (unsigned char*) calloc(linewidth * height, 3);
            for (int i = 0; i < linewidth * height * 3; i++) {
                *(data + i) = *(orig.data + i);
            }
            break;

        case 32:
            data = (unsigned char*) calloc(linewidth * height, 4);
            for (int i = 0; i < linewidth * height * 4; i++) {
                *(data + i) = *(orig.data + i);
            }
            break;

        default:
            cout << "[Costruttore di copia]: Errore di lettura, numero di bit per pixel errato." << endl;
    }


}

BmpImage & BmpImage::operator=(const BmpImage &orig) {
    if (this == &orig) {
        return *this;
    }
    width = orig.width;
    height = orig.height;
    linewidth = orig.linewidth;
    imageFileHeader = orig.imageFileHeader;
    imageInfoHeader = orig.imageInfoHeader;
    free(imagePalette);
    if (orig.imagePalette != NULL) {
        unsigned int colors;
        if (imageInfoHeader.getBiClrUsed() == 0) {
            colors = pow(2, (int) imageInfoHeader.getBiBitCount());
        } else {
            colors = imageInfoHeader.getBiClrUsed();
        }
        imagePalette = (RgbQuad*) malloc(colors * sizeof (RgbQuad));
        for (int i = 0; i < colors; i++) {
            imagePalette[i] = orig.imagePalette[i];
        }
    } else {
        imagePalette = NULL;
    }

    free(data);
    switch ((int) imageInfoHeader.getBiBitCount()) {
        case 1:
        {
            unsigned int bytes = (unsigned int) ceil(linewidth * height / 8.0);
            data = (unsigned char*) calloc(bytes, 1);

            for (int i = 0; i < bytes; i++) {
                *(data + i) = *(orig.data + i);
            }

            return *this;
        }

        case 4:
        {
            unsigned int bytes = (unsigned int) ceil(linewidth * height / 2.0);
            data = (unsigned char*) calloc(bytes, 1);
            for (int i = 0; i < bytes; i++) {
                *(data + i) = *(orig.data + i);
            }

            return *this;
        }
        case 8:
            data = (unsigned char*) calloc(linewidth * height, 1);

            for (int i = 0; i < linewidth * height; i++) {
                *(data + i) = *(orig.data + i);
            }

            return *this;
        case 16:
            data = (unsigned char*) calloc(linewidth * height, 2);
            for (int i = 0; i < linewidth * height * 2; i++) {
                *(data + i) = *(orig.data + i);
            }

            return *this;

        case 24:
            data = (unsigned char*) calloc(linewidth * height, 3);
            for (int i = 0; i < linewidth * height * 3; i++) {
                *(data + i) = *(orig.data + i);
            }

            return *this;

        case 32:
            data = (unsigned char*) calloc(linewidth * height, 4);
            for (int i = 0; i < linewidth * height * 4; i++) {
                *(data + i) = *(orig.data + i);
            }

            return *this;


        default:
            cout << "Operator =: Errore di lettura, numero di bit per pixel errato." << endl;
    }

    return *this;
}

unsigned char & BmpImage::operator() (unsigned x, unsigned y) {
    return data[y * linewidth + x];
}

unsigned BmpImage::getWidth() const {
    return width;
}

unsigned BmpImage::getHeight() const {
    return height;
}

unsigned BmpImage::getLinewidth() const {
    return linewidth;
}

unsigned char* BmpImage::getData() {
    return data;
}

unsigned char* BmpImage::getLineptr(unsigned y) {
    return data + linewidth * y;
}

unsigned char* BmpImage::getLineptr(unsigned int y, unsigned char channel) {
    return data + linewidth * y + (channel * linewidth * height);
}

BmpFileHeader BmpImage::getFileHeader() const {
    return imageFileHeader;
}

BmpInfoHeader BmpImage::getInfoHeader() const {
    return imageInfoHeader;
}

RgbQuad* BmpImage::getPalette() {
    return imagePalette;
}

void BmpImage::getPixel(unsigned int x, unsigned int y, unsigned char* rgb) const {
    switch ((int) imageInfoHeader.getBiBitCount()) {
        case 1:
            cout << "getPixel: Errore: valore di bit per pixel non supportato (1)" << endl;
            break;
        case 4:
            cout << "getPixel: Errore: valore di bit per pixel non supportato (4)" << endl;
            break;
        case 8:
            if (imagePalette == NULL) {
                for (int i = 0; i < 3; i++) {
                    rgb[2 - i] = data[y * linewidth + x];
                }
            } else {
                rgb[0] = imagePalette[data[y * linewidth + x ]].getRed();
                rgb[1] = imagePalette[data[y * linewidth + x ]].getGreen();
                rgb[2] = imagePalette[data[y * linewidth + x ]].getBlue();
            }
            break;
        case 24:
            for (int i = 0; i < 3; i++) {
                rgb[2 - i] = data[y * linewidth * 3 + x * 3 + i];
            }
            break;
        case 32:
            for (int i = 0; i < 3; i++) {
                rgb[2 - i] = data[y * linewidth * 4 + x * 4 + i];
            }
            break;
        default:
        {
            cout << "getPixel: Errore: valore di bit per pixel errato ("
                    << dec << (int) imageInfoHeader.getBiBitCount() << ")" << endl;
        }
    }
}

unsigned char BmpImage::getPixel(int x, int y) const {
    return data[y * linewidth + x];
}

void BmpImage::setPixel(unsigned int x, unsigned int y, unsigned char value) {
    data[y * linewidth + x] = value;
}

void BmpImage::setPixel(unsigned int x, unsigned int y, unsigned char value, unsigned char channel) {

    if (x < width && y < height) {
        data[y * linewidth * 3 + x * 3 + 3 - channel] = value;
    }
}

void BmpImage::setPixel(unsigned int x, unsigned int y, unsigned int R, unsigned int G, unsigned int B) {
    if (x < width && y < height) {
        data[y * linewidth * 3 + x * 3 + 0] = B;
        data[y * linewidth * 3 + x * 3 + 1] = G;
        data[y * linewidth * 3 + x * 3 + 2] = R;
    }
}

void BmpImage::setPixel(unsigned int x, unsigned int y, unsigned char* value, unsigned char channels) {
    // Si presume che channels sia tre e che quindi value sia una tripletta RGB
    if ((x < width) && (y < height)) {
        for (int i = 0; i < channels; i++) {
            // BMP usa triplette BGR (non chiedetemi perche')
            data[y * linewidth + x * channels + i] = value[channels - i - 1];
        }
    }
}

void BmpImage::printImagePalette() {
    if (imagePalette == NULL) {
        return;
    }
    cout << endl << "PALETTE" << endl;
    int colors = 0;
    if (imageInfoHeader.getBiClrUsed() == 0) {
        colors = pow(2, imageInfoHeader.getBiBitCount());
    } else {
        colors = imageInfoHeader.getBiClrUsed();
    }
    for (unsigned i = 0; i < colors; i++) {
        cout << setw(3) << i << ": "
                << setw(3) << int(imagePalette[i].getRed()) << ","
                << setw(3) << int(imagePalette[i].getGreen()) << ","
                << setw(3) << int(imagePalette[i].getBlue());

        if ((i + 1) % 4 == 0)
            cout << endl;
        else
            cout << "   ";
    }
}

void BmpImage::printHeader() {
    imageFileHeader.printFileHeader();
    imageInfoHeader.printInfoHeader();
    if (imageInfoHeader.getBiBitCount() <= 8) {
        printImagePalette();
    }
}

bool BmpImage::save(const char* path) {
    ofstream imgFile(path, ios::out | ios::binary);
    imageFileHeader.writeHeader(imgFile);
    imageInfoHeader.writeHeader(imgFile);

    if (imageInfoHeader.getBiBitCount() <= 8) {
        for (int i = 0; i < imageInfoHeader.getBiClrUsed(); i++) {
            imgFile.put((char) imagePalette[i].getRed());
            imgFile.put((char) imagePalette[i].getGreen());
            imgFile.put((char) imagePalette[i].getBlue());
            imgFile.put((char) imagePalette[i].getReserved());
        }
    }
    if (imageInfoHeader.getBiHeight() > 0) {
        for (int y = getHeight() - 1; y >= 0; y--) {
            imgFile.write((char *) getLineptr(y), getLinewidth());
        }
    } else {
        for (int y = 0; y<int(getHeight()); y++)
            imgFile.write((char *) getLineptr(y), getLinewidth());
    }

    imgFile.close();
}
