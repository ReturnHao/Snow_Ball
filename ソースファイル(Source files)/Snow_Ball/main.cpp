//
//  main.cpp
//  Snow_Ball_Game
//
//  Created by Return on 2018/12/30.
//  Copyright © 2018 Return. All rights reserved.
//
#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <algorithm>
#include "Snowboy.hpp"
#include "Texture.hpp"
#include "Font.hpp"

using namespace std;
#define Block_size 0.4f

// View zone
float whRatio;
int wHeight = 0, wWidth = 0;

// Grid count
const int Grid_x = 20, Grid_y = 20;

// Texture mode
GLuint texture[5];

// View Window size
float Size_x = 3.9f, Size_y = 3.9f;

// View point
float center[] = {0, 0, 0};
float eye[] = {0, 0, 5};

// Grid position
float Place_x[40][40], Place_y[40][40];

int Hole_x = 18, Hole_y = 18;
float Hole_radius = 0.5f;

GLfloat eps = 1e-6;
Snowboy *Player;

int _Last = 0;
int isEnd = false, isTextFirst = false, First_call = true, Recall = false;

int map[20][20] = { 0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,
					0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,
					0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,
					0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
					0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,
					0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,
					1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,
					0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,
					0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,
					0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,
					0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,
					0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,
					0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,0,
					0,0,0,0,1,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,
					1,1,1,0,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,
					1,1,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,
					1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
					0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
					0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,
};

void drawText(const char *disp_string)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-5, 5, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0.04f, 0.06f, 0.07f);
	
	if (!isTextFirst)
	{
		selectFont(48, DEFAULT_CHARSET, "Comic Sans MS");
		glRasterPos2f(-3.0f, 0);
		drawString(disp_string);

		selectFont(24, DEFAULT_CHARSET, "Comic Sans MS");
		glRasterPos2f(-1.2f, -0.4f);
		drawString("Press Left Shift to start.");
	}
	else
	{
		selectFont(96, DEFAULT_CHARSET, "Comic Sans MS");
		glRasterPos2f(-1.8f, 0);
		drawString(disp_string);

		selectFont(24, DEFAULT_CHARSET, "Comic Sans MS");
		glRasterPos2f(-0.8f, -0.4f);
		drawString("Press F1 to restart.");
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void Init()
{
	if (Recall) Recall = 0, map[Hole_x][Hole_y] = _Last;
	isEnd = false, isTextFirst = false, First_call = true;
	
	srand((unsigned)time(NULL));
	Hole_radius = (rand() % 10) / 10.0;
	while ((Hole_radius < 0.2f) || (Hole_radius > 0.5f)) Hole_radius = (rand() % 10) / 10.0;
	Hole_x = rand() % 20, Hole_y = rand() % 20;
	while ((Hole_x == 1) && (Hole_y == 1)) Hole_x = rand() % 20, Hole_y = rand() % 20;
	_Last = map[Hole_x][Hole_y]; map[Hole_x][Hole_y] = -1;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	unsigned char color[] = {230, 230, 230};

	// 0: Snow 1: Ground 2: Snowboy 3: Background 4: Flag
	glGenTextures(4, texture);
	loadTex(0, "Snow.bmp", texture);  // 0
	loadTex(1, "Default.bmp", texture); // 1
	loadTex(2, "Snowboy.bmp", texture, color); // 2
	loadTex(3, "Background.bmp", texture); // 3
	loadTex(4, "Flag.bmp", texture, color); // 4

	Player = new Snowboy(-3.4f, -3.4f, texture[2], 0);

}

void drawRect(GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat point[4][2] = {{x1, y1}, {x2, y1}, {x2, y2}, {x1, y2}};
	int dir[4][2] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++)
	{
		glTexCoord2iv(dir[i]);
		glVertex2fv(point[i]);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawScene()
{
	// Draw Map
	glPushMatrix();
	glTranslatef(-3.8f, -3.8f, 0.0f);
	for (int i = 0; i < Grid_x; i++)
	{
		for (int j = 0; j < Grid_y; j++)
		{
			glPushMatrix();
			glScalef(Block_size, Block_size, 1);
			if (map[i][j] == -1)
			{
				drawRect(texture[1]);
				glColor3f(44 / 255.0, 39 / 255.0, 32 / 255.0);
				glutSolidSphere(Hole_radius, 20, 20);
				glColor3d(1.0, 1.0, 1.0);

				glPushMatrix();
				glTranslatef(0, 0, 1);
				drawRect(texture[4]);
				glPopMatrix();
			}
			else drawRect(texture[map[i][j]]);
			glPopMatrix();
			glTranslatef(Block_size, 0, 0);
		}
		glTranslatef(-Grid_y * Block_size, Block_size, 0);
	}
	glPopMatrix();

	// Draw Snowboy
	glPushMatrix();
	GLfloat ct_x = Player->x, ct_y = Player->y;
	if (Player->dir == Snowboy::left) ct_x += 0.15f;
	if (Player->dir == Snowboy::right) ct_x -= 0.15f;
	if (Player->dir == Snowboy::front) ct_y += 0.15f;
	if (Player->dir == Snowboy::back) ct_y -= 0.15f;
	glTranslatef(ct_x, ct_y, 2);
	glScalef(Block_size / 1.5, Block_size / 1.5, 1);
	Player -> drawSnowboy();
	glPopMatrix();

	// Draw Snowball
	glPushMatrix();
	if (Player->dir == Snowboy::front) glTranslatef(Player->x, Player->y, 3);
	else glTranslatef(Player->x, Player->y, 1);
	glScalef(Block_size, Block_size, 1);
	glColor3f(0.2f, 0.2f, 0.2f);
	glutSolidSphere(Player->radius, 20, 20);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();

	if (Player->radius <= 0.03f) return;

	glPushMatrix();
	if (Player->dir == Snowboy::front) glTranslatef(Player->x, Player->y, 3.5);
	else glTranslatef(Player->x, Player->y, 1.5);
	glScalef(Block_size, Block_size, 1);
	glutSolidSphere(Player -> radius -0.03f, 20, 20);
	glPopMatrix();
}

void updateView(int height, int width)
{
	//glViewport(0, 0, width * 2 * 3 / 4, height * 2);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);         
	glLoadIdentity();            
	whRatio = (GLfloat)width / (GLfloat)height;      
	glOrtho(-4, 4, -4, 4, -100, 100); 
	glMatrixMode(GL_MODELVIEW);       
}

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);
    glPolygonMode(GL_FRONT, GL_FILL);
	if (!isTextFirst)
	{
		glPushMatrix();
		glScalef(wHeight / 100, wWidth / 100, 1);
		drawRect(texture[3]);
		glPopMatrix();

		drawText("Push the Snowball into the hole!");

	}
	else drawScene();
	//drawText();
	if (isEnd)
	{
		drawText((isEnd == 1) ? ("You Win!") : ("You  Lose!"));
		if (First_call) First_call = 0, glutSwapBuffers();
		return;
	}
    glutSwapBuffers();
}

int map_inside[Grid_x][Grid_y];

void Match_closed()
{
	if (isEnd) return;

	if (fabs(Player->radius - Hole_radius) < 0.05f)
	{
		isEnd = 1;
		Player->radius = 0.0f;
	}
	else isEnd = 2;
}

void Collision_Detection()
{
	int cur_y = (Player->x + 4.0f) / Block_size, cur_x = (Player->y + 4.0f) / Block_size;
	if (map[cur_x][cur_y] == 0)
	{
		if (++map_inside[cur_x][cur_y] == 2) map[cur_x][cur_y] = 1;
		Player->radius += 0.05;
	}
	else
		if (map[cur_x][cur_y] == 1) Player->radius -= 0.02;
		else
			if (map[cur_x][cur_y] == -1) Match_closed();
	Player->radius = min(0.5f, max(Player->radius, 0.0f));
}

void Keyboard(int key, int x, int y)
{
	Player->step_count++;

    switch (key)
    {
        case GLUT_KEY_LEFT:
		{
			Player->dir = Snowboy::left;
			Player->x -= Player->Step;
			Player->x = max(Player->x, -Size_x);
			Collision_Detection();
			break;
		}
        case GLUT_KEY_RIGHT:
		{
			Player->dir = Snowboy::right;
			Player->x += Player->Step;
			Player->x = min(Player->x, Size_x);
			Collision_Detection();
			break;
		}
        case GLUT_KEY_UP:
		{
			Player->dir = Snowboy::back;
			Player->y += Player->Step;
			Player->y = min(Player->y, Size_y);
			Collision_Detection();
			break;
		}
        case GLUT_KEY_DOWN:
		{
			Player->dir = Snowboy::front;
			Player->y -= Player->Step;
			Player->y = max(Player->y, -Size_y);
			Collision_Detection();
			break;
		}
		case GLUT_KEY_F1:
		{
			Recall = true;
			Init();
			break;
		}
		default: {  isTextFirst = 1; break; }
    }
    
	updateView(wHeight, wWidth);
    //glutPostRedisplay();
    return;
}

void Reshape(int width, int height)
{
	if (height == 0) height = 1;

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);        
}

void Idle() { glutPostRedisplay(); }

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    //glutInitWindowSize(800, 600);
	glutInitWindowSize(800, 800);
    glutCreateWindow("Snow Ball");
    glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
    glutSpecialFunc(Keyboard);
    glutIdleFunc(Idle);
	Init();
    glutMainLoop();
    return 0;
}