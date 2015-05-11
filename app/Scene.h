#pragma once

#include<iostream>
#include<vector>
#include<glm/glm.hpp>

namespace ge{
  namespace db{
    class VertexAttribDataDescriptor{
      public:
        unsigned    components;
        enum Type{
          I8 ,
          I16,
          I32,
          I64,
          U8 ,
          U16,
          U32,
          U64,
          F32,
          F64
        }type;
        enum Semantic{
          POSITION,
          COORD   ,
          NORMAL  ,
          BINORMAL,
          TANGENT ,
          COLOR   
        }semantic;
        VertexAttribDataDescriptor(
            unsigned      components = 4       ,
            enum Type     type       = F32     ,
            enum Semantic semantic   = POSITION);
        unsigned size();
        std::string type2Str();
        std::string semantic2Str();
        std::string toStr();
    };

    class VertexAttrib{
      protected:
        const void*                _ptr           ;
        unsigned                   _stride        ;
        unsigned                   _offset        ;
        VertexAttribDataDescriptor _dataDescriptor;
      public:
        VertexAttrib(
            const void*ptr                               ,
            VertexAttribDataDescriptor dataDescriptor    ,
            unsigned   offset                         = 0,
            unsigned   stride                         = 0);
        const void*get(unsigned vertex);
        const void*getPtr();
        unsigned   getStride();
        unsigned   getOffset();
        VertexAttribDataDescriptor&getDataDescriptor();
        std::string toStr();
    };

    class MeshVertices{
      protected:
        std::vector<VertexAttrib>_attribs;
        unsigned                 _nofVertices;
      public:
        MeshVertices(unsigned nofVertices);
        unsigned   getNofAttribs();
        unsigned   addVertexAttrib(VertexAttrib&attrib);
        const void*getAttrib(unsigned index,unsigned vertex);
        unsigned   getAttribSize(unsigned index);
        std::string toStr();
    };

    class MeshPrimitives{
      public:
        enum Type{
          POINT                   ,
          LINE                    ,
          LINE_STRIP              ,
          LINE_LOOP               ,
          LINE_ADJACENCY          ,
          LINE_STRIP_ADJACENCY    ,
          TRIANGLE                ,
          TRIANGLE_STRIP          ,
          TRIANGLE_FAN            ,
          TRIANGLE_ADJACENCY      ,
          TRIANGLE_STRIP_ADJACENCY,
          PATCH                   
        }type;
        unsigned             nofVerticesPerPrimitive;
        bool                 indexed                ;
        std::vector<unsigned>indices                ;
        unsigned             restartIndex           ;
        bool                 enableRestart          ;
        unsigned             nofVertices               ;
        MeshPrimitives(
            unsigned  nofVertices                       ,
            enum Type type                    = TRIANGLE,
            unsigned  nofVerticesPerPrimitive = 3       );
        MeshPrimitives(
            unsigned              nofVertices                         ,
            enum Type             type                                ,
            unsigned              nofVerticesPerPrimitive             ,
            std::vector<unsigned>&indices                             ,
            bool                  enableRestart           = false     ,
            unsigned              restartIndex            = 0xffffffff);
    };

    class MeshGeometry{
      public:
        void* positionData;
        void* normalData;

        MeshVertices*   vertices  ;
        MeshPrimitives* primitives;

        static void createxAttribData(
            MeshVertices*vertices,
            void**data,
            unsigned nofVertices,
            VertexAttribDataDescriptor dataDescriptor);

        MeshGeometry();
        ~MeshGeometry();
    };

    class Mesh{
      public:
        MeshGeometry*geometry;
        glm::mat4    matrix  ;
    };

    class Model{
      public:
        std::vector<Mesh* >meshes;
        std::vector<Model*>models;
        glm::mat4          matrix;
        ~Model();
    };

    class Scene{
      public:
        std::vector<MeshGeometry*>geometries;
        Model*root;
        ~Scene();
    };
    class ModelLoader{
      public:
        virtual Scene*load(std::string file)=0;
    };
    class ModelLoaderManager{
      protected:
        std::vector<ModelLoader*>_loaders;
        std::vector<std::string >_loadersRegExps;
        bool _canLoad(std::string regexp,std::string file);
      public:
        void registerLoader(ModelLoader*loader,std::string regexp="");
        Scene*load(std::string file);
    };

  }
}
