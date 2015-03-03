#include"sintorn_tiles.hpp"

#include<math.h>
#include<stdlib.h>

/**
 * @brief This function finds out possible choices of size of tile
 *
 * @param Choices possible choices, size is NumChoices*2
 * @param WarpSize size of warp (NVIDIA usually 32) (AMD ussually 64)
 *
 * @return returns number of possible choices - NumChoices
 */
unsigned TileSizeChoises(
		unsigned**Choices,
		unsigned WarpSize){
	unsigned NumChoices=0;//number of possible choices
	for(unsigned x=1;x<=WarpSize;++x)//loop over all choices
		if((WarpSize%x)==0)//is this choice of x possible?
			NumChoices++;//it is -> increment number of choices
	*Choices=new unsigned[NumChoices*2];//allocate array of choices
	NumChoices=0;//set choice iterator to 0
	for(unsigned x=1;x<=WarpSize;++x)//loop over all choices
		if((WarpSize%x)==0){//is this choice of x possible?
			(*Choices)[NumChoices++]=x;//write size x
			(*Choices)[NumChoices++]=WarpSize/x;//write size y
		}
	return NumChoices/2;//we incremented number of choices twice per possible choice
}

unsigned DivRoundUp(unsigned X,unsigned Y){
	return(X/Y)+(X%Y==0?0:1);
}

unsigned DivRoundDown(unsigned X,unsigned Y){
	return(X/Y);
}

unsigned ComputeIdleInvocation(
		unsigned*Active,
		unsigned NumLevels,
		unsigned*TileChoices,
		unsigned*SolutionIndex,
		unsigned*WindowSize){
	unsigned*TileSize=new unsigned[NumLevels*2];
	for(int l=NumLevels-1;l>=0;--l){
		TileSize[l*2+0]=1;//TileChoices[SolutionIndex[NumLevels-1]*2+0];
		TileSize[l*2+1]=1;//TileChoices[SolutionIndex[NumLevels-1]*2+1];
	}
	for(unsigned k=1;k<NumLevels;++k)
		for(unsigned l=0;l<k;++l){
			TileSize[l*2+0]*=TileChoices[SolutionIndex[k]*2+0];
			TileSize[l*2+1]*=TileChoices[SolutionIndex[k]*2+1];
		}

	unsigned NumInvocations=0;
	unsigned BrachingFactor=TileChoices[SolutionIndex[0]*2+0]*TileChoices[SolutionIndex[0]*2+1];
	//BF+BF*BF+BF*BF*BF+BF*BF*BF*BF;
	for(unsigned l=0;l<NumLevels;++l){
		NumInvocations+=1;
		NumInvocations*=BrachingFactor;
	}
	unsigned Idle=0;
	*Active=0;

	for(unsigned l=0;l<NumLevels;++l){
		unsigned PrevTileSize[2];
		if(l>0){
			PrevTileSize[0]=TileSize[(l-1)*2+0];
			PrevTileSize[1]=TileSize[(l-1)*2+1];
		}else{
			PrevTileSize[0]=WindowSize[0];
			PrevTileSize[1]=WindowSize[1];
		}
		/*
		std::cerr<<"  G:"<<DivRoundUp(WindowSize[0],PrevTileSize[0])*DivRoundUp(WindowSize[1],PrevTileSize[1])*BrachingFactor-
		DivRoundUp(WindowSize[0],TileSize[l*2+0])*DivRoundUp(WindowSize[1],TileSize[l*2+1])<<" "<<
		DivRoundUp(WindowSize[0],PrevTileSize[0])<<" "<<DivRoundUp(WindowSize[1],PrevTileSize[1])<<" "<<
		DivRoundUp(WindowSize[0],TileSize[l*2+0])<<" "<<DivRoundUp(WindowSize[1],TileSize[l*2+1])
		<<"::: "<<WindowSize[1]<<" -->> "<<TileSize[l*2+1]<<std::endl;
		*/
		Idle+=DivRoundUp(WindowSize[0],PrevTileSize[0])*DivRoundUp(WindowSize[1],PrevTileSize[1])*BrachingFactor-
		DivRoundUp(WindowSize[0],TileSize[l*2+0])*DivRoundUp(WindowSize[1],TileSize[l*2+1]);

		*Active+=
			(DivRoundUp(WindowSize[0],PrevTileSize[0])*DivRoundUp(WindowSize[1],PrevTileSize[1])*BrachingFactor-
			DivRoundDown(WindowSize[0],PrevTileSize[0])*DivRoundDown(WindowSize[1],PrevTileSize[1])*BrachingFactor)
			-(DivRoundUp(WindowSize[0],PrevTileSize[0])*DivRoundUp(WindowSize[1],PrevTileSize[1])*BrachingFactor-
			DivRoundUp(WindowSize[0],TileSize[l*2+0])*DivRoundUp(WindowSize[1],TileSize[l*2+1]));

	}

	//for(unsigned l=0;l<NumLevels;++l)
	//	std::cerr<<" "<<TileSize[l*2+0]<<"x"<<TileSize[l*2+1];
	//std::cerr<<" idle:    "<<Idle<<" active: "<<*Active;
	//std::cerr<<std::endl;
	delete[]TileSize;
	return Idle;
}

unsigned*SortPtr;

int SortByIdleInvocation(const void*A,const void*B){
	unsigned*APtr=(unsigned*)A;
	unsigned*BPtr=(unsigned*)B;
	if(SortPtr[*APtr]<SortPtr[*BPtr])return-1;
	else if(SortPtr[*APtr]>SortPtr[*BPtr])return 1;
	return 0;
}


unsigned ChooseTileSizes(
		unsigned**TileSize,
		unsigned*WindowSize,
		unsigned WavefrontSize){
	unsigned*TileChoices;//choices of tile size
	unsigned NumTileChoices=TileSizeChoises(&TileChoices,WavefrontSize);//determine
	unsigned NumLevels=ceil(log(WindowSize[0]*WindowSize[1])/log(WavefrontSize));
	(*TileSize)=new unsigned[NumLevels*2];//allocate tile sizes

	unsigned NumSolutions=pow(NumTileChoices,NumLevels);//total number of sol.

	unsigned*SolutionIndex=new unsigned[NumSolutions*NumLevels];//ind. to choices
	unsigned*SolutionSizeReq=new unsigned[NumSolutions];//pass size requirement
	unsigned*SolutionIdleInvocations=new unsigned[NumSolutions];//num of idle inv.
	unsigned*SolutionActiveInvocations=new unsigned[NumSolutions];//active inv.
	unsigned NumSolutionSizeReq=0;

	unsigned II=0;//index into index
	unsigned*Index=new unsigned[NumLevels];//N-dimensional index
	for(unsigned i=0;i<NumLevels;++i)Index[i]=0;//cleat index

	unsigned Index1D=0;//1D version of index

	do{//loop over all solutions
		for(unsigned l=0;l<NumLevels;++l)//loop over levels
			SolutionIndex[Index1D*NumLevels+l]=Index[l];//write solution index
		
		//test size requirement
		unsigned Size[2]={1,1};
		for(unsigned l=0;l<NumLevels;++l)//loop over levels
			for(int k=0;k<2;++k)//loop over dimension of size
				Size[k]*=TileChoices[Index[l]*2+k];//compute size
		if(Size[0]>=WindowSize[0]&&Size[1]>=WindowSize[1]){//resolution is ok
			unsigned Active;
			SolutionIdleInvocations[NumSolutionSizeReq]=ComputeIdleInvocation(
						&Active,
						NumLevels,
						TileChoices,
						SolutionIndex+Index1D*NumLevels,
						WindowSize);
			SolutionActiveInvocations[NumSolutionSizeReq]=Active;
			SolutionSizeReq[NumSolutionSizeReq]=Index1D;
			++NumSolutionSizeReq;
		}

		++Index1D;//increment 1D version of index
		//increment index to solutions
		II=0;
		do{//loop over levels
			++Index[II];//increment index in II level
			if(Index[II]>=NumTileChoices){//index at II level overflows
				Index[II]=0;//clear index in II level
				++II;//increment level
			}else break;//we are done incrementing
		}while(II<NumLevels);
	}while(II<NumLevels);

	//sort solutions by number of idle invocations
	unsigned*IndexToSolution=new unsigned	[NumSolutionSizeReq];
	for(unsigned i=0;i<NumSolutionSizeReq;++i)IndexToSolution[i]=i;
	SortPtr=SolutionIdleInvocations;
	qsort(IndexToSolution,NumSolutionSizeReq,sizeof(unsigned),SortByIdleInvocation);

	//compute the count of fewest idle solutions
	unsigned MinIdle=SolutionIdleInvocations[IndexToSolution[0]];
	unsigned NumFewestIdle=0;
	for(unsigned i=0;i<NumSolutionSizeReq;++i)
		if(MinIdle!=SolutionIdleInvocations[IndexToSolution[i]])break;
		else NumFewestIdle++;

	unsigned*FewestIdle=new unsigned[NumFewestIdle];
	for(unsigned i=0;i<NumFewestIdle;++i)
		FewestIdle[i]=SolutionSizeReq[IndexToSolution[i]];

	unsigned*Squareness=new unsigned[NumFewestIdle];
	for(unsigned i=0;i<NumFewestIdle;++i){
		Squareness[i]=0;
		unsigned ActX=1;
		unsigned ActY=1;
		for(int l=NumLevels-1;l>=0;--l){
			unsigned SolIndex=SolutionIndex[FewestIdle[i]*NumLevels+l];
			ActX*=TileChoices[SolIndex*2+0];//width of tile
			ActY*=TileChoices[SolIndex*2+1];//height of tile
			Squareness[i]*=2;
			if(ActX<ActY)Squareness[i]+=ActY/ActX;
			else Squareness[i]+=ActX/ActY;//squareness of a level
		}
	}

	unsigned*IndexToFewestIdle=new unsigned[NumFewestIdle];
	for(unsigned i=0;i<NumFewestIdle;++i)
		IndexToFewestIdle[i]=i;
	SortPtr=Squareness;
	qsort(IndexToFewestIdle,NumFewestIdle,sizeof(unsigned),SortByIdleInvocation);

	for(unsigned l=0;l<NumLevels;++l){
		unsigned SolIndex=SolutionIndex[FewestIdle[IndexToFewestIdle[0]]*NumLevels+l];
		(*TileSize)[l*2+0]=TileChoices[SolIndex*2+0];
		(*TileSize)[l*2+1]=TileChoices[SolIndex*2+1];
	}

	delete[]IndexToFewestIdle;
	delete[]Squareness;
	delete[]FewestIdle;
	delete[]SolutionActiveInvocations;
	delete[]SolutionIdleInvocations;
	delete[]IndexToSolution;
	delete[]SolutionSizeReq;
	delete[]Index;
	delete[]SolutionIndex;
	delete[]TileChoices;//free tile size choices
	return NumLevels;//return number of required levels
}

