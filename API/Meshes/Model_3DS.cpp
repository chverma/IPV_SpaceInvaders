//////////////////////////////////////////////////////////////////////
//
// 3D Studio Model Class
// by: Matthew Fairfax
//
// Model_3DS.cpp: implementation of the Model_3DS class.
// This is a simple class for loading and viewing
// 3D Studio model files (.3ds). It supports models
// with multiple objects. It also supports multiple
// textures per object. It does not support the animation
// for 3D Studio models b/c there are simply too many
// ways for an artist to animate a 3D Studio model and
// I didn't want to impose huge limitations on the artists.
// However, I have imposed a limitation on how the models are
// textured:
// 1) Every faces must be assigned a material
// 2) If you want the face to be textured assign the
//    texture to the Diffuse Color map
// 3) The texture must be supported by the GLTexture class
//    which only supports bitmap and targa right now
// 4) The texture must be located in the same directory as
//    the model
//
// Support for non-textured faces is done by reading the color
// from the material's diffuse color.
//
// Some models have problems loading even if you follow all of
// the restrictions I have stated and I don't know why. If you
// can import the 3D Studio file into Milkshape 3D 
// (http://www.swissquake.ch/chumbalum-soft) and then export it
// to a new 3D Studio file. This seems to fix many of the problems
// but there is a limit on the number of faces and vertices Milkshape 3D
// can read.
//
// Usage:
// Model_3DS m;
//
// m.Load("model.3ds"); // Load the model
// m.Draw();			// Renders the model to the screen
//
// // If you want to show the model's normals
// m.shownormals = true;
//
// // If the model is not going to be lit then set the lit
// // variable to false. It defaults to true.
// m.lit = false;
//
// // You can disable the rendering of the model
// m.visible = false;
// 
// // You can move and rotate the model like this:
// m.rot.x = 90.0f;
// m.rot.y = 30.0f;
// m.rot.z = 0.0f;
//
// m.pos.x = 10.0f;
// m.pos.y = 0.0f;
// m.pos.z = 0.0f;
//
// // If you want to move or rotate individual objects
// m.Objects[0].rot.x = 90.0f;
// m.Objects[0].rot.y = 30.0f;
// m.Objects[0].rot.z = 0.0f;
//
// m.Objects[0].pos.x = 10.0f;
// m.Objects[0].pos.y = 0.0f;
// m.Objects[0].pos.z = 0.0f;
//
//////////////////////////////////////////////////////////////////////

// Modified by Santiago Piqueras, jan 2014

// This is used to generate a warning from the compiler
// #define _QUOTE(x) # x
// #define QUOTE(x) _QUOTE(x)
// #define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "
// #define warn( x )  message( __FILE__LINE__ #x UGKS_NEW_LINE_STRING ) 

// You need to uncomment this if you are using MFC <- Probably not
// #pragma warn( You need to uncomment this if you are using MFC )
//#include "stdafx.h"

#include "Model_3DS.h"

#include <math.h>			// Header file for the math library

#ifdef MODEL_API_ASSIMP
Assimp::Importer Model_3DS::importer;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Model_3DS::Model_3DS()
{
	// Initialization

	// Don't show the normals by default
	shownormals = false;

	// The model is lit by default
	lit = true;

	// The model is visible by default
	visible = true;

	// Set up the default position
	pos.x = 0.0f;
	pos.y = 0.0f;
	pos.z = 0.0f;
	// Set up the default rotation
	rot.x = 0.0f;
	rot.y = 0.0f;
	rot.z = 0.0f;

	// Set up the path
	path = new char[80];
	sprintf(path, "\n");

	// Zero out our counters for MFC
	numObjects = 0;
	numMaterials = 0;

	// Set the scale to one
	scaleX=scaleY=scaleZ = 1.0f;
}

Model_3DS::~Model_3DS(){
}

#ifdef MODEL_API_ASSIMP
void Model_3DS::Load(char *name){
	unsigned int j;

	if (strstr(name, "\"\n"))
		name = strtok(name, "\"\n");
	
	modelname = name;

	// Find the path
	if (strstr(name, "/") || strstr(name, "\\"))
	{
		// Holds the name of the model minus the path
		char *temp;

		// Find the name without the path
		if (strstr(name, "/"))
			temp = strrchr(name, '/');
		else
			temp = strrchr(name, '\\');

		// Allocate space for the path
		path = new char[strlen(name)-strlen(temp)+1];

		// Get a pointer to the end of the path and name
		char *src = name + strlen(name) - 1;

		// Back up until a \ or the start
		while (src != path && !((*(src-1)) == '\\' || (*(src-1)) == '/'))
			src--;

		// Copy the path into path
		memcpy (path, name, src-name);
		path[src-name] = 0;
	}

	scene = importer.ReadFile(name, aiProcess_TransformUVCoords | aiProcess_JoinIdenticalVertices | 0);
 
    // If the import failed, report it
    if( !scene)
    {
        printf("%s\n", importer.GetErrorString());
		return;
    }

	modelname = name;
	
	totalFaces = 0;
	totalVerts = 0;

	if(scene->HasMeshes()){
		this->numObjects = scene->mNumMeshes;
		this->Objects = new Object[numObjects];

		aiMesh* mesh;
		Object* object;

		// Set the textured variable to false until we find a texture
		for (int k = 0; k < numObjects; k++)
			Objects[k].textured = false;

		// Zero the objects position and rotation
		for (int m = 0; m < numObjects; m++)
		{
			Objects[m].pos.x = 0.0f;
			Objects[m].pos.y = 0.0f;
			Objects[m].pos.z = 0.0f;

			Objects[m].rot.x = 0.0f;
			Objects[m].rot.y = 0.0f;
			Objects[m].rot.z = 0.0f;
		}

		// Objects
		for(int i = 0; i < numObjects; ++i){
			mesh = scene->mMeshes[i];
			object = &(Objects[i]);
			
			// Name
			strcpy(object->name, mesh->mName.C_Str());

			// Faces
			if(mesh->HasFaces()){
				totalFaces += mesh->mNumFaces;
				object->numFaces = mesh->mNumFaces*3;
				object->Faces= new unsigned short[object->numFaces * 3];
				for(j = 0; j < mesh->mNumFaces; ++j){
					object->Faces[j*3] = mesh->mFaces[j].mIndices[0];
					object->Faces[j*3 + 1] = mesh->mFaces[j].mIndices[1];
					object->Faces[j*3 + 2] = mesh->mFaces[j].mIndices[2];
				}
			}
			
			// Vertices
			if(mesh->HasPositions()){
				totalVerts += mesh->mNumVertices;
				object->numVerts = mesh->mNumVertices;
				object->Vertexes = new float[object->numVerts*3];
				object->Normals = new float[object->numVerts*3];
				for(j = 0; j < mesh->mNumVertices; ++j){
					// CARE: y and z coords are switched AND z is inverted
					// Original code did that
					//                         God knows why
					object->Vertexes[j*3] = mesh->mVertices[j].x;
					object->Vertexes[j*3 + 2] = - mesh->mVertices[j].y;
					object->Vertexes[j*3 + 1] = mesh->mVertices[j].z;
				}
			}
			
			// TexCoords
			if(mesh->HasTextureCoords(0)){
				object->numTexCoords = object->numVerts;
				object->TexCoords = new float[object->numVerts*2];
				for(int j = 0; j < object->numVerts; ++j){
					object->TexCoords[j*2] = mesh->mTextureCoords[0][j].x;
					object->TexCoords[j*2+1] = mesh->mTextureCoords[0][j].y;
				}
				object->textured = true;
			}
			
			// Materials
			object->numMatFaces = 1;
			object->MatFaces = new MaterialFaces[1];
			object->MatFaces[0].MatIndex = mesh->mMaterialIndex;
			object->MatFaces[0].subFaces = object->Faces;  // Meshes share a single material in assimp
			object->MatFaces[0].numSubFaces = object->numFaces;
		}
		
		// We could use the model normals if it has any
		CalculateNormals();

	}

	// Materials
	if(scene->HasMaterials()){
		this->numMaterials = scene->mNumMaterials;
		this->Materials = new Material[numMaterials];
		

		for(int i = 0; i < numMaterials; ++i){
			aiMaterial* material = scene->mMaterials[i];
			Material* myMaterial = &(this->Materials[i]);

			if(AI_SUCCESS != material->Get(AI_MATKEY_NAME, myMaterial->name)){
				// Assimp doesn't seem to get this right
				// Do something, maybe?
			}

			// Color
			aiColor4D color;
			if(AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color)){
				// Color is always in range 0..1 in assimp
				myMaterial->color.r = color.r*255.0f;
				myMaterial->color.g = color.g*255.0f;
				myMaterial->color.b = color.b*255.0f;
				myMaterial->color.a = 255;
			}

			// Texture
			aiString texPath;	//contains filename of texture
			if(AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)){
				char fullname[80];
				sprintf(fullname, "%s%s", path, texPath.data);
				myMaterial->tex.Load(texPath.data);
				myMaterial->textured = true;
			}else
				myMaterial->textured = false;
		}

	}else{
		// Screw this, we'll give you a material anyway

		this->numMaterials = 1;
		this->Materials = new Material[1];
		Materials[1].textured = false;
	}
	
	// If the object doesn't have any texcoords generate some
	for (int k = 0; k < numObjects; k++)
	{
		if (Objects[k].numTexCoords == 0)
		{
			// Set the number of texture coords
			Objects[k].numTexCoords = Objects[k].numVerts;

			// Allocate an array to hold the texture coordinates
			Objects[k].TexCoords = new GLfloat[Objects[k].numTexCoords * 2];

			// Make some texture coords
			for (int m = 0; m < Objects[k].numTexCoords; m++)
			{
				Objects[k].TexCoords[2*m] = Objects[k].Vertexes[3*m];
				Objects[k].TexCoords[2*m+1] = Objects[k].Vertexes[3*m+1];
			}
		}
	}

	// Let's build simple colored textures for the materials w/o a texture
	for (int j = 0; j < numMaterials; j++)
	{
		if (Materials[j].textured == false)
		{
			unsigned char r = Materials[j].color.r;
			unsigned char g = Materials[j].color.g;
			unsigned char b = Materials[j].color.b;
			Materials[j].tex.BuildColorTexture(r, g, b);
			Materials[j].textured = true;
		}
	}

	// We don't need the scene anymore
	importer.FreeScene();
}
#endif

void Model_3DS::Draw()
{
	if (visible)
	{
	glPushMatrix();

		// Move the model
		glTranslatef(pos.x, pos.y, pos.z);

		// Rotate the model
		glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
		glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
		glRotatef(rot.z, 0.0f, 0.0f, 1.0f);

		glScalef(scaleX, scaleY, scaleZ);

		// Loop through the objects
		for (int i = 0; i < numObjects; i++)
		{
			// Enable texture coordiantes, normals, and vertices arrays
			if (Objects[i].textured)
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			if (lit)
				glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);

			// Point them to the objects arrays
			if (Objects[i].textured)
				glTexCoordPointer(2, GL_FLOAT, 0, Objects[i].TexCoords);
			if (lit)
				glNormalPointer(GL_FLOAT, 0, Objects[i].Normals);
			glVertexPointer(3, GL_FLOAT, 0, Objects[i].Vertexes);

			// Loop through the faces as sorted by material and draw them
			for (int j = 0; j < Objects[i].numMatFaces; j ++)
			{
				// Use the material's texture
				Materials[Objects[i].MatFaces[j].MatIndex].tex.Use();

				glPushMatrix();

					// Move the model
					glTranslatef(Objects[i].pos.x, Objects[i].pos.y, Objects[i].pos.z);

					// Rotate the model

					glRotatef(Objects[i].rot.z, 0.0f, 0.0f, 1.0f);
					glRotatef(Objects[i].rot.y, 0.0f, 1.0f, 0.0f);
					glRotatef(Objects[i].rot.x, 1.0f, 0.0f, 0.0f);

					// Draw the faces using an index to the vertex array
					glDrawElements(GL_TRIANGLES, Objects[i].MatFaces[j].numSubFaces, GL_UNSIGNED_SHORT, Objects[i].MatFaces[j].subFaces);

				glPopMatrix();
			}

			// Show the normals?
			if (shownormals)
			{
				// Loop through the vertices and normals and draw the normal
				for (int k = 0; k < Objects[i].numVerts * 3; k += 3)
				{
					// Disable texturing
					glDisable(GL_TEXTURE_2D);
					// Disbale lighting if the model is lit
					if (lit)
						glDisable(GL_LIGHTING);
					// Draw the normals blue
					glColor3f(0.0f, 0.0f, 1.0f);

					// Draw a line between the vertex and the end of the normal
					glBegin(GL_LINES);
						glVertex3f(Objects[i].Vertexes[k], Objects[i].Vertexes[k+1], Objects[i].Vertexes[k+2]);
						glVertex3f(Objects[i].Vertexes[k]+Objects[i].Normals[k], Objects[i].Vertexes[k+1]+Objects[i].Normals[k+1], Objects[i].Vertexes[k+2]+Objects[i].Normals[k+2]);
					glEnd();

					// Reset the color to white
					glColor3f(1.0f, 1.0f, 1.0f);
					// If the model is lit then renable lighting
					if (lit)
						glEnable(GL_LIGHTING);
				}
			}
		}

	glPopMatrix();
	}
}

void Model_3DS::CalculateNormals()
{
	// Let's build some normals
	for (int i = 0; i < numObjects; i++)
	{
		for (int g = 0; g < Objects[i].numVerts; g++)
		{
			// Reduce each vert's normal to unit
			float length;
			Vector unit;

			unit.x = Objects[i].Normals[g*3];
			unit.y = Objects[i].Normals[g*3+1];
			unit.z = Objects[i].Normals[g*3+2];

			length = (float)sqrt((unit.x*unit.x) + (unit.y*unit.y) + (unit.z*unit.z));

			if (length == 0.0f)
				length = 1.0f;

			unit.x /= length;
			unit.y /= length;
			unit.z /= length;

			Objects[i].Normals[g*3]   = unit.x;
			Objects[i].Normals[g*3+1] = unit.y;
			Objects[i].Normals[g*3+2] = unit.z;
		}
	}
}