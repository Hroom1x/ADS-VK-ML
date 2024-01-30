// 1.2
// Реализуйте структуру данных типа “множество строк” на основе динамической хеш - таблицы с открытой адресацией.
// Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш - функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
// Начальный размер таблицы должен быть равным 8 - ми.Перехеширование выполняйте при добавлении элементов в случае,
// когда коэффициент заполнения таблицы достигает 3 / 4.
// Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и
// проверки принадлежности данной строки множеству.
// Вариант: Для разрешения коллизий используйте двойное хеширование.

#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

using std::vector;


struct iteratorHasher {
    unsigned int operator()(const std::string& key) const {
        unsigned int hash = 0;
        for (unsigned int i = 0; i < key.size(); ++i) {
            hash += key[i];
        }
        return hash * 2 + 1;
    }
};

struct stringHasher {
    unsigned int operator()(const std::string& key) const {
        unsigned int hash = 0;
        for (unsigned int i = 0; i < key.size(); ++i) {
            hash = hash * 137 + key[i];
        }
        return hash * 2 + 1;
    }
};

template <class T, class H1, class H2>
class HashTable {
public:
    HashTable(int initial_size, const H1& string_hasher, const H2& iterator_hasher);
    ~HashTable();

    bool has(const T& key) const;
    bool add(const T& key);
    bool remove(const T& key);

private:
    H1 _hasher;
    H2 _iter_hasher;

    struct HashTableNode
    {
        T _key;
        enum State { Empty, Key, Deleted };
        HashTableNode::State _state;
        HashTableNode() : _state(Empty) { }
        explicit HashTableNode(const T& key) :
            _key(key), _state(Key) { }
    };
    vector<HashTableNode> _table;

    unsigned int _keys_count;

    void growTable();
};

template <class T, class H1, class H2>
HashTable<T, H1, H2>::HashTable(int initial_size, const H1& string_hasher, const H2& iterator_hasher) :
    _hasher(string_hasher),
    _iter_hasher(iterator_hasher),
    _table(initial_size),
    _keys_count(0)
{
    //
}

template <class T, class H1, class H2>
HashTable<T, H1, H2>::~HashTable()
{
    //
}

template <class T, class H1, class H2>
bool HashTable<T, H1, H2>::has(const T& key) const
{
    unsigned int hash = _hasher(key) % _table.size();
    unsigned int step = _iter_hasher(key) % _table.size();  // _table.size() = 2^k
    unsigned int i = 0;
    while (_table[hash]._state != HashTableNode::Empty) {
        if (_table[hash]._key == key && _table[hash]._state == HashTableNode::Key) {
            return true;
        }
        // step
        ++i;
        hash = (hash + i * step) % _table.size();
    }
    return false;
}

template <class T, class H1, class H2>
bool HashTable<T, H1, H2>::add(const T& key)
{
    if (_keys_count > _table.size() * 3 / 4) {
        growTable();
    }

    unsigned int hash = _hasher(key) % _table.size();
    unsigned int step = _iter_hasher(key) % _table.size();  // _table.size() = 2^k
    unsigned int first_deleted = 0;
    bool has_deleted = false;
    HashTableNode node = _table[hash];
    unsigned int i = 0;
    while (node._state != HashTableNode::Empty) {
        if (node._key == key && node._state == HashTableNode::Key) {
            return false;
        }
        if (node._state == HashTableNode::Deleted) {
            if (!has_deleted) {
                has_deleted = true;
                first_deleted = hash;
            }
        }
        // step
        ++i;
        hash = (hash + i * step) % _table.size();
        node = _table[hash];
    }

    HashTableNode new_node(key);
    if (has_deleted) {
        _table[first_deleted] = std::move(new_node);
    }
    else {
        _table[hash] = std::move(new_node);
    }
    ++_keys_count;
    return true;
}

template <class T, class H1, class H2>
bool HashTable<T, H1, H2>::remove(const T& key)
{
    unsigned int hash = _hasher(key) % _table.size();
    unsigned int step = _iter_hasher(key) % _table.size();  // _table.size() = 2^k
    unsigned int i = 0;
    while (_table[hash]._state != HashTableNode::Empty) {
        if (_table[hash]._state == HashTableNode::Key && _table[hash]._key == key) {
            --_keys_count;
            _table[hash]._state = HashTableNode::Deleted;
            return true;
        }
        // step
        ++i;
        hash = (hash + i * step) % _table.size();
    }

    return false;
}

template <class T, class H1, class H2>
void HashTable<T, H1, H2>::growTable()
{
    unsigned int new_size = _table.size() * 2;
    vector<HashTableNode> new_table(new_size);
    for (unsigned int i = 0; i < _table.size(); ++i) {
        HashTableNode node = _table[i];
        if (node._state == HashTableNode::Key) {
            unsigned int new_hash = _hasher(node._key) % new_size;
            unsigned int step = _iter_hasher(node._key) % new_size;
            unsigned int j = 0;
            while (new_table[new_hash]._state != HashTableNode::Empty) {
                ++j;
                new_hash = (new_hash + j * step) % new_size;
            }
            _table[i]._state = HashTableNode::Deleted;
            new_table[new_hash] = node;
        }
    }
    _table = std::move(new_table);
}

int run(std::istream& input, std::ostream& output)
{
    stringHasher h1;
    iteratorHasher h2;
    HashTable<std::string, stringHasher, iteratorHasher> table(8, h1, h2);
    char operation = 0;
    std::string word;
    while (input >> operation >> word) {
        switch (operation)
        {
        case '+':
            output << (table.add(word) ? "OK" : "FAIL") << std::endl;
            break;
        case '-':
            output << (table.remove(word) ? "OK" : "FAIL") << std::endl;
            break;
        case '?':
            output << (table.has(word) ? "OK" : "FAIL") << std::endl;
            break;
        default:
            break;
        }
    }
    return 0;
}

int main()
{
    /*{
        std::stringstream input;
        std::stringstream output;
        input << "+ hello + bye ? bye + bye - bye ? bye ? hello";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "OK\nOK\nOK\nFAIL\nOK\nFAIL\nOK\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "+0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +10 +11 +12 +13 +14 +15 +16 +17 +18 +19 +20 +21 +22 +23 +24 +25 +26 +27 +28 +29 +30 +31 +32 +33 +34 +35 +36 +37 +38 +39 +40 +41 +42 +43 +44 +45 +46 +47 +48 +49 +50 +51 +52 +53 +54 +55 +56 +57 +58 +59 +60 +61 +62 +63 +64 +65 +66 +67 +68 +69 +70 +71 +72 +73 +74 +75 +76 +77 +78 +79 +80 +81 +82 +83 +84 +85 +86 +87 +88 +89 +90 +91 +92 +93 +94 +95 +96 +97 +98 +99 +100 +101 +102 +103 +104 +105 +106 +107 +108 +109 +110 +111 +112 +113 +114 +115 +116 +117 +118 +119 ?119 ?0 -0 ?0 +0 +0";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "OK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nOK\nFAIL\nOK\nFAIL\n");
    }*/

    run(std::cin, std::cout);
    //_CrtDumpMemoryLeaks();
    return 0;
}