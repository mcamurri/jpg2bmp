#include <vector>
#include <sstream>
#include <iomanip>
#include <BitReader.h>

BitReader::BitReader(fstream &strm) : bitPos(0), buffer(0), startPos(0), endPos(0), the_end(false) {
    stuffbytes = strm.tellg();
    int i = 0;
    char c;
    try {
        while ((i < sizeof (unsigned int)) && !strm.eof() && !the_end) {
            strm.get(c);
            if ((unsigned char) c == 0xFF) {
                char d;
                strm.get(d);
                if ((unsigned char) d == 0x00) {
                    buffer = (buffer << 8) | (unsigned char) c;
                    i++;
                    continue;
                }
            }
            buffer = (buffer << 8) | (unsigned char) c;
            i++;
        }
    } catch (fstream::failure e) {
        cout << "Costruttore(): Errore, lettura dati fallita" << endl;
    }
}

void BitReader::restart(fstream &strm) {
    int nbyte = ((endPos / 8) + 3);
    int i = 0;
    char c = 0;
    while ((i < nbyte) && !strm.eof()) {
        strm.get(c);
        
        if ((unsigned char) c == 0xFF) {
            char d;
            strm.get(d);
            if ((unsigned char) d == 0x00) {
                buffer = (buffer << 8) | (unsigned char) c;
                stuffbytes++;
                i++;
                continue;
            } else {
                strm.seekg(-1, ios::cur);
            }
        }
        buffer = (buffer << 8) | (unsigned char) c;
        stuffbytes++;
        i++;
    }
    bitPos = 0;
    startPos = 0;
    endPos = 0;
    the_end = false;
}

void BitReader::refresh(fstream &strm) {

    if (startPos < 8) {
        return;
    }
    unsigned int nbyte = startPos / 8;

    char c;
    int i = 0;
    try {
        while ((i < nbyte) && !strm.eof()) {
            strm.get(c);
            
            if ((unsigned char) c == 0xFF) {
                char d;
                strm.get(d);
                if ((unsigned char) d == 0x00) {
                    buffer = (buffer << 8) | (unsigned char) c;
                    stuffbytes++;
                    i++;
                    
                    continue;
                } else
                    if ((((unsigned char) d & 0xF0) == 0xD0) && (((unsigned char) d & 0x0F) < 0x08)) {
                    buffer <<= 8;
                    buffer |= 0x000000FF;
                    stuffbytes++;
                    strm.seekg(-1, ios::cur);
                    i++;
                    continue;
                } else {
                    the_end = true;
                    strm.seekg(-2, ios::cur);
                    cout << "BitReader(): Errore, finiti i dati della scan" << endl;
                    stuffbytes += 2;
                    buffer = (buffer << 16) & 0xFFFF0000;


                    bitPos -= 8 * i + 16;
                    startPos -= 8 * i + 16;
                    endPos -= 8 * i + 16;
                    return;
                }
            }
            buffer = (buffer << 8) | (unsigned char) c;
            stuffbytes++;
            i++;
            
        }
    } catch (fstream::failure e) {
        cout << "flush(): Errore di lettura dello stream" << endl;
    }
    bitPos -= 8 * i;
    startPos -= 8 * i;
    endPos -= 8 * i;  
}

int BitReader::readBits(fstream &strm, int numbit, unsigned int& data) {

    if ((sizeof (buffer) *8 - 1) - bitPos < numbit) {
        unsigned int temp = (sizeof (buffer) *8) - bitPos;
        data = (data << temp) | ((buffer << bitPos) >> (sizeof (buffer) *8 - temp));
        bitPos += temp;
        return temp;
    }
    endPos += numbit;
    data = (data << numbit) | ((buffer << bitPos) >> (sizeof (buffer) *8 - numbit));
    bitPos += numbit;

    return numbit;

}

int BitReader::readBits(fstream &strm, int numbit, unsigned int& data, bool isNewCode) {

    if (isNewCode) {        
        if (endPos == 0) {
            startPos = endPos;
        } else {
            startPos = endPos + 1;
        }
        endPos = startPos + numbit - 1;
        refresh(strm);
    } else {
        endPos = startPos + numbit - 1;
    }
    if ((sizeof (buffer) *8 - 1) - bitPos < numbit) {
        unsigned int temp = (sizeof (buffer) *8) - bitPos;
        data = (data << temp) | ((buffer << bitPos) >> (sizeof (buffer) *8 - temp));
        bitPos += temp;
        return temp;
    }

    data = (data << numbit) | ((buffer << bitPos) >> (sizeof (buffer) *8 - numbit));
    bitPos += numbit;

    return numbit;

}

int BitReader::readBits(fstream &strm, int numbit, vector<bool>& data) {
    unsigned int mydata;
    int nbit = readBits(strm, numbit, mydata);
    int i = 0;
    while (nbit > 0) {
        bool b = (mydata >> (nbit - i)) & 1;
        data.push_back(b);
        i++;
    }
    return nbit;
}

string BitReader::binary(unsigned int value, unsigned int numbit) {
    ostringstream oss;
    for (int i = 0; i < numbit; i++) {
        switch ((value >> (numbit - i - 1)) & 1) {
            case 0: oss << "0";
                break;
            case 1: oss << "1";
                break;
        }
    }
    return oss.str();


}

string BitReader::printFilePos(fstream &fs) {
    ostringstream oss;
    int position = (int)fs.tellg() - 4 + (startPos / 8);

    oss << "0x" << setw(8) << setfill('0') << hex << uppercase << position;
    oss << "." << hex << (int)startPos;

    return oss.str();
}

string BitReader::printBuffer() {
    ostringstream oss;
    oss << "[0x ";
    for (int i = 3; i >= 0; i--) {
        oss << hex << uppercase << setw(2) << setfill('0') << ((buffer >> 8 * i) & 0x000000FF) << " ";
    }
    oss << "= 0b (";

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {

            if (i * 8 + j < startPos || i * 8 + j > endPos) {
                oss << "-";
            } else {
                if ((buffer << i * 8 + j) >> (sizeof (buffer)*8 - 1) == 0) {
                    oss << "0";
                } else {
                    oss << "1";
                }
            }

        }
        if (i == 3) {
            oss << ")]";
        } else {
            oss << " ";
        }
    }

    return oss.str();
}
