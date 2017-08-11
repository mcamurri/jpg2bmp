
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <vector>

using namespace std;

#ifndef BITREADER_H
#define	BITREADER_H

/**
 * @breaf Classe per la lettura dei dati contenuti nella scan, preceduti dal marker (SOS).
 * Gestisce automaticamente:<br />
 * <ul>
 * <li>byte di stuffing (0xFF00);</li>
 * <li>terminazione dei dati dello scan;</li>
 * <li>eventuale fine inaspettata del file.</li>
 * </ul>
 * @author Alessandro Benisi, Marco Camurri
 */
class BitReader {
private:
    bool the_end; /**< flag per indicare che si è arrivati alla fine della parte dati dello scan. */
    unsigned int bitPos; /**< Cursore che indica la posizione all'interno del buffer. */
    unsigned int buffer; /**< Dato a 32 bit; agisce da sliding window lungo il file. */
    
    unsigned int stuffbytes;
    
    
    unsigned char startPos;
    unsigned char endPos;


public:
    /**
     * Costruttore vuoto. Non usata.
     * @author Alessandro Benisi
     */
    BitReader();

    /**
     * Questo costruttore effettua l'inizializzazione del buffer (riempimento) e 
     * inizializza la posizione del cursore bitPos all'interno del buffer.
     * @param strm fstream, corrispondente al file immagine, con la testina già posizionata in corrispondenza dei dati della scan
     * @param pos posizione del cursore
     * @see BitReader(fstream &file)
     * @see bitPos
     * @author Alessandro Benisi
     */
    BitReader(fstream &strm, int pos);

    /**
     * Questo costruttore effettua l'inizializzazione del buffer (riempimento).
     * @param strm fstream, corrispondente al file immagine, con la testina già posizionata in corrispondenza dei dati della scan
     * @warning Il cursore del file stream deve essere già posizionato all'inizio 
     * della parte dati dello scan.
     * @author Alessandro Benisi
     */
    BitReader(fstream &strm);

    /**
     * Legge un numero di bit arbitrario dal buffer, ignorando contestualmente 
     * i byte di stuffing (0xFF00), e li restituisce all'interno di un unsigned int.
     * @param strm fstream da cui leggere i dati dello scan
     * @param numBit Il numero di bit da leggere
     * @param data L'intero senza segno su cui salvare i bit letti
     * @return Ritorna 0 se sono stati letti correttamente
     * tutti i bit, ritorna un numero diverso da zero se si cerca di leggere
     * piu' bit di quelli che mancano alla fine del file, e piu' precisamente
     * ritorna l'esatto numero di bit letti in quest'ultimo caso.
     * @see buffer
     * @author Alessandro Benisi
     */
    int readBits(fstream &strm, int numbit, unsigned int &data);
    
   int readBits(fstream &strm, int numbit, unsigned int& data, bool isNewCode);

    /**
     * Legge un numero di bit arbitrario dal buffer, ignorando contestualmente 
     * i byte di stuffing (0xFF00), e li restituisce all'interno di un vettore di booleani.
     * @see readBits(fstream &strm, int numbit, unsigned int &data)
     * @author Alessandro Benisi
     */
    int readBits(fstream &strm, int numbit, vector<bool> &data);

    /**
     * Metodo che, dato un unsigned int e un numero di bit, restituisce una rappresentazione 
     * binaria di value, di lunghezza numbit sotto forma di stringa.
     * @param value valore da rappresentare in binario
     * @param numbit numero di bit da rappresentare
     * @return stringa di tipo string contenente la rappresentazione binaria di value
     * @author Marco Camurri
     */
    string binary(unsigned int value, unsigned int numbit);
    
    /**
     * Metodo invocato ogni <b>Ri</b> intervalli di MCU, dove <b>Ri</b> &egrave;
     * rappresenta il numero di MCU tra un restart interval. Gestisce automaticamente
     * la presenza del marker RSTm (con -1 < m < 8 ) che viene saltato. bitPos viene
     * resettato a 0.
     * @param strm
     * Lo stream contenente i dati della scan
     */
    void restart(fstream &strm);
    
    /**
     * Metodo che rimuove n byte già letti e inserisce n byte nuovi prelevati dal file, 
     * aggiornando contestualmente bitPos (arretrandolo opportunamente).
     * @param strm filestream contenente i dati dell'immagine
     * @author Alessandro Benisi
     */
    void refresh(fstream &strm);
    
    /**
     * Metodo che restituisce una stringa che mostra il contenuto del buffer
     * in formato esadecimale
     * @return il contenuto del buffer, in formato esadecimale maiuscolo separato da
     * spazi (es. "0x 01 23 45 67")
     */
    string printBuffer();
    
    
    string printFilePos(fstream &fs);

};

#endif	/* BITREADER_H */

