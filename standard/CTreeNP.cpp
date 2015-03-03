#include"CTreeNP.hpp"

CTreeNP::CTreeNP(CTreeNP*P,unsigned ID,unsigned N){
	this->Prev=P;
	this->ChildID=ID;
	this->NumChild=N;
	if(this->NumChild)this->Child=new CTreeNP*[this->NumChild];
}

CTreeNP::~CTreeNP(){
	for(unsigned i=0;i<this->NumChild;++i)//projdeme potomky
		delete this->Child[i];//uvolnime je
	delete[]this->Child;//uvolnime pole pro potomky
}

unsigned CTreeNP::NumNode(){
	unsigned Result=1;
	for(unsigned i=0;i<this->NumChild;++i)
		Result+=this->Child[i]->NumNode();
	return Result;
}

CTreeNP*CTreeNP::GetParent(){
	return this->Prev;
}

unsigned CTreeNP::GetNumChild(){
	return this->NumChild;
}

unsigned CTreeNP::GetChildId(){
	return this->ChildID;
}

void CTreeNP::SetChildID(unsigned ID){
	this->ChildID=ID;
}

CTreeNP*CTreeNP::GetChild(unsigned ID){
	return this->Child[ID];
}

void CTreeNP::SetChild(unsigned I,CTreeNP*C){
	this->Child[I]=C;
}

