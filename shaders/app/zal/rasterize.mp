#version 430

//number of levels
#define NUM_LEVELS 4

//hierarchical depth buffer
layout(binding=0)uniform sampler2D HDB[NUM_LEVELS];
//hierarchical stencil buffer
layout(r32i,binding=0)uniform iimage2D HSB[NUM_LEVELS];

//size of tile in each level
//0. level contains one bounding box which contains TileSize[0].x*TileSize[0].y sub-bounding boxes
//1. level contains TileSize[0].x*TileSize[0].y bounding boxes that contain TileSize[1].x*TileSize[1].y subBB
//NUM_LEVELS-1. level contains TileSize[NUM_LEVELS-1].x*TileSize[NUM_LEVELS-1].y screen samples
uniform ivec2 TileSize[NUM_LEVELS];

vec2 GetCoord(int Level){
	//invocation id in work group
	const int InvocationInWorkGroup=gl_LocalInvocationID.y*gl_WorkGroupSize.x+gl_LocalInvocationID.x;
	//number of elements in one boudnig box
	const int NumElementsInBoundingBox=TileSize[Level].x*TileSize[Level].y
	//
	const int wid=InvocationInWorkGroup%NumElementsInBoundingBox;
	return ivec2(wid%TileSize[Level].x,wid%TileSize[Level].y);

}

vec2 GetHDB(ivec2 FullCoord,int Level){
	return texelFetch(HDB[Level],FullCoord,0).xy;
}

float GetDepth(ivec2 FullCoord){
	return texelFetch(HDB[NUM_LEVELS-1],FullCoord,0).x;
}

vec2 TrivialReject(vec3 Edge){
	//<0 =0 >0
	// 1  1  0 
	return vec2(1-ivec2(sign(Edge.xy)+1)/2);
}

vec3 TrivialAcceptCorner(vec4 Plane){
	//<0 =0 >0
	// 0  0  1
	return vec3((ivec3(sign(Plane.xyz))+1)/2);
}

bool SampleInsideEdge(vec2 Sample,vec3 Edge){
	
}

