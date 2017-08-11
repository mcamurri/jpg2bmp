/* 
 * File:   JFIFextractor.cpp
 * Author: Cam
 * 
 * Created on 11 marzo 2011, 16.23
 */

#include <JFIFextractor.h>
#include <ByteHandler.h>

/**
 * Estrattore di header del formato di interscambio JFIF e delle informazioni
 * in esso contenute.
 * @param fs
 * lo stream dell'immagine da cui trarre gli header JFIF
 */
JFIFextractor::JFIFextractor(fstream &fs) {
    // byteHandler recupera i dati con le dovute conversioni tra little e big endian
    ByteHandler bh;
    
    // Lettura della lunghezza dell'header
    int pos = fs.tellg();
    bh.getBig(fs, length);



    // Lettura dell'identificatore dell'header
    unsigned int identifier;

    bh.getBig(fs, identifier);

    if (identifier == 0x4A464946) {
        fs.get();

        cout << hex << uppercase << identifier << endl;

        // Lettura del numero di versione
        bh.get(fs, version[0]);
        bh.get(fs, version[1]);

        bh.get(fs, units);

        bh.getBig(fs, Xdensity);
        bh.getBig(fs, Ydensity);

        bh.get(fs, tw);
        bh.get(fs, th);
    }



    fs.seekg(pos + length, ios::beg);
}

void JFIFextractor::printData() {


    cout << "Versione JFIF: " << (int)version[0] << "." << setw(2) << setfill('0') << (int)version[1] << endl;

    cout << "Lunghezza dell'header: " << length << " byte" << endl;

    cout << "Risoluzione: " << dec << Xdensity << " x " << Ydensity;
    switch (units) {
        case 0:
            cout << endl;
            break;
        case 1:
            cout << " pixel per pollice" << endl;
            break;
        case 2:
            cout << " pixel per centimetro" << endl;
            break;
        default:
            cout << "Errore in lettura della densità di pixel" << endl;
            break;
    }
    if (tw != 0 && th != 0) {
        cout << "Thumbnail: " << (int)tw << " x " << (int)th << " pixel" << endl;
    }

}

