
#include <ByteHandler.h>

ByteHandler::ByteHandler() {
}

bool ByteHandler::get(istream &stream, BYTE &b) {
    int i = stream.get();
    if (stream.eof())
        return false;
    b = i;
    return true;
};

bool ByteHandler::getLittle(istream &stream, WORD &w) {
    BYTE low, high;
    if (!get(stream, low))
        return false;
    if (!get(stream, high))
        return false;
    
    if (high == 0xFF) {
        if (low == 0x00) {
            cout << "Trovato FF00" << endl;
            if (!get(stream, low))
                return false;
        } else {
            cout << "Trovato FF<qualcosa da 1 byte diverso da zero>" << endl;
            return false;
        }
    }
    w = (WORD(high) << 8) | WORD(low);
    return true;
};

bool ByteHandler::getLittle(istream &stream, DWORD &dw) {
    WORD low, high, middle;
    BYTE low_;
    if (!getLittle(stream, low))
        return false;
    if (!getLittle(stream, high))
        return false;
    
    // controllo l'ultimo caso che è FF<qualcosa> in mezzo alla DWORD
    middle = dw & 0x00FFFF00;
    if ((middle & 0xFF00) == 0xFF){
        if ((middle & 0x00FF) == 0x00){
            cout << "Trovato FF00" << endl;
            if(!get(stream, low_))
                return false;
            dw = (DWORD(high) << 16) | ((DWORD(low) & 0x00FF) << 8) | (DWORD(low_));
            return true;
        } else {
            cout << "Trovato FF<qualcosa da 1 byte diverso da zero>" << endl;
            return false;
        }
    }
    dw = (DWORD(high) << 16) | DWORD(low);
    return true;
};

bool ByteHandler::getLittle(istream &stream, LONG &l) {
    DWORD dw;
    if (!getLittle(stream, dw))
        return false;
    l = dw;
    return true;
};

/**
 * Recupera una WORD (2 byte) trattandola con ordine big endian
 * @param stream
 * Lo stream da cui pescare la WORD
 * @param w
 * Il dato in cui salvare la word
 * @return
 * <i>true</i> se l'operazione va a buon fine, <i>false</i> altrimenti
 */
bool ByteHandler::getBig(istream &stream, WORD &w) {
    BYTE low, high;
    if (!get(stream, high))
        return false;
    if (!get(stream, low))
        return false;
    if (high == 0xFF) {
        if (low == 0x00) {
            cout << "Trovato FF00" << endl;
            if (!get(stream, low))
                return false;
        } else {
            cout << "Trovato FF<qualcosa da 1 byte diverso da zero>" << endl;
            return false;
        }
    }
    w = (WORD(high) << 8) | WORD(low);
    return true;
};

bool ByteHandler::getBig(istream &stream, DWORD &dw) {
    WORD low, high, middle;
    BYTE low_;
    if (!getBig(stream, high))
        return false;
    if (!getBig(stream, low))
        return false;
    
    dw = (DWORD(high) << 16) | DWORD(low);
    // controllo l'ultimo caso che è FF<qualcosa> in mezzo alla DWORD
    middle = dw & 0x00FFFF00;
    if ((middle & 0xFF00) == 0xFF){
        if ((middle & 0x00FF) == 0x00){
            cout << "Trovato FF00" << endl;
            if(!get(stream, low_))
                return false;
            dw = (DWORD(high) << 16) | ((DWORD(low) & 0x00FF) << 8) | (DWORD(low_));
            return true;
        } else {
            cout << "Trovato FF<qualcosa da 1 byte diverso da zero>" << endl;
            return false;
        }
    }
    dw = (DWORD(high) << 16) | DWORD(low);
    return true;
};

bool ByteHandler::getBig(istream &stream, LONG &l) {
    DWORD dw;
    if (!getBig(stream, dw))
        return false;
    l = dw;
    return true;
};

void ByteHandler::wordBuffer(WORD littleWord, char* array) {
    array[0] = BYTE(littleWord & 0x00FF);
    array[1] = BYTE(littleWord >> 8);
}

void ByteHandler::dwordBuffer(DWORD littledword, char* array) {
    for (int i = 0; i < sizeof (DWORD); i++) {
        array[i] = (littledword >> (i * 8)) & 0x000000FF;
    }
}

void ByteHandler::longBuffer(LONG littlelword, char* array) {
    //    int j = sizeof(LONG);
    for (int i = 0; i < sizeof (LONG); i++) {
        array[i] = (littlelword >> (i * 8)) & 0x000000FF;
    }
}

/*=======================================
 |      Metodi di Alessandro Benisi
 ========================================
 */

/* Metodo aggiunto da Alessandro Benisi.
 * L'esigenza di questo metodo è nata durante la stesura del metodo JpegHeader::load_quantization_table().
 * Dalle specifiche risulta che la tabella di quantizzazione è definita anche dai valori Pq e Tq che sono valori da 4 bit...
 */
void splitByte(BYTE b, BYTE &high, BYTE &low) {
    // teniamo come "unità di misura base" il byte.
    // La parte alta la otteniamo con un and bit a bit con il byte 11110000 (0xF0) e poi shiftiamo verso destra per togliere gli zeri a destra.
    high = (b & 0xF0) >> 4;
    // La parte bassa la otteniamo con un and bit a bit con ili byte 00001111 (0x0F). Non è necessario in questo caso alcuno shift.
    low = b & 0x0F;
}

/* Versione modificata di splitbyte, siccome per Pq della tabella di quantizzazione
 * si può avere solo 0 o 1 di valori, uso un boolean per risparmiare spazio
 *
 */
void splitByte(BYTE b, bool &high, BYTE &low) {

    if (((b & 0xF0) >> 4) == 1) {
        high = true;
    } else {
        high = false;
    }
    // La parte bassa la otteniamo con un and bit a bit con ili byte 00001111 (0x0F). Non è necessario in questo caso alcuno shift.
    low = b & 0x0F;
}


// getByte: legge il byte corrente e sposta l'indice su quello successivo

bool ByteHandler::getByte(fstream &file, BYTE &b) {
    int i;
    i = file.get();
    if (file.eof()) {
        return true;
    }
    b = i;
    return false;
}
// getWord: legge una word (due byte) a partire dal byte corrente e si porta al byte successivo alla word letta

bool ByteHandler::getWord(fstream &file, WORD &w) {
    /*
        WORD i;
        i = ((WORD) file.get()) << 8;
        if (file.eof()){
            return false;
        }
        i += (BYTE) file.get();
        if (file.eof()){
            return false;
        }
        w = i;
        return true;
     */
    getBig(file, w);
}

// Costruisce una Word a partire da due byte (uno alto e uno basso)

WORD ByteHandler::makeWord(BYTE high, BYTE low) {
    WORD w = 0;
    w |= high;
    w <<= 8;
    w |= low;
    return w;
}

// Preleva il byte alto/basso a seconda che h_l sia vero/falso.
//

BYTE ByteHandler::takeByteFromWord(bool h_l, WORD w) {
    BYTE b;
    if (!h_l) {
        w << 8;
    }
    b = (BYTE) w >> 8;
    return b;
}
