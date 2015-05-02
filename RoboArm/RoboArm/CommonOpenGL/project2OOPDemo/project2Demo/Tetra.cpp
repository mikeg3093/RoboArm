#include "Tetra.h"


Tetra::Tetra(void)
{
	
}


Tetra::~Tetra(void)
{
}


bool Tetra::Generate(void)
{
	 NumVertices = 4; 
     NumIndices= 12;
	

	vec4 cVertices[4] = {
      // front

    vec4(0.0f, 1.0f, 0.0f,1.0),
    vec4( 0.943f, -0.333f, 0.0f,1),
    vec4(-0.471f, -0.333f, 0.8165f,1),
    vec4(-0.471f, -0.333f, -0.8165f,1),
};

GLuint cIndices[12]={
	    // front
    0, 2, 1,
	1, 3, 0,
	2, 1, 3,
	3, 2, 0
};
   int i;
   Vertices = new vec4 [NumVertices];
   for(i=0;i<NumVertices;i++)Vertices[i] = cVertices[i];

   Indices = new GLuint [NumIndices];
   for(i=0;i<NumIndices;i++)Indices[i] = cIndices[i];
	return true;
}


void Tetra::Load()
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


bool Tetra::Draw(void)
{
	glBindVertexArray( vao );
	glDrawElements( GL_TRIANGLES,NumIndices, GL_UNSIGNED_INT , BUFFER_OFFSET(0));
	glBindVertexArray( 0 );
	return false;
}
