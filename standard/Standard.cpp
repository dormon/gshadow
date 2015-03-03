#include"Standard.hpp"
#include<iostream>

int FileTypeSwitch(std::string FileName,unsigned NumType,...){
	va_list args;//argumenty
	va_start(args,NumType);//zacatek argumentu
	for(size_t i=0;i<NumType;++i){
		char* ext=(char*)va_arg(args,char*);
		if(FileName.length()-FileName.rfind(ext)==strlen(ext)){//nalezl se
			va_end(args);//konec sekvence argumentu
			return i;//vraceni cisla
		}
	}
	va_end(args);//konec sekcence
	return-1;//ani jeden
}

unsigned char*ReadWholeFile(int*Len,std::string File){
	std::ifstream f(File.data(),std::ifstream::binary);//otevreme soubor
	if(!f.good())throw std::string("File "+File+" is corrupt");
	f.seekg(0,std::ios::end);//jdeme na konec
	*Len=f.tellg();//tak tu mame delku (pozice hlavy)
	f.seekg(0,std::ios::beg);//navrat na zacatek
	unsigned char*Data=new unsigned char[*Len];//alokace dat
	f.read((char*)Data,*Len);//precteni souboru
	f.close();//zavreme soubor
	return Data;
}

unsigned UTF8NumByte(char Char){
	if(Char&0x80&&Char&0x40){
		if(Char&0x20){
			if(Char&0x10){
				if(Char&0x08){
					if(Char&0x04)return 6;
					else return 5;}
				else return 4;}
			else return 3;}
		else return 2;}
	else return 1;//neni to tozsireny znak
}

unsigned UTF8Lenght(std::string Text){
	//	setlocale(LC_CTYPE,"");
	//	return mbstowcs(NULL,Text.data(),0);

	unsigned Result=0;//vysledek
	unsigned ia=0;//index v ASCII
	while(ia<Text.length()){//dokud jsou ASCII znaky
		ia+=UTF8NumByte(Text[ia]);//posuneme se o pocet byte
		++Result;//zvysime pocitadlo znaku
	}
	return Result;
}

std::string UTF8GetChar(std::string Text,unsigned Index){
	unsigned ia=0;//index v ASCII
	unsigned iu=0;//index v UTF-8
	while(iu<Index){//dokud jsme nedorazili na pozadovany znak
		ia+=UTF8NumByte(Text[ia]);//posuneme se o pocet byte
		++iu;//zvysime pocitadlo znaku v UTF8
	}
	return Text.substr(ia,UTF8NumByte(Text[ia]));
}

int UTF8CharCmp(std::string Char0,std::string Char1){
	return strcoll(Char0.data(),Char1.data());
}

std::string UTF8EscapeSeq(std::string Text,unsigned*Len){
	if(Text.empty())return "";
	std::string Res;//vystupni znak
	*Len=0;//zatim se nic neprecetlo
	if(Text[*Len]=='\\'){//je zde sekvence
		if(Text.length()==1)throw std::string("The character \\ is alone");
		(*Len)++;//posunem se o znak sekvence
		if(	Text.length()>2&&//kontrola velikosti pro hexa
				Text[*Len]=='0'&&//hexa zacina 0
				(Text[*Len+1]=='x'||Text[*Len+1]=='X')){//nasledovano x nebo X 0xff
			(*Len)+=2;//posuneme se na hexa kod
			unsigned C;//zpacovavany dvojice hexa znaku
			unsigned NumC;//pocet dvojic hexa znaku v sekvenci
			sscanf(Text.data()+*Len,"%2x",&C);//precteme prvni dvojici
			//cout<<hex<<C<<endl;
			NumC=UTF8NumByte(C)-1;//zjistime kolik dvojic tady ještě je
			//cout<<NumC<<endl;
			Res+=(char)C;//pripojime znak
			(*Len)+=2;//posuneme ukazatel
			for(unsigned i=0;i<NumC;++i){
				sscanf(Text.data()+*Len,"%2x",&C);//precteme prvni dvojici
				Res+=(char)C;//pripojime znak
				(*Len)+=2;//posuneme ukazatel
			}
		}else{//neni to hexa
			Res=UTF8GetChar(Text,1);//precteme nasledujici znak
			*Len+=Res.length();//pripocteme velikost
		}
	}else{//zadna sekvence
		Res=UTF8GetChar(Text,0);//prvni znak
		*Len=Res.length();//jeho delka
	}
	return Res;
}


std::string IntToStr(int c){
	char B[256];
	sprintf(B,"%i",c);
	return std::string(B);
}

std::string ULLToStr(unsigned long long c){
	char B[256];
	sprintf(B,"%llu",c);
	return std::string(B);
}

