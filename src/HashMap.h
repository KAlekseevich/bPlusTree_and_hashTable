#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <list>

//std::string Text = "������ ������ ����� ������ ���� ����� �����, � ��� ������ ������� ��������� ��� ������ ������, ������� �� �� ��� �����������, �� ������ �����������. �������� ���� ������, �� � ���������.";

class HashMap
{

	std::vector<std::vector<std::pair<char, std::string>>> Value;
	int capacity;
	int size;

public:
	HashMap();
	void insert(std::string keys, bool flag = true);
	void remove(std::string keys);
	//std::wstring find(std::wstring keys);
	bool empty();
	void prepare();
	std::pair<int, int> findIND(std::string keys);
	void clear();
	int make_hash(char keys);
	void make_dict(std::string Text, bool flag1 = true);
	void show();
	void addFromFile();

};

