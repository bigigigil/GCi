#include "GameObject.h"


GameObject::GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef) {
	this->OpenGL = OpenGL;
	this->hwnd = hwnd;
	this->LoaderTexRef = LoaderTextureRef;
	this->worldmatrix = 0;
	ShaderOfModel = 0;
}

GameObject::GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef, const char* objPath, string texturePath, string normalMappingPath) {
	this->OpenGL = OpenGL;
	this->LoaderTexRef = LoaderTextureRef;
	this->worldmatrix = 0;
	this->hwnd = hwnd;
	ObjectPath = objPath;
	TexturePath1 = texturePath;
	NormalMapPath = normalMappingPath;
	ShaderOfModel = 0;
}

GameObject::GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef, const char* objPath, string texturePath) {
	this->OpenGL = OpenGL;
	this->hwnd = hwnd;
	this->LoaderTexRef = LoaderTextureRef;
	this->worldmatrix = 0;
	ObjectPath = objPath;
	TexturePath1 = texturePath;
	ShaderOfModel = 0;
}

GameObject::~GameObject() {
}

float * GameObject::GetWorldMatrix() {
	return this->worldmatrix;
}

bool GameObject::SetShaderModel(Shader* ModelShader) {
	ShaderOfModel = ModelShader;
	return true;
}

bool GameObject::AddTexture(string texturePath) {
	try {;
	model->AddTexture(texturePath, true);
		return true;
	}
	catch (exception e) {
		printf("Error,&s", e);
		return false;
	}
}

bool GameObject::Initialize() {
	bool result = true;
	string resultDetail = "";

	worldmatrix = new float[16] { 0.0f };
	if (!OpenGL) {
		result = false;
		_RPT1(0, "Alert! GameObject OpenGL does not defined. \n", 0);
		return result;
	}
	else
		OpenGL->BuildIdentityMatrix(worldmatrix);
	model = new Model(OpenGL, hwnd, LoaderTexRef);
	model->Initialize(ObjectPath, TexturePath1);
	if (!result) {
		_RPT1(0, "Alert! GameObject Model does not initializdd. \n", 0);
		return result;
	}

	return result;
}

void GameObject::ChangeTexture(int From, int To) {
	model->ChangeTexture(From, To);
}

void GameObject::Render(float* viewMatrix,float* projectionMatrix) {
	
	if (model) {
		ShaderOfModel->SetShader();
		ShaderOfModel->PonMatriz4x4((char*)"worldMatrix", worldmatrix);
		ShaderOfModel->PonMatriz4x4((char*)"viewMatrix", viewMatrix);
		ShaderOfModel->PonMatriz4x4((char*)"projectionMatrix", projectionMatrix);
		model->Render(ShaderOfModel);
	}
}

float * GameObject::MultiplyVectorMatrix(float*matrix, float*vector) {
	float* vr = new float[4] {0.0f};
	vr[3] = 1.0f;
	vr[0] = vector[0] * matrix[0] + matrix[12];//matrix[0] * vector[0] + matrix[1] * vector[1] + matrix[2] * vector[2] + matrix[3] * vector[3];
	vr[1] = vector[1] * matrix[5] + matrix[13];//matrix[4] * vector[0] + matrix[5] * vector[1] + matrix[6] * vector[2] + matrix[7] * vector[3];
	vr[2] = vector[2] * matrix[10] + matrix[14];//matrix[8] * vector[0] + matrix[9] * vector[1] + matrix[10] * vector[2] + matrix[11] * vector[3];
	vr[3] = 1.0f;//matrix[12] * vector[0] + matrix[13] * vector[1] + matrix[14] * vector[2] + matrix[15] * vector[3];
	return vr;
}

void GameObject::Shutdown() {
	if (worldmatrix) {
		delete worldmatrix;
		worldmatrix = 0;
	}
	if (model)
		model = NULL;
}