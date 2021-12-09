/*---------------------------------------------------------*/
/* ----------------  Proyecto Final       -----------*/
/*-----------------    2022-1   --------------------------- */
/*------------- Alumno: De Jesus Moreno Yolanda---------------*/
/*------------- No. Cuenta      315088960   ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
#include <stdlib.h>
#include<fstream>
#include <string>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting //Mueve la luz	X	Y	Z
glm::vec3 lightPosition(0.0f, 0.0f, 0.0f);
glm::vec3 lightDirection(1.0f, 1.0f, 1.0f);
//glm::vec3 colorDirection(0.0f, 0.0f, 0.0f);//agregado

double myVariable = 0.0f;

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 0.0f,
movAuto_z = 0.0f,
orienta = 0.0f,
giroLlantas = 0.0f,
movAuto_xAux = 0.0f,
movAuto_zAux = 0.0f,

movCopo_x = 0.0f,
movCopo_z = 0.0f,
orientaCopo = 0.0f,
movCopo_xAux = 0.0f,
movCopo_zAux = 0.0f;

bool	animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false,
recorrido1Copo = true,
recorrido2Copo = false,
recorrido3Copo = false,
recorrido4Copo = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f,
		movBrazoIzq = 0.0f,
		movBrazoDer = 0.0f,
		rotRodDer = 0.0f,
		rotCabeza = 0.0f,

		posX_monito = 0.0f,
		posY_monito = 0.0f,
		posZ_monito = 0.0f,
		movBrazoIzq_monito = 0.0f,
		giroMonito_monito = 0.0f,
		movBrazoDer_monito = 0.0f,

		posX_kim = 0.0f,
		posY_kim = 0.0f,
		posZ_kim = 0.0f,
		rotRodIzq_kim = 0.0f,
		giroMonito_kim = 0.0f,
		movBrazoIzq_kim = 0.0f,
		movBrazoDer_kim = 0.0f,
		rotRodDer_kim = 0.0f,
		rotCabeza_kim = 0.0f;


float	incX = 0.0f, //incremento para cada componente
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f,
		movBrazoIzqINC = 0.0f,
		movBrazoDerINC = 0.0f,
		rotRodDerINC = 0.0f,
		rotCabezaINC = 0.0f,
	
		rotInc_monito = 0.0f,
		giroMonitoInc_monito = 0.0f,
		incX_monito = 0.0f, //incremento para cada componente
		incY_monito = 0.0f,
		incZ_monito = 0.0f,
		movBrazoIzqINC_monito = 0.0f,
		movBrazoDerINC_monito = 0.0f,

		incX_kim = 0.0f, //incremento para cada componente
		incY_kim = 0.0f,
		incZ_kim = 0.0f,
		rotInc_kim = 0.0f,
		giroMonitoInc_kim = 0.0f,
		movBrazoIzqINC_kim = 0.0f,
		movBrazoDerINC_kim = 0.0f,
		rotRodDerINC_kim = 0.0f,
		rotCabezaINC_kim = 0.0f;

#define MAX_FRAMES 20//cuentos cuadro claves puede guardar (Cuidar la memoria por los cuadro clave)
int i_max_steps = 60;//Cuadros intermedios generados (de los delgaditos), para que sea más rapida hay que poner menos
int i_curr_steps = 0;
typedef struct _frame//guarda y se tiene que ver reflejada aqui
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float movBrazoIzq;
	float movBrazoDer;
	float rotRodDer;
	float rotCabeza;

	float posX_monito;		//Variable para PosicionX
	float posY_monito;		//Variable para PosicionY
	float posZ_monito;		//Variable para PosicionZ
	float movBrazoIzq_monito;
	float movBrazoDer_monito;
	float giroMonito_monito;

	float posX_kim;		//Variable para PosicionX
	float posY_kim;		//Variable para PosicionY
	float posZ_kim;		//Variable para PosicionZ
	float rotRodIzq_kim;
	float giroMonito_kim;
	float movBrazoIzq_kim;
	float movBrazoDer_kim;
	float rotRodDer_kim;
	float rotCabeza_kim;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 20;			//introducir datos
bool play = false;
int playIndex = 0;

void escribir() {
	ofstream archivo;

	//Abrir archivo
	archivo.open("animacionDino.txt", ios::out);
	if (archivo.fail()) {
		cout << "Error en archivo";
		exit(1);
	}
	archivo << KeyFrame;
	archivo.close();
}



void saveFrame(void)//Esta funcion se manda a llamar con la tecla L
{
	printf("frameindex %d\n", FrameIndex);
	//lectura2();
	std::cout << "Frame Index = " << FrameIndex << std::endl;
	//escribir2();
	KeyFrame[FrameIndex].posX = posX;//se preservan para hacer operaciones
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].posX_monito = posX_monito;//se preservan para hacer operaciones
	KeyFrame[FrameIndex].posY_monito = posY_monito;
	KeyFrame[FrameIndex].posZ_monito = posZ_monito;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;
	
	KeyFrame[FrameIndex].movBrazoIzq = movBrazoIzq;//lo que tiene la variable va al arreglo
	KeyFrame[FrameIndex].movBrazoIzq = movBrazoIzq;

	KeyFrame[FrameIndex].movBrazoIzq_monito = movBrazoIzq_monito;//lo que tiene la variable va al arreglo
	KeyFrame[FrameIndex].movBrazoIzq_monito = movBrazoIzq_monito;
	KeyFrame[FrameIndex].giroMonito_monito = giroMonito_monito;

	KeyFrame[FrameIndex].movBrazoDer = movBrazoDer;
	KeyFrame[FrameIndex].rotRodDer = rotRodDer;
	KeyFrame[FrameIndex].rotCabeza = rotCabeza;
	//******************************KIM
	KeyFrame[FrameIndex].posX_kim = posX_kim;//se preservan para hacer operaciones
	KeyFrame[FrameIndex].posY_kim = posY_kim;
	KeyFrame[FrameIndex].posZ_kim = posZ_kim;

	KeyFrame[FrameIndex].rotRodIzq_kim = rotRodIzq_kim;
	KeyFrame[FrameIndex].giroMonito_kim = giroMonito_kim;

	KeyFrame[FrameIndex].movBrazoIzq_kim = movBrazoIzq_kim;//lo que tiene la variable va al arreglo
	KeyFrame[FrameIndex].movBrazoIzq_kim = movBrazoIzq_kim;
	KeyFrame[FrameIndex].movBrazoDer_kim = movBrazoDer_kim;
	KeyFrame[FrameIndex].rotRodDer_kim = rotRodDer_kim;
	KeyFrame[FrameIndex].rotCabeza_kim = rotCabeza_kim;

	FrameIndex++;
}

void resetElements(void)//Se llama cuando se ejecuta la animación en la tecla P
{
	posX = KeyFrame[0].posX;//lo que tengo almacenado en la clave cero
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	posX = KeyFrame[0].posX_monito;//lo que tengo almacenado en la clave cero
	posY = KeyFrame[0].posY_monito;
	posZ = KeyFrame[0].posZ_monito;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;

	movBrazoIzq=KeyFrame[0].movBrazoIzq;//sino reseteo va comenzar desde los valores que tiene actualmente
	movBrazoDer = KeyFrame[0].movBrazoDer;

	movBrazoIzq = KeyFrame[0].movBrazoIzq_monito;
	movBrazoDer = KeyFrame[0].movBrazoDer_monito;
	giroMonito_monito = KeyFrame[0].giroMonito_monito;

	rotRodDer = KeyFrame[0].rotRodDer;
	rotCabeza = KeyFrame[0].rotCabeza;

	//*************************KIM
	posX_kim = KeyFrame[0].posX_kim;//lo que tengo almacenado en la clave cero
	posY_kim = KeyFrame[0].posY_kim;
	posZ_kim = KeyFrame[0].posZ_kim;
	rotRodIzq_kim = KeyFrame[0].rotRodIzq_kim;
	giroMonito_kim = KeyFrame[0].giroMonito_kim;

	movBrazoIzq_kim = KeyFrame[0].movBrazoIzq_kim;//sino reseteo va comenzar desde los valores que tiene actualmente
	movBrazoDer_kim = KeyFrame[0].movBrazoDer_kim;
	rotRodDer_kim = KeyFrame[0].rotRodDer_kim;
	rotCabeza_kim = KeyFrame[0].rotCabeza_kim;
}

void interpolation(void)//hace la operacion del incremento
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	incX_monito = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX_monito) / i_max_steps;
	incY_monito = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY_monito) / i_max_steps;
	incZ_monito = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ_monito) / i_max_steps;

	//***********************KIM
	incX_kim = (KeyFrame[playIndex + 1].posX_kim - KeyFrame[playIndex].posX_kim) / i_max_steps;
	incY_kim = (KeyFrame[playIndex + 1].posY_kim - KeyFrame[playIndex].posY_kim) / i_max_steps;
	incZ_kim = (KeyFrame[playIndex + 1].posZ_kim - KeyFrame[playIndex].posZ_kim) / i_max_steps;

	//				inicio de los cuadros clade (playindex) de las animaciones
	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

	movBrazoIzqINC=(KeyFrame[playIndex + 1].movBrazoIzq - KeyFrame[playIndex].movBrazoIzq) / i_max_steps;
	movBrazoDerINC = (KeyFrame[playIndex + 1].movBrazoDer - KeyFrame[playIndex].movBrazoDer) / i_max_steps;

	movBrazoIzqINC_monito = (KeyFrame[playIndex + 1].movBrazoIzq_monito - KeyFrame[playIndex].movBrazoIzq_monito) / i_max_steps;
	movBrazoDerINC_monito = (KeyFrame[playIndex + 1].movBrazoDer_monito - KeyFrame[playIndex].movBrazoDer_monito) / i_max_steps;
	//rotInc_monito = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc_monito = (KeyFrame[playIndex + 1].giroMonito_monito - KeyFrame[playIndex].giroMonito_monito) / i_max_steps;

	rotRodDerINC = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;
	rotCabezaINC = (KeyFrame[playIndex + 1].rotCabeza - KeyFrame[playIndex].rotCabeza) / i_max_steps;

	//****************KIM
	rotInc_kim = (KeyFrame[playIndex + 1].rotRodIzq_kim - KeyFrame[playIndex].rotRodIzq_kim) / i_max_steps;
	giroMonitoInc_kim = (KeyFrame[playIndex + 1].giroMonito_kim - KeyFrame[playIndex].giroMonito_kim) / i_max_steps;
	movBrazoIzqINC_kim = (KeyFrame[playIndex + 1].movBrazoIzq_kim - KeyFrame[playIndex].movBrazoIzq_kim) / i_max_steps;
	movBrazoDerINC_kim = (KeyFrame[playIndex + 1].movBrazoDer_kim - KeyFrame[playIndex].movBrazoDer_kim) / i_max_steps;
	rotRodDerINC_kim = (KeyFrame[playIndex + 1].rotRodDer_kim - KeyFrame[playIndex].rotRodDer_kim) / i_max_steps;
	rotCabezaINC_kim = (KeyFrame[playIndex + 1].rotCabeza_kim - KeyFrame[playIndex].rotCabeza_kim) / i_max_steps;
}

void animate(void)
{				//Que tan amplio camina la luz en el 50
		lightPosition.y = 1000.0 * cos(myVariable); //anima la posición de x en proporsión de coseno.
		lightPosition.x = 900.0 * sin(myVariable);
		//lightPosition.z = 500.0; //* cos(myVariable);
		if (myVariable < 360.0)
			myVariable += 0.01; //le agregamos un incremento de 0.01 que tan rapido
		else
			myVariable = 0.0; ///Para evitar el des de mis variables
		//cout << "myVariable: " << myVariable << endl; //Para ver el comportamiento en la pantalla
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			posX_monito += incX_monito;
			posY_monito += incY_monito;
			posZ_monito += incZ_monito;

			posX_kim += incX_kim;
			posY_kim += incY_kim;
			posZ_kim += incZ_kim;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;
			movBrazoIzq += movBrazoIzqINC;//modificar el dibujo

			rotRodIzq_kim += rotInc_kim;
			giroMonito_kim += giroMonitoInc_kim;
			movBrazoIzq_kim += movBrazoIzqINC_kim;//modificar el dibujo

			movBrazoIzq_monito += movBrazoIzqINC_monito;//modificar el dibujo
			movBrazoDer_monito += movBrazoDerINC_monito;
			giroMonito_monito += giroMonitoInc_monito;

			movBrazoDer += movBrazoDerINC;
			rotRodDer += rotRodDerINC;
			rotCabeza += rotCabezaINC;

			movBrazoDer_kim += movBrazoDerINC_kim;
			rotRodDer_kim += rotRodDerINC_kim;
			rotCabeza_kim += rotCabezaINC_kim;

			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacion)
	{
		if (movAuto_z >= -276.0f && movAuto_xAux==0.0f) {
			orienta = 0.0f;
			movAuto_z -= 3.0f; // va hacia atras
			giroLlantas -= 3.0f;
			//cout << "movAuto_z UNO: " << movAuto_z << endl;
		 }
		else if(movAuto_xAux <= 354.0f && movAuto_z==-279.0f) {
			orienta = 90.0f;
			movAuto_xAux += 3.0f; // va hacia el frente controla la rapidez para reversa sería -=
			giroLlantas += 3.0f;
			//cout << "movAuto_x DOS: " << movAuto_xAux << endl;
			movAuto_x = movAuto_xAux;
		}
		else if (movAuto_z <= 273.0f && movAuto_xAux == 357.0f)
		{
			orienta = 0.0f;
			movAuto_z += 3.0f; // va hacia delante
			giroLlantas += 3.0f;
			//cout << "movAuto_z TRES: " << movAuto_z << endl;
		}
		else if (movAuto_z == 276.0f && movAuto_x >= 0.0f)
		{
			orienta = -90.0f;
			movAuto_x -= 3.0f;
			giroLlantas -= 3.0f;
			//cout << "movAuto_X CUATRO: " << movAuto_x << endl;
		}
		else if (movAuto_z <= 456.0f && movAuto_x == -3.0f)
		{ 
			orienta = 0.0f; 
			movAuto_z += 3.0f;
			giroLlantas += 3.0f;
			//cout << "movAuto_X CINCO: " << movAuto_z << endl;
		}
		else {
			animacion = false;
			}
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}




int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs"); //no me deja hacercarme al fondo
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs"); //Cargar modelo con ya animaciones desde ek sotfware de modelado

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	//Model casaMedieval1("resources/objects/casaMedieval1/casaMedieval1.obj"); //Carga al modelo en memoria <--- se agrega
	
	Model cabezaKim("resources/objects/kimPer1/cabezaKim.obj");
	Model cuerpoKim("resources/objects/kimPer1/cuerpoKim.obj");
	Model brazoDerKim("resources/objects/kimPer1/brazoDerkim.obj");
	Model monoDerKim("resources/objects/kimPer1/manoDerKim.obj");
	Model brazoIzqKim("resources/objects/kimPer1/brazoIzqKim.obj");
	Model manoIzqKim("resources/objects/kimPer1/manoIzqKim.obj");
	Model piernaIzqKim("resources/objects/kimPer1/piernaIzqKim.obj");
	Model pieIzqKim("resources/objects/kimPer1/pieIzqKim.obj");
	Model pieDerKim("resources/objects/kimPer1/pieDerKim.obj");

	Model piso("resources/objects/piso/piso.obj");
	Model brazoDerPer2("resources/objects/personaje2/brazoDerPer2_Tije.obj");
	Model brazoIzqPer2("resources/objects/personaje2/brazoIzqPer2_Tije.obj");
	Model torzoPer2("resources/objects/personaje2/torzoPer2.obj");
	Model cabezaPer2("resources/objects/personaje2/cabezaP2.obj");
	Model torCobPer2("resources/objects/personaje2/torCobPer2.obj");
	Model pieDerPer2("resources/objects/personaje2/pieDerPer2_Tije.obj");
	Model pieDerPerP("resources/objects/personaje2/piederPrueba.obj");
	Model pieIzqPer2("resources/objects/personaje2/pieIzqPer2_Tije.obj");
	
	Model torsoDino("resources/objects/dino/cuerpo.obj");
	Model pieDerDinD("resources/objects/dino/pataDerD.obj");
	Model pieIzqDinD("resources/objects/dino/pataIzqD.obj");
	Model pieIzqDinT("resources/objects/dino/pataIzqT.obj");
	Model pieDerDinT("resources/objects/dino/pataDerT.obj");

	//Model torPP2("resources/objects/dino/torPruebaP2.obj");
	Model arbolito("resources/objects/arbolito/arbolitoNavi.obj");
	Model cuerpo("resources/objects/personaje2/cuerpo.obj");
	Model manoDer("resources/objects/personaje2/manoDer.obj");
	Model manoIzq("resources/objects/personaje2/manoIzq.obj");

	Model casita("resources/objects/casaCom/casitaCompleta.obj");
	/*Model casaNuevo("resources/objects/casaNuevo/casaNuevo.obj");
	Model arbolN("resources/objects/arbolNormal/arbolNor.obj");
	Model cama("resources/objects/cama/cama.obj");
	Model buro("resources/objects/buro/buro.obj");
	Model tocador("resources/objects/tocador/tocador.obj");
	Model pino("resources/objects/pino/pinoN.obj");
	Model estrella("resources/objects/estrella/estrella.obj");
	//Model esfera("resources/objects/tresEsf/esferas.obj");
	Model esferaEscar("resources/objects/esferaEscar/esferaEscar.obj");
	Model kim("resources/objects/kim/kim.obj");
	Model sala("resources/objects/sillonCom/sillonCom.obj");
	Model tele("resources/objects/teleCom/tvI.obj");
	Model librero("resources/objects/libreroSala/librero.obj");
	Model campana("resources/objects/campana/campana.obj");
	//Model esfera3("resources/objects/esferaRec/esfera3.obj");//-->ARREGLAR EL MODELO
	Model sofaEst("resources/objects/sofaEstudi/sofaEstudio.obj");
	Model lavadora("resources/objects/lavadora/lava.obj");
	Model cortina("resources/objects/cortina/cortina.obj");
	Model wc("resources/objects/wc/ban.obj");
	Model lavabo("resources/objects/wc/lavabo.obj");
	Model bolsaRopa("resources/objects/wc/bolsaRopa.obj");
	Model tende("resources/objects/lavadora/tendede.obj");
	//Model ropero("resources/objects/closet/ropero.obj");-->Arreglar
	Model comedor("resources/objects/sala/sala.obj");
	//Model trineo("resources/objects/trineo/trineo.obj");
	//Model monio("resources/objects/monio/monio.obj");
	Model snowman("resources/objects/hombreNieve/muNieve.obj");
	//Model perchero("resources/objects/closet/perchero.obj");
	Model escarchaP("resources/objects/escarchaP/escarchaPeluda.obj");
	Model libros("resources/objects/libreroSala/libros.obj");
	Model dino("resources/objects/dino/dino.obj");
	Model cocina("resources/objects/cocina/cocina.obj");
	//Model sc1("resources/objects/scene/sc1.obj");
	/*Model sc2("resources/objects/scene/sc2.obj");
	Model sc3("resources/objects/scene/sc3.obj");
	Model sc4("resources/objects/scene/sc4.obj");
	Model sc5("resources/objects/scene/sc5.obj");
	Model sc6("resources/objects/scene/sc6.obj");
	*/
//	Model cuarto("resources/objects/room/cuartoPeq.obj");
	//Model esfera("resources/objects/esfera/esferasRec.obj"); -->TIENE PROBLEMAS DE LECTURA
	/*ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	animacionPersonaje.initShaders(animShader.ID);

	ModelAnim ninja("resources/objects/ZombieWalk/ZombieWalk.dae");
	ninja.initShaders(animShader.ID);
	*/
	//Inicialización de KeyFrames
	/*
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		
		
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
		KeyFrame[i].movBrazoIzq = 0;
		KeyFrame[i].rotRodDer = 0;
		KeyFrame[i].rotCabeza = 0;
		
	}
	*/
	
	//***********DINO	
		KeyFrame[0].posX = -15;
		KeyFrame[0].rotRodIzq = 10;
		KeyFrame[0].rotRodDer = -10;
		KeyFrame[0].movBrazoIzq = 10;
		KeyFrame[0].movBrazoDer = -10;
		
		KeyFrame[1].posX = -20;
		KeyFrame[1].rotRodIzq = -10;
		KeyFrame[1].rotRodDer = 10;
		KeyFrame[1].movBrazoIzq = 10;
		KeyFrame[1].movBrazoDer = -10;
		
		KeyFrame[2].posX = -25;
		KeyFrame[2].rotRodIzq = 10;
		KeyFrame[2].rotRodDer = -10;
		KeyFrame[2].movBrazoIzq = 10;
		KeyFrame[2].movBrazoDer = -10;
		
		KeyFrame[3].posX = -30;
		KeyFrame[3].rotRodIzq = -10;
		KeyFrame[3].rotRodDer = 10;
		KeyFrame[3].movBrazoIzq = 10;
		KeyFrame[3].movBrazoDer = -10;

	//***********HOBRE MANOS TIJERAS
		KeyFrame[4].posZ_monito = 15;
		KeyFrame[4].movBrazoIzq_monito = 110;
		KeyFrame[4].movBrazoDer_monito = -110;

		KeyFrame[5].posZ_monito = 20;
		KeyFrame[5].movBrazoIzq_monito = -110;
		KeyFrame[5].movBrazoDer_monito = 110;

		KeyFrame[6].posZ_monito = 25;
		KeyFrame[6].movBrazoIzq_monito = 110;
		KeyFrame[6].movBrazoDer_monito = -110;

		KeyFrame[7].posZ_monito = 30;
		KeyFrame[7].movBrazoIzq_monito = -110;
		KeyFrame[7].movBrazoDer_monito = 110;

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		glm::mat4 tmp2 = glm::mat4(1.0f);
		glm::mat4 tmp3 = glm::mat4(1.0f);
		glm::mat4 tmp4 = glm::mat4(1.0f);
		glm::mat4 tmp5 = glm::mat4(1.0f);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		//animShader.setVec3("light.ambient", colorDirection); //agregado
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//ninja.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
/*
		//CASA NUEVO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f)); //Por la matriz de modelos pasa lo que se dibuja
		staticShader.setMat4("model", model); //Shader para dibujar obj estaticos
		casaNuevo.Draw(staticShader); //Lo dibuja con draw con ayuda del shader estatico

		//ÁRBOL NORMAL
		model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 4.0f, 250.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f)); //Por la matriz de modelos pasa lo que se dibuja
		staticShader.setMat4("model", model); //Shader para dibujar obj estaticos
		arbolN.Draw(staticShader); //Lo dibuja con draw con ayuda del shader estatico

		// -------------------------------------------------------------------------------------------------------------------------
		// BAÑO
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-225.0f, 0.0f, -95.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		wc.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-170.0f, 0.0f, -113.0f));
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		staticShader.setMat4("model", model);
		lavabo.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-160.0f, 0.0f, -70.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		bolsaRopa.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Cuartos
		// -------------------------------------------------------------------------------------------------------------------------
		
		//CAMAS
		model = glm::translate(glm::mat4(1.0f), glm::vec3(85.0f, 0.0f, -215.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		cama.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 0.0f, -370.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		cama.Draw(staticShader);

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(85.0f, 0.0f, -170.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.5f));
		//staticShader.setMat4("model", model);
		//ropero.Draw(staticShader);

		//BUROS Y TOCADOR
		model = glm::translate(glm::mat4(1.0f), glm::vec3(85.0f, 0.0f, -255.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		buro.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(65.0f, 0.0f, -410.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		buro.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, 0.0f, -255.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		tocador.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -295.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		tocador.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 14.5f, 119.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		cortina.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(90.0f, 14.5f, 119.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		cortina.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-170.0f, -4.0f, 299.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		cortina.Draw(staticShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, 0.0f, 220.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		comedor.Draw(staticShader);
		
		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 0.0f, 200.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		trineo.Draw(staticShader);
/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 0.0f, 200.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		monio.Draw(staticShader);*/
/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 200.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		snowman.Draw(staticShader);//->CHECAR EL MODELO PORQUE SE VE TODO ROJO

/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 200.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		perchero.Draw(staticShader);
*/

/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 350.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		dino.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Estudio
		// -------------------------------------------------------------------------------------------------------------------------
			model = glm::translate(glm::mat4(1.0f), glm::vec3(-250.0f, 2.0f, -275.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(40.0f));
			staticShader.setMat4("model", model);
			sofaEst.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-220.0f, 2.0f, -450.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		staticShader.setMat4("model", model);
		lavadora.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 0.0f, -350.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		bolsaRopa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 30.0f, -370.0f));
		model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		tende.Draw(staticShader);

			/*model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 100.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f));
			staticShader.setMat4("model", model);
			esfera3.Draw(staticShader);
	*/
/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-165.0f, 2.0f, 150.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		cocina.Draw(staticShader);
	// -------------------------------------------------------------------------------------------------------------------------
	// PINO DE NAVIDAD
	// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 2.0f, 20.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		pino.Draw(staticShader);

		//ESTRELLA ARBOL
		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 73.5f, 17.55f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		estrella.Draw(staticShader);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// PERSONAJE
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-60.0f, 2.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		kim.Draw(staticShader);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// SALA
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, 2.0f, 20.0f));
		model = glm::rotate(model, glm::radians(-270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		sala.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-235.0f, 2.0f, 20.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		staticShader.setMat4("model", model);
		tele.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(180.0f, -5.0f, 20.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		librero.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// ADORNOS EN LA CASA
		// -------------------------------------------------------------------------------------------------------------------------

		/*model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, 50.0f, -255.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		esfera.Draw(staticShader);*/
/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(180.0f, -5.0f, 20.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		libros.Draw(staticShader);
		//CAMPANAS DE LIBRERO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(160.0f, 60.0f, 100.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(160.0f, 60.0f, 50.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(160.0f, 60.0f, -30.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);
//CAMPANAS DE LARGO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(110.0f, 60.0f, -42.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(60.0f, 60.0f, -42.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 60.0f, -42.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, 60.0f, -42.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-80.0f, 55.0f, -55.0f));
		model = glm::scale(model, glm::vec3(0.02f));
		staticShader.setMat4("model", model);
		esferaEscar.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(175.0f, 45.0f, 105.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(175.0f, 45.0f, 70.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(175.0f, 45.0f, 10.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(175.0f, 45.0f, -27.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-110.0f, 60.0f, -42.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-160.0f, 60.0f, -42.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-210.0f, 60.0f, -42.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);

		//ESCARCHA LARGO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(150.0f, 45.0f, -52.0f));
		//model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(105.0f, 45.0f, -52.0f));
		//model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(60.0f, 45.0f, -52.0f));
		//model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 45.0f, -52.0f));
		//model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 45.0f, -52.0f));
		//model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-120.0f, 45.0f, -52.0f));
		//model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-165.0f, 45.0f, -52.0f));
		//model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-210.0f, 45.0f, -52.0f));
		//model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-233.0f, 45.0f, -52.0f));
		//model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);
		
		//PARED 3
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-255.0f, 45.0f, -20.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-255.0f, 45.0f, 25.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-255.0f, 45.0f, 70.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-255.0f, 45.0f, 105.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);
		
		//PARED 4
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-233.0f, 45.0f, 115.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-188.0f, 45.0f, 115.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-143.0f, 45.0f, 115.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-98.0f, 45.0f, 115.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-58.0f, 45.0f, 115.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-13.0f, 45.0f, 115.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(32.0f, 45.0f, 115.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(77.0f, 45.0f, 115.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(122.0f, 45.0f, 115.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(152.0f, 45.0f, 115.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		escarchaP.Draw(staticShader);

		//CAMPANAS PARED 3
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-247.0f, 65.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-247.0f, 65.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-247.0f, 65.0f, 80.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);
		//CAMPANAS PARED 4
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-223.0f, 63.0f, 108.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-173.0f, 63.0f, 108.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-123.0f, 63.0f, 108.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-73.0f, 63.0f, 108.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0f, 63.0f, 108.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(27.0f, 63.0f, 108.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(75.0f, 63.0f, 108.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		campana.Draw(staticShader);
*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		/*model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //alinear el veiculo en el sentido que yo quiera
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f, movAuto_z)); //mueve sobre el eje x y el eje z
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f)); //delantera izq
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trase
		

		
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		torso.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDer.Draw(staticShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		botaDer.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(-rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaIzq.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoIzq.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotCabeza), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		cabeza.Draw(staticShader);
		*/

		// ************************************************************************************************************************
		// Personaje
		// *************************************************************************************************************************
		model = glm::translate(glm::mat4(1.0f), glm::vec3(100, 15, 300));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		torsoDino.Draw(staticShader);
		
		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-2.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pieDerDinD.Draw(staticShader);
		
		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(2.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pieIzqDinD.Draw(staticShader);
	
		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pieIzqDinT.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pieDerDinT.Draw(staticShader);




		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 67.0f, -50.0f));
		model = glm::scale(model, glm::vec3(65.0f));
		staticShader.setMat4("model", model);
		casita.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 10.0f, 20.0f));
		model = glm::scale(model, glm::vec3(19.0f));
		staticShader.setMat4("model", model);
		arbolito.Draw(staticShader);

		/*
		

		//Pierna Izq
		model = glm::translate(tmp2, glm::vec3(-0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pieDerPerP.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp2, glm::vec3(0.5f, 0.0f, 00.0f));
		model = glm::rotate(model, glm::radians(-rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pieIzqPer2.Draw(staticShader);
*/
		//***************PER 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30, 50, 0));
		model = glm::translate(model, glm::vec3(posX_monito, posY_monito, posZ_monito));
		tmp2 = model = glm::rotate(model, glm::radians(giroMonito_monito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		cuerpo.Draw(staticShader);

		//Brazo derecho
		model = glm::translate(tmp2, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoDer_monito), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		manoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp2, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoIzq_monito), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		manoIzq.Draw(staticShader);
/*
		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotCabeza), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		cabezaPer2.Draw(staticShader);
*/
		///*************************KIM
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 100, 400));
		model = glm::translate(model, glm::vec3(posX_kim, posY_kim, posZ_kim));
		model = glm::scale(model, glm::vec3(0.1f));
		tmp3 = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		cuerpoKim.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp3, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq_kim), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pieDerKim.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp3, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(-rotRodDer_kim), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaIzqKim.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		pieDerKim.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp3, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(movBrazoDer_kim), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoDerKim.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp3, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(movBrazoIzq_kim), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoIzqKim.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp3, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotCabeza_kim), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		cabezaKim.Draw(staticShader);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------

		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) //RESET
	{
		movAuto_z = 0.0f;
		movAuto_x = 0.0f;
		movAuto_xAux = 0.0f;
	};
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		movBrazoIzq+=2.0f;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		movBrazoIzq -=2.0f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		movBrazoDer += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		movBrazoDer -= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotRodDer+=2.0f;
	if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS)
		rotRodDer -=2.0f;
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		rotCabeza--;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotCabeza++;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		posY++;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		escribir();
	}
		

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true; // es un xor

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)//arreglo no pase de la cantidad maxima de frames
		{
			saveFrame();
			//escribir();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}