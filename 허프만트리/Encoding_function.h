

#include "data.h"
using namespace std;

bool calculate_freq(char*, int[][94], int*);
void make_node(int[][94], int*, priority_queue<huffmantree, vector<huffmantree>, compresstree>&);

void make_hufftree(priority_queue<huffmantree, vector<huffmantree>, compresstree>&);

void inorder(const huffmantree&, priority_queue<code, vector<code>, compresscode>&, string);
bool make_code(const priority_queue<huffmantree, vector<huffmantree>, compresstree>&, priority_queue<code, vector<code>, compresscode>&);
bool huffman_encode(string, priority_queue<code, vector<code>, compresscode>&);
void convert_binary(string, priority_queue<code, vector<code>, compresscode>&);



