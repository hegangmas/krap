
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

//#include "glut.h"

#include <time.h>



uint32_t GetTickCount()
{
    timespec timespec_time;
    __uint32_t theTick = 0;

    clock_gettime(CLOCK_REALTIME, &timespec_time);

    theTick = timespec_time.tv_nsec / 1000000;
    theTick += timespec_time.tv_sec * 1000;


}

const int FRAME_RATE = 100;

bool g_fullScreen = false;	// ali smo v celozaslonskem naèinu?
int g_width = 400, g_height = 400;
int g_posX = 100, g_posY = 100;
int g_startTime = 0, g_endTime;

float rotacija_x           = 0.0f;
float rotacija_y           = 0.0f;
int   last_x = 0;
int   last_y = 0;

bool init_s=true;

float kot=0;
int Y=25;
int X=25;
int Z=25;
float speed=0.5;



void init(void) {
	GLfloat global_ambient[] = {0.5,0.5,0.5,1.0};
	GLfloat light_position[] = {1.0,10.0,0.0,1.0};
	GLfloat light_diffuse[] = {1.0,1.0,1.0,1.0};

	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,global_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

GLUquadricObj* objekt= gluNewQuadric();

void onDraw(void) {
	g_endTime = GetTickCount();
	int diffTime = g_endTime-g_startTime;
	if (diffTime<1000.0/FRAME_RATE) return;
	g_startTime = g_endTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0,0,-5);
	glRotatef(-rotacija_y,1,0,0);
	glRotatef(-rotacija_x,0,1,0);





	gluLookAt(X,Y,Z,0,0,0,0,1,0);//vp xyz, lp xyz, v xyz




	glPushMatrix();//tirnice
	glColor3f(1,1,1);//bela
	glRotatef(90,1,0,0);
//	gluDisk(objekt,29.5,30,100,1);
	gluDisk(objekt,29.5,30,100,1);
	gluCylinder(objekt,29.5,29.5,0.5,50,50);
	gluCylinder(objekt,30,30,0.5,50,10);
	//glTranslatef(-0.5,0,0);

//	gluDisk(objekt,26,26.5,100,1);
	gluDisk(objekt,25.5,26,100,1);
	gluCylinder(objekt,25.5,26,0.5,50,50);
	gluCylinder(objekt,26,26,0.5,50,50);
	glPopMatrix();//tirnice
glPushMatrix();//vlak


int i;
for (i=0;i<=360;i=i+10)
{

glPushMatrix();//tirnice
glColor3f(0.7,0.4,0.01);//rjava
glRotatef(i,0,1,0);		

glTranslatef(0,-0.5,27.8);
	glScalef(1,0.5,6);
	glutSolidCube(1);//kabina
glPopMatrix();//vlak

}
glPopMatrix();//vlak

glPushMatrix();//vlak
	glRotatef(kot,0,1,0);//vlak se pelje
	glTranslatef(0,1.5,27.4);// da se vlakec vozi po tirnicah

glPushMatrix();//podvozje
	glColor3f(1,1,0);//rumena
	glTranslatef(0,1,0);
	glScalef(12.9,2.5,3);
	glutSolidCube(1);
glPopMatrix();
		
	glPushMatrix();//trup
	glColor3f(0,0.7,0);//zelena
	glTranslatef(0,2.5,0);
	glScalef(13,1.5,5);
	glutSolidCube(1);
	glPopMatrix();
/////////////////////////////////////////////pal?ke na kabini
	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-5.8,7.3,2.2);
	glScalef(0.5,8,0.5);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-5.8,7.3,-2.2);
	glScalef(0.5,8,0.5);
	glutSolidCube(1);//kabina
	glPopMatrix();	

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-1.4,7.3,2.2);
	glScalef(0.5,8,0.5);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-1.4,7.3,-2.2);
	glScalef(0.5,8,0.5);
	glutSolidCube(1);//kabina
	glPopMatrix();
/////////////////////////////konc pal?k

////////////////////stene kabine
	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-3.6,5.5,-2.2);
	glScalef(4,3.3,0.2);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-3.6,5.5,2.2);
	glScalef(4,3.3,0.2);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-1.2,5.5,0);
	glScalef(0.2,3.3,4);
	glutSolidCube(1);//kabina
	glPopMatrix();

/////////////////////////////////////?ipa

	glPushMatrix();
	glColor4f(0.1,0.1,0.1,0.6);//rde?a
	glTranslatef(-1.2,9,0);
	glScalef(0.2,3.3,4);
	glutSolidCube(1);//kabina
	glPopMatrix();

//////////////////////////////////////

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-3.6,3.5,2.2);
	glScalef(4,0.5,0.5);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-3.6,3.5,-2.2);
	glScalef(4,0.5,0.5);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-1.4,3.5,0);
	glScalef(0.5,0.5,4);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-3.6,7,2.2);
	glScalef(4,0.5,0.5);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-3.6,7,-2.2);
	glScalef(4,0.5,0.5);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-1.4,7,0);
	glScalef(0.5,0.5,4);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-3.6,11,2.2);
	glScalef(4,0.5,0.5);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-3.6,11,-2.2);
	glScalef(4,0.5,0.5);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-1.4,11,0);
	glScalef(0.5,0.5,4);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-5.8,11,0);
	glScalef(0.5,0.5,4);
	glutSolidCube(1);//kabina
	glPopMatrix();


/////////////////////////stene kabine

//streha
	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-3.7,11.9,-1.3);
	
	
	glRotatef(-20,1,0,0);
	glScalef(5,0.1,2.8);
	glutSolidCube(1);//kabina
	glPopMatrix();

	glPushMatrix();
	glColor3f(1,0,0);//rde?a
	glTranslatef(-3.7,11.9,1.3);
	
		
	glRotatef(20,1,0,0);
	glScalef(5,0.2,2.8);
	glutSolidCube(1);//kabina
	glPopMatrix();
//streha

	glPushMatrix();//bojler pri vlaku
	glColor3f(0,0.7,0);//pink
	glTranslatef(-1.1,5.1,0);
	glRotatef(90,0,1,0);
	gluCylinder(objekt,2,2,7.5,50,10);
	gluDisk(objekt,0,2,50,1);
	glTranslatef(0,0,7.5);
	gluDisk(objekt,0,2,50,1);
	glPopMatrix();
	
	glPushMatrix();//raufnk
	glColor3f(1,1,0);//pink	
	glTranslatef(3,8,0);
	glRotatef(-90,1,0,0);	
	gluDisk(objekt,0,0.8,50,1);
	glTranslatef(0,0,-1);
	glColor3f(0,1,1);//pink
	gluCylinder(objekt,0.4,0.8,1,20,10);

	glPopMatrix();



	glPushMatrix();

		glTranslatef(7,2.5,-1);
		glRotatef(-90,0,1,0);
		glColor3f(0,1,1);
		gluCylinder(objekt,0.2,0.2,0.5,20,10);
		gluDisk(objekt,0.2,0.5,50,1);
		glTranslatef(0,0,-0.2);
		glColor3f(1,1,0);
		gluCylinder(objekt,0.5,0.5,0.2,20,10);
					glColor3f(1,0,0);
		gluDisk(objekt,0,0.5,50,1);






	glPopMatrix();

	glPushMatrix();//krnekej


		glTranslatef(7,2.5,1);
		glRotatef(-90,0,1,0);
		glColor3f(0,1,1);
		gluCylinder(objekt,0.2,0.2,0.5,20,10);
		gluDisk(objekt,0.2,0.5,50,1);
		glTranslatef(0,0,-0.2);
		glColor3f(1,1,0);
		gluCylinder(objekt,0.5,0.5,0.2,20,10);
			glColor3f(1,0,0);
		gluDisk(objekt,0,0.5,50,1);

	glPopMatrix();


		glPushMatrix();//luc

		glTranslatef(7,5,0);
		glRotatef(-90,0,1,0);
		glColor3f(0,1,1);
		gluCylinder(objekt,0.5,0.2,0.5,20,10);
		glColor4f(1,1,1,0.8);
		gluDisk(objekt,0.2,0.5,50,1);
	
	glPopMatrix();




	glPushMatrix();//kolo..
glTranslatef(-4,0,-2);
		glColor3f(1,1,1);//belo
		glPushMatrix();//os
			glRotatef(kot*-6.4,0,0,1);//kolesa se vrtijo
			glPushMatrix();//os
				glTranslatef(0,0,-0.2);
				gluCylinder(objekt,0.4,0.4,4.5,50,50);//os	
				gluDisk(objekt,0.0,0.4,50,1);//zakljucek osi
				glPushMatrix();//os
					glTranslatef(0,0,4.5);
					gluDisk(objekt,0.0,0.4,50,1);//zakljucek osi
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();//feltna
				glPushMatrix();//feltna
					glTranslatef(0,0,-0.2);
					gluCylinder(objekt,1.5,1.5,0.5,50,50);//zunanja feltna
					gluCylinder(objekt,1.4,1.4,0.5,50,50);//notranji del fltne
					gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
					glPushMatrix();
						glTranslatef(0,0,0.5);
						gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
					glPopMatrix();	
				glPopMatrix();//feltna
				glTranslatef(0,0,0.05);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);	
					glutSolidCube(1);
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);	
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);
				glPopMatrix();
			glPopMatrix();
			glTranslatef(0,0,4);
			glPushMatrix();//feltna
				glTranslatef(0,0,-0.2);
				gluCylinder(objekt,1.5,1.5,0.5,50,50);//zunanja feltna
				gluCylinder(objekt,1.4,1.4,0.5,50,50);//notranji del feltne
				gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
				glPushMatrix();
					glTranslatef(0,0,0.5);
					gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
				glPopMatrix();
			glPopMatrix();//feltna
			glPushMatrix();//?prikle:)
				glPushMatrix();
					glScalef(0.5,2.8,0.4);	
					glutSolidCube(1);
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);
				glPopMatrix();
			glPopMatrix();//?prikle:)
		glPopMatrix();	
	glPopMatrix();

	glPushMatrix();//kolo..
	glTranslatef(4,0,-2);
		glColor3f(1,1,1);//belo
		glPushMatrix();//os
			glRotatef(kot*-6,0,0,1);//kolesa se vrtijo
			glPushMatrix();//os
				glTranslatef(0,0,-0.2);
				gluCylinder(objekt,0.4,0.4,4.5,50,50);//os	
				gluDisk(objekt,0.0,0.4,50,1);//zakljucek osi
				glPushMatrix();//os
					glTranslatef(0,0,4.5);
					gluDisk(objekt,0.0,0.4,50,1);//zakljucek osi
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();//feltna
				glPushMatrix();//feltna
					glTranslatef(0,0,-0.2);
					gluCylinder(objekt,1.5,1.5,0.5,50,50);//zunanja feltna
					gluCylinder(objekt,1.4,1.4,0.5,50,50);//notranji del fltne
					gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
					glPushMatrix();
						glTranslatef(0,0,0.5);
						gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
					glPopMatrix();	
				glPopMatrix();//feltna
				glTranslatef(0,0,0.05);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);	
					glutSolidCube(1);
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);	
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);
				glPopMatrix();
			glPopMatrix();
			glTranslatef(0,0,4);
			glPushMatrix();//feltna
				glTranslatef(0,0,-0.2);
				gluCylinder(objekt,1.5,1.5,0.5,50,50);//zunanja feltna
				gluCylinder(objekt,1.4,1.4,0.5,50,50);//notranji del feltne
				gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
				glPushMatrix();
					glTranslatef(0,0,0.5);
					gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
				glPopMatrix();
			glPopMatrix();//feltna
			glPushMatrix();//?prikle:)
				glPushMatrix();
					glScalef(0.5,2.8,0.4);	
					glutSolidCube(1);
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);
				glPopMatrix();
			glPopMatrix();//?prikle:)
		glPopMatrix();	
	glPopMatrix();

	glPopMatrix();//vse se bo vozilo po tirnicah 



glPushMatrix();//podvozje vagon?ka
//glTranslatef(20,0,0);
glRotatef(kot,0,1,0);//vlak se pelje
glTranslatef(-15,1.5,23.2);// da se vlakec vozi po tirnicah

glRotatef(-38,0,1,0);//vlak se pelje


	glPushMatrix();//kolo..
glTranslatef(-4,0,-2);
		glColor3f(1,1,1);//belo
		glPushMatrix();//os
			glRotatef(kot*-6,0,0,1);//kolesa se vrtijo
			glPushMatrix();//os
				glTranslatef(0,0,-0.2);
				gluCylinder(objekt,0.4,0.4,4.5,50,50);//os	
				gluDisk(objekt,0.0,0.4,50,1);//zakljucek osi
				glPushMatrix();//os
					glTranslatef(0,0,4.5);
					gluDisk(objekt,0.0,0.4,50,1);//zakljucek osi
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();//feltna
				glPushMatrix();//feltna
					glTranslatef(0,0,-0.2);
					gluCylinder(objekt,1.5,1.5,0.5,50,50);//zunanja feltna
					gluCylinder(objekt,1.4,1.4,0.5,50,50);//notranji del fltne
					gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
					glPushMatrix();
						glTranslatef(0,0,0.5);
						gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
					glPopMatrix();	
				glPopMatrix();//feltna
				glTranslatef(0,0,0.05);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);	
					glutSolidCube(1);
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);	
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);
				glPopMatrix();
			glPopMatrix();
			glTranslatef(0,0,4);
			glPushMatrix();//feltna
				glTranslatef(0,0,-0.2);
				gluCylinder(objekt,1.5,1.5,0.5,50,50);//zunanja feltna
				gluCylinder(objekt,1.4,1.4,0.5,50,50);//notranji del feltne
				gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
				glPushMatrix();
					glTranslatef(0,0,0.5);
					gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
				glPopMatrix();
			glPopMatrix();//feltna
			glPushMatrix();//?prikle:)
				glPushMatrix();
					glScalef(0.5,2.8,0.4);	
					glutSolidCube(1);
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);
				glPopMatrix();
			glPopMatrix();//?prikle:)
		glPopMatrix();	
	glPopMatrix();

	glPushMatrix();//kolo..
glTranslatef(4,0,-2.5);
		glColor3f(1,1,1);//belo
		glPushMatrix();//os
			glRotatef(kot*-6,0,0,1);//kolesa se vrtijo
			glPushMatrix();//os
				glTranslatef(0,0,-0.2);
				gluCylinder(objekt,0.4,0.4,4.5,50,50);//os	
				gluDisk(objekt,0.0,0.4,50,1);//zakljucek osi
				glPushMatrix();//os
					glTranslatef(0,0,4.5);
					gluDisk(objekt,0.0,0.4,50,1);//zakljucek osi
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();//feltna
				glPushMatrix();//feltna
					glTranslatef(0,0,-0.2);
					gluCylinder(objekt,1.5,1.5,0.5,50,50);//zunanja feltna
					gluCylinder(objekt,1.4,1.4,0.5,50,50);//notranji del fltne
					gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
					glPushMatrix();
						glTranslatef(0,0,0.5);
						gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
					glPopMatrix();	
				glPopMatrix();//feltna
				glTranslatef(0,0,0.05);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);	
					glutSolidCube(1);
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);	
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);
				glPopMatrix();
			glPopMatrix();
			glTranslatef(0,0,4);	//zunanje sprednje kolo pri vagon?ku
			glPushMatrix();//?prikle:)
				glPushMatrix();
					glScalef(0.5,2.8,0.4);	
					glutSolidCube(1);
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);
				glPopMatrix();
				glRotatef(66,0,0,1);
				glPushMatrix();
					glScalef(0.5,2.8,0.4);
					glutSolidCube(1);
				glPopMatrix();
			glPopMatrix();//?prikle:)
			glPushMatrix();//feltna
				glTranslatef(0,0,-0.2);
				gluCylinder(objekt,1.5,1.5,0.5,50,50);//zunanja feltna
				gluCylinder(objekt,1.4,1.4,0.5,50,50);//notranji del feltne
				gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
				glPushMatrix();
					glTranslatef(0,0,0.5);
					gluDisk(objekt,1.4,1.5,50,1);//zakljucek feltne
				glPopMatrix();
			glPopMatrix();//feltna

		glPopMatrix();	
	glPopMatrix();

glTranslatef(0,0,-0.3);// popravek da se vagon?ek vozi po tirnicah

glRotatef(2.9,0,1,0);//vlak se pelje


glPushMatrix();//podvozje
	glColor3f(1,1,0);//rumena
	glTranslatef(0,0.5,0);
	glScalef(12.9,2,3);
	glutSolidCube(1);
glPopMatrix();


/////////////////////////////?tenge
glPushMatrix();
glTranslatef(0,0,1.5);
glPushMatrix();//podvozje
	glColor3f(1,1,0);//rumena
	glTranslatef(0,0,0);
	glScalef(1,0.3,1);
	glutSolidCube(1);
glPopMatrix();

glPushMatrix();//podvozje
	glColor3f(1,1,0);//rumena
	glTranslatef(0,-0.3,0.5);
	glScalef(1,1,0.3);
	glutSolidCube(1);
glPopMatrix();


glPushMatrix();//podvozje
	glColor3f(1,1,0);//rumena
	glTranslatef(0,-0.8,0.8);
	glScalef(1,0.3,0.8);
	glutSolidCube(1);
glPopMatrix();
glPopMatrix();


glPushMatrix();//trup
	glColor3f(1,1,0);//rumena
	glTranslatef(6,4,1.8);
	glRotatef(8,1,0,0);	
	glScalef(0.3,5,0.3);
	glutSolidCube(1);
glPopMatrix();

glPushMatrix();//trup
	glColor3f(1,1,0);//rumena
	glTranslatef(-6,4,1.8);
	glRotatef(8,1,0,0);	
	glScalef(0.3,5,0.3);
	glutSolidCube(1);
glPopMatrix();

glPushMatrix();//trup
	glColor3f(1,1,0);//rumena
	glTranslatef(6,4,-1.8);
	glRotatef(-8,1,0,0);	
	glScalef(0.3,5,0.3);
	glutSolidCube(1);
glPopMatrix();

glPushMatrix();//trup pal?ke
	glColor3f(1,1,0);//rumena
	glTranslatef(-6,4,-1.8);
	glRotatef(-8,1,0,0);	
	glScalef(0.3,5,0.3);
	glutSolidCube(1);
glPopMatrix();

glPushMatrix();//tovor
glColor3f(0.7,0.4,0.1);//rjava
glTranslatef(-6,3,0);
glRotatef(90,0,1,0);

glPushMatrix();//tovor

glPushMatrix();//tovor
gluCylinder(objekt,0.4,0.6,13,50,50);//hlod
gluDisk(objekt,0,0.4,50,1);
glTranslatef(0,0,13);
gluDisk(objekt,0,0.6,50,1);
glPopMatrix();

glPushMatrix();
glTranslatef(1,0.3,1);
gluCylinder(objekt,0.6,0.4,13,50,50);//hlod
gluDisk(objekt,0,0.6,50,1);
glTranslatef(0,0,13);
gluDisk(objekt,0,0.4,50,1);
glPopMatrix();

glPushMatrix();
glTranslatef(-1,-0.1,-1.5);
glRotatef(4,0,0,1);

gluCylinder(objekt,0.5,0.3,13,50,50);//hlod

gluDisk(objekt,0,0.5,50,1);
glTranslatef(0,0,13);
gluDisk(objekt,0,0.3,50,1);

glPopMatrix();

glPushMatrix();
glTranslatef(-1,1,0);
glRotatef(1,1,0,0);

gluCylinder(objekt,0.27,0.21,10,50,50);//hlod
gluDisk(objekt,0,0.27,50,1);
glTranslatef(0,0,10);
gluDisk(objekt,0,0.21,50,1);

glPopMatrix();

glPushMatrix();
glTranslatef(1.4,1.1,0);
glRotatef(1,1,0,0);
//glRotatef(179.5,0,1,0);
gluCylinder(objekt,0.2,0.27,13,50,50);//hlod
gluDisk(objekt,0,0.2,50,1);
glTranslatef(0,0,13);
gluDisk(objekt,0,0.27,50,1);
glPopMatrix();

glPushMatrix();
glTranslatef(1.1,-0.9,0);
glRotatef(1,1,0,0);
//glRotatef(179.5,0,1,0);
gluCylinder(objekt,0.2,0.27,13,50,50);//hlod
gluDisk(objekt,0,0.2,50,1);
glTranslatef(0,0,13);
gluDisk(objekt,0,0.27,50,1);

glPopMatrix();

glPushMatrix();
glTranslatef(-1.2,-0.7,0);
glRotatef(1,1,0,0);
//glRotatef(179.5,0,1,0);
gluCylinder(objekt,0.2,0.27,13,50,50);//hlod
gluDisk(objekt,0,0.2,50,1);
glTranslatef(0,0,13);
gluDisk(objekt,0,0.27,50,1);

glPopMatrix();

glPopMatrix();
glPopMatrix();



	glPopMatrix();//vagon?ek

	glPopMatrix();//vse se bo vozilo po tirnicah 
	
	kot=kot+speed;

	glutSwapBuffers();
}

void onResize(int width, int height) {
	glViewport(0,0,(GLsizei)width,(GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0,1.0,1.0,100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void onKeyDown(unsigned char key, int x, int y) {
	switch (key) {
		case 27: 
		//   glutDestroyWindow();
		exit(0);
     break;
	}
}

void onSpecKeyDown(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_F1: {
			g_fullScreen = !g_fullScreen;
			if (g_fullScreen) {
				g_width = glutGet(GLUT_WINDOW_WIDTH); g_height = glutGet(GLUT_WINDOW_HEIGHT);
				g_posX = glutGet(GLUT_WINDOW_X); g_posY = glutGet(GLUT_WINDOW_Y);
				glutFullScreen();
			}
			else {
				glutReshapeWindow(g_width,g_height);
				glutPositionWindow(g_posX, g_posY);
			}
			glutPostRedisplay();
			break;

						  }
			case GLUT_KEY_UP: {
			X++;
			
			glutPostRedisplay();
			break;}

			case GLUT_KEY_DOWN: {
			X--;
			
			glutPostRedisplay();
			break;}		
			
			case GLUT_KEY_LEFT: {
			Z++;
			
			glutPostRedisplay();
			break;}

			case GLUT_KEY_RIGHT: {
			Z--;
			glutPostRedisplay();
			break;
								 }	

			case GLUT_KEY_PAGE_UP: {
			Y++;
			glutPostRedisplay();
			break;	}
			
			case GLUT_KEY_PAGE_DOWN: {
			Y--;
			glutPostRedisplay();
			break;
								 }	
			case GLUT_KEY_HOME: {
			speed=speed+0.5;
			glutPostRedisplay();
			break;
								 }	
			case GLUT_KEY_END: {
			speed=speed-0.5;
			glutPostRedisplay();
			break;
								 }	
				  }
	
}

void onMouse(int button, int state, int x, int y) {
	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		last_x = x;
        last_y = y;
	}

}

void motionFunc( int x, int y )
{
	rotacija_x -= (x - last_x);
	rotacija_y -= (y - last_y);

	last_x = x;
    last_y = y;
}

int main(int argc, char** argv) {
	//int argc = 1;
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_width,g_height);
	glutInitWindowPosition(g_posX, g_posY);
	glutCreateWindow("Jebemti Jebemti Chu Chu");
	init();
	glutDisplayFunc(onDraw);
	glutIdleFunc(onDraw);
	glutReshapeFunc(onResize);
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecKeyDown);
	glutMouseFunc(onMouse);
		
	glutMotionFunc( motionFunc );







	if (g_fullScreen){ glutFullScreen()
		;}
	g_startTime = GetTickCount();
	glutMainLoop();
	return 0;
}
