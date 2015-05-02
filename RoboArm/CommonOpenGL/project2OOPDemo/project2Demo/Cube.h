#pragma once
#include "Angel.h"
class Cube
{
GLuint vao;
int NumVertices; //(6 faces)(2 triangles/face)(3 vertices/triangle)
int NumIndices;
 vec4 * Vertices;
 GLuint * Indices;
public:
	Cube(void);
	~Cube(void);
	bool Generate(void);
	void Load(void);
	bool Draw(void);
};

 