/* 
 * File:   BitWriter.h
 * Author: Cam
 *
 * Created on 10 settembre 2010, 16.34
 */
#include <fstream>

using namespace std;

#ifndef BITWRITER_H
#define	BITWRITER_H

class BitWriter {
public:
    unsigned char byBuffer;
    int iPos;

    BitWriter();
    BitWriter(const BitWriter& orig);

    /**
     * Scrive un bit sullo stream assegnato. Viene usata da Write.
     * @param out Lo stream su cui scrivere
     * @param uData L'intero che racchiude il bit da scrivere
     */
    void WriteBit(fstream &out, unsigned uData);

    /**
     * Scrive un numero arbitrario di bit su uno stream.
     * @param out Lo stream da utiliare per la scrittura
     * @param uData Il dato che racchiude i bit da scrivere
     * @param iNumBits Il numero di bit da scrivere
     */
    void Write(fstream &out, unsigned uData, int iNumBits);

    void Flush(fstream &out);
};

#endif	/* BITWRITER_H */

