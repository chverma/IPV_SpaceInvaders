/**	General Texture class that manages general functions of any given image

*	Prefix CT_: Class Texture

*	@Author: Vicente Broseta Toribio: 2013-06
*	@author Ramon Molla
*	@version 01-2015
*/

#include <Texture.h>
#include <UGKOS.h>

CTexture::CTexture(){
	IndexInList	= CT_NOT_INDEXED;
	FilePath	= NULL;
	FileName	= NULL;
	Loaded		= false;
}

char *CTexture::GetPathFile (char *filename)
{
	char dir[1024];
	
	if (NULL == filename)
		return NULL;

	strcpy(dir, UGKOS_THIS_DIRECTORY);
	strcat(dir, UGKOS_DIRECTORY_SEPARATOR);

	if (NULL != FilePath)
	{
		strcat(dir, FilePath);
		strcat(dir, UGKOS_DIRECTORY_SEPARATOR);
	}
	strcat(dir, filename);
	return dir;
}

void CTexture::SetFileName(char* filename)
{
	if (NULL != FileName)
		delete FileName;

	if (NULL == filename)
		FileName = NULL;
	else
	{
		FileName = new char[strlen(filename)+1];	//One more for the "\n" string ending char
		strcpy(FileName, filename);
	}
	
}

void CTexture::SetFilePath(char* filepath)
{
	if (NULL != FilePath)
		delete FilePath;

	if (NULL == filepath)
		FileName = NULL;
	else
	{
		FilePath = new char[strlen(filepath)+1];	//One more for the "\n" string ending char
		strcpy(FilePath, filepath);
	}
}

void CTexture::SetTexture(void){
	glBindTexture(GL_TEXTURE_2D, Texture.gl_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Texture.wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Texture.wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Texture.maxMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture.minMode);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, Texture.tmode);
}

#define ERRORMSG(id) { error=id; goto _error; }

int CTexture::Load(char *filename, Ttexture *dest)
{
	unsigned char *pixels;
	string strg;
	char dir[1024];
		
	if (NULL==dest) return CT_ERROR;
	SetFileName(filename);

	strg = FileName;
	strcpy(dir, GetPathFile());
	
	if (NULL == dir) return CT_ERROR;

	if (strg.find(".bmp")!=string::npos){
		//if (Read_imageBMP(dir, &dest->width, &dest->height, &pixels)) return -2;
		LoadBMP(dir);
	
		dest->tmode=GL_MODULATE;	
		dest->minMode=GL_NEAREST; 
		dest->maxMode=GL_NEAREST; 
		dest->wrap_s=GL_CLAMP;    
		dest->wrap_t=GL_CLAMP;	

		glGenTextures(1, &dest->gl_id);
		SetTexture();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
		glTexImage2D(GL_TEXTURE_2D, 0, 4, dest->width, dest->height,0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		free(pixels);
	}

	Loaded = true;

	return CT_NO_ERROR;
}

int CTexture::Load(char *filename)
{
	string strg;
	char dir[1024];

	SetFileName(filename);
	
	strg = FileName;
	strcpy(dir, GetPathFile());
	

	if (strg.find(".bmp")!=string::npos){
		LoadBMP(dir);
	}
	if (strg.find(".tga")!=string::npos){
		LoadTGA(dir);
	}

	//Other extension file
	/*if (strg.find(".jpg")!=string::npos){
	}
	if (strg.find(".gif")!=string::npos){
	}
	if (strg.find(".png")!=string::npos){
	}*/

	Loaded = true;
	return CT_NO_ERROR;
}

//---------------------------------------------------------------------------//

#pragma pack(push, 2)
typedef struct { 
  char  id[2]; 
  unsigned int filesize;
  unsigned int nada0;
  unsigned int offBytes;
  unsigned int nada1;
  int ancho, alto;
  short uno;
  short bpp;
  unsigned int compresion;
  unsigned char randomcrap[20];
} BMP_SIMPLIFICADO;
#pragma pack(pop)

int CTexture::LoadBMP(char *name)
{
	// Create a place to store the texture
	AUX_RGBImageRec *TextureImage[1];

	// Set the pointer to NULL
	memset(TextureImage,0,sizeof(void *)*1);

	// Load the bitmap and assign our pointer to it
	TextureImage[0] = auxDIBImageLoad(name);

	// Just in case we want to use the width and height later
	Texture.width = TextureImage[0]->sizeX;
	Texture.height = TextureImage[0]->sizeY;

	Texture.tmode=GL_MODULATE;	
	Texture.minMode=GL_NEAREST; 
	Texture.maxMode=GL_NEAREST; 
	Texture.wrap_s=GL_CLAMP;    
	Texture.wrap_t=GL_CLAMP;	

	// Generate the OpenGL texture id
	glGenTextures(1, &Texture.gl_id);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, Texture.gl_id);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

	// Cleanup
	if (TextureImage[0])
	{
		if (TextureImage[0]->data)
			free(TextureImage[0]->data);

		free(TextureImage[0]);
	}

	return CT_NO_ERROR;
}

int CTexture::LoadTGA(char *name)
{
	GLubyte		TGAheader[12]	= {0,0,2,0,0,0,0,0,0,0,0,0};// Uncompressed TGA header
	GLubyte		TGAcompare[12];								// Used to compare TGA header
	GLubyte		header[6];									// First 6 useful bytes of the header
	GLuint		bytesPerPixel;								// Holds the number of bytes per pixel used
	GLuint		imageSize;									// Used to store the image size
	GLuint		temp;										// Temporary variable
	GLuint		type			= GL_RGBA;					// Set the default type to RBGA (32 BPP)
	GLubyte		*imageData;									// Image data (up to 32 Bits)
	GLuint		bpp;										// Image color depth in bits per pixel.

	FILE *file = fopen(name, "rb");							// Open the TGA file

	// Load the file and perform checks
	if(file == NULL ||														// Does file exist?
	   fread(TGAcompare,1,sizeof(TGAcompare),file) != sizeof(TGAcompare) ||	// Are there 12 bytes to read?
	   memcmp(TGAheader,TGAcompare,sizeof(TGAheader)) != 0				 ||	// Is it the right format?
	   fread(header,1,sizeof(header),file) != sizeof(header))				// If so then read the next 6 header bytes
	{
		if (file == NULL)									// If the file didn't exist then return
			return -1;
		else
		{
			fclose(file);									// If something broke then close the file and return
			return -2;
		}
	}

	// Determine the TGA width and height (highbyte*256+lowbyte)
	Texture.width  = header[1] * 256 + header[0];
	Texture.height = header[3] * 256 + header[2];
    
	// Check to make sure the targa is valid and is 24 bit or 32 bit
	if(Texture.width	<=0	||										// Is the width less than or equal to zero
	   Texture.height	<=0	||										// Is the height less than or equal to zero
	   (header[4] != 24 && header[4] != 32))				// Is it 24 or 32 bit?
	{
		fclose(file);										// If anything didn't check out then close the file and return
		return -1;
	}

	bpp				= header[4];							// Grab the bits per pixel
	bytesPerPixel	= bpp / 8;								// Divide by 8 to get the bytes per pixel
	imageSize		= Texture.width * Texture.height * bytesPerPixel;		// Calculate the memory required for the data

	// Allocate the memory for the image data
	imageData		= new GLubyte[imageSize];

	// Make sure the data is allocated write and load it
	if(imageData == NULL ||									// Does the memory storage exist?
	   fread(imageData, 1, imageSize, file) != imageSize)	// Does the image size match the memory reserved?
	{
		if(imageData != NULL)								// Was the image data loaded
			free(imageData);								// If so, then release the image data

		fclose(file);										// Close the file
		return -2;
	}

	// Loop through the image data and swap the 1st and 3rd bytes (red and blue)
	for(GLuint i = 0; i < int(imageSize); i += bytesPerPixel)
	{
		temp = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = temp;
	}

	// We are done with the file so close it
	fclose(file);

	// Set the type
	if (bpp == 24)
		type = GL_RGB;

	Texture.tmode=GL_MODULATE;	
	Texture.minMode=GL_NEAREST; 
	Texture.maxMode=GL_NEAREST; 
	Texture.wrap_s=GL_CLAMP;    
	Texture.wrap_t=GL_CLAMP;
	
	// Generate the OpenGL texture id
	glGenTextures(1, &Texture.gl_id);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, Texture.gl_id);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, type, Texture.width, Texture.height, type, GL_UNSIGNED_BYTE, imageData);

	// Cleanup
	free(imageData);

	return CT_NO_ERROR;
}

void CTexture::FreeTexture(void){
	glDeleteTextures(1, &Texture.gl_id);
	Loaded = false;
}

//int CTexture::Read_imageBMP (char * filename, unsigned int *width, unsigned int *height, unsigned char **buffer){
//	FILE *f;
//	BMP_SIMPLIFICADO bmph;
//	unsigned int i, tam, c;
//	unsigned char *mem, paleta[256*4];
//	int error=0;
//
//	*buffer=NULL;
//	if ((f=fopen(filename, "rb"))==NULL) ERRORMSG(-1);
//
//	if (fread(&bmph, sizeof(BMP_SIMPLIFICADO), 1, f)!=1) ERRORMSG(-2);
//
//	if (bmph.id[0]!='B' || bmph.id[1]!='M' || (bmph.bpp != 24 && bmph.bpp !=8) || bmph.compresion!=0) ERRORMSG(-3);
//
//	if (bmph.bpp==8) {
//		fseek(f, sizeof(BMP_SIMPLIFICADO), SEEK_SET);
//		fread(paleta, sizeof(unsigned char)*4, 256, f);
//	}
//
//	fseek(f, bmph.offBytes, SEEK_SET);
//	mem=(unsigned char *)malloc(sizeof(unsigned char)*bmph.alto*bmph.ancho*4);
//	if (mem==NULL) ERRORMSG(-4);
//	
//	*buffer=mem;
//	*width=bmph.ancho;
//	*height=bmph.alto;
//
//	tam=bmph.alto*bmph.ancho;
//	for (i=0; i<tam; i++) {
//		mem[3]=255;
//		if (bmph.bpp==8) {
//			c=fgetc(f);
//			mem[2]=paleta[c*4];
//			mem[1]=paleta[c*4+1];
//			mem[0]=paleta[c*4+2];
//		} else {
//			mem[2]=fgetc(f);
//			mem[1]=fgetc(f);
//			mem[0]=fgetc(f);
//		}
//		mem+=4;
//	}
//
//_error:
//	if (f!=NULL) fclose(f);
//	switch (error) {
//		case -1:
//			fprintf(stderr, "No se ha podido abrir el fichero %s\n", filename);
//			break;
//		case -2:
//			fprintf(stderr, "Error leyendo desde el fichero %s\n", filename);
//			break;
//		case -3:
//			fprintf(stderr, "Formato de imagen no soportado. Solo se aceptan BMP de 24 bits sin compresión\n");
//			break;
//		case -4:
//			fprintf(stderr, "Sin memoria (fichero corrupto?)\n");
//			break;
//	}
//	return error;
//}


//---------------------------------------------------------------------------//



