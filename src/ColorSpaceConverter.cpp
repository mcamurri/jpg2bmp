/* 
 * File:   colorSpaceConverter.cpp
 * Author: Cam
 * 
 * Created on 16 marzo 2011, 10.42
 */

#include <ColorSpaceConverter.h>
#include <math.h>

ColorSpaceConverter::ColorSpaceConverter() {
}

void ColorSpaceConverter::rgbToYCbCr(int* RGB, int*YCbCr) {


    YCbCr[0] = (int)round(0.299 * RGB[0] + 0.587 * RGB[1] + 0.114 * RGB[2]);
    YCbCr[1] = (int)round(-0.1687 * RGB[0] - 0.3313 * RGB[1] + 0.5 * RGB[2] + 128);
    YCbCr[2] = (int)round(0.5 * RGB[0] - 0.4187 * RGB[1] - 0.0813 * RGB[2] + 128);

    return;
}

void ColorSpaceConverter::rgbToYCbCr(int R, int G, int B, int &Y, int &Cb, int &Cr) {


    Y = (int)round(0.299 * R + 0.587 * G + 0.114 * B);
    Cb = (int)round(128.0 - (0.168736 * R) -(0.331264 * G) + (0.5 * B));
    Cr = (int)round((0.5 * R) - 0.418688 * G - 0.081312 * B + 128.0);

    return;
}

void ColorSpaceConverter::yCbCrToRGB(int* YCbCr, int* RGB) {


    RGB[0] = (int)round(YCbCr[0] + 1.402 * (YCbCr[2] - 128));
    RGB[1] = (int)round(YCbCr[0] - 0.34414 * (YCbCr[1] - 128) - 0.71414 * (YCbCr[2] - 128));
    RGB[2] = (int)round(YCbCr[0] + 1.772 * (YCbCr[1] - 128));

    return;
}

void ColorSpaceConverter::yCbCrToRGB(int*YCbCr, unsigned char* RGB){
    
        for (int i = 0; i < 3; i++) {
        int value = 0;
        switch (i) {
            case 0:
                value = (int)round(YCbCr[0] + 1.402 * (YCbCr[2] - 128));
                break;
            case 1:
                value = (int)round(YCbCr[0] - 0.34414 * (YCbCr[1] - 128) - 0.71414 * (YCbCr[2] - 128));
                break;
            case 2:
                value = (int)round(YCbCr[0] + 1.772 * (YCbCr[1] - 128));
                break;
        }
        RGB[i] = trim(value);
    }
        if((RGB[0] <= 0) && (RGB[1] <= 0) && (RGB[2] <= 0)){
            cout << "CAZZOO" << (int)RGB[0] << " " << (int)RGB[1] << " " << (int)RGB[2] << endl;
        }

    return;
    
}

unsigned char ColorSpaceConverter::trim(int value) {
    if (value < 0)
        return 0;

    if (value > 255)
        return 255;
    return (unsigned char)value;
}

void ColorSpaceConverter::yCbCrToRGB(int Y, int Cb, int Cr, unsigned char* RGB) {

    for (int i = 0; i < 3; i++) {
        int value = 0;
        switch (i) {
            case 0:
                value = (int)round(Y + 1.402 * (Cr - 128));
                break;
            case 1:
                value = (int)round(Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128));
                break;
            case 2:
                value = (int)round(Y + 1.772 * (Cb - 128));
                break;
        }
        RGB[i] = trim(value);
    }

    return;
}


void ColorSpaceConverter::yCbCrToRGB(int Y, int Cb, int Cr, int &R, int &G, int &B) {


    R = (int)round(Y + 1.402 * (Cr - 128));
    G = (int)round(Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128));
    B = (int)round(Y + 1.772 * (Cb - 128));

    if (R < 0) {
        R = 0;
    }
    if (G < 0) {
        G = 0;
    }
    if (B < 0) {
        B = 0;
    }
    if (R > 255) {
        R = 255;
    }
    if (G > 255) {
        G = 255;
    }
    if (B > 255) {
        B = 255;
    }

    return;
}

// Calcolo dei coefficienti C

double ColorSpaceConverter::C(unsigned char x) {
    if (x == 0) {
        return (1.0 / sqrt(2.0));
    } else {
        return 1.0;
    }
}

// Valore della trasformata inversa

double ColorSpaceConverter::inverseDCT(int x, int y, int S[8][8]) {
    double pi = 3.1415926535897932384626433832795;
    double sxy = 0.0;


    return sxy;
}

double ColorSpaceConverter::inverseDCT(int x, int y, int* S) {
    double pi = 3.1415926535897932384626433832795;
    double sxy = 0.0;

    unsigned char rindex[] = {
        0, 1, 8, 16, 9, 2, 3, 10,
        17, 24, 32, 25, 18, 11, 4, 5,
        12, 19, 26, 33, 40, 48, 41, 34,
        27, 20, 13, 6, 7, 14, 21, 28,
        35, 42, 49, 56, 57, 50, 43, 36,
        29, 22, 15, 23, 30, 37, 44, 51,
        58, 59, 52, 45, 38, 31, 39, 46,
        53, 60, 61, 54, 47, 55, 62, 63
    };

    for (unsigned char i = 0; i < 64; i++) {
        unsigned char u = rindex[i] % 8;
        unsigned char v = rindex[i] / 8;
        sxy += C(u) * C(v) * (*(S + i)) * (cos(((2 * x + 1) * u * pi) / 16.0))*(cos(((2 * y + 1) * v * pi) / 16.0));

    }
    sxy = (1.0 / 4.0) * sxy;
    return sxy;
}

int ColorSpaceConverter::inverseDCT(unsigned char x, unsigned char y, int* S) {
    double pi = 3.1415926535897932384626433832795;
    double sxy = 0.0;

    unsigned char rindex[] = {
        0, 1, 8, 16, 9, 2, 3, 10,
        17, 24, 32, 25, 18, 11, 4, 5,
        12, 19, 26, 33, 40, 48, 41, 34,
        27, 20, 13, 6, 7, 14, 21, 28,
        35, 42, 49, 56, 57, 50, 43, 36,
        29, 22, 15, 23, 30, 37, 44, 51,
        58, 59, 52, 45, 38, 31, 39, 46,
        53, 60, 61, 54, 47, 55, 62, 63
    };

    for (unsigned char i = 0; i < 64; i++) {
        unsigned char u = rindex[i] % 8;
        unsigned char v = rindex[i] / 8;
        sxy += C(u) * C(v) * (*(S + i)) * (cos(((2 * x + 1) * u * pi) / 16.0))*(cos(((2 * y + 1) * v * pi) / 16.0));

    }
    sxy = (1.0 / 4.0) * sxy;
    return (int)round(sxy);
}

double ColorSpaceConverter::inverseDCT(int index, int S[64]) {
    double pi = 3.1415926535897932384626433832795;
    double sxy = 0.0;

    return sxy;
}


// Stampa la matrice su uno stream



// Stampa a video la matrice




