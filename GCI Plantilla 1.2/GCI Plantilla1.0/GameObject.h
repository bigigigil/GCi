#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_
#include "openglclass.h"
#include "ShaderBoundingBox.h"
#include "lightshaderclass.h"
#include "Model.h"
#include "TextureLoaderForOpenGL.h"

class GameObject {
private:
	OpenGLClass* OpenGL;
	TextureClass* LoaderTexRef;
	Model* model;
	HWND hwnd;
	const char* ObjectPath;
	string TexturePath1;
	string TexturePath2;
	string TexturePath3;
	string TexturePath4;
	string NormalMapPath;
	float * worldmatrix;
	Shader* ShaderOfModel;
public:
	GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef);
	GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef, const char* objPath, string texturePath, string normalMappingPath);
	GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef, const char* objPath, string texturePath);
	~GameObject();
	bool Initialize();
	bool SetShaderModel(Shader* ModelShader);
	bool AddTexture(string TextureAditional);
	void ChangeTexture(int From, int To);
	float* GetWorldMatrix();
	void Render(float* viewMatrix, float* projectionMatrix);
	float* MultiplyVectorMatrix(float*,float*);
	float scale;
	void Shutdown();
};

#endif