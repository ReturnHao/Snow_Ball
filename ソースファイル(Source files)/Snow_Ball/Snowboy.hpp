//
//  Snowboy.hpp
//  Snow_Ball_Game
//
//  Created by Return on 2018/12/30.
//  Copyright © 2018 Return. All rights reserved.
//

#ifndef Snowboy_hpp
#define Snowboy_hpp

#include <stdio.h>
#include <GL/glut.h>

class Snowboy
{
public:
	GLfloat x, y;
	int col, num, index[4][3][2];

	int count, step_count, _step_count;
   	float radius;
	unsigned int texture;

	typedef enum { left, right, front, back }direction;

	direction dir = front;

	float Step = 0.2f;

	Snowboy(GLfloat pos_x, GLfloat pos_y, unsigned int _texture, float radius);

	void drawRect(unsigned int texture, int i, int j);

	void drawSnowboy();
};

#endif /* Snowboy_hpp */
