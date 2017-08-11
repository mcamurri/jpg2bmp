
#include "BmpFileHeader.h"
#include "BmpInfoHeader.h"
#include "RgbQuad.h"

class BmpImage {
public:
    /**
     * Costruttore di default. Non fa nulla.
     */
    BmpImage();
    /**
     * Questo costruttore inizializza una immagine in scala di grigi delle
     * dimensioni indicate dai parametri
     * @param w
     * La larghezza dell'immagine, indicata come intero senza segno.
     * @param h
     * L'altezza dell'immagine, indicata come intero senza segno.
     */
    BmpImage(unsigned w, unsigned h);
    /**
     * Costruttore che inizializza l'immagine prendendo da file.
     * @param path
     * Il percorso assoluto dell'immagine bmp.
     */
    BmpImage(const char *path);
    /**
     * Costruttore che inizializza l'immagine con i le dimensioni indicate
     * e con la profondità colore indicata
     * @param w
     * La larghezza dell'immagine, come intero senza segno
     * @param h
     * L'altezza dell'immagine, come intero senza segno
     * @param depth
     * La profondità, indicata in bit, per ogni pixel. Sono ammessi solo i valori
     * 1, 4, 8, 16, 24 e 32.
     */
    BmpImage(unsigned int w, unsigned int h, unsigned char depth);
    /**
     * Distruttore
     */
    ~BmpImage();
    
    BmpImage(const BmpImage &orig);

    /**
     * Funzione che indica la larghezza dell'immagine, in pixel.
     * @return la larghezza in pixel dell'immagine, come intero senza segno
     */
    unsigned getWidth() const;
    /**
     * Funzione che indica l'altezza dell'immagine, in pixel.
     * @return l'altezza in pixel dell'immagine, come intero senza segno
     */
    unsigned getHeight() const;
    /**
     * Funzione che ritorna la larghezza di una linea dell'immagine.
     * La larghezza della linea è uguale alla larghezza reale divisa per quattro
     * e arrotondata per eccesso, in modo che sia divisibile per quattro.
     * @return 
     * La larghezza effettiva in pixel dell'immagine.
     */
    unsigned getLinewidth() const;
    
    //Metodi getter
    BmpFileHeader getFileHeader() const;
    BmpInfoHeader getInfoHeader() const;
    
    unsigned char getPixel(int x, int y) const;
    void getPixel(unsigned int x, unsigned int y, unsigned char* rgb) const;

    bool save(const char* path);
    void setPixel(unsigned int x, unsigned int y, unsigned char value);
    void setPixel(unsigned int x, unsigned int y, unsigned char* value, unsigned char channels);
    void setPixel(unsigned int x, unsigned int y, unsigned int R, unsigned int G, unsigned int B);
    void setPixel(unsigned int x, unsigned int y, unsigned char value, unsigned char channel);
    void setPaletteColor(unsigned char index, RgbQuad quad);

    void printHeader();
    void printImagePalette();

    BmpImage & operator=(const BmpImage &orig);
    unsigned char & operator() (unsigned x, unsigned y);

    void setFileHeader(BmpFileHeader imageFileHeader) {
        this->imageFileHeader = imageFileHeader;
    }

    void setInfoHeader(BmpInfoHeader imageInfoHeader) {
        this->imageInfoHeader = imageInfoHeader;
    }

private:
    unsigned int width;
    unsigned int height;
    unsigned int linewidth;
    BmpFileHeader imageFileHeader;
    BmpInfoHeader imageInfoHeader;
    RgbQuad* imagePalette;
    unsigned char* data;
    // Getter private
    unsigned char *getData();
    unsigned char *getLineptr(unsigned y);
    unsigned char *getLineptr(unsigned int y, unsigned char channel);
    RgbQuad* getPalette();
};




