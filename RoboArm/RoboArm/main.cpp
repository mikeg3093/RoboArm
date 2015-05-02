/*	Michael Grayson	

	Project 5 - RoboArm

	This is an OpenGL Program that uses .obj files to draw and move a
	Robotic arm. Each portion of the robot is created as a model and then
	each portion is placed in the starting position. Each keyboard assignment
	corresponds to a movement on the robot and when pressed, the robot will
	move accordingly.
*/



#pragma comment(lib, "glew32")
#pragma comment(lib, "freeglut")
#include <fstream>
#include <iostream>
#include "Angel.h"
#include "Model.h"

using namespace std;


typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

GLfloat ThetaX = 10.0, ThetaY = 10.0,
		baseTheta = 0, arm1Theta = 0, arm2Theta = 0, arm3Theta = 0, clawTheta = 0;// initialize rotations

GLuint ambient_product_loc;// a quick and dirty hack for setting the ambient color for different objects
Model base;	//Model class obj
Model arm1;
Model arm2;
Model arm3;//Model class obj
Model claw1;
Model claw2;
GLint ctm_loc,r_loc;	//GLints for uniform variable locations in vshader		

void
init()
{
	//set up lighting values for ambient, diffuse etc
	// this should normally be in every object class and NOT here.  
	// We will add this to the objects when we read in the material files
	point4 light_position= vec4(1,1,0,0.0);
	color4 light_ambient =color4(.6,.6,.6,1.0);
	color4 material_ambient = color4(0.6,0.6,0.6,1.0);
	color4 ambient_product = product(light_ambient,material_ambient);

	color4 light_diffuse =color4(1.0,1.0,1.0,1.0);
	color4 material_diffuse = color4(1.0,1.0,1.0,1.0);
	color4 diffuse_product = product(light_diffuse,material_diffuse);

	color4 light_specular =color4(1.0,1.0,1.0,1.0);
	color4 material_specular = color4(1.0,1.0,0.0,1.0);
	color4 specular_product = product(light_specular,material_specular);
	float material_shininess = 100.0;

	mat4 projection = Ortho(-1.5,1.5,-1.5,1.5,-1.5,1.5);



    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

	ctm_loc = glGetUniformLocation(program,"ctm");
	r_loc = glGetUniformLocation(program,"R");

	int projection_loc = glGetUniformLocation(program,"Projection");
	glUniformMatrix4fv(projection_loc,1,GL_FALSE,projection);

	//set up lighting and materials
	//GLuint ambient_product_loc;
	ambient_product_loc = glGetUniformLocation(program,"AmbientProduct");
	glUniform4fv(ambient_product_loc,1,ambient_product);

	GLuint diffuse_product_loc;
	diffuse_product_loc = glGetUniformLocation(program,"DiffuseProduct");
	glUniform4fv(diffuse_product_loc,1,diffuse_product);

	GLuint specular_product_loc;
	specular_product_loc = glGetUniformLocation(program,"SpecularProduct");
	glUniform4fv(specular_product_loc,1,specular_product);
	// a shorter way
//	glUniform4fv(glGetUniformLocation(program,"LightPosition"),4,light_position);
	GLuint light_position_loc;
	light_position_loc = glGetUniformLocation(program,"LightPosition");
	glUniform4fv(light_position_loc,1,light_position);

    glUniform1f(glGetUniformLocation(program,"Shininess"),material_shininess);

	// Load the sphere we will use for the bodys
	// Note that the polygon count in ssphere.obj is
	// too high for this apps.  We are drawing small spheres
	// so are wasting a lot of time.  Its no big deal here
	// since our frame rate is high as well.  Way above 60
	// If we were drawing 1000's of bodys this would probably
	// be a problem so we would need to create a lower polygon
	// count sphere with blender and use it instead.
	// Note that I am loading only one copy of the sphere
	// We will just re-render it in different locations.

	base.Load(program,"base.obj");// a smooth sphere with normals
	arm1.Load(program,"arm1.obj");// a smooth sphere with normals
	arm2.Load(program,"arm2.obj");// a smooth sphere with normals
	arm3.Load(program,"arm3.obj");// a smooth sphere with normals
	claw1.Load(program,"claw1.obj");// a smooth sphere with normals
	claw2.Load(program,"claw2.obj");// a smooth sphere with normals
	
	
   glEnable( GL_DEPTH_TEST );
   glClearColor( 0.0, 0.0, 0.0, 1.0 );// A black background
	
	
}

int frame,fps,time,timebase=0;
void
display( void )
{ 
	mat4 ctm,R; // matrices for Position in vshade'
	glColor3f(1,0,0); 

	R = RotateX(ThetaX)*RotateY(ThetaY); // X and Y rotations of all objects based on keyboard input UP,DOWN,LFT,RIGHT
	glUniformMatrix4fv(r_loc,1,GL_TRUE,R);// Applying a rotation matrix in Position in vshader

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	ctm *= Translate(0,-1,0);										// BASE TRS
	ctm *= Scale(1,1,1);
	ctm *= RotateY(baseTheta);
	glUniformMatrix4fv(ctm_loc,1,GL_TRUE,ctm);
	glUniform4fv(ambient_product_loc,1,vec4(0.5,.5,0.5,1.0));
	base.Draw();

	ctm *= Translate(0,.17,0);										// ARM1 TRS
	ctm *= Scale(1,1,1);
	ctm *= RotateZ(arm1Theta);
	ctm *= Translate(0,-.17,0);
	glUniformMatrix4fv(ctm_loc,1,GL_TRUE,ctm);
	glUniform4fv(ambient_product_loc,1,vec4(0.5,.5,0.5,1.0));
	arm1.Draw();

	ctm *= Translate(0,.48,0);										// ARM2 TRS
	ctm *= Scale(1,1,1);
	ctm *= RotateZ(arm2Theta);
	ctm *= Translate(0,-.48,0);
	glUniformMatrix4fv(ctm_loc,1,GL_TRUE,ctm);
	glUniform4fv(ambient_product_loc,1,vec4(0.5,.5,0.5,1.0));
	arm2.Draw();

	ctm *= Translate(0,.733,0);										// ARM3 TRS
	ctm *= Scale(1,1,1);
	ctm *= RotateZ(arm3Theta);
	ctm *= Translate(0,-.733,0);
	glUniformMatrix4fv(ctm_loc,1,GL_TRUE,ctm);
	glUniform4fv(ambient_product_loc,1,vec4(0.5,.5,0.5,1.0));
	arm3.Draw();

	ctm *= Translate(-.064,1.08,0);									// CLAW1 TRS
	ctm *= Scale(1,1,1);
	ctm *= RotateX(clawTheta);
	ctm *= Translate(.064,-1.08,0);
	glUniformMatrix4fv(ctm_loc,1,GL_TRUE,ctm);
	glUniform4fv(ambient_product_loc,1,vec4(0.5,.5,0.5,1.0));
	claw1.Draw();

	ctm *= Translate(.064,1.08,0);									// CALW2 TRS
	ctm *= Scale(1,1,1);
	ctm *= RotateX(-clawTheta);
	ctm *= Translate(-.064,-1.08,0);
	glUniformMatrix4fv(ctm_loc,1,GL_TRUE,ctm);
	glUniform4fv(ambient_product_loc,1,vec4(0.5,.5,0.5,1.0));
	claw2.Draw();

	

	// Collect the frame rate and display	
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	char  display_string[100];                   
	if(time- timebase>1000){
		 fps = frame*1000.0/(time-timebase);
		 sprintf(display_string,"RoboArm  FPS:%d",fps);
		 glutSetWindowTitle(display_string);
		 timebase = time;
		 frame=0;
	}
    glutSwapBuffers();
}
void
keyboard( unsigned char key, int x, int y )
{// The polygon count of the spheres are high here so you might not notice the differenc
 // in LINE drawing!!
    switch( key ) {
	case 033: // Escape Key
	case 'r':glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);break;
	case 't':glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);break;

	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;

								//MOVEMENT FOR ROBOT
	case 'z': case 'Z':					
		baseTheta += 1;			//BASE
		break;
	case 'x': case 'X':
		baseTheta -= 1;
		break;

	case 'y': case 'Y':			//ARM1
		arm1Theta += 1;
		break;
	case 'u': case 'U':
		arm1Theta -= 1;
		break;

	case 'h': case 'H':			//ARM2
		arm2Theta += 1;
		break;
	case 'j': case 'J':
		arm2Theta -= 1;
		break;

	case 'n': case 'N':			//ARM3
		arm3Theta += 1;
		break;
	case 'm': case 'M':
		arm3Theta -= 1;
		break;

	case 'o': case 'O':			//CLAW
		clawTheta += 1;
		break;
	case 'p': case 'P':
		clawTheta -= 1;
		break;



    }
}


//	GLUT SpecialFunc for arrow keys input for
//	ThetaX Rotation and ThetaY Rotation
void keyboard2(int key, int x, int y){

	switch( key ) {
	case GLUT_KEY_UP:
		ThetaX += 2;
		break;

	case GLUT_KEY_DOWN:
		ThetaX -= 2;
		break;
	case GLUT_KEY_LEFT:
		ThetaY -= 2;
		break;
	case GLUT_KEY_RIGHT:
		ThetaY += 2;
		break;
	}
}

//----------------------------------------------------------------------------
// Nothing is being done here!!
void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
	switch( button ) {
	    case GLUT_LEFT_BUTTON:      break;
	    case GLUT_MIDDLE_BUTTON:   break;  
	    case GLUT_RIGHT_BUTTON:    break;
	}
    }
}

//----------------------------------------------------------------------------

//Call for redisplay
void
idle( void )
{  
    glutPostRedisplay();
}

//----------------------------------------------------------------------------
// Nothing is being done here!!
void processMenuEvents(int option) {

	switch (option) {
		case 1 : 
			 
		 break;
		case 2 :
		
		 break;
		
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
// Nothing is being done here!!
void createGLUTMenus() {

	int menu;

	// create the menu and
	// tell glut that "processMenuEvents" will 
	// handle the events
	//menu = glutCreateMenu(processMenuEvents);
	
	//add entries to our menu
//	glutAddMenuEntry("Sphere",1);
	//glutAddMenuEntry("Icosahedron",2);
	
	// attach the menu to the right button
	//glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 800, 800);
	glutInitWindowPosition( 512, 10 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
	createGLUTMenus();
    glutCreateWindow( "RoboArm" );
	glewExperimental = GL_TRUE;
    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutSpecialFunc(keyboard2);
    glutMouseFunc( mouse );
    glutIdleFunc( idle );

    glutMainLoop();

    return 0;
}