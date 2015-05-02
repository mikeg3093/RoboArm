#version 150
#pragma debug(on)
in vec3 vNormal;
in  vec4 vPosition;    
out vec4 color;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ctm;// current transformation matrix
uniform mat4 R;
uniform vec4 LightPosition;
uniform float Shininess;
uniform mat4 Projection;

void main() 
{
    
   vec3 Normal;
   vec4 ambient, diffuse, specular;
   vec4 Position =Projection*R*ctm * vPosition;
   vec4 NN=ctm*vec4(vNormal.xyz,0);
   vec3 N = normalize(NN.xyz);
   vec3 L= normalize(LightPosition.xyz - (ctm*vPosition).xyz);
   vec3 E=-normalize((ctm*vPosition).xyz);
   vec3 H = normalize(L+E);
   float Kd = max(dot(L,N),0.0);
   float Ks = pow(max(dot(N,H),0.0),Shininess);
   ambient = AmbientProduct;
   diffuse = Kd*DiffuseProduct;
   specular = max(pow(max(dot(N,H),0.0),Shininess)*SpecularProduct,0.0);
   //output testing
 //  color = vec4(vNormal.xyz,1);// to test with
   //color = vPosition;
    //color = vec4( normalize(vPosition.xyz),1);
   color = vec4 ((ambient + diffuse + specular).xyz,1.0);
  //color =vec4( N,1);
	
   gl_Position =Position;
	
	
} 
