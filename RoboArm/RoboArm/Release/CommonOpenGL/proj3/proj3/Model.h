#pragma once
#include "Angel.h"
#include <fstream>
#include <string>

using namespace std;
class Model
{
GLuint vao;
int NumVertices;
int NumIndices;
 vec4 * Vertices;
 GLuint * Indices;
public:
	Model(void);
	~Model(void);
	void Load(char*);
	bool Draw(void);
};

