#include <iostream>									 
#include <fstream>									 
#include <glrv/include/images.h>

using namespace std;								 

#pragma pack(push,2)								//Alinea a 2Bytes
typedef struct{
	char BM[2];					//debe ser 'B''M'
	unsigned int tamanofich;	//tamaño del fichero
	unsigned short reservado1;
	unsigned short reservado2;
	unsigned int offset;		//offset a la imagen
	unsigned int tcabeza;		//Tamaño cabecera debe ser 40
	unsigned int ancho;			//Numero de columnas
	unsigned int alto;			//Numero de filas
	unsigned short planos;		//Debe ser 1
	unsigned short bitsXpx;		//Bits/pixel. Debería ser 24
	unsigned int compresion;	//Debe ser 0 para no comprimido
	unsigned int timagen;		//Tamaño de la imagen en bytes
	unsigned int xpels;			//
	unsigned int ypels;			//
	unsigned int nLUT;			//
	unsigned int impcolor;		//
} BMP_HEAD;
#pragma pack(pop)

int leerFicheroBMP(const char* nombre,  unsigned int &numCols,  unsigned int &numFilas, GLenum &formato, GLubyte* &tex )
{
	BMP_HEAD bmph;
	char c;

	//Abrimos el fichero de imagen. Debe ser un bitmap de windows sin compresión
	ifstream f;
	f.open(nombre,ios::in|ios::binary); //Abre el fichero en stream binario de lectura
	if(!f){ cerr << "images::No puedo abrir el fichero: " << nombre << endl; return 0;};

	f.read((char *) &bmph, sizeof(BMP_HEAD));

	numCols= bmph.ancho;
	numFilas= bmph.alto;

	if(bmph.bitsXpx!=24 || bmph.compresion!=0 ){
		cerr << "images::No son 24 bits / pixel o va comprimido el fichero: "<< nombre << endl;
		f.close();
		return 0; 
	};

	int numBytesRelleno,numBytesFila;
	numBytesFila= ((numCols+1)*3 / 4)*4;		//Tiene que ser (el siguiente) múltiplo de 4
	numBytesRelleno= numBytesFila - numCols*3;	//Bytes de relleno hasta fin de fila

	tex= new GLubyte[numCols*numFilas*3];		//Array de textura.RGBRGBRGB....

	//Rellenamos el array de textura
	int cont=0;
	for(unsigned int j=0;j<numFilas;j++){
		for(unsigned int i=0;i<numCols;i++){
			char r,g,b;
			f.get(b); f.get(g);f.get(r);		//Vienen justo al revés BGRBGRBGR...
			tex[cont++]= r;
			tex[cont++]= g;
			tex[cont++]= b;
		};
		for(int i=0; i<numBytesRelleno; i++){
			f.get(c); //No sirven, son de relleno
		};
	};
	formato= GL_RGB;							//Formato de empaquetado RGB

	f.close();
	return 1;
};

#define ERRORMSG(id) { error=id; goto _error; }

int read_image (char * filename, unsigned int *width, unsigned int *height, unsigned char **buffer) {

	FILE *f;
	BMP_HEAD bmph;
	unsigned int i, tam, c;
	unsigned char *mem, paleta[256*4];
	int error=0;

	*buffer=NULL;
	if ((f=fopen(filename, "rb"))==NULL) ERRORMSG(-1);

	if (fread(&bmph, sizeof(BMP_HEAD), 1, f)!=1) ERRORMSG(-2);

	if (bmph.BM[0]!='B' || bmph.BM[1]!='M' || (bmph.bitsXpx != 24 && bmph.bitsXpx !=8) || bmph.compresion!=0) ERRORMSG(-3);

	if (bmph.bitsXpx==8) {
		fseek(f, sizeof(BMP_HEAD), SEEK_SET);
		fread(paleta, sizeof(unsigned char)*4, 256, f);
	}

	fseek(f, bmph.offset, SEEK_SET);
	mem=(unsigned char *)malloc(sizeof(unsigned char)*bmph.alto*bmph.ancho*4);
	if (mem==NULL) ERRORMSG(-4);
	
	*buffer=mem;
	*width=bmph.ancho;
	*height=bmph.alto;

	tam=bmph.alto*bmph.ancho;
	for (i=0; i<tam; i++) {
		mem[3]=255;
		if (bmph.bitsXpx==8) {
			c=fgetc(f);
			mem[2]=paleta[c*4];
			mem[1]=paleta[c*4+1];
			mem[0]=paleta[c*4+2];
		} else {
			mem[2]=fgetc(f);
			mem[1]=fgetc(f);
			mem[0]=fgetc(f);
		}
		mem+=4;
	}

_error:
	if (f!=NULL) fclose(f);
	switch (error) {
		case -1:
			fprintf(stderr, "No se ha podido abrir el fichero %s\n", filename);
			break;
		case -2:
			fprintf(stderr, "Error leyendo desde el fichero %s\n", filename);
			break;
		case -3:
			fprintf(stderr, "Formato de imagen no soportado. Solo se aceptan BMP de 24 bits sin compresión\n");
			break;
		case -4:
			fprintf(stderr, "Sin memoria (fichero corrupto?)\n");
			break;
	}
	return error;
}