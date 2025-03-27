#include "HashMap.h"
#include "cmath"
#include <fstream>
#include <windows.h>
#include <cwctype>

std::string toLow(std::string word) {

	std::string result = "";
	for (char c : word) {
		if (c >= 'А' && c <= 'Я')
			result += c + ('а' - 'А');
		else
			result += c;
	}
	return result;
}

void HashMap::prepare() {

	capacity = 5;
	size = 0;
	Value.resize(capacity);
}

bool HashMap::empty() {
	return (capacity == 0);
}

HashMap::HashMap() {

	capacity = 5;
	size = 0;
	Value.resize(capacity);

}

int HashMap::make_hash(char keys) {

	return abs(keys % capacity);

}

void HashMap::insert(std::string keys, bool flag) {

	keys = toLow(keys);

	if (findIND(keys).first != -1) {

		if (flag) std::cout << "Такой элемент уже есть в таблице!" << std::endl;		
		return;

	}

	int index = 0;

	if (size>=0.7* capacity) {

		std::vector<std::vector<std::pair<char, std::string>>> New_Value;
		capacity *= 2;
		New_Value.resize(capacity);

		for (std::vector<std::pair<char, std::string>> Vectors : Value) {
			for (std::pair<char, std::string> Pairs : Vectors) {
				index = make_hash(Pairs.first);
				New_Value[index].push_back(make_pair(Pairs.first, Pairs.second));
			}
		}

		Value = New_Value;
	}

	wchar_t bukv = std::tolower(keys[0]);
	index = make_hash(bukv);

	Value[index].push_back(make_pair(bukv, keys));

	size++;

	if (flag) std::cout << "Слово " << keys << " было успешно добавлено в словарь!" << std::endl;
	
}

void HashMap::remove(std::string keys) {

	std::pair<int, int> INDEXES = findIND(keys);

	if (INDEXES.first == -1) {
		std::cout << "Такого элемента нет!" << std::endl;
		return;
	}


	Value[INDEXES.first].erase(Value[INDEXES.first].begin() + INDEXES.second);
	size--;

	std::cout << "Слово " << keys << " было успешно удалено из словаря!" << std::endl;

}

std::pair<int, int> HashMap::findIND(std::string keys) {

	if (size == 0) return std::make_pair(-1, -1);

	int index = make_hash(std::tolower(keys[0]));

	if (Value[index].size() == 0) return std::make_pair(-1, -1);

	for (int i = 0; i < Value[index].size(); i++) {
		if (Value[index][i].second == keys)
			return std::make_pair(index, i);
	}

	return std::make_pair(-1, -1);
}

//std::wstring HashMap::find(std::wstring keys) {
//
//	if (size==0) return L"";
//
//	int index = make_hash(tolower(keys[0]));
//
//	if (Value[index].size() == 0) return L"";
//	
//	for (std::pair<wchar_t, std::wstring> Pairs : Value[index]) {
//		if (Pairs.second == keys)
//			return Pairs.second;
//	}
//
//	return L"";
//
//}

void HashMap::clear() {

	if (size == 0) {
		std::cout << "Чистить нечего!" << std::endl;
		return;
	}

	Value.clear();
	capacity = 0;
	size = 0;

	std::cout << "Хэш-таблица очищена!" << std::endl;

}

void HashMap::make_dict(std::string Text, bool flag1) {

	std::vector<std::string> splitStr;
	std::string word = "";

	for (char i : Text) {
		if (static_cast<int>(i) >= -64 && static_cast<int>(i) <= -1) {
			word += i;
		}
		else if (i == ' ') {
			splitStr.push_back(word);
			word = "";
		}
	}

	if (word != "")
		splitStr.push_back(word);

	for (auto w : splitStr)
		insert(w,false);

	if (flag1) std::cout << "Таблица сформирована" << std::endl;

}

void HashMap::show() {

	if (size == 0) {
		std::cout << "Хэш-таблица пуста!" << std::endl;
		return;
	}

	for (int i = 0; i < Value.size(); i++) {

		if (!Value[i].empty()) std::cout << Value[i][0].first << ":";

		for (int j = 0; j < Value[i].size(); j++) {
			std::cout << '\t' << Value[i][j].second << std::endl;
		}

	}

}

void HashMap::addFromFile() {

	std::ifstream file("BookText.txt");
	file.imbue(std::locale(".utf8"));
	std::string res = "";

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			res += line;
		}
		file.close();
	}
	else {
		std::cerr << "Файл не был найден!" << std::endl;
	}

	make_dict(res, false);

	std::cout << "Слова из файла добавлены!" << std::endl;

}