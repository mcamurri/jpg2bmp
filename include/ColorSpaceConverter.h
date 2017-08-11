/* 
 * File:   colorSpaceConverter.h
 * Author: Cam
 *
 * Created on 16 marzo 2011, 10.42
 */
#include <ostream>
#include <iostream>
#include <iomanip>
#include <math.h>

#ifndef COLORSPACECONVERTER_H
#define	COLORSPACECONVERTER_H

using namespace std;

class ColorSpaceConverter {
public:
    ColorSpaceConverter();
    void rgbToYCbCr(int* RGB, int* YCbCr);
    void yCbCrToRGB(int* YCbCr, int* RGB);
    void rgbToYCbCr(int R, int G, int B, int &Y, int &Cb, int &Cr);
    void yCbCrToRGB(int Y, int Cb, int Cr, int &R, int &G, int &B);
    void yCbCrToRGB(int Y, int Cb, int Cr, unsigned char*RGB);
    void yCbCrToRGB(int*YCbCr, unsigned char* RGB);
    unsigned char trim(int value);
    double inverseDCT(int x, int y, int S[8][8]);
    double C(unsigned char x);

    template <typename T1> void printMatrix(const char*name, T1 blocco[8][8]) {

        // Stampa del blocco originale
        cout << name << ":" << endl;
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                cout << setw(4) << setfill(' ') << dec << (int) blocco[x][y] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    void printMatrix(ostream &out, const char* name, int blocco[8][8]) {
        // Stampa del blocco originale
        out << name << ":" << endl;
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                out << setw(4) << setfill(' ') << dec << blocco[x][y] << " ";
            }
            out << endl;
        }
        out << endl;
    }
    double inverseDCT(int x, int y, int S[64]);
    int inverseDCT(unsigned char x, unsigned char y, int* S);

    double forwardDCT(int u, int v, int s[8][8]) {
        double pi = 3.1415926535897932384626433832795;
        double Suv = 0.0;

        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                Suv += s[x][y] * (cos(((2 * x + 1) * u * pi) / 16.0))*(cos(((2 * y + 1) * v * pi) / 16.0));
            }
        }

        Suv = (1 / 4.0) * C(u) * C(v) * Suv;
        return Suv;
    }

    int getZigZagIndex(unsigned int px, unsigned int py) {

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

        return index[py * 8 + px];
    }

    int getReverseIndex(int index) {
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
        int rrindex[] = {
            0, 1, 5, 6, 14, 15, 27, 28,
            2, 4, 7, 13, 16, 26, 29, 42,
            3, 8, 12, 17, 25, 30, 41, 43,
            9, 11, 18, 24, 31, 40, 44, 53,
            10, 19, 23, 32, 39, 45, 52, 54,
            20, 22, 33, 38, 46, 51, 55, 60,
            21, 34, 37, 47, 50, 56, 59, 61,
            35, 36, 48, 49, 57, 58, 62, 63
        };
        return rindex[index];
    }

    double inverseDCT(int index, int S[64]);

    template <typename T1, typename T2> void getMatrixFromZZ(T1 bloccoQuant[8][8], T2 zigZag[64]) {

        int j = 0, x = 0, y = 0;
        for (int i = 0; i < 15; i++) {
            while (true) {

                bloccoQuant[y][x] = zigZag[j];
                j++;

                if (i % 2 == 0) {
                    if (y == 7) {
                        x++;
                    } else {
                        if (y < 7) {
                            y++;
                        }
                        if (x > 0) {
                            x--;
                        }
                    }
                } else {
                    if (x == 7) {
                        y++;
                    } else {
                        if (y > 0) {
                            y--;
                        }
                        if (x < 7) {
                            x++;
                        }
                    }
                }
                if (x + y == i + 1) {
                    break;
                }
            }
        }


    }
private:

};

#endif	/* COLORSPACECONVERTER_H */

