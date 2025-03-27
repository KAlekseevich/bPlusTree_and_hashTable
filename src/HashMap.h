#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <list>

//std::string Text = "—права –остов видел первые р€ды своих гусар, а еще дальше впереди виднелась ему темна€ полоса, которую он не мог рассмотреть, но считал непри€телем. ¬ыстрелы были слышны, но в отдалении.";

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

