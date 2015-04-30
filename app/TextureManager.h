#pragma once

#include<geGL/geGL.h>
#include<map>
#include<vector>
#include<deque>

namespace ge{
  namespace gl{
    class UTexture{
      public:
        virtual unsigned long long getSize()=0;
        virtual ~UTexture();

    };
    class UniversalTexture: public UTexture{
      protected:
        std::string _name;
        GLenum  _target;
        GLsizei _width;
        GLsizei _height;
        GLsizei _depth;
        GLenum  _format;
        GLenum  _type;
        GLvoid* _data;
      public:
        UniversalTexture(std::string name);
        ~UniversalTexture();
        GLenum  getTarget();
        GLsizei getWidth ();
        GLsizei getHeight();
        GLsizei getDepth ();
        GLenum  getFormat();
        GLenum  getType  ();
        GLvoid* getData  ();
        void setTarget(GLenum  target);
        void setWidth (GLsizei width );
        void setHeight(GLsizei height);
        void setDepth (GLsizei depth );
        void setFormat(GLenum  format);
        void setType  (GLenum  type  );
        void setData  (GLvoid* data  );
        unsigned long long getSize();
    };

    class CpuTextureCache;
    class CachedTexture: public UTexture{
      protected:
        CpuTextureCache*_cache;
        UTexture*       _texture;
      public:
        CachedTexture(UTexture*texture,CpuTextureCache*cache);
        unsigned long long getSize();
    };

    class CacheOwner{
      protected:
        std::vector<std::string>_freed;
      public:
        void insertFreed(std::string name);
        void processFreed();
    };
    class Cache{
      protected:
        class CacheElement{
          public:
            unsigned long long size;
            unsigned long long timeStamp;
            std::string name;
            CacheElement(std::string name,unsigned long long size,unsigned long long timeStamp);
        };
        unsigned long long       _size;
        unsigned long long       _used;
        std::deque<CacheElement> _queue;
        std::map<std::string,unsigned>_name2queue;
        CacheOwner* _owner;
        bool _insert(
            std::vector<std::string>&toFree,
            std::string name,
            unsigned long long size,
            unsigned long long timeStamp);
      public:
        Cache(unsigned long long size,CacheOwner*owner=NULL);
        void update(std::string name,unsigned long long timeStamp);
        void setOwner(CacheOwner*owner);
    };

    class CpuTextureCache: public Cache{
      protected:
        std::map<std::string,UTexture*>_cache;
      public:
        CpuTextureCache(unsigned long long size,CacheOwner*owner=NULL);
        bool insert(std::string name,UTexture*texture,unsigned long long timeStamp);
        UTexture*getTexture(std::string name);
    };

    class GpuTextureCache: public Cache{
      protected:
        std::map<std::string,ge::gl::TextureObject*>_cache;
      public:
        GpuTextureCache(unsigned long long size,CacheOwner*owner=NULL);
        bool insert(std::string name,ge::gl::TextureObject*texture,unsigned long long timeStamp);
        ge::gl::TextureObject*getTexture(std::string name);
    };

    class TextureLoader{
      private:
      public:
        TextureLoader();
        ~TextureLoader();
        virtual bool test(std::string file)=0;
        virtual UniversalTexture*read(std::string file)=0;
    };

    class TextureLoaders{
      protected:
        std::vector<TextureLoader*>_loaders;
      public:
        void addLoader(TextureLoader*loader);
        UniversalTexture*read(std::string file);
    };

    class DevilTextureLoader: public TextureLoader{
      public:
        DevilTextureLoader();
        ~DevilTextureLoader();
        UniversalTexture*read(std::string file);
        bool test(std::string file);
    };

    class TextureFactory: public CacheOwner{
      protected:
        TextureLoaders* _loaders;
        CpuTextureCache*_cpuCache;
        GpuTextureCache*_gpuCache;
      public:
        TextureFactory(TextureLoaders*loaders,CpuTextureCache*cpuCache=NULL,GpuTextureCache*gpuCache=NULL);
        void setCpuCache(CpuTextureCache*cache=NULL);
        void setGpuCache(GpuTextureCache*cache=NULL);
        ge::gl::TextureObject*getTexture(std::string name);
    };


    class TextureManager{
      protected:
        std::map<std::string,ge::gl::TextureObject*>_textures;
        std::map<std::string,GLuint64>              _handles;
      public:
        TextureManager();
        ~TextureManager();
        GLuint64 addTexture(std::string name,ge::gl::TextureObject*texture);
    };
  }
}
