#include"adjacency.hpp"
#include<stdlib.h>
#include"Standard.hpp"
#include<iostream>
#include"CGenerateGeometryShaderSilhouette.hpp"

#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>


using namespace std;

float*Adjacency_RawTriangles;

int IsVecEqual(float*A,float*B){
	if(A[0]==B[0]&&A[1]==B[1]&&A[2]==B[2])return 1;
	return 0;
}

int CompareVec(const void*A,const void*B){
	unsigned a=*((unsigned*)A);
	unsigned b=*((unsigned*)B);
	if(Adjacency_RawTriangles[a*3+0]<Adjacency_RawTriangles[b*3+0])
		return-1;
	else if(Adjacency_RawTriangles[a*3+0]>Adjacency_RawTriangles[b*3+0])
		return 1;
	else{
		if(Adjacency_RawTriangles[a*3+1]<Adjacency_RawTriangles[b*3+1])
			return-1;
		else if(Adjacency_RawTriangles[a*3+1]>Adjacency_RawTriangles[b*3+1])
			return 1;
		else{
			if(Adjacency_RawTriangles[a*3+2]<Adjacency_RawTriangles[b*3+2])
				return-1;
			else if(Adjacency_RawTriangles[a*3+2]>Adjacency_RawTriangles[b*3+2])
				return 1;
			else
				return 0;
		}
	}
}

void SortTriangles(
		float**PointsOut,
		unsigned*NumPointsOut,
		unsigned**TrianglesOut,
		unsigned*NumTrianglesOut,
		float*TrianglesIn,
		unsigned NumTrianglesIn){

	//1. sort vertices - sort their indices
	unsigned*RawIndex=new unsigned[NumTrianglesIn*3];//indices for sorting
	for(unsigned i=0;i<NumTrianglesIn*3;++i)RawIndex[i]=i;//compute indices
	Adjacency_RawTriangles=TrianglesIn;//set triangles for sorting
	qsort(RawIndex,NumTrianglesIn*3,sizeof(unsigned),CompareVec);//sort indices

	//2. find out unique vertices
	//UniqueIndex represents mapping function from sorted raw indices to
	//unique indices
	unsigned*UniqueIndex=new unsigned[NumTrianglesIn*3];//allocate
	UniqueIndex[0]=0;//first sorted raw index points to first unique index

	//ULookUp represents mapping funcion from unique index to raw index
	//which represents vertex
	unsigned*ULookUp=new unsigned [NumTrianglesIn*3];//allocate
	unsigned ULookUpNum=1;//number of unique indices
	ULookUp[0]=RawIndex[0];//first unique index points to first raw index

	for(unsigned i=1;i<NumTrianglesIn*3;++i){//loop over all indices
		if(IsVecEqual(//is current vertex same as previous?
					TrianglesIn+RawIndex[i]*3,//current vertex
					TrianglesIn+RawIndex[i-1]*3)){//previous vertex
			UniqueIndex[i]=ULookUpNum-1;//set mapping func. to previous unique index
		}else{//current vertex is unique
			ULookUp[ULookUpNum++]=RawIndex[i];//new unique index
			UniqueIndex[i]=ULookUpNum-1;//set mapping func. to new unique index
		}
	}
	/*
		 for(unsigned i=0;i<NumTrianglesIn*3;++i)
		 cout<<RawIndex[i]<<" ";cout<<endl;
		 for(unsigned i=0;i<NumTrianglesIn*3;++i)
		 cout<<UniqueIndex[i]<<" ";cout<<endl;
		 for(unsigned i=0;i<ULookUpNum;++i)
		 cout<<ULookUp[i]<<" ";cout<<endl;
	// */
	//3.) create sorted array of vertices
	*PointsOut=new float[ULookUpNum*3];//allocate to number of unique vertices
	for(unsigned ui=0;ui<ULookUpNum;++ui){//loop over unique indices
		for(unsigned p=0;p<3;++p)//copy data
			(*PointsOut)[ui*3+p]=TrianglesIn[ULookUp[ui]*3+p];
	}
	*NumPointsOut=ULookUpNum;//set number of unique indices

	//4.) create sorted arrayo f indices
	*TrianglesOut=new unsigned[NumTrianglesIn*3];//allocate indices
	for(unsigned i=0;i<NumTrianglesIn*3;++i)//loop over indices
		(*TrianglesOut)[RawIndex[i]]=UniqueIndex[i];//write index
	*NumTrianglesOut=NumTrianglesIn;//set number of trinagles
	/*
		 for(unsigned i=0;i<NumTrianglesIn*3;++i)
		 cout<<(*TrianglesOut)[i]<<" ";cout<<endl;
	// */
	delete[]ULookUp;//free memory
	delete[]UniqueIndex;//free memory
	delete[]RawIndex;//free memory
}

void ComputePointTriangleAdjacency(
		unsigned**PointTriangleNum,
		unsigned***PointTriangle,
		unsigned NumPoints,
		unsigned*Triangles,
		unsigned NumTriangles){
	//1.) allocate number of triangles per point and set it to 0
	*PointTriangleNum=new unsigned[NumPoints];//allocate
	for(unsigned p=0;p<NumPoints;++p)(*PointTriangleNum)[p]=0;//set it to 0

	//2.) compute number of triangles per point
	for(unsigned t=0;t<NumTriangles;++t)//loop over trinagles
		for(unsigned p=0;p<3;++p)//loop over points of trinagle
			(*PointTriangleNum)[Triangles[t*3+p]]++;//add triangle to every point

	//3.) allocate mapping function from point to triangles
	*PointTriangle=new unsigned*[NumPoints];//allocate
	for(unsigned p=0;p<NumPoints;++p){//loop over points
		(*PointTriangle)[p]=new unsigned[(*PointTriangleNum)[p]];//allocate
	}

	//4.) clear triangle counters for every point
	for(unsigned p=0;p<NumPoints;++p)
		(*PointTriangleNum)[p]=0;

	//5.) create mapping function
	for(unsigned t=0;t<NumTriangles;++t){
		for(unsigned p=0;p<3;++p){
			unsigned pi=Triangles[t*3+p];
			(*PointTriangle)[pi][(*PointTriangleNum)[pi]++]=t;
		}
	}
	/*
		 for(unsigned p=0;p<NumPoints;++p)
		 cout<<(*PointTriangleNum)[p]<<" ";cout<<endl;

		 for(unsigned p=0;p<NumPoints;++p){
		 for(unsigned t=0;t<(*PointTriangleNum)[p];++t)
		 cout<<(*PointTriangle)[p][t]<<" ";
		 cout<<endl;
		 }
	// */
}

void ComputePointEdgeAdjacency(
		unsigned**PointEdgeNum,
		unsigned***PointEdge,
		unsigned*PointTriangleNum,
		unsigned**PointTriangle,
		unsigned NumPoints,
		unsigned*Triangles,
		unsigned NumTriangles){
	//1.) allocate number of edges per point and set it to 0
	*PointEdgeNum=new unsigned[NumPoints];//allocate
	for(unsigned p=0;p<NumPoints;++p)//loop over points
		(*PointEdgeNum)[p]=0;//set it to 0

	//2.) allocate number of edge per point
	*PointEdge=new unsigned*[NumPoints];//allocate
	for(unsigned p=0;p<NumPoints;++p){//loop over points
		(*PointEdge)[p]=new unsigned[PointTriangleNum[p]*2];//allocate
	}

	//3.) compute edges per points
	for(unsigned p=0;p<NumPoints;++p){//loop over points
		for(unsigned ti=0;ti<PointTriangleNum[p];++ti){//loop over triangles
			unsigned t=PointTriangle[p][ti];//triangle index
			unsigned AB[2];unsigned iab=0;//two points of the triangle
			for(unsigned i=0;i<3;++i){//loop over points of triangle
				if(Triangles[t*3+i]==p)continue;//skip same point
				AB[iab++]=Triangles[t*3+i];//set two points of the triangle
			}
			for(unsigned e=0;e<2;++e){//loop over wto points (edges) of the triangle
				int in=0;//flag, 0 edge is unique, 1 edge is already in array
				for(unsigned pi=0;pi<(*PointEdgeNum)[p];++pi){//loop over edges
					if((*PointEdge)[p][pi]==AB[e]){//is edge already inside?
						in=1;//set flag
						break;//we have alread found edge
					}
				}
				if(!in){//is flag set
					(*PointEdge)[p][(*PointEdgeNum)[p]++]=AB[e];//write edge
				}
			}
		}
	}
	/*
		 for(unsigned p=0;p<NumPoints;++p)
		 cout<<(*PointEdgeNum)[p]<<" ";cout<<endl;

		 for(unsigned p=0;p<NumPoints;++p){
		 for(unsigned e=0;e<(*PointEdgeNum)[p];++e)
		 cout<<(*PointEdge)[p][e]<<" ";
		 cout<<endl;
		 }
	// */
}

void ComputeEdge(
		unsigned***Edge,
		unsigned*NumEdge,
		unsigned*PointEdgeNum,
		unsigned**PointEdge,
		unsigned NumPoints){
	*NumEdge=0;
	//1.) estimate number of edges
	for(unsigned p=0;p<NumPoints;++p){//loop over points
		for(unsigned ei=0;ei<PointEdgeNum[p];++ei)//loop over points connected to point
			if(PointEdge[p][ei]>p)(*NumEdge)++;//count only those edges (a,b), b>a
	}

	//2.) allocate edges
	*Edge=new unsigned*[*NumEdge];//allocate
	for(unsigned e=0;e<*NumEdge;++e)//loop over edges
		(*Edge)[e]=new unsigned[2];//allocate

	//3.) copy edges
	*NumEdge=0;
	for(unsigned p=0;p<NumPoints;++p){//loop over points
		for(unsigned ei=0;ei<PointEdgeNum[p];++ei)//loop over points connected to point
			if(PointEdge[p][ei]>p){//count only those edges (a,b), b>a
				(*Edge)[*NumEdge][0]=p;//starting point
				(*Edge)[(*NumEdge)++][1]=PointEdge[p][ei];//end point
			}
	}
	/*
		 cout<<"EdgeNum: "<<*NumEdge<<endl;
		 for(unsigned e=0;e<*NumEdge;++e)
		 cout<<(*Edge)[e][0]<<":"<<(*Edge)[e][1]<<" ";cout<<endl;
	// */
}

void ComputeEdgeIndex(
		unsigned***EdgeIndex,
		unsigned NumEdge,
		unsigned*PointEdgeNum,
		unsigned**PointEdge,
		unsigned NumPoints){
	//1.) allocate edge index
	*EdgeIndex=new unsigned*[NumPoints];
	for(unsigned p=0;p<NumPoints;++p){//loop over points
		(*EdgeIndex)[p]=new unsigned[PointEdgeNum[p]];//allocate
	}

	//2.) compute edge index
	NumEdge=0;//edge counter
	for(unsigned p=0;p<NumPoints;++p){//loop over points
		for(unsigned ei=0;ei<PointEdgeNum[p];++ei){//loop over points connected to point
			if(PointEdge[p][ei]>p){//count only those edge (a,b), b>a
				(*EdgeIndex)[p][ei]=NumEdge;//this edge has this index
				NumEdge++;//increment index
			}else
				(*EdgeIndex)[p][ei]=(unsigned)-1;//this edge has already been counted
		}
	}
	/*
		 for(unsigned p=0;p<NumPoints;++p){
		 for(unsigned ei=0;ei<PointEdgeNum[p];++ei)
		 cout<<(*EdgeIndex)[p][ei]<<" ";cout<<endl;
		 }
	// */
}

void ComputeOppositeNum(
		unsigned**EdgeOppositeNum,
		unsigned NumEdge,
		unsigned*PointEdgeNum,
		unsigned**PointEdge,
		unsigned**EdgeIndex,
		unsigned*Triangles,
		unsigned NumTriangles){
	//1.) allocate opposite num
	*EdgeOppositeNum=new unsigned[NumEdge];//allocate
	for(unsigned e=0;e<NumEdge;++e)//loop over edges
		(*EdgeOppositeNum)[e]=0;

	//2.) count number of opposite points to edge
	for(unsigned t=0;t<NumTriangles;++t){//loop over triangles
		unsigned ABC[3]={Triangles[t*3+0],Triangles[t*3+1],Triangles[t*3+2]};//3 triangle points
		if(ABC[0]>ABC[1]){unsigned c=ABC[0];ABC[0]=ABC[1];ABC[1]=c;}//sort points
		if(ABC[1]>ABC[2]){unsigned c=ABC[1];ABC[1]=ABC[2];ABC[2]=c;}//sort points
		if(ABC[0]>ABC[1]){unsigned c=ABC[0];ABC[0]=ABC[1];ABC[1]=c;}//sort points
		unsigned E[3][2]={{ABC[0],ABC[1]},{ABC[0],ABC[2]},{ABC[1],ABC[2]}};//triangle edges
		for(unsigned k=0;k<3;++k){//loop over triangle edges
			//loop over points connected to starting point of edge
			for(unsigned ei=0;ei<PointEdgeNum[E[k][0]];++ei)//loop over points connected to ...
				if(PointEdge[E[k][0]][ei]==E[k][1]){//is end point of t. edge same as connected point
					(*EdgeOppositeNum)[EdgeIndex[E[k][0]][ei]]++;//increase opposite num
					break;
				}
		}
	}
}

void ComputeOpposite(
		unsigned***EdgeOpposite,
		unsigned**EdgeIndex,
		unsigned*EdgeOppositeNum,
		unsigned EdgeNum,
		unsigned**PointEdge,
		unsigned*PointEdgeNum,
		unsigned*Triangles,
		unsigned NumTriangles){
	//1.) allocate opposite points
	*EdgeOpposite=new unsigned*[EdgeNum];//allocate
	for(unsigned e=0;e<EdgeNum;++e){//loop over edges
		(*EdgeOpposite)[e]=new unsigned[EdgeOppositeNum[e]];//allocate opposite points
	}

	//2.) set opposite points counters to 0
	for(unsigned e=0;e<EdgeNum;++e)//loop over edges
		EdgeOppositeNum[e]=0;//clear counter

	//3.) compute opposite
	for(unsigned t=0;t<NumTriangles;++t){//loop over triangles
		unsigned ABC[3]={Triangles[t*3+0],Triangles[t*3+1],Triangles[t*3+2]};//3 triangle points
		if(ABC[0]>ABC[1]){unsigned c=ABC[0];ABC[0]=ABC[1];ABC[1]=c;}//sort points
		if(ABC[1]>ABC[2]){unsigned c=ABC[1];ABC[1]=ABC[2];ABC[2]=c;}//sort points
		if(ABC[0]>ABC[1]){unsigned c=ABC[0];ABC[0]=ABC[1];ABC[1]=c;}//sort points
		unsigned E[3][3]={
			{ABC[0],ABC[1],ABC[2]},//AB edge and opposite point C
			{ABC[0],ABC[2],ABC[1]},//AC edge and opposite point B
			{ABC[1],ABC[2],ABC[0]}};//BC edge and opposite point A
		for(unsigned k=0;k<3;++k){//loop over edges of triangle
			for(unsigned ei=0;ei<PointEdgeNum[E[k][0]];++ei)//loop over edges connected to point
				if(PointEdge[E[k][0]][ei]==E[k][1]){//same end point
					(*EdgeOpposite)[EdgeIndex[E[k][0]][ei]][EdgeOppositeNum[EdgeIndex[E[k][0]][ei]]++]=
						E[k][2];//add opposite point
					break;
				}
		}
	}
}
void FreeAdjacency(SAdjecency*Adj){
	delete[]Adj->Points;
	delete[]Adj->Triangles;
	delete[]Adj->PointTriangleNum;
	for(unsigned p=0;p<Adj->NumPoints;++p)delete[]Adj->PointTriangles[p];
	delete[]Adj->PointTriangles;
	delete[]Adj->PointEdgeNum;
	for(unsigned p=0;p<Adj->NumPoints;++p)delete[]Adj->PointEdge[p];
	delete[]Adj->PointEdge;
	for(unsigned p=0;p<Adj->NumPoints;++p)delete[]Adj->EdgeIndex[p];
	delete[]Adj->EdgeIndex;
	for(unsigned e=0;e<Adj->NumEdges;++e)delete[]Adj->Edges[e];
	delete[]Adj->Edges;
	delete[]Adj->EdgeOppositeNum;
	for(unsigned e=0;e<Adj->NumEdges;++e)delete[]Adj->EdgeOpposite[e];
	delete[]Adj->EdgeOpposite;
}


void ComputeAdjacency(
		SAdjecency*Adjacency,
		float*Points,
		unsigned NumTriangles){
	SortTriangles(
			&Adjacency->Points,
			&Adjacency->NumPoints,
			&Adjacency->Triangles,
			&Adjacency->NumTriangles,
			Points,
			NumTriangles);

	ComputePointTriangleAdjacency(
			&Adjacency->PointTriangleNum,
			&Adjacency->PointTriangles,
			Adjacency->NumPoints,
			Adjacency->Triangles,
			Adjacency->NumTriangles);

	ComputePointEdgeAdjacency(
			&Adjacency->PointEdgeNum,
			&Adjacency->PointEdge,
			Adjacency->PointTriangleNum,
			Adjacency->PointTriangles,
			Adjacency->NumPoints,
			Adjacency->Triangles,
			Adjacency->NumTriangles);

	ComputeEdge(
			&Adjacency->Edges,
			&Adjacency->NumEdges,
			Adjacency->PointEdgeNum,
			Adjacency->PointEdge,
			Adjacency->NumPoints);

	ComputeEdgeIndex(
			&Adjacency->EdgeIndex,
			Adjacency->NumEdges,
			Adjacency->PointEdgeNum,
			Adjacency->PointEdge,
			Adjacency->NumPoints);

	ComputeOppositeNum(
			&Adjacency->EdgeOppositeNum,
			Adjacency->NumEdges,
			Adjacency->PointEdgeNum,
			Adjacency->PointEdge,
			Adjacency->EdgeIndex,
			Adjacency->Triangles,
			Adjacency->NumTriangles);

	ComputeOpposite(
			&Adjacency->EdgeOpposite,
			Adjacency->EdgeIndex,
			Adjacency->EdgeOppositeNum,
			Adjacency->NumEdges,
			Adjacency->PointEdge,
			Adjacency->PointEdgeNum,
			Adjacency->Triangles,
			Adjacency->NumTriangles);
  Adjacency->MaxOpposite=0;
  for(unsigned e=0;e<Adjacency->NumEdges;++e){
    if(Adjacency->EdgeOppositeNum[e]>Adjacency->MaxOpposite)
      Adjacency->MaxOpposite=Adjacency->EdgeOppositeNum[e];
  }
  Adjacency->ClassCard=new unsigned[Adjacency->MaxOpposite];
  for(unsigned m=0;m<Adjacency->MaxOpposite;++m)Adjacency->ClassCard[m]=0;
  for(unsigned e=0;e<Adjacency->NumEdges;++e)
    Adjacency->ClassCard[Adjacency->EdgeOppositeNum[e]-1]++;

  unsigned OppositePairRemoved=0;
  for(unsigned e=0;e<Adjacency->NumEdges;++e){
    unsigned numO=Adjacency->EdgeOppositeNum[e];
    for(unsigned o0=0;o0<numO-1;++o0){
      for(unsigned o1=o0+1;o1<numO-1;++o1){
        unsigned o0i=Adjacency->EdgeOpposite[e][o0];
        unsigned o1i=Adjacency->EdgeOpposite[e][o1];
        unsigned ai=Adjacency->Edges[e][0];
        unsigned bi=Adjacency->Edges[e][1];
        
        glm::vec3 o0v=glm::vec3(
            Adjacency->Points[o0i*3+0],
            Adjacency->Points[o0i*3+1],
            Adjacency->Points[o0i*3+2]);
        glm::vec3 o1v=glm::vec3(
            Adjacency->Points[o1i*3+0],
            Adjacency->Points[o1i*3+1],
            Adjacency->Points[o1i*3+2]);
        glm::vec3 av=glm::vec3(
            Adjacency->Points[ai*3+0],
            Adjacency->Points[ai*3+1],
            Adjacency->Points[ai*3+2]);
        glm::vec3 bv=glm::vec3(
            Adjacency->Points[bi*3+0],
            Adjacency->Points[bi*3+1],
            Adjacency->Points[bi*3+2]);
        glm::vec3 n0=glm::normalize(glm::cross(o1v-o0v,av-o0v));
        glm::vec3 n1=glm::normalize(glm::cross(bv-o0v,o1v-o0v));
        if(glm::length(n1-n0)<=0.00001)
          OppositePairRemoved++;
      }
    }
  }
  std::cerr<<"OppositePairRemoved: "<<OppositePairRemoved<<std::endl;
}

unsigned*Adjacency_ClassCard;
int CompareClassIndex(const void*A,const void*B){
	unsigned a=*((unsigned*)A);
	unsigned b=*((unsigned*)B);
	if(Adjacency_ClassCard[a]<Adjacency_ClassCard[b])
		return+1;
	else if(Adjacency_ClassCard[a]>Adjacency_ClassCard[b])
		return-1;
	return 0;
}

void CShadowShader::ComputeSpecificEdges(
		SSpecificEdge*SpecificEdge,
		SAdjecency*Adjacency,
		unsigned MinNum){
	//1.) compute max opposite num
	unsigned NumClass=0;//max opposite num
	for(unsigned e=0;e<Adjacency->NumEdges;++e)//loop over edges
		if(NumClass<Adjacency->EdgeOppositeNum[e])//current is lesser
			NumClass=Adjacency->EdgeOppositeNum[e];//set to new value

	SpecificEdge->MaxN=NumClass;//set number of classes
	SpecificEdge->NEdge=new SEdgePointVAO[SpecificEdge->MaxN];//allocate classes

	//2.) compute classes
	unsigned*ClassCard=new unsigned[NumClass];//allocete count
	for(unsigned c=0;c<NumClass;++c)//loop over classes
		ClassCard[c]=0;//set count for class to 0

	//3.) compute classes' counts
	for(unsigned e=0;e<Adjacency->NumEdges;++e)//loop over edges
		ClassCard[Adjacency->EdgeOppositeNum[e]-1]++;//increase number

	//*
	cout<<"Histogram"<<endl;
		 for(unsigned c=0;c<NumClass;++c)
		 cout<<c+1<<": "<<ClassCard[c]<<endl;
	// */

	//4.) compute class index
	unsigned*ClassIndex=new unsigned[NumClass];//allocate class index
	for(unsigned c=0;c<NumClass;++c)//loop over classes
		ClassIndex[c]=c;//set class index

	//5.) sort class index by classes' cardinality
	Adjacency_ClassCard=ClassCard;//set global variable for sorting
	qsort(ClassIndex,NumClass,sizeof(unsigned),CompareClassIndex);//sort index

	/*
		 for(unsigned c=0;c<NumClass;++c)
		 cout<<ClassIndex[c]+1<<": "<<ClassCard[ClassIndex[c]]<<endl;
	// */

	//6.) compute number of classes over threshold
	unsigned NumSpecific=0;//set number of specific cases to 0
	for(unsigned c=0;c<NumClass;++c)//loop over classes
		if(ClassCard[ClassIndex[c]]>MinNum)//class cardinality is geater
			++NumSpecific;//increase number of specific cases

	//7.)alter allocate VBO cascade
	// compute lenghts of cascade VBOs
	
	//compute length of universal VBO
	SpecificEdge->Cascade.LenUniversal=0;
	SpecificEdge->Cascade.StartUniversal=0;
	for(unsigned c=NumSpecific;c<NumClass;++c)//loop over universal classes
		SpecificEdge->Cascade.LenUniversal+=ClassCard[ClassIndex[c]];//add amount of edges

	//compute length and start of cascade VBO
	for(unsigned i=0;i<15;++i)
		SpecificEdge->Cascade.StartCascade[i]=SpecificEdge->Cascade.LenCascade[i]=0;

	//compute lengths
	for(int i=NumClass-1;i>=0;--i){//loop over classes
		if(ClassCard[i]>MinNum){//class is sufficiently big
			SpecificEdge->Cascade.LenCascade[2+i]=ClassCard[i];//set len
			for(int j=i;j>=0;--j)//loop over lesser classes
				if(ClassCard[j]>MinNum){//lesser class is sufficiently big
					for(int k=0;k<j;++k)//loop over attribues
						SpecificEdge->Cascade.StartCascade[2+k]+=ClassCard[i];//add offset of class
					break;
				}
		}
	}
	if(NumSpecific>0){
		SpecificEdge->Cascade.LenCascade[0]=Adjacency->NumEdges;
		SpecificEdge->Cascade.LenCascade[1]=Adjacency->NumEdges;
	}

	for(unsigned i=0;i<15;++i)
		std::cerr<<i<<": "<<SpecificEdge->Cascade.StartCascade[i]<<" "<<SpecificEdge->Cascade.LenCascade[i]<<std::endl;

	std::cerr<<"Universal: "<<SpecificEdge->Cascade.StartUniversal<<" "<<SpecificEdge->Cascade.LenUniversal<<std::endl;



/*
	// allocate cascade VBO
	glGenBuffers(15,SpecificEdge->Cascade.VBOCascade);
	for(unsigned i=0;i<15;++i){
		glBindBuffer(GL_ARRAY_BUFFER,SpecificEdge->Cascade.VBOCascade[i]);
		glBufferData(GL_ARRAY_BUFFER,sizeof(float)*4*SpecificEdge->Cascade.LenCascade[i],NULL,GL_STATIC_DRAW);
	}
	// allocate universal VBO
	glGenBuffers(1,&SpecificEdge->Cascade.VBOUniversal);
	glBindBuffer(GL_ARRAY_BUFFER,SpecificEdge->Cascade.VBOUniversal);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float)*4*SpecificEdge->Cascade.LenUniversal,NULL,GL_STATIC_DRAW);
	*/



	//7.) allocate VBO
	glGenBuffers(1,&SpecificEdge->VBO);//vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER,SpecificEdge->VBO);//bind VBO
	glBufferData(GL_ARRAY_BUFFER,sizeof(float)*4*16*Adjacency->NumEdges,
			NULL,GL_STATIC_DRAW);

	//8.) set data to VBO
	float*ptr=(float*)glMapBuffer(GL_ARRAY_BUFFER,GL_READ_WRITE);
	for(unsigned e=0;e<Adjacency->NumEdges;++e){//loop over edges
		//A point of the edge
		ptr[e*4*16+0*4+0]=Adjacency->Points[Adjacency->Edges[e][0]*3+0];
		ptr[e*4*16+0*4+1]=Adjacency->Points[Adjacency->Edges[e][0]*3+1];
		ptr[e*4*16+0*4+2]=Adjacency->Points[Adjacency->Edges[e][0]*3+2];
		ptr[e*4*16+0*4+3]=1;
		//B point of the edge
		ptr[e*4*16+1*4+0]=Adjacency->Points[Adjacency->Edges[e][1]*3+0];
		ptr[e*4*16+1*4+1]=Adjacency->Points[Adjacency->Edges[e][1]*3+1];
		ptr[e*4*16+1*4+2]=Adjacency->Points[Adjacency->Edges[e][1]*3+2];
		ptr[e*4*16+1*4+3]=1;
		//opposite points of the edge
		for(unsigned o=0;o<Adjacency->EdgeOppositeNum[e];++o){
			ptr[e*4*16+(2+o)*4+0]=Adjacency->Points[Adjacency->EdgeOpposite[e][o]*3+0];
			ptr[e*4*16+(2+o)*4+1]=Adjacency->Points[Adjacency->EdgeOpposite[e][o]*3+1];
			ptr[e*4*16+(2+o)*4+2]=Adjacency->Points[Adjacency->EdgeOpposite[e][o]*3+2];
			if(	Adjacency->EdgeOpposite[e][o]>Adjacency->Edges[e][0]&&
					Adjacency->EdgeOpposite[e][o]>Adjacency->Edges[e][1])
				ptr[e*4*16+(2+o)*4+3]=1;
			else
				ptr[e*4*16+(2+o)*4+3]=0;
		}
		//number of opposite points of the edge
		ptr[e*4*16+15*4+0]=Adjacency->EdgeOppositeNum[e];
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	//9.) compute universal buffer size
	SpecificEdge->Universal.N=0;//compute number of edges which don't fall into any class
	for(unsigned c=NumSpecific;c<NumClass;++c)//loop over useless classes
		SpecificEdge->Universal.N+=ClassCard[ClassIndex[c]];//add count

	/*
		 cout<<"Universal: "<<SpecificEdge->Universal.N<<endl;
	// */

	//10.) allocate universal buffer
	if(SpecificEdge->Universal.N>0){//are there edge which has to be handled universally
		//10a) create VAO
		glGenVertexArrays(1,&SpecificEdge->Universal.VAO);//vertex array object
		glBindVertexArray(SpecificEdge->Universal.VAO);//bind VAO

		//10b) bind VBO
		glBindBuffer(GL_ARRAY_BUFFER,SpecificEdge->VBO);//bind VBO

		//10c) create EBO
		glGenBuffers(1,&SpecificEdge->Universal.EBO);//generate EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,SpecificEdge->Universal.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned)*SpecificEdge->Universal.N,
				NULL,GL_STATIC_DRAW);

		//10d) copy data to EBO
		unsigned*uptr=(unsigned*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,GL_READ_WRITE);
		SpecificEdge->Universal.N=0;//set counter to 0
		for(unsigned e=0;e<Adjacency->NumEdges;++e)//loop over edges
			if(ClassCard[Adjacency->EdgeOppositeNum[e]-1]<=MinNum)//number of these edges is small
				uptr[SpecificEdge->Universal.N++]=e;//set index to edge
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		//10e) set Pointers
		for(unsigned p=0;p<16;++p){//loop over pointers
			glEnableVertexAttribArray(p);
			glVertexAttribPointer(p,4,GL_FLOAT,GL_FALSE,
					sizeof(float)*4*16,(GLvoid*)(sizeof(float)*4*p));
		}

		//10f) unbind VAO
		glBindVertexArray(0);
	}

	//11.) allocate specific buffers
	for(unsigned c=0;c<NumClass;++c){//loop over classes
		if(ClassCard[c]<=MinNum)//number of edges is too small
			SpecificEdge->NEdge[c].N=0;//no edges
		else{//number of edges is sufficient
			//11a) create VAO
			glGenVertexArrays(1,&SpecificEdge->NEdge[c].VAO);
			glBindVertexArray(SpecificEdge->NEdge[c].VAO);

			//11b) bind VBO
			glBindBuffer(GL_ARRAY_BUFFER,SpecificEdge->VBO);//bind VBO

			//11c) create EBO
			glGenBuffers(1,&(SpecificEdge->NEdge[c].EBO));//generate EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,SpecificEdge->NEdge[c].EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned)*ClassCard[c],
					NULL,GL_STATIC_DRAW);

			//11d) copy data to EBO
			unsigned*sptr=(unsigned*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,GL_READ_WRITE);
			SpecificEdge->NEdge[c].N=0;//set counter to 0
			for(unsigned e=0;e<Adjacency->NumEdges;++e)//loop over edges
				if(Adjacency->EdgeOppositeNum[e]-1==c)//this is the class we are searching for
					sptr[SpecificEdge->NEdge[c].N++]=e;//set index to edge
			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

			//11e) set Pointers
			for(unsigned p=0;p<16;++p){//loop over pointers
				if(p<=c+2){//useful pointer
					glEnableVertexAttribArray(p);//enable pointer
					glVertexAttribPointer(p,4,GL_FLOAT,GL_FALSE,//set pointer
							sizeof(float)*4*16,(GLvoid*)(sizeof(float)*4*p));
				}else
					glDisableVertexAttribArray(p);//disable pointer
			}

			//11f) unbind VAO
			glBindVertexArray(0);
		}
	}

	delete[]ClassIndex;
	delete[]ClassCard;
}

ge::gl::ProgramObject*GenHull(SGSSilTemplate T,unsigned s){
  ge::gl::ShaderObject*SSNH[3]={
    new ge::gl::ShaderObject(GL_VERTEX_SHADER,GenGSSilVertex(s,T)),
    new ge::gl::ShaderObject(GL_GEOMETRY_SHADER,GenGSSilGeometryHull(s,T)),
    new ge::gl::ShaderObject(GL_FRAGMENT_SHADER,GenGSSilFragment(T))
  };
  ge::gl::ProgramObject*Program=new ge::gl::ProgramObject(SSNH,3);
  delete SSNH[0];
  delete SSNH[1];
  delete SSNH[2];
  return Program;
}

ge::gl::ProgramObject*GenCap(SGSSilTemplate T,unsigned s){
  ge::gl::ShaderObject*SSNH[3]={
    new ge::gl::ShaderObject(GL_VERTEX_SHADER,GenGSSilVertex(s,T)),
    new ge::gl::ShaderObject(GL_GEOMETRY_SHADER,GenGSSilGeometryCap(s,T)),
    new ge::gl::ShaderObject(GL_FRAGMENT_SHADER,GenGSSilFragment(T))
  };
  ge::gl::ProgramObject*Program=new ge::gl::ProgramObject(SSNH,3);
  delete SSNH[0];
  delete SSNH[1];
  delete SSNH[2];
  return Program;
}

ge::gl::ProgramObject*GenProgram(NGeometrySilhouetteShader::STemplate T){
  ge::gl::ShaderObject*SSNH[3]={
    new ge::gl::ShaderObject(GL_VERTEX_SHADER,NGeometrySilhouetteShader::getVertex(T)),
    new ge::gl::ShaderObject(GL_GEOMETRY_SHADER,NGeometrySilhouetteShader::getGeometry(T)),
    new ge::gl::ShaderObject(GL_FRAGMENT_SHADER,NGeometrySilhouetteShader::getFragment(T))
  };
  ge::gl::ProgramObject*Program=new ge::gl::ProgramObject(SSNH,3);
  delete SSNH[0];
  delete SSNH[1];
  delete SSNH[2];
  return Program;
}

void CShadowShader::LoadShaders(){
  
	SGSSilTemplate T;
	T.Version=430;
	T.Compatible=false;
	T.Universal=false;
	T.Framebuffer=false;
	T.Deterministic=false;
	T.OneRepresenatative=false;
	T.ZFail=false;
	T.UseGSInvocation=false;
  T.CullFace=!true;

	for(unsigned s=0;s<14;++s){//loop over shaders

		T.Deterministic=false;
		T.Framebuffer=false;
		T.ImageAtomicAdd=false;

		this->SpecStencilNonDetHull[s]=GenHull(T,s+1);
    this->SpecStencilNonDetCap[s]= GenCap(T,s+1);

		T.Framebuffer=true;

		this->SpecFrameNonDetHull[s]=GenHull(T,s+1);
    this->SpecFrameNonDetCap[s]=GenCap(T,s+1);

		T.Deterministic=true;
		T.Framebuffer=false;
/*
    this->SpecStencilDetHull[s]=GenHull(T,s+1);
    this->SpecStencilDetCap[s]=GenCap(T,s+1);
*/
    NGeometrySilhouetteShader::STemplate NewT;
    NewT.Deterministic=true;
    NewT.ReferenceEdge=true;
    NewT.Universal=false;
    NewT.UseVS2GSArray=true;
    NewT.UseVertexArray=false;
    NewT.UseLayouts=true;
    NewT.UseStencilValueExport=false;
    NewT.CCWIsFrontFace=true;
    NewT.FrontFaceInside=false;
    NewT.CullSides=true;
    NewT.GenerateSides=true;
    NewT.GenerateCaps=false;
    NewT.Visualize=true;
    NewT.FrontCapToInfinity=true;
    NewT.Matrix="mvp";
    NewT.Version=430;
    NewT.Profile=CORE;
    NewT.MaxMultiplicity=s+1;
    NewT.LightUniform="LightPosition";
    NewT.VertexAttribName="Position";
    this->SpecStencilDetHull[s]=GenProgram(NewT);
    NewT.GenerateCaps=true;
    NewT.GenerateSides=false;
    this->SpecStencilDetCap[s]=GenProgram(NewT);


		T.Framebuffer=true;

    this->SpecFrameDetHull[s]=GenHull(T,s+1);
		this->SpecFrameDetCap[s]=GenCap(T,s+1);

		T.Deterministic=false;
		T.Framebuffer=true;
		T.ImageAtomicAdd=true;

    this->SpecFrameAtomicNonDetHull[s]=GenHull(T,s+1);
		this->SpecFrameAtomicNonDetCap[s] =GenCap(T,s+1);

		T.Deterministic=true;
 		this->SpecFrameAtomicDetHull[s]=GenHull(T,s+1);
		this->SpecFrameAtomicDetCap[s] =GenCap(T,s+1);
	}

	T.Universal=true;
	T.Framebuffer=false;
	T.Deterministic=false;

	this->UniStencilNonDetHull=GenHull(T,13);
	this->UniStencilNonDetCap= GenCap(T,13);

	T.Framebuffer=true;

	this->UniFrameNonDetHull=GenHull(T,13);
	this->UniFrameNonDetCap= GenCap(T,13);

	T.Framebuffer=false;
	T.Deterministic=true;
/*
	this->UniStencilDetHull=GenHull(T,13);
	this->UniStencilDetCap= GenCap(T,13);
*/
  NGeometrySilhouetteShader::STemplate NewT;
  NewT.Deterministic=true;
  NewT.ReferenceEdge=true;
  NewT.Universal=true;
  NewT.UseVS2GSArray=true;
  NewT.UseVertexArray=false;
  NewT.UseLayouts=true;
  NewT.UseStencilValueExport=false;
  NewT.CCWIsFrontFace=true;
  NewT.FrontFaceInside=false;
  NewT.CullSides=true;
  NewT.GenerateSides=true;
  NewT.GenerateCaps=false;
  NewT.Visualize=true;
  NewT.FrontCapToInfinity=true;
  NewT.Matrix="mvp";
  NewT.Version=430;
  NewT.Profile=CORE;
  NewT.MaxMultiplicity=13;
  NewT.LightUniform="LightPosition";
  NewT.VertexAttribName="Position";
  this->UniStencilDetHull=GenProgram(NewT);
  NewT.GenerateCaps=true;
  NewT.GenerateSides=false;
  this->UniStencilDetCap=GenProgram(NewT);

	T.Framebuffer=true;

	this->UniFrameDetHull=GenHull(T,13);
	this->UniFrameDetCap= GenCap(T,13);

	T.Universal=true;
	T.Framebuffer=true;
	T.ImageAtomicAdd=true;
	T.Deterministic=false;

	this->UniFrameAtomicNonDetHull=GenHull(T,13);
	this->UniFrameAtomicNonDetCap= GenCap(T,13);

	T.Deterministic=false;

	this->UniFrameAtomicDetHull=GenHull(T,13);
	this->UniFrameAtomicDetCap= GenCap(T,13);
}


void CShadowShader::SetUniforms(ge::gl::ProgramObject*Prg,float*M,float*V,float*P,float*L){
	//Prg->Set("m",1,GL_FALSE,M);
	//Prg->Set("v",1,GL_FALSE,V);
	//Prg->Set("p",1,GL_FALSE,P);
	Prg->set("mvp",1,GL_FALSE,M);
	Prg->set("LightPosition",1,L);
}

void CShadowShader::Draw(GLuint VAO,unsigned Num){
	glBindVertexArray(VAO);//bind VAO
	glDrawElements(GL_POINTS,Num,GL_UNSIGNED_INT,NULL);//draw
	glBindVertexArray(0);//unbind VAO
}

void CShadowShader::DrawUniversal(ge::gl::ProgramObject*Prg,float*M,float*V,float*P,float*L){
	if(this->SpecificEdge.Universal.N){
		Prg->use();
		this->SetUniforms(Prg,M,V,P,L);
		this->Draw(this->SpecificEdge.Universal.VAO,this->SpecificEdge.Universal.N);
	}
}

void CShadowShader::DrawSpecific(ge::gl::ProgramObject**Prg,float*M,float*V,float*P,float*L){
	for(unsigned c=0;c<this->SpecificEdge.MaxN;++c){//loop over classes
		if(this->SpecificEdge.NEdge[c].N){//there are edges
			Prg[c]->use();
			this->SetUniforms(Prg[c],M,V,P,L);
			this->Draw(this->SpecificEdge.NEdge[c].VAO,this->SpecificEdge.NEdge[c].N);
		}
	}
}

void CShadowShader::DrawShadowVolume(float*M,float*V,float*P,float*L){
	this->DrawSpecific(this->SpecificShader,M,V,P,L);
	this->DrawUniversal(this->UniversalShader,M,V,P,L);
}

void CShadowShader::DrawShadowVolumeDeterministic(float*M,float*V,float*P,float*L){
	this->DrawSpecific(this->SpecificShaderDeterministic,M,V,P,L);
	this->DrawUniversal(this->UniversalShaderDeterministic,M,V,P,L);
}

void CShadowShader::DrawShadowVolumeMulti(float*M,float*V,float*P,float*L){
	this->DrawSpecific(this->SpecificShaderMulti,M,V,P,L);
	this->DrawUniversal(this->UniversalShaderMulti,M,V,P,L);
}





void CShadowShader::DrawShadowVolumeStencilNondeterministicZpassShader(
		float*M,float*V,float*P,float*L){
	/*
	this->DrawSpecific(this->SpecificStencilNondeterministicZpassShader,M,V,P,L);
	this->DrawUniversal(this->UniversalStencilNondeterministicZpassShader,M,V,P,L);
	*/
	this->DrawSpecific(this->SpecStencilNonDetHull,M,V,P,L);
	this->DrawUniversal(this->UniStencilNonDetHull,M,V,P,L);

}

void CShadowShader::DrawShadowVolumeFramebufferNondeterministicZpassShader(
		float*M,float*V,float*P,float*L){
	/*
	this->DrawSpecific(this->SpecificFramebufferNondeterministicZpassShader,M,V,P,L);
	this->DrawUniversal(this->UniversalFramebufferNondeterministicZpassShader,M,V,P,L);
	*/
	this->DrawSpecific(this->SpecFrameNonDetHull,M,V,P,L);
	this->DrawUniversal(this->UniFrameNonDetHull,M,V,P,L);

}

void CShadowShader::DrawShadowVolumeFramebufferAtomicNondeterministicZpassShader(
		float*M,float*V,float*P,float*L){
	this->DrawSpecific(this->SpecFrameAtomicNonDetHull,M,V,P,L);
	this->DrawUniversal(this->UniFrameAtomicNonDetHull,M,V,P,L);
}


void CShadowShader::DrawShadowVolumeStencilDeterministicZpassShader(
		float*M,float*V,float*P,float*L){
	/*
	this->DrawSpecific(this->SpecificStencilDeterministicZpassShader,M,V,P,L);
	this->DrawUniversal(this->UniversalStencilDeterministicZpassShader,M,V,P,L);
	*/
	this->DrawSpecific(this->SpecStencilDetHull,M,V,P,L);
	this->DrawUniversal(this->UniStencilDetHull,M,V,P,L);

}

void CShadowShader::DrawShadowVolumeFramebufferDeterministicZpassShader(
		float*M,float*V,float*P,float*L){
	/*
	this->DrawSpecific(this->SpecificFramebufferDeterministicZpassShader,M,V,P,L);
	this->DrawUniversal(this->UniversalFramebufferDeterministicZpassShader,M,V,P,L);
	*/
	this->DrawSpecific(this->SpecFrameDetHull,M,V,P,L);
	this->DrawUniversal(this->UniFrameDetHull,M,V,P,L);

}

void CShadowShader::DrawShadowVolumeFramebufferAtomicDeterministicZpassShader(
		float*M,float*V,float*P,float*L){
	this->DrawSpecific(this->SpecFrameAtomicDetHull,M,V,P,L);
	this->DrawUniversal(this->UniFrameAtomicDetHull,M,V,P,L);
}

void CShadowShader::DrawShadowVolumeStencilNondeterministicZfailShader(
		float*M,float*V,float*P,float*L){
	/*
	this->DrawSpecific(this->SpecificStencilNondeterministicZfailShader,M,V,P,L);
	this->DrawUniversal(this->UniversalStencilNondeterministicZfailShader,M,V,P,L);
	*/
	this->DrawSpecific(this->SpecStencilNonDetHull,M,V,P,L);
	this->DrawUniversal(this->UniStencilNonDetHull,M,V,P,L);
	this->DrawSpecific(this->SpecStencilNonDetCap,M,V,P,L);
	this->DrawUniversal(this->UniStencilNonDetCap,M,V,P,L);

}

void CShadowShader::DrawShadowVolumeFramebufferNondeterministicZfailShader(
		float*M,float*V,float*P,float*L){
	/*
	this->DrawSpecific(this->SpecificFramebufferNondeterministicZfailShader,M,V,P,L);
	this->DrawUniversal(this->UniversalFramebufferNondeterministicZfailShader,M,V,P,L);
	*/
	this->DrawSpecific(this->SpecFrameNonDetHull,M,V,P,L);
	this->DrawUniversal(this->UniFrameNonDetHull,M,V,P,L);
	this->DrawSpecific(this->SpecFrameNonDetCap,M,V,P,L);
	this->DrawUniversal(this->UniFrameNonDetCap,M,V,P,L);

}

void CShadowShader::DrawShadowVolumeFramebufferAtomicNondeterministicZfailShader(
		float*M,float*V,float*P,float*L){
	this->DrawSpecific(this->SpecFrameAtomicNonDetHull,M,V,P,L);
	this->DrawUniversal(this->UniFrameAtomicNonDetHull,M,V,P,L);
	this->DrawSpecific(this->SpecFrameAtomicNonDetCap,M,V,P,L);
	this->DrawUniversal(this->UniFrameAtomicNonDetCap,M,V,P,L);
}

void CShadowShader::DrawShadowVolumeStencilDeterministicZfailShader(
		float*M,float*V,float*P,float*L){
	/*
	this->DrawSpecific(this->SpecificStencilDeterministicZfailShader,M,V,P,L);
	this->DrawUniversal(this->UniversalStencilDeterministicZfailShader,M,V,P,L);
	*/
	this->DrawSpecific(this->SpecStencilDetHull,M,V,P,L);
	this->DrawUniversal(this->UniStencilDetHull,M,V,P,L);
	this->DrawSpecific(this->SpecStencilDetCap,M,V,P,L);
	this->DrawUniversal(this->UniStencilDetCap,M,V,P,L);

}

void CShadowShader::DrawShadowVolumeFramebufferDeterministicZfailShader(
		float*M,float*V,float*P,float*L){
	/*
	this->DrawSpecific(this->SpecificFramebufferDeterministicZfailShader,M,V,P,L);
	this->DrawUniversal(this->UniversalFramebufferDeterministicZfailShader,M,V,P,L);
	*/
	this->DrawSpecific(this->SpecFrameDetHull,M,V,P,L);
	this->DrawUniversal(this->UniFrameDetHull,M,V,P,L);
	this->DrawSpecific(this->SpecFrameDetCap,M,V,P,L);
	this->DrawUniversal(this->UniFrameDetCap,M,V,P,L);

}

void CShadowShader::DrawShadowVolumeFramebufferAtomicDeterministicZfailShader(
		float*M,float*V,float*P,float*L){
	this->DrawSpecific(this->SpecFrameAtomicDetHull,M,V,P,L);
	this->DrawUniversal(this->UniFrameAtomicDetHull,M,V,P,L);
	this->DrawSpecific(this->SpecFrameAtomicDetCap,M,V,P,L);
	this->DrawUniversal(this->UniFrameAtomicDetCap,M,V,P,L);
}

void CShadowShader::DrawShadowVolumeGI(
				float*M,float*V,float*P,float*L){
	this->DrawSpecific(this->SpecStencilDetHullGI,M,V,P,L);
	this->DrawUniversal(this->UniStencilDetHullGI,M,V,P,L);
	this->DrawSpecific(this->SpecStencilDetCap,M,V,P,L);
	this->DrawUniversal(this->UniStencilDetCap,M,V,P,L);
}



CShadowShader::CShadowShader(SAdjecency*Adjacency,unsigned MinNum){
	this->LoadShaders();
	this->ComputeSpecificEdges(&this->SpecificEdge,Adjacency,MinNum);
}

CShadowShader::~CShadowShader(){
	for(unsigned s=0;s<14;++s){//loop over shaders
		delete this->SpecificShader[s];
		delete this->SpecificShaderMulti[s];
		delete this->SpecificShaderDeterministic[s];
	}
	delete this->UniversalShader;
	delete this->UniversalShaderMulti;
	delete this->UniversalShaderDeterministic;
	delete[]this->SpecificEdge.NEdge;
}
