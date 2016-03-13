#include"main.hpp"

#include <string>

//*
#undef ___
#define ___
// */

#define QUERY_COMPUTE
#define QUERY_STENCIL
//#define QUERY_MULTIPLICITY

void InitModel(const char*File);
void DrawScene();
void DrawShadow();
void Draw();
void Init();
void init();
void destroy();
void Idle();
void idle();
void Mouse();
void Wheel(int d);
void DrawGBuffer();
void DrawAmbient();
void DrawDeferred();

ge::gl::AsynchronousQueryObject*gbufferQuery = nullptr;

ge::util::CameraPath*CameraMation = nullptr;
std::string CameraMationFile="sponzaprulet";

ge::util::FPSPrinter*fpsPrinter = nullptr;

objconf::CameraPathConfiguration* cameraPathConfiguration  = nullptr;
objconf::CameraConfiguration    * cameraConfiguration      = nullptr;
objconf::LightConfiguration     * lightConfiguration       = nullptr;
objconf::ShadowMethodConfig     * navyConfig               = nullptr;
objconf::ShadowMethodConfig     * rtwConfig                = nullptr;
objconf::ShadowMethodConfig     * dualParaboloidConfig     = nullptr;
objconf::ShadowMethodConfig     * navyDualParaboloidConfig = nullptr;
objconf::ShadowMethodConfig     * cubeShadowMapConfig      = nullptr;
objconf::ShadowMethodConfig     * cubeNavyMapConfig        = nullptr;

std::shared_ptr<ShadowMethod>shadowMethod = nullptr;

ge::gl::VertexArrayObject*EmptyVAO = nullptr;

void DrawShadowless();


ModelPN*model = nullptr;

ge::gl::BufferObject*sceneDIBO     = nullptr;
ge::gl::BufferObject*sceneMaterial = nullptr;
ge::gl::BufferObject*sceneAABBData = nullptr;
unsigned sceneDIBOSize=0;
ge::gl::ProgramObject*frustumCullingProgram = nullptr;
#define FRUSTUMCULLING_BINDING_DIBO      0
#define FRUSTUMCULLING_BINDING_AABB      1
#define FRUSTUMCULLING_WORKGROUP_SIZE_X 64

AxisAlignBoundingBox*sceneAABB = nullptr;

Adjacency*fastAdjacency=nullptr;

//sintorn
ge::gl::BufferObject*SintornVBO = nullptr;

//base shader
ge::gl::ProgramObject*DrawShader = nullptr;


DrawPrimitive*simpleDraw = NULL;

ge::gl::ProgramPipelineObject*programPipeline = nullptr;

//Deferred*deferred = nullptr;

std::string ModelFile = "";

float Angle[3]={0,0,0};

glm::mat4 Model      = glm::mat4(1.f);
glm::mat4 View       = glm::mat4(1.f);
glm::mat4 Projection = glm::mat4(1.f);
glm::mat4 mvp        = glm::mat4(1.f);

ge::util::WindowObject*Window = nullptr;

NDormon::GpuPerfApi*GPA = nullptr;
bool GPAavalable        = true;

bool SSEnable         = true;
bool SSAOEnable       = false;
bool SSMeasureGbuffer = true;
enum ESSMethod{
  SS_COMPUTE,
  SS_DORMONSHADOWTEST0,
  SS_SHADOWMAP,
  SS_DUALPARABOLOID,
  SS_CUBESHADOWMAP,
  SS_CUBENAVYMAPPING,
  SS_NAVYDUALPARABOLOID,
  SS_RTW,
  SS_NAVYMAPPING,
  SS_RAYTRACE,
  SS_TS,
  SS_NO
//}SSMethod=SS_NAVYDUALPARABOLOID,LastSSMethod=SS_NAVYDUALPARABOLOID;
}SSMethod=SS_CUBESHADOWMAP,LastSSMethod=SS_CUBESHADOWMAP;

bool DSDrawLightSilhouette = false;
bool DSDrawViewSilhouette  = false;
bool DSDrawEverything      = false;

bool DisableAnttweakbar = false;

simulation::SimulationData*simData = nullptr;

ge::util::ArgumentManager*argm = nullptr;
std::shared_ptr<ge::core::TypeRegister>typeRegister = nullptr;
std::shared_ptr<ge::util::sim::NamespaceWithUsers>sData = nullptr;

std::shared_ptr<NamespaceManipulator>sDataManipulator = nullptr;

int main(int Argc,char*Argv[]){
  typeRegister=std::make_shared<ge::core::TypeRegister>();
  typeRegister->addClassD<ge::gl::VertexArrayObject>("VAO");
  typeRegister->addClassD<ge::gl::BufferObject>("BO" );
  typeRegister->addClassD<ge::gl::TextureObject>("Texture");
  typeRegister->addClassD<ge::util::CameraObject>("Camera");
  typeRegister->addType("Light",ge::core::TypeRegister::STRUCT,3,"vec4","vec3","vec3");
  typeRegister->addClassD<simulation::GpuGauge>("GPUGauge");

  sData= std::make_shared<ge::util::sim::NamespaceWithUsers>("*");
  argm = new ge::util::ArgumentManager(Argc-1,Argv+1);
  for(auto x:*argm)
    std::cerr<<x.first<<": "<<x.second->toStr()<<std::endl;
  ge::util::sim::copyArgumentManager2Namespace(sData,argm,typeRegister);
  std::cout<<sData->toStr(0)<<std::endl;

  //ge::util::ArgumentLoader*argLoader=new ge::util::ArgumentLoader(Argc,Argv);
  //for(unsigned i=0;i<argLoader->getNumVariables();++i)
  //  std::cerr<<argLoader->getVariable(i)<<":"<<argLoader->getData(i)<<std::endl;
  simData=new simulation::SimulationData();
  //for(unsigned i=0;i<argLoader->getNumVariables();++i)
  //  simData->insertVariable(argLoader->getVariable(i),argLoader->getData(i));

  //ModelFile = "models/o/o.3ds";
  //ModelFile = "models/bagr/bagr.obj";
  //ModelFile = "models/bugbagr/bugbagr.obj";
  //ModelFile = "models/bunny/bunny.obj";
  //ModelFile = "models/robots/robots.obj";
  //ModelFile = "models/robot/robot.obj";
  //ModelFile = "models/tessrobot/tessrobot.obj";
  //ModelFile = "/home/dormon/Desktop/lost_empire/lost_empire.obj";
  //ModelFile = "/home/dormon/Desktop/sponza/sponza.obj";
  //ModelFile = "/home/dormon/Desktop/san-miguel/san-miguel.obj";
  //ModelFile = "/home/dormon/Desktop/conference/conference.obj";
  //ModelFile = "/media/data/models/Sponza/sponza.obj";
  //ModelFile = "/media/data/models/sibenik/sibenik.obj";
  //ModelFile = "/data/models/conference_corrected/conference.obj";
  //ModelFile = "/media/data/models/lost_empire/lost_empire.obj";
  //ModelFile = "/home/dormon/Desktop/hairball.obj";
  //ModelFile = "models/o/o.3ds";
  ModelFile = "models/2quads/2quads.obj";
  //ModelFile = "models/2_3quads/2_3quads.obj";
  //ModelFile = "/media/old/home/dormon/Plocha/sponza/sponza.obj";

  ShaderDir          = "shaders/";
  DisableAnttweakbar = false;

  Window=new ge::util::WindowObject(
      sData->get<unsigned[]>("window.size")[0],
      sData->get<unsigned[]>("window.size")[1],
      sData->get<bool>("window.fullscreen"),
      idle,
      Mouse,
      !DisableAnttweakbar,
      sData->get<unsigned>("context.version"),
      sData->get<std::string>("context.profile"),
      sData->get<std::string>("context.debug"));

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
    sData->get<ge::util::CameraObject>("camera").fpsCamera(Angle[0],Angle[1],Angle[2]);
    //cameraConfiguration->getCamera()->fpsCamera(Angle[0],Angle[1],Angle[2]);
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

  sData->get<Deferred>("gbuffer").setTextures();
  glUseProgram(0);
  sData->get<ge::gl::TextureObject>("shadowMask").bind(8);
  if(!Linked){
    DrawShader->setSeparable();
    DrawShader->setRetrievable();
    Linked=true;
    programPipeline->useProgramStages(GL_ALL_SHADER_BITS,DrawShader->getId());
  }
  programPipeline->bind();
  DrawShader->setdsa("La",0.f,0.f,0.f);
  DrawShader->setdsa("Ld",1,sData->get<float[]>("light.diffuse"));
  DrawShader->setdsa("Ls",1,sData->get<float[]>("light.specular"));
  DrawShader->setdsa("LightPosition",1,sData->get<float[]>("light.position"));
  DrawShader->setdsa("CameraPosition",1,glm::value_ptr(sData->get<ge::util::CameraObject>("camera").getPosition()));
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

class ProgramObjectFactor: public ge::core::Function{
  public:
    ProgramObjectFactor():Function(10){}
    void operator()(){


    }
};

void idle(){
  sData->get<ge::util::CameraObject>("camera").right  ((Window->isKeyDown('d')-Window->isKeyDown('a'))*Speed);
  sData->get<ge::util::CameraObject>("camera").up     ((Window->isKeyDown(' ')-Window->isKeyDown('c'))*Speed);
  sData->get<ge::util::CameraObject>("camera").forward((Window->isKeyDown('w')-Window->isKeyDown('s'))*Speed);
  sData->get<ge::util::CameraObject>("camera").getView(&View);
  sData->get<ge::util::CameraObject>("camera").getProjection(&Projection);
  mvp=Projection*View;

  ___;
  /*
  setConfig(&navyConfig,navyMapping,"navyconfig");
  setConfig(&rtwConfig ,rtw        ,"rtwconfig" );
  setConfig(&dualParaboloidConfig,dualParaboloid,"dpsmconfig");
  setConfig(&navyDualParaboloidConfig,navyDualParaboloid,"navydpsmconfig");
  setConfig(&cubeShadowMapConfig,cubeShadowMapping,"cubeShadowMap");
  setConfig(&cubeNavyMapConfig,cubeNavyMapping,"cubeNavyMapping");
  */


  if(Window->isKeyOn('q'))exit(0);
  if(Window->isKeyDown('g')){
    sData->get<float[]>("light.position")[1]-=0.4;
    simData->setAsChanged("light");
  }
  if(Window->isKeyDown('t')){
    sData->get<float[]>("light.position")[1]+=0.4;
    simData->setAsChanged("light");
  }

  simData->sendUpdate();
  ___;
  if(SSEnable){
    if(SSMeasureGbuffer)gbufferQuery->begin();
    DrawGBuffer();
    if(SSMeasureGbuffer)gbufferQuery->end();
    DrawAmbient();

    if(shadowMethod)shadowMethod->createShadowMask();
    drawDiffuseSpecular(true,lightConfiguration->getLight());

  }else{
    DrawShadowless();
  }
  ___;
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  sData->get<Deferred>("gbuffer").blitDepth2Default();
  ___;
  glDepthFunc(GL_LESS);
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glDisable(GL_BLEND);
  ___;
  //*
  if(SSMethod==SS_NAVYMAPPING){
    simpleDraw->drawDepth((std::dynamic_pointer_cast<NavyMapping>(shadowMethod))->getShadowMap()->getId(),.5f,.0f,.5f,.5f,1.f,1000.f);
  }
  ___;
  if(SSMethod==SS_RTW){
    if(simData->getBool("rtw.drawSM"))
      simpleDraw->drawDepth((std::dynamic_pointer_cast<RTWBack>(shadowMethod))->getShadowMap()->getId(),.0f,.0f,1.f,1.0f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"));
  }
  ___;
  if(SSMethod==SS_SHADOWMAP){
    simpleDraw->drawDepth((std::dynamic_pointer_cast<CShadowMapping>(shadowMethod))->getShadowMap()->getId(),.5f,.0f,.5f,.5f,sData->get<float>("shadowMapMethods.near"),sData->get<float>("shadowMapMethods.far"));
  }
  ___;
  if(SSMethod==SS_CUBESHADOWMAP){
    if(simData->getBool("csm.drawSM")){
      GLuint sm=(std::dynamic_pointer_cast<CubeShadowMapping>(shadowMethod))->getShadowMap()->getId();
      simpleDraw->drawCubeDepth(sm,.0f ,.25f,.25f,.25f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),5);
      simpleDraw->drawCubeDepth(sm,.25f,.25f,.25f,.25f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),1);
      simpleDraw->drawCubeDepth(sm,.5f ,.25f,.25f,.25f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),4);
      simpleDraw->drawCubeDepth(sm,.75f,.25f,.25f,.25f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),0);
      simpleDraw->drawCubeDepth(sm,.25f,.0f ,.25f,.25f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),2);
      simpleDraw->drawCubeDepth(sm,.25f,.5f ,.25f,.25f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),3);
    }
  }
  ___;
  if(SSMethod==SS_CUBENAVYMAPPING){
    auto cnm=std::dynamic_pointer_cast<CubeNavyMapping>(shadowMethod);
    GLuint sm=cnm->getShadowMap()->getId();
    if(simData->getBool("cnm.drawSM")){
      simpleDraw->drawCubeDepth(sm,.0f ,.25f,.25f,.25f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),5);
      simpleDraw->drawCubeDepth(sm,.25f,.25f,.25f,.25f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),1);
      simpleDraw->drawCubeDepth(sm,.5f ,.25f,.25f,.25f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),4);
      simpleDraw->drawCubeDepth(sm,.75f,.25f,.25f,.25f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),0);
      simpleDraw->drawCubeDepth(sm,.25f,.0f ,.25f,.25f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),2);
      simpleDraw->drawCubeDepth(sm,.25f,.5f ,.25f,.25f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),3);
    }
    if(simData->getBool("cnm.drawCountMap")){
      simpleDraw->drawHeatMap(cnm->getCountMap(5)->getId(),.0f ,.25f,.25f,.25f,0u,10u,0);
      simpleDraw->drawHeatMap(cnm->getCountMap(1)->getId(),.25f,.25f,.25f,.25f,0u,10u,6);
      simpleDraw->drawHeatMap(cnm->getCountMap(4)->getId(),.5f ,.25f,.25f,.25f,0u,10u,1);
      simpleDraw->drawHeatMap(cnm->getCountMap(0)->getId(),.75f,.25f,.25f,.25f,0u,10u,4);
      simpleDraw->drawHeatMap(cnm->getCountMap(2)->getId(),.25f,.0f ,.25f,.25f,0u,10u,6);
      simpleDraw->drawHeatMap(cnm->getCountMap(3)->getId(),.25f,.5f ,.25f,.25f,0u,10u,7);
    }
    if(simData->getBool("cnm.drawWarpedCountMap")){
      simpleDraw->drawHeatMap(cnm->getWarpedCountMap(5)->getId(),.0f ,.25f,.25f,.25f,0u,10u,0);
      simpleDraw->drawHeatMap(cnm->getWarpedCountMap(1)->getId(),.25f,.25f,.25f,.25f,0u,10u,6);
      simpleDraw->drawHeatMap(cnm->getWarpedCountMap(4)->getId(),.5f ,.25f,.25f,.25f,0u,10u,1);
      simpleDraw->drawHeatMap(cnm->getWarpedCountMap(0)->getId(),.75f,.25f,.25f,.25f,0u,10u,4);
      simpleDraw->drawHeatMap(cnm->getWarpedCountMap(2)->getId(),.25f,.0f ,.25f,.25f,0u,10u,6);
      simpleDraw->drawHeatMap(cnm->getWarpedCountMap(3)->getId(),.25f,.5f ,.25f,.25f,0u,10u,7);
    }
  }
  ___;
  if(SSMethod==SS_DUALPARABOLOID){
    auto dp=std::dynamic_pointer_cast<DualParaboloid>(shadowMethod);
    if(simData->getBool("dp.drawSM0"))
      simpleDraw->drawDepth(dp->getShadowMap(0)->getId(),.0f,.0f,1.0f,1.0f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),DrawPrimitive::DP);
    if(simData->getBool("dp.drawSM1"))
      simpleDraw->drawDepth(dp->getShadowMap(1)->getId(),.0f,.0f,1.0f,1.0f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),DrawPrimitive::DP);
  }
  ___;
  if(SSMethod==SS_NAVYDUALPARABOLOID){
    auto ndp=std::dynamic_pointer_cast<NavyDualParaboloid>(shadowMethod);
    if(simData->getBool("ndp.drawSM0"))
      simpleDraw->drawDepth(ndp->getShadowMap(0)->getId(),.0f,.0f,1.0f,1.0f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),DrawPrimitive::DP);
    if(simData->getBool("ndp.drawSM1"))
      simpleDraw->drawDepth(ndp->getShadowMap(1)->getId(),.0f,.0f,1.0f,1.0f,simData->getFloat("shadowMapMethods.near"),simData->getFloat("shadowMapMethods.far"),DrawPrimitive::DP);

    if(simData->getBool("ndp.drawCountMap0"))
      simpleDraw->drawHeatMap(ndp->getCountMap(0)->getId(),.0f,.0f,1.f,1.f,0u,10u);
    if(simData->getBool("ndp.drawCountMap1"))
      simpleDraw->drawHeatMap(ndp->getCountMap(1)->getId(),.0f,.0f,1.f,1.f,0u,10u);

    if(simData->getBool("ndp.drawWarpedCountMap0"))
      simpleDraw->drawHeatMap(ndp->getWarpedCountMap(0)->getId(),.0f,.0f,1.f,1.f,0u,10u);
    if(simData->getBool("ndp.drawWarpedCountMap1"))
      simpleDraw->drawHeatMap(ndp->getWarpedCountMap(1)->getId(),.0f,.0f,1.f,1.f,0u,10u);
  }
  ___;



  //*/

  if(cameraPathConfiguration->isDraw())
    if(cameraPathConfiguration->getPath())
      cameraPathConfiguration->getPath()->draw(glm::value_ptr(mvp));


  ___;
  Window->swap();
  fpsPrinter->endOfFrame();
  ___;
}

void methodChangeSet(const void*A,void*D){
  ESSMethod newMethod=*((ESSMethod*)A);
  ESSMethod*oldMethod= ((ESSMethod*)D);
  if(*oldMethod!=newMethod||!shadowMethod){
    switch(newMethod){
      case SS_NAVYMAPPING       :shadowMethod = std::make_shared<NavyMapping       >(simData);break;
      case SS_SHADOWMAP         :shadowMethod = std::make_shared<CShadowMapping    >(sData  );break;
      case SS_DUALPARABOLOID    :shadowMethod = std::make_shared<DualParaboloid    >(simData);break;
      case SS_CUBESHADOWMAP     :shadowMethod = std::make_shared<CubeShadowMapping >(simData);break;
      case SS_CUBENAVYMAPPING   :shadowMethod = std::make_shared<CubeNavyMapping   >(simData);break;
      case SS_NAVYDUALPARABOLOID:shadowMethod = std::make_shared<NavyDualParaboloid>(simData);break;
      case SS_RTW               :shadowMethod = std::make_shared<RTWBack           >(simData);break;
      case SS_COMPUTE           :shadowMethod = std::make_shared<ComputeGeometry   >(sData  );break;
      case SS_DORMONSHADOWTEST0 :shadowMethod = std::make_shared<DormonShadowTest0 >(sData  );break;
      case SS_RAYTRACE          :shadowMethod = std::make_shared<RayTrace          >(simData);break;
      default                   :shadowMethod = nullptr                                      ;break;
    }
  }
  *oldMethod=newMethod;
}

void methodChangeGet(void*A,void*D){
  *((ESSMethod*)A)=*((ESSMethod*)D);
}


void init(){
  glewExperimental=GL_TRUE;
  glewInit();

  ge::gl::init();
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

  gbufferQuery = new ge::gl::AsynchronousQueryObject(GL_TIME_ELAPSED,GL_QUERY_RESULT_NO_WAIT,ge::gl::AsynchronousQueryObject::UINT64);
  programPipeline=new ge::gl::ProgramPipelineObject();
  try{
    DrawShader = new ge::gl::ProgramObject(
        ShaderDir+"app/dgb.vp",
        ShaderDir+"app/dgb.gp",
        ShaderDir+"app/dgb.fp");

  }catch(std::string&e){
    std::cerr<<e<<std::endl;
  }

  sData->insert("sceneVAO",typeRegister->sharedEmptyAccessor("VAO",[](unsigned char*ptr){delete(ge::gl::VertexArrayObject*)ptr;}));
  sData->insert("sceneVBO",typeRegister->sharedEmptyAccessor("BO",[](unsigned char*ptr){delete(ge::gl::BufferObject*)ptr;}));
  sData->insert("gbuffer",typeRegister->sharedAccessorAddD<Deferred>("Deferred",
        sData->get<unsigned[]>("window.size")[0],
        sData->get<unsigned[]>("window.size")[1],
        sData->get<std::string>("shaderDirectory")));
  InitModel(ModelFile.c_str());
  sData->insert("fastAdjacency",typeRegister->sharedAccessorAddD<Adjacency>("Adjacency",model->getPositions(),model->getNofVertices()/3,2));
  sData->insert("computeMethod.program.WORKGROUPSIZE" ,typeRegister->sharedAccessor<int>("i32",64));
  sData->insert("computeMethod.program.CULL_SIDE"     ,typeRegister->sharedAccessor<bool>("bool",true));
  sData->insert("measure.computeGeometry.computeSides",typeRegister->sharedAccessor<simulation::GpuGauge>("GPUGauge",false,true));
  sData->insert("measure.computeGeometry.draw"        ,typeRegister->sharedAccessor<simulation::GpuGauge>("GPUGauge",false,true));
  sData->insert("measure.computeGeometry.blit"        ,typeRegister->sharedAccessor<simulation::GpuGauge>("GPUGauge",false,true));


  sData->insert("shadowMask",typeRegister->sharedAccessor<ge::gl::TextureObject>("Texture",GL_TEXTURE_2D,GL_R32F,1,
      sData->get<unsigned[]>("window.size")[0],
      sData->get<unsigned[]>("window.size")[1]));


  sData->insert("camera",typeRegister->sharedAccessor<ge::util::CameraObject>("Camera",
    sData->get<unsigned[]>("window.size"),1.f,std::numeric_limits<float>::infinity(),45.f));

  sData->insert("light"   ,typeRegister->sharedAccessor("Light"));

  sDataManipulator = std::make_shared<NamespaceManipulator>(sData);


  std::cerr<<"NumTriangles: "   <<fastAdjacency->getNofTriangles   ()<<std::endl;
  std::cerr<<"NumEdges: "       <<fastAdjacency->getNofEdges       ()<<std::endl;
  std::cerr<<"MaxMultiplicity: "<<fastAdjacency->getMaxMultiplicity()<<std::endl;

  cameraConfiguration = new objconf::CameraConfiguration(
      glm::uvec2(
        sData->get<unsigned[2]>("window.size")[0],
        sData->get<unsigned[2]>("window.size")[1]));
  cameraPathConfiguration = new objconf::CameraPathConfiguration();
  cameraPathConfiguration->setCamera(cameraConfiguration->getCamera());
  lightConfiguration = new objconf::LightConfiguration();

  TwBar*Bar = nullptr;
  Bar=TwNewBar("TweakBar");
  TwAddVarRW(Bar,"Speed"  ,TW_TYPE_FLOAT  ,&Speed     ," label='Movement speed' min=0 max=2 step=0.01"  );
  TwAddVarRW(Bar,"SSAO"   ,TW_TYPE_BOOLCPP,&SSAOEnable," help='Toggle screenspace ambient occlussion.' ");
  TwAddVarRW(Bar,"Shadows",TW_TYPE_BOOLCPP,&SSEnable  ," help='Toggle shadows on.' "                    );

  TwEnumVal MethodDef[]={
    {SS_COMPUTE           ,"compute"                            },
    {SS_DORMONSHADOWTEST0 ,"dormonShadowTest0"                  },
    {SS_SHADOWMAP         ,"shadowmapping"                      },
    {SS_DUALPARABOLOID    ,"dualParaboloid"                     },
    {SS_CUBESHADOWMAP     ,"cubeShadowMapping"                  },
    {SS_CUBENAVYMAPPING   ,"cubeNavyMapping"                    },
    {SS_NAVYDUALPARABOLOID,"navyDualParaboloid"                 },
    {SS_RTW               ,"rtv"                                },
    {SS_NAVYMAPPING       ,"ours"                               },
    {SS_TS                ,"Tessellation Shader Silhouette Edge"},
    {SS_RAYTRACE          ,"raytrace"                           },
    {SS_NO                ,"No shadows"                         }
  };
  TwType MethodType=TwDefineEnum("SS mode",MethodDef,sizeof(MethodDef)/sizeof(TwEnumVal));
  TwAddVarCB(Bar,"Method",MethodType,methodChangeSet,methodChangeGet,&SSMethod,"help='Cange shadow method'");

  /*
  simData->insertVariable("emptyVAO"                            ,new simulation::Object(EmptyVAO)          );
  simData->insertVariable("sceneVAO"                            ,new simulation::Object(sceneVAO)          );
  simData->insertVariable("sceneVBO"                            ,new simulation::Object(SceneBuffer)       );
  simData->insertVariable("light"                               ,lightConfiguration->getLight()            );
  simData->insertVariable("fastAdjacency"                       ,new simulation::Object(fastAdjacency)     );
  simData->insertVariable("gbuffer.position"                    ,new simulation::Object(deferred->position));
  simData->insertVariable("gbuffer.fbo"                         ,new simulation::Object(deferred->fbo)     );
  simData->insertVariable("gbuffer.stencil"                     ,new simulation::Object(deferred->depth)   );
  simData->insertVariable("gpa"                                 ,new simulation::Object(GPA)               );
  simData->insertVariable("computeMethod.program.WORKGROUPSIZE" ,new simulation::Int(64));
  simData->insertVariable("computeMethod.program.CULL_SIDE"     ,new simulation::Bool(true));
  simData->insertVariable("measure.computeGeometry.computeSides",new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.computeGeometry.draw"        ,new simulation::GpuGauge(false,true));
  simData->insertVariable("measure.computeGeometry.blit"        ,new simulation::GpuGauge(false,true));


  simData->insertVariable("shadowMask"                        ,new simulation::Object(shadowMask)  );
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
  simData->insertVariable("rtw.drawSM"                  ,new simulation::Bool(false));

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


  simData->insertVariable("csm.drawSM",new simulation::Bool(false));

  simData->insertVariable("cnm.drawSM"              ,new simulation::Bool(false));
  simData->insertVariable("cnm.computeVisualisation",new simulation::Bool(false));
  simData->insertVariable("cnm.drawSM"              ,new simulation::Bool(false));
  simData->insertVariable("cnm.drawCountMap"        ,new simulation::Bool(false));
  simData->insertVariable("cnm.drawWarpedCountMap"  ,new simulation::Bool(false));

  simData->insertVariable("dp.drawSM0", new simulation::Bool(false));
  simData->insertVariable("dp.drawSM1", new simulation::Bool(false));

  simData->insertVariable("ndp.computeVisualisation",new simulation::Bool(true));
  simData->insertVariable("ndp.drawSM0"             ,new simulation::Bool(false));
  simData->insertVariable("ndp.drawSM1"             ,new simulation::Bool(false));
  simData->insertVariable("ndp.drawCountMap0"       ,new simulation::Bool(false));
  simData->insertVariable("ndp.drawCountMap1"       ,new simulation::Bool(false));
  simData->insertVariable("ndp.drawWarpedCountMap0" ,new simulation::Bool(false));
  simData->insertVariable("ndp.drawWarpedCountMap1" ,new simulation::Bool(false));

  simData->insertVariable("camera",new simulation::Object(cameraConfiguration->getCamera()));
  */

  ___;
  simpleDraw = new DrawPrimitive(ShaderDir+"app/");
  ___;
  //simpleDraw->setWindowSize(simData->getUVec2("window.size"));
  simpleDraw->setWindowSize(sData->get<unsigned[]>("window.size"));
  ___;



  copyTexInit(ShaderDir);
  //std::cerr<<"konec initu\n";
  ___;
  fpsPrinter = new ge::util::FPSPrinter(200);
  fpsPrinter->start();

}

void DrawScene(){
  if(Window->isKeyOn('f'))glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glDisable(GL_STENCIL_TEST);
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  sData->get<ge::gl::VertexArrayObject>("sceneVAO").bind();
  //sceneVAO->bind();
  sceneMaterial->bindBase(GL_SHADER_STORAGE_BUFFER,0);
  sceneDIBO->bind(GL_DRAW_INDIRECT_BUFFER);
  glMultiDrawArraysIndirect(GL_TRIANGLES,NULL,sceneDIBOSize,sizeof(unsigned)*4);
  sData->get<ge::gl::VertexArrayObject>("sceneVAO").unbind();
  //sceneVAO->unbind();
  if(Window->isKeyOn('f'))glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

void ShaderSetMatrix(ge::gl::ProgramObject*P){
  P->set("m"  ,1,GL_FALSE,(const float*)glm::value_ptr(Model     ));
  P->set("v"  ,1,GL_FALSE,(const float*)glm::value_ptr(View      ));
  P->set("p"  ,1,GL_FALSE,(const float*)glm::value_ptr(Projection));
  P->set("mvp",1,GL_FALSE,(const float*)glm::value_ptr(mvp       ));
}

void DrawGBuffer(){
  frustumCullingProgram->use();
  frustumCullingProgram->bindSSBO("dibo",sceneDIBO);
  frustumCullingProgram->bindSSBO("aabb",sceneAABBData);
  frustumCullingProgram->set("numAABB",(unsigned)sceneDIBOSize);
  frustumCullingProgram->set("mvp",1,GL_FALSE,(const float*)glm::value_ptr(mvp));
  glDispatchCompute(ge::core::getDispatchSize(sceneDIBOSize,FRUSTUMCULLING_WORKGROUP_SIZE_X),1,1);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);

  sData->get<Deferred>("gbuffer").activate();
  sData->get<Deferred>("gbuffer").setTextures();
  sData->get<Deferred>("gbuffer").createProgram->use();
  ShaderSetMatrix(sData->get<Deferred>("gbuffer").createProgram);
  DrawScene();
  sData->get<Deferred>("gbuffer").deactivate();
}

void DrawAmbient(){
  sData->get<Deferred>("gbuffer").setTextures();

  DrawShader->use();

  DrawShader->set("La",.3f,.3f,.3f);
  DrawShader->set("Ld",.0f,.0f,.0f);
  DrawShader->set("Ls",.0f,.0f,.0f);
  DrawShader->set("CameraPosition",1,glm::value_ptr(sData->get<ge::util::CameraObject>("camera").getPosition()));
  DrawShader->set("SSAO",SSAOEnable);
  DrawShader->set("useShadows",false);

  EmptyVAO->bind();
  glDrawArrays(GL_POINTS,0,1);
  EmptyVAO->unbind();
}

void setGLForStencil(bool zfail){

  sData->get<Deferred>("gbuffer").activateCreateStencil();
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

void DrawShadowless(){
  DrawGBuffer();
  glDepthFunc(GL_ALWAYS);
  glDepthMask(GL_FALSE);
  DrawAmbient();
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE,GL_ONE);
  glBlendEquation(GL_FUNC_ADD);

  glDepthFunc(GL_ALWAYS);
  drawDiffuseSpecular(false,lightConfiguration->getLight());
  //DrawDiffuseSpecular(false,false,lightConfiguration->getLight());
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
}

void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString){
  destinationClientString = sourceLibraryString;
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
  ___;
  model=new ModelPN(SceneModel);
  ___;
  ModelPN2VAO(*sData->getVariable("sceneVAO"),*sData->getVariable("sceneVBO"),model)();
  ___;
  Model2AABB(&sceneAABBData,model)();
  Model2DIBO(&sceneDIBO,model)();

  sceneAABB=new AxisAlignBoundingBox();
  sceneDIBOSize=model->getNofMeshes();
  unsigned actface=0;

  sceneMaterial = new ge::gl::BufferObject(sizeof(float)*4*2*model->getNofMeshes());
  MeshMaterial*mmptr= (MeshMaterial*)sceneMaterial->map();
  for(unsigned mesh=0;mesh<SceneModel->mNumMeshes;++mesh){//loop over meshes
    aiMesh*Mesh=SceneModel->mMeshes[mesh];

    aiColor3D c;
    aiMaterial*mat=SceneModel->mMaterials[Mesh->mMaterialIndex];
    mat->Get(AI_MATKEY_COLOR_DIFFUSE,c);
    for(int i=0;i<3;++i)mmptr->diffuse[i]=c[i];
    mat->Get(AI_MATKEY_COLOR_SPECULAR,c);
    for(int i=0;i<3;++i)mmptr->specular[i]=c[i];
    mmptr++;

    for(unsigned face=0;face<Mesh->mNumFaces;++face){//loop over faces
      aiFace*Face=Mesh->mFaces+face;
      for(unsigned i=0;i<3;++i){
        aiVector3D*V=Mesh->mVertices+Face->mIndices[i];
        sceneAABB->addPoint(glm::vec3(V->x,V->y,V->z));
      }
      actface++;
    }
  }
  sceneMaterial->unmap();

  frustumCullingProgram=new ge::gl::ProgramObject(
      ShaderDir+"app/frustumCulling.comp",
      ge::gl::ShaderObject::define("FRUSTUMCULLING_BINDING_DIBO"    ,FRUSTUMCULLING_BINDING_DIBO    )+
      ge::gl::ShaderObject::define("FRUSTUMCULLING_BINDING_AABB"    ,FRUSTUMCULLING_BINDING_AABB    )+
      ge::gl::ShaderObject::define("FRUSTUMCULLING_WORKGROUP_SIZE_X",FRUSTUMCULLING_WORKGROUP_SIZE_X)+
      "");

  aiReleaseImport(SceneModel);
  auto start=SDL_GetTicks();
  fastAdjacency = new Adjacency(model->getPositions(),model->getNofVertices()/3,2);
  auto end=SDL_GetTicks();
  std::cout<<(end-start)<<" ms"<<std::endl;
}

void destroy(){
  delete programPipeline;
  delete CameraMation;
  delete DrawShader;
  //delete SceneBuffer;
  delete model;
}
