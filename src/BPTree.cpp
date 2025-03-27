#include "BPTree.h"
#include <sstream>

const int MaxKeysNum = 4;

Node::Node()
{
	keys = new std::string[MaxKeysNum];
	child = new Node * [MaxKeysNum + 1];
	parent = nullptr;
	leaf = false;
	KeysNum = 0;
}
BPTree::BPTree()
{
	root = nullptr;
}

std::string toLowWord(std::string word) {

	std::string result = "";
	for (char c : word) {
		if (c >= 'А' && c <= 'Я')
			result += c + ('а' - 'А');
		else
			result += c;
	}
	return result;
}

void BPTree::insert(std::string x, bool flag)
{
	//делаем все буквы слова с нижнего регистра. 
	// Чтоб одно слово написанное с заглавной и строчной буквы было добавлено в словарь 1 раз
	x = toLowWord(x);

	//проверка, что слова нет в словаре. если слово найдено, не добавляем
	if (find(x)) {

		if (flag) std::cout << "Слово " << x << " уже добавлено в словарь!" << std::endl;
		return;

	}

	//если дерево не создано, создаем корень, вставляем новое значение
	if (root == nullptr) {
		root = new Node;
		root->keys[0] = x;
		root->leaf = true;
		root->KeysNum++;
	}
	else {
		Node* current = root;
		Node* parent = nullptr;
		//спускаемся до нужного листа, в который можно вставить новое значение
		while (!current->leaf) {
			parent = current;
			for (int i = 0; i < current->KeysNum; i++) {
				if (x < current->keys[i]) {
					current = current->child[i];
					break;
				}
				if (i == current->KeysNum - 1) {
					current = current->child[i + 1];
					break;
				}
			}
		}
		// если лист не полон, вставляем новое значение
		if (current->KeysNum < MaxKeysNum) {

			int i = 0;

			while (x > current->keys[i] && i < current->KeysNum) i++;

			for (int j = current->KeysNum; j > i; j--)
				current->keys[j] = current->keys[j - 1];

			current->keys[i] = x;
			current->KeysNum++;
			//переносим указатель каждый раз в конец
			current->child[current->KeysNum] = current->child[current->KeysNum - 1];
			//очищаем прошлое место хранения
			current->child[current->KeysNum - 1] = nullptr;

		}

		else {

			// в противном случае создаем новый лист
			Node* NewLeaf = new Node;
			NewLeaf->leaf = true;

			// Newkeys будет хранить отсортированные значения листа и нового значения
			std::string* Newkeys = new std::string[MaxKeysNum + 1];

			for (int i = 0; i < MaxKeysNum; i++)
				Newkeys[i] = current->keys[i];

			int i = 0;
			while (x > Newkeys[i] && i < MaxKeysNum) i++;

			for (int j = MaxKeysNum; j > i; j--)
				Newkeys[j] = Newkeys[j - 1];

			Newkeys[i] = x;

			//заполним старый лист первый половиной значений из Newkeys
			current->KeysNum = (MaxKeysNum + 1) / 2;

			//заполним новый лист второй половиной значений из Newkeys
			for (int i = current->KeysNum; i < MaxKeysNum + 1; i++) {

				NewLeaf->keys[NewLeaf->KeysNum++] = Newkeys[i];

				//очищаем в старом листе значения которые были перенесены в новый лист
				if (i < MaxKeysNum)
					current->keys[i] = "";

			}

			//в старом листе оставим первую половину значений
			for (int i = 0; i < current->KeysNum; i++)
				current->keys[i] = Newkeys[i];

			//текущий лист будет указывать на новый
			current->child[current->KeysNum] = NewLeaf;

			//теперь новый лист будет содержать указатель старого
			NewLeaf->child[NewLeaf->KeysNum] = current->child[MaxKeysNum];

			//занулим указатель старого
			current->child[MaxKeysNum] = nullptr;

			//если переполненный лист оказался корнем, создаем новый.
			//листья - текущий и новый лист
			if (current == root) {

				Node* NewRoot = new Node;
				NewRoot->keys[0] = NewLeaf->keys[0];
				NewRoot->child[0] = current;
				NewRoot->child[1] = NewLeaf;
				NewRoot->KeysNum++;
				root = NewRoot;

			}
			else {
				//в противном случае поднимаемся вверх,
				//вставляем ключ в родителя
				insertInternal(NewLeaf->keys[0], parent, NewLeaf);
			}
		}
	}

	if (flag) std::cout << "Слово " << x << " добавлено в словарь!" << std::endl;

}

void BPTree::insertInternal(std::string x, Node* current, Node* Child)
{

	// если в родительском узле хватает места то вставляем ключ и указатель в нужное место
	if (current->KeysNum < MaxKeysNum) {

		int i = 0;

		while (x > current->keys[i] && i < current->KeysNum) i++;

		for (int j = current->KeysNum; j > i; j--)
			current->keys[j] = current->keys[j - 1];

		for (int j = current->KeysNum + 1; j > i + 1; j--)
			current->child[j] = current->child[j - 1];

		current->keys[i] = x;
		current->KeysNum++;
		current->child[i + 1] = Child;
	}
	// если родительский узел полон нужно создать новый узел куда будут перенесена вторая половина данных старого
	else {

		Node* NewInternal = new Node;

		std::string* Newkeys = new std::string[MaxKeysNum + 1];
		Node** Newchild = new Node * [MaxKeysNum + 2];

		for (int i = 0; i < MaxKeysNum; i++)
			Newkeys[i] = current->keys[i];

		for (int i = 0; i < MaxKeysNum + 1; i++)
			Newchild[i] = current->child[i];

		int i = 0;

		while (x > Newkeys[i] && i < MaxKeysNum) i++;

		for (int j = MaxKeysNum; j > i; j--) {
			Newkeys[j] = Newkeys[j - 1];
		}

		Newkeys[i] = x;

		for (int j = MaxKeysNum + 1; j > i + 1; j--) {
			Newchild[j] = Newchild[j - 1];
		}

		Newchild[i + 1] = Child;

		////////////
		current->KeysNum = (MaxKeysNum + 1) / 2;

		//заполним новый лист второй половиной значений из Newkeys
		for (int i = current->KeysNum + 1; i < MaxKeysNum + 1; i++)
			NewInternal->keys[NewInternal->KeysNum++] = Newkeys[i];

		for (int i = current->KeysNum + 1, j = 0; i < MaxKeysNum + 2; i++, j++)
			NewInternal->child[j] = Newchild[i];

		//заполним старый лист первой половиной значений из Newkeys
		for (int i = 0; i < MaxKeysNum + 1; i++) {
			if (i < current->KeysNum + 1)
				current->child[i] = Newchild[i];
			else
				current->child[i] = nullptr;
		}

		for (int i = 0; i < MaxKeysNum; i++) {
			if (i < current->KeysNum)
				current->keys[i] = Newkeys[i];
			else
				current->keys[i] = "";
		}

		if (current == root) {
			//если узел для которого нужно создать родителя - корень, создаем новый корень
			Node* NewRoot = new Node;
			NewRoot->keys[0] = Newkeys[current->KeysNum];
			NewRoot->child[0] = current;
			NewRoot->child[1] = NewInternal;
			NewRoot->leaf = false;
			NewRoot->KeysNum = 1;
			root = NewRoot;
		}
		else {
			// в противном случае ищем родителя и заносим в него
			insertInternal(Newkeys[current->KeysNum], findParent(root, current), NewInternal);
		}
	}
}

Node* BPTree::findParent(Node* current, Node* Child)
{
	// Поиск в ширину родителя. В листе и предпоследнем уровне не ищем.
	Node* parent = nullptr;
	if (current->leaf || (current->child[0])->leaf)
		return nullptr;

	for (int i = 0; i < current->KeysNum + 1; i++) {
		if (current->child[i] == Child) {
			parent = current;
			return parent;
		}
		else {
			parent = findParent(current->child[i], Child);
			if (parent != nullptr) return parent;
		}
	}
	return parent;
}

void BPTree::show(Node* current, int depth, std::string prefix) {
	if (current != nullptr) {
		
		if (depth > 0) 			
			std::cout << prefix << " \\-- ";
		
		std::cout << "[";

		for (int i = 0; i < current->KeysNum; i++) 
			std::cout << current->keys[i] << (i < current->KeysNum - 1 ? ", " : "");
		
		std::cout << "]" << std::endl;

		if (!current->leaf) {
			for (int i = 0; i <= current->KeysNum; i++) 				
				show(current->child[i], depth + 1, prefix + (depth > 0 ? "|   " : "    "));			
		}
	}
}


Node* BPTree::getRoot()
{
	return root;
}

void BPTree::make_bptree(std::string Text,bool flag1) {

	std::vector<std::string> splitStr;
	std::string word = "";

	for (char i : Text) {

		if (static_cast<int>(i) >= -64 && static_cast<int>(i) <= -1)
			word += i;

		else if (i == ' ') {
			splitStr.push_back(word);
			word = "";
		}
	}

	if (word != "")
		splitStr.push_back(word);

	for (auto w : splitStr) {
		//std::cout << w << std::endl;
		insert(w,false);
	}

	if (flag1) std::cout << "B+ дерево сформировано!" << std::endl;
}


bool BPTree::find(std::string x) {

	if (root == nullptr) return false;

	Node* current = root;

	while (!current->leaf) {
		for (int i = 0; i < current->KeysNum; i++) {
			if (x < current->keys[i]) {
				current = current->child[i];
				break;
			}
			if (i == current->KeysNum - 1) {
				current = current->child[i + 1];
				break;
			}
		}
	}

	for (int i = 0; i < current->KeysNum; i++)
		if (current->keys[i] == x) return true;

	return false;

}

void BPTree::addFromFile() {

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

	make_bptree(res,false);

	std::cout << "Слова из файла добавлены!" << std::endl;

}

void BPTree::changeOld(Node* current, Node* Сhild, std::string lastfirstel) {

	if (Сhild->keys[0] == lastfirstel) return;

	for (int i = 0; i < current->KeysNum; i++) {
		if (current->keys[i] == lastfirstel) {
			current->keys[i] = Сhild->keys[0];
			return;
		}
	}

	Node* Parent = findParent(root, current);

	for (int i = 0; i < Parent->KeysNum; i++) {
		if (Parent->keys[i] == lastfirstel) {
			Parent->keys[i] = Сhild->keys[0];
			return;
		}
	}
}

void BPTree::remove(std::string x)
{
	if (root == nullptr) {
		std::cout << "Словарь пуст" << std::endl;
		return;
	}

	x = toLowWord(x);

	Node* current = root;
	Node* parent = nullptr;
	int leftSibling = -1, rightSibling = 100;

	//спускаемся до нужного листа, находим левого и правого соседа в родительском узле
	while (!current->leaf) {
		for (int i = 0; i < current->KeysNum; i++) {
			parent = current;
			leftSibling = i - 1;
			rightSibling = i + 1;
			if (x < current->keys[i]) {
				current = current->child[i];
				break;
			}
			if (i == current->KeysNum - 1) {
				leftSibling = i;
				rightSibling = i + 2;
				current = current->child[i + 1];
				break;
			}
		}
	}

	// проверяем, существует ли такое слово

	int index = -1;
	for (int i = 0; i < current->KeysNum; i++) {
		if (current->keys[i] == x) {
			index = i;
			break;
		}
	}

	// если нет, выводим соответствующее сообщение и выходим из функции
	if (index == -1) {
		std::cout << "Слово " << x << " отсутствует в словаре" << std::endl;
		return;
	}

	//запоминаем первое слово текущего листа
	std::string firstel = current->keys[0];

	//удаляем слово
	if (current->leaf) {
		if (current->KeysNum != 1) {
			for (int i = index; i < current->KeysNum - 1; i++)
				current->keys[i] = current->keys[i + 1];
		}
		current->keys[--current->KeysNum] = "";
	}

	//если мы находимся в корне, значит больше узлов нет
	// соответственно указатели на потомков должны быть нулевые
	if (current == root) {
		for (int i = 0; i < MaxKeysNum + 1; i++)
			current->child[i] = nullptr;
		// если корень, пуст удаляем его
		if (current->KeysNum == 0) {
			delete[] current->keys;
			delete[] current->child;
			delete current;
			root = nullptr;
			std::cout << "Словарь пуст" << std::endl;
			return;
		}
		std::cout << "\n Слово " << x << " удалено " << std::endl;
		return;
	}

	//т.к мы удалили слово и уменьшили кол-во ключей, нужно сдвинуть указатель
	//который хранится под индексом, равным числу ключей
	current->child[current->KeysNum] = current->child[current->KeysNum + 1];
	current->child[current->KeysNum + 1] = nullptr;

	//если удаленный из листа ключ находился в родительском узле, меняем его на новый
	changeOld(parent, current, firstel);

	// если ключей больше половины, ничего не делаем и выходим
	if (current->KeysNum >= (MaxKeysNum + 1) / 2) {
		std::cout << "\n Слово " << x << " удалено " << std::endl;
		return;
	}

	//вначале пытаемся перенести ключ из соседей
	//проверяем, существует ли левый сосед
	if (leftSibling >= 0) {
		Node* leftNode = parent->child[leftSibling];
		//проверяем, что ключей больше половины
		if (leftNode->KeysNum >= (MaxKeysNum + 1) / 2 + 1) {
			std::string First = current->keys[0];
			//сдвигаем ключи вперед на 1 шаг, чтобы вставить в начало ключ из левого соседа
			for (int i = current->KeysNum; i > 0; i--)
				current->keys[i] = current->keys[i - 1];

			current->KeysNum++;
			current->child[current->KeysNum] = current->child[current->KeysNum - 1];
			current->child[current->KeysNum - 1] = nullptr;
			//вставляем в начало текущего листа последний ключ из левого соседа
			current->keys[0] = leftNode->keys[leftNode->KeysNum - 1];
			leftNode->keys[leftNode->KeysNum - 1] = "";

			leftNode->KeysNum--;
			leftNode->child[leftNode->KeysNum] = current;
			leftNode->child[leftNode->KeysNum + 1] = nullptr;
			//в родителе меняем ключ
			parent->keys[leftSibling] = current->keys[0];

			changeOld(parent, current, First);

			std::cout << "\n Слово " << x << " удалено " << std::endl;
			return;
		}
	}
	//проверяем, существует ли правый сосед
	if (rightSibling <= parent->KeysNum) {
		Node* rightNode = parent->child[rightSibling];
		//проверяем, что ключей больше половины
		if (rightNode->KeysNum >= (MaxKeysNum + 1) / 2 + 1) {
			current->KeysNum++;
			current->child[current->KeysNum] = current->child[current->KeysNum - 1];
			current->child[current->KeysNum - 1] = nullptr;
			//вставляем в конец текущего листа первый ключ из правого соседа
			current->keys[current->KeysNum - 1] = rightNode->keys[0];

			rightNode->KeysNum--;
			rightNode->child[rightNode->KeysNum] = rightNode->child[rightNode->KeysNum + 1];
			rightNode->child[rightNode->KeysNum + 1] = nullptr;
			//сдвигаем ключи правого листа на 1 шаг назад
			for (int i = 0; i < rightNode->KeysNum; i++)
				rightNode->keys[i] = rightNode->keys[i + 1];

			rightNode->keys[rightNode->KeysNum] = "";
			//в родителе меняем ключ
			parent->keys[rightSibling - 1] = rightNode->keys[0];

			std::cout << "\n Слово " << x << " удалено " << std::endl;
			return;
		}
	}

	//если не получилось забрать ключ из соседа, будем объеденять текущий узел с соседним

	//проверяем, существует ли левый сосед
	if (leftSibling >= 0) {

		Node* leftNode = parent->child[leftSibling];

		//переносим  ключи из текущего листа в левого соседа
		for (int i = leftNode->KeysNum, j = 0; j < current->KeysNum; i++, j++)
			leftNode->keys[i] = current->keys[j];

		leftNode->child[leftNode->KeysNum] = nullptr;
		leftNode->KeysNum += current->KeysNum;
		//теперь левый узел содержит указатель текущего узла
		leftNode->child[leftNode->KeysNum] = current->child[current->KeysNum];

		//удаляем ключ и указатель из родителя
		removeInternal(parent->keys[leftSibling], parent, current);
		
	}
	//проверяем, существует ли правый сосед
	else if (rightSibling <= parent->KeysNum) {
		Node* rightNode = parent->child[rightSibling];

		//переносим ключи из правого соседа в текущий лист
		for (int i = current->KeysNum, j = 0; j < rightNode->KeysNum; i++, j++)
			current->keys[i] = rightNode->keys[j];

		current->child[current->KeysNum] = nullptr;
		current->KeysNum += rightNode->KeysNum;
		//теперь текущий узел содержит указатель правого соседа
		current->child[current->KeysNum] = rightNode->child[rightNode->KeysNum];

		//удаляем ключ и указатель из родителя
		removeInternal(parent->keys[rightSibling - 1], parent, rightNode);
	
	}

	std::cout << "\n Слово " << x << " удалено " << std::endl;

}

void BPTree::removeInternal(std::string x, Node* current, Node* Сhild)
{

	if (current == root) {
		//если узел который нужно удалить - один из двух в корне, удаляем старый корень
		//корень будет одним из листов 
		if (current->KeysNum == 1) {
			if (current->child[1] == Сhild) {
				delete[] Сhild->keys;
				delete[] Сhild->child;
				delete Сhild;
				root = current->child[0];
				delete[] current->keys;
				delete[] current->child;
				delete current;
				return;
			}
			else if (current->child[0] == Сhild) {
				delete[] Сhild->keys;
				delete[] Сhild->child;
				delete Сhild;
				root = current->child[1];
				delete[] current->keys;
				delete[] current->child;
				delete current;
				return;
			}
		}
	}

	//находим индекс ключа, который нужно удалить
	int index;
	for (index = 0; index < current->KeysNum; index++) {
		if (current->keys[index] == x)
			break;
	}

	current->KeysNum--;

	//удаляем ключ
	for (int i = index; i < current->KeysNum; i++)
		current->keys[i] = current->keys[i + 1];


	current->keys[current->KeysNum] = "";

	//находим индекс указателя, который нужно удалить
	for (index = 0; index < current->KeysNum + 2; index++) {
		if (current->child[index] == Сhild)
			break;

	}

	//удаляем указатель
	for (int i = index; i < current->KeysNum + 1; i++)
		current->child[i] = current->child[i + 1];

	current->child[current->KeysNum + 1] = nullptr;

	//если ключей больше половины, дополнительных действий не требуется
	if (current->KeysNum >= (MaxKeysNum + 1) / 2 - 1)
		return;

	//если мы в корне, выходим
	if (current == root)
		return;

	Node* parent = findParent(root, current);
	int leftSibling = -1, rightSibling = 100;
	//ищем левого и правого соседа
	for (index = 0; index < parent->KeysNum + 1; index++) {
		if (parent->child[index] == current) {
			leftSibling = index - 1;
			rightSibling = index + 1;
			break;
		}
	}

	if (leftSibling >= 0) {

		Node* leftNode = parent->child[leftSibling];

		if (leftNode->KeysNum >= (MaxKeysNum + 1) / 2) {

			for (int i = current->KeysNum; i > 0; i--)
				current->keys[i] = current->keys[i - 1];


			current->keys[0] = parent->keys[leftSibling];
			parent->keys[leftSibling] = leftNode->keys[leftNode->KeysNum - 1];

			for (int i = current->KeysNum + 1; i > 0; i--)
				current->child[i] = current->child[i - 1];

			current->child[0] = leftNode->child[leftNode->KeysNum];
			current->KeysNum++;
			leftNode->KeysNum--;

			return;
		}
	}
	if (rightSibling <= parent->KeysNum) {

		Node* rightNode = parent->child[rightSibling];

		if (rightNode->KeysNum >= (MaxKeysNum + 1) / 2) {

			current->keys[current->KeysNum] = parent->keys[index];
			parent->keys[index] = rightNode->keys[0];
			for (int i = 0; i < rightNode->KeysNum - 1; i++)
				rightNode->keys[i] = rightNode->keys[i + 1];

			current->child[current->KeysNum + 1] = rightNode->child[0];
			for (int i = 0; i < rightNode->KeysNum; ++i)
				rightNode->child[i] = rightNode->child[i + 1];

			current->KeysNum++;
			rightNode->KeysNum--;

			return;
		}
	}

	if (leftSibling >= 0) {

		Node* leftNode = parent->child[leftSibling];
		leftNode->keys[leftNode->KeysNum] = parent->keys[leftSibling];
		for (int i = leftNode->KeysNum + 1, j = 0; j < current->KeysNum; j++)
			leftNode->keys[i] = current->keys[j];

		for (int i = leftNode->KeysNum + 1, j = 0; j < current->KeysNum + 1; j++) {
			leftNode->child[i] = current->child[j];
			current->child[j] = nullptr;
		}
		leftNode->KeysNum += current->KeysNum + 1;
		current->KeysNum = 0;

		removeInternal(parent->keys[leftSibling], parent, current);

	}
	else if (rightSibling <= parent->KeysNum) {

		Node* rightNode = parent->child[rightSibling];
		current->keys[current->KeysNum] = parent->keys[rightSibling - 1];
		for (int i = current->KeysNum + 1, j = 0; j < rightNode->KeysNum; j++)
			current->keys[i] = rightNode->keys[j];

		for (int i = current->KeysNum + 1, j = 0; j < rightNode->KeysNum + 1; j++) {
			current->child[i] = rightNode->child[j];
			rightNode->child[j] = nullptr;
		}
		current->KeysNum += rightNode->KeysNum + 1;
		rightNode->KeysNum = 0;

		removeInternal(parent->keys[rightSibling - 1], parent, rightNode);

	}
}

void BPTree::clearNode(Node* node) {

	if (node == nullptr)
		return;

	// Если узел не листовой, рекурсивно очищаем дочерние узлы
	if (!node->leaf) {
		for (int i = 0; i <= node->KeysNum; ++i)
			clearNode(node->child[i]);
	}

	// Освобождаем память, занятую текущим узлом
	delete node;

}

void BPTree::clear() {

	clearNode(root);
	root = nullptr; // Устанавливаем корень в nullptr после очистки
	std::cout << "Словарь очищен!" << std::endl;

}


