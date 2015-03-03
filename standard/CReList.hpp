#ifndef _CRELIST_HPP_
#define _CRELIST_HPP_

#include"Standard.hpp"

template<class T>
class CReList{
	private:
		T*Data;///data
		unsigned Used;//pocet pouzitych policek
		unsigned Size;//celkova velikost
		void Realloc(){//provede zvetseni
			if(this->Size==0){//prvni alokace
				this->Data=(T*)malloc(sizeof(T));
				this->Size=1;
			}else{
				T*Ptr=(T*)realloc(Data,sizeof(T)*this->Size*2);
				this->Data=Ptr;
				this->Size*=2;
			}
		}
	public:
		/**
		 * Konec
		 */
		CReList(){
			this->Data=NULL;
			this->Used=0;
			this->Size=0;
		}
		~CReList(){
			free(this->Data);
			this->Data=NULL;
			this->Used=0;
			this->Size=0;
		}
		/**
		 * Adresace
		 * @param I index
		 * @return vraci prvek
		 */
		T&operator[](unsigned I){
			return this->Data[I];
		}
		/**
		 * Vlozi prvek na konec pole
		 * @param Data data
		 * @return vraci index do pole kam se vlozil prvek
		 */
		unsigned Push(T Data){
			if(this->Used>=this->Size)Realloc();//je potreba zvetsi pole
			this->Data[this->Used++]=Data;//zapise data a zvysi pocitadlo
			return this->Used-1;//vraci index
		}
		/**
		 * Ziska pocet prvku v poli
		 */
		unsigned GetSize(){
			return this->Used;
		}
};

#endif//_CRELIST_HPP_
