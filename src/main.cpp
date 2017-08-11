/* 
 * File:   main.cpp
 * Autore: Cam
 *
 * Creato il 11 marzo 2011, alle 16.12
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <ColorSpaceConverter.h>

#include <JFIFextractor.h>
#include <ByteHandler.h>
#include <EXIFextractor.h>

#include <JpegHeader.hpp>
#include <Dht.h>
#include <BmpImage.h>
#include <BitReader.h>

using namespace std;

#if 0

int main() {
    fstream in("prova", ios::in | ios::binary);
    fstream out("casper", ios::out | ios::binary);
    BitReader bh(in);
    int total = 0;
    int i = 0;
    int nBit = 0;
    while (true) {
        unsigned int data = 9;
        int numbit = rand() % 16 + 1;
        total += numbit;
        if (total > 30 * 8) {
            numbit -= (total - 21 * 8);
            nBit = bh.readBits(in, numbit, data);
            if (nBit != 0) {
                cout << "Data: " << dec << numbit << " " << hex << data << " " << bh.binary(data, 32) << endl;
                out << bh.binary(data, numbit);
                break;
            } else {
                cout << "There is no data into file and the buffer is empty." << endl;
                break;
            }
        }
        nBit = bh.readBits(in, numbit, data);
        cout << "Richiesti " << dec << numbit << ". Letti " << dec << nBit << "." << endl;
        if (nBit != 0) {
            cout << "Data: " << dec << numbit << " " << hex << data << " " << bh.binary(data, 32) << endl;
            out << bh.binary(data, numbit);
            i++;
        } else {
            cout << "There is no data into file and the buffer is empty." << endl;
            break;
        }
    }
    in.close();
    out.close();
}
#endif




// Main di Marco
#if 0

int main(int argc, char** argv) {
#if 0
    BmpImage bmp(10, 20, 24);
    bmp.getFileHeader().printFileHeader();
    bmp.getInfoHeader().printInfoHeader();
    cout << "linea: " << bmp.getLinewidth() << endl;
    cout << "larghezza: " << bmp.getWidth() << endl;
    cout << "altezza: " << bmp.getHeight() << endl;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            unsigned char r[3];
            bmp.getPixel(i, j, r);
            cout << hex << setw(2) << setfill('0') << uppercase << (int) r[0] << (int) r[1] << (int) r[2] << " ";
        }
        cout << endl;
    }
    bmp.save("provacchio.bmp");
    BmpImage bmp2("C:/Users/Cam/Desktop/bmp/test24.bmp");
    bmp2.printHeader();
    cout << "Pixel (10,10): ";
    for (int i = 0; i < 3; i++) {
        unsigned char rgb[3];
        bmp2.getPixel(10, 10, rgb);
        cout << (int) rgb[i] << " ";
    }
    cout << endl;


#endif
    ColorSpaceConverter cs;
    fstream out("index2.txt", ios::out | ios::binary);


    int index[] = {
        0, 1, 5, 6, 14, 15, 27, 28,
        2, 4, 7, 13, 16, 26, 29, 42,
        3, 8, 12, 17, 25, 30, 41, 43,
        9, 11, 18, 24, 31, 40, 44, 53,
        10, 19, 23, 32, 39, 45, 52, 54,
        20, 22, 33, 38, 46, 51, 55, 60,
        21, 34, 37, 47, 50, 56, 59, 61,
        35, 36, 48, 49, 57, 58, 62, 63
    };


    int rindex[] = {
        0, 1, 8, 16, 9, 2, 3, 10,
        17, 24, 32, 25, 18, 11, 4, 5,
        12, 19, 26, 33, 40, 48, 41, 34,
        27, 20, 13, 6, 7, 14, 21, 28,
        35, 42, 49, 56, 57, 50, 43, 36,
        29, 22, 15, 23, 30, 37, 44, 51,
        58, 59, 52, 45, 38, 31, 39, 46,
        53, 60, 61, 54, 47, 55, 62, 63
    };

    int original[] = {
        124, 164, 159, 58, 231, 154, 22, 61,
        111, 97, 150, 44, 250, 181, 67, 117,
        114, 207, 53, 58, 112, 57, 204, 246,
        78, 136, 77, 111, 28, 30, 7, 139,
        130, 89, 120, 79, 66, 76, 237, 133,
        130, 239, 59, 235, 104, 81, 186, 59,
        208, 223, 215, 110, 152, 108, 125, 125,
        203, 140, 50, 47, 67, 130, 148, 159
    };

    int transformed[] = {
        985, 51, 103, 13, -13, -83, -70, 29,
        -47, -41, -102, 14, 22, -133, 66, 63,
        72, 52, -57, 118, -3, -99, 98, 59,
        52, 85, -145, 28, -38, 42, -27, 13,
        -122, -6, 28, 5, 7, 24, 68, -10,
        -3, 110, -8, -76, 13, -60, 1, -36,
        3, -30, 60, 0, 3, 25, -138, -81,
        75, -59, 92, -4, -43, 24, -7, 93
    };

    int antitransformed[] = {
        124, 164, 159, 58, 231, 154, 22, 61,
        111, 97, 151, 44, 250, 181, 67, 117,
        113, 206, 53, 58, 112, 57, 204, 246,
        78, 136, 77, 111, 28, 30, 7, 139,
        130, 89, 120, 79, 66, 76, 237, 133,
        130, 239, 59, 235, 104, 82, 186, 59,
        208, 223, 215, 110, 151, 108, 125, 125,
        203, 140, 50, 47, 67, 130, 148, 159
    };

    int trans[64];
    int antitrans[64];

    int zigzag[64];
    for (int i = 0; i < 64; i++) {
        zigzag[i] = transformed[rindex[i]];
    }


    for (int i = 0; i < 64; i++) {
        antitrans[i] = (int) round(cs.inverseDCT(i % 8, i / 8, zigzag));
    }

    int rgb[] = {123, 200, 50};
    int ycbcr[3];
    cs.rgbToYCbCr(rgb, ycbcr);
    cout << "Y: " << ycbcr[0] << " Cb: " << ycbcr[1] << " Cr: " << ycbcr[2] << endl;
    cs.yCbCrToRGB(ycbcr, rgb);
    cout << "R: " << rgb[0] << " G: " << rgb[1] << " B: " << rgb[2] << endl;


    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            cout << setw(4) << setfill(' ') << antitrans[8 * y + x] - antitransformed[8 * y + x] << "\t";
        }
        cout << endl;
    }

    int imgR[8][8] = {
        {35, 29, 27, 19, 109, 22, 23, 24},
        {55, 27, 29, 23, 106, 24, 23, 23},
        {87, 70, 24, 19, 143, 33, 29, 30},
        {214, 8, 10, 14, 193, 54, 81, 84},
        {249, 2, 4, 22, 212, 103, 158, 169},
        {249, 1, 155, 34, 198, 149, 167, 188},
        {210, 4, 151, 36, 196, 153, 169, 191},
        {122, 12, 6, 21, 218, 109, 192, 214}
    };

    int imgG[8][8] = {
        {98, 106, 90, 80, 215, 219, 218, 218},
        {89, 116, 96, 87, 208, 216, 218, 218},
        {86, 90, 80, 64, 203, 203, 211, 211},
        {18, 211, 36, 26, 197, 173, 145, 168},
        {3, 246, 8, 7, 157, 102, 47, 131},
        {3, 252, 67, 4, 100, 32, 15, 87},
        {20, 234, 68, 3, 98, 29, 11, 82},
        {59, 192, 14, 3, 157, 94, 8, 144}
    };

    int imgB[8][8] = {
        {5, 5, 29, 22, 194, 195, 193, 192},
        {4, 4, 15, 22, 190, 195, 193, 193},
        {34, 26, 56, 15, 198, 197, 193, 191},
        {1, 1, 167, 10, 216, 202, 192, 166},
        {0, 0, 233, 21, 215, 195, 190, 114},
        {0, 0, 71, 35, 199, 173, 172, 117},
        {1, 1, 69, 37, 198, 172, 170, 118},
        {3, 2, 220, 21, 219, 196, 193, 75}
    };

    out << "r = [";
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            out << imgR[i][j] << " ";
        }
        out << ";" << endl;
    }
    out << "]" << endl;

    out << "g = [";
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            out << imgG[i][j] << " ";
        }
        out << ";" << endl;
    }
    out << "]" << endl;

    out << "b = [";
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            out << imgB[i][j] << " ";
        }
        out << ";" << endl;
    }
    out << "]" << endl;


    int imgY[8][8];
    int imgCb[8][8];
    int imgCr[8][8];

    ColorSpaceConverter sc;
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            sc.rgbToYCbCr(imgR[i][j], imgG[i][j], imgB[i][j], imgY[i][j], imgCb[i][j], imgCr[i][j]);
        }
    }

    out << "y = [";
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            out << imgY[i][j] << " ";
        }
        out << ";" << endl;
    }
    out << "]" << endl;

    out << "cb = [";
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            out << imgCb[i][j] << " ";
        }
        out << ";" << endl;
    }
    out << "]" << endl;

    out << "cr = [";
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            out << imgCr[i][j] << " ";
        }
        out << ";" << endl;
    }
    out << "]" << endl;

}

#endif
#if 0
unsigned char R[8][8] = {
    {0xA6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA3},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0x87, 0x3F, 0xFF, 0xFF, 0xFF},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0x4A, 0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBB}
};
unsigned char G[8][8] = {
    {0x12, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00},
    {0x32, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C, 0xFF},
    {0x54, 0x00, 0x00, 0x52, 0x00, 0x00, 0x00, 0x00},
    {0x94, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0xA9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    {0xB3, 0xFF, 0xFF, 0xFF, 0x21, 0xFF, 0xFF, 0xFF},
    {0x00, 0x52, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00},
    {0xFF, 0x90, 0xE8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};
unsigned char B[8][8] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    {0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0x53, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFB, 0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};


int imgR[8][8] = {
    {35, 29, 27, 19, 109, 22, 23, 24},
    {55, 27, 29, 23, 106, 24, 23, 23},
    {87, 70, 24, 19, 143, 33, 29, 30},
    {214, 8, 10, 14, 193, 54, 81, 84},
    {249, 2, 4, 22, 212, 103, 158, 169},
    {249, 1, 155, 34, 198, 149, 167, 188},
    {210, 4, 151, 36, 196, 153, 169, 191},
    {122, 12, 6, 21, 218, 109, 192, 214}
};

int imgG[8][8] = {
    {98, 106, 90, 80, 215, 219, 218, 218},
    {89, 116, 96, 87, 208, 216, 218, 218},
    {86, 90, 80, 64, 203, 203, 211, 211},
    {18, 211, 36, 26, 197, 173, 145, 168},
    {3, 246, 8, 7, 157, 102, 47, 131},
    {3, 252, 67, 4, 100, 32, 15, 87},
    {20, 234, 68, 3, 98, 29, 11, 82},
    {59, 192, 14, 3, 157, 94, 8, 144}
};

int imgB[8][8] = {
    {5, 5, 29, 22, 194, 195, 193, 192},
    {4, 4, 15, 22, 190, 195, 193, 193},
    {34, 26, 56, 15, 198, 197, 193, 191},
    {1, 1, 167, 10, 216, 202, 192, 166},
    {0, 0, 233, 21, 215, 195, 190, 114},
    {0, 0, 71, 35, 199, 173, 172, 117},
    {1, 1, 69, 37, 198, 172, 170, 118},
    {3, 2, 220, 21, 219, 196, 193, 75}
};

int imgY[8][8];
int imgCb[8][8];
int imgCr[8][8];

ColorSpaceConverter sc;
for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
        sc.rgbToYCbCr(imgR[i][j], imgG[i][j], imgB[i][j], imgY[i][j], imgCb[i][j], imgCr[i][j]);
    }
}

//    sc.printMatrix("Matrice R", imgR);
//    sc.printMatrix("Matrice G", imgG);
//    sc.printMatrix("Matrice B", imgB);

int imgRr[8][8];
int imgGr[8][8];
int imgBr[8][8];
//    sc.printMatrix("Matrice R", R);
//    sc.printMatrix("Matrice G", G);
//    sc.printMatrix("Matrice B", B);

for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
        sc.yCbCrToRGB(imgY[i][j], imgCb[i][j], imgCr[i][j], imgRr[i][j], imgGr[i][j], imgBr[i][j]);
    }
}

int diff[8][8];
for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
        diff[i][j] = imgR[i][j] - imgRr[i][j];
    }
}
//    sc.printMatrix("diff R", diff);

for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
        diff[i][j] = imgG[i][j] - imgGr[i][j];
    }
}
//    sc.printMatrix("diff G", diff);
for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
        diff[i][j] = -imgB[i][j] + imgBr[i][j];
    }
}
//    sc.printMatrix("diff B", diff);


fstream out("matrici.txt", ios::out | ios::binary);



//    sc.printMatrix(out, "Matrice Rr", imgRr);
//    sc.printMatrix(out, "Matrice Gr", imgGr);
//    sc.printMatrix(out, "Matrice Br", imgBr);

out.close();

BmpImage img(512, 512, 24);
BmpImage img2("C:/Users/Cam/Desktop/lena.bmp");
cout << "RGB(450,16) = [";
for (int i = 0; i < img.getHeight(); i++) {
    for (int j = 0; j < img.getWidth(); j++) {
        unsigned char rgb[3];
        int brgb[3];
        img2.getPixel(i, j, rgb, 3);


        for (int k = 0; k < 3; k++) {

            rgb[k] % 2 == 0 ? brgb[k] = rgb[k] : brgb[k] = rgb[k] + 1;
            if (i == 450 && j == 16) {
                cout << dec << (int) brgb[k] << " ";

            }
            if (brgb[k] < 0 || brgb[k] > 255) {

                if (brgb[k] < 0) {
                    brgb[k] = 0;
                } else {
                    brgb[k] = 255;
                }

            }
        }

        unsigned char lrgb[3];
        for (int m = 0; m < 3; m++) {
            lrgb[m] = brgb[m];
        }
        img.setPixel(i, j, lrgb, 3);
    }
}
cout << "]" << endl;

img.printHeader();
img.save("C:/Users/Cam/Desktop/prova.bmp");




//    bh.getBig(in, app1);
//    if (app1 != 0xFFE1) {
//        cout << "Errore, marker APP1 non trovato" << endl;
//        return -1;
//    }
//
//    EXIFextractor ee(in);
//    ee.printData();

//je.printData();

//ColorSpaceConverter csc;

//int ycb[3];
//int rgb[3] = {144,141,98};
//csc.rgbToYCbCr(rgb,ycb);

//cout << ycb[0] << "\t" << ycb[1] << "\t" << ycb[2] << endl;
return 0;
}
#endif

#if 0

int main(int argc, char** argv) {


    int width = 17;
    int height = 9;
    // modifica per simulare una immagine di dimensioni arbitrarie
    BmpFileHeader header(width, height, 24);
    BmpInfoHeader infoheader(width, height, 24);
    BmpImage image(width, height, 24);
    image.setFileHeader(header);
    image.setInfoHeader(infoheader);


    unsigned char pixel[3];
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {

            unsigned char rgb[3];
            rgb[0] = i * 15;
            rgb[1] = j * 28;
            rgb[2] = (i * 15 + j*28) / 2;
            
            image.setPixel(i, j, rgb, 3);
        }
    }
    image.save("prova.bmp");

}

#endif


// Main di Alessandro
#if 1

int main(int argc, char** argv) {
    JpegHeader jpgh; // oggetto JpegHeader che esegue l'analisi del file e conterrà tutti i dati dell'header jpeg del file
    fstream file; // file immagine di input
    // se argc è uguale a 1 vuol dire che non è stato inserito alcun parametro. Ma senza nome del file...
    if (argc == 1) {
        cout << "Warning: missing filename." << endl;
        cout << endl;
        cout << "Usage: jpg2bmp <filename>" << endl;
        cout << "This program will be terminated with error." << endl;
        return 1;
    }

    // apriamo il file
    cout << "Attempting to open file " << argv[1] << "...";
    file.open(argv[1], ios_base::in | ios::binary);
    if (!file.is_open()) {
        cout << "[ failed ]" << endl;
        cout << "" << endl;
        cout << "This program will be terminated with error.";
        return 1;
    } else {
        cout << "[ OK ]" << endl;
    }

    jpgh.get_header(file);



}

#endif


// Main di Francesco
#if 0

int main(int argc, char** argv) {
    fstream in("C:/Users/Cam/Desktop/immagini/citta_metro/londra1.jpg", ios::in | ios::binary);

    Dht dh(in);
}
#endif
