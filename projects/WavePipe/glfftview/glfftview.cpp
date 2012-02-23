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



#define BUF_LENGTH	2048
#define FRAME_SIZE	BUF_LENGTH/4


BOOL	debug = FALSE;			// Shows debug message
short	buffer[BUF_LENGTH*2];	// intermediate buffer for reading
float	lbuffer[BUF_LENGTH];	// left FFT buffer
float	rbuffer[BUF_LENGTH];	// right FFT buffer
int		readBytes = 1;
DWORD	gdwThreadId;
HANDLE	ghThread;
CRITICAL_SECTION	gCriticalSection;	// synchronisation object


void	smbFft(float *fftBuffer, long fftFrameSize, long sign);
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
DWORD	WINAPI ThreadProc(LPVOID pParam);
void	ProcessFFT(short *buff);



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
	short localBuffer[BUF_LENGTH*2];

	EnterCriticalSection(&gCriticalSection);

	if(readBytes == 0) {
		LeaveCriticalSection(&gCriticalSection);
		exit(0);
	} else if (readBytes == 0) {
		LeaveCriticalSection(&gCriticalSection);
		return;
	}

	memcpy(localBuffer, buffer, sizeof(buffer));

	LeaveCriticalSection(&gCriticalSection);


	ProcessFFT(localBuffer);

	int x=0;
	int samples = FRAME_SIZE;


	GLfloat yl, yr, n=0.f, step=0.01, ylPrev=0.f, yrPrev=0.f;

	//GLint width = samples;//sizeof(buffer) / sizeof(short) / 2;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
	glTranslatef (-eye[0], -eye[1], -eye[2]);	  

    glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
    glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
    glRotatef(rot[2], 0.0f, 0.0f, 1.0f);

	glColor4f(0.0f, 0.0f, 1.0f, alpha);

	GLfloat f = samples * step / 2.f;
	glTranslatef(-f, 0.0f, 0.0f);

	glBegin(GL_LINES);
	for(x=1; x<=samples; x++)
	{
		yl = abs(lbuffer[x] / 65535.f) / 100.f + 0.2;
		yr = -abs(rbuffer[x] / 65535.f) / 100.f - 0.2;

		if(x > 1)
		{
			glVertex2f(n-step, ylPrev);
			glVertex2f(n, yl);

			glVertex2f(n-step, yrPrev);
			glVertex2f(n, yr);
		}

		ylPrev = yl;
		yrPrev = yr;

		n += step;
	}
/*
	short lamp=0, ramp=0;
	for(x=0; x<BUF_LENGTH; x+=2)
	{
		if(buffer[x] > lamp)
			lamp = buffer[x];
		if(buffer[x+1] > ramp)
			ramp = buffer[x+1];
	}
	GLfloat dbLeft  = 10.f * log(lamp / 32767.f);
	GLfloat dbRight = 10.f * log(ramp / 32767.f);

	glVertex2f(0.0f, 0.1f);
	glVertex2f(1.0f-abs(dbLeft), 0.1f);

	glVertex2f(0.0f, -0.1f);
	glVertex2f(1.0f-abs(dbRight), -0.1f);
*/
	glEnd();

/*
	glTranslatef(-0.5f, 0.0f, 0.0f);

	glBegin(GL_QUADS);						// Draw A Quad
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.05f, dbLeft, 0.0f);	// Top Left
		glVertex3f( 0.05f, dbLeft, 0.0f);	// Top Right
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f( 0.05f, 0.0f, 0.0f);		// Bottom Right
		glVertex3f(-0.05f, 0.0f, 0.0f);		// Bottom Left

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, -0.0f, 0.0f);	// Top Left
		glVertex3f( 0.05f, -0.0f, 0.0f);	// Top Right
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f( 0.05f, -dbRight, 0.0f);	// Bottom Right
		glVertex3f(-0.05f, -dbRight, 0.0f);	// Bottom Left
	glEnd();								// Done Drawing The Quad
*/
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


	// initialize critical section
	InitializeCriticalSection(&gCriticalSection);

    ghThread = CreateThread(NULL,			// no security attributes 
							0,				// use default stack size  
							ThreadProc,		// thread function 
							NULL,			// argument to thread function 
							0,				// use default creation flags 
							&gdwThreadId);	// returns the thread identifier 


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);// | GLUT_DEPTH);
	glutInitWindowSize(300, 150);
	glutInitWindowPosition(100, 100);

	char *name = ( argc == 2 ? argv[1] : TEXT("untitled") );
	char caption[100] = {0};
	sprintf(caption, "OpenGL FFT View - %s\0", name);
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

void ProcessFFT(short *buff)
{
	int i=0, n=0;

	int readSamples = readBytes / sizeof(short);

	n=0;
	for(i=0; i<readSamples; i+=2)
	{
		lbuffer[n++] = (float)buff[i];
	}
	smbFft(lbuffer, FRAME_SIZE, -1);


	n=0;
	for(i=1; i<readSamples; i+=2)
	{
		rbuffer[n++] = (float)buff[i];
	}
	smbFft(rbuffer, FRAME_SIZE, -1);
}
