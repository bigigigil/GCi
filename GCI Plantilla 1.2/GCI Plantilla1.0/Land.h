#ifndef _Land
#define _Land
#include "Geometrias.h"
#include "openglclass.h"
#include <gl/glu.h>
#include "TextureLoaderForOpenGL.h"

class Land: public Primitivos
{

private:
	float anchoFinal;
	float profunidadFinal;
	float deltax, deltaz;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;

	Shader* LandShader;

public:

	Maya land;
	int verx, verz;
	//el nombre numerico de la indiceTextura1 en cuestion, por lo pronto una
	unsigned int planoTextura;
	unsigned int m_textureID;
	unsigned int m_textureID2;

	Land(HWND hWnd, OpenGLClass* OpenGL, TextureClass* LoaderTextureReference, const wchar_t* mapaDeAlturaHeightmap, string indiceTextura1, string indiceTextura2,float anchoDeseado, float profundidadDeseada, int numtext, int numtext2)
	{

		//int Ancho = 0, Alto = 0;
		//int Pixeles = 0;
		//unsigned int Direccion = 0;
		//LoaderTextureReference->CargaBitmapYRegresaDatos(mapaDeAlturaHeightmap, Ancho, Alto, Pixeles, Direccion);

		//unsigned int* Dir = &Direccion;

		//anchoFinal = anchoDeseado;
		//profunidadFinal = profundidadDeseada;
		//// Cargamos el mapa de alturas(Heightmap) de la figura
		////Carga(mapaDeAlturaHeightmap);
		////en caso del puntero de la imagen sea nulo se brinca esta opcion
		//land = Plano(Ancho, Alto, anchoDeseado, profundidadDeseada, Dir, 80);
		//deltax = anchoFinal / Ancho;
		//deltaz = profunidadFinal / Alto;
		//verx = Ancho;
		//verz = Alto;


		anchoFinal = anchoDeseado;
		profunidadFinal = profundidadDeseada;
		// Cargamos el mapa de alturas(Heightmap) de la figura
		Carga(mapaDeAlturaHeightmap);
		//en caso del puntero de la imagen sea nulo se brinca esta opcion
		land = Plano(Ancho(), Alto(), anchoDeseado, profundidadDeseada, Dir_Imagen(), 80);
		deltax = anchoFinal / Ancho();
		deltaz = profunidadFinal / Alto();
		verx = Ancho();
		verz = Alto();
		// Disponemos la Textura1 del gdi.
		Descarga();

		// Aloja un objeto de vertices para OpenGL.
		OpenGL->glGenVertexArrays(1, &m_vertexArrayId);

		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
		OpenGL->glBindVertexArray(m_vertexArrayId);

		// Generate an ID for the vertex buffer.
		OpenGL->glGenBuffers(1, &m_vertexBufferId);

		// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glBufferData(GL_ARRAY_BUFFER, cantVert * sizeof(Vertices),
			land.maya, GL_STATIC_DRAW);

		// Enable the three vertex array attributes.
		OpenGL->glEnableVertexAttribArray(0);  // Vertex position.
		OpenGL->glEnableVertexAttribArray(1);  // Normals.
		OpenGL->glEnableVertexAttribArray(2);  // Texture coordinates.
		OpenGL->glEnableVertexAttribArray(3);  // Tangent.
		OpenGL->glEnableVertexAttribArray(4);  // BiNormals.

		// Specify the location and format of the position portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertices), 0);

		// Specify the location and format of the normal vector portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (3 * sizeof(float)));

		// Specify the location and format of the texture coordinate portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (6 * sizeof(float)));

		// Specify the location and format of the texture coordinate portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (8 * sizeof(float)));

		// Specify the location and format of the texture coordinate portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (11 * sizeof(float)));

		// Generate an ID for the index buffer.
		OpenGL->glGenBuffers(1, &m_indexBufferId);

		// Bind the index buffer and load the index data into it.
		OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
		OpenGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, cantIndices * sizeof(unsigned int), 
			land.indices, GL_STATIC_DRAW);

		m_textureID = LoaderTextureReference->Initialize(indiceTextura1, true);

		m_textureID2 = LoaderTextureReference->Initialize(indiceTextura2, true);
	}

	Land()
	{}

	~Land()
	{
		// Now that the buffers have been loaded we can release the array data.
		if (land.maya) {
			delete[] land.maya;
			land.maya = 0;
		}
		if (land.indices) {
			delete[] land.indices;
			land.indices = 0;
		}
		//nos aseguramos de disponer de los recursos previamente reservados			
		glDeleteTextures(1, &planoTextura);
	}
	
	void Shutdown() {

	}

	float Superficie(float x, float z)
	{
		//obtenemos el indice pero podria incluir una fraccion
		float indicefx = (x + anchoFinal / 2) / deltax;
		float indicefz = (z + profunidadFinal / 2) / deltaz;
		//nos quedamos con solo la parte entera del indice
		int indiceix = (int)indicefx;
		int indiceiz = (int)indicefz;
		//nos quedamos con solo la fraccion del indice
		float difx = indicefx - indiceix;
		float difz = indicefz - indiceiz;

		float altura;
		float D;

		//el cuadro del land esta formado por dos triangulos, si difx es mayor que dify 
		//entonces estamos en el triangulo de abajo en caso contrario arriba
		if (difx > difz)
		{
			//obtenemos el vector 1 de dos que se necesitan
			VectorRR v1(land.maya[indiceix + 1 + (indiceiz + 1) * verx].Posx - land.maya[indiceix + indiceiz * verx].Posx,
				land.maya[indiceix + 1 + (indiceiz + 1) * verx].Posy - land.maya[indiceix + indiceiz * verx].Posy,
				land.maya[indiceix + 1 + (indiceiz + 1) * verx].Posz - land.maya[indiceix + indiceiz * verx].Posz);
			//obtenemos el vector 2 de dos
			VectorRR v2(land.maya[indiceix + 1 + indiceiz * verx].Posx - land.maya[indiceix + indiceiz * verx].Posx,
				land.maya[indiceix + 1 + indiceiz * verx].Posy - land.maya[indiceix + indiceiz * verx].Posy,
				land.maya[indiceix + 1 + indiceiz * verx].Posz - land.maya[indiceix + indiceiz * verx].Posz);

			//con el producto punto obtenemos la normal y podremos obtener la ecuacion del plano
			//la parte x de la normal nos da A, la parte y nos da B y la parte z nos da C
			VectorRR normalPlano = Cruz(v1, v2);
			//entonces solo falta calcular D
			D = -1 * (normalPlano.X * land.maya[indiceix + indiceiz * verx].Posx +
				normalPlano.Y * land.maya[indiceix + indiceiz * verx].Posy +
				normalPlano.Z * land.maya[indiceix + indiceiz * verx].Posz);
			//sustituyendo obtenemos la altura de contacto en el land
			altura = ((-normalPlano.X * x - normalPlano.Z * z - D) / normalPlano.Y);
		}
		else
		{
			VectorRR v1(land.maya[indiceix + (indiceiz + 1) * verx].Posx - land.maya[indiceix + indiceiz * verx].Posx,
				land.maya[indiceix + (indiceiz + 1) * verx].Posy - land.maya[indiceix + indiceiz * verx].Posy,
				land.maya[indiceix + (indiceiz + 1) * verx].Posz - land.maya[indiceix + indiceiz * verx].Posz);

			VectorRR v2(land.maya[indiceix + 1 + (indiceiz + 1) * verx].Posx - land.maya[indiceix + indiceiz * verx].Posx,
				land.maya[indiceix + 1 + (indiceiz + 1)* verx].Posy - land.maya[indiceix + indiceiz * verx].Posy,
				land.maya[indiceix + 1 + (indiceiz + 1)* verx].Posz - land.maya[indiceix + indiceiz * verx].Posz);


			VectorRR normalPlano = Cruz(v1, v2);

			D = -1 * (normalPlano.X * land.maya[indiceix + indiceiz * verx].Posx +
				normalPlano.Y * land.maya[indiceix + indiceiz * verx].Posy +
				normalPlano.Z * land.maya[indiceix + indiceiz * verx].Posz);

			altura = ((-normalPlano.X * x - normalPlano.Z * z - D) / normalPlano.Y);
		}

		return altura;

	}

	///////////////////////////////////////////////////////////////////////////////////////////
	   

	void Shutdown(OpenGLClass* OpenGL)
	{

		// Release the vertex and index buffers.
		ShutdownBuffers(OpenGL);

		return;
	}


	void Render(OpenGLClass* OpenGL)
	{
		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
		RenderBuffers(OpenGL);

		return;
	}

	void Render(OpenGLClass* OpenGL, float* worldMatrix, float* viewMatrix, float* projectionMatrix) {
		// Seleccionar el shader para el dibujo del terreno
		LandShader->SetShader();
		LandShader->PonMatriz4x4((char*)"worldMatrix", worldMatrix);
		LandShader->PonMatriz4x4((char*)"viewMatrix", viewMatrix);
		LandShader->PonMatriz4x4((char*)"projectionMatrix", projectionMatrix);

		LandShader->Pon1Entero((char*)"shaderTexture", m_textureID);
		LandShader->Pon1Entero((char*)"shaderTexture2", m_textureID2);

		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
		RenderBuffers(OpenGL);

		return;
	}

	void ShutdownBuffers(OpenGLClass* OpenGL)
	{
		// Disable the two vertex array attributes.
		OpenGL->glDisableVertexAttribArray(0);
		OpenGL->glDisableVertexAttribArray(1);

		// Release the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, 0);
		OpenGL->glDeleteBuffers(1, &m_vertexBufferId);

		// Release the index buffer.
		OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		OpenGL->glDeleteBuffers(1, &m_indexBufferId);

		// Release the vertex array object.
		OpenGL->glBindVertexArray(0);
		OpenGL->glDeleteVertexArrays(1, &m_vertexArrayId);

		return;
	}
	
	void RenderBuffers(OpenGLClass* OpenGL)
	{
		// Bind the vertex array object that stored all the information about the vertex and index buffers.
		OpenGL->glBindVertexArray(m_vertexArrayId);

		// Render the vertex buffer using the index buffer.
		glDrawElements(GL_TRIANGLES, cantIndices, GL_UNSIGNED_INT, 0);

		return;
	}	

	void SetLandShader(Shader* LandShader) {
		this->LandShader = LandShader;
	}

	Shader* GetLandShader() {
		return this->LandShader;
	}
};

#endif 