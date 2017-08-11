/* 
 * File:   BitWriter.cpp
 * Author: Cam
 * 
 * Created on 10 settembre 2010, 16.34
 */

#include <BitWriter.h>

BitWriter::BitWriter() : byBuffer(0), iPos(0) {
}

BitWriter::BitWriter(const BitWriter& orig) {
    byBuffer = orig.byBuffer;
    iPos = orig.iPos;
}

void BitWriter::WriteBit(fstream &out, unsigned uData) {
    byBuffer = (byBuffer << 1) | (uData & 1);
    iPos++;
    if (iPos == 8) {
        out.put(byBuffer);
        iPos = 0;
    }
}

void BitWriter::Write(fstream &out, unsigned uData, int iNumBits) {
    while (iNumBits > 0) {
        iNumBits--;
        WriteBit(out, uData >> iNumBits);
    }
}

void BitWriter::Flush(fstream &out) {
    while (iPos > 0)
        WriteBit(out, 0);
}
