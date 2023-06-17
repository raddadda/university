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
		cout << ">> 1. ���Ͼ���(Compression)  \n2. ��ȣȭ(Decompression)  \n3. ���� \n>>";
		cin >> input;
		clock_t start;
		string name;
		if (input == 1)
		{
			priority_queue<code, vector<code>, compresscode> huffcode;
			cout << ">> �����ϰ����ϴ� ����(Ȯ��������)�� �Է�.\n>>";
			cin >> name;

			start = clock();
			if (huffman_encode(name, huffcode) == false)	//error
			{
				printf("���� �̸��� �߸� �Ǿ����ϴ�.\n");
				return 0;
			}
		}
		else if (input == 2)
		{
			cout << ">> ��ȣȭ�ϰ����ϴ� ����(Ȯ��������)�� �Է�\n>>";
			cin >> name;
			start = clock();
			if (huffman_decode(name) == false)
			{
				printf("���� �̸��� �߸� �Ǿ����ϴ�.\n");
				return false;
			}
		}
		else if (input == 3)
		{
			cout << ">> �����մϴ�\n";
			flag = false;
		}
		else
		{
			cout << ">> 1~3�� �� �Է� \n";
		}
	}
	return 0;
}