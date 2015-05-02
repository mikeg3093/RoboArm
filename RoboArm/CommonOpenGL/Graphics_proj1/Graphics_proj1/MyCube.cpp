#include "MyCube.h"


MyCube::MyCube(void)
{
}


MyCube::~MyCube(void)
{
}

bool MyCube::Generate(void){

	NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

	vec4 vertices[8] = {
		vec4( -0.5, -0.5,  0.5, 1.0 ),
		vec4( -0.5,  0.5,  0.5, 1.0 ),
		vec4(  0.5,  0.5,  0.5, 1.0 ),
		vec4(  0.5, -0.5,  0.5, 1.0 ),
		vec4( -0.5, -0.5, -0.5, 1.0 ),
		vec4( -0.5,  0.5, -0.5, 1.0 ),
		vec4(  0.5,  0.5, -0.5, 1.0 ),
		vec4(  0.5, -0.5, -0.5, 1.0 )
	};

	vec4 vertex_colors[8] = {
		vec4( 0.0, 0.0, 0.0, 1.0 ),  // black
		vec4( 1.0, 0.0, 0.0, 1.0 ),  // red
		vec4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
		vec4( 0.0, 1.0, 0.0, 1.0 ),  // green
		vec4( 0.0, 0.0, 1.0, 1.0 ),  // blue
		vec4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
		vec4( 1.0, 1.0, 1.0, 1.0 ),  // white
		vec4( 0.0, 1.0, 1.0, 1.0 )   // cyan
	};

	int indices[24] = {
		1, 0, 3, 2, 
		2, 3, 7, 6, 
		3, 0, 4, 7, 
		6, 5, 1, 2, 
		4, 5, 6, 7, 
		5, 4, 0, 1
	};

	for(int i = 0; i < 24; i++){
		colors[i] = indices[i]; 
		points[i] = indices[i];
	}

	return true;
}

void MyCube::Load()
{
        // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );	

	GLuint program = InitShader( "vshader_a4.glsl", "fshader_a4.glsl" );
	glUseProgram( program );

	 // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

	GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(sizeof(points)) );

}

bool MyCube::Draw(void)
{
	glBindVertexArray( vao );
	glDrawElements( GL_TRIANGLES,NumVertices, GL_UNSIGNED_INT , BUFFER_OFFSET(0));
	glBindVertexArray( 0 );
	return false;
}

