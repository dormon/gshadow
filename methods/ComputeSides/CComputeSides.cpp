#include"CComputeSides.hpp"

#include<sstream>

std::string CComputeSides::GenerateCompute(
    unsigned WorkGroupSize,
    unsigned MaxMultiplicity,
    bool CullSides){
	std::stringstream Result;
  Result<<"#version 430 core\n";
  Result<<"\n";
  //Result<<"#extension GL_AMD_gpu_shader_int64 : enable\n";
  Result<<"layout(local_size_x="<<WorkGroupSize<<",local_size_y=1,local_size_z=1)in;\n";
  Result<<"\n";
  Result<<"layout(std430,binding=0)buffer SInput{vec4 IBuffer[];};\n";
  Result<<"layout(std430,binding=1)buffer SOutput{vec4 OBuffer[];};\n";
  Result<<"layout(std430,binding=2)buffer SCounter{uint Counter[];};\n";
  Result<<"\n";
  Result<<"\n";
  Result<<"uniform uint NumEdge=0;\n";
  Result<<"uniform vec4 LightPosition;\n";
  Result<<"uniform mat4 mvp;\n";
  Result<<"\n";
  Result<<"int GreaterVec(vec3 a,vec3 b){\n";
  Result<<"	return int(dot(ivec3(sign(a-b)),ivec3(4,2,1)));\n";
  Result<<"}\n";
  Result<<"\n";
  Result<<"int ComputeMult(vec3 A,vec3 B,vec3 C,vec4 L){\n";
  Result<<"  vec3 n=cross(C-A,L.xyz-A*L.w);\n";
  Result<<"	return int(sign(dot(n,B-A)));\n";
  Result<<"}\n";
  Result<<"\n";
  if(CullSides){
    Result<<"bool IsEdgeVisible(in vec4 A,in vec4 B){\n";
    Result<<"	vec3 M=+A.xyz+A.www;\n";
    Result<<"	vec3 N=+B.xyz+B.www;\n";
    Result<<"	vec3 O=-A.xyz+A.www;\n";
    Result<<"	vec3 P=-B.xyz+B.www;\n";
    Result<<"	vec3 NM=N-M;\n";
    Result<<"	vec3 PO=P-O;\n";
    Result<<"	float Left=0;\n";
    Result<<"	float Right=1;\n";
    Result<<"	for(int i=0;i<3;++i){\n";
    Result<<"		if(NM[i]==0){\n";
    Result<<"			if(M[i]<0)return false;\n";
    Result<<"		}else{\n";
    Result<<"			if(NM[i]>0)Left=max(Left,-M[i]/NM[i]);\n";
    Result<<"			else Right=min(Right,-M[i]/NM[i]);\n";
    Result<<"		}\n";
    Result<<" 	                                           \n";
    Result<<"		if(PO[i]==0){\n";
    Result<<"			if(O[i]<0)return false;\n";
    Result<<"		}else{\n";
    Result<<"			if(PO[i]>0)Left=max(Left,-O[i]/PO[i]);\n";
    Result<<"			else Right=min(Right,-O[i]/PO[i]);\n";
    Result<<"		}\n";
    Result<<"	}\n";
    Result<<"	return Left<=Right;\n";
    Result<<"}\n";
    Result<<"\n";
    Result<<"bool IsFullVisible(in vec4 A,in vec4 B,in vec4 C,int Diag){\n";
    Result<<"  vec3 a=A.xyz;\n";
    Result<<"  vec3 b=B.xyz;\n";
    Result<<"  vec3 c=C.xyz;\n";
    Result<<"  if(Diag>=0){\n";
    Result<<"    a[Diag]=-a[Diag];\n";
    Result<<"    b[Diag]=-b[Diag];\n";
    Result<<"    c[Diag]=-c[Diag];\n";
    Result<<"  }\n";
    Result<<"	float m=(a.x-a.y);\n";
    Result<<"	float n=(b.x-b.y);\n";
    Result<<"	float o=(c.x-c.y);\n";
    Result<<"	float p=(a.x-a.z);\n";
    Result<<"	float q=(b.x-b.z);\n";
    Result<<"	float r=(c.x-c.z);\n";
    Result<<"  float d=(q*o-n*r);\n";
    Result<<"  float t=(m*r-p*o)/d;\n";
    Result<<"  float l=-(m*q-p*n)/d;\n";
    Result<<"	vec4 X=A+t*B+l*C;\n";
    Result<<"  return (t>0)&&(t<1)&&(l>0)&&(l<1)&&\n";
    Result<<"      all(greaterThan(X.xyz,-X.www))&&all(lessThan(X.xyz,X.www));\n";
    Result<<"}\n";
    Result<<"\n";
    Result<<"bool IsVisible(in vec4 a,in vec4 b,in vec4 c,in vec4 d,vec4 l){\n";
    Result<<"	vec4 A=mvp*a;\n";
    Result<<"	vec4 B=mvp*b;\n";
    Result<<"	vec4 C=mvp*c;\n";
    Result<<"	vec4 D=mvp*d;\n";
    Result<<"	vec3 n=(mvp*vec4(cross(b.xyz-a.xyz,l.xyz-a.xyz*l.w),0)).xyz;\n";
    Result<<"	ivec3 Corner=ivec3(1+sign(n))>>1;\n";
    Result<<"  if(Corner.z==1)Corner=ivec3(1)-Corner;\n";
    Result<<"	int Diag=Corner.x+(Corner.y<<1)-1;\n";
    Result<<"	if(IsFullVisible(A,B-A,C-A,Diag))return true;\n";
    Result<<"	if(IsEdgeVisible(A,B))return true;\n";
    Result<<"	if(IsEdgeVisible(A,C))return true;\n";
    Result<<"	if(IsEdgeVisible(B,D))return true;\n";
    Result<<"	if(IsEdgeVisible(C,D))return true;\n";
    Result<<"	return false;\n";
    Result<<"}\n";
    Result<<"\n";
    Result<<"bool IsVisible(in vec4 P[4],in int Diag){\n";
    Result<<"	if(IsFullVisible(P[0],P[1]-P[0],P[2]-P[0],Diag))return true;\n";
    Result<<"	if(IsEdgeVisible(P[0],P[1]))return true;\n";
    Result<<"	if(IsEdgeVisible(P[0],P[2]))return true;\n";
    Result<<"	if(IsEdgeVisible(P[1],P[3]))return true;\n";
    Result<<"	if(IsEdgeVisible(P[2],P[3]))return true;\n";
    Result<<"	return false;\n";
    Result<<"}\n";
  }
  Result<<"\n";
  Result<<"void main(){\n";
  //Result<<"uint64_t a;\n";
  Result<<"	uint gid=gl_GlobalInvocationID.x;\n";
  Result<<"	if(gid<NumEdge){//compute silhouette\n";
  Result<<"		vec4 P[4];\n";
  Result<<"		gid*="<<2+1+MaxMultiplicity<<";\n";
  Result<<"		\n";
  Result<<"		P[0]=IBuffer[gid+0];\n";
  Result<<"		P[1]=IBuffer[gid+1];\n";
  Result<<"		P[2]=vec4(P[0].xyz*LightPosition.w-LightPosition.xyz,0);\n";
  Result<<"		P[3]=vec4(P[1].xyz*LightPosition.w-LightPosition.xyz,0);\n";
  Result<<"\n";
  if(CullSides){
    Result<<"    vec4 ClipP[4];\n";
    Result<<"    for(int i=0;i<4;++i)\n";
    Result<<"      ClipP[i]=mvp*P[i];\n";
    Result<<"\n";
    Result<<"    vec3 Normal=(mvp*vec4(cross(\n";
    Result<<"        P[1].xyz-P[0].xyz,\n";
    Result<<"        LightPosition.xyz-P[0].xyz*LightPosition.w),0)).xyz;\n";
    Result<<"	  ivec3 Corner=ivec3(1+sign(Normal))>>1;\n";
    Result<<"    if(Corner.z==1);Corner=ivec3(1)-Corner;\n";
    Result<<"	  int Diag=Corner.x+(Corner.y<<1)-1;\n";
    Result<<"\n";
    Result<<"		if(!IsVisible(ClipP,Diag))return;\n";
  }

  Result<<"\n";
  Result<<"		precise int Multiplicity=0;\n";
  Result<<"		int Num=int(IBuffer[gid+2].x)+3;\n";
  Result<<"\n";
  Result<<"		for(int i=3;i<Num;++i){\n";
  Result<<"			#define T0 P[0].xyz\n";
  Result<<"			#define T1 P[1].xyz\n";
  Result<<"			#define T2 IBuffer[gid+i].xyz\n";
  Result<<"			if(GreaterVec(T0,T2)>0){//T[2] T[0] T[1]?\n";
  Result<<"				Multiplicity+=ComputeMult(T2,T0,T1,LightPosition);\n";
  Result<<"			}else{\n";
  Result<<"				if(GreaterVec(T1,T2)>0){//T[0] T[2] T[1]?\n";
  Result<<"					Multiplicity-=ComputeMult(T0,T2,T1,LightPosition);\n";
  Result<<"				}else{//T[0] T[1] T[2]?\n";
  Result<<"					Multiplicity+=ComputeMult(T0,T1,T2,LightPosition);\n";
  Result<<"				}\n";
  Result<<"			}\n";
  Result<<"		}\n";
  Result<<"		if(Multiplicity>0){\n";
  Result<<"			uint WH=atomicAdd(Counter[0],4*Multiplicity);\n";
  Result<<"     for(int m=0;m<Multiplicity;++m){\n";
  Result<<"			  OBuffer[WH++]=P[1];\n";
  Result<<"			  OBuffer[WH++]=P[0];\n";
  Result<<"			  OBuffer[WH++]=P[3];\n";
  Result<<"			  OBuffer[WH++]=P[2];\n";
  Result<<"     }\n";
  Result<<"		}\n";
  Result<<"		if(Multiplicity<0){\n";
  Result<<"			Multiplicity=-Multiplicity;\n";
  Result<<"			uint WH=atomicAdd(Counter[0],4*Multiplicity);\n";
  Result<<"     for(int m=0;m<Multiplicity;++m){\n";
  Result<<"			  OBuffer[WH++]=P[0];\n";
  Result<<"			  OBuffer[WH++]=P[1];\n";
  Result<<"			  OBuffer[WH++]=P[2];\n";
  Result<<"			  OBuffer[WH++]=P[3];\n";
  Result<<"     }\n";
  Result<<"		}\n";
  Result<<"	}\n";
  Result<<"}\n";
  return Result.str();
}

CComputeSides::CComputeSides(SAdjecency*Adjacency,
    unsigned WorkGroupSize,bool CullSides){

  //std::cerr<<"################################## new ComputeSides sides "<<std::endl;
  this->Adjacency=Adjacency;
  unsigned NumVer=2+1+this->Adjacency->MaxOpposite;

  //input
  this->Input=new ge::gl::BufferObject(
      sizeof(float)*4*NumVer*this->Adjacency->NumEdges,
      NULL,GL_STATIC_DRAW);

  float*Ptr=(float*)this->Input->map();
  for(unsigned e=0;e<this->Adjacency->NumEdges;++e){
    //A
    for(int k=0;k<3;++k)
      Ptr[(e*NumVer+0)*4+k]=this->Adjacency->Points[this->Adjacency->Edges[e][0]*3+k];
    Ptr[(e*NumVer+0)*4+3]=1;

    //B
    for(int k=0;k<3;++k)
      Ptr[(e*NumVer+1)*4+k]=this->Adjacency->Points[this->Adjacency->Edges[e][1]*3+k];
    Ptr[(e*NumVer+1)*4+3]=1;

    //N
    Ptr[(e*NumVer+2)*4+0]=this->Adjacency->EdgeOppositeNum[e];
    for(int k=1;k<4;++k)
      Ptr[(e*NumVer+2)*4+k]=0;

    unsigned NumCap=0;
    for(unsigned o=0;o<this->Adjacency->EdgeOppositeNum[e];++o){
      if(	this->Adjacency->EdgeOpposite[e][o]>this->Adjacency->Edges[e][0]&&
          this->Adjacency->EdgeOpposite[e][o]>this->Adjacency->Edges[e][1]){
        for(int k=0;k<3;++k)
          Ptr[(e*NumVer+3+NumCap)*4+k]=
            this->Adjacency->Points[this->Adjacency->EdgeOpposite[e][o]*3+k];
        Ptr[(e*NumVer+3+NumCap)*4+3]=1;
        NumCap++;
      }
    }
    Ptr[(e*NumVer+2)*4+1]=NumCap;
    for(unsigned o=0;o<this->Adjacency->EdgeOppositeNum[e];++o){
      if(!(	this->Adjacency->EdgeOpposite[e][o]>this->Adjacency->Edges[e][0]&&
            this->Adjacency->EdgeOpposite[e][o]>this->Adjacency->Edges[e][1])){
        for(int k=0;k<3;++k)
          Ptr[(e*NumVer+3+NumCap)*4+k]=
            this->Adjacency->Points[this->Adjacency->EdgeOpposite[e][o]*3+k];
        Ptr[(e*NumVer+3+NumCap)*4+3]=0;
        NumCap++;
      }
    }
    for(unsigned o=NumCap;o<this->Adjacency->MaxOpposite;++o)
      for(int k=0;k<4;++k)
        Ptr[(e*NumVer+3+o)*4+k]=0;
  }
  this->Input->unmap();
  //std::cerr<<"aftermap"<<std::endl;
  //output
  this->Output=new ge::gl::BufferObject(
      sizeof(float)*4*4*
        this->Adjacency->NumEdges*this->Adjacency->MaxOpposite,
      NULL,GL_DYNAMIC_COPY);
  this->Output->clear(GL_R32F,GL_RED,GL_FLOAT);
  //counter
  this->Counter=new ge::gl::BufferObject(sizeof(unsigned)*4);



  unsigned*CPtr=(unsigned*)this->Counter->map();
  CPtr[0]=0;
  CPtr[1]=1;
  CPtr[2]=0;
  CPtr[3]=0;
  this->Counter->unmap();

  //vao
  this->VAO=new ge::gl::VertexArrayObject();
  this->VAO->bind();
  this->Output->bind(GL_ARRAY_BUFFER);
  this->Counter->bind(GL_DRAW_INDIRECT_BUFFER);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,
      sizeof(float)*4,(GLvoid*)(sizeof(float)*0));
  this->VAO->unbind();

  //std::cerr<<"after vao"<<std::endl;
  //programs
  ge::gl::ShaderObject*ComputeShader=new ge::gl::ShaderObject(
      GL_COMPUTE_SHADER,this->GenerateCompute(
        WorkGroupSize,
        this->Adjacency->MaxOpposite,
        false+0*CullSides));
  this->ComputeProgram=new ge::gl::ProgramObject(1,ComputeShader);
  //delete ComputeShader;

  this->DrawProgram=new ge::gl::ProgramObject(
      ShaderDir+"methods/ComputeSidesSOE/draw.vp",
      ShaderDir+"methods/ComputeSidesSOE/draw.cp",
      ShaderDir+"methods/ComputeSidesSOE/draw.ep",
      ShaderDir+"methods/ComputeSidesSOE/draw.fp");
  //std::cerr<<"after program"<<std::endl;

  //if(glIsBuffer(this->Counter->getId())==GL_FALSE){
  //  std::cerr<<"neni buffer!!!!!!!!"<<std::endl;
  //}


}

CComputeSides::~CComputeSides(){
  //std::cerr<<"DESTRUCTOR COMPUTE"<<std::endl;
  //std::cerr<<"################################## delete TessellationSides sides "<<std::endl;

  this->Input->unbindBase(GL_SHADER_STORAGE_BUFFER,0);
  this->Output->unbindBase(GL_SHADER_STORAGE_BUFFER,1);
  this->Counter->unbindBase(GL_SHADER_STORAGE_BUFFER,2);

  delete this->VAO;
  delete this->Input;
  delete this->Output;
  delete this->Counter;
  delete this->ComputeProgram;
  delete this->DrawProgram;
}

void CComputeSides::ComputeSides(float*mvp,simulation::Light*Light){
  this->Counter->clear(GL_R32UI,0,sizeof(unsigned),GL_RED_INTEGER,GL_UNSIGNED_INT);

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  this->ComputeProgram->use();
  this->ComputeProgram->set("NumEdge",this->Adjacency->NumEdges);

  this->ComputeProgram->set("LightPosition",1,glm::value_ptr(Light->position));
  this->ComputeProgram->set("mvp",1,GL_FALSE,mvp);

  this->Input->bindRange(GL_SHADER_STORAGE_BUFFER,0,0,
      sizeof(float)*4*(2+1+this->Adjacency->MaxOpposite)*this->Adjacency->NumEdges);
  this->Output->bindRange(GL_SHADER_STORAGE_BUFFER,1,0,
      sizeof(float)*4*4*(this->Adjacency->MaxOpposite*this->Adjacency->NumEdges));
  this->Counter->bindRange(GL_SHADER_STORAGE_BUFFER,2,0,
      sizeof(unsigned)*4);
  
  unsigned WorkSize=
      (this->Adjacency->NumEdges/this->ComputeProgram->workGroupSize[0])+
      ((this->Adjacency->NumEdges%this->ComputeProgram->workGroupSize[0])?1:0);
  WorkSize=this->Adjacency->NumEdges/64+1;

  glDispatchCompute(WorkSize,1,1);
  
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void CComputeSides::DrawSides(float*mvp,simulation::Light*Light){
  this->DrawProgram->use();
  this->DrawProgram->set("mvp",1,GL_FALSE,mvp);
  this->VAO->bind();
  this->Counter->bind(GL_DRAW_INDIRECT_BUFFER);
  glPatchParameteri(GL_PATCH_VERTICES,4);
  glDrawArraysIndirect(GL_PATCHES,NULL);
  this->VAO->unbind();
}

