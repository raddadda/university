#include <iostream>
#include "Encoding_function.h"
#include "Decoding_function.h"
#include "data.h"
#include <ctime>
using namespace std;

int main()
{
	int input;
	bool flag = true;

	while (flag) {
		cout << ">> 1. 파일압축(Compression)  \n2. 복호화(Decompression)  \n3. 종료 \n>>";
		cin >> input;
		clock_t start;
		string name;
		if (input == 1)
		{
			priority_queue<code, vector<code>, compresscode> huffcode;
			cout << ">> 압축하고자하는 파일(확장자포함)명 입력.\n>>";
			cin >> name;

			start = clock();
			if (huffman_encode(name, huffcode) == false)	//error
			{
				printf("파일 이름이 잘못 되었습니다.\n");
				return 0;
			}
		}
		else if (input == 2)
		{
			cout << ">> 복호화하고자하는 파일(확장자포함)명 입력\n>>";
			cin >> name;
			start = clock();
			if (huffman_decode(name) == false)
			{
				printf("파일 이름이 잘못 되었습니다.\n");
				return false;
			}
		}
		else if (input == 3)
		{
			cout << ">> 종료합니다\n";
			flag = false;
		}
		else
		{
			cout << ">> 1~3번 중 입력 \n";
		}
	}
	return 0;
}