#ifndef _ARGUMENTLOADER_H_
#define _ARGUMENTLOADER_H_

#include<iostream>
#include<vector>
#include<map>

namespace ge{
  namespace util{
    class ArgumentLoader{
      private:
        std::vector<std::string>_variables;
        std::vector<std::string>_data;
        bool _isNumber(std::string name);
        std::string _loadArgumentsFromFile(std::string fileName);
        void _tokenize(std::vector<std::string>&tokens,std::string file);
        bool ENTRY    (std::vector<std::string>tokens,unsigned*actToken,std::string prefix);
        bool NUMARRAY (std::string*data,std::vector<std::string>tokens,unsigned*actToken);
        bool STRDATA  (std::string*data,std::vector<std::string>tokens,unsigned*actToken);
        void ARGUMENTS(std::vector<std::string>tokens,unsigned*actToken,std::string prefix);
      public:
        ArgumentLoader(int argc,char*argv[]);
        unsigned getNumVariables();
        std::string getVariable(unsigned i);
        std::string getData    (unsigned i);
    };
  }
}

#endif//_ARGUMENTLOADER_H_
