#include"main.hpp"

#include <string>

#undef ___
#define ___

#define QUERY_COMPUTE
#define QUERY_STENCIL
//#define QUERY_MULTIPLICITY

void Init();
void init();
void destroy();
void Idle();
void idle();
void Mouse();
void Wheel(int d);
void SetMatrix();
void DrawGBuffer();
void DrawAmbient();
void DrawDeferred();
void DrawStencilToTexture();
void InitDrawStencilToTexture();

void DrawStencilShadow(simulation::Light*Light);

void DrawVertex           (simulation::Light*Light);
void DrawGeometry         (simulation::Light*Light);
void DrawGeometrySidesCaps(simulation::Light*Light);
void DrawComputeSOEPlane  (simulation::Light*Light);
void DrawComputeSOE       (simulation::Light*Light);
void DrawCompute          (simulation::Light*Light);
void DrawTessellation     (simulation::Light*Light);
void DrawSintorn          (simulation::Light*Light);
void drawNavyMapping(simulation::Light*Light);
void drawCubeShadowmapShadow(simulation::Light*Light);

ge::gl::AsynchronousQueryObject*MergeQuery            = NULL;
ge::gl::AsynchronousQueryObject*MergeTextureQuery     = NULL;
ge::gl::AsynchronousQueryObject*RasterizeQuery        = NULL;
ge::gl::AsynchronousQueryObject*RasterizeTextureQuery = NULL;
ge::gl::AsynchronousQueryObject*ShadowFrustumQuery    = NULL;
ge::gl::AsynchronousQueryObject*HierarchyQuery        = NULL;
ge::gl::AsynchronousQueryObject*HierarchyTextureQuery = NULL;
ge::gl::AsynchronousQueryObject*stencilShadowsQuery   = NULL;
ge::gl::AsynchronousQueryObject*gbufferQuery          = NULL;


ge::util::ArgumentObject*Args;

ge::util::CameraPath*CameraMation;
std::string CameraMationFile="sponzaprulet";

ge::util::FPSPrinter*fpsPrinter;

CGeometryCaps        *GeometryCaps         = NULL;
CGeometryCapsAlt     *GeometryCapsAlt      = NULL;
CGeometrySidesCaps   *GeometrySidesCaps    = NULL;
CComputeSidesSOEPlane*ComputeSidesSOEPlane = NULL;
CComputeSidesSOE     *ComputeSidesSOE      = NULL;
CComputeSides        *ComputeSides         = NULL;
CSintorn             *Sintorn              = NULL;

CGeometrySides    *GeometrySides     = NULL;
CTessellationSides*TessellationSides = NULL;
CVertexSides      *VertexSides       = NULL;
CVertexCaps       *VertexCaps        = NULL;
CShadowMapping    *Shadowmapping     = NULL;
NavyMapping       *navyMapping       = NULL;
CubeShadowMapping *cubeShadowMapping = NULL;
CubeNavyMapping   *cubeNavyMapping   = NULL;
RayTrace          *raytrace          = NULL;

ComputeGeometry   *computeGeometry   = NULL;
RTWBack           *rtw = NULL;

objconf::CameraPathConfiguration* cameraPathConfiguration = NULL;
objconf::CameraConfiguration    * cameraConfiguration     = NULL;
objconf::LightConfiguration     * lightConfiguration      = NULL;
objconf::ShadowMethodConfig     * navyConfig              = NULL;
objconf::ShadowMethodConfig     * rtwConfig               = NULL;

ShadowMethod*mm;

ge::gl::VertexArrayObject*EmptyVAO;

void DrawShadowless();
void DrawShadowlessWithoutGBuffer(simulation::Light*Light);
void DrawExtended();


//SceneModel
ge::db::ModelLoaderManager*modelLoaderManager = NULL;
ge::db::AssimpLoader*      assimpLoader       = NULL;
ge::db::Scene*             scene              = NULL;


ge::gl::BufferObject*SceneBuffer;
//GLuint SceneVAO;
ge::gl::VertexArrayObject*sceneVAO;
ge::gl::BufferObject*sceneDIBO;
ge::gl::BufferObject*sceneMaterial;
ge::gl::BufferObject*sceneAABBData;
unsigned sceneDIBOSize=0;
ge::gl::ProgramObject*frustumCullingProgram;
#define FRUSTUMCULLING_BINDING_DIBO      0
#define FRUSTUMCULLING_BINDING_AABB      1
#define FRUSTUMCULLING_WORKGROUP_SIZE_X 64

AxisAlignBoundingBox*sceneAABB;
std::vector<AxisAlignBoundingBox*>sceneBB;
std::vector<OrthoBoundingBox*>sceneOBB;
unsigned SceneNumTriangles=0;
float*RawTriangles;

Adjacency*fastAdjacency=NULL;

SSpecificEdge SE;

//sintorn
ge::gl::BufferObject*SintornVBO;

//base shader
ge::gl::ProgramObject*DrawShader;

GLuint StencilToTextureFBO;
GLuint ShadowTexture;
ge::gl::ProgramObject*StencilToTexture;
ge::gl::ProgramObject*Rasterizer;

ge::gl::TextureObject*shadowMask;


ge::gl::BufferObject*VertexSilhouetteUniversal;
ge::gl::ProgramObject*VertexSilhouetteShaderHull;
GLuint VertexSilhouetteVAO;
unsigned MaxMultiplicity=4;

ge::gl::BufferObject*VertexSilhouetteUniversalCap;
ge::gl::ProgramObject*VertexSilhouetteShaderCap;
GLuint VertexSilhouetteVAOCap;

ge::gl::ProgramObject*DrawSilhouetteShader;
ge::gl::BufferObject*DrawSilhouetteBuffer;
GLuint DrawSilhouetteVAO;

ge::gl::ProgramObject *CompShaderMulti             = NULL;
ge::gl::ProgramObject *CompShaderMultiplicity      = NULL;
ge::gl::ProgramObject *CompShaderHulls             = NULL;
ge::gl::ProgramObject *CompShaderCaps              = NULL;
ge::gl::ProgramObject *CompShaderCapsDraw          = NULL;
ge::gl::ProgramObject *CompShaderHullsQuads        = NULL;
ge::gl::ProgramObject *CompShaderHullsQuadsDraw    = NULL;
ge::gl::ProgramObject *CompShaderHullsQuadsAMD     = NULL;
ge::gl::ProgramObject *CompShaderHullsQuadsAMDDraw = NULL;
ge::gl::ProgramObject *ExtendedStencilProgram      = NULL;
ge::gl::ProgramObject *DrawTexture                 = NULL;
ge::gl::ProgramObject *Hybridmapping               = NULL;
ge::gl::ProgramObject *DrawHDB                     = NULL;
ge::gl::ProgramObject *ManifoldHullCap             = NULL;
ge::gl::ProgramObject *drawAABBProgram             = NULL;
ge::gl::ProgramObject *drawOBBProgram              = NULL;
ge::gl::BufferObject  *ManifoldHullCapBuffer       = NULL;

DrawPrimitive*simpleDraw = NULL;

ge::gl::ProgramPipelineObject*programPipeline;

//pro vykresleni sceny/hloubky
//STerrain Terrain;
SDeferred Deferred;

float Near=.1,Far=20;
std::string ModelFile;

glm::vec3 Pos=glm::vec3(0.f,10.f,0.f);
float Angle[3]={0,0,0};

glm::mat4 Model;
glm::mat4 View;
glm::mat4 Projection;
glm::mat4 mvp;

ge::util::WindowObject*Window;

NDormon::GpuPerfApi*GPA=NULL;
bool GPAavalable=true;

unsigned framecount=0;

Uint32 Last=SDL_GetTicks();
Uint32 Diff;

bool SSMeasureSidesPreprocess = true;
bool SSMeasureCapsPreprocess  = true;
bool SSMeasureSidesDraw       = true;
bool SSMeasureCapsDraw        = true;

bool     SSWithoutGBuffer      = false;
bool     SSEnable              = true;
bool     SSAOEnable            = false;
bool     SSDeterministic       = true;
bool     SSZFail               = true;
bool     SSReferenceEdge       = true;
bool     SSCullSides           = true;
unsigned SSWorkGroupSize       = 64;
bool     SSMeasureGbuffer      = true;
bool     SSMeasureStencil      = true;
bool     SSMeasureCompute      = true;
bool     SSMeasureShadowmap    = true;
bool     SSMeasureSmapping     = true;
bool     SSMeasureSBlit        = true;
bool     SSMeasureSMQuad       = true;
bool     SSMeasureVS           = true;
bool     SSMeasureTSE          = true;
bool     SSMeasureSintornHDB   = true;
bool     SSMeasureSintornSF    = true;
bool     SSMeasureSintornR     = true;
bool     SSMeasureSintornMerge = true;
enum ESSMethod{
  SS_GEOMETRY=0,
  SS_GEOMETRY_SIDES_CAPS,
  SS_COMPUTE_SOE_PLANE,
  SS_COMPUTE_SOE,
  SS_COMPUTE,
  SS_VS,
  SS_SHADOWMAP,
  SS_CUBESHADOWMAP,
  SS_CUBENAVYMAPPING,
  SS_RTW,
  SS_NAVYMAPPING,
  SS_RAYTRACE,
  SS_TS,
  SS_SINTORN,
  SS_NO
}SSMethod=SS_CUBENAVYMAPPING,LastSSMethod=SS_CUBENAVYMAPPING;

bool DSDrawLightSilhouette = false;
bool DSDrawViewSilhouette  = false;
bool DSDrawEverything      = false;

bool DisableAnttweakbar=false;

struct STestParam{
  bool Test;
  std::string CameraMationFile;
  float Duration;
  float TimeBefore;
  bool MeasureCompute;
  bool MeasureStencil;
  bool MeasureFPS;
  enum ESSMethod Method;
}TestParam;

//SGeometryParam                   GeometryParam,         GeometryParamLast;
SComputeSOEParam          ComputeSOEParam       ,ComputeSOEParamLast       ;
SSintornParam             SintornParam          ,SintornParamLast          ;

simulation::SimulationData*simData=NULL;

int main(int Argc,char*Argv[]){
  geom::ConvexHull a(glm::vec3(0.f,0.f,0.f),glm::vec3(2.f,2.f,2.f));
  //geom::ConvexHull b(glm::vec3(1.f,1.f,1.f),glm::vec3(3.f,3.f,3.f));
  //geom::ConvexHull c(a,b);
  geom::ConvexHull d(a,geom::Point(10.f,10.f,10.f));
  geom::ConvexHull e(d,a);
  std::cout<<e.toStr()<<std::endl;
  return 0;



  //*
  ge::util::ArgumentLoader*argLoader=NULL;
  try{
    argLoader=new ge::util::ArgumentLoader(Argc,Argv);
  }catch(std::string&e){
    std::cerr<<e<<std::endl;
  }
  for(unsigned i=0;i<argLoader->getNumVariables();++i){
    std::cerr<<argLoader->getVariable(i)<<":"<<argLoader->getData(i)<<std::endl;
  }
  simData=new simulation::SimulationData();
  for(unsigned i=0;i<argLoader->getNumVariables();++i){
    simData->insertVariable(argLoader->getVariable(i),argLoader->getData(i));
  }

  /*
     std::cerr<<simData->toStr()<<std::endl;
     std::cerr<<simData->define("");
     delete argLoader
     delete simData;
     exit(0);
     */

  // */
  //return 0;

  Args=new ge::util::ArgumentObject(Argc,Argv);

  //ModelFile          = Args->getArg("-m","models/o/o.3ds");
  //ModelFile          = Args->getArg("-m","models/bunny/bunny.obj");
  ModelFile          = Args->getArg("-m","models/robots/robots.obj");
  //ModelFile          = Args->getArg("-m","/home/dormon/Desktop/lost_empire/lost_empire.obj");
  //ModelFile          = Args->getArg("-m","/home/dormon/Desktop/sponza/sponza.obj");
  //ModelFile          = Args->getArg("-m","/home/dormon/Desktop/san-miguel/san-miguel.obj");
  //ModelFile          = Args->getArg("-m","/home/dormon/Desktop/conference/conference.obj");
  //ModelFile          = Args->getArg("-m","/media/data/models/Sponza/sponza.obj");
  //ModelFile          = Args->getArg("-m","/media/data/models/sibenik/sibenik.obj");
  //ModelFile          = Args->getArg("-m","/data/models/conference_corrected/conference.obj");
  //ModelFile          = Args->getArg("-m","/media/data/models/lost_empire/lost_empire.obj");
  //ModelFile          = Args->getArg("-m","/home/dormon/Desktop/hairball.obj");

  //ModelFile          = Args->getArg("-m","models/o/o.3ds");

  //ModelFile          = Args->getArg("-m","models/2quads/2quads.obj");
  //ModelFile          = Args->getArg("-m","models/2_3quads/2_3quads.obj");

  //ModelFile          = Args->getArg("-m","/media/old/home/dormon/Plocha/sponza/sponza.obj");
  //ModelFile          = Args->getArg("-m","/home/dormon/Desktop/koule_10000_1x1.obj");
  //ModelFile          = Args->getArg("-m","/home/dormon/Desktop/powerplant/powerplant.obj");



//  ModelFile          = Args->getArg("-m","/home/dormon/Plocha/ot/o.obj");
//  ModelFile          = Args->getArg("-m","/home/dormon/Desktop/sphere_2_9999.obj");
//  ModelFile          = Args->getArg("-m","/home/dormon/Desktop/sphere_4_9999.obj");
//  ModelFile          = Args->getArg("-m","/home/dormon/Desktop/sphere_6_9999.obj");

//  ModelFile          = Args->getArg("-m","/home/dormon/Desktop/sphere_2_19998.obj");
//  ModelFile          = Args->getArg("-m","/home/dormon/Desktop/sphere_2_30000.obj");
//  ModelFile          = Args->getArg("-m","/home/dormon/Desktop/sphere_2_39999.obj");
//  ModelFile          = Args->getArg("-m","/home/dormon/Desktop/sphere_2_49998.obj");
//  ModelFile          = Args->getArg("-m","/home/dormon/Desktop/sphere_8_90000.obj");
//
//  ModelFile          = Args->getArg("-m","/home/dormon/Desktop/triangle_1_9999.obj");
//  ModelFile          = Args->getArg("-m","/home/dormon/Desktop/triangle_9_90000.obj");



  ShaderDir          = Args->getArg("--shader-directory","shaders/");
  DisableAnttweakbar = Args->isPresent("--disable-anttweakbar");

  //test args
  TestParam.Test             = Args->isPresent("--test"                     );
  TestParam.CameraMationFile = Args->getArg   ("--test-cameramation"   ,""  );
  TestParam.Duration         = Args->getArgf  ("--test-duration"       ,"30");
  TestParam.TimeBefore       = Args->getArgf  ("--test-timebefore"     ,"5" );
  TestParam.MeasureCompute   = Args->isPresent("--test-measure-compute"     );
  TestParam.MeasureStencil   = Args->isPresent("--test-measure-stencil"     );
  TestParam.MeasureFPS       = Args->isPresent("--test-measure-fps"         );
  std::string MethodString   = Args->getArg   ("--test-method"              );

  if     (MethodString=="no"               )TestParam.Method = SS_NO;
  else if(MethodString=="gs"               )TestParam.Method = SS_GEOMETRY;
  else if(MethodString=="gs_sides_and_caps")TestParam.Method = SS_GEOMETRY_SIDES_CAPS;
  else if(MethodString=="cs"               )TestParam.Method = SS_COMPUTE;
  else if(MethodString=="cs_soe"           )TestParam.Method = SS_COMPUTE_SOE;
  else if(MethodString=="cs_soe_plane"     )TestParam.Method = SS_COMPUTE_SOE_PLANE;
  else if(MethodString=="ts"               )TestParam.Method = SS_TS;
  else if(MethodString=="vs"               )TestParam.Method = SS_VS;
  else if(MethodString=="sm"               )TestParam.Method = SS_SHADOWMAP;
  else if(MethodString=="rtw"              )TestParam.Method = SS_RTW;
  else if(MethodString=="nm"               )TestParam.Method = SS_NAVYMAPPING;
  else if(MethodString=="cm"               )TestParam.Method = SS_CUBESHADOWMAP;
  else if(MethodString=="cn"               )TestParam.Method = SS_CUBENAVYMAPPING;
  else if(MethodString=="si"               )TestParam.Method = SS_SINTORN;
  else if(MethodString=="ra"               )TestParam.Method = SS_RAYTRACE;
  else TestParam.Method=SS_NO;

  //computesoe args
  ComputeSOEParam.WorkGroupSize = Args->getArgi  ("--computesoe-start","--computesoe-end","-w","64");
  ComputeSOEParam.CullSides     = Args->isPresent("--computesoe-start","--computesoe-end","-c");
  ComputeSOEParamLast=ComputeSOEParam;
  ComputeSOEParamLast.CullSides=!ComputeSOEParam.CullSides;

  //sintorn args
  SintornParam.WavefrontSize            = Args->getArgi("--sintorn-start","--sintorn-end","-wf"   ,"64");
  SintornParam.ShadowFrustaPerWorkGroup = Args->getArgi("--sintorn-start","--sintorn-end","-sfpwg","1" );
  SintornParamLast=SintornParam;
  SintornParamLast.ShadowFrustaPerWorkGroup++;

  delete Args;
  Window=new ge::util::WindowObject(
      simData->getUVec2("window.size").x,
      simData->getUVec2("window.size").y,
      simData->getBool("window.fullscreen"),
      idle,
      Mouse,
      !DisableAnttweakbar,
      simData->getUint("context.version"),
      simData->getString("context.profile"),
      simData->getString("context.debug"));

  glewExperimental=GL_TRUE;
  glewInit();

  try{
    ge::gl::initShadersAndPrograms();
  }catch(std::string&e){
    std::cerr<<e<<std::endl;
  }

  try{
    GPA=new NDormon::GpuPerfApi(Window->getContext());
    GPA->enableComputeShader();
  }catch(std::string&e){
    std::cerr<<e<<std::endl;
    GPAavalable=false;
  }

  ge::gl::setHighDebugMessage();
  EmptyVAO=new ge::gl::VertexArrayObject();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);


  init();
  Window->mainLoop();

  destroy();

  delete EmptyVAO;
  if(GPAavalable)delete GPA;
  delete Window;
  return 0;
}

void Mouse(){

  if(!cameraConfiguration->isMouseLock()){
    if(Window->isLeftDown()){
      Angle[1]+=Window->getDeltaMousePosition()[0]*.01;
      Angle[0]+=Window->getDeltaMousePosition()[1]*.01;
    }
    if(Window->isMiddleDown()){
      Angle[2]+=Window->getDeltaMousePosition()[0]*.01;
    }
    cameraConfiguration->getCamera()->fpsCamera(Angle[0],Angle[1],Angle[2]);
  }
}

void Wheel(int d){
}

float Speed=.5f;

bool Linked=false;

void drawDiffuseSpecular(bool useShadows,simulation::Light*L){
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE,GL_ONE);
  glBlendEquation(GL_FUNC_ADD);
  glDepthFunc(GL_ALWAYS);

  deferred_SetTextures(&Deferred);
  glUseProgram(0);
  shadowMask->bind(GL_TEXTURE8);
  if(!Linked){
    DrawShader->setSeparable();
    DrawShader->setRetrievable();
    Linked=true;
    programPipeline->useProgramStages(GL_ALL_SHADER_BITS,DrawShader->getId());
  }
  programPipeline->bind();
  DrawShader->setdsa("La",0.f,0.f,0.f);
  DrawShader->setdsa("Ld",1,glm::value_ptr(L->diffuse));
  DrawShader->setdsa("Ls",1,glm::value_ptr(L->specular));
  DrawShader->setdsa("LightPosition",1,glm::value_ptr(L->position));
  DrawShader->setdsa("CameraPosition",-Pos[0],-Pos[1],-Pos[2]);
  DrawShader->setdsa("SSAO",SSAOEnable);
  DrawShader->setdsa("useShadows",useShadows);
  EmptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  EmptyVAO->unbind();
}


glm::mat4 camView=glm::mat4(1.f),camProj=glm::perspective(1.5f,1.f,1.f,1000.f);

void setConfig(objconf::ShadowMethodConfig**config,simulation::SimulationObject*method,std::string name){
  if(( method) && (!*config))*config=new objconf::ShadowMethodConfig(name,(simulation::SimulationObject*)method,simData);
  if((!method) && ( *config)){delete *config;*config = NULL;}
}

void idle(){
  ___;
  cameraConfiguration->getCamera()->right  ((Window->isKeyDown('d')-Window->isKeyDown('a'))*Speed);
  cameraConfiguration->getCamera()->up     ((Window->isKeyDown(' ')-Window->isKeyDown('c'))*Speed);
  cameraConfiguration->getCamera()->forward((Window->isKeyDown('w')-Window->isKeyDown('s'))*Speed);
  Pos=cameraConfiguration->getCamera()->getPosition();

  cameraConfiguration->getCamera()->getView(&View);
  cameraConfiguration->getCamera()->getProjection(&Projection);
  mvp=Projection*View;


  ___;
  setConfig(&navyConfig,navyMapping,"navyconfig");
  setConfig(&rtwConfig ,rtw        ,"rtwconfig" );

  if(Window->isKeyOn('q'))exit(0);

  if(Window->isKeyDown('g')){lightConfiguration->getLight()->position[1]-=0.4;simData->setAsChanged("light");}
  if(Window->isKeyDown('t')){lightConfiguration->getLight()->position[1]+=0.4;simData->setAsChanged("light");}
  ___;
  simData->sendUpdate();
  ___;
  if(SSEnable){
    if(SSMeasureGbuffer)gbufferQuery->begin();
    DrawGBuffer();
    if(SSMeasureGbuffer)gbufferQuery->end();
    DrawAmbient();

    switch(SSMethod){
      case SS_TS:
        DrawTessellation(lightConfiguration->getLight());
        break;
      case SS_NAVYMAPPING:
        mm=navyMapping;
        mm->createShadowMask();
        drawDiffuseSpecular(true,lightConfiguration->getLight());
        break;
      case SS_SHADOWMAP:
        mm=Shadowmapping;
        if(Window->isKeyOn('p')){
          glm::mat4 pp;
          glm::mat4 vv;
          if(geometry::getMinVP(&pp,&vv,Projection,View,
                sceneAABB->minPoint,sceneAABB->maxPoint,glm::vec3(lightConfiguration->getLight()->position)))
            Shadowmapping->setMatrices(pp,vv);
        }
        mm->createShadowMask();
        drawDiffuseSpecular(true,lightConfiguration->getLight());
        break;
      case SS_CUBESHADOWMAP:
        mm=cubeShadowMapping;
        mm->createShadowMask();
        ___;
        drawDiffuseSpecular(true,lightConfiguration->getLight());
        ___;
        break;
      case SS_CUBENAVYMAPPING:
        mm=cubeNavyMapping;
        mm->createShadowMask();
        drawDiffuseSpecular(true,lightConfiguration->getLight());
        break;
      case SS_RTW:
        mm=rtw;
        mm->createShadowMask();
        drawDiffuseSpecular(true,lightConfiguration->getLight());
        break;
      case SS_COMPUTE:
        mm=computeGeometry;
        mm->createShadowMask();
        drawDiffuseSpecular(true,lightConfiguration->getLight());
        break;
      case SS_RAYTRACE:
        mm=raytrace;
        if(Window->isKeyOn('b')){
          std::cerr<<GPA->getResults([](void*A){mm->createShadowMask();},mm)<<std::endl;
        }else{
          mm->createShadowMask();
        }
        drawDiffuseSpecular(true,lightConfiguration->getLight());
        break;
      default:
        break;
    }
  }else{
    DrawShadowless();
  }
  ___;
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  deferred_BlitDepthToDefault(&Deferred);
  ___;
  glDepthFunc(GL_LESS);
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glDisable(GL_BLEND);

  //*
  if(SSMethod==SS_NAVYMAPPING){
    //simpleDraw->drawHeatMap(navyMapping->getOffsetX()->getId(),.75,0,.25,.25,0u,40u);

    //simpleDraw->drawHeatMap(navyMapping->getOffsetX()->getId(),.0,0,.5,.5,-1.f,1.f);
    //simpleDraw->drawHeatMap(navyMapping->getSmoothX()->getId(),.5,0,.5,.5,-1.f,1.f);
    simpleDraw->drawDepth(navyMapping->getShadowMap()->getId(),.5f,.0f,.5f,.5f,1.f,1000.f);
  }
  if(SSMethod==SS_RTW){
    simpleDraw->drawDepth(rtw->getShadowMap()->getId(),.5f,.0f,.5f,.5f,1.f,1000.f);
  }
  if(SSMethod==SS_SHADOWMAP){
    simpleDraw->drawDepth(Shadowmapping->getShadowMap()->getId(),.5f,.0f,.5f,.5f,1.f,1000.f);
  }




  //*/

  if(cameraPathConfiguration->isDraw())
    if(cameraPathConfiguration->getPath())
      cameraPathConfiguration->getPath()->draw(glm::value_ptr(mvp));


  ___;
  Window->swap();
  fpsPrinter->endOfFrame();
  ___;
}


void init(){
  MergeQuery=new ge::gl::AsynchronousQueryObject(GL_TIME_ELAPSED,GL_QUERY_RESULT_NO_WAIT,ge::gl::AsynchronousQueryObject::UINT64);
  gbufferQuery = new ge::gl::AsynchronousQueryObject(MergeQuery);
  programPipeline=new ge::gl::ProgramPipelineObject();
  try{
    DrawShader = new ge::gl::ProgramObject(
        ShaderDir+"app/dgb.vp",
        ShaderDir+"app/dgb.gp",
        ShaderDir+"app/dgb.fp");

    DrawSilhouetteShader=new ge::gl::ProgramObject(
        ShaderDir+"app/drawsilhouette.vp",
        ShaderDir+"app/drawsilhouette.gp",
        ShaderDir+"app/drawsilhouette.fp");
  }catch(std::string&e){
    std::cerr<<e<<std::endl;
  }
  deferred_Init(&Deferred,simData->getUVec2("window.size").x,simData->getUVec2("window.size").y);
  InitDrawStencilToTexture();

  /*
  modelLoaderManager = new ge::db::ModelLoaderManager();
  assimpLoader       = new ge::db::AssimpLoader();
  modelLoaderManager->registerLoader(assimpLoader);
  scene = modelLoaderManager->load(ModelFile);
  std::cerr<<"sceneGeometries: "<<scene->geometries.size()<<std::endl;
  std::cerr<<scene->geometries[0]->vertices->toStr()<<std::endl;
  */


  InitModel(ModelFile.c_str());
  std::cerr<<"NumTriangles: "   <<fastAdjacency->getNofTriangles   ()<<std::endl;
  std::cerr<<"NumEdges: "       <<fastAdjacency->getNofEdges       ()<<std::endl;
  std::cerr<<"MaxMultiplicity: "<<fastAdjacency->getMaxMultiplicity()<<std::endl;

  cameraConfiguration = new objconf::CameraConfiguration(simData->getUVec2("window.size"));
  cameraPathConfiguration = new objconf::CameraPathConfiguration();
  cameraPathConfiguration->setCamera(cameraConfiguration->getCamera());
  lightConfiguration = new objconf::LightConfiguration();


  //test::setTestConvexHull(simpleDraw);

  SetMatrix();



  TwBar*Bar;
  Bar=TwNewBar("TweakBar");
  TwAddVarRW(Bar,"Speed"  ,TW_TYPE_FLOAT  ,&Speed     ," label='Movement speed' min=0 max=2 step=0.01"  );
  TwAddVarRW(Bar,"SSAO"   ,TW_TYPE_BOOLCPP,&SSAOEnable," help='Toggle screenspace ambient occlussion.' ");
  TwAddVarRW(Bar,"Shadows",TW_TYPE_BOOLCPP,&SSEnable  ," help='Toggle shadows on.' "                    );

  TwEnumVal MethodDef[]={
    {SS_COMPUTE        ,"compute"                            },
    {SS_SHADOWMAP      ,"shadowmapping"                      },
    {SS_CUBESHADOWMAP  ,"cubeShadowMapping"                  },
    {SS_CUBENAVYMAPPING,"cubeNavyMapping"                    },
    {SS_RTW            ,"rtv"                                },
    {SS_NAVYMAPPING    ,"ours"                               },
    {SS_TS             ,"Tessellation Shader Silhouette Edge"},
    {SS_RAYTRACE       ,"raytrace"                           },
    {SS_NO             ,"No shadows"                         }
  };
  TwType MethodType=TwDefineEnum("SS mode",MethodDef,sizeof(MethodDef)/sizeof(TwEnumVal));
  TwAddVarRW(Bar,"Method",MethodType,&SSMethod,
      "help='Change shadow volume stencil buffer type' ");


  TwAddVarRW(Bar,"Deterministic",TW_TYPE_BOOLCPP,&SSDeterministic," help='Toggle determinism on.' ");
  TwAddVarRW(Bar,"ZFail"        ,TW_TYPE_BOOLCPP,&SSZFail        ," help='Toggle ZFail on.' "      );

  TwAddVarRW(Bar,"ReferenceEdge",TW_TYPE_BOOLCPP,&SSReferenceEdge," help='Toggle Reference Edge.' "  );
  TwAddVarRW(Bar,"CullSides"    ,TW_TYPE_BOOLCPP,&SSCullSides    ," help='Toggle Culling of sides.' ");

  shadowMask=new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32F,1,
      simData->getIVec2("window.size",glm::ivec2(1024)).x,
      simData->getIVec2("window.size",glm::ivec2(1024)).y);

  simData->insertVariable("emptyVAO" ,new simulation::Object(EmptyVAO       ));
  simData->insertVariable("sceneVAO" ,new simulation::Object(sceneVAO       ));
  simData->insertVariable("sceneVBO" ,new simulation::Object(SceneBuffer));
  simData->insertVariable("light"    ,lightConfiguration->getLight()                                 );
  //simData->insertVariable("adjacency",new simulation::Object(&ModelAdjacency));
  simData->insertVariable("fastAdjacency",new simulation::Object(fastAdjacency));
  simData->insertVariable("gbuffer.position",new simulation::Object(Deferred.position));
  simData->insertVariable("gbuffer.fbo"     ,new simulation::Object(Deferred.fbo     ));
  simData->insertVariable("gbuffer.stencil" ,new simulation::Object(Deferred.depth ));
  simData->insertVariable("gpa"             ,new simulation::Object(GPA            ));
  simData->insertVariable("computeMethod.program.WORKGROUPSIZE", new simulation::Int(64));
  simData->insertVariable("computeMethod.program.CULL_SIDE", new simulation::Bool(true));
  simData->insertVariable("measure.computeGeometry.computeSides",new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.computeGeometry.draw",new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.computeGeometry.blit",new simulation::GpuGauge(false,true));


  simData->insertVariable("shadowMask",new simulation::Object(shadowMask));
  simData->insertVariable("measure.shadowMap.createShadowMap" ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.shadowMap.createShadowMask",new simulation::GpuGauge(false,true));

  simData->insertVariable("measure.rtw.createImportance",new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.rtw.createShadowMap" ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.rtw.createShadowMask",new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.rtw.importance2D"    ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.rtw.importance1D"    ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.rtw.smooth"          ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.rtw.sum"             ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.rtw.whole"           ,new simulation::GpuGauge(false,true));

  simData->insertVariable("measure.nv.computeViewSamples",new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.dv"                ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.countmap"          ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.integratex"        ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.offsetx"           ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.smootx"            ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.unwarpx"           ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.integratey"        ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.offsety"           ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.smooty"            ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.wholewarp"         ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.whole"             ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.wholex"            ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.nv.wholey"            ,new simulation::GpuGauge(false,true));








  simData->insertVariable("camera",new simulation::Object(cameraConfiguration->getCamera()));



  //std::cerr<<simData->toStr()<<std::endl;

  navyMapping       = new NavyMapping(simData);
  Shadowmapping     = new CShadowMapping(simData);
  cubeShadowMapping = new CubeShadowMapping(simData);
  cubeNavyMapping   = new CubeNavyMapping(simData);
  rtw               = new RTWBack(simData);
  computeGeometry   = new ComputeGeometry(simData);

  TessellationSides=new CTessellationSides(
      fastAdjacency,
      simData->getBool("tessellation.use_reference_edge"),
      simData->getBool("tessellation.cull_sides"),
      simData->getBool("tessellation.use_stencil_value_export"));
  GeometryCapsAlt=new CGeometryCapsAlt(fastAdjacency);

  raytrace = new RayTrace(simData);
  simpleDraw = new DrawPrimitive(ShaderDir+"app/");
  simpleDraw->setWindowSize(simData->getUVec2("window.size"));

  //std::cerr<<"konec initu\n";

  fpsPrinter = new ge::util::FPSPrinter(200);
  fpsPrinter->start();

}



//#define xm 30
//#define ym 30

void DrawScene(){
  if(Window->isKeyOn('f'))glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glDisable(GL_STENCIL_TEST);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  sceneVAO->bind();
  sceneMaterial->bindBase(GL_SHADER_STORAGE_BUFFER,0);
  sceneDIBO->bind(GL_DRAW_INDIRECT_BUFFER);
  glMultiDrawArraysIndirect(GL_TRIANGLES,NULL,sceneDIBOSize,sizeof(unsigned)*4);
  //glDrawArrays(GL_TRIANGLES,0,SceneNumTriangles*3);
  sceneVAO->unbind();
  if(Window->isKeyOn('f'))glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

void ShaderSetMatrix(ge::gl::ProgramObject*P){
  P->set("m"  ,1,GL_FALSE,(const float*)glm::value_ptr(Model     ));
  P->set("v"  ,1,GL_FALSE,(const float*)glm::value_ptr(View      ));
  P->set("p"  ,1,GL_FALSE,(const float*)glm::value_ptr(Projection));
  P->set("mvp",1,GL_FALSE,(const float*)glm::value_ptr(mvp       ));
}

void DrawGBuffer(){
  //*  
  frustumCullingProgram->use();
  sceneDIBO    ->bindBase(GL_SHADER_STORAGE_BUFFER,FRUSTUMCULLING_BINDING_DIBO);
  sceneAABBData->bindBase(GL_SHADER_STORAGE_BUFFER,FRUSTUMCULLING_BINDING_AABB);
  frustumCullingProgram->set("numAABB",(unsigned)sceneDIBOSize);
  frustumCullingProgram->set("mvp",1,GL_FALSE,(const float*)glm::value_ptr(mvp));
  glDispatchCompute(sceneDIBOSize/FRUSTUMCULLING_WORKGROUP_SIZE_X+1,1,1);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);

  // */
  deferred_EnableFBO(&Deferred);
  deferred_SetTextures(&Deferred);
  Deferred.Create->use();
  ShaderSetMatrix(Deferred.Create);
  DrawScene();
  deferred_DisableFBO(&Deferred);
}

void DrawAmbient(){
  deferred_SetTextures(&Deferred);

  DrawShader->use();

  DrawShader->set("La",.3f,.3f,.3f);
  DrawShader->set("Ld",.0f,.0f,.0f);
  DrawShader->set("Ls",.0f,.0f,.0f);
  DrawShader->set("CameraPosition",-Pos[0],-Pos[1],-Pos[2]);
  DrawShader->set("SSAO",SSAOEnable);
  DrawShader->set("useShadows",false);

  EmptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  EmptyVAO->unbind();
}


void DrawDiffuseSpecular(bool FrameBuffer,bool ImageAtomicAdd,simulation::Light*L){
  deferred_SetTextures(&Deferred);
  //DrawShader->use();
  //*
  glUseProgram(0);

  if(!Linked){
    /*glProgramParameteri(DrawShader->getId(),GL_PROGRAM_SEPARABLE,GL_TRUE);
      glLinkProgram(DrawShader->getId());*/
    DrawShader->setSeparable();
    DrawShader->setRetrievable();
    /*
       const GLsizei bufferSize=100000;
       unsigned char buffer[bufferSize];
       GLsizei length;
       GLenum format;
       glGetProgramBinary(DrawShader->getId(),bufferSize,&length,&format,buffer);
       FILE*f=fopen("sem.bin","wb+");
       for(GLsizei c=0;c<bufferSize;++c)
       fputc(buffer[c],f);
       fclose(f);*/
    Linked=true;
    programPipeline->useProgramStages(GL_ALL_SHADER_BITS,DrawShader->getId());
  }
  programPipeline->bind();

  //programPipeline->useVertexStage(DrawShader->getId());
  //programPipeline->useGeometryStage(DrawShader->getId());
  //programPipeline->useFragmentStage(DrawShader->getId());
  DrawShader->setdsa("La",0.f,0.f,0.f);
  DrawShader->setdsa("Ld",1,glm::value_ptr(L->diffuse));
  DrawShader->setdsa("Ls",1,glm::value_ptr(L->specular));
  DrawShader->setdsa("LightPosition",1,glm::value_ptr(L->position));
  DrawShader->setdsa("CameraPosition",-Pos[0],-Pos[1],-Pos[2]);
  DrawShader->setdsa("SSAO",SSAOEnable);
  //DrawShader->setdsa("FrameBufferFlag",FrameBuffer);
  //DrawShader->setdsa("ImageAtomicAddFlag",ImageAtomicAdd);







  /*
     DrawShader->set("La",0.f,0.f,0.f);
     DrawShader->set("Ld",1,L->Diffuse);
     DrawShader->set("Ls",1,L->Specular);
     DrawShader->set("LightPosition",1,L->Position);
     DrawShader->set("CameraPosition",-Pos[0],-Pos[1],-Pos[2]);
     DrawShader->set("SSAO",SSAOEnable);
     DrawShader->set("FrameBufferFlag",FrameBuffer);
     DrawShader->set("ImageAtomicAddFlag",ImageAtomicAdd);
  // */
  glActiveTexture(GL_TEXTURE7);
  glBindTexture(GL_TEXTURE_2D,ShadowTexture);
  EmptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  EmptyVAO->unbind();
}


void DrawManifold(simulation::Light*L){

}

int COC=0;

void setGLForStencil(bool zfail){
  deferred_StartCreateStencil(&Deferred);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS,0,0);
  if(zfail){
    glStencilOpSeparate(GL_FRONT,GL_KEEP,GL_INCR_WRAP,GL_KEEP);
    glStencilOpSeparate(GL_BACK,GL_KEEP,GL_DECR_WRAP,GL_KEEP);
  }else{
    glStencilOpSeparate(GL_FRONT,GL_KEEP,GL_KEEP,GL_INCR_WRAP);
    glStencilOpSeparate(GL_BACK,GL_KEEP,GL_KEEP,GL_DECR_WRAP);
  }
  glDepthFunc(GL_LESS);
  glDepthMask(GL_FALSE);
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
}

void drawStencil(simulation::Light*Light){
  deferred_EndCreateStencil(&Deferred);
  deferred_BlitStencilBuffer(&Deferred);
  glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
  glStencilFunc(GL_EQUAL,0,0xff);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE,GL_ONE);
  glBlendEquation(GL_FUNC_ADD);
  glDepthFunc(GL_ALWAYS);
  DrawDiffuseSpecular(false,false,Light);
  glDepthFunc(GL_LESS);
  glDisable(GL_STENCIL_TEST);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
}


void DrawTessellation(simulation::Light*Light){
  setGLForStencil(SSZFail);
  if(TestParam.MeasureStencil)stencilShadowsQuery->begin();
  TessellationSides->DrawSides(glm::value_ptr(mvp),Light);
  if(SSZFail)GeometryCapsAlt->DrawCaps(glm::value_ptr(mvp),Light);
  if(TestParam.MeasureStencil)stencilShadowsQuery->end();
  drawStencil(Light);
}

void DrawSintorn(simulation::Light*L){

  //std::cerr<<"DrawSintorn\n";

  /*
     HierarchyQuery->begin();
     Sintorn->GenerateHierarchy(Deferred.Depth,Deferred.Normal,L->Position);
     HierarchyQuery->end();
  // */

  //*

  if(Window->isKeyOn('b')){
    if(GPAavalable){
      GPA->beginSession();
      for(unsigned p=0;p<GPA->getNumPasses();++p){
        GPA->beginPass();

        GPA->beginSample(0);
        Sintorn->GenerateHierarchyTexture(Deferred.depth->getId(),Deferred.normal->getId(),glm::value_ptr(L->position));
        //Sintorn->GenerateHierarchy(Deferred.depth->getId(),Deferred.normal->getId(),L->Position);
        GPA->endSample();

        GPA->endPass();
      }
      GPA->endSession();
      std::cerr<<GPA->getResults()<<std::endl;
    }
  }else{

    HierarchyTextureQuery->begin();
    Sintorn->GenerateHierarchyTexture(Deferred.depth->getId(),Deferred.normal->getId(),glm::value_ptr(L->position));
    //Sintorn->GenerateHierarchy(Deferred.depth->getId(),Deferred.normal->getId(),L->Position);
    HierarchyTextureQuery->end();
  }
  glFinish();
  // */
  //std::cerr<<"After GenerateHierarchy\n";
  if(!Window->isKeyOn('r'))Sintorn->DrawHDB(Window->isKeyOn('y')*2+Window->isKeyOn('x')+Window->isKeyOn('v')*4,Window->isKeyOn('e'));

  //std::cerr<<"ANO ANO TADY JE DORMON 0\n";
  glFinish();

  ShadowFrustumQuery->begin();
  Sintorn->ComputeShadowFrusta(glm::value_ptr(L->position),mvp);
  ShadowFrustumQuery->end();
  glFinish();

  //std::cerr<<"ANO ANO TADY JE DORMON 1\n";
  //
  if(Window->isKeyOn('n')){
    if(GPAavalable){
      GPA->beginSession();
      for(unsigned p=0;p<GPA->getNumPasses();++p){
        GPA->beginPass();

        GPA->beginSample(0);

        Sintorn->RasterizeTexture();
        //Sintorn->Rasterize();

        GPA->endSample();

        GPA->endPass();
      }
      GPA->endSession();
      std::cerr<<GPA->getResults()<<std::endl;
    }
  }else{
    //*
    RasterizeTextureQuery->begin();
    Sintorn->RasterizeTexture();
    RasterizeTextureQuery->end();
    // */
    /*
       RasterizeQuery->begin();
       Sintorn->Rasterize();
       RasterizeQuery->end();
    // */
  }
  //std::cerr<<"ANO ANO TADY JE DORMON 2\n";
  //*
  MergeTextureQuery->begin();
  Sintorn->MergeTexture();
  MergeTextureQuery->end();
  // */
  /*
     MergeQuery->begin();
     Sintorn->Merge();
     MergeQuery->end();
  // */
  glFinish();


  if(Window->isKeyOn('r'))Sintorn->DrawHSB(Window->isKeyOn('y')*2+Window->isKeyOn('x')+Window->isKeyOn('v')*4);


  //std::cerr<<"After rasterize\n";
}


void DrawVertex(simulation::Light*Light){
  setGLForStencil(SSZFail);
  if(TestParam.MeasureStencil)stencilShadowsQuery->begin();
  VertexSides->DrawSides(glm::value_ptr(mvp),Light);
  if(SSZFail)VertexCaps->DrawCaps(glm::value_ptr(mvp),Light);
  if(TestParam.MeasureStencil)stencilShadowsQuery->end();
  drawStencil(Light);
}

void DrawGeometry(simulation::Light*Light){
  setGLForStencil(SSZFail);
  if(TestParam.MeasureStencil)stencilShadowsQuery->begin();
  GeometrySides->DrawSides(glm::value_ptr(mvp),Light);
  if(SSZFail)GeometryCaps->DrawCaps(glm::value_ptr(mvp),Light);
  if(TestParam.MeasureStencil)stencilShadowsQuery->end();
  drawStencil(Light);
}

void DrawGeometrySidesCaps(simulation::Light*Light){
  setGLForStencil(true);
  if(TestParam.MeasureStencil)stencilShadowsQuery->begin();
  GeometrySidesCaps->DrawSidesCaps(glm::value_ptr(mvp),Light);
  if(TestParam.MeasureStencil)stencilShadowsQuery->end();
  drawStencil(Light);
}

void DrawComputeSOEPlane(simulation::Light*Light){
  ComputeSidesSOEPlane->ComputeSides(glm::value_ptr(mvp),Light);
  setGLForStencil(SSZFail);
  if(TestParam.MeasureStencil)stencilShadowsQuery->begin();
  ComputeSidesSOEPlane->DrawSides(glm::value_ptr(mvp),Light);
  if(SSZFail)GeometryCapsAlt->DrawCaps(glm::value_ptr(mvp),Light);
  if(TestParam.MeasureStencil)stencilShadowsQuery->end();
  drawStencil(Light);
}

void DrawStencilToTexture(){
  glBindFramebuffer(GL_FRAMEBUFFER,StencilToTextureFBO);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  GLbitfield Ahoj=GL_COLOR_BUFFER_BIT;
  glClear(Ahoj);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
  glStencilFunc(GL_NOTEQUAL,0,0xff);
  glDepthFunc(GL_ALWAYS);
  StencilToTexture->use();
  glDrawArrays(GL_POINTS,0,1);
  glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void InitDrawStencilToTexture(){
  glGenTextures(1,&ShadowTexture);
  glBindTexture(GL_TEXTURE_2D,ShadowTexture);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,Deferred.Size[0],Deferred.Size[1],0,GL_RGBA,
      GL_FLOAT,NULL);

  glGenFramebuffers(1,&StencilToTextureFBO);
  glBindFramebuffer(GL_FRAMEBUFFER,StencilToTextureFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D,ShadowTexture,0);
  glFramebufferTexture2D(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,
      GL_TEXTURE_RECTANGLE,Deferred.depth->getId(),0);

  GLenum DrawBuffers[]={
    GL_COLOR_ATTACHMENT0
  };
  glDrawBuffers(1,DrawBuffers);
  glClear(GL_COLOR_BUFFER_BIT);
  GLenum Status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if(Status!=GL_FRAMEBUFFER_COMPLETE)
    std::cerr<<"FBO"<<std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER,0);

  StencilToTexture=new ge::gl::ProgramObject(
      ShaderDir+"app/stenciltotexture.vp",
      ShaderDir+"app/stenciltotexture.gp",
      ShaderDir+"app/stenciltotexture.fp");
}

void DrawComputeSOE(simulation::Light*Light){
  /*if(TestParam.MeasureCompute)
    glBeginQuery(GL_TIME_ELAPSED,QueryTime);*/

  if(Window->isKeyOn('n')){
    if(GPAavalable){
      GPA->beginSession();
      for(unsigned p=0;p<GPA->getNumPasses();++p){
        GPA->beginPass();

        GPA->beginSample(0);
        ComputeSidesSOE->ComputeSides(glm::value_ptr(mvp),Light);

        GPA->endSample();

        GPA->endPass();
      }
      GPA->endSession();
      std::cerr<<GPA->getResults()<<std::endl;
    }
  }else
    ComputeSidesSOE->ComputeSides(glm::value_ptr(mvp),Light);

  /*if(TestParam.MeasureCompute){
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectuiv(QueryTime,GL_QUERY_RESULT_NO_WAIT,&QueryTimePassed);
    }*/
  setGLForStencil(SSZFail);
  if(TestParam.MeasureStencil)stencilShadowsQuery->begin();
  ComputeSidesSOE->DrawSides(glm::value_ptr(mvp),Light);
  if(SSZFail)GeometryCapsAlt->DrawCaps(glm::value_ptr(mvp),Light);
  if(TestParam.MeasureStencil)stencilShadowsQuery->end();
  drawStencil(Light);
}

void DrawCompute(simulation::Light*Light){
  ComputeSides->ComputeSides(glm::value_ptr(mvp),Light);
  setGLForStencil(SSZFail);
  if(TestParam.MeasureStencil)stencilShadowsQuery->begin();
  ComputeSides->DrawSides(glm::value_ptr(mvp),Light);
  if(SSZFail)GeometryCapsAlt->DrawCaps(glm::value_ptr(mvp),Light);
  if(TestParam.MeasureStencil)stencilShadowsQuery->end();
  drawStencil(Light);
}

void DrawShadowless(){
  DrawGBuffer();
  glDepthFunc(GL_ALWAYS);
  glDepthMask(GL_FALSE);
  DrawAmbient();
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE,GL_ONE);
  glBlendEquation(GL_FUNC_ADD);

  glDepthFunc(GL_ALWAYS);
  DrawDiffuseSpecular(false,false,lightConfiguration->getLight());
  glDepthFunc(GL_LESS);

  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
}

void InsertKeyPointFromCamera(void*A){
  ge::util::CameraObject*C=(ge::util::CameraObject*)A;
  CameraMation->insertToEnd(new ge::util::CameraKeyPoint(C));
}

void SaveCameraMation(void*A){
  std::string*Name=(std::string*)A;
  CameraMation->saveCSV(*Name);
  //CameraMation->SaveCSV("mujprulet");

}

void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString){
  destinationClientString = sourceLibraryString;
}





void SetMatrix(){
  cameraConfiguration->getCamera()->getProjection(&Projection);
  cameraConfiguration->getCamera()->getView(&View);
  Model=glm::mat4(1.);
  mvp=Projection*View*Model;
}

struct MeshMaterial{
  float diffuse[4];
  float specular[4];
};

void InitModel(const char* File){
  const aiScene*SceneModel=aiImportFile(File,
      aiProcess_CalcTangentSpace|
      aiProcess_Triangulate|
      aiProcess_GenNormals|
      //aiProcess_JoinIdenticalVertices|
      aiProcess_SortByPType);
  if(SceneModel==NULL){
    std::cerr<<"SCENE ERROR: "<<File<<std::endl;
    exit(1);
  }
  //glGenVertexArrays(1,&SceneVAO);
  //glBindVertexArray(SceneVAO);
  sceneVAO=new ge::gl::VertexArrayObject();

  SceneNumTriangles=0;
  for(unsigned mesh=0;mesh<SceneModel->mNumMeshes;++mesh){
    aiMesh*Mesh=SceneModel->mMeshes[mesh];
    SceneNumTriangles+=Mesh->mNumFaces;
  }

  SceneBuffer=new ge::gl::BufferObject(sizeof(float)*SceneNumTriangles*3*2*3);

  sceneMaterial = new ge::gl::BufferObject(sizeof(float)*4*2*SceneModel->mNumMeshes);

  MeshMaterial*mmptr= (MeshMaterial*)sceneMaterial->map();


  sceneAABB=new AxisAlignBoundingBox();
  sceneDIBOSize=SceneModel->mNumMeshes;
  sceneDIBO=new ge::gl::BufferObject(sizeof(unsigned)*4*sceneDIBOSize);
  sceneAABBData=new ge::gl::BufferObject(sizeof(float)*4*2*sceneDIBOSize);
  unsigned*dibo=(unsigned*)sceneDIBO->map();
  float*aabbData=(float*)sceneAABBData->map();
  float*ptr=(float*)SceneBuffer->map();
  unsigned actface=0;
  RawTriangles=new float[SceneNumTriangles*3*3];
  for(unsigned mesh=0;mesh<SceneModel->mNumMeshes;++mesh){//loop over meshes
    aiMesh*Mesh=SceneModel->mMeshes[mesh];

    aiColor3D c;
    aiMaterial*mat=SceneModel->mMaterials[Mesh->mMaterialIndex];
    mat->Get(AI_MATKEY_COLOR_DIFFUSE,c);
    for(int i=0;i<3;++i)mmptr->diffuse[i]=c[i];
    mat->Get(AI_MATKEY_COLOR_SPECULAR,c);
    for(int i=0;i<3;++i)mmptr->specular[i]=c[i];
    mmptr++;

    AxisAlignBoundingBox*aabb=new AxisAlignBoundingBox();
    sceneBB.push_back(aabb);
    dibo[mesh*4+0]=Mesh->mNumFaces*3;
    dibo[mesh*4+1]=1;
    dibo[mesh*4+2]=actface*3;
    dibo[mesh*4+3]=0;
    glm::vec3 center=glm::vec3(0.f);
    for(unsigned face=0;face<Mesh->mNumFaces;++face){//loop over faces
      aiFace*Face=Mesh->mFaces+face;
      for(unsigned i=0;i<3;++i){
        aiVector3D*N=Mesh->mNormals+Face->mIndices[i];
        aiVector3D*V=Mesh->mVertices+Face->mIndices[i];
        ptr[actface*3*2*3+i*6+0]=V->x;
        ptr[actface*3*2*3+i*6+1]=V->y;
        ptr[actface*3*2*3+i*6+2]=V->z;
        RawTriangles[actface*3*3+i*3+0]=V->x;
        RawTriangles[actface*3*3+i*3+1]=V->y;
        RawTriangles[actface*3*3+i*3+2]=V->z;
        ptr[actface*3*2*3+i*6+3]=N->x;
        ptr[actface*3*2*3+i*6+4]=N->y;
        ptr[actface*3*2*3+i*6+5]=N->z;
        sceneAABB->addPoint(glm::vec3(V->x,V->y,V->z));
        aabb     ->addPoint(glm::vec3(V->x,V->y,V->z));
        center+=glm::vec3(V->x,V->y,V->z);
      }
      actface++;
    }
    center/=(float)Mesh->mNumFaces*3;
    aabbData[mesh*4*2+0]=aabb->minPoint[0];
    aabbData[mesh*4*2+1]=aabb->minPoint[1];
    aabbData[mesh*4*2+2]=aabb->minPoint[2];
    aabbData[mesh*4*2+3]=1;

    aabbData[mesh*4*2+4]=aabb->maxPoint[0];
    aabbData[mesh*4*2+5]=aabb->maxPoint[1];
    aabbData[mesh*4*2+6]=aabb->maxPoint[2];
    aabbData[mesh*4*2+7]=1;
  }
  sceneAABBData->unmap();
  sceneDIBO->unmap();

  frustumCullingProgram=new ge::gl::ProgramObject(
      ShaderDir+"app/frustumCulling.comp",
      ge::gl::ShaderObject::define("FRUSTUMCULLING_BINDING_DIBO"    ,FRUSTUMCULLING_BINDING_DIBO    )+
      ge::gl::ShaderObject::define("FRUSTUMCULLING_BINDING_AABB"    ,FRUSTUMCULLING_BINDING_AABB    )+
      ge::gl::ShaderObject::define("FRUSTUMCULLING_WORKGROUP_SIZE_X",FRUSTUMCULLING_WORKGROUP_SIZE_X)+
      "");

  SceneBuffer->unmap();

  sceneMaterial->unmap();


  aiReleaseImport(SceneModel);

  sceneVAO->addAttrib(SceneBuffer,0,3,GL_FLOAT,sizeof(float)*6,(GLvoid*)(sizeof(float)*0));
  sceneVAO->addAttrib(SceneBuffer,1,3,GL_FLOAT,sizeof(float)*6,(GLvoid*)(sizeof(float)*3));

  //ComputeAdjacency(&ModelAdjacency,RawTriangles,SceneNumTriangles);

  fastAdjacency = new Adjacency((const float*)RawTriangles,SceneNumTriangles,2);




}

void destroy(){
  delete MergeQuery;
  delete MergeTextureQuery;
  delete RasterizeQuery;
  delete RasterizeTextureQuery;
  delete HierarchyQuery;
  delete HierarchyTextureQuery;
  delete ShadowFrustumQuery;
  delete stencilShadowsQuery;
  delete programPipeline;
  delete CameraMation;
  delete DrawShader;
  delete DrawSilhouetteShader;
  deferred_Free(&Deferred);
  delete StencilToTexture;
  delete SceneBuffer;
  delete[]RawTriangles;
  //delete CameraMation;
}
