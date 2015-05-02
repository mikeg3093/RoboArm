#include "Model.h"
#include <vector>
#include <sstream>

Model::Model(void)
{
}


Model::~Model(void)
{
}


bool Model::Load(GLuint program, char * file_name)
{
	// Open the file and make sure it is there
	
	ifstream fin;
	bool testing=true;

	fin.open(file_name,ios::in);
	if (!fin) {
		cerr << "Can't open input file " << file_name << endl;
		exit(1);
	};
	string command="";
	GLfloat x,y,z;
	string line,cmd;
	bool HasPositions= true;
	bool HasNormals = false;
	vector<vec4> vertexList,normalList,newVertexList,newNormalList;
	
	vector<unsigned int> NormalIndexList;
	vector<unsigned int> VertexIndexList;
	vector<unsigned int> newIndexList;
	int i,v1,v2,v3,n1,n2,n3 ;// the vertices and associated normals for a face

	// The following code reads the .obj file and builds a list of the vertices and normals together
	// with two arrays showing a vertex / normal index paring in two arrays VertexIndexList and 
	// NormalIndexList.  These come directly from the face data.  NOTE: this is not what we send
	// to the gpu!!  
	while (!fin.eof()){
		istringstream ss;
		getline(fin,line);
		if(line.substr(0,2)=="v "){// get the vertices 
			//cout<<line<<endl;
			ss.str(line);
			ss>>cmd>> x >> y >> z;
		//	cout<<x<<" "<<y<<" "<<z<<endl;
			vertexList.push_back(vec4(x,y,z,1));
		}
		NumVertices = vertexList.size();
		if(line.substr(0,2)=="vn"){// get the vertices.
			HasNormals=true;
			//cout<<line<<endl;
			ss.str(line);
			ss>>cmd>> x>>y>>z;
			//cout<<x<<" "<<y<<" "<<z<<endl;
			normalList.push_back(vec4(x,y,z,1));
		}
		
		if(line.substr(0,2)=="f "){// get the vertices
			//	cout<<line<<endl;
			for(i=0;i<line.length()-1;i++){
				if (!isdigit(line[i])) line[i]=' ';// replace the f and slashes with blanks
			//	cout << line[i];
			}
			ss.str(line);
			ss>> v1 >> n1 >> v2 >> n2 >> v3 >>n3;
			VertexIndexList.push_back(v1);NormalIndexList.push_back(n1);
			VertexIndexList.push_back(v2);NormalIndexList.push_back(n2);
			VertexIndexList.push_back(v3);NormalIndexList.push_back(n3);
		}
	}
	int NumVerticesInFile = vertexList.size();
	int NumNormalsInFile = normalList.size();
	int SizeIndexList= VertexIndexList.size();


	cout<< "Number of vertices listed in file is "<<NumVerticesInFile<<endl;
	cout<<"Number of normals listed in fileis "<<NumNormalsInFile<<endl;
	
	// We now must build the actual vertex array , normal array and index arrays.  
	// The normal array and vertex array must match up, ie each vertex goes with the
	// adjacent normal in the normal array.  Since vertices may have several normals
	// associated with the same vertex we need to duplicate these.  In the case of
	// a flat cube with six normals and 8 vertices we will end up with 24 vertices
	// together with 24 normals.  Of course we dont know this so we will build a
	// list that has no dupulicate vertex-normal pairs.
	// This pairs array is a support DS that helps up remember whether or not a pair
	// has been already used
     int ** pairs= new int*[NumVerticesInFile];
	 for(int p=0;p<NumVerticesInFile;p++)pairs[p]=new int[NumNormalsInFile];
	 // The -1 means that this pair v,n has not been seen so far
	 // When a pair is seen the index is stored in this array at 
	 // location pairs[v][n]. Note that indexing starts a 1 not 0!!!!
	 for(int v=0;v<NumVerticesInFile;v++)
		 for(int n=0;n<NumNormalsInFile;n++)
			 pairs[v][n]=-1;
	  //Now lets go thru the vertex-normal index pairs and build our
	 // new list.
	 int v_idx,n_idx, index=0;
	 for (int idx=0;idx<SizeIndexList;idx++){
		 v_idx= VertexIndexList[idx]-1;// Convert to 0 indexing 
		 n_idx= NormalIndexList[idx]-1;
		 if (pairs[v_idx][n_idx]==-1){ // we not seen this pairing
			 // insert in new undup list
			 newVertexList.push_back(vertexList[v_idx]);
			 newNormalList.push_back(normalList[n_idx]);
			 newIndexList.push_back(newVertexList.size()-1);// insert new index which is end of list
			 pairs[v_idx][n_idx]=newVertexList.size()-1;
		 }else{// We have seen this pairing
			 // so use the existing index taken from the table
           newIndexList.push_back(pairs[v_idx][n_idx]);
		 }
	 }
	  NumIndices = newIndexList.size();
	  NumVertices= newVertexList.size();
      NumNormals = newNormalList.size();
	 // We have now built the lists so we need to change them to real arrays so that we can move them
	 // to the gpu
	 Vertices = new vec4[NumVertices];
	 Normals = new vec4[NumNormals]; 
	 Indices = new unsigned int[NumIndices];

	 for(int i=0;i<NumVertices;i++){
		   Vertices[i]=newVertexList[i];
		   Normals[i]=newNormalList[i];
	 }
	 for(int i=0;i<NumIndices;i++)
		   Indices[i]=newIndexList[i];

    // Now we need to move Vertices, Normals and Indices over to the GPU
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
    
	// index buffer 
	GLuint buffer;
	glGenBuffers(1,&buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer);
	// The following command moves the Indices over to the GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumIndices*sizeof(GLuint),Indices,GL_STATIC_DRAW);

	//vertex array
	if(HasPositions){
	    GLint position_loc = glGetAttribLocation(program,"vPosition");
		glGenBuffers(1,&buffer);
		glBindBuffer(GL_ARRAY_BUFFER,buffer);
		glBufferData(GL_ARRAY_BUFFER,NumVertices*4*sizeof(float),Vertices,GL_STATIC_DRAW);
		glEnableVertexAttribArray(position_loc);
		glVertexAttribPointer(position_loc,4,GL_FLOAT,0,0,0);
	}else cout <<"No vertices found "<<endl;

	//vertex array
	if(HasNormals){
	    GLint normal_loc = glGetAttribLocation(program,"vNormal");
		glGenBuffers(1,&buffer);
		glBindBuffer(GL_ARRAY_BUFFER,buffer);
		glBufferData(GL_ARRAY_BUFFER,NumNormals*4*sizeof(float),Normals,GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(normal_loc);
	    glVertexAttribPointer(normal_loc,4,GL_FLOAT,0,0,0);
	}else cout <<"No Normals found "<<endl;
if (testing) 
	// Dump the arrays for testing
	/*    for(i=0;i<NumVertices;i++)
		cout<< "V= "<<Vertices[i].x<<" "<<Vertices[i].y<<" "<<Vertices[i].z<<
			"  N="<<Normals[i].x<<" "<<Normals[i].y<<" "<<Normals[i].z<<endl;
		for(i=0;i<NumIndices;i++)
			cout<<Indices[i]<<" ";
		cout<<endl;*/

	// unbind the buffers and vao
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	return false;
}


void Model::Draw(void)
{
	glBindVertexArray( vao );
    glDrawElements( GL_TRIANGLES , NumIndices, GL_UNSIGNED_INT , BUFFER_OFFSET(0));
	glBindVertexArray( 0 );
	return;
}
