#pragma once

#include<AntTweakBar.h>
#include<vector>
#include<iostream>

class AntOpenDialog
{
  public:
    typedef void (*CALLBACK)(std::string fileName,void*data);
  private:
    std::string             _ownerName      ;
    std::string             _currentDir     ;
    std::string             _currentFile    ;
    TwBar*                  _bar            ;
    TwEnumVal*              _dirContent     ;
    std::vector<std::string>_dirContentNames;
    unsigned                _selectedFile   ;
    bool                    _skipHidden     ;
    std::string             _group          ;
    std::string             _getGroup();
    std::string             _getWithGroup(std::string what);
    bool                    _listExists;
    CALLBACK                _openCallBack;
    void*                   _openCallBackData;
    CALLBACK                _saveCallBack;
    void*                   _saveCallBackData;
    static void _open(void*A);
    static void _save(void*A);
    static void _up(void*A);
    static void TW_CALL _cpyStdStringToClient(std::string&destinationClientString,const std::string&sourceLibraryString);
    void _loadDir(std::string dir);
    void _deleteDirContent();
    void _listDir(std::vector<std::string>&dirs,std::vector<std::string>&files,std::string dirName);
  public:
    AntOpenDialog(TwBar*bar,std::string ownerName,std::string dir="/home/dormon/git/gshadowzal/gshadow/",std::string group="files");
    void setOpenCallBack(CALLBACK callback,void*data=NULL);
    void setSaveCallBack(CALLBACK callback,void*data=NULL);
    ~AntOpenDialog();
};


