/* 
 * File:   main.cpp
 * Author: Cam
 *
 * Created on 16 agosto 2010, 16.10
 */

#include <string>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#if 0
using namespace std;

// Calcolo dei coefficienti C
double C(int x) {
    if (x == 0) {
        return 1 / sqrt(2);
    } else {
        return 1;
    }
}

// Valore della trasformata diretta
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

// Valore della trasformata inversa
double inverseDCT(int x, int y, int S[8][8]) {
    double pi = 3.1415926535897932384626433832795;
    double sxy = 0.0;

    for (int v = 0; v < 8; v++) {
        for (int u = 0; u < 8; u++) {
            sxy += C(u) * C(v) * S[u][v] * (cos(((2 * x + 1) * u * pi) / 16.0))*(cos(((2 * y + 1) * v * pi) / 16.0));
        }
    }
    sxy = (1 / 4.0) * sxy;
    return sxy;
}

// Dato uno stream crea la matrice 8 x 8
void getBlockMatrix(istream &in, int blocco[8][8]) {
    int y = 0;
    int x = 0;
    string buffer;
    string bufferLine;
    while (true) {

        int j = 0;
        std::getline(in, bufferLine);
        x = 0;
        while (x < 8) {

            if (bufferLine.c_str()[j] == ' ' || j == bufferLine.size()) {
                blocco[x][y] = atoi(buffer.c_str());
                buffer.clear();
                j++;
                x++;
            } else {
                buffer += bufferLine.c_str()[j];
                j++;
            }
            if (bufferLine.size() < j) {
                break;
            }
        }
        if (++y == 8) {
            break;
        }
    }

}

// Prende i valori da tastiera e li mette nella matrice 8 x 8
void getBlock(int blocco[8][8]) {
    getBlockMatrix(cin, blocco);
}

// Prende la matrice 8 x 8 e crea l'array zig zag
void getZigZagArray(int zigZag[64], int bloccoQuant[8][8]) {

    int j = 0, x = 0, y = 0;
    for (int i = 0; i < 15; i++) {
        while (true) {
            zigZag[j] = bloccoQuant[y][x];
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

// Stampa la matrice su uno stream
void printMatrix(ostream &out, const char* name, int blocco[8][8]) {

    // Stampa del blocco originale
    out << name << ":" << endl;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            out << setw(4) << blocco[x][y] << " ";
        }
        out << endl;
    }
    out << endl;
}

// Stampa a video la matrice
void printMatrix(const char*name, int blocco[8][8]) {
    printMatrix(cout, name, blocco);
}

// Stampa su uno stream i valori JPEG da codificare
void printBlockCode(ostream &out, int zigZag[64]) {
    int sum = 0;
    for (int i = 0; i < 64; i++) {
        sum += abs(zigZag[i]);
    }
    out << "Valori:" << endl;

    for (int i = 0; i < 64; i++) {
        int semisum = 0;

        for (int j = 0; j < i; j++) {
            semisum += abs(zigZag[j]);
        }
        if (sum == semisum) {
            break;
        }
        out << zigZag[i] << " ";
    }
    out << "EOB" << endl;
}

// Stampa a video i valori JPEG da codificare
void printBlockCode(int zigZag[64]) {
    printBlockCode(cout, zigZag);
}


int main(int argc, char** argv) {
    // Bit per pixel
    unsigned int bpp = 8;


    // Inserimento della matrice del blocco da file
    int blocco [8][8];
    fstream in("b.txt", ios::in | ios::binary);

    getBlockMatrix(in, blocco);


    // Inserimento da tastiera:
    // cout << "Digitare i valori della matrice del blocco: << endl;
    // getBlockMatrix(blocco);


    // Stampa a video del blocco originale
    printMatrix("Blocco originale", blocco);


    // Stampa del blocco scalato
    int bloccoScalato[8][8];

    cout << "Blocco scalato:" << endl;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            bloccoScalato[x][y] = blocco[x][y] - (int)pow(2, bpp - 1);
            cout << setw(4) << bloccoScalato[x][y] << " ";
        }
        cout << endl;
    }
    cout << endl;


    // Stampa del blocco trasformato
    int bloccoTrasf[8][8];

    cout << "DCT:" << endl;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            bloccoTrasf[x][y] = (int)round(forwardDCT(x, y, bloccoScalato));
            cout << setw(4) << bloccoTrasf[x][y] << " ";
        }
        cout << endl;
    }
    cout << endl;


    // Stampa del blocco antitrasformato
    int bloccoAntitrasf [8][8];

    cout << "IDCT:" << endl;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            bloccoAntitrasf[x][y] = (int)round(inverseDCT(x, y, bloccoTrasf));
            cout << setw(4) << bloccoAntitrasf[x][y] << " ";
        }
        cout << endl;
    }
    cout << endl;


    // Errore dovuto all'arrotondamento. Blocco scalato meno l'antitrasformato
    cout << "Errore:" << endl;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            cout << setw(4) << bloccoScalato[x][y] - bloccoAntitrasf[x][y] << " ";
        }
        cout << endl;
    }
    cout << endl;

    // Tabella di quantizzazione
    int tabQuant[8][8] = {
        { 27, 20, 23, 23, 30, 40, 81, 120},
        { 18, 20, 22, 28, 37, 58, 106, 153},
        { 17, 23, 27, 37, 61, 91, 129, 158},
        { 27, 32, 40, 48, 93, 106, 144, 163},
        { 40, 43, 66, 85, 113, 134, 171, 186},
        { 66, 96, 95, 144, 181, 173, 201, 166},
        { 85, 100, 115, 133, 171, 188, 199, 171},
        { 101, 91, 93, 103, 128, 153, 168, 164}
    };

    // Tabella di quantizzazione presa da tastiera
    // cout << "Digitare i valori della matrice di quantizzazione: << endl;
    // getBlockMatrix(tabQuant);

    // Tabella di quantizzazione presa da file
    // fstream qin("tabella.txt",ios::in | ios::binary);
    // getBlockMatrix(quin,tabQuant);


    // Stampa a video della tabella di quantizzazione
    printMatrix("Tabella di quantizzazione", tabQuant);


    // Blocco quantizzato
    int bloccoQuant[8][8];

    cout << "Blocco quantizzato:" << endl;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            bloccoQuant[x][y] = (int)round(((float)bloccoTrasf[x][y]) / ((float)tabQuant[x][y]));
            cout << setw(4) << bloccoQuant[x][y] << " ";
        }
        cout << endl;
    }
    cout << endl;


    // Ordinamento zig-zag
    int zigZag[64];

    // Matrice a zigZag di prova per verificare la funzione getZigZagArray()
    //    int zigZagMatrix[8][8] = {
    //        { 0, 2, 3, 9, 10, 20, 21, 35},
    //        { 1, 4, 8, 11, 19, 22, 34, 36},
    //        { 5, 7, 12, 18, 23, 33, 37, 48},
    //        { 6, 13, 17, 24, 32, 38, 47, 49},
    //        { 14, 16, 25, 31, 39, 46, 50, 57},
    //        { 15, 26, 30, 40, 45, 51, 56, 58},
    //        { 27, 29, 41, 44, 52, 55, 59, 62},
    //        { 28, 42, 43, 53, 54, 60, 61, 63}
    //    };

    getZigZagArray(zigZag, bloccoQuant);
    printBlockCode(zigZag);

    return 0;
}



#endif