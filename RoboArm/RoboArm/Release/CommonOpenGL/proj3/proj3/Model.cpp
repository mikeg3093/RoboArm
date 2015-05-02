#include "Model.h"


Model::Model(void)
{
	
}


Model::~Model(void)
{
}

void Model::Load(char* filename)
{
	NumVertices = 0;
	NumIndices = 0;

	float v1 = 0;
	float v2 = 0;
	float v3 = 0;
	string vstr = "";
	string fstr = "";
	string vstring = "";
	string fstring = "";

	ifstream infile;
	infile.open(filename);
	string line="";	
			
	while(getline(infile, line)){ 

		if(line.front() == 'v'){

			for(int i = 2; i < line.length(); i++){				
				
				vstring.push_back(line[i]);
				if(i + 1 == line.length()){
						vstring.push_back(' ');	
						NumVertices++;
				}
			}
		}
	
		if(line.front() == 'f'){
			
			for(int i = 2; i < line.length(); i++){	
				
				fstring.push_back(line[i]);	
				if(i + 1 == line.length()){
						fstring.push_back(' ');
						NumIndices+=3;	
				}
			}
		}
	}	

	Vertices = new vec4 [NumVertices];
	int count = 0;
	int i = 0;
	for(std::string::iterator it = vstring.begin(); it != vstring.end(); ++it) {
		
		if(*it != ' ')
			vstr.push_back(*it);
		else if(count%3 == 0){
			v1 = stof(vstr);
			cout << v1 << " ";
			vstr = "";
			count++;
		}
		else if(count%3 == 1){
			v2 = stof(vstr);
			cout << v2 << " ";
			vstr = "";
			count++;
		}
		else if(count%3 == 2){
			v3 = stof(vstr);
			cout << v3 << endl;
			vstr = "";
			Vertices[i] = vec4(v1,v2,v3,1);
			count++;
			i++;
		}
	}
	

	Indices = new GLuint [NumIndices];
	i = 0;
	for(std::string::iterator it = fstring.begin(); it != fstring.end(); ++it) {
		
		if(*it != ' ')
			fstr.push_back(*it);
		else{
			Indices[i] = stoi(fstr) - 1;
			cout << fstr << endl;
			fstr = "";
			i++;
		}
	}

	cout << NumVertices << endl;
	cout << NumIndices << endl;

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
	glBindVertexArray( 0 );
}


bool Model::Draw(void)
{
	glBindVertexArray( vao );
	glDrawElements( GL_TRIANGLES,NumIndices, GL_UNSIGNED_INT , BUFFER_OFFSET(0));
	glBindVertexArray( 0 );
	return false;
}
