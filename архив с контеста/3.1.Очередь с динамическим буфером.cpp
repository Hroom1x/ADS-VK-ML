// 3.1
// Реализовать очередь с динамическим зацикленным буфером.
// Обрабатывать команды push back и pop front.
// В первой строке количество команд n.n ≤ 1000000.
// Каждая команда задаётся как 2 целых числа : a b.
// a = 2 - pop front
// a = 3 - push back
// Если дана команда pop front, то число b - ожидаемое значение.
// Если команда pop front вызвана для пустой структуры данных, то ожидается " - 1".
// Требуется напечатать YES - если все ожидаемые значения совпали.Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

#include <cassert>
#include <iostream>
#include <sstream>
#include <cstring>

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>


class Queue {
public:
    Queue() : _buffer(0), _buffer_size(0), _real_size(0), _head(0), _tail(0) {}
    ~Queue() { delete[] _buffer; }
    void pushBack(int a);
    int popFront();
    bool isEmpty() const { return _real_size == 0; }


    int size() { return _buffer_size; }
    int* buf() { return _buffer; }

private:
    void grow();

    int* _buffer;
    std::size_t _buffer_size;
    std::size_t _real_size;
    std::size_t _head;
    std::size_t _tail;
};

void Queue::pushBack(int a) {
    if (_real_size == _buffer_size) {
        grow();
    }
    // Если последний элемент очереди не в конце буфера
    if (_tail != _buffer_size) {
        _buffer[_tail] = a;
        ++_tail;
    }
    else {  // Иначе переставляем указатель конца очереди в начало буфера
        _buffer[0] = a;
        _tail = 1;
    }
    ++_real_size;
}

int Queue::popFront() {
    if (isEmpty()) {
        return -1;
    }
    int res = _buffer[_head];
    // Если первый элемент очереди не в конце буфера
    if (_head != _buffer_size - 1) {
        ++_head;
    }
    else {  // Иначе переставляем указатель начала очереди в начало буфера
        _head = 0;
    }
    --_real_size;
    return res;
}

void Queue::grow() {
    if (_buffer_size == 0) {
        _buffer = new int[2];
        _buffer_size = 2;
    }
    else {
        int new_buffer_size = _buffer_size * 2;
        int* temp = new int[new_buffer_size];
        // Если начало очереди левее конца
        if (_head < _tail) {
            std::memcpy(temp, _buffer + _head, (_tail - _head) * sizeof(int));
        }
        else {
            std::memcpy(temp, _buffer + _head, (_buffer_size - _head) * sizeof(int));
            std::memcpy(temp + _buffer_size - _head, _buffer, _tail * sizeof(int));
        }
        _head = 0;
        _tail = _real_size;
        delete[] _buffer;
        _buffer = temp;
        _buffer_size = new_buffer_size;
    }
}

void run(std::istream& input, std::ostream& output) {
    Queue queue;
    int n = 0;
    input >> n;
    int a = 0;
    int b = 0;
    for (int i = 0; i < n; ++i) {
        input >> a >> b;
        assert(a == 2 || a == 3);
        if (a == 2) {
            if (b != queue.popFront()) {
                output << "NO";
                return;
            }
        }
        else if (a == 3) {
            queue.pushBack(b);
        }
    }
    output << "YES";
}

int main()
{
    /*{
        std::stringstream input;
        std::stringstream output;
        input << "14 2 -1 3 10 3 11 2 10 3 12 3 13 3 14 3 15 2 11 2 12 2 13 2 14 2 15 2 -1";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "YES");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "3 3 44 3 50 2 44";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "YES");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "2 2 -1 3 10";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "YES");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "2 3 44 2 66";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "NO");
    }*/

    run(std::cin, std::cout);
    //_CrtDumpMemoryLeaks();
    return 0;
}
