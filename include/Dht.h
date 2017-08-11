/* 
 * File:   Dht.h
 * Author: .dumbass
 *
 * Created on 30 marzo 2011, 23.40
 */


#ifndef DHT_H
#define	DHT_H
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include "ByteHandler.h"


/* ============================
 * DATI INSERITI DA MARCO
 * ============================
 */

typedef struct ht {
    unsigned char valuesPerLength[16];
    vector<unsigned char> values;
    vector<unsigned short int> codes;
    unsigned int Tc:4;
    unsigned int Th:4;
} HuffmanTable;

using namespace std;

class Dht {
public:
    Dht();
    Dht(const Dht& dh);
    Dht(fstream &fs);
    int Extract(fstream &in);
    void printBinaryCodes();
    void printData();
    vector<HuffmanTable> huffmanTables;
private:







    void createTable();




};

#endif	/* DHT_H */

