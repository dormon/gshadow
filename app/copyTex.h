#pragma once

#include<iostream>
#include<geGL/geGL.h>

void copyTexInit(std::string dir);
void copyTexDeinit();

void copyTex(ge::gl::TextureObject*output,ge::gl::TextureObject*input,unsigned x,unsigned y);
