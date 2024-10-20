#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//dado
float rotardadoX = 0.0f;
float rotardadoY = 0.0f;
float rotardadoZ = 0.0f;
float movdado = 30.0f; 
static int dadoResultado = 1;
bool rod = false; 

//helicoptero
float posX = 0.0f;
float posY = 0.0f;
float rotacionHelicoptero = 0.0f; 
float velocidad = 0.5f; 
static int esquinaActual = 0;

Model heli1;
Model heli2;
Model copter;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture pisoTexture;
Texture tableroTexture;
Texture dado_diez;



Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;



//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj1);

	unsigned int tableroIndices[] = {
		8, 9, 10,
		11, 10, 9,

		4, 5, 6,
		7, 6, 5,

		0, 2, 4,
		2, 4, 6,

		0, 1, 4,
		1, 4, 5,

		1, 3, 5,
		3, 5, 7,

		2, 3, 6,
		3, 6, 7
	};

	GLfloat tableroVertices[] = {
		//sin textura
		-1.0f,	0.0f,	-1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f,	0.0f,	-1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-1.0f,	0.0f,	1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f,	0.0f,	1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,

		-1.0f,	-1.0f,	-1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f,	-1.0f,	-1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-1.0f,	-1.0f,	1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f,	-1.0f,	1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,

		//con textura
		-1.0f,	0.0f,	-1.0f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f,	0.0f,	-1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-1.0f,	0.0f,	1.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		1.0f,	0.0f,	1.0f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	};

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(tableroVertices, tableroIndices, 96, 36);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
	0, 1, 2,
	3, 4, 5,
	6, 7, 8,
	9, 10, 11,
	12, 13, 14,
	15, 16, 17,
	18, 19, 20,
	21, 22, 23,
	24, 25, 26,
	27, 28, 29,
	30, 31, 32,
	33, 34, 35,
	36, 37, 38,
	39, 40, 41,
	42, 43, 44,
	45, 46, 47,
	48, 49, 50,
	51, 52, 53,
	54, 55, 56,
	57, 58, 59,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		//x			y		z			S		T			NX		NY		NZ
		//1
		 0.0f,		1.0f,	0.0f,		0.1f,	1.0f,		1.0f,	0.0f,	0.0f,
		 0.309f,	0.1f,	0.951f,		0.18f,	0.609f,		1.0f,	0.0f,	0.0f,
		-0.809f,	0.1f,	0.588f,		0.02f,	0.609f,		1.0f,	0.0f,	0.0f,

		 0.309f,	0.1f,	0.951f,		0.900f, -2.98f,		1.0f,	0.0f,	0.0f,
		 0.809f,   -0.1,	0.588f,		0.905f, 0.364f,		1.0f,	0.0f,	0.0f,
		-0.309f,   -0.1,	0.951f,		0.902f, 0.364f,		1.0f,	0.0f,	0.0f,

		//x			y		z			S		T			NX		NY		NZ
		//2				
		 0.0f,		1.0f,	0.0f,		0.3f,	0.9f,		1.0f,	0.0f,	0.0f,
		-0.809f,	0.1f,	0.588f,		0.38f,	0.609f,		1.0f,	0.0f,	0.0f,
		-0.809f,	0.1f,  -0.588f,		0.23f,	0.609f,		1.0f,	0.0f,	0.0f,

		-0.309f,	-0.1,	0.951f,		0.1f,	1.0f,		1.0f,	0.0f,	0.0f,
		 0.309f,	0.1f,	0.951f,		0.12f,	0.8f,		1.0f,	0.0f,	0.0f,
		-0.809f,	0.1f,	0.588f,		0.08f,	0.8f,		1.0f,	0.0f,	0.0f,

		//x			y		z			S		T			NX		NY		NZ
		//3							
		 0.0f,		1.0f,	0.0f,		0.5f,	0.9f,		1.0f,	0.0f,	0.0f,
		-0.809f,	0.1f,  -0.588f,		0.58f,	0.609f,		1.0f,	0.0f,	0.0f,
		 0.309f,	0.1f,  -0.951f,		0.42f,	0.609f,		1.0f,	0.0f,	0.0f,

		-0.809f,	0.1f,	0.588f,		0.102f, 0.365f,		1.0f,	0.0f,	0.0f,
		-0.309f,   -0.1,	0.951f,		0.103f, 0.364f,		1.0f,	0.0f,	0.0f,
		-1.0f,	   -0.1,	0.0f,		0.104f, 0.364f,		1.0f,	0.0f,	0.0f,

		//x			y		z			S		T			NX		NY		NZ
		//4						
		 0.0f,		1.0f,	0.0f,		0.7f,	0.9f,		1.0f,	0.0f,	0.0f,
		 0.309f,	0.1f,  -0.951f,		0.78f,	0.609f,		1.0f,	0.0f,	0.0f,
		 1.0f,		0.1f,	0.0f,		0.62f,	0.609f,		1.0f,	0.0f,	0.0f,

		-1.0f,	   -0.1,	0.0f,		0.302f, 0.865f,		1.0f,	0.0f,	0.0f,
		-0.809f,	0.1f,	0.588f,		0.303f, 0.864f,		1.0f,	0.0f,	0.0f,
		-0.809f,	0.1f,  -0.588f,		0.304f, 0.864f,		1.0f,	0.0f,	0.0f,

		//x			y		z			S		T			NX		NY		NZ
		//5							
		 0.0f,		1.0f,	0.0f,		0.9f,	0.9f,		1.0f,	0.0f,	0.0f,
		 1.0f,		0.1f,	0.0f,		0.98f,	0.609f,		1.0f,	0.0f,	0.0f,
		 0.309f,	0.1f,	0.951f,		0.82f,	0.609f,		1.0f,	0.0f,	0.0f,

		-0.809f,	0.1f,  -0.588f,		0.302f, 0.365f,		1.0f,	0.0f,	0.0f,
		-1.0f,	   -0.1,	0.0f,		0.303f, 0.364f,		1.0f,	0.0f,	0.0f,
		-0.309f,   -0.1,   -0.951f,		0.304f, 0.364f,		1.0f,	0.0f,	0.0f,

		//x			y		z			S		T			NX		NY		NZ
		//6							
		 0.0f,	   -1.0,	0.0f,		0.1f,	0.4f,		1.0f,	0.0f,	0.0f,
		-0.309f,   -0.1,	0.951f,		0.02f,	0.108f,		1.0f,	0.0f,	0.0f,
		-1.0f,     -0.1,	0.0f,		0.18f,	0.108f,		1.0f,	0.0f,	0.0f,

		-0.309f,   -0.1,   -0.951f,		0.502f, 0.865f,		1.0f,	0.0f,	0.0f,
		-0.809f,	0.1f,  -0.588f,		0.503f, 0.864f,		1.0f,	0.0f,	0.0f,
		 0.309f,	0.1f,  -0.951f,		0.504f, 0.864f,		1.0f,	0.0f,	0.0f,

		 //x			y		z			S		T			NX		NY		NZ
		 //7							
		  0.0f,     -1.0,	0.0f,		0.3f,	0.4f,		1.0f,	0.0f,	0.0f,
		 -1.0f,	   -0.1,	0.0f,		0.22f,	0.108f,		1.0f,	0.0f,	0.0f,
		 -0.309f,   -0.1,   -0.951f,		0.38f,	0.108f,		1.0f,	0.0f,	0.0f,

		  0.309f,	0.1f,  -0.951f,		0.502f, 0.365f,		1.0f,	0.0f,	0.0f,
		 -0.309f,   -0.1,   -0.951f,		0.503f, 0.364f,		1.0f,	0.0f,	0.0f,
		  0.809f,   -0.1,   -0.588f,		0.504f, 0.364f,		1.0f,	0.0f,	0.0f,

		  //x			y		z			S		T			NX		NY		NZ
		  //8							
		   0.0f,	   -1.0,	0.0f,		0.5f,	0.4f,		1.0f,	0.0f,	0.0f,
		  -0.309f,   -0.1,   -0.951f,		0.42f,	0.108f,		1.0f,	0.0f,	0.0f,
		   0.809f,   -0.1,   -0.588f,		0.58f,	0.108f,		1.0f,	0.0f,	0.0f,

		   0.809f,   -0.1,   -0.588f,		0.702f, 0.865f,		1.0f,	0.0f,	0.0f,
		   0.309f,	0.1f,  -0.951f,		0.703f, 0.864f,		1.0f,	0.0f,	0.0f,
		   1.0f,		0.1f,	0.0f,		0.704f, 0.864f,		1.0f,	0.0f,	0.0f,

		   //x			y		z			S		T			NX		NY		NZ
		   //9								
			0.0f,	   -1.0,	0.0f,		0.7f,	0.4f,		1.0f,	0.0f,	0.0f,
			0.809f,   -0.1,   -0.588f,		0.62f,	0.108f,		1.0f,	0.0f,	0.0f,
			0.809f,   -0.1,	0.588f,		0.78f,	0.108f,		1.0f,	0.0f,	0.0f,

			1.0f,		0.1f,	0.0f,		0.702f, 0.365f,		1.0f,	0.0f,	0.0f,
			0.809f,   -0.1,   -0.588f,		0.703f, 0.364f,		1.0f,	0.0f,	0.0f,
			0.809f,   -0.1,	0.588f,		0.704f, 0.364f,		1.0f,	0.0f,	0.0f,

			//x		y		z			S		T			NX		NY		NZ
			//10
			0.0f,	   -1.0,	0.0f,		0.9f,	0.4f,		1.0f,	0.0f,	0.0f,
			0.809f,   -0.1,	0.588f,		0.82f,	0.108f,		1.0f,	0.0f,	0.0f,
		   -0.309f,   -0.1,	0.951f,		0.98f,	0.108f,		1.0f,	0.0f,	0.0f,

			0.809f,   -0.1,	0.588f,		0.502f, 0.865f,		1.0f,	0.0f,	0.0f,
			1.0f,		0.1f,	0.0f,		0.503f, 0.864f,		1.0f,	0.0f,	0.0f,
			0.309f,	0.1f,	0.951f,		0.504f, 0.864f,		1.0f,	0.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 480, 60);
	meshList.push_back(dado);

}

//heli
void inicializar(float& objetivoX, float& objetivoY) {
	switch (esquinaActual) {
	case 0: objetivoX = 0.0f; objetivoY = 0.0f; break; 
	case 1: objetivoX = 15.0f; objetivoY = 0.0f; break;
	case 2: objetivoX = 15.0f; objetivoY = 15.0f; break; 
	case 3: objetivoX = 0.0f; objetivoY = 15.0f; break; 
	}
}

//mov
void moverHelicoptero(float deltaTime) {
	float objetivoX, objetivoY;
	inicializar(objetivoX, objetivoY); 
	float dirX = objetivoX - posX;
	float dirY = objetivoY - posY;
	float distancia = sqrt(dirX * dirX + dirY * dirY);

	if (distancia > 0.1f) { 
		dirX /= distancia;
		dirY /= distancia;

		posX += dirX * velocidad * deltaTime;
		posY += dirY * velocidad * deltaTime;

		
		rotacionHelicoptero = atan2(dirY, dirX) * (180.0f / 3.14159f);
	}
	else {
		esquinaActual = (esquinaActual + 1) % 4;
	}
}

int main()
{
	srand(time(0));
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CreateShaders();
	

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	tableroTexture = Texture("Textures/tablero.jpg");
	tableroTexture.LoadTextureA();
	dado_diez = Texture("Textures/carasdadodiez.png");
	dado_diez.LoadTextureA();

	heli1 = Model();
	heli1.LoadModel("Models/heli1.obj");
	heli2 = Model();
	heli2.LoadModel("Models/heli2.obj");
	copter = Model();
	copter.LoadModel("Models/copter.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.7f, 1.0f,
		0.0f, 0.0f, -1.0f);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//dado
		if (mainWindow.getsKeys()[GLFW_KEY_H] && !rod) {
			dadoResultado = rand() % 10 + 1; 
			rotardadoX = 0.0f;
			rotardadoY = 0.0f;
			rotardadoZ = 0.0f; 
			movdado = 50.0f; 
			rod = true;
		}

		if (rod) {
			movdado -= 0.2f * deltaTime;
			rotardadoX += (rand() % 10 + 5) * deltaTime;
			rotardadoY += (rand() % 10 + 5) * deltaTime;

			if (movdado <= 3.4f) {
				movdado = 3.4f;
				rod = false;
				switch (dadoResultado) {
				case 1: rotardadoY = 0.0f; break;      
				case 2: rotardadoY = 90.0f; break;    
				case 3: rotardadoY = 180.0f; break;    
				case 4: rotardadoY = 270.0f; break;   
				case 5: rotardadoX = 90.0f; break;     
				case 6: rotardadoX = -90.0f; break;   
				case 7: rotardadoZ = 90.0f; break;    
				case 8: rotardadoZ = -90.0f; break;    
				case 9: rotardadoY = 45.0f; break;     
				case 10: rotardadoY = 135.0f; break;  
				}
			}
		}

		
		moverHelicoptero(deltaTime);

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);

		glm::mat4 modelaux(1.0);

		float escalaTablero = 15.0f;

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//piso
		glm::mat4 model(1.0);
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		pisoTexture.UseTexture();
		meshList[0]->RenderMesh();

		//tablero
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(0.0f, 1.01f, 0.0f));
		model = glm::scale(model, glm::vec3(escalaTablero, 1.0f, escalaTablero));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		tableroTexture.UseTexture();
		meshList[1]->RenderMesh();

		//dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, movdado, -2.0f));
		model = glm::rotate(model, glm::radians(rotardadoX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotardadoY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotardadoZ), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado_diez.UseTexture();
		meshList[2]->RenderMesh(); 

		//heli 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posX, posY, 6.0f)); // Ajustar la altura
		model = glm::rotate(model, glm::radians(rotacionHelicoptero), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación sobre Y
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		copter.RenderModel(); // Renderiza el modelo del helicóptero

		//heli1
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 5.2f, 1.7));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		heli1.RenderModel();

		//heli2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -2.4));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		heli2.RenderModel();

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
