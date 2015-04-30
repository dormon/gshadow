#include"TextureManager.h"

using namespace ge::gl;

void CacheOwner::insertFreed(std::string name){
  this->_freed.push_back(name);
}

Cache::Cache(unsigned long long size,CacheOwner*owner){
  this->_size = size;
  this->_used = 0;
  this->_owner = owner;
}

Cache::CacheElement::CacheElement(
    std::string name,
    unsigned long long size,
    unsigned long long timeStamp){
  this->name      = name;
  this->size      = size;
  this->timeStamp = timeStamp;
}

bool Cache::_insert(
    std::vector<std::string>&toFree,
    std::string name,
    unsigned long long size,
    unsigned long long timeStamp){
  if(size>=this->_size)return false;
  if(this->_queue.end()->timeStamp>timeStamp)return false;

  while(size>this->_size-this->_used){
    this->_used-=this->_queue.front().size;
    toFree.push_back(this->_queue.front().name);
    if(this->_owner)this->_owner->insertFreed(this->_queue.front().name);
    this->_queue.pop_front();
    this->_name2queue.erase(name);
  }

  this->_queue.push_back(CacheElement(name,size,timeStamp));
  this->_name2queue.insert(std::pair<std::string,unsigned>(name,this->_queue.size()-1));
  this->_used+=size;
  return true;
}

void Cache::update(std::string name,unsigned long long timeStamp){
  if(!this->_name2queue.count(name))return;
  unsigned id=this->_name2queue[name];
  unsigned long long size=this->_queue[id].size;
  this->_queue.erase(this->_queue.begin()+id);
  this->_queue.push_back(CacheElement(name,size,timeStamp));
}
void Cache::setOwner(CacheOwner*owner){
  this->_owner=owner;
}

CpuTextureCache::CpuTextureCache(unsigned long long size,CacheOwner*owner):Cache(size,owner){}

bool CpuTextureCache::insert(std::string name,UTexture*texture,unsigned long long timeStamp){
  std::vector<std::string>toFree;
  if(!this->_insert(toFree,name,texture->getSize(),timeStamp))return false;
  for(unsigned i=0;i<toFree.size();++i){
    delete this->_cache[toFree[i]];
    this->_cache.erase(toFree[i]);
  }
  return true;
}

UTexture*CpuTextureCache::getTexture(std::string name){
  if(!this->_cache.count(name))return NULL;
  return this->_cache[name];
}

GpuTextureCache::GpuTextureCache(unsigned long long size,CacheOwner*owner):Cache(size,owner){}

bool GpuTextureCache::insert(std::string name,ge::gl::TextureObject*texture,unsigned long long timeStamp){
  std::vector<std::string>toFree;
  if(!this->_insert(toFree,name,10/*texture->getSize()*/,timeStamp))return false;
  for(unsigned i=0;i<toFree.size();++i){
    delete this->_cache[toFree[i]];
    this->_cache.erase(toFree[i]);
  }
  return true;
}

ge::gl::TextureObject*GpuTextureCache::getTexture(std::string name){
  if(!this->_cache.count(name))return NULL;
  return this->_cache[name];
}

void TextureLoaders::addLoader(TextureLoader*loader){
  this->_loaders.push_back(loader);
}

UniversalTexture*TextureLoaders::read(std::string file){
  for(unsigned i=0;i<this->_loaders.size();++i){
    if(this->_loaders[i]->test(file))
      return this->_loaders[i]->read(file);
  }
  return NULL;
}

TextureFactory::TextureFactory(
    TextureLoaders*loaders,
    CpuTextureCache*cpuCache,
    GpuTextureCache*gpuCache){
  this->_loaders=loaders;
  this->_cpuCache=cpuCache;
  this->_gpuCache=gpuCache;
}

void TextureFactory::setCpuCache(CpuTextureCache*cache){
  this->_cpuCache = cache;
  if(this->_cpuCache)this->_cpuCache->setOwner(this);
}
void TextureFactory::setGpuCache(GpuTextureCache*cache){
  this->_gpuCache = cache;
  if(this->_gpuCache)this->_gpuCache->setOwner(this);
}

ge::gl::TextureObject*TextureFactory::getTexture(std::string name){
  return NULL;
}
