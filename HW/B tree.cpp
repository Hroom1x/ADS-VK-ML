// 3 В-деревья
// Постройте B-дерево минимального порядка t и выведите его по слоям.
// В качестве ключа используются числа, лежащие в диапазоне 0..2^32 - 1


#include <cassert>
#include <iostream>
#include <sstream>
#include <queue>
#include <vector>

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

using std::vector;


template <class T, class Compare>
class BTree {
public:
    BTree(std::size_t t, Compare cmp) : _root(nullptr), _t(t), _cmp(cmp) { }
    ~BTree() { delete _root; }

    void insert(const T& key);
    void print(std::ostream& output) const;

private:
    struct Node {
        bool _is_leaf;
        vector<T> _keys;
        vector<Node*> _children;

        Node(bool is_leaf) : _is_leaf(is_leaf) { }
        ~Node() { for (Node* child : _children) { delete child; } }
    };

    Node* _root;
    size_t _t;
    Compare _cmp;

    bool isNodeFull(Node* node) const { return node->_keys.size() == 2 * _t - 1; }
    void insertNonFull(Node* node, const T& key);
    void splitChild(Node* node, size_t pos);
};

template<class T, class Compare>
void BTree<T, Compare>::insert(const T& key)
{
    if (_root == nullptr) {
        _root = new Node(true);
    }

    if (isNodeFull(_root)) {
        Node* new_root = new Node(false);
        new_root->_children.push_back(_root);
        _root = new_root;
        splitChild(_root, 0);
    }

    insertNonFull(_root, key);
}

template<class T, class Compare>
void BTree<T, Compare>::insertNonFull(BTree::Node* node, const T& key)
{
    if (node->_is_leaf) {
        node->_keys.resize(node->_keys.size() + 1);
        size_t pos = node->_keys.size() - 1;
        while (pos >= 0 && key < node->_keys[pos]) {
            node->_keys[pos + 1] = node->_keys[pos];
            --pos;
        }
        node->_keys[pos] = key;
    }
    else {
        size_t pos = node->_keys.size() - 1;
        while (pos >= 0 && key < node->_keys[pos]) {
            --pos;
        }
        if (isNodeFull(node->_children[pos + 1])) {
            splitChild(node, pos + 1);
            if (key > node->_keys[pos + 1]) {
                ++pos;
            }
        }
        insertNonFull(node->_children[pos + 1], key);
    }
}

template<class T, class Compare>
void BTree<T, Compare>::splitChild(BTree::Node* node, size_t pos)
{
    Node* Y = node->_children[pos];
    Node* Z = new Node(Y->_is_leaf);
    Z->_keys.resize(_t - 1);
    for (size_t j = 0; j < _t - 1; ++j) {
        Z->_keys[j] = Y->_keys[j];
    }
    if (!Y->_is_leaf) {
        Z->_children.resize(_t);
        for (size_t j = 0; j < _t; ++j) {
            Z->_children[j] = Y->_children[j];
        }
        Y->_children.erase(Y->_children.begin(), Y->_children.begin() + Z->_children.size());
    }
    node->_keys.resize(node->_keys.size() + 1);
    for (size_t j = node->_keys.size() - 1; j > pos; --j) {
        node->_keys[j] = node->_keys[j - 1];
    }
    node->_keys[pos] = Y->_keys[_t - 1];
    Y->_keys.erase(Y->_keys.begin(), Y->_keys.begin() + Z->_keys.size() + 1);
    for (size_t j = node->_keys.size() - 1; j > pos; --j) {
        node->_children[j + 1] = node->_children[j];
    }
    node->_children.resize(node->_children.size() + 1);
    node->_children[pos] = Z;
    node->_children[pos + 1] = Y;
}

template<class T, class Compare>
void BTree<T, Compare>::print(std::ostream& output) const
{
    std::queue<Node*> q;
    q.push(_root);

    while (!q.empty()) {
        int n = q.size();
        for (int i = 0; i < n; i++) {
            Node* current = q.front();
            q.pop();
            for (int j = 0; j < current->_keys.size(); ++j) {
                std::cout << current->_keys[j] << " ";
            }
            if (!current->_is_leaf) {
                for (int j = 0; j < current->_children.size(); ++j) {
                    q.push(current->_children[j]);
                }
            }
        }
        std::cout << std::endl;
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
    int t = 0;
    input >> t;
    BTree<size_t, isLessDefault<int>> tree(t, cmp);
    int a = 0;
    while (input >> a) {
        tree.insert(a);
    }
    /*for (int i =0; i< 10;++i) {
        input >> a;
        tree.insert(a);
    }*/
    tree.print(output);
    return 0;
}

int main()
{
    /*{
        std::stringstream input;
        std::stringstream output;
        input << "2\n"
                 "0 1 2 3 4 5 6 7 8 9";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "3 \n"
                               "1 5 7 \n"
                               "0 2 4 6 8 9 \n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n"
                 "0 1 2 3 4 5 6 7 8 9";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "3 \n"
                               "0 1 2 4 5 6 7 8 9 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "8\n"
                 "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25";
        run(input, output);
        std::cout << output.str() << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2\n"
                 "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25";
        run(input, output);
        std::cout << output.str() << std::endl;
    }*/

    run(std::cin, std::cout);
    //_CrtDumpMemoryLeaks();
    return 0;
}
