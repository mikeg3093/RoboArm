#include "Cube.h"


Cube::Cube(void)
{
	
}


Cube::~Cube(void)
{
}


bool Cube::Generate(void)
{
	 NumVertices = 8; //(6 faces)(2 triangles/face)(3 vertices/triangle)
     NumIndices= 36;
	

	vec4 cVertices[8] = {
      // front
    vec4(-0.5, -0.5, 0.5,1.0),
    vec4( 0.5, -0.5, 0.5,1),
    vec4(0.5,  0.5,  0.5,1),
    vec4(-0.5,  0.5, 0.5,1),
    // back
    vec4(-0.5, -0.5, -0.5,1),
    vec4( 0.5, -0.5, -0.5,1),
    vec4( 0.5,  0.5, -0.5,1),
    vec4(-0.5,  0.5, -0.5,1)
};

GLuint cIndices[36]={
	    // front
    0, 1, 2,
    2, 3, 0,
    // top
    3, 2, 6,
    6, 7, 3,
    // back
    7, 6, 5,
    5, 4, 7,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // left
    4, 0, 3,
    3, 7, 4,
    // right
    1, 5, 6,
    6, 2, 1
};
   int i;
   Vertices = new vec4 [NumVertices];
   for(i=0;i<NumVertices;i++)Vertices[i] = cVertices[i];

   Indices = new GLuint [NumIndices];
   for(i=0;i<NumIndices;i++)Indices[i] = cIndices[i];
	return true;
}


void Cube::Load()
{
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

	 // get locations of vertex shader variables
    GLuint vPosition = 0;  // We know that the position is 0

	    // Create and initialize a buffer object for the vertex, ie point data
    GLuint vertex_buffer;
    glGenBuffers( 1, &vertex_buffer );
    glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
	glBufferData( GL_ARRAY_BUFFER, NumVertices*16, Vertices, GL_STATIC_DRAW );
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,BUFFER_OFFSET(0) );

	GLuint index_buffer;
    glGenBuffers( 1, &index_buffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_buffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, NumIndices*4, Indices, GL_STATIC_DRAW );
}


bool Cube::Draw(void)
{
	glBindVertexArray( vao );
	glDrawElements( GL_TRIANGLES,NumIndices, GL_UNSIGNED_INT , BUFFER_OFFSET(0));
	glBindVertexArray( 0 );
	return false;
}
