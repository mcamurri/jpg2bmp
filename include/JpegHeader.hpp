#include <string>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include "ByteHandler.h"
#include "BitReader.h"
#include "Dht.h"
#include "BmpFileHeader.h"
#include "BmpInfoHeader.h"
#include <sstream>
#include <iostream>


using namespace std;

#ifndef JPEGHEADER_HPP
#define	JPEGHEADER_HPP


// Marker tipo C
#define DHT 0xC4 /**< Define Huffman Table*/
#define JPG 0xC8 /**< JPG Reserved*/
#define DAC 0xCC /**< Define Arithmetic Coding*/
#define SOF 0xC0
#define SOF0 0x0 /**< DCT di tipo Baseline */ 
#define SOF1 0x1 /**< DCT di tipo Extended Sequential */
#define SOF2 0x2 /**< DCT di tipo Progressive*/ 
#define SOF3 0x3 /**< DCT di tipo Lossless*/ 
#define SOF5 0x5 /**< DCT di tipo Differential sequential*/ 
#define SOF6 0x6 /**< DCT di tipo Differential progressive*/ 
#define SOF7 0x7 /**< DCT di tipo Differential lossless*/ 
#define SOF9 0x9 /**< DCT di tipo Arithmetic Extended Sequential*/ 
#define SOF10 0xA /**< DCT di tipo Arithmetic Progressive*/ 
#define SOF11 0xB /**< DCT di tipo Arithmetic Lossless*/ 
#define SOF13 0xD /**< DCT di tipo Arithmetic Differential Sequential*/ 
#define SOF14 0xE /**< DCT di tipo Arithmetic Differential Progressive*/ 
#define SOF15 0xF /**< DCT di tipo Arithmetic Differential Lossless*/ 

// Marker tipo D
#define SOI 0xD8
#define EOI 0xD9
#define SOS 0xDA
#define DQT 0xDB
#define DNL 0xDC
#define DRI 0xDD
#define DHP 0xDE
#define EXP 0xDF

// Marker tipo E
#define APP 0xE0
#define APP0 0xE0
#define APP1 0xE1

// Marker tipo F
#define COM 0xFE


 struct ScanHeader {
    unsigned char Ns;
    unsigned char* Cs;
    unsigned char* Td;
    unsigned char* Ta;
    unsigned char Ss;
    unsigned char Se;
    unsigned int Ah:4;
    unsigned int Al:4;
};

 struct ArithmeticTable {
    BYTE Tc;
    BYTE Tb;
    BYTE C;
};

 struct quant_table {
    bool pq; // precisione della tabella di quantizzazione
    unsigned int tq:4; // identificazione di destinazione
    WORD t[64]; // elementi della tabella
};

 struct sof_header {
    BYTE sof_type; // sof number type
    WORD length; // lunghezza header
    BYTE P;
    WORD Y;
    WORD X;
    BYTE Nf;
    BYTE *C;
    BYTE *H;
    BYTE *V;
    BYTE *Tq;
};

class JpegHeader {
private:

    // componenti header jpeg
    quant_table* qt; // tabelle di quantizzazione (a seconda della destinazione numerate da 0 a 3)
    unsigned char maxQuantIndex; // numero di tabelle di quantizzazione a disposizione dal decoder
    sof_header sofh;
    ArithmeticTable* at;
    ScanHeader sh;
    ByteHandler bh;
    bool verify_jpeg(fstream &file);
    void load_quantization_tables(fstream &file);
    int load_sofn_header(sof_header& sof, fstream& file);
    void build_streamdata(istream &file, stringstream &strmdata);
    Dht* tabelle;

    BmpFileHeader* outFileHeader;
    BmpInfoHeader* outInfoHeader;

    // Restart Interval
    unsigned short int Ri;
    // Number of Lines (specificato nel marker DNL)
    unsigned short int NL;
    // Espansione orizzontale e verticale delle componenti
    //  a partire dal prossimo frame
    unsigned char EhEv;

    void decodeMCU(int col, int row, fstream &log, fstream &fs, BitReader &br, int*mcu, int*lastDC);


public:
    JpegHeader();
    ~JpegHeader();
    int get_header(fstream &file);
    int getTable(fstream &fs);
    void printDetailed(int k, int comp, unsigned short int leadingzeros, fstream &log, fstream &file,BitReader &br);
    void printDetailed(int k, int comp, unsigned short int leadingzeros, fstream &log, fstream &file, BitReader &br, bool isEOB);

};

#endif	/* JPEGHEADER_HPP */

