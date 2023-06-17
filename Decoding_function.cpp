#define _CRT_SECURE_NO_DEPRECATE
#include "Decoding_function.h"
#include <algorithm>
using namespace std;

bool huff_sort(const code& l, const code& r)
{
	return l.huffcode < r.huffcode;	
}

bool search_code(vector<code>& v, string& s, BYTE word[2])
{
	
	int left = 0;
	int right = v.size() - 1;

	while (left <= right)
	{
		int mid = (left + right) / 2;

		if (v[mid].huffcode < s)
			left = mid + 1;

		else if (s < v[mid].huffcode)
			right = mid - 1;

		else {
			word[0] = v[mid].name[0];
			word[1] = v[mid].name[1];
			return true;
		}
	}
	return false;
}

bool huffman_decode(string name)
{
	
	vector<code> v;	

	FILE* file = fopen(name.c_str(), "rb");
	if (file == NULL)
	{
		return false;
	}
	string outfilename = name;
	outfilename.pop_back();
	outfilename.pop_back();
	outfilename.pop_back();
	outfilename += "txt";
	FILE* decoded = fopen(outfilename.c_str(), "wt");
	char msb;
	int codenum;

	fscanf(file, "%c", &msb);
	fscanf(file, "%d", &codenum);

	for (int i = 0; i < codenum; ++i)
	{
		code item;
		char validbit;

		fscanf(file, "%c", &item.name[0]);

		if (item.name[0] > 127)
			fscanf(file, "%c", &item.name[1]);
		else
			item.name[1] = 0;

		fscanf(file, "%c", &validbit);

		BYTE buffer = 0;
		while (validbit > 0)
		{
			fscanf(file, "%c", &buffer);

			for (int j = 7; j >= 0; --j) {
				if (validbit <= 0)
					break;
				char bitdata = (buffer >> j) & 1;

				item.huffcode.push_back(bitdata + '0');
				--validbit;
			}
		}
		v.push_back(item);
	}

	
	
	sort(v.begin(), v.end(), huff_sort);

	BYTE buffer, EOFcheck;
	int cnt = 0;
	string huffcode;
	while (fscanf(file, "%c", &buffer) != EOF)
	{
		if (fscanf(file, "%c", &EOFcheck) == EOF)
		{
			for (int i = 7; i >= 7 - msb; --i)	
			{
				char bitdata = (buffer >> i) & 1;
				huffcode.push_back(bitdata + '0');

				BYTE write_word[2] = { 0 };
				bool found = false;


				found = search_code(v, huffcode, write_word);

				if (found)
				{
					fprintf(decoded, "%c", write_word[0]);
				
					if (write_word[0] > 127) {
						fprintf(decoded, "%c", write_word[1]);
						
					}
					huffcode.clear();
					break;
				}
			}
		}
		else
		{
			fseek(file, -1, SEEK_CUR);
			for (int i = 7; i >= 0; --i)
			{
				char bitdata = (buffer >> i) & 1;
				huffcode.push_back(bitdata + '0');

				BYTE write_word[2] = { 0 };
				bool found = false;


				found = search_code(v, huffcode, write_word);

				if (found)
				{
					fprintf(decoded, "%c", write_word[0]);
					
					if (write_word[0] > 127) {
						fprintf(decoded, "%c", write_word[1]);
						
					}
					huffcode.clear();
				}
			}
		}
	}

	fclose(file);
	fclose(decoded);

	return true;
}