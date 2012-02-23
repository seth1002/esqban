// Echo.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <wchar.h>
#include <stdio.h>
#include <math.h>
#include <process.h>
#include <gl/glut.h>


short buffer[1024];	// intermediate buffer for reading
int readBytes = 1;

DWORD				gdwThreadId;
HANDLE				ghThread;

// synchronisation objects
CRITICAL_SECTION	gCriticalSection;


DWORD WINAPI ThreadProc(LPVOID pParam);
void StartThread();
BOOL StopThread();


typedef float vec_t;
typedef vec_t vec3_t[3];

enum {
    LEFT = 0, 
    RIGHT,
};

int mButton = -1;
int mOldY, mOldX;

vec3_t eye = {0.0f, 0.0f, 2.5f};
//vec3_t rot = {45.0f, 45.0f, 0.0f};
vec3_t rot = {0.0f, 0.0f, 0.0f};

float alpha = 1.0f;

void glutResize(int width, int height)
{   
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55.0, (float)width/(float)height, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
}

void glutKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
			exit(0);

		case 'A':
		case 'a':
			alpha += 0.1f;
			return;

		case 'Z':
		case 'z':
			alpha -= 0.1f;
			return;
			
		case 'Q':
		case 'q':
			exit(0);
	}
}

void clamp(vec3_t v)
{
    int i;
    
    for (i = 0; i < 3; i ++)
        if (v[i] > 360 || v[i] < -360)
            v[i] = 0;
}

void glutMotion(int x, int y) 
{
    if (mButton == LEFT) 
    {
        rot[0] -= ((mOldY - y) * 180.0f) / 200.0f;
        rot[1] -= ((mOldX - x) * 180.0f) / 200.0f;
        clamp (rot);
    }
	else if (mButton == RIGHT) 
    {
        eye[2] -= ((mOldY - y) * 360.0f) / 1000.0f;
		if(eye[2] <= 1.0)
			eye[2] = 1.1;
        clamp (rot);
    } 

    mOldX = x; 
    mOldY = y;
}

void glutMouse(int button, int state, int x, int y) 
{
    if(state == GLUT_DOWN) 
    {
        mOldX = x;
        mOldY = y;
        switch(button)  
        {
            case GLUT_LEFT_BUTTON:
                mButton = LEFT;
            break;
            case GLUT_RIGHT_BUTTON:
                mButton = RIGHT;
            break;
        }
        } else if (state == GLUT_UP) 
            mButton = -1;
}

void glutMenu(int value)
{
	switch (value)
	{
		case 1:
			glutFullScreen();
			return;
		
		case 2:
			exit(0);
	}
}

void glutDisplay(void)
{   
//	short buffer[1024];	// intermediate buffer for reading
//	int pos = 0;
//	int readBytes = 1;
//	int bufflen = sizeof(buffer);
	int x=0;

	int sampleSize = sizeof(short) * 8;
		
//	readBytes = fread( buffer, sizeof(char), bufflen, stdin );

	short localBuffer[1024];

	EnterCriticalSection(&gCriticalSection);

//	readBytes = fread( buffer, sizeof(char), bufflen, stdin );

//	fwrite( buffer, 1, readBytes, stdout );

	int samples = (readBytes*8) / sampleSize;

	if(readBytes == 0)
	{
		LeaveCriticalSection(&gCriticalSection);
		exit(0);
	} else if (readBytes == 0)
	{
		LeaveCriticalSection(&gCriticalSection);
		return;
	}

	memcpy(localBuffer, buffer, sizeof(buffer));

	LeaveCriticalSection(&gCriticalSection);

	//GLint x, y, n=0, yPrev=0;
	GLfloat yl, yr, n=-0.3, step=0.01, ylPrev=0.f, yrPrev=0.f, lmax=0.f, rmax=0.f;

	GLint width = sizeof(buffer) / sizeof(short) / 2;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
	glTranslatef (-eye[0], -eye[1], -eye[2]);	  

    glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
    glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
    glRotatef(rot[2], 0.0f, 0.0f, 1.0f);

	glColor4f(0.0f, 0.0f, 1.0f, alpha);
	//glColor3f(0.0f, 0.0f, 1.0f);

	//fprintf(stderr, "samples=%d\n", samples);
	//glTranslatef(-samples/2*step, 0.0f, 0.0f);
	glTranslatef(-0.9f, 0.0f, 0.0f);

	glBegin(GL_LINES);
	for(x=0 ; x < samples/2; x+=2)  // display Input
	{
		yl = (GLfloat)localBuffer[x] / 100000.f;
		yr = (GLfloat)localBuffer[x+1] / 100000.f;

		if(yl > lmax)
			lmax = yl;
		if(yr > rmax)
			rmax = yr;

		yl += 0.2;
		yr -= 0.2;

		//printf("yl=%f lmax=%f\n", yl, lmax);
		//printf("yr=%f rmax=%f\n", yr, rmax);

		if(x != 0)
		{
			//glColor3f(0.0f, 0.0f, 1.0f);
			glVertex2f(n-step, ylPrev);

			//glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(n, yl);
		}


		if(x != 0)
		{
			//glColor3f(0.0f, 0.0f, 1.0f);
			glVertex2f(n-step, yrPrev);

			//glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(n, yr);
		}

		ylPrev = yl;
		yrPrev = yr;

		n += step;
	}
	glEnd();

	glTranslatef(-0.5f, 0.0f, 0.0f);

	glBegin(GL_QUADS);						// Draw A Quad
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.05f, lmax*4, 0.0f);	// Top Left
		glVertex3f( 0.05f, lmax*4, 0.0f);	// Top Right
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f( 0.05f, 0.0f, 0.0f);		// Bottom Right
		glVertex3f(-0.05f, 0.0f, 0.0f);		// Bottom Left

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, -0.0f, 0.0f);	// Top Left
		glVertex3f( 0.05f, -0.0f, 0.0f);	// Top Right
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f( 0.05f, -rmax*4, 0.0f);	// Bottom Right
		glVertex3f(-0.05f, -rmax*4, 0.0f);	// Bottom Left
	glEnd();								// Done Drawing The Quad

	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}



int main(int argc, char* argv[])
{
	if( -1 == setmode ( fileno ( stdin ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return -1;
	}

	if( -1 == setmode ( fileno ( stdout ), O_BINARY ) )
	{
		perror( "Cannot set mode" );
		return -1;
	}


	StartThread();


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);// | GLUT_DEPTH);
	glutInitWindowSize(200, 150);
	glutInitWindowPosition(100, 100);

	char *name = ( argc == 2 ? argv[1] : TEXT("untitled") );
	char caption[100] = {0};
	sprintf(caption, "OpenGL Buffer View - %s\0", name);
	glutCreateWindow(caption);

	glutReshapeFunc(glutResize);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutDisplay);
    glutKeyboardFunc(glutKeyboard);
	glutMouseFunc(glutMouse);
    glutMotionFunc(glutMotion);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	glutCreateMenu(glutMenu);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	glutAddMenuEntry("Full Screen", 1);
	glutAddMenuEntry("Exit", 2);

	glutMainLoop();



/*
	while(readBytes > 0) 
	{
		readBytes = fread( buffer, sizeof(char), bufflen, stdin );

		int samples = (readBytes*8) / sampleSize;

		for(y=0; y<samples/2; y+=2)
		{

		//	fprintf(fp, "%c %f %f ", (y==0?'M':'L'), 
		//		(65535+buffer[y])/100.f, (65535+buffer[y+1])/100.f);
		
		//	fprintf(fp, "%c %d %f ", (y==0?'M':'L'), 
		//		x, (65535+buffer[y])/10.f);


			//fprintf(fp, "%c %d %d ", (y==0?'M':'L'), x, buffer[y]);

			counter++;
			x++;
		}

		UINT n = fwrite( buffer, 1, readBytes, stdout );
	}
*/
	return 0;
}


DWORD WINAPI ThreadProc(LPVOID pParam)
{
	while(readBytes > 0) 
	{
		EnterCriticalSection(&gCriticalSection);

		readBytes = fread( buffer, sizeof(char), sizeof(buffer), stdin );
		UINT n = fwrite( buffer, 1, readBytes, stdout );

		LeaveCriticalSection(&gCriticalSection);
	}

	return 0;
}

void StartThread()
{
	// initialize critical section
	InitializeCriticalSection(&gCriticalSection);

    ghThread = CreateThread(NULL,			// no security attributes 
							0,				// use default stack size  
							ThreadProc,		// thread function 
							NULL,			// argument to thread function 
							0,				// use default creation flags 
							&gdwThreadId);	// returns the thread identifier 
}

BOOL StopThread()
{
	return( SuspendThread(ghThread) != -1 );
}
