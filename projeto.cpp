/*************************************************************
**		Trabalho realizado por Pedro Silva - 2007183130		**
*************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "RgbImage.h"
#include "materiais.h"
#include "glm.h"

//==================================================================== Definir cores
#define AZUL     0.0, 0.4, 1.0, 1.0
#define VIDRO 	 0.69, 0.87, 0.9, 0.5
#define VERMELHO 1.0, 0.0, 0.0, 1.0
#define AMARELO  1.0, 1.0, 0.0, 1.0
#define VERDE    0.0, 1.0, 0.0, 0.5
#define LARANJA  0.8, 0.6, 0.1, 1.0
#define CASTANHO 0.36, 0.25, 0.2, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY1    0.2, 0.2, 0.2, 1.0
#define GRAY2    0.93, 0.93, 0.93, 1.0
//=================================================================== Sistema de coordenadas
GLfloat xC=100.0;
GLint wScreen=1300, hScreen=700;

//=================================================================== Variaveis
GLMmodel* bmodel;
GLMmodel* arrow;

float PI=3.141592654f;
GLint dim=2;  //dimensao da malha de poligonos do chao
int bow=1, arrowOn=0;
GLint msec=10;
char texto[30];

GLfloat vento[10];						//Nivel do vento (esquerda ou direita, fraco ou forte)
GLfloat niveisVento[]={-0.01,-0.02,0,0,0.01,0.02};

GLint fog=0;
GLfloat corNevoeiro[]={0.75, 0.75, 0.75, 1.0};

//=================================================================== Camera
GLfloat xpos=0, ypos=1, zpos=-3, xrot=0, yrot=180, lastx, lasty;
int buttonDown=-1;

//=================================================================== Alvo
int dist[]={10,25,50};
GLfloat alvoPos[]={0,1.2,dist[0]};
GLfloat tamPes=0.6;

//=================================================================== Grade/tabuas
GLfloat tabuaCump=0.2, tabuaAltura=1, tabuaLargura=0.05;
GLfloat xGrade=5, zGradeMin=10, zGradeMax=60;

//=================================================================== Arrows
int countArrows=0;

struct arrow{
	GLint activo;
	GLfloat xA,yA,zA,xrotA,yrotA,zrotA;
}listaArrows[10];

int pontuacao[10]={0,0,0,0,0,0,0,0,0,0};

//=================================================================== Iluminaçao
GLint noite=0;
GLfloat luzGlobalCor[]={1.0, 1.0, 1.0, 1.0}; //{0.2, 0.2, 0.2, 1.0}

GLfloat solPos[]={0.0, -1.0, -1.0, 0.0};
GLfloat solCor[]={1.0, 1.0, 1.0, 1.0};
GLfloat solCorDif[]={1.0, 1.0, 1.0, 1.0};
GLfloat solCorEsp[]={1.0, 1.0, 1.0, 1.0};
GLfloat solAttCon =1.0;
GLfloat solAttLin =0.05;
GLfloat solAttQua =0.0;

GLfloat focoPos[] = {0, 2.5, -4, 1.0};
GLfloat focoDir[] = {0, -1, 0};
GLfloat focoExp   = 1.0;
GLfloat focoCut   = 90.0;

GLfloat focoCorEsp[] ={1.0 ,  1.0, 1.0, 1.0}; 
GLfloat focoCorDif[] ={1.0 ,  1.0, 1.0, 1.0}; 

GLfloat focoPos2[] = {0, 4, dist[0]+3, 1.0};
GLfloat focoDir2[] = {0, -1, 1};
GLfloat focoExp2   = 10.0;
GLfloat focoCut2   = 45.0;

//=================================================================== Texturas
GLuint texture[4];
RgbImage imag;


void initMaterials(int material){
	switch (material){
		case 0: //esmerald
			glMaterialfv(GL_FRONT,GL_AMBIENT,  esmeraldAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  esmeraldDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, esmeraldSpec);
			glMateriali (GL_FRONT,GL_SHININESS,esmeraldCoef);
			break;
		case 1: //jade
			glMaterialfv(GL_FRONT,GL_AMBIENT,  jadeAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  jadeDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, jadeSpec);
			glMateriali (GL_FRONT,GL_SHININESS,jadeCoef);
			break;
	       case 2: //obsidian
			glMaterialfv(GL_FRONT,GL_AMBIENT,  obsidianAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  obsidianDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, obsidianSpec);
			glMateriali (GL_FRONT,GL_SHININESS,obsidianCoef);
			break;
		case 3: //pearl
			glMaterialfv(GL_FRONT,GL_AMBIENT,  pearlAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  pearlDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, pearlSpec);
			glMateriali (GL_FRONT,GL_SHININESS,pearlCoef);
			break;
		case 4: //ruby
			glMaterialfv(GL_FRONT,GL_AMBIENT,  rubyAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  rubyDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, rubySpec);
			glMateriali (GL_FRONT,GL_SHININESS,rubyCoef);
			break;
	         case 5: //turquoise
			glMaterialfv(GL_FRONT,GL_AMBIENT,  turquoiseAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  turquoiseDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, turquoiseSpec);
			glMateriali (GL_FRONT,GL_SHININESS,turquoiseCoef);
			break;
		case 6: //brass
			glMaterialfv(GL_FRONT,GL_AMBIENT,  brassAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  brassDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, brassSpec);
			glMateriali (GL_FRONT,GL_SHININESS,brassCoef);
			break;
		case 7: //bronze
			glMaterialfv(GL_FRONT,GL_AMBIENT,  bronzeAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  bronzeDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, bronzeSpec);
			glMateriali (GL_FRONT,GL_SHININESS,bronzeCoef);
			break;
		case 8: //chrome
			glMaterialfv(GL_FRONT,GL_AMBIENT,  chromeAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  chromeDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, chromeSpec);
			glMateriali (GL_FRONT,GL_SHININESS,chromeCoef);
			break;
	         case 9: //copper
			glMaterialfv(GL_FRONT,GL_AMBIENT,  copperAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  copperDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, copperSpec);
			glMateriali (GL_FRONT,GL_SHININESS,copperCoef);
			break;
	         case 10: //gold
			glMaterialfv(GL_FRONT,GL_AMBIENT,  goldAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  goldDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, goldSpec);
			glMateriali (GL_FRONT,GL_SHININESS,goldCoef);
			break;
		case 11: //silver
			glMaterialfv(GL_FRONT,GL_AMBIENT,  silverAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  silverDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, silverSpec);
			glMateriali (GL_FRONT,GL_SHININESS,silverCoef);
			break;
		case 12: //blackPlastic
			glMaterialfv(GL_FRONT,GL_AMBIENT,  blackPlasticAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  blackPlasticDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, blackPlasticSpec);
			glMateriali (GL_FRONT,GL_SHININESS,blackPlasticCoef);
			break;
		case 13: //cyankPlastic
			glMaterialfv(GL_FRONT,GL_AMBIENT,  cyanPlasticAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  cyanPlasticDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, cyanPlasticSpec);
			glMateriali (GL_FRONT,GL_SHININESS,cyanPlasticCoef);
			break;
		case 14: //greenPlastic
			glMaterialfv(GL_FRONT,GL_AMBIENT,  greenPlasticAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  greenPlasticDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, greenPlasticSpec);
			glMateriali (GL_FRONT,GL_SHININESS,greenPlasticCoef);
			break;
		case 15: //redPlastic
			glMaterialfv(GL_FRONT,GL_AMBIENT,  redPlasticAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  redPlasticDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, redPlasticSpec);
			glMateriali (GL_FRONT,GL_SHININESS,redPlasticCoef);
			break;
	        case 16: //yellowPlastic
			glMaterialfv(GL_FRONT,GL_AMBIENT,  whitePlasticAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  whitePlasticDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, whitePlasticSpec);
			glMateriali (GL_FRONT,GL_SHININESS,whitePlasticCoef);
			break;
		case 17: //yellowPlastic
			glMaterialfv(GL_FRONT,GL_AMBIENT,  yellowPlasticAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  yellowPlasticDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, yellowPlasticSpec);
			glMateriali (GL_FRONT,GL_SHININESS,yellowPlasticCoef);
			break;
	        case 18: //blackRubber
			glMaterialfv(GL_FRONT,GL_AMBIENT,  blackRubberAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  blackRubberDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, blackRubberSpec);
			glMateriali (GL_FRONT,GL_SHININESS,blackRubberCoef);
			break;
		case 19: //cyanRubber
			glMaterialfv(GL_FRONT,GL_AMBIENT,  cyanRubberAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  cyanRubberDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, cyanRubberSpec);
			glMateriali (GL_FRONT,GL_SHININESS,cyanRubberCoef);
			break;
		case 20: //greenRubber
			glMaterialfv(GL_FRONT,GL_AMBIENT,  greenRubberAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  greenRubberDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, greenRubberSpec);
			glMateriali (GL_FRONT,GL_SHININESS,greenRubberCoef);
			break;
		case 21: //redRubber
			glMaterialfv(GL_FRONT,GL_AMBIENT,  redRubberAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  redRubberDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, redRubberSpec);
			glMateriali (GL_FRONT,GL_SHININESS,redRubberCoef);
			break;
		case 22: //redRubber
			glMaterialfv(GL_FRONT,GL_AMBIENT,  whiteRubberAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  whiteRubberDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, whiteRubberSpec);
			glMateriali (GL_FRONT,GL_SHININESS,whiteRubberCoef);
			break;
		case 23: //redRubber
			glMaterialfv(GL_FRONT,GL_AMBIENT,  yellowRubberAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  yellowRubberDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, yellowRubberSpec);
			glMateriali (GL_FRONT,GL_SHININESS,yellowRubberCoef);
			break;
		case 24:
			glMaterialfv(GL_FRONT,GL_AMBIENT,  blueAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  blueDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, blueSpec);
			glMateriali (GL_FRONT,GL_SHININESS,blueCoef);
			break;
		case 25:
			glMaterialfv(GL_FRONT,GL_AMBIENT,  whiteAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  whiteDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, whiteSpec);
			glMateriali (GL_FRONT,GL_SHININESS,whiteCoef);
			break;
		case 26:
			glMaterialfv(GL_FRONT,GL_AMBIENT,  yellowAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  yellowDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, yellowSpec);
			glMateriali (GL_FRONT,GL_SHININESS,yellowCoef);
			break;
		case 27:
			glMaterialfv(GL_FRONT,GL_AMBIENT,  redAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  redDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, redSpec);
			glMateriali (GL_FRONT,GL_SHININESS,redCoef);
			break;
		case 28:
			glMaterialfv(GL_FRONT,GL_AMBIENT,  blackAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  blackDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, blackSpec);
			glMateriali (GL_FRONT,GL_SHININESS,blackCoef);
			break;
		case 29:
			glMaterialfv(GL_FRONT,GL_AMBIENT,  greenAmb  );
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  greenDif );
			glMaterialfv(GL_FRONT,GL_SPECULAR, greenSpec);
			glMateriali (GL_FRONT,GL_SHININESS,greenCoef);
			break;
	}
}

void initLights(){
	//Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
	//Sol
	glLightfv(GL_LIGHT0, GL_POSITION, solPos);   
	glLightfv(GL_LIGHT0, GL_AMBIENT, solCor);   
	glLightfv(GL_LIGHT0, GL_DIFFUSE, solCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, solCorEsp); 
	glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, solAttCon);
	glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, solAttLin);         
	glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, solAttQua);          
	//Foco na casa
	glLightfv(GL_LIGHT1, GL_POSITION, focoPos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, focoDir);
	glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, focoExp);
	glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, focoCut);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, focoCorDif);   
	glLightfv(GL_LIGHT1, GL_SPECULAR, focoCorEsp);
	//Foco no alvo
	glLightfv(GL_LIGHT2, GL_POSITION, focoPos2);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, focoDir2);
	glLightf (GL_LIGHT2, GL_SPOT_EXPONENT, focoExp2);
	glLightf (GL_LIGHT2, GL_SPOT_CUTOFF, focoCut2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, focoCorDif);   
	glLightfv(GL_LIGHT2, GL_SPECULAR, focoCorEsp);
}

void criaDefineTexturas(){
	//----------------------------------------- Chao
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("texturas/grass.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	//------------------------------------------- Lados
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("texturas/sky.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	//------------------------------------------- Ceu
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("texturas/sky2.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	//------------------------------------------- Madeira
	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("texturas/madeira.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
}

void initModels(){
	bmodel=glmReadOBJ(strdup("Bow_compound/Bow_compound.obj"));
	glmUnitize(bmodel);
	glmFacetNormals(bmodel);
	glmVertexNormals(bmodel, 180.0);
	glmScale(bmodel,0.5);
	glmScale(bmodel,0.5);

	arrow=glmReadOBJ(strdup("Arrow/Arrow.obj"));
	glmUnitize(arrow);
	glmFacetNormals(arrow);
	glmVertexNormals(arrow, 180.0);
	glmScale(arrow,0.5);
}

void aleatorioVento(){
    GLint y;
	
	for(int i=0;i<10;i++){
		y = rand()%(sizeof(niveisVento)/sizeof(niveisVento[0]));
		vento[i]=niveisVento[y];
	}
}

void initNevoeiro(void){
    glFogfv(GL_FOG_COLOR, corNevoeiro); //Cor do nevoeiro
    glFogi(GL_FOG_MODE, GL_LINEAR); 
    glFogf(GL_FOG_START, 1); 
    glFogf(GL_FOG_END, 60);
    glFogf (GL_FOG_DENSITY, 0.35);
}

void init(){
	glClearColor(BLACK);
	glShadeModel(GL_SMOOTH);

	initLights();

	criaDefineTexturas();

	initModels();

	aleatorioVento();

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	initNevoeiro();
	glDisable(GL_FOG);

	glEnable(GL_LIGHTING);  
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);

	glEnable(GL_DEPTH_TEST);
}

void camera(){
	glRotatef(xrot,1.0,0.0,0.0);		//Roda a camera para a esquerda e direita
	glRotatef(yrot,0.0,1.0,0.0);		//Roda a camera para cima e para baixo

	if(xpos<-(xGrade-1.5))				//Limita o sitio onde o player pode estar(dentro da casa)
		xpos=-(xGrade-1.5);
	if(xpos>xGrade-1.5)
		xpos=xGrade-1.5;
	if(zpos<-(zGradeMin-2.5))
		zpos=-(zGradeMin-2.5);
	if(zpos>-0.5)
		zpos=-0.5;
	glTranslated(-xpos,-ypos,-zpos);	//move a camera para a sua posicao atual dentro dos limites

	glLightfv(GL_LIGHT1, GL_POSITION, focoPos); //para impedir k o foco se mova com a camera
	glLightfv(GL_LIGHT2, GL_POSITION, focoPos2);
}

void drawArrow(){
	glPushMatrix();
		initMaterials(29);
		glRotatef(180,0,1,0);
		glTranslated(0.04,-0.05,0.02);
		glRotatef(90,0,0,1);

		glmDraw(arrow, GLM_SMOOTH);
	glPopMatrix();
}

void drawBow(){	
	glTranslated(xpos,ypos,zpos);
	glRotatef(xrot,1.0,0.0,0.0);
	glRotatef(-yrot,0.0,1.0,0.0);
	glPushMatrix();
		initMaterials(24);
		glRotatef(180,0,1,0);
		glTranslated(0.07,-0.05,0.2);

		glmDraw(bmodel, GLM_SMOOTH);
	glPopMatrix();
	if(arrowOn==1)
		drawArrow();
}

void drawDisk(GLfloat raio){
	GLUquadricObj* disk;

	disk=gluNewQuadric();
	gluQuadricDrawStyle(disk, GLU_FILL);
	gluQuadricNormals(disk, GLU_SMOOTH);
	gluQuadricOrientation(disk, GLU_OUTSIDE);
	//gluQuadricTexture(disk, GL_TRUE); //em caso de usar texturas

	gluDisk(disk, 0, raio, 100, 100);
}

void drawAlvo(){
	initMaterials(6);
	glPushMatrix();
		//glColor4f(CASTANHO);
		glTranslatef(alvoPos[0]-0.4, 0.5, alvoPos[2]+0.04);
		glScalef(0.1,2,0.1);
		glutSolidCube(tamPes);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(alvoPos[0]+0.4, 0.5, alvoPos[2]+0.04);
		glScalef(0.1,2,0.1);
		glutSolidCube(tamPes);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(alvoPos[0], 0.4, alvoPos[2]+0.35);
		glRotatef(-30,1,0,0);
		glScalef(0.1,2,0.1);
		glutSolidCube(tamPes);
	glPopMatrix();

	glPushMatrix();
		initMaterials(25);
		//glColor4f(WHITE);
		glTranslatef(alvoPos[0], alvoPos[1], alvoPos[2]);
		drawDisk(0.5);
		initMaterials(28);
		//glColor4f(BLACK);
		glTranslatef(0,0,-0.01);
		drawDisk(0.4);
		initMaterials(24);
		//glColor4f(AZUL);
		glTranslatef(0,0,-0.01);
		drawDisk(0.3);
		initMaterials(27);
		//glColor4f(VERMELHO);
		glTranslatef(0,0,-0.01);
		drawDisk(0.2);
		initMaterials(26);
		//glColor4f(AMARELO);
		glTranslatef(0,0,-0.01);
		drawDisk(0.1);
	glPopMatrix();
}

void drawLimites(){
	//--------------------------------------- Chao
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glPushMatrix();
	glColor4f(VERDE);
		for(int i=-xC;i<xC;i+=dim){
			for(int j=-zGradeMin+1;j<xC;j+=dim){
				glBegin(GL_QUADS);
				glNormal3f(0,1,0);
					glTexCoord2f(0.0f,0.0f); 	glVertex3i(i, 0, j);
					glTexCoord2f(1.0f,0.0f); 	glVertex3i(i+dim, 0, j);
					glTexCoord2f(1.0f,1.0f); 	glVertex3i(i+dim, 0, j+dim);
					glTexCoord2f(0.0f,1.0f); 	glVertex3i(i, 0, j+dim);
				glEnd();
			}
		}
	glPopMatrix();

	//---------------------------------------- Frente
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPushMatrix();
	glColor4f(VIDRO);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); 		glVertex3i(-xC, 0, xC);
			glTexCoord2f(1.0f,0.0f);		glVertex3i(xC, 0, xC);
			glTexCoord2f(1.0f,1.0f);		glVertex3i(xC, xC, xC);
			glTexCoord2f(0.0f,1.0f);		glVertex3i(-xC, xC, xC);
		glEnd();
	glPopMatrix();

	//------------------------------------------ Esquerda
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPushMatrix();
	glColor4f(VIDRO);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);		glVertex3i(-xC, 0, xC);
			glTexCoord2f(1.0f,0.0f);		glVertex3i(-xC, 0, -xC);
			glTexCoord2f(1.0f,1.0f);		glVertex3i(-xC, xC, -xC);
			glTexCoord2f(0.0f,1.0f);		glVertex3i(-xC, xC, xC);
		glEnd();
	glPopMatrix();

	//------------------------------------------ Direita
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPushMatrix();
	glColor4f(VIDRO);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);		glVertex3i(xC, 0, -xC);
			glTexCoord2f(1.0f,0.0f);		glVertex3i(xC, 0, xC);
			glTexCoord2f(1.0f,1.0f);		glVertex3i(xC, xC, xC);
			glTexCoord2f(0.0f,1.0f);		glVertex3i(xC, xC, -xC);
		glEnd();
	glPopMatrix();

	//------------------------------------------ Tras
	/*glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPushMatrix();
	glColor4f(VIDRO);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);		glVertex3i(xC, 0, -xC);
			glTexCoord2f(1.0f,0.0f);		glVertex3i(-xC, 0, -xC);
			glTexCoord2f(1.0f,1.0f);		glVertex3i(-xC, xC, -xC);
			glTexCoord2f(0.0f,1.0f);		glVertex3i(xC, xC, -xC);
		glEnd();
	glPopMatrix();*/
	//------------------------------------------ Cima
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPushMatrix();
	glColor4f(VIDRO);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);		glVertex3i(-xC, xC, -xC);
			glTexCoord2f(1.0f,0.0f);		glVertex3i(xC, xC, -xC);
			glTexCoord2f(1.0f,1.0f);		glVertex3i(xC, xC, xC);
			glTexCoord2f(0.0f,1.0f);		glVertex3i(-xC, xC, xC);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void drawTabua(){
	glBegin(GL_QUADS);
		//------------------ Parte de baixo
		glTexCoord2f(0.0f,0.0f);	glVertex3f(0,0,0);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(tabuaCump,0,0);
		glTexCoord2f(1.0f,1.0f);	glVertex3f(tabuaCump,0,-tabuaLargura);
		glTexCoord2f(0.0f,1.0f);	glVertex3f(0,0,-tabuaLargura);
		//------------------ Parte de cima
		glTexCoord2f(0.0f,0.0f);	glVertex3f(0,tabuaAltura,0);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(tabuaCump,tabuaAltura,0);
		glTexCoord2f(1.0f,1.0f);	glVertex3f(tabuaCump,tabuaAltura,-tabuaLargura);
		glTexCoord2f(0.0f,1.0f);	glVertex3f(0,0,-tabuaLargura);
		//------------------ Parte da frente
		glTexCoord2f(0.0f,0.0f);	glVertex3f(0,0,0);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(tabuaCump,0,0);
		glTexCoord2f(1.0f,1.0f);	glVertex3f(tabuaCump,tabuaAltura,0);
		glTexCoord2f(0.0f,1.0f);	glVertex3f(0,tabuaAltura,0);
		//------------------ Parte de tras
		glTexCoord2f(0.0f,0.0f);	glVertex3f(0,0,-tabuaLargura);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(tabuaCump,0,-tabuaLargura);
		glTexCoord2f(1.0f,1.0f);	glVertex3f(tabuaCump,tabuaAltura,-tabuaLargura);
		glTexCoord2f(0.0f,1.0f);	glVertex3f(0,tabuaAltura,-tabuaLargura);
		//------------------ Parte da esquerda
		glTexCoord2f(0.0f,0.0f);	glVertex3f(0,0,0);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(0,tabuaAltura,0);
		glTexCoord2f(1.0f,1.0f);	glVertex3f(0,tabuaAltura,-tabuaLargura);
		glTexCoord2f(0.0f,1.0f);	glVertex3f(0,0,-tabuaLargura);
		//------------------ Parte da direita
		glTexCoord2f(0.0f,0.0f);	glVertex3f(tabuaCump,0,0);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(tabuaCump,tabuaAltura,0);
		glTexCoord2f(1.0f,1.0f);	glVertex3f(tabuaCump,tabuaAltura,-tabuaLargura);
		glTexCoord2f(0.0f,1.0f);	glVertex3f(tabuaCump,0,-tabuaLargura);
	glEnd();
}

void drawGrades(){
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,texture[3]);
		glColor4f(CASTANHO);

		glPushMatrix();
			glTranslatef(-xGrade,2*tabuaAltura/3,-zGradeMin);
			glRotatef(90,0,0,1);
			glRotatef(90,1,0,0);
			glScalef(0.5,(zGradeMax+zGradeMin)/tabuaAltura,1);
			drawTabua();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-xGrade,tabuaAltura/3,-zGradeMin);
			glRotatef(90,0,0,1);
			glRotatef(90,1,0,0);
			glScalef(0.5,(zGradeMax+zGradeMin)/tabuaAltura,1);
			drawTabua();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(xGrade,2*tabuaAltura/3,-zGradeMin);
			glRotatef(90,0,0,1);
			glRotatef(90,1,0,0);
			glScalef(0.5,(zGradeMax+zGradeMin)/tabuaAltura,1);
			drawTabua();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(xGrade,tabuaAltura/3,-zGradeMin);
			glRotatef(90,0,0,1);
			glRotatef(90,1,0,0);
			glScalef(0.5,(zGradeMax+zGradeMin)/tabuaAltura,1);
			drawTabua();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(xGrade,2*tabuaAltura/3,-zGradeMin);
			glRotatef(90,0,0,1);
			glScalef(0.5,(xGrade*2)/tabuaAltura,1);
			drawTabua();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(xGrade,tabuaAltura/3,-zGradeMin);
			glRotatef(90,0,0,1);
			glScalef(0.5,(xGrade*2)/tabuaAltura,1);
			drawTabua();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(xGrade,2*tabuaAltura/3,zGradeMax);
			glRotatef(90,0,0,1);
			glScalef(0.5,(xGrade*2)/tabuaAltura,1);
			drawTabua();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(xGrade,tabuaAltura/3,zGradeMax);
			glRotatef(90,0,0,1);
			glScalef(0.5,(xGrade*2)/tabuaAltura,1);
			drawTabua();
		glPopMatrix();
		
		for(GLfloat i=-zGradeMin;i<=zGradeMax;i++){
			glPushMatrix();
				glTranslatef(-xGrade,0,i);
				glRotatef(90,0,1,0);
				drawTabua();
			glPopMatrix();
			glPushMatrix();
				glTranslatef(xGrade,0,i);
				glRotatef(90,0,1,0);
				drawTabua();
			glPopMatrix();
		}
		for(GLfloat i=-xGrade;i<=xGrade;i++){
			glPushMatrix();
				glTranslatef(i,0,-zGradeMin);
				drawTabua();
			glPopMatrix();
			glPushMatrix();
				glTranslatef(i,0,zGradeMax);
				drawTabua();
			glPopMatrix();
		}	
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawCylinder(float altura){
	GLUquadricObj* cylinder;

	cylinder=gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluQuadricOrientation(cylinder, GLU_OUTSIDE);
	//gluQuadricTexture(disk, GL_TRUE); //em caso de usar texturas

	glRotatef(90,1,0,0);
	gluCylinder(cylinder, 0.1, 0.1, altura, 100, 100);
}

void drawParedeLateral(){
	for(int i=0;i<3;i++){
		for(int j=-(zGradeMin-2);j<0;j++){
			glBegin(GL_QUADS);
				glVertex3f(xGrade-1,i,j);
				glVertex3f(xGrade-1,i+1,j);
				glVertex3f(xGrade-1,i+1,j+1);
				glVertex3f(xGrade-1,i,j+1);
			glEnd();
		}
	}
}

void drawParedeFrontal(){
	glBegin(GL_QUADS);
		glVertex3f(xGrade-1,0,0);
		glVertex3f(1.5,0,0);
		glVertex3f(1.5,0.7,0);
		glVertex3f(xGrade-1,0.7,0);
	glEnd();
}

void drawVidro(){
	glBegin(GL_QUADS);
		glVertex3f(xGrade-1,0.7,0);
		glVertex3f(1.5,0.7,0);
		glVertex3f(1.5,2.5,0);
		glVertex3f(xGrade-1,2.5,0);
	glEnd();
}

void drawCasa(){
	//------------------------------------- Paredes
	//glColor4f(VERMELHO);
	initMaterials(6);
	glPushMatrix();
		for(int i=-(xGrade-1);i<xGrade-1;i++){
			for(int j=0;j<3;j++){
				glBegin(GL_QUADS);
					glVertex3f(i,j,-(zGradeMin-2));
					glVertex3f(i+1,j,-(zGradeMin-2));
					glVertex3f(i+1,j+1,-(zGradeMin-2));
					glVertex3f(i,j+1,-(zGradeMin-2));
				glEnd();	
			}
		}		
	glPopMatrix();
	glPushMatrix();
		drawParedeLateral();
		glTranslatef(-((xGrade-1)*2),0,0);
		drawParedeLateral();
	glPopMatrix();
	glPushMatrix();
		drawParedeFrontal();
		glTranslatef(-5.5,0,0);
		drawParedeFrontal();
	glPopMatrix();
	//------------------------------------- Tecto
	//glColor4f(VERDE);
	initMaterials(4);
	glPushMatrix();
		glBegin(GL_QUADS);
			glVertex3f(-(xGrade-1),2.5,0);
			glVertex3f(xGrade-1,2.5,0);
			glVertex3f(xGrade-1,3,-(zGradeMin-2));
			glVertex3f(-(xGrade-1),3,-(zGradeMin-2));
		glEnd();
	glPopMatrix();
	//------------------------------------- Vidros/Janelas
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(VIDRO);
	glPushMatrix();
		drawVidro();
		glTranslatef(-5.5,0,0);
		drawVidro();
	glPopMatrix();
	glDisable(GL_BLEND);
	//------------------------------------- linhaBranca
	glColor4f(WHITE);
	glPushMatrix();
		glLineWidth(20);
		glBegin(GL_LINES);
			glVertex3f(-1.4,0.05,0);
			glVertex3f(1.4,0.05,0);
		glEnd();
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);
	//------------------------------------- Pilares
	//glColor4f(GRAY1);
	initMaterials(11);
	glPushMatrix();
		glTranslatef(xGrade-1,2.5,0);
		drawCylinder(2.5);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-(xGrade-1),2.5,0);
		drawCylinder(2.5);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-1.5,2.5,0);
		drawCylinder(2.5);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(1.5,2.5,0);
		drawCylinder(2.5);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(xGrade-1,3,-(zGradeMin-2));
		drawCylinder(3);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-(xGrade-1),3,-(zGradeMin-2));
		drawCylinder(3);
	glPopMatrix();
}

void desenhaTexto(char *string, GLfloat x, GLfloat y) {  
	glRasterPos2f(x,y);
	while(*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
}

void drawPontuacao(){
	float alt=-1;
	int total=0;

	for(int i=0;i<(sizeof(pontuacao)/sizeof(pontuacao[0]));i++){
		sprintf(texto, "Jogada %d\t-\t%d pontos", i+1, pontuacao[i]);
		total+=pontuacao[i];
		
		if(i==countArrows)			//Poe a vermelho a jogada actual
			glColor3f(1,0,0);
		else
			glColor3f(1,1,0);

		desenhaTexto(texto, -7, alt);
		alt+=-0.75;
	}

	glColor3f(1,1,0);
	sprintf(texto, "Total - %d pontos | Vento: %d", total, (int)(vento[countArrows]*100)*-1);
	desenhaTexto(texto, -8, alt-0.75);
}

void drawCrosshair(){
    glDisable(GL_LIGHTING);
    glPushMatrix();
    	glViewport (wScreen/4, 0, wScreen, hScreen);
    	glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, wScreen, hScreen, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
 
        glColor3ub(240, 240, 240);//white
        glLineWidth(2.0);
        glBegin(GL_LINES);
	        //horizontal line
	        glVertex2i(wScreen / 2 - 7, hScreen / 2);
	        glVertex2i(wScreen / 2 + 7, hScreen / 2);
        glEnd();
        //vertical line
        glBegin(GL_LINES);
	        glVertex2i(wScreen / 2, hScreen / 2 + 7);
	        glVertex2i(wScreen / 2, hScreen / 2 - 7);
        glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawArrowMove(GLfloat xM, GLfloat yM, GLfloat zM, GLfloat xrotA, GLfloat yrotA){
	glPushMatrix();
		initMaterials(29);
		
		glTranslatef(xM, yM, zM);
		glRotatef(xrotA,1.0,0.0,0.0);
		glRotatef(-yrotA+180, 0,1,0);
		glRotatef(90,0,0,1);

		glmDraw(arrow, GLM_SMOOTH);
	glPopMatrix();
}

void dispara(GLfloat xDisp, GLfloat yDisp, GLfloat zDisp, GLfloat angX, GLfloat angY){
	listaArrows[countArrows].activo=1;
	listaArrows[countArrows].xA=xDisp;
	listaArrows[countArrows].yA=yDisp;
	listaArrows[countArrows].zA=zDisp;
	listaArrows[countArrows].xrotA=angX;
	listaArrows[countArrows].yrotA=angY;

	if(countArrows<10)
		countArrows++;
}

void drawScene(){
	//------------------------------------------ Limites
	drawLimites();
	//------------------------------------------ Gradeamento
	drawGrades();
	//------------------------------------------ Alvo
	glDisable(GL_COLOR_MATERIAL);
	drawAlvo();
	//------------------------------------------ Casa
	drawCasa();
	//------------------------------------------ Arrows disparadas
	for(int i=0;i<countArrows;i++){
		if(listaArrows[i].activo==1)
			drawArrowMove(listaArrows[i].xA, listaArrows[i].yA, listaArrows[i].zA, listaArrows[i].xrotA, listaArrows[i].yrotA);
	}
	//------------------------------------------ Bow
	if(bow==1)
		drawBow();

	glEnable(GL_COLOR_MATERIAL);
}

void display(){
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//------------------------------------- ViewPort 1
	drawCrosshair();

	//------------------------------------- ViewPort 2
	glViewport (wScreen/4, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, wScreen/hScreen, 0.1, 250.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera();

	drawScene();

	//------------------------------------- ViewPort 3
	glViewport (0, 0, wScreen/4, hScreen/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, wScreen/hScreen, 0.1, 250.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 1.2, alvoPos[2]-1, 0, 1.2, alvoPos[2], 0, 1, 0);

	drawScene();

	//------------------------------------- ViewPort 4
	glViewport (0, hScreen/2, wScreen/4, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-10,10,-10,10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	drawPontuacao();
	glEnable(GL_LIGHTING);

	//-------------------------------------
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){

    if (key=='w' or key=='W'){
        float xrotrad, yrotrad;
        yrotrad = (yrot / 180 * PI);
        xrotrad = (xrot / 180 * PI); 
        xpos += float(sin(yrotrad)) ;
        zpos -= float(cos(yrotrad)) ;
        //ypos -= float(sin(xrotrad)) ;
    }

    if (key=='s' or key=='S'){
        float xrotrad, yrotrad;
        yrotrad = (yrot / 180 * PI);
        xrotrad = (xrot / 180 * PI); 
        xpos -= float(sin(yrotrad));
        zpos += float(cos(yrotrad)) ;
        //ypos += float(sin(xrotrad));
    }

    if (key=='d' or key=='D'){
        float yrotrad;
        yrotrad = (yrot / 180 * PI);
        xpos += float(cos(yrotrad)) * 0.2;
        zpos += float(sin(yrotrad)) * 0.2;
    }

    if (key=='a' or key=='A'){
        float yrotrad;
        yrotrad = (yrot / 180 * PI);
        xpos -= float(cos(yrotrad)) * 0.2;
        zpos -= float(sin(yrotrad)) * 0.2;
    }

    if(key=='1'){
    	alvoPos[2]=dist[0];
    	focoPos2[2]=dist[0]+3;
    }

    if(key=='2'){
    	alvoPos[2]=dist[1];
    	focoPos2[2]=dist[1]+3;
    }

    if(key=='3'){
    	alvoPos[2]=dist[2];
    	focoPos2[2]=dist[2]+3;
    }

    if(key=='n' or key=='N'){
    	if(noite==0){
    		luzGlobalCor[0]=luzGlobalCor[1]=luzGlobalCor[2]=0.2;
    		glDisable(GL_LIGHT0);
    		glEnable(GL_LIGHT1);
    		glEnable(GL_LIGHT2);
    		noite=1;
    	}
    	else{
    		luzGlobalCor[0]=luzGlobalCor[1]=luzGlobalCor[2]=1.0;
    		glDisable(GL_LIGHT1);
    		glDisable(GL_LIGHT2);
    		glEnable(GL_LIGHT0);
    		noite=0;
    	}
    	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
    }

    if(key=='b' or key=='B'){
    	if(bow==1)
    		bow=0;
    	else
    		bow=1;
    }

    if(key=='f' or key=='F'){
    	if(fog==0){
    		glEnable(GL_FOG);
    		fog=1;
    	}
    	else{
    		glDisable(GL_FOG);
    		fog=0;
    	} 		
    }

    if (key==27){
    	exit(0);
    }
}

void mouseMovement(int x, int y){
	if(buttonDown>=0){
	    int diffx=x-lastx; 		//faz a diferença do ultimo e actual x
	    int diffy=y-lasty; 		//faz a diferença do ultimo e actual y
	    lastx=x;				//guarda o x actual como o ultimo
	    lasty=y; 				//guarda o y actual como o ultimo

	    diffx *= 0.5;			//diminui a sensibilidade
	    diffy *= 0.5;

	    if(diffx==0 && diffy==0)
	    	return;

	    if(xrot>89)				//bloqueia a rotaçao vertical para impedir a camara de se virar ao contrario
	    	xrot=89;
	    if(xrot<-89)
	    	xrot=-89;

	    xrot += (float) diffy;	//guarda a rotaçao feita
	    yrot += (float) diffx;  
	}
}

void mouseButton(int button, int state, int x, int y){
	if(button==GLUT_RIGHT_BUTTON){
		if(state==GLUT_DOWN){
			glutSetCursor(GLUT_CURSOR_NONE);
			buttonDown=0;
			lastx=x;				
	    	lasty=y;
	    }
		else{
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			buttonDown=-1;
		}
	}
	if(button==GLUT_LEFT_BUTTON){
		if(state==GLUT_DOWN){
			arrowOn=1;
			listaArrows[countArrows-1].activo=0;		//retira a seta anterior
	    }
		else{
			arrowOn=0;
			if(countArrows<10)
				dispara(xpos+0.04,ypos-0.05,zpos+0.02, xrot, yrot);  //soma os valores para ficar no arco
		}
	}
}

void calculaPontos(int numArrow, GLfloat xF, GLfloat yF){
	float pos=pow((xF-alvoPos[0]),2)+pow((yF-alvoPos[1]),2);

	if(pos<pow(0.1,2)){
		pontuacao[numArrow]=10;
	}
	else if(pos<pow(0.2,2)){
		pontuacao[numArrow]=8;
	}
	else if(pos<pow(0.3,2)){
		pontuacao[numArrow]=6;
	}
	else if(pos<pow(0.4,2)){
		pontuacao[numArrow]=4;
	}
	else if(pos<pow(0.5,2)){
		pontuacao[numArrow]=2;
	}
}

void timer(int value){
	for(int i=0;i<countArrows;i++){
		if(listaArrows[i].activo==1){
			if(listaArrows[i].zA<=alvoPos[2]-0.25){
				float xrotrad, yrotrad;
		        yrotrad = (listaArrows[i].yrotA / 180 * PI);
		        xrotrad = (listaArrows[i].xrotA / 180 * PI);

		        listaArrows[i].xA += float(sin(yrotrad))+vento[i];
		        listaArrows[i].zA -= float(cos(yrotrad));
		        listaArrows[i].yA -= float(sin(xrotrad))+0.01;	//mais 0.01 para a seta ir perdendo altitude
	    	}
	    	else{
	    		calculaPontos(i, listaArrows[i].xA, listaArrows[i].yA);
	    	}

	        if(listaArrows[i].yA<=0 or listaArrows[i].xA<=-xGrade or listaArrows[i].xA>=xGrade)
				listaArrows[i].activo=0;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(msec,timer, 1);
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(0,0);
	
	glutCreateWindow("Archery");
	//glutFullScreen();
	init();
	
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMovement); //check for mouse movement
	glutTimerFunc(msec, timer, 1);

	glutMainLoop();

	return 0;
}