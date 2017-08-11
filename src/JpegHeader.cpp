#include <JpegHeader.hpp>
#include <ByteHandler.h>
#include <JFIFextractor.h>
#include <EXIFextractor.h>
#include <ColorSpaceConverter.h>
#include <Dht.h>
#include <BitReader.h>
#include <BmpImage.h>

using namespace std;

/**
 * Funzione per verificare che l'immagine data in input al programm sia realmente un'immagine JPEG.
 * @param file immagine su cui effettuare la verifica
 * @return true se è un jpeg, false in caso contrario
 * @author Alessandro Benisi
 */
bool JpegHeader::verify_jpeg(fstream &file) {

    BYTE buf[2]; // buffer per verifica intestazione jpeg
    int i;
    // prelevo i dati per la verifica
    for (i = 0; i < 2; i++) buf[i] = file.get();
    if ((buf[0] != 0xFF) || (buf[1] != SOI)) {
        cout << "E' un file jpeg?" << endl;
        return false;
    }
    return true;
}

JpegHeader::~JpegHeader() {
    free(sh.Cs);
    sh.Cs = NULL;
    free(sh.Td);
    sh.Td = NULL;
    free(sh.Ta);
    sh.Ta = NULL;
    free(qt);
    qt = NULL;
    free(at);
    at = NULL;
    delete tabelle;

}

/**
 * Metodo per la costruzione di uno stream contenente solo la parte dati del SOS Marker
 * @param file file di input da cui leggere. Deve essere un fstream già aperto e il cursore di lettura già sul primo byte della parte dati
 * @param strm_data stream di uscita con la sola parte dati
 * @author Alessandro Benisi
 */
void JpegHeader::build_streamdata(istream &file, stringstream & strm_data) {
    char c;
    try {
        while (!file.eof()) {
            file.get(c);
            if ((unsigned char) c == 0xFF) {
                char d;
                file.get(d);
                if (d == 0x00) {
                    strm_data.put(c);
                    continue;
                } else {
                    file.seekg(-2, ios::cur);
                    break;
                }
            }
            strm_data.put(c);
        }
    } catch (exception &e) {
        cout << "The following error occurred while reading file or write data stream." << endl;
        cerr << e.what() << endl;
        exit(1);
    }
}

/**
 * Metodo per il caricamento delle tabelle di quantizzazione nelle rispettive strutture
 * @param file
 * @author Alessandro Benisi
 */
void JpegHeader::load_quantization_tables(fstream & file) {

    unsigned short int length;

    BYTE b;

    ByteHandler bh;
    int pos = file.tellg();
    // prelevo la lunghezza della definizione della tabella di quantizzazione
    bh.getWord(file, length);

    // flag per sapere se c'è bisogno di riallocare lo spazio
    bool reall = false;

    int tabQuantNumber = 0;
    while (file.tellg() < (pos + length)) {
        unsigned char b;
        bh.get(file, b);
        if (maxQuantIndex < (b & 0x0F)) {
            reall = true;
            maxQuantIndex = (b & 0x0F);
        }
        switch (b & 0xF0) {
            case 0x10:
                file.seekg(64 * 2, ios::cur);
                break;
            case 0x00:
                file.seekg(64, ios::cur);
        }
        tabQuantNumber++;
    }
    file.seekg((pos + 2), ios::beg);
    // Il controllo su qt copre il caso in cui siamo alla prima tabella trovata
    // in questo caso realloc si comporta come una malloc
    if (reall || qt == NULL) {
        qt = (quant_table*) realloc(qt, (maxQuantIndex + 1) * sizeof (quant_table));
    }
    int j = 0;
    while (j < tabQuantNumber) {
        bh.getByte(file, b);

        int k = b & 0x0F;
        qt[k].pq = b >> 4;
        qt[k].tq = (unsigned char) k;


        for (int i = 0; i < 64; i++) {
            // se pq è uguale a 0 allora la precisione è di 8 bit...
            if (qt[k].pq == 0) {
                BYTE b_;
                bh.getByte(file, b_);
                WORD v = bh.makeWord(0x00, b_);
                qt[k].t[i] = v;

            } else {
                bh.getBig(file, qt[k].t[i]);
            }
        }
        int block[8][8];
        ColorSpaceConverter cs;

        cs.getMatrixFromZZ<int, WORD > (block, qt[k].t);
        stringstream ss;
        ss << "Tabella di quantizzazione " << j;

        string s = ss.str();

        cs.printMatrix<int>(s.c_str(), block);
        j++;
    }
    file.seekg((length + pos), ios::beg);
}

/**
 * Metodo per la lettura degli header SOF. Verifica anche il tipo di SOF fra baseline, sequential, progressive, lossless. Infine convalida i valori rilevati dal file, verificando che siano compliance con lo Standard JPEG.
 * @param sof struct su cui depositare i dati dell'header sof
 * @param file immagine da cui leggere l'header sof
 * @return 
 * @author Alessandro Benisi
 */
int JpegHeader::load_sofn_header(sof_header& sof, fstream & file) {
    bool check; // per verificare la corretta lettura da file
    bool is_baseline = sof.sof_type == SOF0;
    bool is_sequential = (sof.sof_type == SOF1) || (sof.sof_type == SOF5) || (sof.sof_type == SOF9) || (sof.sof_type == SOF13);
    bool is_progressive = (sof.sof_type == SOF2) || (sof.sof_type == SOF6) || (sof.sof_type == SOF10) || (sof.sof_type == SOF14);
    bool is_lossless = (sof.sof_type == SOF3) || (sof.sof_type == SOF7) || (sof.sof_type == SOF11) || (sof.sof_type == SOF15);
    BYTE b;
    int pos = file.tellg();
    // prelevo la lunghezza dell'header
    check = bh.getWord(file, sof.length);

    // controllo P
    bh.get(file, b);
    if (is_baseline) {
        if (b == 8) {
            sof.P = b;
        } else {
            cout << "Error in JpegHeader::load_sofn_header." << endl;
            cout << "P value is not compliance with Jpeg Standard" << endl;
            return -1;
        }
    } else if (is_sequential || is_progressive) {
        if ((b = 8) || (b == 12)) {
            sof.P = b;
        } else {
            cout << "Error in JpegHeader::load_sofn_header." << endl;
            cout << "P value is not compliance with Jpeg Standard" << endl;
            return -1;
        }
    } else if (is_lossless) {
        if ((b >= 2) && (b <= 16)) {
            sof.P = b;
        } else {
            cout << "Error in JpegHeader::load_sofn_header." << endl;
            cout << "P value is not compliance with Jpeg Standard" << endl;
            return -1;
        }
    }

    bh.getWord(file, sof.Y);
    bh.getWord(file, sof.X);

    check = bh.getByte(file, b);

    // controllo i valori per Nf
    if ((sof.sof_type == SOF0) || is_sequential || is_lossless) {
        // basta controllare che sia maggiore di 0, essendo un solo byte
        // non serve controllare sia minore di 256 (by Marco)
        if (b > 0) {
            sof.Nf = b;
        } else {
            cout << "Error in JpegHeader::load_sofn_header." << endl;
            cout << "Nf value is not compliance with Jpeg Standard" << endl;
            return -1;
        }
    } else {
        if ((b > 0) && (b < 5)) {
            sof.Nf = b;
        } else {
            cout << "Error in JpegHeader::load_sofn_header." << endl;
            cout << "Nf value is not compliance with Jpeg Standard" << endl;
            return -1;
        }
    }
    /*
    if (sof.length != (8 + 3 * sof.Nf)) {
        cout << "Dimensione di Nf scorretta, aborting" << endl;
        return -1;
    }

     */

    //  Parte vecchia di Marco
    sof.C = (BYTE*) malloc(((int) sof.Nf));
    sof.H = (BYTE*) malloc(((int) sof.Nf));
    sof.V = (BYTE*) malloc(((int) sof.Nf));
    sof.Tq = (BYTE*) malloc(((int) sof.Nf));

    int i;
    // ad ogni ciclo leggo Ci, Hi, Vi, Tqi
    for (i = 0; i < sof.Nf; i++) {
        // leggo e controllo C
        bh.get(file, b);
        if ((b >= 0) && (b <= 255)) {
            sof.C[i] = b;
        } else {
            cout << "Error in JpegHeader::load_sofn_header." << endl;
            cout << "C value is not compliance with Jpeg Standard" << endl;
            return -1;
        }

        // leggo e controllo H
        // H e V sono da 4 bit, quindi leggo un byte e lo uso per entrambi
        // (by Marco)
        bh.get(file, b);
        if (((b >> 4) > 0) && ((b >> 4) < 5)) {
            sof.H[i] = (b >> 4);
        } else {
            cout << "Error in JpegHeader::load_sofn_header." << endl;
            cout << "H value is not compliance with Jpeg Standard" << endl;
            return -1;
        }

        // leggo e controllo V

        if (((b & 0x0F) > 0) && ((b & 0x0F) < 5)) {
            sof.V[i] = (b & 0x0F);
        } else {
            cout << "Error in JpegHeader::load_sofn_header." << endl;
            cout << "V value is not compliance with Jpeg Standard" << endl;
            return -1;
        }

        // leggo e controllo Tq
        bh.get(file, b);
        if (is_baseline || is_sequential || is_progressive) {
            if ((b >= 0) && (b <= 3)) {
                sof.Tq[i] = b;
            } else {
                cout << "Error in JpegHeader::load_sofn_header." << endl;
                cout << "Tq value is not compliance with Jpeg Standard" << endl;
                return -1;
            }
        } else if (is_lossless) {
            if (b == 0) {
                sof.Tq[i] = b;
            } else {
                cout << "Error in JpegHeader::load_sofn_header." << endl;
                cout << "Tq value is not compliance with Jpeg Standard" << endl;
                return -1;
            }
        }
    }

    outInfoHeader = new BmpInfoHeader((int) sof.X, (int) sof.Y);

    switch ((unsigned int) sof.Nf * sof.P) {
        case 1:
            break;
        case 4:
            break;
        case 8:
            break;
        case 16:
            break;
        case 24:
            break;
        case 32:
            break;
        default:
            cout << "Il formato di immagine JPEG ha una profondita' colore";
            cout << " non supportata dallo standard BMP." << endl;
            cout << "Il programma verra' ora terminato" << endl;
            return -1;
    }

    outInfoHeader->setBiBitCount(sof.Nf * sof.P);
    outFileHeader = new BmpFileHeader(sof.X, sof.Y);


    // In realtà non dovrebbe essere normale se la testina non è in fondo al
    // marker quindi lo segnalo (by Marco)
    if (file.tellg() != (pos + sof.length)) {
        cout << "Warning: truncating SOF marker << endl" << endl;
        file.seekg(pos + sof.length, ios::beg);
    }

    return 0;
}

// Costruttore

JpegHeader::JpegHeader() : outFileHeader(NULL), outInfoHeader(NULL), Ri(0), NL(0), EhEv(0), at(NULL), qt(NULL), maxQuantIndex(0), tabelle(NULL) {

    sh.Cs = NULL;
    sh.Ta = NULL;
    sh.Td = NULL;
    sofh.C = NULL;
    sofh.H = NULL;
    sofh.Tq = NULL;
    sofh.V = NULL;

}

/**
 * Questo metodo fa partire tutto il processo di analisi degli header jpeg.
 * @author Alessandro Benisi, Marco Camurri
 * @param file
 * @return 
 */
int JpegHeader::get_header(fstream & file) {
    BYTE b;
    bool SOS_found;
    bool eof = false;
    //verifico che sia un file jpeg
    if (!verify_jpeg(file)) {
        return -1;
    }

    //ora che ho verificato che il file sia (almeno dichiarato) un jpeg procedo a catturare i marker

    // comincio a prelevare i marker jpeg
    SOS_found = false;

    while (!eof && !SOS_found) {
        eof = bh.getByte(file, b);
        int pos = file.tellg();
        // se il byte corrente non ha valore 0xFF allora posso continuare
        if ((b != 0xFF) || eof) {
            continue;
        }

        // altrimenti subito dopo potrei trovare un marker..............
        eof = bh.getByte(file, b);


        switch (b & 0xF0) {
            case 0xC0:
            {
                switch (b) {
                        // DHT
                    case DHT:
                    {
                        cout << "Trovata la tabella di Huffman" << endl;

                        if (tabelle == NULL) {
                            tabelle = new Dht(file);
                        } else {
                            (*tabelle).Extract(file);
                        }
                        cout << (*tabelle).huffmanTables.size() << endl;

                        //                        tabelle.printData();
                        //                        tabelle.printBinaryCodes();
                    }
                        break;
                        // JPGreserved
                    case JPG:
                    {
                        cout << "JPG reserved marker found. Skipping..." << endl;
                        // Se individuo un marker sconosciuto, lo salto immediatamente
                        unsigned short int length;
                        bh.getBig(file, length);
                        file.seekg(length - 2, ios::cur);
                    }
                        break;
                        // Arithmetic code ceccavolitioning
                    case DAC:
                    {
                        cout << "Arithmetic Coding conditioning marker found." << endl;
                        unsigned short int length;
                        unsigned int pos = file.tellg();
                        bh.getBig(file, length);
                        if (length % 2 == 0) {
                            unsigned short int num = (length - 2) / 2;
                            at = (ArithmeticTable*) malloc(num * sizeof (ArithmeticTable));
                            for (int i = 0; i < num; i++) {
                                unsigned char temp;
                                bh.get(file, temp);

                                at[i].Tb = (temp & 0x0F);
                                at[i].Tc = ((temp & 0xF0) >> 4);
                                bh.get(file, at[i].C);
                                if (((at[i].C == 0) || (at[i].C > 63)) && at[i].Tc == 1) {
                                    cout << "Errore: valore di C inatteso ("
                                            << (int) at[i].C << "),"
                                            << " dovrebbe essere tra 1 e 63" << endl;
                                } else if (at[i].Tc == 0) {
                                    unsigned char U = at[i].C >> 4;
                                    unsigned char L = at[i].C & 0x0F;

                                    if (!((L >= 0) && (L <= U) && (U <= 15))) {
                                        cout << "Errore: valore di C inatteso ("
                                                << (int) at[i].C << "),"
                                                << " dovrebbe essere tra 1 e 63" << endl;
                                    } else if ((at[i].C) != (L * 16 * U)) {
                                        cout << "Errore: valore di C inatteso ("
                                                << (int) at[i].C << "),"
                                                << " dovrebbe essere tra 1 e 63" << endl;
                                    }
                                }
                            }
                        } else {
                            cout << "Errore nel recupero del DAC" << endl;
                        }
                        file.seekg(pos + length, ios::beg);
                    }
                        break;
                        // SOF di un qualche tipo.
                    default:
                    {
                        sofh.sof_type = (b & 0x0F);
                        // carico l'header
                        int r = load_sofn_header(sofh, file);
                        if (r == -1) {
                            cout << "SOF header is not compliance with Jpeg Standard" << endl;
                            cout << "The program will be closed" << endl;
                            return -1;
                        } else if (r == 1) {
                            cout << "File jpeg non supportato." << endl;
                            return 1;
                        }
                    }
                }
                cout << "Dati raccolti da SOF:" << endl;
                cout << "X: " << dec << (int) sofh.X << endl;
                cout << "Y: " << dec << (int) sofh.Y << endl;
                cout << "Nf: " << dec << (int) sofh.Nf << endl;
                cout << "P: " << dec << (int) sofh.P << endl;
                cout << "C: ";
                for (int i = 0; i < sofh.Nf; i++) {
                    cout << dec << (unsigned int) sofh.C[i] << " ";
                }
                cout << endl << "H: ";
                for (int i = 0; i < sofh.Nf; i++) {
                    cout << dec << (unsigned int) sofh.H[i] << " ";
                }
                cout << endl << "V: ";
                for (int i = 0; i < sofh.Nf; i++) {
                    cout << dec << (unsigned int) sofh.V[i] << " ";
                }
                cout << endl << "Tq: ";
                for (int i = 0; i < sofh.Nf; i++) {
                    cout << dec << (unsigned int) sofh.Tq[i] << " ";
                }
                cout << endl << endl;
            }
                break;
                // APP marker (gestisco solo il tipo 0 e 1)
            case APP:
            {
                switch (b) {

                    case APP0:
                    {
                        cout << "APP0 marker found." << endl;
                        JFIFextractor je(file);

                        je.printData();
                    }
                        break;

                    case APP1:
                    {
                        cout << endl << endl << "APP1 marker found." << endl;
                        EXIFextractor ee(file);
                    }
                        break;

                    default:
                    {
                        cout << endl << endl << "APP" << hex << uppercase
                                << (int) (b & 0x0F) << " marker found."
                                << " Skipping..." << endl;
                        // Se individuo un marker sconosciuto, lo salto immediatamente
                        unsigned short int length;
                        bh.getBig(file, length);
                        file.seekg(length - 2, ios::cur);
                    }
                }
            }

                break;
            case 0xF0:
            {
                switch (b) {
                    case COM:
                    {
                        cout << "Comment found. Skipping..." << endl;
                        unsigned short int length;
                        bh.getBig(file, length);
                        file.seekg(length - 2, ios::cur);
                    }
                        break;
                        // Il famoso marker 0xFFFF, non devo fare nulla.
                    case 0x0F:
                        break;
                    default:
                    {
                        cout << "JPEG reserved extension found. Skipping..." << endl;
                        unsigned short int length;
                        bh.getBig(file, length);
                        file.seekg(length - 2, ios::cur);
                    }
                }
            }
                break;

            case 0xD0:
            {
                switch (b) {
                    case DQT:
                    {
                        cout << "Trovata la tabella di quantizzazione" << endl;
                        load_quantization_tables(file);
                    }
                        break;
                    case SOS:
                    {
                        cout << endl << "Trovato Start of Scan" << endl;
                        unsigned short int length;
                        unsigned int pos = file.tellg();
                        bh.getBig(file, length);

                        bh.get(file, sh.Ns);
                        free(sh.Cs);
                        sh.Cs = NULL;
                        free(sh.Ta);
                        sh.Ta = NULL;
                        free(sh.Td);
                        sh.Td = NULL;
                        sh.Cs = (unsigned char*) malloc((int) sh.Ns);
                        sh.Ta = (unsigned char*) malloc((int) sh.Ns);
                        sh.Td = (unsigned char*) malloc((int) sh.Ns);
                        for (int i = 0; i < sh.Ns; i++) {
                            unsigned char b;
                            bh.get(file, b);
                            sh.Cs[i] = b;
                            bh.get(file, b);
                            sh.Td[i] = ((b & 0xF0) >> 4);
                            sh.Ta[i] = (b & 0x0F);
                        }
                        bh.get(file, sh.Ss);
                        bh.get(file, sh.Se);
                        {
                            unsigned char b;
                            bh.get(file, b);
                            sh.Ah = (b & 0xF0) >> 4;
                            sh.Al = (b & 0x0F);
                        }

                        if (file.tellg() != pos + length) {
                            cout << "Warning: truncating SOS" << endl;
                            file.seekg(pos + length, ios::beg);
                        }


                        // Dimensioni reali
                        int width = sofh.X;
                        int height = sofh.Y;
                        // modifica per simulare una immagine di dimensioni arbitrarie
                        BmpFileHeader header(width, height, 24);
                        BmpInfoHeader infoheader(width, height, 24);
                        BmpImage image(width, height, sh.Ns * 8);
                        image.setFileHeader(header);
                        image.setInfoHeader(infoheader);



                        int* lastDC = (int*) calloc((int) sh.Ns, sizeof (int));
                        // Per ogni macroriga dell'immagine
                        BitReader br(file);
                        fstream log("small.log", ios::out | ios::binary);

                        unsigned char Hmax = 0;
                        unsigned char Vmax = 0;
                        for (unsigned char i = 0; i < sh.Ns; i++) {
                            if (Hmax < sofh.H[i]) {
                                Hmax = sofh.H[i];
                            }
                            if (Vmax < sofh.V[i]) {
                                Vmax = sofh.V[i];
                            }
                        }
                        unsigned int linewidth = (width + (8 * Hmax - 1)) / (8 * Hmax);
                        unsigned int lineheight = (height + (8 * Vmax - 1)) / (8 * Vmax);

                        unsigned char space = 0;
                        for (int k = 0; k < sofh.Nf; k++) {
                            space += sofh.H[k] * sofh.V[k];
                        }
                        // Creo un array con le componenti messe in fila e zigzag


                        for (unsigned int j = 0; j < lineheight; j++) {
                            // Per ogni macrocolonna dell'immagine
                            for (unsigned int i = 0; i < linewidth; i++) {

                                int* mcu = (int*) calloc(64 * space, (sizeof (int)));
                                int* dmcu = (int*) calloc(64 * space, sizeof (int));


                                log << endl;
                                if (Ri != 0 && !(i == 0 && j == 0)) {
                                    if ((i + j * linewidth) % Ri == 0) {
                                        for (int h = 0; h < sh.Ns; h++) {
                                            lastDC[h] = 0;
                                        }
                                        br.restart(file);
                                    }
                                }
                                decodeMCU(i, j, log, file, br, mcu, lastDC);


                                ColorSpaceConverter cs;


                                for (unsigned char k = 0; k < sh.Ns; k++) {
                                    int t = 0;
                                    for (char v = k - 1; v >= 0; v--) {
                                        t += sofh.V[v] * sofh.H[v];
                                    }
                                    for (unsigned char l = 0; l < sofh.V[k]; l++) {
                                        for (unsigned char p = 0; p < sofh.H[k]; p++) {
                                            for (unsigned char w = 0; w < 64; w++) {
                                                unsigned char x = w % 8;
                                                unsigned char y = w / 8;

                                                log << "dmcu[" << (int) ((sofh.H[k] * l + p + t)*64) + w << "] = " << (int) cs.inverseDCT(x, y, (mcu + (sofh.H[k] * l + p + t)*64)) + 128 << endl;


                                                dmcu[((sofh.H[k] * l + p + t)*64) + w] = cs.inverseDCT(x, y, (mcu + (sofh.H[k] * l + p + t)*64)) + 128;
                                            }
                                        }
                                    }
                                }

                                for (int l = 0; l < Vmax; l++) {
                                    for (int p = 0; p < Hmax; p++) {

                                        for (int w = 0; w < 64; w++) {
                                            int destination[3];

                                            for (int k = 0; k < sh.Ns; k++) {

                                                int t = 0;
                                                for (char v = k - 1; v >= 0; v--) {
                                                    t += sofh.V[v] * sofh.H[v];
                                                }

                                                int x = (((w % 8) + 8 * p) * sofh.H[k]) / Hmax;
                                                int y = (((w / 8) + 8 * l) * sofh.V[k]) / Vmax;
                                                log << "destination[" << (int) k << "] = dmcu[" << t * 64 + (x % 8) + 64 * (x / 8) + 64 * Hmax * (y / 8) + (y % 8) * 8 << "] \t";
                                                log << " x: " << x;
                                                log << " y: " << y;
                                                log << "\t controllo: (x, y) = (" << ((x % 8) + 64 * (x / 8) + 64 * Hmax * (y / 8) + (y % 8) * 8) % 8;
                                                log << ", " << ((x % 8) + 64 * (x / 8) + 64 * Hmax * (y / 8) + (y % 8) * 8) / 8 << ")" << endl;
                                                destination[k] = dmcu[t * 64 + (x % 8) + 64 * (x / 8) + 64 * Hmax * (y / 8) + (y % 8) * 8];

                                            }
                                            log << "setting pixel (" << (w % 8) + i * 8 * Hmax + p * 8 << ", " << (w / 8) + j * 8 * Vmax + l * 8 << ")";

                                            unsigned char RGB[3];
                                            for (int ii = 0; ii < 3; ii++) {
                                                RGB[ii] = 0;
                                            }
                                            cs.yCbCrToRGB(destination, RGB);
                                            log << " to " << (int) RGB[0] << " " << (int) RGB[1] << " " << (int) RGB[2] << endl << endl;
                                            // il mini array non serve piu', dealloco

                                            // setto il pixel dell'immagine
                                            image.setPixel((w % 8) + i * 8 * Hmax + p * 8, (w / 8) + j * 8 * Vmax + l * 8, RGB, 3);
                                        }
                                    }
                                }
                                free(mcu);
                                mcu = NULL;
                                free(dmcu);
                                dmcu = NULL;
                            }
                        }


                        log.close();


                        free(lastDC);
                        lastDC = NULL;
                        image.save("fake.bmp");

                    }

                        break;
                    case EOI:
                    {
                        free(qt);
                        qt = NULL;

                        cout << "End of file reached." << endl;

                        return 0;
                    }

                        break;
                    case DNL:
                    {
                        cout << "Define number of lines marker found." << endl;
                        unsigned short int length;
                        unsigned int pos = file.tellg();
                        bh.getBig(file, length);
                        bh.getBig(file, NL);
                        if (file.tellg() != pos + length) {
                            cout << "Warning: truncating DNL marker" << endl;
                            file.seekg(pos + length, ios::beg);
                        }
                    }
                        break;
                    case DRI:
                    {
                        cout << "Define Restart Interval marker found." << endl;
                        unsigned short int length;
                        unsigned int pos = file.tellg();
                        bh.getBig(file, length);
                        bh.getBig(file, Ri);
                        if (file.tellg() != pos + length) {
                            cout << "Warning: truncating DRI marker" << endl;
                            file.seekg(pos + length, ios::beg);
                        }

                    }
                        break;
                    case DHP:
                    {
                        cout << "Define hierarchical progression marker found." << endl;
                        cout << "Skipping (for now)..." << endl;
                        unsigned short int length;
                        unsigned int pos = file.tellg();
                        bh.getBig(file, length);
                        // Questo marker dovrebbe sostituire i SOFn marker
                        // bisogna inventarsi qualcosa qua
                        file.seekg(pos + length, ios::beg);

                    }
                        break;
                    case EXP:
                    {
                        cout << "Expand reference components marker found." << endl;
                        unsigned short int length;
                        unsigned int pos = file.tellg();
                        bh.getBig(file, length);
                        bh.get(file, EhEv);

                        if ((EhEv & 0x0F) > 1 || ((EhEv & 0xF0) >> 4) > 1) {
                            cout << "Error (EXP marker): valori non validi" << endl;
                        }
                        file.seekg(pos + length, ios::beg);
                    }
                        break;

                    default:
                    {
                        unsigned short int length;
                        unsigned int pos = file.tellg();
                        bh.getBig(file, length);
                        file.seekg(pos + length, ios::beg);
                    }
                }
            }
                break;

            default:
            {

                unsigned short int length;
                unsigned int pos = file.tellg();
                bh.getBig(file, length);
                file.seekg(pos + length, ios::beg);
            }
                break;

        }
    }
}

void JpegHeader::printDetailed(int k, int comp, unsigned short int leadingzeros, fstream &log, fstream &file, BitReader & br) {

    printDetailed(k, comp, leadingzeros, log, file, br, false);
}

void JpegHeader::printDetailed(int k, int comp, unsigned short int leadingzeros, fstream &log, fstream &file, BitReader &br, bool isEOB) {
    log << "      ["
            << br.printFilePos(file) << "]:"
            << " ZRL=[";
    if (isEOB) {
        log << " 0";
    } else {
        log << setw(2) << setfill(' ') << leadingzeros;
    }
    log << "]"
            << " Val=[" << setw(5) << setfill(' ') << comp << "]"
            << " Coef=[";
    if (!isEOB) {
        if (k == 0) {
            log << "00= DC]";
        } else {
            log << setw(2) << setfill('0') << (k - leadingzeros)
                    << ".." << setw(2) << setfill('0') << k << "]";
        }
    } else {
        log << setw(2) << setfill('0') << (k - leadingzeros)
                << ".." << setw(2) << setfill('0') << (k - leadingzeros) << "]";
    }
    log << " Data=" << br.printBuffer();
    if (isEOB) {
        log << " EOB" << endl;
    } else {

        log << endl;
    }
}

void JpegHeader::decodeMCU(int col, int row, fstream &log, fstream &in, BitReader &br, int* mcu, int* lastDC) {

    int iDC = -1; //indice della tabella di huffman dei coefficienti DC
    int iAC = -1; //indice della tabella di huffman dei coefficienti AC
    int nBit = 0; //numero di bit da leggere
    int nBitApp = 0; //numero di bit da leggere per l'append
    int kCodes = 0; //contatore dei codici di Huffman
    int maxCodes = 0; //massimo indice del contatore dei codici di huffman
    unsigned int appVal; //bit da mettere in append al valore già letto
    unsigned int val = 0; //valore letto del bitreader
    unsigned char decodedVal; //codice di huffman decodificato
    unsigned int comp; //componente letto dal file 
    bool searchCode = true; //indica che si sta cercando il valore tra quelli di lunghezza uguale



    for (int j = 0; j < sh.Ns; j++) {
        int t = 0;
        for (int v = j - 1; v >= 0; v--) {
            t += sofh.V[v] * sofh.H[v];
        }

        // per ogni componente della scan
        //prima di tutto seleziono le tabelle di huffman che verranno utilizzate
        for (int i = 0; i < (*tabelle).huffmanTables.size(); i++) {
            if ((*tabelle).huffmanTables[i].Tc == 0) {
                //tabella DC
                if (sh.Td[j] == (*tabelle).huffmanTables[i].Th)
                    iDC = i;
            } else {
                //tabella AC
                if (sh.Ta[j] == (*tabelle).huffmanTables[i].Th)
                    iAC = i;
            }
        }

        if (iDC < 0)
            cout << "Error (decodeMCU): nessuna tabella entropica DC trovata" << endl;
        if (iAC < 0)
            cout << "Error (decodeMCU): nessuna tabella entropica AC trovata" << endl;


        for (int l = 0; l < sofh.H[j]; l++) {
            for (int p = 0; p < sofh.V[j]; p++) {
                switch (j) {
                    case 0:
                        log << "    Lum (Tbl #0), MCU=[" << col << "," << row << "]" << endl;
                        break;
                    case 1:
                    case 2:
                        log << "    Chr(0) (Tbl #1), MCU=[" << col << "," << row << "]" << endl;
                        break;
                }

                //ciclo i componenti dell'MCU
                for (int k = 0; (k < 64); k++) {

                    unsigned short int leadingzeros = 0;

                    //inizializzo i valori
                    nBit = 0;
                    val = 0;
                    kCodes = 0;
                    maxCodes = 0;
                    if (k == 0) { //il primo valore è il componente DC
                        //trovo il numero di bit da leggere
                        while ((*tabelle).huffmanTables[iDC].valuesPerLength[nBit] == 0) {
                            nBit++;
                        }
                        maxCodes += (*tabelle).huffmanTables[iDC].valuesPerLength[nBit];
                        br.readBits(in, nBit + 1, val, true);
                        //codice per ricercare un valore nella tabella di huffman
                        searchCode = true;
                        while (searchCode) {
                            if (((*tabelle).huffmanTables[iDC].codes[kCodes] == val) && (kCodes < maxCodes) && (kCodes >= (maxCodes - (int) (*tabelle).huffmanTables[iDC].valuesPerLength[nBit]))) {
                                //recupero il valore corrispondente al codice
                                decodedVal = (*tabelle).huffmanTables[iDC].values[kCodes];

                                if (decodedVal == 0) {
                                    //se il valore corrisponde al codice di EOB non leggo nessun bit successivo

                                    mcu[(sofh.V[j] * l + p + t)*64 + k] = lastDC[j] * qt[sofh.Tq[j]].t[k];
                                    printDetailed(k, decodedVal, leadingzeros, log, in, br);

                                } else {
                                    //se il valore letto corrisponde al kCodes-esimo codice nella tabella di huffman
                                    //e l'indice del codice è tra quelli che hanno lunghezza nBit
                                    //allora leggo i "decodedVal" bit successivi
                                    comp = 0;
                                    br.readBits(in, (int) decodedVal, comp);
                                    if (((comp << (32 - decodedVal)) >> 31) == 0) {
                                        //se inizia per 0 lo nego
                                        int comp1 = comp;
                                        comp = (~(comp << (32 - decodedVal))) >> (32 - decodedVal);
                                        //salvo il valore e tolgo la codifica per differenza
                                        mcu[(sofh.V[j] * l + p + t)*64 + k] = ((-(int) comp) + lastDC[j]) * qt[sofh.Tq[j]].t[k];
                                        lastDC[j] = ((-(int) comp) + lastDC[j]);

                                        printDetailed(k, (-(int) comp), leadingzeros, log, in, br);

                                    } else {
                                        //salvo il valore e tolgo la codifica per differenza
                                        mcu[(sofh.V[j] * l + p + t)*64 + k] = (((int) comp) + lastDC[j]) * qt[sofh.Tq[j]].t[k];
                                        printDetailed(k, (int) comp, leadingzeros, log, in, br);
                                        lastDC[j] = (((int) comp) + lastDC[j]);
                                    }
                                }
                                //termino il ciclo
                                searchCode = false;
                            } else {
                                //se il valore non corrisponde
                                if (kCodes == maxCodes) {
                                    //se ho letto tutti i codici di lunghezza nBit e non ho trovato nessun valore
                                    //allora determino quanti bit leggere in piu
                                    nBitApp = 1;
                                    while ((*tabelle).huffmanTables[iDC].valuesPerLength[nBit + nBitApp] == 0) {
                                        nBitApp++;
                                    }
                                    br.readBits(in, nBitApp, val);
                                    //aggiungo i bit letti al valore
                                    nBit += nBitApp;
                                    maxCodes += (*tabelle).huffmanTables[iDC].valuesPerLength[nBit];
                                } else {
                                    //se non ho letto tutti i codici di lunghezza nBit incremento il contatore                            
                                    kCodes++;
                                }
                            }

                        }
                        //passo al componente successivo (che sarà un AC)
                    } else {
                        //valori AC
                        //trovo il numero di bit da leggere
                        while ((*tabelle).huffmanTables[iAC].valuesPerLength[nBit] == 0) {
                            nBit++;
                        }
                        maxCodes += (*tabelle).huffmanTables[iAC].valuesPerLength[nBit];

                        br.readBits(in, nBit + 1, val, true);
                        //codice per ricercare un valore nella tabella di huffman
                        searchCode = true;
                        while (searchCode) {
                            if (((*tabelle).huffmanTables[iAC].codes[kCodes] == val) && (kCodes < maxCodes) && (kCodes >= (maxCodes - (*tabelle).huffmanTables[iAC].valuesPerLength[nBit]))) {
                                //se il valore letto corrisponde all'entry kCodes-esima nella tabella di huffman
                                //e l'indice del codice è tra quelli che hanno lunghezza nBit
                                //allora procedo con la decodifica
                                decodedVal = (*tabelle).huffmanTables[iAC].values[kCodes];
                                //                        cout << "decodedVal: " << (int)decodedVal << endl;
                                //controllo se comp corrisponde ad:
                                //->un valore
                                //->EOB
                                //->ZRL
                                if (decodedVal == 0) {
                                    //------>EOB

                                    for (; k < 64; k++) {
                                        mcu[(sofh.V[j] * l + p + t)*64 + k] = 0;
                                        leadingzeros++;
                                    }
                                    printDetailed(k, 0, leadingzeros, log, in, br, true);
                                } else if (decodedVal == 0xF0) {
                                    //------>ZRL
                                    for (int count = 0; count < 15; count++) {
                                        mcu[(sofh.V[j] * l + p + t)*64 + k] = 0;
                                        k++;
                                        leadingzeros++;
                                    }
                                    printDetailed(k, 0, leadingzeros, log, in, br);
                                } else {
                                    //------>Valore
                                    //Aggiungo gli zeri dovuti al RLE
                                    for (int count = 0; count < (decodedVal >> 4); count++) {
                                        mcu[(sofh.V[j] * l + p + t)*64 + k] = 0;

                                        k++;
                                        leadingzeros++;
                                    }
                                    //tengo i 4 bit meno significativi e leggo i decodedVal bit successivi
                                    decodedVal = (decodedVal & 0x0F);
                                    comp = 0;

                                    br.readBits(in, (int) decodedVal, comp);
                                    if (((comp << (32 - (int) decodedVal)) >> 31) == 0) {
                                        //se inizia per 0 lo nego
                                        int comp1 = comp;
                                        comp = (~(comp << (32 - (int) decodedVal))) >> (32 - (int) decodedVal);
                                        //salvo il valore
                                        mcu[(sofh.V[j] * l + p + t)*64 + k] = (-(int) comp) * qt[sofh.Tq[j]].t[k];

                                        printDetailed(k, (-(int) comp), leadingzeros, log, in, br);
                                    } else {
                                        //salvo il valore
                                        mcu[(sofh.V[j] * l + p + t)*64 + k] = comp * qt[sofh.Tq[j]].t[k];
                                        printDetailed(k, (int) comp, leadingzeros, log, in, br);
                                    }

                                }
                                //termino il ciclo
                                searchCode = false;
                            } else {
                                //se il valore non corrisponde
                                if (kCodes == maxCodes) {
                                    //se ho letto tutti i codici di lunghezza nBit e non ho trovato nessun valore
                                    //allora determino quanti bit leggere in piu
                                    appVal = 0;
                                    nBitApp = 1;
                                    while ((*tabelle).huffmanTables[iAC].valuesPerLength[nBit + nBitApp] == 0) {
                                        nBitApp++;
                                    }
                                    br.readBits(in, nBitApp, val);
                                    //aggiungo i bit letti al valore
                                    nBit += nBitApp;
                                    maxCodes += (*tabelle).huffmanTables[iAC].valuesPerLength[nBit];
                                } else {
                                    //se non ho letto tutti i codici di lunghezza nBit incremento il contatore
                                    kCodes++;
                                }
                            }
                        }
                    }
                }
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
                log << "                      DCT Matrix=";
                for (int v = 0; v < 8; v++) {
                    if (v != 0) {
                        log << "                                 [";
                    } else {
                        log << "[";
                    }
                    for (int w = 0; w < 8; w++) {
                        if (w != 0) {
                            log << " ";
                        }
                        log << setw(5) << setfill(' ') << mcu[index[v * 8 + w] + (l * sofh.H[j] + p + t) * 64];
                    }
                    log << "]" << endl;
                }
                log << endl;
            }
        }
    }
}
