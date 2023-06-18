#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <iostream>
using namespace std;

void part1() {
	cout << "3x1 vectors" << endl;

	glm::vec3  a(0);
	cout << "a= " << to_string(a) << endl;

	glm::vec<3, float> b(3, 2, 1);

	cout << "b= " << to_string(b) << endl;

	a = glm::vec3(1, 2, 3);
	cout << "a= " << to_string(a) << endl;

	a.z = 3;
	cout << "a[0]= " << a[0] << endl;
	cout << "a.z= " << a.z << endl;
}

void
part2()
{
	cout << "Vector operations" << endl;

	glm::vec3 a(1, 2, 3);
	glm::vec3 b(3, 2, 1);
	cout << "a=	" << to_string(a) << endl;
	cout << "b=	" << to_string(b) << endl;

	cout << "a+b=	" << to_string(a + b) << endl;
	cout << "a-b=	" << to_string(a - b) << endl;

	cout << "-a =	" << to_string(-a) << endl;

	cout << "1.5*a=	" << to_string(1.5f * a) << endl;

	cout << "dot(a,b)=	" << dot(a, b) << endl;

	cout << "cross(a,b)=	" << to_string(cross(a, b)) << endl;

	cout << "length(a)=	" << length(a) << endl;
	cout << endl;

}

void
part3() {
	cout << "3x3 matrices" << endl;

	glm::mat3 A(0);
	cout << "A= " << to_string(A) << endl;


	A = glm::mat3(1.0);
	cout << "A= " << to_string(A) << endl;

	glm::mat3  B(1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0);
	cout << "B= " << to_string(B) << endl;

	float B_data[9] = { 1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0 };
	B = glm::make_mat3(B_data);
	cout << "B= " << to_string(B) << endl;

	cout << "3rd col of B=" << to_string(B[2]) << endl;
	cout << "3rd col B =" << to_string(column(B, 2)) << endl;
	cout << "3rd row of B=" << to_string(row(B, 2)) << endl;
	cout << "1st row 3rd col of B= " << B[2][0] << endl;
	cout << "1st row 3rd col of B= " << B[2].x << endl;
}

void
part4()
{
	cout << "Matrix operations" << endl;

	glm::mat3 A(1.0);
	glm::mat3 B(1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0);
	cout << "A= " << to_string(A) << endl;
	cout << "B= " << to_string(B) << endl;

	cout << "A+B= " << to_string(A + B) << endl;
	cout << "A-B= " << to_string(A - B) << endl;


	cout << "-A =" << to_string(-A) << endl;

	cout << "1.5*A = " << to_string(1.5f * A) << endl;

	cout << "A x B = " << to_string(A * B) << endl;

	cout << "transpose(B) = " << to_string(transpose(B)) << endl;

	cout << "inverse(B)= " << to_string(inverse(B)) << endl;
	cout << endl;

	cout << "inverse(B)*B=" << to_string(inverse(B) * B) << endl;


}

void
part5()
{
	cout << "Matrix-vector multoplication and assembling" << endl;

	glm::vec3 a(1, 2, 3);
	cout << "a= " << to_string(a) << endl;

	glm::mat3 B(1.0, 0.0, 0.0, 2.0, 1.0, 0.0, 3.0, 0.0, 1.0);
	cout << "B = " << to_string(B) << endl;

	cout << "B x a = " << to_string(B * a) << endl;
	cout << "axB=" << to_string(a * B) << endl;

	cout << "(a,1.0)= " << to_string(glm::vec4(a, 1.0)) << endl;
	cout << "(1.0,a)= " << to_string(glm::vec4(1.0, a)) << endl;

	glm::mat4 C = glm::mat4(B);
	cout << "C = " << to_string(C) << endl;
}
void 
part6(){
	cout << "1.Compute the following vector operations" << endl;

	glm::vec3 v1(1, 2, 3);
	glm::vec3 v2(3, 2, 1);
	cout << "v1=	" << to_string(v1) << endl;
	cout << "v2=	" << to_string(v2) << endl;

	cout << "1.v1+v2=	" << to_string(v1 + v2) << endl;
	cout << "2.v1-v2=	" << to_string(v1 - v2) << endl;
	cout << "3.-v1 =		" << to_string(-v1) << endl;
	cout << "4.v1-2v2=	" << to_string(v1 - 2.0f * v2) << endl;
	cout << "5.dot(v1,v2)=	" << dot(v1, v2) << endl;
	cout << "6.cross(a,b)=	" << to_string(cross(v1, v2)) << endl;
	
	cout << endl;
}
void
part7() {
	cout << "2.Compute the following matrix operations" << endl;


	glm::mat3 A1(1.0, 2.0, 1.0, 2.0, 3.0, 1.0, 3.0, 2.0, 2.0);
	cout << "A1 = " << to_string(A1) << endl;
	glm::mat3 A2(2.0, 2.0, 1.0, 1.0, 2.0, 1.0, 2.0, 1.0, 1.0);
	cout << "A2 = " << to_string(A2) << endl;
	glm::vec3 v1(1, 2, 3);
	cout << "v1=" << to_string(v1) << endl;
	glm::vec3 v2(3, 2, 1);
	cout << "v2=" << to_string(v2) << endl;

	cout << "1.A1+A2=" << to_string(A1 + A2) << endl;
	cout << "2.A1-A2=" << to_string(A1 - A2) << endl;
	cout << "3.-A1 =" << to_string(-A1) << endl;
	cout << "4.A1-2A2=" << to_string(A1 - 2.0f * A2) << endl;
	cout << "5.A1*A2=" << to_string(A1 * A2) << endl;
	cout << "6.A2*A1=" << to_string(A2 * A1) << endl;
	cout << "7.A1*v1=" << to_string(A1 * v1) << endl;
	cout << "8.A2*v2=" << to_string(A2 * v2) << endl;
	cout << endl;

}
int main(int argc, char* argv[])
{
	int e = (argc < 2) ? 1 : atoi(argv[1]);

	switch (e)
	{
	case 1: part1(); break;
	case 2: part2(); break;
	case 3: part3(); break;
	case 4: part4(); break;
	case 5: part5(); break;
	case 6: part6(); break;
	case 7: part7(); break;
	}

	return 0;
}