#include"CList.hpp"

CList::CList(){
	this->Next=NULL;//nasledujici prvek
}

CList::~CList(){
	if(this->Next)delete this->Next;//zrusi svechny nasledujici
}

CList*CList::GetNext(){
	return this->Next;//vraci nasledujici
}

CList**CList::GetANext(){
	return &this->Next;//vraci adresu
}


void CList::Extend(CList*N){
	this->Next=N;//nastavi nasledujici
}
