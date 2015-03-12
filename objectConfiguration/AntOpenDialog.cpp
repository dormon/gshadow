#include"AntOpenDialog.h"

#include<algorithm>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>

std::string AntOpenDialog::_getGroup(){
  return std::string("group=")+this->_group;
}

std::string AntOpenDialog::_getWithGroup(std::string what){
  return this->_getGroup()+" "+what;
}

void AntOpenDialog::setOpenCallBack(CALLBACK callback,void*data){
  this->_openCallBack=callback;
  this->_openCallBackData=data;
}

void AntOpenDialog::setSaveCallBack(CALLBACK callback,void*data){
  this->_saveCallBack=callback;
  this->_saveCallBackData=data;
}

void AntOpenDialog::_open(void*A){
  AntOpenDialog*_this=(AntOpenDialog*)A;
  std::string file=_this->_currentDir+_this->_dirContentNames[_this->_selectedFile];
  struct stat st;
  lstat(file.c_str(), &st);
  if(S_ISDIR(st.st_mode)){
    _this->_currentDir=file;
    _this->_loadDir(_this->_currentDir);
  }else
    if(_this->_openCallBack)
      _this->_openCallBack(file,_this->_openCallBackData);
}

void AntOpenDialog::_save(void*A){
  AntOpenDialog*_this=(AntOpenDialog*)A;
  if(_this->_currentFile=="")return;
  std::string file=_this->_currentDir+_this->_currentFile;
  if(_this->_saveCallBack)
    _this->_saveCallBack(file,_this->_saveCallBackData);
}

void AntOpenDialog::_up(void*A){
  AntOpenDialog*_this=(AntOpenDialog*)A;
  std::string dir=_this->_currentDir;
  std::size_t pos=dir.rfind("/");
  if(pos==std::string::npos)return;
  if(pos==dir.length()-1){
    if(dir.length()<2)return;
    pos=dir.rfind("/",dir.length()-2);
  }
  if(pos==std::string::npos)return;
  dir=dir.substr(0,pos+1);
  _this->_currentDir=dir;
  _this->_loadDir(_this->_currentDir);
}

void AntOpenDialog::_deleteDirContent(){
  this->_dirContentNames.clear();
  if(this->_dirContent)delete this->_dirContent;
  this->_dirContent=NULL;
}

void AntOpenDialog::_loadDir(std::string dir){
  std::vector<std::string>dirs;
  std::vector<std::string>files;
  this->_listDir(dirs,files,dir);

  this->_deleteDirContent();
  if(this->_listExists)TwRemoveVar(this->_bar,"list");

  this->_dirContent = new TwEnumVal[dirs.size()+files.size()];

  for(unsigned i=0;i<files.size();++i){
    this->_dirContentNames.push_back(files[i]);
    this->_dirContent[i].Value=i;
    this->_dirContent[i].Label=this->_dirContentNames[this->_dirContentNames.size()-1].c_str();
  }

  for(unsigned i=0;i<dirs.size();++i){
    this->_dirContentNames.push_back(dirs[i]);
    this->_dirContent[i+files.size()].Value=i+files.size();
    this->_dirContent[i+files.size()].Label=this->_dirContentNames[this->_dirContentNames.size()-1].c_str();
  }

  this->_selectedFile = 0;

  TwType DirContentType=TwDefineEnum("dircontent",this->_dirContent,this->_dirContentNames.size());

  TwAddVarRW(this->_bar,"list",DirContentType,&this->_selectedFile,
      this->_getWithGroup("help='list of files in current dirr' ").c_str());

  this->_listExists=true;
}

void AntOpenDialog::_listDir(std::vector<std::string>&dirs,std::vector<std::string>&files,std::string dirName){
  DIR *dir = opendir(dirName.c_str());
  if(!dir)return;
  struct dirent *ent;
  int Counter=-1;
  while((ent=readdir(dir))!=NULL){
    Counter++;
    if(this->_skipHidden)if(ent->d_name[0]=='.')continue;
    if(ent->d_type==DT_DIR)dirs.push_back(std::string(ent->d_name)+"/");
    else files.push_back(std::string(ent->d_name));
  }
  closedir(dir);
  std::sort(dirs.begin(),dirs.end());
  std::sort(files.begin(),files.end());
}

void TW_CALL AntOpenDialog::_cpyStdStringToClient(
    std::string&destinationClientString, 
    const std::string&sourceLibraryString){
  destinationClientString = sourceLibraryString;
}

AntOpenDialog::AntOpenDialog(TwBar*bar,std::string dir,std::string group){
  TwCopyStdStringToClientFunc(this->_cpyStdStringToClient);
  this->_listExists = false;
  this->_bar        = bar;
  this->_skipHidden = true;
  this->_currentDir = dir;
  this->_currentFile = "";
  this->_group      = group;
  this->_dirContent = NULL;
  this->_listExists = false;
  this->_openCallBack = NULL;
  this->_openCallBackData = NULL;
  this->_saveCallBack = NULL;
  this->_saveCallBackData = NULL;
  
  char buf[PATH_MAX+1];
  realpath(dir.c_str(),buf);
  this->_currentDir = std::string(buf)+"/";
  TwAddVarRW(this->_bar,"hidden",TW_TYPE_BOOLCPP,&this->_skipHidden,this->_getWithGroup("label='hide hidden'").c_str());
  TwAddButton(this->_bar,"open",this->_open,this,this->_getWithGroup("label='open'").c_str());
  TwAddButton(this->_bar,"save",this->_save,this,this->_getWithGroup("label='save'").c_str());
  TwAddButton(this->_bar,"up"  ,this->_up  ,this,this->_getWithGroup("label='up'").c_str());
  TwAddVarRO (this->_bar,"dir",TW_TYPE_STDSTRING,&this->_currentDir,this->_getWithGroup("label='dir'").c_str());
  TwAddVarRW(this->_bar,"savefile",TW_TYPE_STDSTRING,&this->_currentFile,this->_getWithGroup("label='save'").c_str());
  this->_loadDir(this->_currentDir);
}

AntOpenDialog::~AntOpenDialog(){
  this->_deleteDirContent();
  TwRemoveVar(this->_bar,"hide");
  TwRemoveVar(this->_bar,"open");
  TwRemoveVar(this->_bar,"up");
  TwRemoveVar(this->_bar,"dir");
  TwRemoveVar(this->_bar,"list");
}


