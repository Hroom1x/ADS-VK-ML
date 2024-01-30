// 4.2 АВЛ-деревья
// Дано число N и N строк. Каждая строка содержащий команду добавления или удаления натуральных чисел,
// а также запрос на получение k - ой порядковой статистики. Команда добавления числа A задается положительным
// числом A, команда удаления числа A задается отрицательным числом "- A". Запрос на получение k - ой порядковой
// статистики задается числом k. Требуемая скорость выполнения запроса - O(log n).


#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>


using std::vector;

template<class T, class Compare>
class BinaryTree {
private:
    struct Node {
        Node* _left;
        Node* _right;
        T _key;
        int _height;
        int _count;

        explicit Node(const T& key) :
            _left(nullptr), _right(nullptr), _key(key), _height(1), _count(1) { }

        int bfactor()
        {
            int hl = _left != nullptr ? _left->_height : 0;
            int hr = _right != nullptr ? _right->_height : 0;
            return hr - hl;
        }
        void fixHeightCount()
        {
            int hl = (_left != nullptr ? _left->_height : 0);
            int hr = (_right != nullptr ? _right->_height : 0);
            _height = (hl > hr ? hl : hr) + 1;

            int cl = (_left != nullptr ? _left->_count : 0);
            int cr = (_right != nullptr ? _right->_count : 0);
            _count = cl + cr + 1;
        }
        Node* rotateRight()
        {
            Node* q = this->_left;
            this->_left = q->_right;
            q->_right = this;
            this->fixHeightCount();
            q->fixHeightCount();
            return q;
        }
        Node* rotateLeft()
        {
            Node* p = this->_right;
            this->_right = p->_left;
            p->_left = this;
            this->fixHeightCount();
            p->fixHeightCount();
            return p;
        }
    };
    Node* _root;
    Compare _cmp;

    Node* insert(Node* node, const T& key);
    Node* remove(Node* node, const T& key);
    int findKStat(const Node* node, const int& k) const;
    Node* removeMin(Node* node, Node*& min);

    Node* balance(Node* node);

    void del_node(Node* node);
    void copy_node(const Node* other_node, Node*& node);

public:
    explicit BinaryTree(const Compare& cmp) : _root(nullptr), _cmp(cmp) { }
    BinaryTree(const BinaryTree& other);
    ~BinaryTree();

    BinaryTree& operator=(const BinaryTree& other)
    {
        BinaryTree tmp(other);
        swap(*this, tmp);
        return *this;
    }

    void add(const T& key);
    void del(const T& key);
    int find(const int& k) const;
};

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree()
{
    if (_root == nullptr) {
        return;
    }
    del_node(_root);
}

template<class T, class Compare>
typename BinaryTree<T, Compare>::Node* BinaryTree<T, Compare>::insert(Node* node, const T& key)
{
    if (node == nullptr)
        return new Node(key);
    if (_cmp(key, node->_key))
        node->_left = insert(node->_left, key);
    else
        node->_right = insert(node->_right, key);
    if (node == _root) {
        _root = balance(node);
        return _root;
    }
    return balance(node);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::add(const T& key) {
    if (_root == nullptr)
        _root = new Node(key);
    else
        this->insert(_root, key);
}

template<class T, class Compare>
typename BinaryTree<T, Compare>::Node* BinaryTree<T, Compare>::removeMin(
    BinaryTree::Node* node, BinaryTree::Node*& min)
{
    if (node->_left == nullptr) {
        min = node;
        return node->_right;
    }
    node->_left = removeMin(node->_left, min);
    return balance(node);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::del(const T& key) {
    if (_root != nullptr)
        remove(_root, key);
}

template<class T, class Compare>
typename BinaryTree<T, Compare>::Node* BinaryTree<T, Compare>::remove(Node* node, const T& key)
{
    if (node == nullptr)
        return nullptr;
    if (_cmp(key, node->_key)) {
        node->_left = remove(node->_left, key);
    }
    else if (_cmp(node->_key, key)) {
        node->_right = remove(node->_right, key);
    }
    else
    {
        Node* left = node->_left;
        Node* right = node->_right;
        if (right == nullptr) {
            delete node;
            return left;
        }

        Node* min = nullptr;
        Node* tmp = removeMin(right, min);
        min->_right = (tmp != min ? tmp : nullptr);

        min->_left = left;
        if (node == _root)
            _root = min;
        delete node;
        return balance(min);
    }
    Node* tmp = balance(node);
    if (node == _root)
        _root = tmp;
    return tmp;
}

template<class T, class Compare>
int BinaryTree<T, Compare>::find(const int& k) const {
    return findKStat(_root, k);
}

template<class T, class Compare>
int BinaryTree<T, Compare>::findKStat(const BinaryTree::Node* node, const int& k) const
{
    int left = (node->_left != nullptr ? node->_left->_count : 0);
    if (k < left) {
        return findKStat(node->_left, k);
    }
    else if (k > left) {
        return findKStat(node->_right, k - left - 1);
    }
    else {
        return node->_key;
    }
}

template<class T, class Compare>
typename BinaryTree<T, Compare>::Node* BinaryTree<T, Compare>::balance(BinaryTree::Node* node)
{
    node->fixHeightCount();
    if (node->bfactor() == 2)
    {
        if (node->_right->bfactor() < 0)
            node->_right = node->_right->rotateRight();
        return node->rotateLeft();
    }
    if (node->bfactor() == -2)
    {
        if (node->_left->bfactor() > 0)
            node->_left = node->_left->rotateLeft();
        return node->rotateRight();
    }
    return node;
}

template<class T, class Compare>
BinaryTree<T, Compare>::BinaryTree(const BinaryTree& other) : _root(nullptr), _cmp(other._cmp)
{
    copy_node(other._root, _root);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::del_node(BinaryTree::Node* node)
{
    if (node != nullptr) {
        del_node(node->_left);
        del_node(node->_right);
        delete node;
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::copy_node(const BinaryTree::Node* other_node, BinaryTree::Node*& node)
{
    if (other_node != nullptr) {
        node = new Node(other_node->_key);
        copy_node(other_node->_left, node->_left);
        copy_node(other_node->_right, node->_right);
        node->_key = other_node->_key;
    }
}

template <class T>
struct isLessDefault {
    bool operator()(const T& l, const T& r) const {
        return l < r;
    }
};

int run(std::istream& input, std::ostream& output)
{
    isLessDefault<int> cmp;
    BinaryTree<int, isLessDefault<int>> tree(cmp);
    int n = 0;
    int a = 0;
    int k = 0;
    input >> n;
    for (int i = 0; i < n; ++i) {
        input >> a;
        input >> k;
        a < 0 ? tree.del(-a) : tree.add(a);
        output << tree.find(k) << "\n";
    }

    return 0;
}

int main()
{
    /*{
        std::stringstream input;
        std::stringstream output;
        input << "3 1 0 2 0 -1 0";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "1\n1\n2\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 40 0 10 1 4 1 -10 0 50 2";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "40\n"
                               "40\n"
                               "10\n"
                               "4\n"
                               "50\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "9 20 0 10 0 50 0 6 0 19 0 40 0 60 0 70 0 -40 0";
        run(input, output);
        std::cout << output.str() << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "20 0 0 1 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 0 10 0 11 0 12 0 13 0 14 0 15 0 16 0 17 0 18 0 19 0";
        run(input, output);
        std::cout << output.str() << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "18 0 0 1 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 0 10 0 11 0 12 0 13 0 14 0 15 0 16 0 -13 0";
        run(input, output);
        std::cout << output.str() << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 1 0 2 0 3 0 -1 0";
        run(input, output);
        std::cout << output.str() << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "11 1 0 2 1 3 2 4 3 5 4 6 5 7 6 8 7 9 8 7 9 -5 8 -6 7";
        run(input, output);
        std::cout << output.str() << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "9 13 0 10 0 15 0 9 0 14 0 16 0 13 0 17 0 -9 0";
        run(input, output);
        std::cout << output.str() << std::endl;
    }*/

    run(std::cin, std::cout);
    //_CrtDumpMemoryLeaks();
    return 0;
}
