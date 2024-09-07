//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader

static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* vShaderVerde = "shaders/shaderVerde.vert";
static const char* vShaderVerdeOscuro = "shaders/shaderverdeoscuro.vert";
static const char* vShaderCafe = "shaders/shadercafe.vert";


//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {

		//A 8*3=24 24*6=144	
		//X		Y		Z				R		G		B
		-0.9f,	-0.5f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.7f,	-0.5f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.7f,	 0.5f,	0.5f,		1.0f,	0.7f,	0.7f,

		-0.5f,	-0.5f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.3f,	-0.5f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.5f,	 0.5f,	0.5f,		1.0f,	0.7f,	0.7f,

		-0.7f,	-0.1f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.5f,	 0.5f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.7f,	 0.5f,	0.5f,		1.0f,	0.7f,	0.7f,

		-0.5f,	-0.1f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.7f,	 0.5f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.5f,	 0.5f,	0.5f,		1.0f,	0.7f,	0.7f,

		-0.7f,	-0.2f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.5f,	-0.2f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.7f,	-0.1f,	0.5f,		1.0f,	0.7f,	0.7f,

		-0.5f,	-0.2f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.5f,	-0.1f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.7f,	-0.1f,	0.5f,		1.0f,	0.7f,	0.7f,

		-0.5f,	-0.5f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.55f, -0.2f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.5f,	-0.2f,	0.5f,		1.0f,	0.7f,	0.7f,

		-0.7f,	-0.5f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.7f,	-0.2f,	0.5f,		1.0f,	0.7f,	0.7f,
		-0.65f, -0.2f,	0.5f,		1.0f,	0.7f,	0.7f,

		//C 10*3=30 30*6=180
		 0.1f,	 0.2f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.3f,	 0.2f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.3f,	 0.5f,	0.5f,		0.7f,	0.2f,	0.4f,

		 0.1f,	 0.2f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.1f,	 0.5f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.3f,	 0.5f,	0.5f,		0.7f,	0.2f,	0.4f,

		 0.1f,	 0.3f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.1f,	 0.5f,	0.5f,		0.7f,	0.2f,	0.4f,
		-0.2f,	 0.5f,	0.5f,		0.7f,	0.2f,	0.4f,

		 0.1f,	 0.3f,	0.5f,		0.7f,	0.2f,	0.4f,
		-0.2f,	 0.3f,	0.5f,		0.7f,	0.2f,	0.4f,
		-0.2f,	 0.5f,	0.5f,		0.7f,	0.2f,	0.4f,

		 0.0f,	 0.3f,	0.5f,		0.7f,	0.2f,	0.4f,
		-0.2f,	 0.3f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.0f,	-0.3f,	0.5f,		0.7f,	0.2f,	0.4f,

		-0.2f,	 0.3f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.0f,	-0.3f,	0.5f,		0.7f,	0.2f,	0.4f,
		-0.2f,	-0.3f,	0.5f,		0.7f,	0.2f,	0.4f,

		-0.2f,	-0.3f,	0.5f,		0.7f,	0.2f,	0.4f,
		-0.2f,	-0.5f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.1f,	-0.5f,	0.5f,		0.7f,	0.2f,	0.4f,

		-0.2f,	-0.3f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.1f,	-0.3f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.1f,	-0.5f,	0.5f,		0.7f,	0.2f,	0.4f,

		 0.1f,	-0.2f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.1f,	-0.5f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.3f,	-0.5f,	0.5f,		0.7f,	0.2f,	0.4f,

		 0.1f,	-0.2f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.3f,	-0.2f,	0.5f,		0.7f,	0.2f,	0.4f,
		 0.3f,	-0.5f,	0.5f,		0.7f,	0.2f,	0.4f,

		 //B 17*3=51  51*6=306  
		 0.4f,	 0.5f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.4f,	-0.5f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.6f,	-0.5f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.4f,	 0.5f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.6f,	-0.5f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.6f,	 0.5f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.6f,	 0.3f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.6f,	 0.5f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.8f,	 0.5f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.6f,    0.3f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.8f,	 0.5f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.9f,	 0.4f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.6f,	 0.3f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.9f,	 0.4f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.9f,	 0.3f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.7f,	 0.3f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.9f,	 0.3f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.9f,	 0.1f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.7f,	 0.3f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.9f,	 0.1f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.8f,	 0.0f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.8f,	 0.0f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.7f,	-0.2f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.7f,	 0.3f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.7f,	-0.2f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.7f,	 0.2f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.6f,	 0.2f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.7f,	-0.2f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.6f,	-0.2f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.6f,	 0.2f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.8f,	 0.0f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.9f,	-0.1f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.7f,	-0.2f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.9f,	-0.1f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.9f,	-0.2f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.7f,	-0.2f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.9f,	-0.2f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.9f,	-0.4f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.7f,	-0.2f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.9f,	-0.4f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.8f,	-0.5f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.7f,	-0.2f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.7f,	-0.2f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.8f,	-0.5f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.7f,	-0.5f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.7f,	-0.3f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.7f,	-0.5f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.6f,	-0.3f,	0.5f,		0.7f,	0.6f,	0.7f,

		 0.6f,	-0.3f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.6f,	-0.5f,	0.5f,		0.7f,	0.6f,	0.7f,
		 0.7f,	-0.5f,	0.5f,		0.7f,	0.6f,	0.7f,

		 //total 144+180+306=630

	};
	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 630);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,

	};
	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};
	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);
}


void CreateShaders()
{
	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	//azul [2]
	Shader* azul = new Shader();//shader para usar color azul
	azul->CreateFromFiles(vShaderAzul, fShaderColor);
	shaderList.push_back(*azul);

	//rojo [3]
	Shader* rojo = new Shader();//shader para usar color rojo
	rojo->CreateFromFiles(vShaderRojo, fShaderColor);
	shaderList.push_back(*rojo);

	//verde [4]
	Shader* verde = new Shader();//shader para usar color verde
	verde->CreateFromFiles(vShaderVerde, fShaderColor);
	shaderList.push_back(*verde);

	//verde oscuro [5]
	Shader* verdoscuro = new Shader();//shader para usar color verde oscuro 
	verdoscuro->CreateFromFiles(vShaderVerdeOscuro, fShaderColor);
	shaderList.push_back(*verdoscuro);

	//cafe  [6]
	Shader* cafe = new Shader();//shader para usar color cafe
	cafe->CreateFromFiles(vShaderCafe, fShaderColor);
	shaderList.push_back(*cafe);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		////////////////////////////////

		//LETAS ACB 
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.47f, -1.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.005f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		///////////////////////////////

		//PIRAMIDE AZUL
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.1f, -4.0f));
		model = glm::scale(model, glm::vec3(3.5f, 0.9f, 0.5f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		/////////////////////////////////

		//CUBO ROJO
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, -4.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.8f, 0.5f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		/////////////////////////////////

		//CUBO VERDE DERECHO
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		angulo += 0.01;

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.15f, 0.0020f, -1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.005f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//CUBO VERDE IZQUIERDO
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.15f, 0.0020f, -1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.005f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshList[1]->RenderMesh();

		//CUBO VERDE PUERTA
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.45f, -1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.005f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshList[1]->RenderMesh();

		/////////////////////

		//CUBO ARBOL CAFE
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.75f, -2.5f, -5.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.1f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// cubo cafe izquierdo 
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.75f, -2.5f, -5.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.1f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();



		////////////////////////////////

		//PIRAMIDE ARBOL DERECHO
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		angulo += 0.01;

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.2f, -0.8f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 0.5f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//PIRAMIDE ARBOL IZQUIERDO
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.2f, -0.8f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 0.5f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}