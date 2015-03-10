#include"testConvexHull.h"

using namespace test;

struct{
  bool ignore;
  bool scene;
  bool camera;
  bool scCam;
  bool scCamEx;
  bool scCamExLim;
  bool minFrustum;
  DrawPrimitive*simpleDraw;
}testConvexHull={
  true,false,false,false,false,false,false,NULL
};


void test::drawConvexHull(
    glm::mat4 view,
    glm::mat4 projection,
    glm::mat4 p,
    glm::mat4 v,
    glm::vec3 minPoint,
    glm::vec3 maxPoint,
    glm::vec3 light){
  if(testConvexHull.ignore)return;
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  testConvexHull.simpleDraw->beginTriangles();
  testConvexHull.simpleDraw->setView(view);
  testConvexHull.simpleDraw->setProjection(projection);
  testConvexHull.simpleDraw->setColor(1,0,0,2);

  geometry::ConvexHull*sceneHull=new geometry::ConvexHull(minPoint,maxPoint);
  geometry::ConvexTriangles*sceneHullTriangles=new geometry::ConvexTriangles(sceneHull);

  geometry::ConvexHull*cameraHull=new geometry::ConvexHull(p,v);
  geometry::ConvexTriangles*cameraHullTriangles=new geometry::ConvexTriangles(cameraHull);

  geometry::ConvexHull*sceneCameraHull = sceneHull->intersect(cameraHull);
  geometry::ConvexTriangles*sceneCameraHullTriangles=new geometry::ConvexTriangles(sceneCameraHull);

  sceneCameraHull->extend(light);
  geometry::ConvexTriangles*sceneCameraExtendedHullTriangles=new geometry::ConvexTriangles(sceneCameraHull);

  geometry::ConvexHull*sceneCameraExtendedLimitedHull=sceneCameraHull->intersect(sceneHull);
  geometry::ConvexTriangles*sceneCameraExtendedLimitedHullTriangles=new geometry::ConvexTriangles(sceneCameraExtendedLimitedHull);
  glm::mat4 lp,lv;
  geometry::getMinVP(&lp,&lv,p,v,minPoint,maxPoint,light);
  geometry::ConvexHull*minFrustumHull=new geometry::ConvexHull(lp,lv);
  geometry::ConvexTriangles*minFrustumHullTriangles=new geometry::ConvexTriangles(minFrustumHull);

  if(testConvexHull.scene)
    sceneHullTriangles->draw(testConvexHull.simpleDraw);

  if(testConvexHull.camera)
    cameraHullTriangles->draw(testConvexHull.simpleDraw);

  if(testConvexHull.scCam)
    sceneCameraHullTriangles->draw(testConvexHull.simpleDraw);

  if(testConvexHull.scCamEx)
    sceneCameraExtendedHullTriangles->draw(testConvexHull.simpleDraw);

  if(testConvexHull.scCamExLim)
    sceneCameraExtendedLimitedHullTriangles->draw(testConvexHull.simpleDraw);

  if(testConvexHull.minFrustum)
    minFrustumHullTriangles->draw(testConvexHull.simpleDraw);

  delete sceneHull;
  delete sceneHullTriangles;
  delete cameraHull;
  delete cameraHullTriangles;
  delete sceneCameraHull;
  delete sceneCameraHullTriangles;
  delete sceneCameraExtendedHullTriangles;
  delete sceneCameraExtendedLimitedHull;
  delete sceneCameraExtendedLimitedHullTriangles;
  delete minFrustumHull;
  delete minFrustumHullTriangles;


  testConvexHull.simpleDraw->end();
  glDisable(GL_BLEND);
}

void test::setTestConvexHull(DrawPrimitive*simp){
  testConvexHull.simpleDraw=simp;
  TwBar*geobar=TwNewBar("convex hull");
  TwAddVarRW(geobar,"ignore"    ,TW_TYPE_BOOLCPP,&testConvexHull.ignore    ," help='ignore' "                            );
  TwAddVarRW(geobar,"camera"    ,TW_TYPE_BOOLCPP,&testConvexHull.camera    ," help='camera' "                            );
  TwAddVarRW(geobar,"Scene"     ,TW_TYPE_BOOLCPP,&testConvexHull.scene     ," help='scene' "                             );
  TwAddVarRW(geobar,"ScCam"     ,TW_TYPE_BOOLCPP,&testConvexHull.scCam     ," help='scene camera intersection' "         );
  TwAddVarRW(geobar,"ScCamEx"   ,TW_TYPE_BOOLCPP,&testConvexHull.scCamEx   ," help='scene camera intersection extended' ");
  TwAddVarRW(geobar,"ScCamExLim",TW_TYPE_BOOLCPP,&testConvexHull.scCamExLim," help='limited scene camera intersection' " );
  TwAddVarRW(geobar,"MinFrustum",TW_TYPE_BOOLCPP,&testConvexHull.minFrustum," help='minimal frustum' "                   );
}
