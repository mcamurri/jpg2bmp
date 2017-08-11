/* 
 * File:   Dht.cpp
 * Author: .dumbass
 * 
 * Created on 30 marzo 2011, 23.40
 */

#include <Dht.h>
#include <BitReader.h>

Dht::Dht() {

}

//Dht::~Dht() {
//
//    for (int i = 0; i < huffmanTables.size(); i++) {
//        free(huffmanTables[i].codes);
//        free(huffmanTables[i].values);
//    }
//
//}
//

Dht::Dht(const Dht& dh) {
    for (int i = 0; i < dh.huffmanTables.size(); i++) {

        HuffmanTable ht;
        ht.Tc = dh.huffmanTables[i].Tc;
        ht.Th = dh.huffmanTables[i].Th;
        for (int j = 0; i < dh.huffmanTables[i].values.size(); j++) {
            ht.values.push_back(dh.huffmanTables[i].values[j]);
        }
        for (int j = 0; i < dh.huffmanTables[i].codes.size(); j++) {
            ht.values.push_back(dh.huffmanTables[i].codes[j]);
        }
        huffmanTables.push_back(ht);
    }
}

Dht::Dht(fstream &fs) {
    Extract(fs);
}

//Metodo che estrae la DHT

int Dht::Extract(fstream& in) {


    ByteHandler bh; //Classe per la lettura dei valori da file

    BYTE byte; //Byte di appoggio per la lettura


    /**
     * Leggo i valori finchè non raggiungo la fine della tabella
     */
    unsigned short int length;
    unsigned int pos = in.tellg();
    bh.getBig(in, length);

    unsigned int byteLetti = 2;

    //cout << hex << (int)Lh;
    while (byteLetti < (int)length) {

        HuffmanTable temp;

        bh.get(in, byte);
        byteLetti++;
        temp.Tc = (byte & 0xF0) >> 4;
        temp.Th = (byte & 0x0F);
        //coefficienti Y DC
        unsigned int valuesNumber = 0;

        /**
         * Nel primo for leggo il numero dei valori di ogni lunghezza e nel
         * secondo passo a leggere i valori.
         **/
        for (int i = 0; i < 16; i++) {
            bh.get(in, byte);
            byteLetti++;
            //added by Marco
            temp.valuesPerLength[i] = byte;
        }


        for (int i = 0; i < 16; i++) {
            valuesNumber += temp.valuesPerLength[i];
        }



        unsigned int k = 0;
        for (int i = 0; i < 16; i++) {
            // added by Marco
            int n = temp.valuesPerLength[i];

            for (int j = 0; j < n; j++) {
                bh.get(in, byte);
                byteLetti++;
                // added by Marco
                temp.values.push_back(byte);

                k++;

            }
        }

        unsigned short int baseValue = 0;
        k = 0;
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < temp.valuesPerLength[i]; j++) {
                temp.codes.push_back(baseValue++);
                k++;
            }
            baseValue <<= 1;
        }
        // riciclo k
        for (k = 0; k < huffmanTables.size(); k++) {
            if (huffmanTables[k].Th == temp.Th && huffmanTables[k].Tc == temp.Tc) {
                huffmanTables[k] = temp;
                break;
            }
        }
        if (k == huffmanTables.size()) {
            huffmanTables.push_back(temp);
        }
    }

    // Ridimensiono l'array in modo che occupi lo spazio strettamente necessario.
    huffmanTables.resize(huffmanTables.size());

    in.seekg(pos + length, ios::beg);
}

void Dht::printData() {

    for (int l = 0; l < huffmanTables.size(); l++) {
        int valuesNumber = 0;
        for (int i = 0; i < 16; i++) {
            valuesNumber += huffmanTables[l].valuesPerLength[i];
        }

        cout << "Lunghezze" << endl;
        for (int i = 0; i < 16; i++) {
            cout << "lunghezza " << i << ": " << uppercase << dec << (unsigned short int)huffmanTables[l].valuesPerLength[i] << endl;
        }
        cout << "Simboli" << endl;
        for (int i = 0; i < valuesNumber; i++) {
            cout << "Simbolo " << dec << i << ": " << setw(2) << setfill('0') << uppercase << hex << (unsigned short int)huffmanTables[l].values[i] << endl;
        }
    }
}

void Dht::printBinaryCodes() {
    fstream out("codes.txt", ios::out | ios::binary);

    for (int l = 0; l < huffmanTables.size(); l++) {

        out << "Codici (DC, destin 0):" << endl;
        unsigned int valuesNumber = 0;
        for (int i = 0; i < 16; i++) {
            valuesNumber += huffmanTables[l].valuesPerLength[i];
        }
        unsigned int counter = 0;
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < huffmanTables[l].valuesPerLength[i]; j++) {
                out << "Codice " << counter << ": ";
                for (int k = i; k > -1; k--) {
                    switch ((huffmanTables[l].codes[counter] >> k) & 0x0001) {
                        case 0x0000:
                            out << "0";
                            break;
                        case 0x0001:
                            out << "1";
                            break;
                    }
                }
                out << endl;
                counter++;
            }
        }
    }
    out.close();

}
