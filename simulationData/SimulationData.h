#ifndef _SIMULATIONDATA_H_
#define _SIMULATIONDATA_H_

#include"StandardTypes.h"

namespace simulation{

  class Namespace
  {
    private:
      std::string _name;
      std::vector<std::string>_subNamespaceNames;
      std::vector<Namespace*> _subNamespaces;
      std::vector<std::string>_subVariableNames;
      std::vector<Data*>      _subVariables;
      std::map<std::string,Namespace*>_name2Namespace;
      std::map<std::string,Data*>     _name2Variable;
      std::string toUpper(std::string str);
    public:
      std::string define(std::string prefix);
      Namespace(std::string name);
      void insertNamespace(std::string name,Namespace*nmspace);
      void insertVariable (std::string name,Data*variable);
      void insert(std::string name,Data*variable);
      std::string toStr(unsigned indentation);
      Namespace*getNamespace(std::string name);
      Data*     getVariable (std::string name);
  };

  class SimulationObject;

  class VariableUsers{
    public:
      std::vector<unsigned>user;
  };

  class SimulationData
  {
    private:
      std::map<std::string,Data*>_variable;
      std::map<std::string,Data*>_lastVariable;
      std::vector<SimulationObject*>_users;
      std::vector<bool>_sendUpdate;
      std::map<std::string,VariableUsers*>_variableUser;

      Namespace*_rootNamespace;
      bool _isNumber(std::string data);
      bool _isFloat(std::string name);
    public:
      SimulationData();
      void insertVariable(std::string name,Data*data);
      void insertVariable(std::string name,std::string data);
      std::string toStr();
      std::string getKeys();
      bool        getBool  (std::string variableName,bool        def);
      int         getInt   (std::string variableName,int         def);
      glm::ivec2  getIVec2 (std::string variableName,glm::ivec2  def);
      glm::ivec3  getIVec3 (std::string variableName,glm::ivec3  def);
      glm::ivec4  getIVec4 (std::string variableName,glm::ivec4  def);
      unsigned    getUint  (std::string variableName,unsigned    def);
      glm::uvec2  getUVec2 (std::string variableName,glm::uvec2  def);
      glm::uvec3  getUVec3 (std::string variableName,glm::uvec3  def);
      glm::uvec4  getUVec4 (std::string variableName,glm::uvec4  def);
      float       getFloat (std::string variableName,float       def);
      glm::vec2   getVec2  (std::string variableName,glm::vec2   def);
      glm::vec3   getVec3  (std::string variableName,glm::vec3   def);
      glm::vec4   getVec4  (std::string variableName,glm::vec4   def);
      std::string getString(std::string variableName,std::string def);
      glm::mat4   getMat4  (std::string variableName,glm::mat4   def);
      void*       getObject(std::string variableName,void*       def);
      Light*      getLight (std::string variableName,Light*      def);
      Gauge*      getGauge (std::string variableName,Gauge*      def);
      GpuGauge*   getGpuGauge(std::string variableName,GpuGauge* def);
      std::string define   (std::string prefix="");
      void        sendUpdate();
      void        registerUser(SimulationObject*user);
      void        unregisterUser(SimulationObject*user);
      void        setAsChanged(std::string name);
  };

  class SimulationObject{
    protected:
      SimulationData*_simulationData;
      std::map<std::string,bool>_changed;
    public:
      void initChanged(std::string name);
      SimulationObject(SimulationData*data);
      virtual ~SimulationObject();
      virtual unsigned    getNofDependentVariables()=0;
      virtual std::string getDependentVariable(unsigned dependentVariableId)=0;
      void                variableChanged(std::string name);
      virtual void        update()=0;
  };

}

#endif//_SIMULATIONDATA_H_
