echo -e "#define MAX_MULTIPLICITY 3\n #define __VERSION__ 100 \n #define GL_ES" | cat - webgl_vertex_side.vp | gcc -E - | grep -v "#" | sed "/^$/d" > webgl_vertex_side3.vp 

