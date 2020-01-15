#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <sstream> 
#include <cmath> 
#include <bitset>
using namespace std;

struct Node
{
	char lt;
	int freq;
	Node* left, * right;
};

Node* getNode(char lt, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->lt = lt;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}


struct compare
{
	bool operator()(Node* l, Node* r)
	{
		return l->freq > r->freq;
	}
};



void encode(Node* root, string str, vector<char>& letter, vector<string>& bit)
{
	if (root == nullptr)
		return;

	if (root->lt != '%') {
		letter.push_back(root->lt);
		bit.push_back(str);

	}
	encode(root->left, str + "0", letter, bit);
	encode(root->right, str + "1", letter, bit);
}


int FromBin(int n)
{
	int increment;
	int Result;
	increment = 1;
	Result = 0;
	while (n != 0)
	{
		if (n % 10 == 1) {
			Result = Result + increment;
			n = n - 1;
		}
		n = n / 10;
		increment = increment * 2;
	}
	return Result;
}


void HTencode(string text, vector<int> freq, int size, vector<char>& letter, vector<string>& bit)
{
	priority_queue<Node*, vector<Node*>, compare> PriorityQ;

	
	for (int i = 0; i < size;i++) {
		PriorityQ.push(getNode(text[i], freq[i], nullptr, nullptr));
	}

	while (PriorityQ.size() != 1)
	{
		Node* left = PriorityQ.top(); 
		PriorityQ.pop();
		Node* right = PriorityQ.top();	
		PriorityQ.pop();

		int sum = left->freq + right->freq;
		PriorityQ.push(getNode('%', sum, left, right));
	}

	Node* root = PriorityQ.top();


	encode(root, "", letter, bit);


	for (int i = 0; i < bit.size(); i++)
	{
		cout << letter[i] << " = " << bit[i] << endl;
	}
	return;
}

string printEncodedString(string text, vector<char>& letter, vector<string>& bit)
{
	string encoded;
	for (int i = 0; i < text.size(); i++)
	{
		for (int j = 0; j < bit.size(); j++)
		{
			if (text[i] == letter[j])
			{
				encoded += bit[j];
				break;
			}		
		}
	}
	return encoded;
}




string CompresEncodedString(string text)
{

	int size = 8 - (text.size() % 8);

	string compress = text;
	string compressed;
	//string s;
	for (int i = 0; i < size; i++)
	{
		compress += '0';
	}

	for (int i = 0; i < compress.length(); i += 8)
	{
		string s = compress.substr(i, 8);


		int x = stoi(s);
	
		int z = FromBin(x);
		compressed += (char)z;
	}




	return compressed;
	
}


void addToFile(string compressed, vector<char>& letter, vector<string>& bit ,int encodedsize)
{
	ofstream outfile;
	outfile.open("compressed.txt", ofstream::trunc); 
	outfile << encodedsize<<endl;
	outfile << bit.size()<<endl;
	
	for (int i = 0; i < bit.size(); i++)
	{
		outfile << letter[i] << bit[i] << endl;
	}
	//cout << compressed;


	outfile << compressed;
	
	outfile.close();
	return;
}

void decode(string enc)
{
	ifstream textfile("compressed.txt");
	string tx;
	vector<char> l;
	vector<string> b;

	char c;
	string bt;

	string text;

	string encoded1;
	string encoded2;
	string encoded;

	int arraySize;
	string aS;
	int txtSize;
	string tS;



	getline(textfile, tS);
	getline(textfile, aS);

	arraySize = stoi(aS);
	txtSize = stoi(tS);

	for (int i = 0; i < arraySize; i++)
	{

		getline(textfile, tx);
		c = tx[0];


		bt = tx.substr(1,tx.length()-1);
		l.push_back(c);
		b.push_back(bt);
		
		
	}

	//getline(textfile, encoded);
	//cout << "HHHSHSHHHS   " << encoded1 << endl;
	while (!textfile.eof())
	{
		textfile >> encoded1;
		encoded += encoded1;
	} 

	//cout << endl<< endl << encoded << endl << endl;
	string decomp;

	for (int i = 0; i < encoded.size(); i++)
	{
		bitset<8> bs = ((int)encoded[i]);
		decomp += bs.to_string();
	}
	decomp=decomp.substr(0, txtSize);


	string decoded;
	string str;
	for (int i = 0; i <= decomp.length(); i++)
	{
		str += decomp[i];
		for (int j = 0; j < b.size();j++)
		{
			if (str == b[j])
			{
				decoded += l[j];
				str.clear();
			
			}
		}
	}


	cout << endl;
	cout << decoded;
	cout << endl;
	return;
}




int main()
{
	ifstream textfile("string.txt");
	string text;
	string txt;

	while (!textfile.eof())
	{
		getline(textfile, txt);
		text += txt;

		//cout << "HHHSHSHHHS   " << encoded1 << endl;
	} 

	

	int frequen[256] = { 0 }; //ascii freq


	int count = 0;
	string let;

	vector<int> fq;

	for (int i = 0; i < text.length(); i++)
	{
		//cout << text[i]<<endl;
		frequen[text[i]]++;
	}

	for (int i = 0; i < 256; i++) {
		if (frequen[i] != 0)
		{
			let += (char)i;
			fq.push_back(frequen[i]);
		}
	}

	vector<char> letter;
	vector<string> bit;

	HTencode(let, fq, fq.size(),letter,bit);

	cout << "This is the encoded file " << endl;
	string encoded=printEncodedString(text, letter, bit);
	
	int encodedsize = encoded.length();

	cout << encoded;
	cout << endl;
	cout << "This is the Compressed file " << endl;

	string compressed= CompresEncodedString(encoded);
	cout << compressed;
	cout << endl;



	addToFile(compressed, letter, bit, encodedsize);
	/*cout << "This is the decoded file ";
	decode(compressed);*/

	float sum = 0;
	for (int i = 0; i < bit.size(); i++)
	{
		sum += bit[i].length();
	}

	float effeciency = sum/ letter.size()/8;
	cout << "Effeciency= " << effeciency;

	return 0;
}

