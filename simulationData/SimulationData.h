#ifndef _SIMULATIONDATA_H_
#define _SIMULATIONDATA_H_

#include"StandardTypes.h"

namespace simulation{

  class Namespace
  {
    private:
      std::string _name = "";
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

      Namespace*_rootNamespace = nullptr;
      bool _isNumber(std::string data);
      bool _isFloat(std::string name);
    public:
      SimulationData();
      void insertVariable(std::string name,Data*data);
      void insertVariable(std::string name,std::string data);
      std::string toStr();
      std::string getKeys();
      simulation::Data*get(std::string variableName,simulation::Data*def);
      bool        getBool    (std::string variableName,bool        def = false         );
      int         getInt     (std::string variableName,int         def = 0             );
      glm::ivec2  getIVec2   (std::string variableName,glm::ivec2  def = glm::ivec2(0) );
      glm::ivec3  getIVec3   (std::string variableName,glm::ivec3  def = glm::ivec3(0) );
      glm::ivec4  getIVec4   (std::string variableName,glm::ivec4  def = glm::ivec4(0) );
      unsigned    getUint    (std::string variableName,unsigned    def = 0u            );
      glm::uvec2  getUVec2   (std::string variableName,glm::uvec2  def = glm::uvec2(0u));
      glm::uvec3  getUVec3   (std::string variableName,glm::uvec3  def = glm::uvec3(0u));
      glm::uvec4  getUVec4   (std::string variableName,glm::uvec4  def = glm::uvec4(0u));
      float       getFloat   (std::string variableName,float       def = 0.0f          );
      glm::vec2   getVec2    (std::string variableName,glm::vec2   def = glm::vec2(0.f));
      glm::vec3   getVec3    (std::string variableName,glm::vec3   def = glm::vec3(0.f));
      glm::vec4   getVec4    (std::string variableName,glm::vec4   def = glm::vec4(0.f));
      std::string getString  (std::string variableName,std::string def = ""            );
      glm::mat4   getMat4    (std::string variableName,glm::mat4   def = glm::mat4(1.f));
      void*       getObject  (std::string variableName,void*       def = NULL          );
      Light*      getLight   (std::string variableName,Light*      def = NULL          );
      Gauge*      getGauge   (std::string variableName,Gauge*      def = NULL          );
      GpuGauge*   getGpuGauge(std::string variableName,GpuGauge*   def = NULL          );
      std::string define   (std::string prefix="");
      void        sendUpdate();
      void        registerUser(SimulationObject*user);
      void        unregisterUser(SimulationObject*user);
      void        setAsChanged(std::string name);
  };

  class SimulationObject{
    protected:
      SimulationData*_simulationData = nullptr;
      std::map<std::string,bool>_changed;
    public:
      void initChanged(std::string name);
      SimulationObject(SimulationData*data);
      virtual ~SimulationObject();
      virtual unsigned    getNofDependentVariables();
      virtual std::string getDependentVariable(unsigned dependentVariableId);
      void                variableChanged(std::string name);
      virtual void        update();
  };

}

#endif//_SIMULATIONDATA_H_
