#ifndef a
#define a
#define MAX_WORD 2478	//최대 글자개수
typedef unsigned char BYTE;
#include <queue>	//for priority queue
#include <list>
#include <string>
#include <vector>
using namespace std;

class huffmantree
{
public:
	huffmantree() {}
	huffmantree(unsigned char name1_, unsigned char name2_, int freq_)
	{ 
	name[0] = name1_; 
	name[1] = name2_; 
	frequency = freq_;
	left_child = list<huffmantree>();
	right_child = list<huffmantree>();
	}
	//data

	unsigned char name[2];	//한글 2bytes
	int frequency;	//frequency

	//child
	list<huffmantree>  left_child, right_child;
};

class code
{
public:

	//data
	unsigned char name[2];
	string huffcode;	//허프만부호화코드가 담길곳
};

struct compresstree {
	bool operator()(const huffmantree& l, const huffmantree& r) {
		return l.frequency > r.frequency;	//ascending . ordered by freq
	}
};

struct compresscode {
	bool operator()(const code& l, const code& r) {
		return l.huffcode > r.huffcode;	//ascending . ordered by string
	}
};

#endif
