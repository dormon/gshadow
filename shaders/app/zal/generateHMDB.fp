#version 430

layout(location=0)uniform sampler2DRect src;//input texture x - min y - max
layout(location=1)uniform sampler2D direct;//fragment is lit by light

uniform ivec2 TileSize;
uniform uint scale_translate=0;

layout(location=0)out vec4 fColor;//vystup

#define FLOAT_MAX 1000.0f

void main(){
	ivec2 srcCoord=ivec2(gl_FragCoord.xy);
	vec2 minmax=vec2(FLOAT_MAX,-FLOAT_MAX);//minimalni a maximalni hloubka
	for(int y=0;y<tileSizeY;++y){//loop over rows
		for(int x=0;x<tileSizeX;++x){//loop over columns
			ivec2 srccrd=srcCoord*TileSize;
			vec2 srcminmax=(scale_translate==0u)? 
				texelFetch(src,srccrd+ivec2(x,y)/*,0*/).rg: 
				texelFetch(src,srccrd+ivec2(x,y)/*,0*/).rr*2.0-1.0; 
			minmax.x=min(minmax.x,srcminmax.x);
			minmax.y=max(minmax.y,srcminmax.y);
		}
	}
	/*if(scale_translate==1u){
		if(texelFetch(direct,srcCoord,0).w<0.0){//nezapocitava fragmenty odvracene od svetla
			minmax.x=1.0;
			minmax.y=1.0;
		}
	}*/
	//	minmax.x=1;
	//	minmax.y=1;
	//minmax=gl_FragCoord.xy/vec2(1024,1024);
	fColor.rg=minmax;
	//fColor=vec4(texelFetch(src,srcCoord/*,0*/).r);
}
