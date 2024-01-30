// 2.2
// Дано число N < 106 и последовательность целых чисел из[-231..231] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
// то узел K добавляется в правое поддерево root; иначе в левое поддерево root.

// Вариант 2.2: Выведите элементы в порядке pre-order (сверху вниз).


#include <cassert>
#include <iostream>
#include <vector>

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>


using std::vector;

template<class T, class Compare>
class BinaryTree {
public:
    BinaryTree(const Compare& cmp) : _root(nullptr), _cmp(cmp) { }
    BinaryTree(const BinaryTree& other);
    ~BinaryTree();

    BinaryTree& operator=(const BinaryTree& other)
    {
        BinaryTree tmp(other);
        swap(*this, tmp);
        return *this;
    }

    void add(const T& key);
    void dfsPreOrder(vector<T>& order);

private:
    struct Node {
        Node* _parent;
        Node* _left;
        Node* _right;
        T _key;
        explicit Node(const T& key) : _parent(nullptr), _left(nullptr), _right(nullptr), _key(key) { }
        Node(const T& key, Node* parent) :
            _parent(parent), _left(nullptr), _right(nullptr), _key(key) { }
    };
    Node* _root;
    Compare _cmp;
};

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree()
{
    if (_root == nullptr) {
        return;
    }
    Node* cur_node = _root;
    while (cur_node != nullptr) {
        if (cur_node->_left != nullptr) {
            // идём в left
            cur_node = cur_node->_left;
        }
        else if (cur_node->_right != nullptr) {
            // идём в right
            cur_node = cur_node->_right;
        }
        else {
            // идём в parent
            Node* temp = cur_node;
            cur_node = cur_node->_parent;
            if (cur_node == nullptr) {
                break;
            }
            if (_cmp(temp->_key, cur_node->_key)) {
                delete cur_node->_left;
                cur_node->_left = nullptr;
            }
            else {
                delete cur_node->_right;
                cur_node->_right = nullptr;
            }
        }
    }
    delete _root;
}

template<class T, class Compare>
void BinaryTree<T, Compare>::add(const T& key)
{
    if (_root == nullptr) {
        Node* new_node = new Node(key);
        _root = new_node;
    }
    else {
        Node* cur_node = _root;
        Node* prev_node = nullptr;
        while (cur_node != nullptr) {
            if (_cmp(key, cur_node->_key)) {
                if (cur_node->_left == nullptr) {
                    Node* tmp = new Node(key, prev_node);
                    cur_node->_left = tmp;
                    tmp->_parent = cur_node;
                    break;
                }
                else {
                    prev_node = cur_node;
                    cur_node = cur_node->_left;
                }
            }
            else {
                if (cur_node->_right == nullptr) {
                    Node* tmp = new Node(key, prev_node);
                    cur_node->_right = tmp;
                    tmp->_parent = cur_node;
                    break;
                }
                else {
                    prev_node = cur_node;
                    cur_node = cur_node->_right;
                }
            }
        }
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::dfsPreOrder(vector<T>& order) {
    if (_root == nullptr) {
        return;
    }
    else {
        order.push_back(_root->_key);
    }
    Node* cur_node = _root;
    while (cur_node != nullptr) {
        if (cur_node->_left != nullptr) {
            // идём в left
            cur_node = cur_node->_left;
            order.push_back(cur_node->_key);
        }
        else if (cur_node->_right != nullptr) {
            // идём в right
            cur_node = cur_node->_right;
            order.push_back(cur_node->_key);
        }
        else {
            // идём в parent
            Node* temp = cur_node;
            cur_node = cur_node->_parent;
            if (cur_node == nullptr) {
                break;
            }
            if (_cmp(temp->_key, cur_node->_key)) {
                delete cur_node->_left;
                cur_node->_left = nullptr;
            }
            else {
                delete cur_node->_right;
                cur_node->_right = nullptr;
            }
        }
    }
}

template<class T, class Compare>
BinaryTree<T, Compare>::BinaryTree(const BinaryTree& other) : _root(nullptr), _cmp(other._cmp)
{
    if (other._root == nullptr) {
        return;
    }
    Node* cur_node = other._root;
    Node* new_node = new Node(other._root->_key);
    _root = new_node;
    Node* tmp = nullptr;
    // Алгоритм проходит одновременно оба дерева, создавая недостающие ветви у нового дерева.
    while (cur_node != nullptr) {
        if (cur_node->_left != nullptr && new_node->_left == nullptr) {
            // идём в _left
            cur_node = cur_node->_left;
            new_node->_left = new Node(cur_node->_key);
            tmp = new_node;
            new_node = new_node->_left;
            new_node->_parent = tmp;
        }
        else if (cur_node->_right != nullptr && new_node->_right == nullptr) {
            // идём в _right
            cur_node = cur_node->_right;
            new_node->_right = new Node(cur_node->_key);
            tmp = new_node;
            new_node = new_node->_right;
            new_node->_parent = tmp;
        }
        else {
            // идём в parent
            cur_node = cur_node->_parent;
            new_node = new_node->_parent;
        }
    }
}

template<class T>
void print_order(const vector<T>& order, std::ostream& output) {
    for (std::size_t i = 0; i < order.size(); ++i) {
        output << order[i] << " ";
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
    input >> n;
    for (int i = 0; i < n; ++i) {
        input >> a;
        tree.add(a);
    }
    vector<int> order;
    tree.dfsPreOrder(order);
    print_order(order, output);
    return 0;
}

int main()
{
    /*{
        std::stringstream input;
        std::stringstream output;
        input << "10 2 5 6 4 7 8 9 3 1 10";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "2 1 5 4 3 6 7 8 9 10 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 5 9 4 3 7 2 6 10 1 8";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "5 4 3 2 1 9 7 6 8 10 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 1 5 10 2 4 3 8 9 7 6";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "1 5 2 4 3 10 8 7 6 9 ");
    }*/

    run(std::cin, std::cout);
    //_CrtDumpMemoryLeaks();
    return 0;
}