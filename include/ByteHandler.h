/* 
 * File:   byteHandler.h
 * Author: cam
 *
 * Created on March 31, 2010, 2:32 PM
 */
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;

#ifndef _BYTEHANDLER_H
#define	_BYTEHANDLER_H

typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned int DWORD;
typedef int LONG;

class ByteHandler {
public:
    ByteHandler();
    bool get(istream &stream, BYTE &b);
    bool getLittle(istream &stream, WORD &w);
    bool getLittle(istream &stream, LONG &l);
    bool getLittle(istream &stream, DWORD &dw);
    bool getBig(istream &stream, WORD &w);
    bool getBig(istream &stream, LONG &l);
    bool getBig(istream &stream, DWORD &dw);
    void wordBuffer(WORD littleWord, char* array);
    void dwordBuffer(DWORD littledword, char* array);
    void longBuffer(LONG littlelong, char* array);

    // Alessandro Benisi
    bool getByte(fstream &file, BYTE &b);
    bool getWord(fstream &file, WORD &w);
    BYTE takeByteFromWord(bool h_l, WORD w);
    WORD makeWord(BYTE high, BYTE low);

private:

};

void splitByte(BYTE b, BYTE &high, BYTE &low);
void splitByte(BYTE b, bool &high, BYTE &low);

#endif	/* _BYTEHANDLER_H */

