#include"CGenerateVertexShaderSilhouette.hpp"

#include<sstream>

std::string GenVSSilLayoutLocation(bool Generate,unsigned Location){
	std::stringstream Result;
	Result<<"";//empty layout
	if(Generate)Result<<"layout(location="<<Location<<")";//generate layout
	return Result.str();//return layout
}

std::string GenVSSilVertexAttribInputs(unsigned NumAttrib,SVSSilTemplate T){
	std::stringstream Result;
	//vertex attrib arrays are available in versions 150 and newer
	bool InputArrayAvailable=(T.Version>=150)&&T.UseVertexAttribArray;
	//vertex attrib input layouts are available in verisions 330 and newer
	bool InputLayoutAvailable=(T.Version>=330)&&T.UseLayouts;
	std::string Input="in";//input qualifier
	if(T.Version<=120)Input="attribute";//input qualifier in older versions
	if(InputArrayAvailable){//are vertex attrib arrays available?
		Result<<GenVSSilLayoutLocation(InputLayoutAvailable,0);//generate layouts
		Result<<Input<<" vec4 "<<T.VertexAttribName<<"["<<NumAttrib<<"];\n";//generate vertex attrib
	}else
		for(unsigned a=0;a<NumAttrib;++a){//loop over attributes
			Result<<GenVSSilLayoutLocation(InputLayoutAvailable,a);//generate layout
			Result<<Input<<" vec4 "<<T.VertexAttribName<<a<<";\n";
		}
	return Result.str();
}

std::string GenVSSilUniforms(SVSSilTemplate T){
	std::stringstream Result;//result
	Result<<"uniform vec4 "<<T.LightPositionUniformName<<";\n";//gen light position uniform
	if(T.MatrixUniformName!=""&&T.MatrixUniformName!="gl_ModelViewProjectionMatrix")
		Result<<"uniform mat4 "<<T.MatrixUniformName<<";\n";//gen mvp matrix uniform
	return Result.str();//return result
}

std::string GenFunction(){
	std::stringstream Result;//result
	Result<<"int GreaterVec(vec3 a,vec3 b){\n";
	Result<<"  return int(dot(sign(a-b),vec3(4,2,1)));\n";
	Result<<"}\n";
	Result<<"int ComputeMult(vec3 A,vec3 B,vec3 C,vec4 L){\n";
	Result<<"  vec3 n=cross(C-A,L.xyz-A*L.w);\n";
	Result<<"  return int(sign(dot(n,B-A)));\n";
	Result<<"}\n";
	return Result.str();//return result
}

std::string GenVSSilHullIDs(unsigned Num,SVSSilTemplate T){
	std::stringstream Result;//result
	if(T.Universal)Result<<"  int Num=int("<<T.VertexAttribName<<"0.w);\n";
	Result<<"  int SideID=int("<<T.VertexAttribName<<"1.w)/(4*4);\n";
	Result<<"  int VertexIDCCW=int("<<T.VertexAttribName<<"1.w)%4;\n";
	Result<<"  int VertexIDCW=(int("<<T.VertexAttribName<<"1.w)/4)%4;\n";
	return Result.str();//return result
}

std::string GenVSSilVertices(SVSSilTemplate T){
	std::stringstream Result;//result
	Result<<"  vec4 P[4];\n";
	Result<<"  P[0]=vec4("<<T.VertexAttribName<<"0.xyz,1);\n";
	Result<<"  P[1]=vec4("<<T.VertexAttribName<<"1.xyz,1);\n";
	Result<<"  P[2]=vec4(P[0].xyz*"<<T.LightPositionUniformName<<".w-"<<
		T.LightPositionUniformName<<".xyz,0);\n";
	Result<<"  P[3]=vec4(P[1].xyz*"<<T.LightPositionUniformName<<".w-"<<
		T.LightPositionUniformName<<".xyz,0);\n";
	return Result.str();//return result
}

std::string GenVSSilOppositeVertices(unsigned Num,SVSSilTemplate T){
	std::stringstream Result;//result
	Result<<"  vec4 O["<<Num<<"];\n";
	for(unsigned o=0;o<Num;++o)
		Result<<"  O["<<o<<"]="<<T.VertexAttribName<<o+2<<";\n";
	return Result.str();//return result
}

std::string GenVSSilCompatibility(SVSSilTemplate T){
	if(T.UseCompatibility||(T.Version>=130&&
				(T.MatrixUniformName==""||T.MatrixUniformName=="gl_ModelViewProjectionMatrix"))){
		return" compatibility";
	}
	return"";
}

std::string GenVSSilVertexHull(unsigned Num,SVSSilTemplate T){
	std::stringstream Result;
	Result<<"#version "<<T.Version<<GenVSSilCompatibility(T)<<"\n";//generate version
	Result<<"\n";
	Result<<GenVSSilVertexAttribInputs(Num+2,T);//generate vertex attribs
	Result<<"\n";
	Result<<GenVSSilUniforms(T);//generate uniforms
	Result<<"\n";
	Result<<GenFunction();//generate functions
	Result<<"\n";
	Result<<"void main(){\n";
	Result<<GenVSSilHullIDs(Num,T);//generate IDs of Sides Vertices,...
	Result<<"\n";
	Result<<GenVSSilVertices(T);//generate vertices of quad
	Result<<"\n";
	//vertex attrib arrays are available in versions 150 and newer
	bool InputArrayAvailable=(T.Version>=150)&&T.UseVertexAttribArray;
	if(!InputArrayAvailable){
		Result<<GenVSSilOppositeVertices(Num,T);
		Result<<"\n";
	};
	Result<<"  int Multiplicity=0;\n";
	if(T.Universal)Result<<"  for(int i=0;i<Num;++i){\n";
	else Result<<"  for(int i=0;i<"<<Num<<";++i){\n";
	Result<<"#define T0 P[0].xyz\n";
	Result<<"#define T1 P[1].xyz\n";
	if(InputArrayAvailable)Result<<"#define T2 "<<T.VertexAttribName<<"[2+i].xyz\n";
	else Result<<"#define T2 O[i].xyz\n";
	Result<<"    if(GreaterVec(T0,T2)>0){//T[2] T[0] T[1]?\n";
	Result<<"      Multiplicity+=ComputeMult(T2,T0,T1,LightPosition);\n";
	Result<<"    }else{\n";
	Result<<"      if(GreaterVec(T1,T2)>0){//T[0] T[2] T[1]?\n";
	Result<<"        Multiplicity-=ComputeMult(T0,T2,T1,LightPosition);\n";
	Result<<"      }else{//T[0] T[1] T[2]?\n";
	Result<<"        Multiplicity+=ComputeMult(T0,T1,T2,LightPosition);\n";
	Result<<"      }\n";
	Result<<"    }\n";
	Result<<"  }\n";//for end
	Result<<"\n";
	std::string Matrix="gl_ModelViewProjectionMatrix";
	if(T.MatrixUniformName!=""&&T.MatrixUniformName!="gl_ModelViewProjectionMatrix")
		Matrix=T.MatrixUniformName;
	Result<<"  if(SideID<int(abs(float(Multiplicity)))){\n";
	Result<<"    if(Multiplicity>0)\n";
	Result<<"      gl_Position="<<Matrix<<"*P[VertexIDCCW];\n";
	Result<<"    if(Multiplicity<0)\n";
	Result<<"      gl_Position="<<Matrix<<"*P[VertexIDCW];\n";
	Result<<"  }else\n";
	Result<<"    gl_Position=vec4(0,0,0,0);\n";
	Result<<"}\n";//main end
	return Result.str();
}

std::string GenVSSilFragmentHull(SVSSilTemplate T){
	std::stringstream Result;
	Result<<"#version "<<T.Version<<GenVSSilCompatibility(T)<<"\n";//generate version
	Result<<"\n";
	bool FragmentOutputLayoutAvailable=false;
	if(T.Version>=330)FragmentOutputLayoutAvailable=true;
	std::string OutputVariable="gl_FragColor";
	if(T.Version>120){
		OutputVariable="fColor";
		Result<<GenVSSilLayoutLocation(FragmentOutputLayoutAvailable,0);
		Result<<"out vec4 "<<OutputVariable<<";\n";
	}
	Result<<"void main(){\n";
	Result<<"  "<<OutputVariable<<"=vec4(1,0,1,1);\n";
	Result<<"}\n";//main end
	return Result.str();
}

std::string GenVSSilVertexCap(SVSSilTemplate T){
	std::stringstream Result;
	Result<<"#version "<<T.Version<<GenVSSilCompatibility(T)<<"\n";//generate version
	Result<<"\n";
	Result<<GenVSSilVertexAttribInputs(3,T);
	Result<<"\n";
	Result<<GenVSSilUniforms(T);//generate uniforms
	Result<<"\n";
	Result<<GenFunction();//generate functions
	Result<<"void Swap(inout vec4 A,inout vec4 B){vec4 C=A;A=B;B=C;}\n";
	Result<<"\n";

	std::string Matrix="gl_ModelViewProjectionMatrix";
	if(T.MatrixUniformName!=""&&T.MatrixUniformName!="gl_ModelViewProjectionMatrix")
		Matrix=T.MatrixUniformName;

	Result<<"void main(){\n";
	Result<<"  vec4 A="<<T.VertexAttribName<<"0;\n";
	Result<<"  vec4 B="<<T.VertexAttribName<<"1;\n";
	Result<<"  vec4 C="<<T.VertexAttribName<<"2;\n";
	Result<<"  if(GreaterVec(A.xyz,B.xyz)>0)Swap(A,B);\n";
	Result<<"  if(GreaterVec(B.xyz,C.xyz)>0)Swap(B,C);\n";
	Result<<"  if(GreaterVec(A.xyz,B.xyz)>0)Swap(A,B);\n";
	Result<<"  int Multiplicity=ComputeMult(A.xyz,B.xyz,C.xyz,LightPosition);\n";
	Result<<"  if(Multiplicity==0){gl_Position=vec4(0,0,0,1);return;}\n";
	Result<<"  if(Multiplicity<0)Swap(A,B);\n";

	Result<<"  int VertexID=int("<<T.VertexAttribName<<"0.w);\n";
	Result<<"  int VID=VertexID%3;\n";
	Result<<"  float FarCap=float(VertexID>2);\n";
	/*Result<<"  vec3 N=cross("<<T.VertexAttribName<<"1.xyz-"<<T.VertexAttribName
		<<"0.xyz,"<<T.VertexAttribName<<"2.xyz-"<<T.VertexAttribName<<"0.xyz);\n";
	Result<<"  int Swap=(int(-sign(dot(vec4(N,-dot(N,"<<T.VertexAttribName<<"0.xyz)),"
		<<T.LightPositionUniformName<<")))+1)/2;\n";
	Result<<"  VID=Swap*(2-VID)+(1-Swap)*VID;\n";
	Result<<"  vec4 P="<<T.VertexAttribName<<"0*float(VID==0)+"<<T.VertexAttribName
		<<"1*float(VID==1)+"<<T.VertexAttribName<<"2*float(VID==2);\n";
	*/
	//Result<<"  int Swap=(1-Multiplicity)/2;\n";
	//Result<<"  VID=Swap*(2-VID)+(1-Swap)*VID;\n";
	Result<<"  vec4 P=A*float(VID==0)+B*float(VID==1)+C*float(VID==2);\n";
	
	Result<<"  P.w=1.0;\n";
	Result<<"  P=P*(1.0-FarCap)+vec4(P.xyz*"<<T.LightPositionUniformName
		<<".w-"<<T.LightPositionUniformName<<".xyz,0)*FarCap;\n";
	Result<<"  gl_Position="<<Matrix<<"*P;\n";
	Result<<"}\n";//main end
	return Result.str();
}

std::string GenVSSilFragmentCap(SVSSilTemplate T){
	std::stringstream Result;
	Result<<"#version "<<T.Version<<GenVSSilCompatibility(T)<<"\n";//generate version
	Result<<"\n";
	bool FragmentOutputLayoutAvailable=false;
	if(T.Version>=330)FragmentOutputLayoutAvailable=true;
	std::string OutputVariable="gl_FragColor";
	if(T.Version>120){
		OutputVariable="fColor";
		Result<<GenVSSilLayoutLocation(FragmentOutputLayoutAvailable,0);
		Result<<"out vec4 "<<OutputVariable<<";\n";
	}
	Result<<"void main(){\n";
	Result<<"  if(gl_FrontFacing)\n";
	Result<<"    "<<OutputVariable<<"=vec4(0,1,0,1);\n";
	Result<<"  else\n";
	Result<<"    "<<OutputVariable<<"=vec4(0,0,1,1);\n";
	Result<<"}\n";//main end
	return Result.str();
}

