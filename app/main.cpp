#include"main.hpp"

#include <string>

#define QUERY_COMPUTE
#define QUERY_STENCIL
//#define QUERY_MULTIPLICITY

void Init();
void destroy();
void TestInit();
void Idle();
void TestIdle();
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

ge::gl::AsynchronousQueryObject*MergeQuery;
ge::gl::AsynchronousQueryObject*MergeTextureQuery;
ge::gl::AsynchronousQueryObject*RasterizeQuery;
ge::gl::AsynchronousQueryObject*RasterizeTextureQuery;
ge::gl::AsynchronousQueryObject*ShadowFrustumQuery;
ge::gl::AsynchronousQueryObject*HierarchyQuery;
ge::gl::AsynchronousQueryObject*HierarchyTextureQuery;
ge::gl::AsynchronousQueryObject*stencilShadowsQuery;
ge::gl::AsynchronousQueryObject*gbufferQuery;


void drawNavyMapping(simulation::Light*Light);
void drawCubeShadowmapShadow(simulation::Light*Light);

ge::util::ArgumentObject*Args;
ge::util::CameraObject*Camera;
ge::util::CameraPath*CameraMation;
std::string CameraMationFile="tadyjeprulet";


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

ShadowMethod*mm;

ge::gl::VertexArrayObject*EmptyVAO;

void DrawShadowless();
void DrawShadowlessWithoutGBuffer(simulation::Light*Light);
void DrawExtended();


//SceneModel
ge::gl::BufferObject*SceneBuffer;
//GLuint SceneVAO;
ge::gl::VertexArrayObject*sceneVAO;
ge::gl::BufferObject*sceneDIBO;
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

SAdjecency ModelAdjacency;
SSpecificEdge SE;
//CShadowShader*ShadowShader;

//Test

//float LightPos[4]={0,-10,0,1};
//simulation::Light Light;
simulation::Light light(glm::vec4(10.f,10.f,100.f,1.f),glm::vec3(1.f),glm::vec3(1.f));
simulation::Light light2(glm::vec4(0.f,0.f,0.f,1.f),glm::vec3(1.f),glm::vec3(1.f));

//sintorn
ge::gl::BufferObject*SintornVBO;

//base shader
ge::gl::ProgramObject*DrawShader;

GLuint StencilToTextureFBO;
GLuint ShadowTexture;
ge::gl::ProgramObject*StencilToTexture;
ge::gl::ProgramObject*Rasterizer;

GLuint QueryTime;
GLuint QueryTimePassed;
GLuint QueryTimePassedShadowmap;
GLuint QueryTimePassedSmapping;
GLuint QueryTImePassedVS;
GLuint QueryTimePassedSintornHDB;
GLuint QueryTimePassedSintornSF;
GLuint QueryTimePassedSintornR;
GLuint QueryTimePassedSintornMerge;

ge::gl::TextureObject*shadowMask;


ge::gl::BufferObject*VertexSilhouetteUniversal;
ge::gl::ProgramObject*VertexSilhouetteShaderHull;
GLuint VertexSilhouetteVAO;
unsigned MaxMultiplicity=4;

ge::gl::BufferObject*VertexSilhouetteUniversalCap;
ge::gl::ProgramObject*VertexSilhouetteShaderCap;
GLuint VertexSilhouetteVAOCap;

/*
   ge::gl::CShaderProgram*TessellationEdgeSilhouetteShader;
   ge::gl::BufferObject*TessellationEdgeSilhouetteUniversal;
   ge::gl::BufferObject*TessellationEdgeSilhouetteUniversalElements;
   ge::gl::VertexArrayObject*TessEdgeSidesVAO;
   GLuint TessellationEdgeSilhouetteVAO;
   unsigned MaxMultiplicityTSE=4;

   ge::gl::CShaderProgram*TessellationEdge2SilhouetteShader;
   */

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
ge::gl::ProgramObject *drawTriangleProgram         = NULL;
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

NDormon::CGpuPerfApi*GPA;
bool GPAavalable=true;

unsigned framecount=0;

Uint32 Last=SDL_GetTicks();
Uint32 Diff;

bool SSMeasureSidesPreprocess = true;
bool SSMeasureCapsPreprocess  = true;
bool SSMeasureSidesDraw       = true;
bool SSMeasureCapsDraw        = true;

bool     SSWithoutGBuffer      = false;
bool     SSEnable              = false;
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
  SS_NAVYMAPPING,
  SS_CUBESHADOWMAP,
  SS_TS,
  SS_SINTORN,
  SS_NO
}SSMethod=SS_NO,LastSSMethod=SS_NO;

bool DSDrawLightSilhouette = false;
bool DSDrawViewSilhouette  = false;
bool DSDrawEverything      = false;

bool DisableAnttweakbar=false;

ge::util::ContextParam contextParam;
ge::util::WindowParam  windowParam;

struct SCameraParam{
  float Near;
  float Far;
  float Fovy;
}CameraParam;

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
SGeometryTemplate         GeometryTemplate      ,GeometryTemplateLast      ;
STessellationParam        TessellationParam     ,TessellationParamLast     ;
SComputeSOEParam          ComputeSOEParam       ,ComputeSOEParamLast       ;
SComputeParam             ComputeParam          ,ComputeParamLast          ;
SShadowMapParam           ShadowmapParam        ,ShadowmapParamLast        ;
SSintornParam             SintornParam          ,SintornParamLast          ;
CubeShadowMappingTemplate cubeShadowMappingParam,cubeShadowMappingParamLast;

simulation::SimulationData*simData=NULL;

int main(int Argc,char*Argv[]){

  //*
  ge::util::ArgumentLoader*argLoader;
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

  ModelFile          = Args->getArg("-m","models/o/o.3ds");
  ShaderDir          = Args->getArg("--shader-directory","shaders/");
  DisableAnttweakbar = Args->isPresent("--disable-anttweakbar");

  ge::util::loadContextParam(&contextParam,Args);
  ge::util::loadWindowParam (&windowParam ,Args);

  contextParam.profile="compatibility";
  contextParam.version=200;

  //camera args
  CameraParam.Near = Args->getArgf("-near","1");
  CameraParam.Far  = Args->getArgf("-far" ,"9999");
  CameraParam.Fovy = Args->getArgf("-fovy","45" );

  //light args
  light.position[0] = Args->getArgf("--light-start","--light-end",0,"10");
  light.position[1] = Args->getArgf("--light-start","--light-end",1,"10");
  light.position[2] = Args->getArgf("--light-start","--light-end",2,"10");
  light.position[3] = Args->getArgf("--light-start","--light-end",3,"1");
  light.diffuse [0] = Args->getArgf("--light-start","--light-end",4,"1");
  light.diffuse [1] = Args->getArgf("--light-start","--light-end",5,"1");
  light.diffuse [2] = Args->getArgf("--light-start","--light-end",6,"1");
  light.specular[0] = Args->getArgf("--light-start","--light-end",7,"1");
  light.specular[1] = Args->getArgf("--light-start","--light-end",8,"1");
  light.specular[2] = Args->getArgf("--light-start","--light-end",9,"1");

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
  else if(MethodString=="nm"               )TestParam.Method = SS_NAVYMAPPING;
  else if(MethodString=="cm"               )TestParam.Method = SS_CUBESHADOWMAP;
  else if(MethodString=="si"               )TestParam.Method = SS_SINTORN;
  else TestParam.Method=SS_NO;

  //geometry args
  GeometryTemplate.Deterministic         = Args->isPresent("--geometry-start","--geometry-end","-d"  );
  GeometryTemplate.ReferenceEdge         = Args->isPresent("--geometry-start","--geometry-end","-r"  );
  GeometryTemplate.Universal             = Args->isPresent("--geometry-start","--geometry-end","-u"  );
  GeometryTemplate.UseVS2GSArray         = Args->isPresent("--geometry-start","--geometry-end","-v2g");
  GeometryTemplate.UseVertexArray        = Args->isPresent("--geometry-start","--geometry-end","-va" );
  GeometryTemplate.UseStencilValueExport = Args->isPresent("--geometry-start","--geometry-end","-sve");
  GeometryTemplate.CullSides             = Args->isPresent("--geometry-start","--geometry-end","-c"  );
  GeometryTemplate.FrontCapToInfinity    = Args->isPresent("--geometry-start","--geometry-end","-fi" );
  GeometryTemplate.Version               = Args->getArgi  ("--geometry-start","--geometry-end","-v","430");
  GeometryTemplate.UseLayouts            = true;
  GeometryTemplate.CCWIsFrontFace        = true;
  GeometryTemplate.FrontFaceInside       = false;
  GeometryTemplate.Visualize             = false;
  GeometryTemplate.GenerateSides         = true;
  GeometryTemplate.GenerateCaps          = false;
  GeometryTemplate.Matrix                = "mvp";
  GeometryTemplate.Profile               = "core";
  GeometryTemplate.LightUniform          = "LightPosition";
  GeometryTemplate.VertexAttribName      = "Position";
  GeometryTemplateLast=GeometryTemplate;
  GeometryTemplateLast.Deterministic=!GeometryTemplate.Deterministic;

  //tessellation args
  TessellationParam.ReferenceEdge         = Args->isPresent("--tessellation-start","--tessellation-end","-r");
  TessellationParam.CullSides             = Args->isPresent("--tessellation-start","--tessellation-end","-c");
  TessellationParam.UseStencilValueExport = Args->isPresent("--tessellation-start","--tessellation-end","-sve");
  TessellationParamLast=TessellationParam;
  TessellationParamLast.ReferenceEdge=!TessellationParam.ReferenceEdge;

  //computesoe args
  ComputeSOEParam.WorkGroupSize = Args->getArgi  ("--computesoe-start","--computesoe-end","-w","64");
  ComputeSOEParam.CullSides     = Args->isPresent("--computesoe-start","--computesoe-end","-c");
  ComputeSOEParamLast=ComputeSOEParam;
  ComputeSOEParamLast.CullSides=!ComputeSOEParam.CullSides;

  //compute args
  ComputeParam.WorkGroupSize = Args->getArgi  ("--compute-start","--compute-end","-w","64");
  ComputeParam.CullSides     = Args->isPresent("--compute-start","--compute-end","-c");
  ComputeParamLast=ComputeParam;
  ComputeParamLast.CullSides=!ComputeParam.CullSides;

  //shadowmap args
  ShadowmapParam.Resolution    = Args->getArgi("--shadowmap-start","--shadowmap-end","-r"   ,"1024");
  ShadowmapParam.Fovy          = Args->getArgf("--shadowmap-start","--shadowmap-end","-fovy","45"  );
  ShadowmapParam.FocusPoint[0] = Args->getArgf("--shadowmap-start","--shadowmap-end","-fpx" ,"1"   );
  ShadowmapParam.FocusPoint[1] = Args->getArgf("--shadowmap-start","--shadowmap-end","-fpy" ,"0"   );
  ShadowmapParam.FocusPoint[2] = Args->getArgf("--shadowmap-start","--shadowmap-end","-fpz" ,"1"   );
  ShadowmapParamLast=ShadowmapParam;
  ShadowmapParamLast.Resolution+=1;

  //cubeShadowMapping args
  cubeShadowMappingParam.resolution    = Args->getArgi("--shadowmap-start","--shadowmap-end","-r","1024");
  cubeShadowMappingParam.screenSize[0] = windowParam.size[0];
  cubeShadowMappingParam.screenSize[1] = windowParam.size[1];
  cubeShadowMappingParam.near          = Args->getArgf("--shadowmap-start","--shadowmap-end","-near","1"   );
  cubeShadowMappingParam.far           = Args->getArgf("--shadowmap-start","--shadowmap-end","-far" ,"1000");
  cubeShadowMappingParamLast=cubeShadowMappingParam;
  cubeShadowMappingParamLast.resolution+=1;

  //sintorn args
  SintornParam.WavefrontSize            = Args->getArgi("--sintorn-start","--sintorn-end","-wf"   ,"64");
  SintornParam.ShadowFrustaPerWorkGroup = Args->getArgi("--sintorn-start","--sintorn-end","-sfpwg","1" );
  SintornParamLast=SintornParam;
  SintornParamLast.ShadowFrustaPerWorkGroup++;

  delete Args;

  //create window
  if(TestParam.Test) Window=new ge::util::WindowObject(
      windowParam.size[0],
      windowParam.size[1],
      windowParam.fullscreen,
      TestIdle,
      Mouse,
      false,
      contextParam.version,
      contextParam.profile,
      contextParam.flag);
  else Window=new ge::util::WindowObject(
      windowParam.size[0],
      windowParam.size[1],
      windowParam.fullscreen,
      Idle,
      Mouse,
      !DisableAnttweakbar,
      contextParam.version,
      contextParam.profile,
      contextParam.flag);

  //glBinding::binding::initialize();
  glewExperimental=GL_TRUE;
  glewInit();


  try{
    ge::gl::initShadersAndPrograms();
  }catch(std::string&e){
    std::cerr<<e<<std::endl;
  }


  try{
    GPA=new NDormon::CGpuPerfApi(Window->getContext());
    GPA->EnableComputeShader();
    //GPA->EnableTiming();
    //GPA->EnableLocalMemory();
  }catch(std::string&e){
    std::cerr<<e<<std::endl;
    GPAavalable=false;
  }


  //ge::gl::setDefaultDebugMessage();
  ge::gl::setHighDebugMessage();

  EmptyVAO=new ge::gl::VertexArrayObject();
  //todo addAttrib

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);

  //create camera
  Camera=new ge::util::CameraObject(
      windowParam.size,
      CameraParam.Near,
      CameraParam.Far,
      CameraParam.Fovy);
  Camera->down(15);

  if(TestParam.Test)TestInit();
  else Init();
  Window->mainLoop();

  destroy();

  delete EmptyVAO;
  if(GPAavalable)delete GPA;
  delete Window;
  return 0;
}

void Mouse(){

  if(Window->isLeftDown()){
    Angle[1]+=Window->getDeltaMousePosition()[0]*.01;
    Angle[0]+=Window->getDeltaMousePosition()[1]*.01;
  }
  if(Window->isMiddleDown()){
    Angle[2]+=Window->getDeltaMousePosition()[0]*.01;
  }
}

void Wheel(int d){
}

float Elapsed=0;
float framelen=0;
int FPS=0;
float Speed=.5f;
int Num=0;
int Lapsed=0;

bool AfterGLInit=false;
float MeasureTime=0;
float InitTime=0;
unsigned FrameCount=0;
unsigned StencilMeasurements=0;
unsigned ComputeMeasurements=0;
unsigned LastStencilMeasurement=0;
unsigned LastComputeMeasurement=0;
unsigned long long StencilMeasurementAccum=0;
unsigned long long ComputeMeasurementAccum=0;

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

void TestIdle(){
  if(AfterGLInit==false){
    InitTime+=Window->getDeltaIdleTime();
    if(InitTime>TestParam.TimeBefore)AfterGLInit=true;
  }
  if(AfterGLInit){
    MeasureTime+=Window->getDeltaIdleTime();
    if(LastComputeMeasurement+=QueryTimePassed){
      ComputeMeasurementAccum+=QueryTimePassed;
      ComputeMeasurements++;
      LastComputeMeasurement=QueryTimePassed;
    }
    FrameCount++;
  }
  if(MeasureTime>=TestParam.Duration){
    std::cerr<<"FPS: "<<FrameCount/TestParam.Duration<<std::endl;
    if(StencilMeasurements)
      std::cerr<<"Stencil: "<<StencilMeasurementAccum/StencilMeasurements<<std::endl;
    if(ComputeMeasurements)
      std::cerr<<"Compute: "<<ComputeMeasurementAccum/ComputeMeasurements<<std::endl;
    StencilMeasurements=0;
    ComputeMeasurements=0;
    LastStencilMeasurement=0;
    LastComputeMeasurement=0;
    StencilMeasurementAccum=0;
    ComputeMeasurementAccum=0;

    FrameCount=0;
    MeasureTime=0;
  }
  //std::cout<<"Compute Shader:       "<<QueryTimePassed<<std::endl;

  //std::cerr<<"MeasureTime: "<<MeasureTime<<std::endl;
  //std::cerr<<"InitTime: "<<InitTime<<std::endl;

  if(CameraMation){
    ge::util::CameraKeyPoint Point;
    CameraMation->getCameraPoint(&Point,MeasureTime);
    Camera->setView(Point.position,Point.viewVector,Point.upVector);
    Camera->setFovy(Point.fovy);
  }else{
    Camera->fpsCamera(Angle[0],Angle[1],Angle[2]);
    Camera->right  ((Window->isKeyDown('d')-Window->isKeyDown('a'))*Speed);
    Camera->up     ((Window->isKeyDown(' ')-Window->isKeyDown('c'))*Speed);
    Camera->forward((Window->isKeyDown('w')-Window->isKeyDown('s'))*Speed);
  }
  Pos=Camera->getPosition();

  Camera->getView(&View);
  Camera->getProjection(&Projection);
  mvp=Projection*View;

  if(Window->isKeyDown('g'))light.position[1]-=0.01;
  if(Window->isKeyDown('t'))light.position[1]+=0.01;
  simData->setAsChanged("light");


  DrawGBuffer();

  glDepthFunc(GL_ALWAYS);
  glDepthMask(GL_FALSE);
  DrawAmbient();

  switch(TestParam.Method){
    case SS_GEOMETRY:
      DrawGeometry(&light);
      break;
    case SS_GEOMETRY_SIDES_CAPS:
      DrawGeometrySidesCaps(&light);
      break;
    case SS_COMPUTE_SOE_PLANE:
      DrawComputeSOEPlane(&light);
      break;
    case SS_COMPUTE_SOE:
      DrawComputeSOE(&light);
      break;
    case SS_COMPUTE:
      DrawCompute(&light);
      break;
    case SS_VS:
      DrawVertex(&light);
      break;
    case SS_TS:
      DrawTessellation(&light);
      break;
    case SS_SHADOWMAP:
      mm=Shadowmapping;
      mm->createShadowMask();
      //Shadowmapping->createShadowMask();
      drawDiffuseSpecular(true,&light);
      break;
    case SS_NAVYMAPPING:
      drawNavyMapping(&light);
      break;
    case SS_CUBESHADOWMAP:
      drawCubeShadowmapShadow(&light);
      break;
    case SS_SINTORN:
      DrawSintorn(&light);
      break;
    case SS_NO:
      break;
  }


  if(CameraMation){
    if(!TestParam.Test){
      glEnable(GL_DEPTH_TEST);
      deferred_BlitDepthToDefault(&Deferred);
      glDepthFunc(GL_LESS);
      glBindFramebuffer(GL_FRAMEBUFFER,0);
      glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
      glDisable(GL_BLEND);
      CameraMation->draw(glm::value_ptr(mvp));
    }
  }

  Window->swap();
  framecount++;
}

glm::mat4 camView=glm::mat4(1.f),camProj=glm::perspective(1.5f,1.f,1.f,1000.f);

void Idle(){
  if(CameraParam.Far>=10000)CameraParam.Far=std::numeric_limits<float>::infinity();
  Camera->setNearFar(CameraParam.Near,CameraParam.Far);


  Elapsed+=Window->getDeltaIdleTime();
  framelen+=Window->getDeltaIdleTime();
  float PerNumSecond=1;
  if(Elapsed>=PerNumSecond){
    FPS=framecount/PerNumSecond;
    cout<<"FPS: "<<FPS<<endl;
    cout<<"PerFrame: "<<framelen/framecount*1000<<" miliseconds"<<endl;
    framelen=0;
    framecount=0;
    Elapsed=0;
    if(QueryTimePassed>0){
      Num++;
      Lapsed+=QueryTimePassed;
    }
    std::cerr<<"GBuffer creation:     "<<gbufferQuery->getui64()         <<std::endl;
    std::cout<<"Compute Shader:       "<<QueryTimePassed                 <<std::endl;
    std::cerr<<"Shadow map Creation:  "<<QueryTimePassedShadowmap        <<std::endl;
    std::cerr<<"Shadow mapping:       "<<QueryTimePassedSmapping         <<std::endl;
    std::cerr<<"Sintorn HDB:          "<<HierarchyQuery       ->getui64()<<std::endl;
    std::cerr<<"Sintorn HDT:          "<<HierarchyTextureQuery->getui64()<<std::endl;
    std::cerr<<"Sintorn SF:           "<<ShadowFrustumQuery   ->getui64()<<std::endl;
    std::cerr<<"Sintorn Rasterize:    "<<RasterizeQuery       ->getui64()<<std::endl;
    std::cerr<<"Sintorn Rasterize tex:"<<RasterizeTextureQuery->getui64()<<std::endl;
    std::cerr<<"Sintorn merge:        "<<MergeQuery           ->getui64()<<std::endl;
    std::cerr<<"Sintorn merge texture:"<<MergeTextureQuery    ->getui64()<<std::endl;
    std::cerr<<"Compute Shader avg:   "<<(float)Lapsed/(float)Num        <<std::endl;
  }
  Uint32 Curr=SDL_GetTicks();
  Diff=Curr-Last;
  Last=Curr;

  Camera->fpsCamera(Angle[0],Angle[1],Angle[2]);
  Camera->right  ((Window->isKeyDown('d')-Window->isKeyDown('a'))*Speed);
  Camera->up     ((Window->isKeyDown(' ')-Window->isKeyDown('c'))*Speed);
  Camera->forward((Window->isKeyDown('w')-Window->isKeyDown('s'))*Speed);
  Pos=Camera->getPosition();

  Camera->getView(&View);
  Camera->getProjection(&Projection);
  mvp=Projection*View;



  if(Window->isKeyDown('g')){light.position[1]-=0.4;simData->setAsChanged("light");}
  if(Window->isKeyDown('t')){light.position[1]+=0.4;simData->setAsChanged("light");}


  simData->sendUpdate();


  //std::cerr<<"AFTER KEYBOARD"<<std::endl;

  simulation::Light*LightList[]={
    &light,
    &light2
  };

  if(SSEnable){

    if(SSMeasureGbuffer)gbufferQuery->begin();
    DrawGBuffer();
    if(SSMeasureGbuffer)gbufferQuery->end();

    glDepthFunc(GL_ALWAYS);
    glDepthMask(GL_FALSE);

    if(Window->isKeyOn('q'))exit(0);
    //if(SSMethod!=SS_SINTORN)
    DrawAmbient();

    switch(SSMethod){
      case SS_TS:
        if(TessellationParam!=TessellationParamLast){
          if(TessellationSides)delete TessellationSides;TessellationSides=NULL;
          if(GeometryCapsAlt)delete GeometryCapsAlt;GeometryCapsAlt=NULL;
          TessellationSides=new CTessellationSides(
              &ModelAdjacency,
              TessellationParam.ReferenceEdge,
              TessellationParam.CullSides,
              TessellationParam.UseStencilValueExport);
          GeometryCapsAlt=new CGeometryCapsAlt(&ModelAdjacency);
        }
        break;
      case SS_GEOMETRY:
        //std::cerr<<"SS_GEOMETRY"<<std::endl;
        if(GeometryTemplate!=GeometryTemplateLast){
          //std::cerr<<"SS_GEOMETRYinside"<<std::endl;
          if(GeometrySides)delete GeometrySides;GeometrySides=NULL;
          if(GeometryCaps)delete GeometryCaps;GeometryCaps=NULL;
          GeometrySides=new CGeometrySides(
              &ModelAdjacency,
              GeometryTemplate);
          GeometryCaps=new CGeometryCaps(
              &ModelAdjacency,
              GeometryTemplate);
        }
        //std::cerr<<"SS_GEOMETRYEND"<<std::endl;
        break;
      case SS_GEOMETRY_SIDES_CAPS:
        if(GeometryTemplate!=GeometryTemplateLast){
          if(GeometrySidesCaps)delete GeometrySidesCaps;GeometrySidesCaps=NULL;
          GeometrySidesCaps=new CGeometrySidesCaps(
              &ModelAdjacency,
              GeometryTemplate.Deterministic,
              GeometryTemplate.ReferenceEdge,
              GeometryTemplate.Universal,
              GeometryTemplate.UseVS2GSArray,
              GeometryTemplate.UseVertexArray,
              GeometryTemplate.UseStencilValueExport,
              GeometryTemplate.CullSides,
              GeometryTemplate.FrontCapToInfinity,
              GeometryTemplate.Version);
        }
        break;
      case SS_COMPUTE_SOE_PLANE:
        if(ComputeSOEParam!=ComputeSOEParamLast){
          if(ComputeSidesSOEPlane)delete ComputeSidesSOEPlane;ComputeSidesSOEPlane=NULL;
          if(GeometryCapsAlt)delete GeometryCapsAlt;GeometryCapsAlt=NULL;
          ComputeSidesSOEPlane=new CComputeSidesSOEPlane(
              &ModelAdjacency,
              ComputeSOEParam.WorkGroupSize,
              ComputeSOEParam.CullSides);
          GeometryCapsAlt=new CGeometryCapsAlt(
              &ModelAdjacency);
        }
        break;
      case SS_COMPUTE_SOE:
        if(ComputeSOEParam!=ComputeSOEParamLast){
          if(ComputeSidesSOE)delete ComputeSidesSOE;ComputeSidesSOE=NULL;
          if(GeometryCapsAlt)delete GeometryCapsAlt;GeometryCapsAlt=NULL;
          ComputeSidesSOE=new CComputeSidesSOE(
              &ModelAdjacency,
              ComputeSOEParam.WorkGroupSize,
              ComputeSOEParam.CullSides);
          GeometryCapsAlt=new CGeometryCapsAlt(
              &ModelAdjacency);
        }
        break;
      case SS_COMPUTE:
        //std::cerr<<"SS_COMPUTE"<<std::endl;
        if(ComputeParam!=ComputeParamLast){
          //std::cerr<<"ComputeParam!="<<std::endl;
          if(ComputeSides)delete ComputeSides;ComputeSides=NULL;
          if(GeometryCapsAlt)delete GeometryCapsAlt;GeometryCapsAlt=NULL;
          ComputeSides=new CComputeSides(
              &ModelAdjacency,
              ComputeParam.WorkGroupSize,
              ComputeParam.CullSides);
          GeometryCapsAlt=new CGeometryCapsAlt(
              &ModelAdjacency);
        }
        break;
      case SS_VS:
        //TODO if Vertex param changed
        break;
      case SS_SHADOWMAP:
        if(ShadowmapParam!=ShadowmapParamLast){
          if(Shadowmapping)delete Shadowmapping;Shadowmapping=NULL;
          Shadowmapping = new CShadowMapping(simData);
          //Shadowmapping=new CShadowMapping(ShadowmapParam.Resolution,
          //    windowParam.size,sceneVAO,&ModelAdjacency);
        }
        break;
      case SS_NAVYMAPPING:
        if(ShadowmapParam!=ShadowmapParamLast){
          if(navyMapping)delete navyMapping;navyMapping=NULL;
          navyMapping=new NavyMapping(ShadowmapParam.Resolution,
              windowParam.size,sceneVAO,&ModelAdjacency);
        }
        break;
      case SS_CUBESHADOWMAP:
        if(cubeShadowMappingParam!=cubeShadowMappingParamLast){
          if(cubeShadowMapping)delete cubeShadowMapping;cubeShadowMapping=NULL;
          cubeShadowMapping=new CubeShadowMapping(
              &ModelAdjacency,sceneVAO,cubeShadowMappingParam);
        }
        break;
      case SS_SINTORN:
        //TODO if sintorn param changed
        break;
      default:
        break;
    }

    GeometryTemplateLast       = GeometryTemplate;
    TessellationParamLast      = TessellationParam;
    ComputeSOEParamLast        = ComputeSOEParam;
    ComputeParamLast           = ComputeParam;
    ShadowmapParamLast         = ShadowmapParam;
    cubeShadowMappingParamLast = cubeShadowMappingParam;

    //TODO add this for others methods


    //std::cerr<<"!="<<LastSSMethod<<std::endl;
    if(LastSSMethod!=SSMethod){
      //delete last methods
      switch(LastSSMethod){
        case SS_TS:
          if(TessellationSides)delete TessellationSides;
          if(GeometryCapsAlt)delete GeometryCapsAlt;
          TessellationSides=NULL;
          GeometryCapsAlt=NULL;
          break;
        case SS_GEOMETRY:
          if(GeometrySides)delete GeometrySides;
          if(GeometryCaps)delete GeometryCaps;
          GeometrySides=NULL;
          GeometryCaps=NULL;
          break;
        case SS_GEOMETRY_SIDES_CAPS:
          if(GeometrySidesCaps)delete GeometrySidesCaps;
          GeometrySidesCaps=NULL;
          break;
        case SS_COMPUTE_SOE_PLANE:
          if(ComputeSidesSOEPlane)delete ComputeSidesSOEPlane;
          if(GeometryCapsAlt)delete GeometryCapsAlt;
          ComputeSidesSOEPlane=NULL;
          GeometryCapsAlt=NULL;
          break;
        case SS_COMPUTE_SOE:
          if(ComputeSidesSOE)delete ComputeSidesSOE;
          if(GeometryCapsAlt)delete GeometryCapsAlt;
          ComputeSidesSOE=NULL;
          GeometryCapsAlt=NULL;
          break;
        case SS_COMPUTE:
          if(ComputeSides)delete ComputeSides;
          if(GeometryCapsAlt)delete GeometryCapsAlt;
          ComputeSides=NULL;
          GeometryCapsAlt=NULL;
          break;
        case SS_VS:
          if(VertexSides)delete VertexSides;
          if(VertexCaps)delete  VertexCaps;
          VertexSides=NULL;
          VertexCaps=NULL;
          break;
        case SS_SHADOWMAP:
          if(Shadowmapping)delete Shadowmapping;
          Shadowmapping=NULL;
          break;
        case SS_NAVYMAPPING:
          if(navyMapping)delete navyMapping;
          navyMapping=NULL;
          break;
        case SS_CUBESHADOWMAP:
          if(cubeShadowMapping)delete cubeShadowMapping;
          cubeShadowMapping=NULL;
          break;
        case SS_SINTORN:
          if(Sintorn)delete Sintorn;
          Sintorn=NULL;
          break;
        default:
          break;
      }
      std::cerr<<"!=middle"<<std::endl;
      //create new method
      switch(SSMethod){
        case SS_TS:
          if(TessellationSides==NULL)
            TessellationSides=new CTessellationSides(
                &ModelAdjacency,
                TessellationParam.ReferenceEdge,
                TessellationParam.CullSides,
                TessellationParam.UseStencilValueExport);
          if(GeometryCapsAlt==NULL)
            GeometryCapsAlt=new CGeometryCapsAlt(
                &ModelAdjacency);
          break;
        case SS_GEOMETRY:
          std::cerr<<"GeometrySides==NULL"<<std::endl;
          if(GeometrySides==NULL){
            GeometrySides=new CGeometrySides(
                &ModelAdjacency,
                GeometryTemplate);
          }
          std::cerr<<"GeometrySides==NULLmiddle"<<std::endl;
          if(GeometryCaps==NULL){
            GeometryCaps=new CGeometryCaps(
                &ModelAdjacency,
                GeometryTemplate);
            GeometryTemplateLast=GeometryTemplate;
          }
          std::cerr<<"GeometrySides==NULLend"<<std::endl;
          break;
        case SS_GEOMETRY_SIDES_CAPS:
          if(GeometrySidesCaps==NULL)
            GeometrySidesCaps=new CGeometrySidesCaps(
                &ModelAdjacency,
                GeometryTemplate.Deterministic,
                GeometryTemplate.ReferenceEdge,
                GeometryTemplate.Universal,
                GeometryTemplate.UseVS2GSArray,
                GeometryTemplate.UseVertexArray,
                GeometryTemplate.UseStencilValueExport,
                GeometryTemplate.CullSides,
                GeometryTemplate.FrontCapToInfinity,
                GeometryTemplate.Version);
          break;
        case SS_COMPUTE_SOE_PLANE:
          if(ComputeSidesSOEPlane==NULL)
            ComputeSidesSOEPlane=new CComputeSidesSOEPlane(
                &ModelAdjacency,
                ComputeSOEParam.WorkGroupSize,
                ComputeSOEParam.CullSides);
          if(GeometryCapsAlt==NULL)
            GeometryCapsAlt=new CGeometryCapsAlt(
                &ModelAdjacency);
          break;
        case SS_COMPUTE_SOE:
          if(ComputeSidesSOE==NULL)
            ComputeSidesSOE=new CComputeSidesSOE(
                &ModelAdjacency,
                ComputeSOEParam.WorkGroupSize,
                ComputeSOEParam.CullSides);
          //std::cerr<<"SS_COMPUTE_SOE"<<std::endl;
          if(GeometryCapsAlt==NULL)
            GeometryCapsAlt=new CGeometryCapsAlt(
                &ModelAdjacency);
          break;
        case SS_COMPUTE:
          //std::cerr<<"!=SS_COMPUTE"<<std::endl;

          if(ComputeSides==NULL)
            ComputeSides=new CComputeSides(
                &ModelAdjacency,
                ComputeParam.WorkGroupSize,
                ComputeParam.CullSides);
          //delete ComputeSides;
          //std::cerr<<"aftercomputesides==NULL"<<std::endl;
          if(GeometryCapsAlt==NULL)
            GeometryCapsAlt=new CGeometryCapsAlt(
                &ModelAdjacency);
          break;
        case SS_VS:
          if(VertexSides==NULL)
            VertexSides=new CVertexSides(&ModelAdjacency);
          if(VertexCaps==NULL)
            VertexCaps=new CVertexCaps(&ModelAdjacency);
          break;
        case SS_SHADOWMAP:
          if(Shadowmapping==NULL)
            Shadowmapping = new CShadowMapping(simData);
          /*Shadowmapping=new CShadowMapping(ShadowmapParam.Resolution,windowParam.size,
            sceneVAO,&ModelAdjacency);*/
          break;
        case SS_NAVYMAPPING:
          if(navyMapping==NULL)
            navyMapping=new NavyMapping(ShadowmapParam.Resolution,windowParam.size,
                sceneVAO,&ModelAdjacency);
          break;
        case SS_CUBESHADOWMAP:
          if(cubeShadowMapping==NULL)
            cubeShadowMapping=new CubeShadowMapping(&ModelAdjacency,sceneVAO,
                cubeShadowMappingParam);
          break;
        case SS_SINTORN:
          if(Sintorn==NULL){
            Sintorn=new CSintorn(windowParam.size,&ModelAdjacency,
                SintornParam.WavefrontSize,SintornParam.ShadowFrustaPerWorkGroup);
          }
          break;
        default:
          break;
      }
    }
    //std::cerr<<"!=end\n";


    LastSSMethod=SSMethod;

    for(int l=0;l<1;++l)
      switch(SSMethod){
        case SS_GEOMETRY:
          DrawGeometry(LightList[l]);
          break;
        case SS_GEOMETRY_SIDES_CAPS:
          DrawGeometrySidesCaps(LightList[l]);
          break;
        case SS_COMPUTE_SOE_PLANE:
          DrawComputeSOEPlane(LightList[l]);
          break;
        case SS_COMPUTE_SOE:
          DrawComputeSOE(LightList[l]);
          break;
        case SS_COMPUTE:
          DrawCompute(LightList[l]);
          break;
        case SS_VS:
          DrawVertex(LightList[l]);
          break;
        case SS_TS:
          DrawTessellation(LightList[l]);
          break;
        case SS_SHADOWMAP:
          {
            mm=Shadowmapping;
            if(Window->isKeyOn('p')){
              glm::mat4 pp;
              glm::mat4 vv;
//              if(geometry::getMinimalVP(&pp,&vv,Projection,View,sceneAABB->minPoint,sceneAABB->maxPoint,glm::vec3(LightList[l]->position)))
              if(geometry::getMinVP(&pp,&vv,Projection,View,sceneAABB->minPoint,sceneAABB->maxPoint,glm::vec3(LightList[l]->position)))
                Shadowmapping->setMatrices(pp,vv);
            }
            mm->createShadowMask();
            //Shadowmapping->createShadowMask();
            drawDiffuseSpecular(true,LightList[l]);
          }
          break;
        case SS_NAVYMAPPING:
          drawNavyMapping(LightList[l]);
          break;
        case SS_CUBESHADOWMAP:
          drawCubeShadowmapShadow(LightList[l]);
          break;
        case SS_SINTORN:
          DrawSintorn(LightList[l]);
          break;
        case SS_NO:
          break;
      }
  }else{
    DrawShadowless();
  }
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);
  EmptyVAO->bind();


  //*
  drawAABBProgram->use();
  drawAABBProgram->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));
  for(unsigned i=0;i<sceneBB.size();++i){
    drawAABBProgram->set("minPoint",1,glm::value_ptr(sceneBB[i]->minPoint));
    drawAABBProgram->set("maxPoint",1,glm::value_ptr(sceneBB[i]->maxPoint));
    glDrawArrays(GL_POINTS,0,1);
  }
  // */

  /*
     drawOBBProgram->use();
     drawOBBProgram->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));
     for(unsigned i=0;i<sceneOBB.size();++i){
     drawOBBProgram->set("minPoint",1,glm::value_ptr(sceneOBB[i]->minPoint));
     drawOBBProgram->set("xAxis",1,glm::value_ptr(sceneOBB[i]->xAxis));
     drawOBBProgram->set("yAxis",1,glm::value_ptr(sceneOBB[i]->yAxis));
     drawOBBProgram->set("zAxis",1,glm::value_ptr(sceneOBB[i]->zAxis));
     glDrawArrays(GL_POINTS,0,1);
     }*/
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);


  EmptyVAO->unbind();

  //std::cerr<<"after draw shdowless"<<std::endl;
  if(SSMethod!=SS_SINTORN){
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    if(DSDrawEverything){
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_STENCIL_TEST);
    }
    glDepthFunc(GL_LEQUAL);
    if(DSDrawLightSilhouette){
      Deferred.fbo->bind(GL_READ_FRAMEBUFFER);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
      glBlitFramebuffer(0,0,Deferred.Size[0],Deferred.Size[1],0,0,Deferred.Size[0],Deferred.Size[1],
          GL_DEPTH_BUFFER_BIT,GL_NEAREST);

      DrawSilhouetteShader->use();
      DrawSilhouetteShader->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));
      DrawSilhouetteShader->set("LightPosition",1,glm::value_ptr(LightList[0]->position));
      //glBindVertexArray(DrawSilhouetteVAO);
      EmptyVAO->bind();
      glDrawArrays(GL_POINTS,0,ModelAdjacency.NumEdges);
      EmptyVAO->unbind();
      //glBindVertexArray(0);
    }

    if(DSDrawViewSilhouette){
      Deferred.fbo->bind(GL_READ_FRAMEBUFFER);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
      glBlitFramebuffer(0,0,Deferred.Size[0],Deferred.Size[1],0,0,Deferred.Size[0],Deferred.Size[1],
          GL_DEPTH_BUFFER_BIT,GL_NEAREST);

      DrawSilhouetteShader->use();
      DrawSilhouetteShader->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));
      DrawSilhouetteShader->set("LightPosition",-Pos[0],-Pos[1],-Pos[2],1.f);
      //glBindVertexArray(DrawSilhouetteVAO);
      EmptyVAO->bind();
      glDrawArrays(GL_POINTS,0,ModelAdjacency.NumEdges);
      EmptyVAO->unbind();
      //glBindVertexArray(0);
    }

    glEnable(GL_DEPTH_TEST);
    deferred_BlitDepthToDefault(&Deferred);
    glDepthFunc(GL_LESS);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    glDisable(GL_BLEND);
    //CameraMation->draw(glm::value_ptr(mvp));
  }

  if(navyMapping){
    if(Window->isKeyDown('z')){
      camView=View;
      camProj=Projection;
    }
    test::drawConvexHull(
        View,Projection,
        camProj,camView,
        sceneAABB->minPoint,sceneAABB->maxPoint,
        glm::vec3(light.position));
  }



  if(SSMethod==SS_NAVYMAPPING&&false){
    //navyMapping->drawShadowMap(0,0,.5);
    navyMapping->writeViewSamples(Deferred.position->getId());
    //navyMapping->drawCountMap(0,0,.5);
    navyMapping->prefixSum(true);
    //navyMapping->drawPrefixSum(.5,0,.5);
    navyMapping->drawCountMap(.5,0,.5);

    navyMapping->smoothGrid(true);
    glDisable(GL_DEPTH_TEST);
    navyMapping->drawGrid(.5,0,.5,true);
    glEnable(GL_DEPTH_TEST);

    navyMapping->unwarpX(Deferred.position->getId());
    //navyMapping->drawUnwarpX(.0,0,.5);  

    navyMapping->prefixSum(false);
    navyMapping->smoothGrid(false);

    navyMapping->createNavyMap(glm::value_ptr(Model));
    navyMapping->drawShadowMap(Window->isKeyOn('u'),0,0,.5);

    glDisable(GL_DEPTH_TEST);
    navyMapping->drawGrid(0,0,.5,false);
    glEnable(GL_DEPTH_TEST);
  }
  //std::cerr<<"before swap"<<std::endl;
  Window->swap();
  framecount++;
  /*
     std::cerr<<"measure.shadowMap.createShadowMap:  "<<(*simData->getGauge("measure.shadowMap.createShadowMap" ,NULL)->getData()).size()<<std::endl;
     std::cerr<<"measure.shadowMap.createShadowMask: "<<(*simData->getGauge("measure.shadowMap.createShadowMask",NULL)->getData()).size()<<std::endl;
     */
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
  sceneDIBO->bind(GL_DRAW_INDIRECT_BUFFER);
  glMultiDrawArraysIndirect(GL_TRIANGLES,NULL,sceneDIBOSize,sizeof(unsigned)*4);
  //glDrawArrays(GL_TRIANGLES,0,SceneNumTriangles*3);
  sceneVAO->unbind();
  if(Window->isKeyOn('f'))glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

void ShaderSetMatrix(ge::gl::ProgramObject*P){
  P->set("m",1,GL_FALSE,glm::value_ptr(Model));
  P->set("v",1,GL_FALSE,glm::value_ptr(View));
  P->set("p",1,GL_FALSE,glm::value_ptr(Projection));
  P->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));
}

void DrawGBuffer(){
  //*  
  frustumCullingProgram->use();
  sceneDIBO    ->bindBase(GL_SHADER_STORAGE_BUFFER,FRUSTUMCULLING_BINDING_DIBO);
  sceneAABBData->bindBase(GL_SHADER_STORAGE_BUFFER,FRUSTUMCULLING_BINDING_AABB);
  frustumCullingProgram->set("numAABB",(unsigned)sceneDIBOSize);
  frustumCullingProgram->set("mvp",1,GL_FALSE,glm::value_ptr(mvp));
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
      GPA->BeginSession();
      for(unsigned p=0;p<GPA->GetNumPasses();++p){
        GPA->BeginPass();

        GPA->BeginSample(0);
        Sintorn->GenerateHierarchyTexture(Deferred.depth->getId(),Deferred.normal->getId(),glm::value_ptr(L->position));
        //Sintorn->GenerateHierarchy(Deferred.depth->getId(),Deferred.normal->getId(),L->Position);
        GPA->EndSample();

        GPA->EndPass();
      }
      GPA->EndSession();
      std::cerr<<GPA->GetResults()<<std::endl;
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
      GPA->BeginSession();
      for(unsigned p=0;p<GPA->GetNumPasses();++p){
        GPA->BeginPass();

        GPA->BeginSample(0);

        Sintorn->RasterizeTexture();
        //Sintorn->Rasterize();

        GPA->EndSample();

        GPA->EndPass();
      }
      GPA->EndSession();
      std::cerr<<GPA->GetResults()<<std::endl;
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

  if(TestParam.MeasureCompute)
    glBeginQuery(GL_TIME_ELAPSED,QueryTime);

  ComputeSidesSOEPlane->ComputeSides(glm::value_ptr(mvp),Light);

  if(TestParam.MeasureCompute){
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectuiv(QueryTime,GL_QUERY_RESULT_NO_WAIT,&QueryTimePassed);
  }

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
      GPA->BeginSession();
      for(unsigned p=0;p<GPA->GetNumPasses();++p){
        GPA->BeginPass();

        GPA->BeginSample(0);
        ComputeSidesSOE->ComputeSides(glm::value_ptr(mvp),Light);

        GPA->EndSample();

        GPA->EndPass();
      }
      GPA->EndSession();
      std::cerr<<GPA->GetResults()<<std::endl;
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
  if(TestParam.MeasureCompute)
    glBeginQuery(GL_TIME_ELAPSED,QueryTime);

  ComputeSides->ComputeSides(glm::value_ptr(mvp),Light);

  if(TestParam.MeasureCompute){
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectuiv(QueryTime,GL_QUERY_RESULT_NO_WAIT,&QueryTimePassed);
  }

  setGLForStencil(SSZFail);
  if(TestParam.MeasureStencil)stencilShadowsQuery->begin();
  ComputeSides->DrawSides(glm::value_ptr(mvp),Light);
  if(SSZFail)GeometryCapsAlt->DrawCaps(glm::value_ptr(mvp),Light);
  if(TestParam.MeasureStencil)stencilShadowsQuery->end();
  drawStencil(Light);
}

void drawCubeShadowmapShadow(simulation::Light*Light){
  if(SSMeasureShadowmap)
    glBeginQuery(GL_TIME_ELAPSED,QueryTime);

  cubeShadowMapping->createShadowMap(glm::value_ptr(Model),Light);

  if(SSMeasureShadowmap){
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectuiv(QueryTime,GL_QUERY_RESULT_NO_WAIT,&QueryTimePassedShadowmap);
  }

  if(SSMeasureSmapping)
    glBeginQuery(GL_TIME_ELAPSED,QueryTime);

  cubeShadowMapping->drawShadowed(glm::value_ptr(Pos),Light);

  if(SSMeasureSmapping){
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectuiv(QueryTime,GL_QUERY_RESULT_NO_WAIT,&QueryTimePassedSmapping);
  }

}


void drawNavyMapping(simulation::Light*Light){
  if(SSMeasureShadowmap)
    glBeginQuery(GL_TIME_ELAPSED,QueryTime);

  //navyMapping->setFocus(Light,ShadowmapParam.FocusPoint,ShadowmapParam.Fovy);
  navyMapping->setFocus(Light,(float*)glm::value_ptr(sceneAABB->getCenter()),ShadowmapParam.Fovy);
  navyMapping->createShadowMap(glm::value_ptr(Model));
  if(Window->isKeyOn('u')){
    navyMapping->writeViewSamples(Deferred.position->getId());
    navyMapping->prefixSum(true);
    navyMapping->smoothGrid(true);
    navyMapping->unwarpX(Deferred.position->getId());
    navyMapping->prefixSum(false);
    navyMapping->smoothGrid(false);
    navyMapping->createNavyMap(glm::value_ptr(Model));
  }


  if(SSMeasureShadowmap){
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectuiv(QueryTime,GL_QUERY_RESULT_NO_WAIT,&QueryTimePassedShadowmap);
  }

  if(SSMeasureSmapping)
    glBeginQuery(GL_TIME_ELAPSED,QueryTime);

  if(Window->isKeyOn('u'))
    navyMapping->drawNavyShadowed(Window->isKeyOn('j'),glm::value_ptr(Pos),Light);
  else
    navyMapping->drawShadowed(glm::value_ptr(Pos),Light);


  if(SSMeasureSmapping){
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectuiv(QueryTime,GL_QUERY_RESULT_NO_WAIT,&QueryTimePassedSmapping);
  }

  navyMapping->minProjection(Projection,View,Projection,View);

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
  DrawDiffuseSpecular(false,false,&light);
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

void TestInit(){
  try{
    DrawShader = new ge::gl::ProgramObject(
        ShaderDir+"app/vertex_dgb.vp",
        ShaderDir+"app/geometry_dgb.gp",
        ShaderDir+"app/fragment_dgb.fp");
  }catch(std::string&e){
    std::cerr<<e<<endl;
  }

  CameraMation=new ge::util::CameraPath();
  if(TestParam.CameraMationFile!="")
    CameraMation->loadCSV(TestParam.CameraMationFile);
  CameraMation->setDuration(TestParam.Duration);

  deferred_Init(&Deferred,windowParam.size[0],windowParam.size[1]);
  InitDrawStencilToTexture();
  SetMatrix();
  InitModel(ModelFile.c_str());
  glGenQueries(1,&QueryTime);

  switch(TestParam.Method){
    case SS_TS:
      if(TessellationParam!=TessellationParamLast){
        if(TessellationSides)delete TessellationSides;TessellationSides=NULL;
        if(GeometryCapsAlt)delete GeometryCapsAlt;GeometryCapsAlt=NULL;
        TessellationSides=new CTessellationSides(
            &ModelAdjacency,
            TessellationParam.ReferenceEdge,
            TessellationParam.CullSides,
            TessellationParam.UseStencilValueExport);
        GeometryCapsAlt=new CGeometryCapsAlt(&ModelAdjacency);
      }
      break;
    case SS_GEOMETRY:
      if(GeometryTemplate!=GeometryTemplateLast){
        if(GeometrySides)delete GeometrySides;GeometrySides=NULL;
        if(GeometryCaps)delete GeometryCaps;GeometryCaps=NULL;
        GeometrySides=new CGeometrySides(
            &ModelAdjacency,
            GeometryTemplate);
        GeometryCaps=new CGeometryCaps(
            &ModelAdjacency,
            GeometryTemplate);
      }
      break;
    case SS_GEOMETRY_SIDES_CAPS:
      if(GeometryTemplate!=GeometryTemplateLast){
        if(GeometrySidesCaps)delete GeometrySidesCaps;GeometrySidesCaps=NULL;
        GeometrySidesCaps=new CGeometrySidesCaps(
            &ModelAdjacency,
            GeometryTemplate.Deterministic,
            GeometryTemplate.ReferenceEdge,
            GeometryTemplate.Universal,
            GeometryTemplate.UseVS2GSArray,
            GeometryTemplate.UseVertexArray,
            GeometryTemplate.UseStencilValueExport,
            GeometryTemplate.CullSides,
            GeometryTemplate.FrontCapToInfinity,
            GeometryTemplate.Version);
      }
      break;
    case SS_COMPUTE_SOE_PLANE:
      if(ComputeSOEParam!=ComputeSOEParamLast){
        if(ComputeSidesSOEPlane)delete ComputeSidesSOEPlane;ComputeSidesSOEPlane=NULL;
        if(GeometryCapsAlt)delete GeometryCapsAlt;GeometryCapsAlt=NULL;
        ComputeSidesSOEPlane=new CComputeSidesSOEPlane(
            &ModelAdjacency,
            ComputeSOEParam.WorkGroupSize,
            ComputeSOEParam.CullSides);
        GeometryCapsAlt=new CGeometryCapsAlt(
            &ModelAdjacency);
      }
      break;
    case SS_COMPUTE_SOE:
      if(ComputeSOEParam!=ComputeSOEParamLast){
        if(ComputeSidesSOE)delete ComputeSidesSOE;ComputeSidesSOE=NULL;
        if(GeometryCapsAlt)delete GeometryCapsAlt;GeometryCapsAlt=NULL;
        ComputeSidesSOE=new CComputeSidesSOE(
            &ModelAdjacency,
            ComputeSOEParam.WorkGroupSize,
            ComputeSOEParam.CullSides);
        GeometryCapsAlt=new CGeometryCapsAlt(
            &ModelAdjacency);
      }
      break;
    case SS_COMPUTE:
      if(ComputeParam!=ComputeParamLast){
        if(ComputeSides)delete ComputeSides;ComputeSides=NULL;
        if(GeometryCapsAlt)delete GeometryCapsAlt;GeometryCapsAlt=NULL;
        ComputeSides=new CComputeSides(
            &ModelAdjacency,
            ComputeParam.WorkGroupSize,
            ComputeParam.CullSides);
        GeometryCapsAlt=new CGeometryCapsAlt(
            &ModelAdjacency);
      }
      break;
    case SS_VS:
      if(VertexSides)delete VertexSides;VertexSides = NULL;
      if(VertexCaps )delete VertexCaps ;VertexCaps  = NULL;
      VertexSides = new CVertexSides(&ModelAdjacency);
      VertexCaps  = new CVertexCaps (&ModelAdjacency);
      break;
    case SS_SHADOWMAP:
      if(Shadowmapping)delete Shadowmapping;Shadowmapping=NULL;
      Shadowmapping = new CShadowMapping(simData);
      //Shadowmapping = new CShadowMapping(ShadowmapParam.Resolution,windowParam.size,
      //    sceneVAO,&ModelAdjacency);
      break;
    case SS_NAVYMAPPING:
      if(navyMapping)delete navyMapping;navyMapping=NULL;
      navyMapping = new NavyMapping(ShadowmapParam.Resolution,windowParam.size,
          sceneVAO,&ModelAdjacency);
      break;
    case SS_CUBESHADOWMAP:
      if(cubeShadowMapping)delete cubeShadowMapping;cubeShadowMapping=NULL;
      cubeShadowMapping=new CubeShadowMapping(&ModelAdjacency,sceneVAO,cubeShadowMappingParam);
      break;
    case SS_SINTORN:
      if(Sintorn)delete Sintorn;Sintorn=NULL;
      Sintorn=new CSintorn(windowParam.size,&ModelAdjacency,
          SintornParam.WavefrontSize,SintornParam.ShadowFrustaPerWorkGroup);
      break;
    default:
      break;
  }


}

void Init(){
  std::cerr<<"Init()"<<std::endl;

  /*
     simulation::SimulationData *sData=new simulation::SimulationData();
     sData->insertVariable("necoi",new simulation::Int(12));
     sData->insertVariable("necof",new simulation::Float(3.4));
     sData->insertVariable("geom.cull",new simulation::Int(1));
     sData->insertVariable("geom.array",new simulation::Int(4));
     sData->insertVariable("geom.name",new simulation::String("mvp"));
     sData->insertVariable("geom.cap.inf",new simulation::Int(0));
     sData->insertVariable("geom.cap.amd",new simulation::Int(43));
     sData->insertVariable("geom.cap.size",new simulation::Vec2(glm::vec2(12.f,14.f)));
     sData->insertVariable("geom.cap.so",new simulation::Vec4(glm::vec4(12.f,14.f,32.f,1.f)));


     std::cerr<<sData->toStr()<<std::endl;
     exit(0);
     */





  glGenQueries(1,&QueryTime);
  MergeQuery=new ge::gl::AsynchronousQueryObject(GL_TIME_ELAPSED,GL_QUERY_RESULT_NO_WAIT,ge::gl::AsynchronousQueryObject::UINT64);
  MergeTextureQuery     = new ge::gl::AsynchronousQueryObject(MergeQuery);
  RasterizeQuery        = new ge::gl::AsynchronousQueryObject(MergeQuery);
  RasterizeTextureQuery = new ge::gl::AsynchronousQueryObject(MergeQuery);
  HierarchyQuery        = new ge::gl::AsynchronousQueryObject(MergeQuery);
  HierarchyTextureQuery = new ge::gl::AsynchronousQueryObject(MergeQuery);
  ShadowFrustumQuery    = new ge::gl::AsynchronousQueryObject(MergeQuery);
  stencilShadowsQuery   = new ge::gl::AsynchronousQueryObject(MergeQuery);
  gbufferQuery          = new ge::gl::AsynchronousQueryObject(MergeQuery);

  programPipeline=new ge::gl::ProgramPipelineObject();

  //create camera mation
  //CameraMation=new ge::util::CameraPath();

  try{
    DrawShader = new ge::gl::ProgramObject(
        ShaderDir+"app/vertex_dgb.vp",
        ShaderDir+"app/geometry_dgb.gp",
        ShaderDir+"app/fragment_dgb.fp");

    DrawSilhouetteShader=new ge::gl::ProgramObject(
        ShaderDir+"app/drawsilhouette.vp",
        ShaderDir+"app/drawsilhouette.gp",
        ShaderDir+"app/drawsilhouette.fp");
  }catch(std::string&e){
    std::cerr<<e<<std::endl;
  }
  std::cerr<<"Tady"<<std::endl;
  /*  ManifoldHullCap=new ge::gl::CShaderProgram(
      ShaderDir+"app/manifoldhullcap.vp",
      ShaderDir+"app/manifoldhullcap.gp",
      ShaderDir+"app/manifoldhullcap.fp");*/

  /*
     SVSSilTemplate TVS;
     TVS.Version=100;
     TVS.UseLayouts=true;
     TVS.UseVertexAttribArray=false;
     TVS.LightPositionUniformName="lightPosition";
     TVS.MatrixUniformName="mvp";
     TVS.VertexAttribName="Position";
     TVS.Universal=true;
     std::cerr<<GenVSSilVertexCap(TVS)<<std::endl;
     std::cerr<<GenVSSilFragmentCap(TVS)<<std::endl;
     exit(0);
  //  */
  ge::gl::ProgramObject*webglVertexSide=new ge::gl::ProgramObject(
      ShaderDir+"methods/Vertex/webgl_vertex_side.vp",
      ShaderDir+"methods/Vertex/webgl_vertex_side.fp"
      );
  ge::gl::ProgramObject*webglVertexCap=new ge::gl::ProgramObject(
      ShaderDir+"methods/Vertex/webgl_vertex_cap.vp",
      ShaderDir+"methods/Vertex/webgl_vertex_side.fp"
      );

  delete webglVertexSide;
  delete webglVertexCap;

  deferred_Init(&Deferred,windowParam.size[0],windowParam.size[1]);
  InitDrawStencilToTexture();


  SetMatrix();

  InitModel(ModelFile.c_str());

  std::cerr<<"asasas"<<std::endl;
  drawAABBProgram = new ge::gl::ProgramObject(
      ShaderDir+"app/drawaabb.vp",
      ShaderDir+"app/drawaabb.gp",
      ShaderDir+"app/drawaabb.fp");
  std::cerr<<"asasas2"<<std::endl;

  drawOBBProgram = new ge::gl::ProgramObject(
      ShaderDir+"app/drawaabb.vp",
      ShaderDir+"app/drawobb.gp",
      ShaderDir+"app/drawaabb.fp");
  std::cerr<<"asasas3"<<std::endl;

  drawTriangleProgram = new ge::gl::ProgramObject(
      ShaderDir+"app/drawTriangle.vp",
      ShaderDir+"app/drawTriangle.gp",
      ShaderDir+"app/drawTriangle.fp");
  std::cerr<<"asasas4"<<std::endl;


  simpleDraw = new DrawPrimitive(ShaderDir+"app/");

  std::cerr<<"asasas5"<<std::endl;


  //*
  TwBar*GeometryBar;
  GeometryBar=TwNewBar("GeometryBar");
  TwAddVarRW(GeometryBar,"Deterministic"     ,TW_TYPE_BOOLCPP,&GeometryTemplate.Deterministic        ," help='Toggle determinism on.' "          );
  TwAddVarRW(GeometryBar,"Reference Edge"    ,TW_TYPE_BOOLCPP,&GeometryTemplate.ReferenceEdge        ," help='Toggle reference edge on.' "       );
  TwAddVarRW(GeometryBar,"Universal"         ,TW_TYPE_BOOLCPP,&GeometryTemplate.Universal            ," help='Toggle universal on.' "            );
  TwAddVarRW(GeometryBar,"UseVS2GSArray"     ,TW_TYPE_BOOLCPP,&GeometryTemplate.UseVS2GSArray        ," help='Toggle vs2gs array on.' "          );
  TwAddVarRW(GeometryBar,"StencilValueExport",TW_TYPE_BOOLCPP,&GeometryTemplate.UseStencilValueExport," help='Toggle stencil value export on.' " );
  TwAddVarRW(GeometryBar,"CullSides"         ,TW_TYPE_BOOLCPP,&GeometryTemplate.CullSides            ," help='Toggle cull sides on.' "           );
  TwAddVarRW(GeometryBar,"FrontCapToInfinity",TW_TYPE_BOOLCPP,&GeometryTemplate.FrontCapToInfinity   ," help='Toggle front cap to infinity on.' ");
  // */
  TwBar*ComputeSOEBar;
  ComputeSOEBar=TwNewBar("ComputeSOEBar");
  TwAddVarRW(ComputeSOEBar,"CullSides"    ,TW_TYPE_BOOLCPP,&ComputeSOEParam.CullSides    ," help='Toggle cull sides on.' "               );
  TwAddVarRW(ComputeSOEBar,"WorkGroupSize",TW_TYPE_UINT32 ,&ComputeSOEParam.WorkGroupSize," label='Work group size' min=8 max=640 step=8");
  /*
     TwBar*ComputeBar;
     ComputeBar=TwNewBar("ComputeBar");
     TwAddVarRW(ComputeBar,"CullSides",TW_TYPE_BOOLCPP,
     &ComputeParam.CullSides,
     " help='Toggle cull sides on.' ");
     TwAddVarRW(ComputeBar,"WorkGroupSize",TW_TYPE_UINT32,&ComputeParam.WorkGroupSize,
     " label='Work group size' min=8 max=640 step=8");

     TwBar*ShadowmapBar;
     ShadowmapBar=TwNewBar("ShadowmapBar");
     TwAddVarRW(ShadowmapBar,"Resolution",TW_TYPE_UINT32,&ShadowmapParam.Resolution,
     " label='Resolution' min=128 max=8192 step=128");
     TwAddVarRW(ShadowmapBar,"Fovy",TW_TYPE_FLOAT,&ShadowmapParam.Fovy,
     " label='Field of view' min=1 max=179 step=1");
     TwAddVarRW(ShadowmapBar,"FocusX",TW_TYPE_FLOAT,ShadowmapParam.FocusPoint+0,
     " label='FocusX' min=-1000 max=1000 step=1");
     TwAddVarRW(ShadowmapBar,"FocusY",TW_TYPE_FLOAT,ShadowmapParam.FocusPoint+1,
     " label='FocusY' min=-1000 max=1000 step=1");
     TwAddVarRW(ShadowmapBar,"FocusZ",TW_TYPE_FLOAT,ShadowmapParam.FocusPoint+2,
     " label='FocusZ' min=-1000 max=1000 step=1");


     TwBar*SintornBar;
     SintornBar=TwNewBar("SintornBar");
     TwAddVarRW(SintornBar,"Wavefront Size",TW_TYPE_UINT32,&SintornParam.WavefrontSize,
     " label='Wavefront size' min=8 max=64 step=8");
     TwAddVarRW(SintornBar,"ShadowFrustaPerWorkGroup",TW_TYPE_UINT32,
     &SintornParam.ShadowFrustaPerWorkGroup,
     " label='Shadow Frusta Per Work Group' min=1 max=10 step=8");

     TwBar*Bar2;
     Bar2=TwNewBar("Camera");
     TwAddVarRW(Bar2,"Fovy",TW_TYPE_FLOAT,&Camera->Fovy,
     " label='Field of view' min=1 max=179 step=1");
     TwAddVarRW(Bar2,"Near",TW_TYPE_FLOAT,&Camera->Near,
     " label='Near Plane' min=0.1 max=10000 step=0.1");


     TwBar*Bar3;
     Bar3=TwNewBar("CameraMation");
     TwDefine(" CameraMation label='CameraMation' fontSize=1 position='800 16' size='200 200' valuesWidth=100 ");

     TwCopyStdStringToClientFunc(CopyStdStringToClient);
     TwAddButton(Bar3,
     "Insert Key Point",
     InsertKeyPointFromCamera,
     Camera, 
     "label='Insert Key Point'");
     TwAddButton(Bar3,
     "Save to file",
     SaveCameraMation,
     &TestParam.CameraMationFile, 
     "label='save to file'");
     TwAddVarRW(Bar3, "CameraMationFile", TW_TYPE_STDSTRING, &TestParam.CameraMationFile, 
     " label='Save file' group=StdString help='Define a title for the new tweak bar.' ");
     */
  TwBar*cameraBar=TwNewBar("cameraBar");
  TwAddVarRW(cameraBar,"near",TW_TYPE_FLOAT,&CameraParam.Near," label='near' help='near plane' min='0.1' max='1000' step=0.1");
  TwAddVarRW(cameraBar,"far" ,TW_TYPE_FLOAT,&CameraParam.Far ," label='far' help='far plane' min='10' max='10000' step=1"    );


  TwBar*Bar;
  Bar=TwNewBar("TweakBar");
  TwAddVarRO(Bar,"FPS"    ,TW_TYPE_INT32  ,&FPS       ," label='FPS' help='Frames per second' "         );
  TwAddVarRW(Bar,"Speed"  ,TW_TYPE_FLOAT  ,&Speed     ," label='Movement speed' min=0 max=2 step=0.01"  );
  TwAddVarRW(Bar,"SSAO"   ,TW_TYPE_BOOLCPP,&SSAOEnable," help='Toggle screenspace ambient occlussion.' ");
  TwAddVarRW(Bar,"Shadows",TW_TYPE_BOOLCPP,&SSEnable  ," help='Toggle shadows on.' "                    );

  TwEnumVal MethodDef[]={
    {SS_GEOMETRY           ,"geometry"                           },
    {SS_GEOMETRY_SIDES_CAPS,"geometry_sides_caps"                },
    {SS_COMPUTE_SOE_PLANE  ,"compute soe plane"                  },
    {SS_COMPUTE_SOE        ,"compute soe"                        },
    {SS_COMPUTE            ,"compute"                            },
    {SS_SHADOWMAP          ,"shadowmapping"                      },
    {SS_NAVYMAPPING        ,"navymapping"                        },
    {SS_CUBESHADOWMAP      ,"cubeshadowmapping"                  },
    {SS_VS                 ,"Vertex Shader Universal"            },
    {SS_TS                 ,"Tessellation Shader Silhouette Edge"},
    {SS_SINTORN            ,"Sintorn"                            },
    {SS_NO                 ,"No shadows"                         }
  };
  TwType MethodType=TwDefineEnum("SS mode",MethodDef,sizeof(MethodDef)/sizeof(TwEnumVal));
  TwAddVarRW(Bar,"Method",MethodType,&SSMethod,
      "help='Change shadow volume stencil buffer type' ");


  TwAddVarRW(Bar,"Deterministic",TW_TYPE_BOOLCPP,&SSDeterministic," help='Toggle determinism on.' ");
  TwAddVarRW(Bar,"ZFail"        ,TW_TYPE_BOOLCPP,&SSZFail        ," help='Toggle ZFail on.' "      );

  TwAddVarRW(Bar,"ReferenceEdge",TW_TYPE_BOOLCPP,&SSReferenceEdge," help='Toggle Reference Edge.' "  );
  TwAddVarRW(Bar,"CullSides"    ,TW_TYPE_BOOLCPP,&SSCullSides    ," help='Toggle Culling of sides.' ");

  TwAddVarRW(Bar,"WithoutGBuffer"           ,TW_TYPE_BOOLCPP,&SSWithoutGBuffer   ," help='Unet GBuffer'"                                  );
  TwAddVarRW(Bar,"MeasureGBuffer"           ,TW_TYPE_BOOLCPP,&SSMeasureGbuffer   ," help='Toggle Measurement of GBuffer'"                 );
  TwAddVarRW(Bar,"MeasureStencil"           ,TW_TYPE_BOOLCPP,&SSMeasureStencil   ," help='Toggle Measurement of stencil'"                 );
  TwAddVarRW(Bar,"MeasureCompute"           ,TW_TYPE_BOOLCPP,&SSMeasureCompute   ," help='Toggle Measurement of compute'"                 );
  TwAddVarRW(Bar,"MeasureShadowmapCreation" ,TW_TYPE_BOOLCPP,&SSMeasureShadowmap ," help='Toggle Measurement of Shadowmap Creation'"      );
  TwAddVarRW(Bar,"Measure Shadow mapping"   ,TW_TYPE_BOOLCPP,&SSMeasureSmapping  ," help='Toggle Measurement of Shadow mapping'"          );
  TwAddVarRW(Bar,"Measure Shadow Blitting"  ,TW_TYPE_BOOLCPP,&SSMeasureSBlit     ," help='Toggle Measurement of Shadow Blitting'"         );
  TwAddVarRW(Bar,"Measure SM Quad"          ,TW_TYPE_BOOLCPP,&SSMeasureSMQuad    ," help='Toggle Measurement of SM Quad'"                 );
  TwAddVarRW(Bar,"Measure Vertex Silhouette",TW_TYPE_BOOLCPP,&SSMeasureVS        ," help='Toggle Measurement of Vertex Shader Silhouette'");
  TwAddVarRW(Bar,"Measure Sintorn HDB"      ,TW_TYPE_BOOLCPP,&SSMeasureSintornHDB," help='Toggle Measurement of Sintorn HDB'"             );


  TwAddVarRW(Bar,"Draw Light Silhouette" ,TW_TYPE_BOOLCPP,&DSDrawLightSilhouette," help='Toggle drawing of Light Silhouettes'" );
  TwAddVarRW(Bar,"Draw Camera Silhouette",TW_TYPE_BOOLCPP,&DSDrawViewSilhouette ," help='Toggle drawing of Camera Silhouettes'");
  TwAddVarRW(Bar,"Draw Every Silhouette" ,TW_TYPE_BOOLCPP,&DSDrawEverything     ," help='Toggle drawing of every Silhouettes'" );


  test::setTestConvexHull(simpleDraw);





  LastSSMethod=SSMethod;

  shadowMask=new ge::gl::TextureObject(GL_TEXTURE_2D,GL_R32F,1,
      simData->getIVec2("window.size",glm::ivec2(1024)).x,
      simData->getIVec2("window.size",glm::ivec2(1024)).y);

  simData->insertVariable("emptyVAO" ,new simulation::Object(EmptyVAO       ));
  simData->insertVariable("sceneVAO" ,new simulation::Object(sceneVAO       ));
  simData->insertVariable("light"    ,&light                                 );
  simData->insertVariable("adjacency",new simulation::Object(&ModelAdjacency));
  simData->insertVariable("gbuffer.position",new simulation::Object(Deferred.position));
  simData->insertVariable("gbuffer.fbo"     ,new simulation::Object(Deferred.fbo     ));
  simData->insertVariable("shadowMask",new simulation::Object(shadowMask));
  simData->insertVariable("measure.shadowMap.createShadowMap" ,new simulation::Gauge());
  simData->insertVariable("measure.shadowMap.createShadowMask",new simulation::Gauge());
  simData->insertVariable("camera",new simulation::Object(Camera));


  std::cerr<<simData->toStr()<<std::endl;
  std::cerr<<simData->define("shadowMapMethods");
  /*std::cerr<<"#"<<simData->getString("shaderDirectory","")<<"#"<<std::endl;
    std::cerr<<simData->getKeys();
    std::cerr<<"count: "<<simData->_variable.count("shaderDirectory")<<std::endl;
    */
  std::cerr<<"after init"<<std::endl;
}

void SetMatrix(){
  Camera->getProjection(&Projection);
  Camera->getView(&View);
  Model=glm::mat4(1.);
  mvp=Projection*View*Model;
}

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
        ptr[actface*3*2*3+i*6+0]=V->x*.1*3;
        ptr[actface*3*2*3+i*6+1]=V->y*.1*3-5*3;
        ptr[actface*3*2*3+i*6+2]=V->z*.1*3;
        RawTriangles[actface*3*3+i*3+0]=V->x*.1*3;
        RawTriangles[actface*3*3+i*3+1]=V->y*.1*3-5*3;
        RawTriangles[actface*3*3+i*3+2]=V->z*.1*3;
        ptr[actface*3*2*3+i*6+3]=N->x;
        ptr[actface*3*2*3+i*6+4]=N->y;
        ptr[actface*3*2*3+i*6+5]=N->z;
        sceneAABB->addPoint(glm::vec3(V->x*.1*3,V->y*.1*3-5*3,V->z*.1*3));
        aabb->addPoint(glm::vec3(V->x*.1*3,V->y*.1*3-5*3,V->z*.1*3));
        center+=glm::vec3(glm::vec3(V->x*.1*3,V->y*.1*3-5*3,V->z*.1*3));
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

    /*
       OrthoBoundingBox*obb=new OrthoBoundingBox();
       obb->xAxis=glm::vec3(0.f);
       for(unsigned face=0;face<Mesh->mNumFaces;++face){//loop over faces
       aiFace*Face=Mesh->mFaces+face;
       for(unsigned i=0;i<3;++i){
       aiVector3D*V=Mesh->mVertices+Face->mIndices[i];
       glm::vec3 actPoint=glm::vec3(V->x*.1*3,V->y*.1*3-5*3,V->z*.1*3);
       glm::vec3 actVector=actPoint-center;
       if(actVector.x<0)actVector*=-1.f;
       obb->xAxis+=actVector;//glm::length(actPoint-center)*actPoint;
       }
       }
       if(obb->xAxis.x==0&&obb->xAxis.y==0&&obb->xAxis.z==0)
       obb->xAxis=glm::vec3(1.f,0.f,0.f);
       else
       obb->xAxis=glm::normalize(obb->xAxis);

       obb->yAxis=glm::vec3(0.f);
       glm::vec3 refAxis;
       if(obb->xAxis.x==obb->xAxis.y&&obb->xAxis.x==obb->xAxis.z)
       refAxis=glm::normalize(glm::cross(obb->xAxis,obb->xAxis+glm::vec3(1.f,1.f,0.f)));
       else{
       if(obb->xAxis.x>0)
       refAxis=glm::normalize(glm::cross(obb->xAxis,obb->xAxis+glm::vec3(1.f,0.f,0.f)));
       else
       refAxis=glm::normalize(glm::cross(obb->xAxis,obb->xAxis+glm::vec3(-1.f,0.f,0.f)));
       }
       for(unsigned face=0;face<Mesh->mNumFaces;++face){//loop over faces
       aiFace*Face=Mesh->mFaces+face;
       for(unsigned i=0;i<3;++i){
       aiVector3D*V=Mesh->mVertices+Face->mIndices[i];
       glm::vec3 actPoint=glm::vec3(V->x*.1*3,V->y*.1*3-5*3,V->z*.1*3);
       glm::vec3 actVector=actPoint-center;
       glm::vec3 yAxis=actVector-obb->xAxis*glm::dot(actVector,obb->xAxis);
       if(glm::dot(yAxis,refAxis)<0)yAxis*=-1.f;
       obb->yAxis+=yAxis;
       }
       }
       obb->yAxis=glm::normalize(obb->yAxis);

       obb->zAxis=glm::normalize(glm::cross(obb->xAxis,obb->yAxis));

       glm::vec3 minPoint=glm::vec3(100000.f);
       glm::vec3 maxPoint=glm::vec3(-100000.f);
       for(unsigned face=0;face<Mesh->mNumFaces;++face){//loop over faces
       aiFace*Face=Mesh->mFaces+face;
       for(unsigned i=0;i<3;++i){
       aiVector3D*V=Mesh->mVertices+Face->mIndices[i];
       glm::vec3 actPoint=glm::vec3(V->x*.1*3,V->y*.1*3-5*3,V->z*.1*3);
       glm::vec3 projectedPoint;
       projectedPoint.x=glm::dot(obb->xAxis,actPoint);
       projectedPoint.y=glm::dot(obb->yAxis,actPoint);
       projectedPoint.z=glm::dot(obb->zAxis,actPoint);
       maxPoint=glm::max(projectedPoint,maxPoint);
       minPoint=glm::min(projectedPoint,minPoint);
       }
       }
       obb->minPoint=obb->xAxis*minPoint.x+obb->yAxis*minPoint.y+obb->zAxis*minPoint.z;
       obb->xAxis*=maxPoint.x-minPoint.x;
       obb->yAxis*=maxPoint.y-minPoint.y;
       obb->zAxis*=maxPoint.z-minPoint.z;

       sceneOBB.push_back(obb);
       */
  }
  sceneAABBData->unmap();
  sceneDIBO->unmap();

  std::cerr<<"AABB number: "<<sceneBB.size()<<std::endl;
  try{
    frustumCullingProgram=new ge::gl::ProgramObject(
        ShaderDir+"app/frustumCulling.comp",
        ge::gl::ShaderObject::define("FRUSTUMCULLING_BINDING_DIBO"    ,FRUSTUMCULLING_BINDING_DIBO    )+
        ge::gl::ShaderObject::define("FRUSTUMCULLING_BINDING_AABB"    ,FRUSTUMCULLING_BINDING_AABB    )+
        ge::gl::ShaderObject::define("FRUSTUMCULLING_WORKGROUP_SIZE_X",FRUSTUMCULLING_WORKGROUP_SIZE_X)+
        "");
  }catch(std::string&e){
    std::cerr<<e<<std::endl;
  }


  SceneBuffer->unmap();



  aiReleaseImport(SceneModel);

  sceneVAO->addAttrib(SceneBuffer,0,3,GL_FLOAT,sizeof(float)*6,(GLvoid*)(sizeof(float)*0));
  sceneVAO->addAttrib(SceneBuffer,1,3,GL_FLOAT,sizeof(float)*6,(GLvoid*)(sizeof(float)*3));

  /*
     SceneBuffer->bind(GL_ARRAY_BUFFER);
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,
     sizeof(float)*6,(GLvoid*)(sizeof(float)*0));

     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,
     sizeof(float)*6,(GLvoid*)(sizeof(float)*3));

     glBindVertexArray(0);
     */
  ComputeAdjacency(&ModelAdjacency,RawTriangles,SceneNumTriangles);

}

void destroy(){
  glDeleteQueries(1,&QueryTime);
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
