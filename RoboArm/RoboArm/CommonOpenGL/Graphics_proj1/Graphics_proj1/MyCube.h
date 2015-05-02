#pragma once
#include "Angel.h"
class MyCube
{
GLuint vao;
int NumVertices; //(6 faces)(2 triangles/face)(3 vertices/triangle)
vec4*points;
vec4*colors;
public:
	MyCube(void);
	~MyCube(void);
	bool Generate(void);
	void Load(void);
	bool Draw(void);
};
