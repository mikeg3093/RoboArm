/*	
	Michael Grayson
	Computer Graphics - 4213
	Project 1

	Creates a square-spiral image. Starts at 0,0 for x and y then creates vertices
	based on a square shape, moving right, down, left, and up, and then loops. 
	Uses GL_LINE_STRIP to create a line between sequential vertices.
*/
/*
#include "Angel.h"
#pragma comment(lib, "freeglut")  //add libraries
#pragma comment(lib, "glew32")

const int NumPoints = 81; //num vertices

//----------------------------------------------------------------------------

void
init( void )
{
	vec2 points[NumPoints];		// array of vec
	float x = 0;
	float y = 0;
	int i = 0;					//increment array
	float w = .05;				//length of a line
	float c = .05;				//increment the line length

	points[i++] = vec2(x,y);	//init point array 0,0

	for (int j = 1; j <= 20; j++){
		x = x + w;					// x to right
		points[i++] = vec2(x,y);
		y = y - w;					// y down
		points[i++] = vec2(x,y);
		w = w +c;					//inc line length
		x = x - w;					// x left
		points[i++] = vec2(x,y);
		y = y + w;					// y up
		points[i++] = vec2(x,y);
		w = w + c;					//inc line length
	}
	

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader_a2.glsl", "fshader_a2.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glDrawArrays( GL_LINE_STRIP, 0, NumPoints );    // draw the points
    glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
	switch( button ) {
	    case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
	    case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
	    case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
	}
    }
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 512, 512 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow( "Sierpinski Gasket" );
	glewExperimental = GL_TRUE;
    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}*/

//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "Angel.h"
#include "MyCube.h"

#pragma comment(lib, "freeglut")  //add libraries
#pragma comment(lib, "glew32")
typedef Angel::vec4  color4;
typedef Angel::vec4  cubepoint4;
typedef Angel::vec4  tetrapoint4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

cubepoint4 points[NumVertices];
color4 colors[NumVertices];
tetrapoint4 tetrapoints[NumVertices];

MyCube cube;

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

GLuint  theta;  // The location of the "theta" shader uniform variable

// Vertices of a unit cube centered at origin, sides aligned with axes
cubepoint4 vertices[8] = {
    cubepoint4( -0.5, -0.5,  0.5, 1.0 ),
    cubepoint4( -0.5,  0.5,  0.5, 1.0 ),
    cubepoint4(  0.5,  0.5,  0.5, 1.0 ),
    cubepoint4(  0.5, -0.5,  0.5, 1.0 ),
    cubepoint4( -0.5, -0.5, -0.5, 1.0 ),
    cubepoint4( -0.5,  0.5, -0.5, 1.0 ),
    cubepoint4(  0.5,  0.5, -0.5, 1.0 ),
    cubepoint4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA olors
color4 vertex_colors[8] = {
	color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

tetrapoint4 tetravertices[4] = {
	tetrapoint4 (0.0, 1.0, 0.0, 1.0), 
	tetrapoint4 (0.943, -0.333, 0.0, 1.0), 
	tetrapoint4 (-0.471, -0.333, 0.8165, 1.0), 
	tetrapoint4 (-0.471, -0.333, -0.8165, 1.0)
}; 

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

//----------------------------------------------------------------------------

// Tetra generates one triangle for each face and assigns colors
//    to the vertices
int TetraIndex = 0;
void tetra( int a, int b, int c, int d)
{
    colors[Index] = vertex_colors[a]; tetrapoints[Index] = tetravertices[a]; Index++;
	colors[Index] = vertex_colors[b]; tetrapoints[Index] = tetravertices[b]; Index++;
    colors[Index] = vertex_colors[c]; tetrapoints[Index] = tetravertices[c]; Index++;
	colors[Index] = vertex_colors[a]; tetrapoints[Index] = tetravertices[a]; Index++;
    colors[Index] = vertex_colors[c]; tetrapoints[Index] = tetravertices[c]; Index++;
    colors[Index] = vertex_colors[d]; tetrapoints[Index] = tetravertices[d]; Index++;
}

//----------------------------------------------------------------------------


// generate 12 triangles: 36 vertices and 36 colors
void GenerateCube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

// generate 4 triangles: 4 vertices 
void GenerateTetra()
{
    tetra( 0, 2, 1, 2 );
    tetra( 0, 1, 3, 6 );
    tetra( 0, 3, 2, 7 );
    tetra( 2, 1, 3, 2 );
}



//----------------------------------------------------------------------------


// OpenGL initialization
void
init()
{
	GenerateTetra();

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(tetrapoints) + sizeof(colors),
		  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(tetrapoints), tetrapoints );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(tetrapoints), sizeof(colors), colors );	

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

	theta = glGetUniformLocation( program, "theta" );
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glUniform3fv( theta, 1, Theta );
    
	glDrawArrays( GL_TRIANGLES, 0, NumVertices );

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
    }
}

//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
	switch( button ) {
	    case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
	    case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
	    case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
	}
    }
}

//----------------------------------------------------------------------------

void
idle( void )
{
    Theta[Axis] += 0.01;

    if ( Theta[Axis] > 360.0 ) {
	Theta[Axis] -= 360.0;
    }
    
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Color Cube" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );

    glutMainLoop();

    return 0;
}

