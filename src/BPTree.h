#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class BPTree;

class Node {
	bool leaf;
	int KeysNum;
	std::string* keys;
	Node* parent;
	Node** child;
	friend class BPTree;
public:
	Node();
};

class BPTree {

	Node* root;
	Node* findParent(Node* current, Node* child);
	void insertInternal(std::string x, Node* current, Node* child);
	void removeInternal(std::string x, Node* current, Node* child);
	void changeOld(Node* current, Node* Ñhild, std::string lastfirstel);

	void clearNode(Node* node);

public:
	BPTree();
	void insert(std::string x, bool flag = true);
	void remove(std::string x);
	void show(Node* current, int depth = 0, std::string prefix = "");
	void make_bptree(std::string Text, bool flag1 = true);
	Node* getRoot();
	bool find(std::string x);
	void addFromFile();
	void clear();
	bool empty() { return root == nullptr; };
};


