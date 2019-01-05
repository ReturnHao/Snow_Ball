//
//  Snowboy.cpp
//  Snow_Ball_Game
//
//  Created by Return on 2018/12/30.
//  Copyright © 2018 Return. All rights reserved.
//

#include "Snowboy.hpp"
#include <GL/glut.h>

int _index[] = { 0, 10, 0, 11, 0, 12, 1, 10, 1, 11, 1, 12, 2, 10, 2, 11, 2, 12, 3, 10, 3, 11, 3, 12 };

Snowboy::Snowboy(GLfloat pos_x, GLfloat pos_y, unsigned int _texture, float _radius)
{
	count = step_count = _step_count = 0;
	col = 12;
	num = 96;
    x = pos_x;
    y = pos_y;
	texture = _texture;
	radius = _radius;

	int Idx = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 2; k++) index[i][j][k] = _index[Idx++];
}

void Snowboy::drawRect(unsigned int texture, int i, int j)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture); 

	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat x = 1.0 / col, y = 1.0 / (num / col);
	const GLfloat point[4][2] = { { x1, y1 }, { x2, y1 }, { x2, y2 }, { x1, y2 } };
	//const GLfloat dir[4][2] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
	const GLfloat dir[4][2] = { { j * x, 1 - (i + 1) * y },{ (j + 1) * x,1 - (i + 1) * y },{ (j + 1) * x , 1 - i * y },{ j * x, 1 - i * y } };
	glBegin(GL_QUADS);

	for (int k = 0; k < 4; k++)
	{
		glTexCoord2fv(dir[k]);
		glVertex2fv(point[k]);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Snowboy::drawSnowboy()
{
	const int step = 50;
	count++;

	if (dir == front) drawRect(texture, index[0][1][0], index[0][1][1]);
	else
		if (dir == back) drawRect(texture, index[3][1][0], index[3][1][1]);
		else
			if (dir == left) drawRect(texture, index[1][1][0], index[1][1][1]);
			else if (dir == right) drawRect(texture, index[2][1][0], index[2][1][1]);

	if (count%step == 0) {
		if (step_count == _step_count)
		{
			if (dir == front) {
				drawRect(texture, index[0][1][0], index[0][1][1]);
			}
			else if (dir == back) {
				drawRect(texture, index[3][1][0], index[3][1][1]);
			}
			else if (dir == left) {
				drawRect(texture, index[1][1][0], index[1][1][1]);
			}
			else if (dir == right) {
				drawRect(texture, index[2][1][0], index[2][1][1]);
			}
		}
		_step_count = step_count;
	}
	if (count == step * 3) {
		count = 0;
	}
	//drawRect(texture, 0, 0);
}