#pragma once
#include "Angel.h"
#include <fstream>
#include <string>
using namespace std;
class Model
{
	GLuint vao;
	int NumVertices;// In vertex array as sent to GPU
	int NumIndices;// Number of indices as sent to GPU
	int NumNormals;// In Normal array as sent to GPU.  Same as NumVertices
	vec4 *Vertices;// Vertex array sent to GPU
	unsigned int *Indices;// Index array sent to GPU
	vec4 *Normals;
public:
	Model(void);
	~Model(void);
	bool Load(GLuint prog, char * file_name);
	void Draw(void);
};

