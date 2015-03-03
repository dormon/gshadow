#version 330

uniform mat4 M;

bool Visible(vec4 A,vec4 B,vec4 L){
	//A + tu + lv + tl(u+w)
	vec4 C=vec4(A.xyz*L.w-L.xyz,0);
	vec4 D=vec4(B.xyz*L.w-L.xyz,0);
	vec4 u=B-A;
	vec4 v=C-A;
	vec4 w=D-C; 
	float t,l;
	vec4 X=M*(A + t*u + l*v + t*l*(u+w));
	X=M*A + t*M*u + l*M*v + t*l*M*u + t*l*M*w;
	X=M*A + t*M*(B-A) + l*M*(C-A) + t*l*M*(B-A) + t*l*M*(D-C);
	X=M*A + t*M*B - t*M*A + l*M*C - l*M*A + t*l*M*B - t*l*M*A + t*l*M*D - t*l*M*C;
	vec4 E=M*A;
	vec4 F=M*B;
	vec4 G=M*C;
	vec4 H=M*D;
	X=E + t*F - t*E + l*G - l*E + t*l*F - t*l*E + t*l*H - t*l*G;
	X=E + t*(F-E) + l*(G-E) + t*l*(F-E+H-G);
	vec4 i=F-E;
	vec4 j=G-E;
	vec4 k=j+H-G;
	X=E + t*i + l*j + t*l*k;
	//magic to find t,l
	float w=E.w + t*i.w + l*j.w + t*l*k.w;
	float x=E.x + t*i.x + l*j.x + t*l*k.x;
	float y=E.y + t*i.y + l*j.y + t*l*k.y;
	float y=E.z + t*i.z + l*j.z + t*l*k.z;
	float dwdt=i.w+l*k.w;
	float dwdl=j.w+t*k.w;

	float dxdt=i.x+l*k.x;
	float dxdl=j.x+t*k.x;
	float dydt=i.y+l*k.y;
	float dydl=j.y+t*k.y;
	float dzdt=i.z+l*k.z;
	float dzdl=j.z+t*k.z;


	//x>=-w && x<=w
	//x+w>=0
	//x-w<=0
	//E.x+E.w + t*(i.x+i.w) + l*(j.x+j.w) + t*l*(k.x+k.w) >=0
	//E.w-E.x + t*(i.w-i.x) + l*(j.w-j.x) + t*l*(k.w-k.x) >=0
	//E.y+E.w + t*(i.y+i.w) + l*(j.y+j.w) + t*l*(k.y+k.w) >=0
	//E.w-E.y + t*(i.w-i.y) + l*(j.w-j.y) + t*l*(k.w-k.y) >=0
	//E.z+E.w + t*(i.z+i.w) + l*(j.z+j.w) + t*l*(k.z+k.w) >=0
	//E.w-E.z + t*(i.w-i.z) + l*(j.w-j.z) + t*l*(k.w-k.z) >=0
	//pmx + t*pnx + l*pox t*l*ppx >=0
	//nmx + t*nnx + l*nox t*l*npx >=0

	//pmy + t*pny + l*poy t*l*ppy >=0
	//nmy + t*nny + l*noy t*l*npy >=0

	//pmz + t*pnz + l*poz t*l*ppz >=0
	//nmz + t*nnz + l*noz t*l*npz >=0

	//pmx + t*pnx + l*pox t*l*ppx = 0
	//l=-(pmx+t*pnx)/(pox+t*ppx)
	//0=-(pmx+t*pnx)/(pox+t*ppx)
	//0=pmx+t*pnx
	//-pmx/pnx=t
	//1=-(pmx+t*pnx)/(pox+t*ppx)
	//pox+t*ppx=-pmx-t*pnx
	//t*ppx+t*pnx=-pox-pmx
	//t=-(pox+pmx)/(ppx+pnx)








	return abs(x)<=w && abs(y)<=w && abs(z)<=w
	return all(lessThanEqual(abs(X.xyz),X.www));
}

int main(){

}
