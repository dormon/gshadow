#version 430

#define NUM_VERTICES 3
#define NUM_NORMALS 4
#define NUM_ATTRIB (NUM_VERTICES+NUM_NORMALS)

layout(points)in;
layout(triangle_strip,max_vertices=)
