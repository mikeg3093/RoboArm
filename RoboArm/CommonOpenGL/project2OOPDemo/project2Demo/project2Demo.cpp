//	Michael Grayson
//
//	CMPS 3233 Graphics - Project 2
//
//	The following code uses example 4 in the text book to generate a cube.
//	It has been updated by Dr. Simpson to also make the cube diplayed in 
//	wireframe. I have added a Tetra class and a flag to
//  switch between both the cube and tetrahedron. To switch, press 's' during 
//	run time. 
//
// The demo uses parts of append_example4 to build the app 
// using objects for a cube
// 

#include "Angel.h"
#include "Cube.h"
#include "Tetra.h"
// These comments are added to the original example 4
#pragma comment(lib,"glew32")
#pragma comment(lib,"freeglut")


const int NumVertices = 8; 
const int NumIndices= 36;
bool flag = true;		// flag to change between drawing a cube/tetra

Cube MyCube;			// class for cube 
Tetra MyTetra;			//class for tetra


// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

GLuint  theta;  // The location of the "theta" shader uniform variable


// OpenGL initialization
void
init()
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE) ;

	MyCube.Generate();
	MyCube.Load();

	MyTetra.Generate();
	MyTetra.Load();

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader_a4.glsl", "fshader_a4.glsl" );
    glUseProgram( program );


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
   
	if(flag == true)			// if 's' key has been pressed
			MyCube.Draw();
	else
			MyTetra.Draw();

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033: // Escape Key
	case 's':
		if(flag == true)		// flag for changing the draw/display
			flag = false;
		else
			flag = true;
		break;
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
    glutInitWindowSize( 1024, 1024 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Color Cube" );
	glewExperimental = GL_TRUE;
    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );

    glutMainLoop();
    return 0;
}
