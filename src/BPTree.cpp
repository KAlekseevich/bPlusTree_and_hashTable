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
		if (c >= '�' && c <= '�')
			result += c + ('�' - '�');
		else
			result += c;
	}
	return result;
}

void BPTree::insert(std::string x, bool flag)
{
	//������ ��� ����� ����� � ������� ��������. 
	// ���� ���� ����� ���������� � ��������� � �������� ����� ���� ��������� � ������� 1 ���
	x = toLowWord(x);

	//��������, ��� ����� ��� � �������. ���� ����� �������, �� ���������
	if (find(x)) {

		if (flag) std::cout << "����� " << x << " ��� ��������� � �������!" << std::endl;
		return;

	}

	//���� ������ �� �������, ������� ������, ��������� ����� ��������
	if (root == nullptr) {
		root = new Node;
		root->keys[0] = x;
		root->leaf = true;
		root->KeysNum++;
	}
	else {
		Node* current = root;
		Node* parent = nullptr;
		//���������� �� ������� �����, � ������� ����� �������� ����� ��������
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
		// ���� ���� �� �����, ��������� ����� ��������
		if (current->KeysNum < MaxKeysNum) {

			int i = 0;

			while (x > current->keys[i] && i < current->KeysNum) i++;

			for (int j = current->KeysNum; j > i; j--)
				current->keys[j] = current->keys[j - 1];

			current->keys[i] = x;
			current->KeysNum++;
			//��������� ��������� ������ ��� � �����
			current->child[current->KeysNum] = current->child[current->KeysNum - 1];
			//������� ������� ����� ��������
			current->child[current->KeysNum - 1] = nullptr;

		}

		else {

			// � ��������� ������ ������� ����� ����
			Node* NewLeaf = new Node;
			NewLeaf->leaf = true;

			// Newkeys ����� ������� ��������������� �������� ����� � ������ ��������
			std::string* Newkeys = new std::string[MaxKeysNum + 1];

			for (int i = 0; i < MaxKeysNum; i++)
				Newkeys[i] = current->keys[i];

			int i = 0;
			while (x > Newkeys[i] && i < MaxKeysNum) i++;

			for (int j = MaxKeysNum; j > i; j--)
				Newkeys[j] = Newkeys[j - 1];

			Newkeys[i] = x;

			//�������� ������ ���� ������ ��������� �������� �� Newkeys
			current->KeysNum = (MaxKeysNum + 1) / 2;

			//�������� ����� ���� ������ ��������� �������� �� Newkeys
			for (int i = current->KeysNum; i < MaxKeysNum + 1; i++) {

				NewLeaf->keys[NewLeaf->KeysNum++] = Newkeys[i];

				//������� � ������ ����� �������� ������� ���� ���������� � ����� ����
				if (i < MaxKeysNum)
					current->keys[i] = "";

			}

			//� ������ ����� ������� ������ �������� ��������
			for (int i = 0; i < current->KeysNum; i++)
				current->keys[i] = Newkeys[i];

			//������� ���� ����� ��������� �� �����
			current->child[current->KeysNum] = NewLeaf;

			//������ ����� ���� ����� ��������� ��������� �������
			NewLeaf->child[NewLeaf->KeysNum] = current->child[MaxKeysNum];

			//������� ��������� �������
			current->child[MaxKeysNum] = nullptr;

			//���� ������������� ���� �������� ������, ������� �����.
			//������ - ������� � ����� ����
			if (current == root) {

				Node* NewRoot = new Node;
				NewRoot->keys[0] = NewLeaf->keys[0];
				NewRoot->child[0] = current;
				NewRoot->child[1] = NewLeaf;
				NewRoot->KeysNum++;
				root = NewRoot;

			}
			else {
				//� ��������� ������ ����������� �����,
				//��������� ���� � ��������
				insertInternal(NewLeaf->keys[0], parent, NewLeaf);
			}
		}
	}

	if (flag) std::cout << "����� " << x << " ��������� � �������!" << std::endl;

}

void BPTree::insertInternal(std::string x, Node* current, Node* Child)
{

	// ���� � ������������ ���� ������� ����� �� ��������� ���� � ��������� � ������ �����
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
	// ���� ������������ ���� ����� ����� ������� ����� ���� ���� ����� ���������� ������ �������� ������ �������
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

		//�������� ����� ���� ������ ��������� �������� �� Newkeys
		for (int i = current->KeysNum + 1; i < MaxKeysNum + 1; i++)
			NewInternal->keys[NewInternal->KeysNum++] = Newkeys[i];

		for (int i = current->KeysNum + 1, j = 0; i < MaxKeysNum + 2; i++, j++)
			NewInternal->child[j] = Newchild[i];

		//�������� ������ ���� ������ ��������� �������� �� Newkeys
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
			//���� ���� ��� �������� ����� ������� �������� - ������, ������� ����� ������
			Node* NewRoot = new Node;
			NewRoot->keys[0] = Newkeys[current->KeysNum];
			NewRoot->child[0] = current;
			NewRoot->child[1] = NewInternal;
			NewRoot->leaf = false;
			NewRoot->KeysNum = 1;
			root = NewRoot;
		}
		else {
			// � ��������� ������ ���� �������� � ������� � ����
			insertInternal(Newkeys[current->KeysNum], findParent(root, current), NewInternal);
		}
	}
}

Node* BPTree::findParent(Node* current, Node* Child)
{
	// ����� � ������ ��������. � ����� � ������������� ������ �� ����.
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

	if (flag1) std::cout << "B+ ������ ������������!" << std::endl;
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
		std::cerr << "���� �� ��� ������!" << std::endl;
	}

	make_bptree(res,false);

	std::cout << "����� �� ����� ���������!" << std::endl;

}

void BPTree::changeOld(Node* current, Node* �hild, std::string lastfirstel) {

	if (�hild->keys[0] == lastfirstel) return;

	for (int i = 0; i < current->KeysNum; i++) {
		if (current->keys[i] == lastfirstel) {
			current->keys[i] = �hild->keys[0];
			return;
		}
	}

	Node* Parent = findParent(root, current);

	for (int i = 0; i < Parent->KeysNum; i++) {
		if (Parent->keys[i] == lastfirstel) {
			Parent->keys[i] = �hild->keys[0];
			return;
		}
	}
}

void BPTree::remove(std::string x)
{
	if (root == nullptr) {
		std::cout << "������� ����" << std::endl;
		return;
	}

	x = toLowWord(x);

	Node* current = root;
	Node* parent = nullptr;
	int leftSibling = -1, rightSibling = 100;

	//���������� �� ������� �����, ������� ������ � ������� ������ � ������������ ����
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

	// ���������, ���������� �� ����� �����

	int index = -1;
	for (int i = 0; i < current->KeysNum; i++) {
		if (current->keys[i] == x) {
			index = i;
			break;
		}
	}

	// ���� ���, ������� ��������������� ��������� � ������� �� �������
	if (index == -1) {
		std::cout << "����� " << x << " ����������� � �������" << std::endl;
		return;
	}

	//���������� ������ ����� �������� �����
	std::string firstel = current->keys[0];

	//������� �����
	if (current->leaf) {
		if (current->KeysNum != 1) {
			for (int i = index; i < current->KeysNum - 1; i++)
				current->keys[i] = current->keys[i + 1];
		}
		current->keys[--current->KeysNum] = "";
	}

	//���� �� ��������� � �����, ������ ������ ����� ���
	// �������������� ��������� �� �������� ������ ���� �������
	if (current == root) {
		for (int i = 0; i < MaxKeysNum + 1; i++)
			current->child[i] = nullptr;
		// ���� ������, ���� ������� ���
		if (current->KeysNum == 0) {
			delete[] current->keys;
			delete[] current->child;
			delete current;
			root = nullptr;
			std::cout << "������� ����" << std::endl;
			return;
		}
		std::cout << "\n ����� " << x << " ������� " << std::endl;
		return;
	}

	//�.� �� ������� ����� � ��������� ���-�� ������, ����� �������� ���������
	//������� �������� ��� ��������, ������ ����� ������
	current->child[current->KeysNum] = current->child[current->KeysNum + 1];
	current->child[current->KeysNum + 1] = nullptr;

	//���� ��������� �� ����� ���� ��������� � ������������ ����, ������ ��� �� �����
	changeOld(parent, current, firstel);

	// ���� ������ ������ ��������, ������ �� ������ � �������
	if (current->KeysNum >= (MaxKeysNum + 1) / 2) {
		std::cout << "\n ����� " << x << " ������� " << std::endl;
		return;
	}

	//������� �������� ��������� ���� �� �������
	//���������, ���������� �� ����� �����
	if (leftSibling >= 0) {
		Node* leftNode = parent->child[leftSibling];
		//���������, ��� ������ ������ ��������
		if (leftNode->KeysNum >= (MaxKeysNum + 1) / 2 + 1) {
			std::string First = current->keys[0];
			//�������� ����� ������ �� 1 ���, ����� �������� � ������ ���� �� ������ ������
			for (int i = current->KeysNum; i > 0; i--)
				current->keys[i] = current->keys[i - 1];

			current->KeysNum++;
			current->child[current->KeysNum] = current->child[current->KeysNum - 1];
			current->child[current->KeysNum - 1] = nullptr;
			//��������� � ������ �������� ����� ��������� ���� �� ������ ������
			current->keys[0] = leftNode->keys[leftNode->KeysNum - 1];
			leftNode->keys[leftNode->KeysNum - 1] = "";

			leftNode->KeysNum--;
			leftNode->child[leftNode->KeysNum] = current;
			leftNode->child[leftNode->KeysNum + 1] = nullptr;
			//� �������� ������ ����
			parent->keys[leftSibling] = current->keys[0];

			changeOld(parent, current, First);

			std::cout << "\n ����� " << x << " ������� " << std::endl;
			return;
		}
	}
	//���������, ���������� �� ������ �����
	if (rightSibling <= parent->KeysNum) {
		Node* rightNode = parent->child[rightSibling];
		//���������, ��� ������ ������ ��������
		if (rightNode->KeysNum >= (MaxKeysNum + 1) / 2 + 1) {
			current->KeysNum++;
			current->child[current->KeysNum] = current->child[current->KeysNum - 1];
			current->child[current->KeysNum - 1] = nullptr;
			//��������� � ����� �������� ����� ������ ���� �� ������� ������
			current->keys[current->KeysNum - 1] = rightNode->keys[0];

			rightNode->KeysNum--;
			rightNode->child[rightNode->KeysNum] = rightNode->child[rightNode->KeysNum + 1];
			rightNode->child[rightNode->KeysNum + 1] = nullptr;
			//�������� ����� ������� ����� �� 1 ��� �����
			for (int i = 0; i < rightNode->KeysNum; i++)
				rightNode->keys[i] = rightNode->keys[i + 1];

			rightNode->keys[rightNode->KeysNum] = "";
			//� �������� ������ ����
			parent->keys[rightSibling - 1] = rightNode->keys[0];

			std::cout << "\n ����� " << x << " ������� " << std::endl;
			return;
		}
	}

	//���� �� ���������� ������� ���� �� ������, ����� ���������� ������� ���� � ��������

	//���������, ���������� �� ����� �����
	if (leftSibling >= 0) {

		Node* leftNode = parent->child[leftSibling];

		//���������  ����� �� �������� ����� � ������ ������
		for (int i = leftNode->KeysNum, j = 0; j < current->KeysNum; i++, j++)
			leftNode->keys[i] = current->keys[j];

		leftNode->child[leftNode->KeysNum] = nullptr;
		leftNode->KeysNum += current->KeysNum;
		//������ ����� ���� �������� ��������� �������� ����
		leftNode->child[leftNode->KeysNum] = current->child[current->KeysNum];

		//������� ���� � ��������� �� ��������
		removeInternal(parent->keys[leftSibling], parent, current);
		
	}
	//���������, ���������� �� ������ �����
	else if (rightSibling <= parent->KeysNum) {
		Node* rightNode = parent->child[rightSibling];

		//��������� ����� �� ������� ������ � ������� ����
		for (int i = current->KeysNum, j = 0; j < rightNode->KeysNum; i++, j++)
			current->keys[i] = rightNode->keys[j];

		current->child[current->KeysNum] = nullptr;
		current->KeysNum += rightNode->KeysNum;
		//������ ������� ���� �������� ��������� ������� ������
		current->child[current->KeysNum] = rightNode->child[rightNode->KeysNum];

		//������� ���� � ��������� �� ��������
		removeInternal(parent->keys[rightSibling - 1], parent, rightNode);
	
	}

	std::cout << "\n ����� " << x << " ������� " << std::endl;

}

void BPTree::removeInternal(std::string x, Node* current, Node* �hild)
{

	if (current == root) {
		//���� ���� ������� ����� ������� - ���� �� ���� � �����, ������� ������ ������
		//������ ����� ����� �� ������ 
		if (current->KeysNum == 1) {
			if (current->child[1] == �hild) {
				delete[] �hild->keys;
				delete[] �hild->child;
				delete �hild;
				root = current->child[0];
				delete[] current->keys;
				delete[] current->child;
				delete current;
				return;
			}
			else if (current->child[0] == �hild) {
				delete[] �hild->keys;
				delete[] �hild->child;
				delete �hild;
				root = current->child[1];
				delete[] current->keys;
				delete[] current->child;
				delete current;
				return;
			}
		}
	}

	//������� ������ �����, ������� ����� �������
	int index;
	for (index = 0; index < current->KeysNum; index++) {
		if (current->keys[index] == x)
			break;
	}

	current->KeysNum--;

	//������� ����
	for (int i = index; i < current->KeysNum; i++)
		current->keys[i] = current->keys[i + 1];


	current->keys[current->KeysNum] = "";

	//������� ������ ���������, ������� ����� �������
	for (index = 0; index < current->KeysNum + 2; index++) {
		if (current->child[index] == �hild)
			break;

	}

	//������� ���������
	for (int i = index; i < current->KeysNum + 1; i++)
		current->child[i] = current->child[i + 1];

	current->child[current->KeysNum + 1] = nullptr;

	//���� ������ ������ ��������, �������������� �������� �� ���������
	if (current->KeysNum >= (MaxKeysNum + 1) / 2 - 1)
		return;

	//���� �� � �����, �������
	if (current == root)
		return;

	Node* parent = findParent(root, current);
	int leftSibling = -1, rightSibling = 100;
	//���� ������ � ������� ������
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

	// ���� ���� �� ��������, ���������� ������� �������� ����
	if (!node->leaf) {
		for (int i = 0; i <= node->KeysNum; ++i)
			clearNode(node->child[i]);
	}

	// ����������� ������, ������� ������� �����
	delete node;

}

void BPTree::clear() {

	clearNode(root);
	root = nullptr; // ������������� ������ � nullptr ����� �������
	std::cout << "������� ������!" << std::endl;

}


