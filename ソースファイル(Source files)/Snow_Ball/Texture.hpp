//
//  Texture.hpp
//  Snow_Ball_Game
//
//  Created by Return on 2018/12/30.
//  Copyright © 2018 Return. All rights reserved.
//
#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <GL/glut.h>
void loadTex(int i, const char *filename, GLuint* texture);

void loadTex(int i, const char *filename, GLuint* texture, unsigned char* backgroundColor);

#endif /* Texture_hpp */
