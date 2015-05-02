#pragma once
#include "Angel.h"
class Tetra
{
GLuint vao;
int NumVertices; //(6 faces)(2 triangles/face)(3 vertices/triangle)
int NumIndices;
 vec4 * Vertices;
 GLuint * Indices;
public:
	Tetra(void);
	~Tetra(void);
	bool Generate(void);
	void Load(void);
	bool Draw(void);
};

