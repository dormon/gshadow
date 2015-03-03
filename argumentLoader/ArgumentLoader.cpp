#include"ArgumentLoader.h"

#include<fstream>
#include<cctype>
#include<locale>

using namespace ge::util;

bool ArgumentLoader::_isNumber(std::string name){
  if(name=="")return false;
  return 
    std::isdigit(name[0])||
    name[0]=='+'||
    name[0]=='-'||
    name.find(".")  !=std::string::npos||
    name.find("inf")!=std::string::npos||
    name.find("nan")!=std::string::npos;
}


std::string ArgumentLoader::_loadArgumentsFromFile(std::string fileName){
  std::ifstream f(fileName.data());//otevreme soubor
  if(!f.good())throw std::string("file "+fileName+" is corrupt");
  f.seekg(0,std::ios::end);//jdeme na konec
  unsigned len=f.tellg();//tak tu mame delku (pozice hlavy)
  f.seekg(0,std::ios::beg);//navrat na zacatek
  unsigned char*data=new unsigned char[len+1];//alokace dat
  f.read((char*)data,len);//precteni souboru
  f.close();//zavreme soubor
  data[len]='\0';
  std::string strData((const char*)data);
  //if(data[len]=='\0')std::cerr<<"jo je konec"<<std::endl;
  //else std::cerr<<"neni konec"<<std::endl;
  delete[]data;
  return strData;
}

void ArgumentLoader::_tokenize(std::vector<std::string>&tokens,std::string file){
  enum{
    STATE_S,
    STATE_E,
    STATE_W,
    STATE_C,
    STATE_ESC
  }actState=STATE_S;
  std::string actWord="";
  unsigned pos=0;
  while(pos<file.size()){
    char ch=file[pos++];
    switch(actState){
      case STATE_S:
        if(std::isspace(ch)||ch=='\n'||ch=='\r')actState=STATE_E;
        else{actWord+=ch;actState=STATE_W;}
        break;
      case STATE_E:
        if(std::isspace(ch)||ch=='\n'||ch=='\r')actState=STATE_E;
        else{actWord+=ch;actState=STATE_W;}
        break;
      case STATE_W:
        if(ch=='\\')actState=STATE_ESC;
        else if(ch=='#')actState=STATE_C;
        else if(std::isspace(ch)||ch=='\n'||ch=='\r'){tokens.push_back(actWord);actWord="";actState=STATE_S;}
        else actWord+=ch;
        break;
      case STATE_C:
        if(ch=='\n'||ch=='\r')actState=STATE_S;
        break;
      case STATE_ESC:
        actWord+=ch;
        actState=STATE_W;
        break;
    }
  }
  if(actWord!="")tokens.push_back(actWord);

  //for(unsigned i=0;i<tokens.size();++i)
  //  std::cerr<<"."<<tokens[i]<<"."<<std::endl;

/*
  std::size_t pos;
  while((pos=file.find(" "))!=std::string::npos){
    //std::cerr<<"."<<file.substr(0,pos)<<"."<<std::endl;
    tokens.push_back(file.substr(0,pos));
    file=file.substr(pos+1);
  }
  if(file!="")tokens.push_back(file);
  */
}

void ArgumentLoader::ARGUMENTS(
    std::vector<std::string>tokens,
    unsigned*actToken,
    std::string prefix){
  if(*actToken>=tokens.size())return;
  bool cont=this->ENTRY(tokens,actToken,prefix);
  if(cont)this->ARGUMENTS(tokens,actToken,prefix);
}

bool ArgumentLoader::NUMARRAY(
    std::string*data,
    std::vector<std::string>tokens,
    unsigned*actToken){
  unsigned start=*actToken;
  while(*actToken<tokens.size()&&this->_isNumber(tokens[*actToken])){
    *data+=tokens[*actToken]+" ";
    (*actToken)++;
  };
  if(*data==""){*actToken=start;return false;}
  *data=data->substr(0,data->size()-1);
  return true;
}
bool ArgumentLoader::STRDATA(
    std::string*data,
    std::vector<std::string>tokens,
    unsigned*actToken){
  unsigned start=*actToken;
  if(*actToken>=tokens.size())return false;
  std::string d=tokens[(*actToken)++];
  if(this->_isNumber(d)||d==""||d[0]=='{'){*actToken=start;return false;}
  *data=d;
  return true;
}


bool ArgumentLoader::ENTRY(
    std::vector<std::string>tokens,
    unsigned*actToken,
    std::string prefix){
  if(*actToken>=tokens.size())return false;
  std::string name=tokens[(*actToken)++];
  //std::cerr<<"zacatek: "<<name<<" "<<*actToken<<std::endl;

  if(name=="}"){(*actToken)--;return false;}
  if(name=="<"){
    //std::cerr<<"load file"<<std::endl;
    if(*actToken>=tokens.size())throw std::string("expected filename after < not EOF");
    std::vector<std::string>newTokens;
    unsigned newActToken=0;
    std::string fileName=tokens[(*actToken)++];
    std::string loadedFile=this->_loadArgumentsFromFile(fileName);
    //std::cerr<<"#"<<loadedFile<<"#"<<std::endl;
    this->_tokenize(newTokens,loadedFile);
    this->ARGUMENTS(newTokens,&newActToken,prefix);
    //std::cerr<<"kokonenec"<<std::endl;
    return true;
  }
  std::string data;
  if(this->STRDATA(&data,tokens,actToken)){
    if(prefix=="")this->_variables.push_back(name);
    else this->_variables.push_back(prefix+"."+name);
    this->_data.push_back(data);
    return true;
  }
  if(this->NUMARRAY(&data,tokens,actToken)){
    if(prefix=="")this->_variables.push_back(name);
    else this->_variables.push_back(prefix+"."+name);
    this->_data.push_back(data);
    return true;
  }
  //std::cerr<<"konec: "<<*actToken<<std::endl;

  if(*actToken+1>=tokens.size())throw std::string("expected { } not EOF 0");
  std::string start=tokens[(*actToken)++];
  if(start!="{")throw std::string("expected { not "+start);
  if(prefix=="")this->ARGUMENTS(tokens,actToken,name);
  else this->ARGUMENTS(tokens,actToken,prefix+"."+name);
  if(*actToken>=tokens.size())throw std::string("expected } not EOF 1");
  std::string end=tokens[(*actToken)++];
  if(end!="}")throw std::string("expected } not "+end);
  return true;
}

ArgumentLoader::ArgumentLoader(int argc,char*argv[]){
  //1. iteration
  //<ARGUMENTS> ::= <ENTRY>
  //<ARGUMENTS> ::= <ENTRY> <ARGUMENTS>
  //<ENTRY> ::= <NAME> <STRDATA>
  //<ENTRY> ::= <NAME> <NUMARRAY>
  //<ENTRY> ::= "<" <STRDATA>
  //<ENTRY> ::= <NAME> "{" <ARGUMENTS> "}"
  //<NUMARRAY> ::= <NUMDATA>
  //<NUMARRAY> ::= <NUMDATA> <NUMARRAY>
  //<NAME> ::= std::string
  //<STRDATA> ::= std::string
  //<NUMDATA> ::= float|int

  std::vector<std::string>tokens;
  for(int t=1;t<argc;++t){
    tokens.push_back(std::string(argv[t]));
    //std::cerr<<argv[t]<<std::endl;
  }

  unsigned actToken=0;
  this->ARGUMENTS(tokens,&actToken,"");
}


unsigned ArgumentLoader::getNumVariables(){
  return this->_variables.size();
}
std::string ArgumentLoader::getVariable(unsigned i){
  return this->_variables[i];
}
std::string ArgumentLoader::getData(unsigned i){
  return this->_data[i];
}

