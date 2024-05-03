#include <iostream>

typedef unsigned long long INT_64;

INT_64 LineRand() {
	return rand() << 16 | rand();
}


using namespace std;

 
template <class K, class T>
class Tree
{
private:
	class Node {
	public:
		Node(K key, T data) {
			this->key = key;
			this->data = data;
			this->left = this->right = nullptr;
		}

		K key;
		T data;
		Node* left;
		Node* right;
	};

	Node* root;
	int size, count;

	Node* Successor(Node* x);
	Node* Predecessor(Node* x);
	Node* LParent(Node* t, Node* x);
	Node* RParent(Node* t, Node* x);


public:
	Tree();
	Tree(const Tree<K, T>& other);
	~Tree();


	int GetSize() { // 1
		return size;
	}


	void _clearNode(Node* current) {
		if (current == nullptr) {
			return;
		}

		if (current->left != nullptr) {
			_clearNode(current->left);
			current->left = nullptr;
		}

		if (current->right != nullptr) {
			_clearNode(current->right);
			current->right = nullptr;
		}

		delete current;
		size = 0;
	};

	void Clear() // 2
	{
		_clearNode(root);
		root = nullptr;
	}


	bool IsEmpty() { //3
		if (this->size != 0) {
			return false;
		}
		else {
			return true;
		}
	}



	Node* _insertNode(Node* current, K key, T data, bool* inserted) {
		count++;
		if (current == nullptr) { // если добавление в корень дерева
			*inserted = true;
			root = new Node(key, data);
			return root;
		}

		if (key == current->key) { // если такой узел уже существует то не вставляем
			*inserted = false;
			return nullptr;
		}

		if (key < current->key && current->left == nullptr) { // если мы должны вставить влево и есть свободное место то вставляем влево
			*inserted = true;
			current->left = new Node(key, data);
			return current->left;
		}
		else if (key < current->key && current->left != nullptr) {
			return _insertNode(current->left, key, data, inserted); // иначе идем влево
		}

		if (key > current->key && current->right == nullptr) { // если мы должны вставить вправо и есть свободное место то вставляем вправо
			*inserted = true;
			current->right = new Node(key, data);
			return current->right;
		}
		else if (key > current->key && current->right != nullptr) {
			return _insertNode(current->right, key, data, inserted); // иначе идем вправо
		}

		return nullptr;
	}

	bool Insert(K key, T data) { // 5
		bool inserted = false;
		_insertNode(root, key, data, &inserted);
		if (inserted) size++; // пересчитываем длину дерева
		return inserted;
	};


	Node* _deleteNode(Node* current, Node* targetParent, K key, bool* deleted) {
		if (current == nullptr) { // если корня нет или если не нашли удаляемого узла то и нечего удалять
			*deleted = false;
			return nullptr;
		}

		count++;

		if (key < current->key) { // если ключ меньше идем влево
			_deleteNode(current->left, current, key, deleted);
			return current;
		}

		if (key > current->key) { // если ключ меньше идем вправо
			_deleteNode(current->right, current, key, deleted);
			return current;
		}

		
		if (*deleted == true) return current; 
		*deleted = true; 

		if (current->left == nullptr && current->right == nullptr) {
			if (targetParent == nullptr) root = nullptr; 
			else {
				if (targetParent->left == current) targetParent->left = nullptr;
				if (targetParent->right == current) targetParent->right = nullptr;
			}
			delete current;
			return nullptr;
		}
		if (current->left == nullptr && current->right != nullptr) {
			Node* x = current->right;
			if (targetParent == nullptr) root = current->right; // если нет родителя то считается что удаляемый - корень
			else {
				if (targetParent->left == current) targetParent->left = current->right; // удаляем либо слева либо справа от его родителя в зависимости где он
				if (targetParent->right == current) targetParent->right = current->right;
			}
			return x;
		}
		if (current->right == nullptr && current->left != nullptr) {
			Node* x = current->left;
			if (targetParent == nullptr) root = current->left; // если нет родителя то считается что удаляемый - корень
			else {
				if (targetParent->left == current) targetParent->left = current->left;
				if (targetParent->right == current) targetParent->right = current->left;
			}
			return x;
		}
		current->right = _delete(current->right, current); // рекурсивная функция по удалению если у удаляемого есть оба потомка
		return current;
	}

	Node* _delete(Node* targetReplaceWith, Node* current) {
		count++;
		if (targetReplaceWith->left != nullptr) { // идем влево до упора
			targetReplaceWith->left = _delete(targetReplaceWith->left, current);
			return targetReplaceWith;
		} // заменяем данные у удаляемого (переписываем его)
		current->key = targetReplaceWith->key;
		current->data = targetReplaceWith->data;
		Node* x = targetReplaceWith->right;
		delete targetReplaceWith; // удаляем найденный на замену узел
		return x;
	}



	bool Delete(K key) { // 6
		bool deleted = false;
		_deleteNode(root, nullptr, key, &deleted);
		if (deleted) size--;; // пересчитываем длину дерева
		return deleted;
	}


	Node* _seachByKey(K key, Node* current) {
		count++;
		if (current == nullptr) {
			throw exception();
		}

		if (current->key == key) {
			return current;
		}

		if (key < current->key) {//если ключ меньше
			return _seachByKey(key, current->left);//идем влево
		}
		else {
			return _seachByKey(key, current->right);//иначе идем вправо
		}
	}

	T& SearchByKey(K key) { // доступ к ячейкам данных по ключу
		if (root == nullptr) {
			throw exception();
		}
		Node* current = _seachByKey(key, root);
		if (current == nullptr) {
			throw exception();
		}

		return current->data; // возвращает данные по ключу
	}


	void _printNode(Node* current, int level) { //вспомогательная функция для вывода структуры
		if (current == nullptr) {
			return;
		}
		else {
			_printNode(current->right, level + 1);

			for (int i = 0; i < (level * 3); i++) {
				cout << " ";
			}

			cout << current->key << endl;

			_printNode(current->left, level + 1);
		}
	}

	void Print() {
		if (root != nullptr) {
			_printNode(root, 0);
		}
	}

	int _getHeight(Node* current) {
		int left, right;

		if (current != nullptr) {
			left = _getHeight(current->left);
			right = _getHeight(current->right);
			return ((left > right) ? left : right) + 1;
		}
		else {
			return 0;
		}
	}

	int GetHeight() {
		return _getHeight(root);
	}

	int GetCount() {
		int temp = this->count;
		this->count = 0;
		return temp;
	}



	class Iterator
	{
	public:
		Node* current;
		Tree* tree;
		bool isBlock = false;

		bool GetStatus() {
			return isBlock;
		}

		Iterator() {
			tree = nullptr;
			current = nullptr;
		}

		T& operator*() {
			if (current == nullptr) {
				isBlock = true;
				throw exception();
			}
			return current->data;
		}

		void operator++(int) {
			if (current == nullptr) {
				return;
			}
			current = tree->Successor(current);
		}

		void operator--(int) {
			if (current == nullptr) {
				return;
				return;
			}
			current = tree->Predecessor(current);
		}

		bool operator==(const Iterator& other) { 
			return current == other.current && isBlock == other.isBlock; 
		}
		bool operator!=(const Iterator& other) { 
			return current != other.current; 
		}

	};
	friend class Iterator;


	class reverseIterator
	{
	public:
		Node* current;
		Tree* tree;
		bool isBlock = false;

		bool GetStatus() {
			return isBlock;
		}

		reverseIterator() {
			tree = nullptr;
			current = nullptr;
		}

		T& operator*() {
			if (current == nullptr) {
				isBlock = true;
				throw exception();
			}
			return current->data;
		}

		void operator--(int) {
			if (current == nullptr) {
				return;
			}
			current = tree->Successor(current);
		}

		void operator++(int) {
			if (current == nullptr) {
				return;
			}
			current = tree->Predecessor(current);
		}

		bool operator==(const reverseIterator& other) {
			return current == other.current && isBlock == other.isBlock;
		}
		bool operator!=(const reverseIterator& other) {
			return current != other.current;
		}

	};
	friend class reverseIterator;


	Iterator begin() {
		Iterator iterator;
		iterator.tree = this;
		Node* current = root;

		if (current != nullptr) {
			while (current->left != nullptr) {
				current = current->left;
			}
		}
	
		iterator.current = current;
		return iterator;
	}

	reverseIterator rbegin() {
		reverseIterator iterator;
		iterator.tree = this;
		Node* current = root;

		if (current != nullptr) {
			while (current->right != nullptr) {
				current = current->right;
			}
		}

		iterator.current = current;
		return iterator;
	}

	Iterator end() {
		Iterator iterator;
		iterator.tree = this;
		Node* current = nullptr;

		iterator.current = nullptr;
		return iterator;

	}

	reverseIterator rend() {
		reverseIterator iterator;
		iterator.tree = this;

		iterator.current = nullptr;
		return iterator;
	}

	void PrintTLR() {
		if (root != nullptr) {
			_printTLR(root);
		}
	}
	void _printTLR(Node* current) {
		if (current == nullptr) {
			return;
		}
		cout << current->key << " ";
		if (current->left != nullptr) {
			_printTLR(current->left);
		}
		if (current->right != nullptr) {
			_printTLR(current->right);
		}
	}

	void HelpCopy(Node *current) {
		if (current == nullptr) {
			return;
		}

		Insert(current->key, current->data);
		HelpCopy(current->left);
		HelpCopy(current->right);
	}
	

};


template <class K, class T> Tree<K, T>::Tree() {
	size = 0, count = 0;
	root = nullptr;
}

template <class K, class T> Tree<K, T>::Tree(const Tree<K, T>& other) {
	size = 0;
	root = nullptr;

	HelpCopy(other.root);

}

template <class K, class T> Tree<K, T>::~Tree() {
	Clear();
}

template<class K, class T>
typename Tree<K, T>::Node* Tree<K, T>::Successor(typename Tree<K, T>::Node* x)
{
	if (x == nullptr)
		return nullptr;
	if (x->right != nullptr) // если есть правый потомок то ищем минимальный узел с ключом большим текущего
	{
		Node* t = x->right;
		while (t->left != nullptr)
			t = t->left;
		return t;
	}
	else
		return LParent(root, x); // иначе возвращаем родительский узел
}

template<class K, class T>
typename Tree<K, T>::Node* Tree<K, T>::Predecessor(typename Tree<K, T>::Node* x)
{
	if (x == nullptr)
		return nullptr;
	if (x->left != nullptr) // если есть левый потомок то ищем максимальный узел с ключом меньшим текущего
	{
		Node* t = x->left;
		while (t->right != nullptr)
			t = t->right;
		return t;
	}
	else
		return RParent(root, x);// иначе возвращаем родительский узел
}

template<class K, class T>
typename Tree<K, T>::Node* Tree<K, T>::LParent(typename Tree<K, T>::Node* t, typename Tree<K, T>::Node* x)
{
	if (t == x)
		return nullptr;
	if (x->key < t->key) {
		Node* rp = LParent(t->left, x);
		if (rp != nullptr)
			return rp;
		else
			return t;
	}
	else
		return LParent(t->right, x);
}

template<class K, class T>
typename Tree<K, T>::Node* Tree<K, T>::RParent(typename Tree<K, T>::Node* t, typename Tree<K, T>::Node* x)
{
	if (t == x)
		return nullptr;
	if (x->key > t->key) {
		Node* rp = RParent(t->right, x);
		if (rp != nullptr)
			return rp;
		else
			return t;
	}
	else
		return RParent(t->left, x);
}


void test_rand(int n)
{
	//создание дерева для 64 – разрядных ключей типа INT_64
	Tree< INT_64, int > tree;
	//массив для ключей, которые присутствуют в дереве
	INT_64* m = new INT_64[n];
	
	//заполнение дерева и массива элементами
	//со случайными ключами
	tree.GetCount();
	for (int i = 0; i < n; i++)
	{
		m[i] = LineRand();
		tree.Insert(m[i], 1);
	}
	//вывод размера дерева до теста
	cout << "items count:" << tree.GetSize() << endl;
	//обнуление счётчиков трудоёмкости вставки,
	//удаления и поиска
	double I = 0;
	double D = 0;
	double S = 0;

	tree.GetCount();
	//генерация потока операций, 10% - промахи операций
	for (int i = 0; i < n / 2; i++)
		if (i % 10 == 0) //10% промахов
		{
			tree.Delete(LineRand());
			D += tree.GetCount();
			tree.Insert(m[rand() % n], 1);
			I += tree.GetCount();
			try {
				tree.SearchByKey(LineRand());
				S += tree.GetCount();
			}
			//обработка исключения при ошибке операции поиска
			catch (exception) { S += tree.GetCount(); }
		}
		else //90% успешных операций
		{
			int ind = rand() % n;
			tree.Delete(m[ind]);
			D += tree.GetCount();
			INT_64 key = LineRand();
			tree.Insert(key, 1);
			I += tree.GetCount();
			m[ind] = key;
			try {
				tree.SearchByKey(m[rand() % n]);
				S += tree.GetCount();
			}
				//обработка исключения при ошибке операции поиска
				catch (exception) { S += tree.GetCount(); }
		} //конец теста
		//вывод результатов:
		//вывод размера дерева после теста
	cout << "items count:" << tree.GetSize() << endl;
	//теоретической оценки трудоёмкости операций BST
	cout << "1.39*log2(n)=" << 1.39 * (log((double)n) / log(2.0)) << endl;
	//экспериментальной оценки трудоёмкости вставки
	cout << "Count insert: " << I / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости удаления
	cout << "Count delete: " << D / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости поиска
	cout << "Count search: " << S / (n / 2) << endl;
	//освобождение памяти массива m[]
	delete[] m;
}

void test_ord(int n) {
	//создание дерева для 64 – разрядных ключей типа INT_64
	Tree< INT_64, int > tree;
	//массив для ключей, которые присутствуют в дереве
	INT_64* m = new INT_64[n];
	//заполнение дерева и массива элементами
	// с возрастающими чётными ключами
	//на интервале [0, 10000, 20000, ... ,10000*n]
	for (int i = 0; i < n; i++) {
		m[i] = i * 10000;
		tree.Insert(m[i], 1);
	}
	//вывод размера дерева до теста
	cout << "items count:" << tree.GetSize() << endl;
	//обнуление счётчиков трудоёмкости вставки,
	// удаления и поиска
	double I = 0;
	double D = 0;
	double S = 0;

	tree.GetCount();
	//генерация потока операций, 10% - промахи операций
	for (int i = 0; i < n / 2; i++)
	{
		if (i % 10 == 0) //10% промахов
		{
			int k = LineRand() % (10000 * n);
			k = k + !(k % 2); //случайный нечётный ключ
			tree.Delete(k);
			D += tree.GetCount();
			tree.Insert(m[rand() % n], 1);
			I += tree.GetCount();
			k = LineRand() % (10000 * n);
			k = k + !(k % 2); // случайный нечётный ключ
			try {
				tree.SearchByKey(k);
				S += tree.GetCount();
			}
			//обработка исключения при ошибке операции поиска
			catch (exception) { S += tree.GetCount(); }
		}
		else //90% успешных операций
		{
			int ind = rand() % n;
			tree.Delete(m[ind]);
			D += tree.GetCount();;
			int k = LineRand() % (10000 * n);
			k = k + k % 2; // случайный чётный ключ
			tree.Insert(k, 1);
			I += tree.GetCount();;
			m[ind] = k;
			try {
				tree.SearchByKey(m[rand() % n]);
				S += tree.GetCount();;
			}
			//обработка исключения при ошибке операции поиска
			catch (exception) { S += tree.GetCount(); }
		}
	}
	//вывод результатов:
	// вывод размера дерева после теста
	cout << "items count:" << tree.GetSize() << endl;
		//теоретической оценки трудоёмкости операций BST
	cout << "n/2 =" << n / 2 << endl;
	//экспериментальной оценки трудоёмкости вставки
	cout << "Count insert: " << I / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости удаления
	cout << "Count delete: " << D / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости поиска
	cout << "Count search: " << S / (n / 2) << endl;
	//освобождение памяти массива m[]
	delete[] m;
} //конец теста





int main() {
	setlocale(LC_ALL, "ru");
	int key = -1;

	Tree<int, int> newTree;
	Tree<int, int>::Iterator begin, end;
	Tree<int, int>::reverseIterator rbegin, rend;

	while (key != 123)
	{
		cout << endl << "Menu:" << endl
			<< "[1]  Size of Tree" << endl
			<< "[2]  Clear Tree" << endl
			<< "[3]  Is Tree empty?" << endl
			<< "[4]  Access by key" << endl
			<< "[5]  Insert" << endl
			<< "[6]  Delete" << endl
			<< "[7]  PrintTLR" << endl
			<< "[8]  Height" << endl
			<< "[9]  Install begin() iterator" << endl
			<< "[10] Install rbegin() reverse iterator" << endl
			<< "[11] Install end() iterator" << endl
			<< "[12] Install rend() reverse iterator" << endl
			<< "[13] Interator ++" << endl
			<< "[14] Iterator --" << endl
			<< "[15] reverse Iterator ++" << endl
			<< "[16] reverse Iterator --" << endl
			<< "[17] Request iterator status" << endl
			<< "[18] Request reverse iterator status" << endl
			<< "[19] Change value by iterator" << endl
			<< "[20] Change value by reverse_iterator" << endl
			<< "[21] Where iterator?" << endl
			<< "[22] Where reverse_iterator?" << endl
			<< "[23] Test rand" << endl
			<< "[24] Test ord" << endl
			<< "[25] begin == end?" << endl
			<< "[26] rbegin != rend?" << endl

			<< "Enter number: ";

		while (!(cin >> key)) {
			cin.clear();
			while (cin.get() != '\n');
			cout << "Incorrect input! \nEnter number: ";
		}

		try
		{
			switch (key)
			{

			case 1: {
				cout << "Result:" << newTree.GetSize() << endl;
				break;
			}

			case 2: {
				newTree.Clear();
				break;

			}
			case 3: {
				cout << "Result: " << newTree.IsEmpty() << endl;
				break;
			}

			case 4: {
				int key;

				cout << "Key: ";
				cin >> key;

				int& dataFromSearch = newTree.SearchByKey(key);
				cout << "Result: " << dataFromSearch << endl;

				char cmd;
				cout << "Change? (y/n): ";
				cin >> cmd;
				if (cmd == 'y')
				{
					cout << "Enter new data: ";
					int data;
					cin >> data;
					dataFromSearch = data;
				}
				break;
			}

			case 5: {
				int key, data;

				cout << "Key: ";
				cin >> key;

				cout << "Data: ";
				cin >> data;

				cout << "Result: " << newTree.Insert(key, data) << endl;
				break;
			}

			case 6: {
				int key;

				cout << "Key: ";
				cin >> key;

				cout << "Result: " << newTree.Delete(key) << endl;
				break;
			}

			case 7: {
				newTree.PrintTLR();
				cout << endl;
				break;
			}

			case 8: {
				cout << "Result: " << newTree.GetHeight() << endl;
				break;
			}

			case 9: {
				begin = newTree.begin();
				cout << "Iterator begin(): " << *begin << endl;
				break;
			}

			case 10: {
				rbegin = newTree.rbegin();
				cout << "Iterator begin(): " << *rbegin << endl;
				break;
			}

			case 11: {
				end = newTree.end();
				cout << "Iterator end(): " << *end << endl;
				break;
			}

			case 12: {
				rend = newTree.rend();
				cout << "Iterator end(): " << *rend << endl;
				break;
			}

			case 13: {
				begin++;
				cout << "Iterator begin()++: " << *begin << endl;
				break;
			}

			case 14: {
				begin--;
				cout << "Iterator begin()--: " << *begin << endl;
				break;
			}

			case 15: {
				rbegin++;
				cout << "reverseIterator rbegin()++: " << *rbegin << endl;
				break;
			}

			case 16: {
				rbegin--;
				cout << "reverseIterator rbegin()--: " << *rbegin << endl;
				break;
			}

			case 17: {
				cout << "Result: " << begin.GetStatus() << endl;
				break;
			}

			case 18: {
				cout << "Result: " << rbegin.GetStatus() << endl;
				break;
			}

			case 19: {
				cin >> *begin;
				break;
			}

			case 20: {
				cin >> *rbegin;
				break;
			}

			case 21: {
				cout << *begin << endl;
				break;
			}

			case 22: {
				cout << *rbegin << endl;
				break;
			}

			case 23: {
				cout << "Test rand: ";
				int test = 0;
				cin >> test;
				test_rand(test);
				break;
			}

			case 24: {
				cout << "Test ord: ";
				int test = 0;
				cin >> test;
				test_ord(test);
				break;
			}

			case 25: {
				cout << "Result: " << (begin == end) << endl;
				break;
			}

			case 26: {
				cout << "Result: " << (rbegin != rend) << endl;
				break;
			}


			case 99: {
				newTree.Print();
				break;
			}

			default:
				break;
			}
		}
		catch (const std::exception&)
		{
			cout << "Exeption." << endl;
		}
	}
	return 0;
}