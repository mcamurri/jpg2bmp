#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>

#ifndef JFIFEXTRACTOR_H
#define	JFIFEXTRACTOR_H

using namespace std;

class JFIFextractor {
public:
    JFIFextractor(fstream &fs);
    void printData();



private:
    unsigned short int length;
    unsigned char version[2];
    unsigned char units;
    unsigned short int Xdensity;
    unsigned short int Ydensity;
    unsigned char tw;
    unsigned char th;
};

#endif	/* JFIFEXTRACTOR_H */

