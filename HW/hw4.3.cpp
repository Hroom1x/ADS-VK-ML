// 4.3 Планировщик процессов
// В операционной системе Technux есть планировщик процессов.
// Каждый процесс характеризуется :
// приоритетом P
// временем, которое он уже отработал t
// временем, которое необходимо для завершения работы процесса T
// Планировщик процессов выбирает процесс с минимальным значением P * (t + 1), выполняет его время P и кладет обратно в очередь процессов.
// Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
// Требуется посчитать кол - во переключений процессора.
// Формат входных данных : Сначала вводится кол - во процессов.После этого процессы в формате P T
// Формат выходных данных : Кол - во переключений процессора.

#include <cassert>
#include <iostream>
#include <sstream>


struct Process {
    // По условию приоритет не должен быть нулевым, т.к. он не сможет выполняться 0 УЕ.
    Process() : _P(0), _t(0), _T(0), _weight(0) {}
    Process(size_t P, size_t t, size_t T) : _P(P), _t(t), _T(T), _weight(P* (t + 1)) {}

    Process& operator = (const Process& other) {
        this->_weight = other._weight;
        this->_P = other._P;
        this->_t = other._t;
        this->_T = other._T;
        return *this;
    }

    void updateWeight() {
        _weight = _P * (_t + 1);
    }

    void execute() {
        _t += _P;
        updateWeight();
    }

    const bool isDone() {
        return _t >= _T;
    }

    inline bool operator == (const Process& other) { return _weight == other._weight; }

    std::size_t _P;        // Приоритет
    std::size_t _t;        // Отработанное время
    std::size_t _T;        // Необходимое время работы
    std::size_t _weight;
};

// Сравнение инвертировано, т.к. сначала выполняются процессы с меньшим приоритетом
bool isLessProcess(const Process& l, const Process& r) {
    return l._weight > r._weight;
}

template <class T>
bool isLessDefault(const T& l, const T& r) {
    return l < r;
}

template <class T, class Compare>
class Heap {
public:
    Heap(Compare cmp) : _buffer(0), _buffer_size(0), _tail(0), _isLess(cmp) { grow(); }
    Heap(const T* arr, std::size_t size, Compare cmp) : _buffer_size(size), _tail(size), _isLess(cmp) {
        _buffer = new T[size];
        for (std::size_t i = 0; i < size; ++i) {
            _buffer[i] = arr[i];
        }
        buildHeap();
    }
    Heap(const Heap& other) : _buffer_size(other._buffer_size), _tail(other._tail), _isLess(other._isLess) {
        _buffer = new T[other._buffer_size];
        std::copy(other._buffer, other._buffer + _tail, _buffer);
    }
    Heap& operator=(const Heap& other) {
        if (this == &other)
            return *this;
        this->_buffer_size = other._buffer_size;
        this->_tail = other._tail;
        this->_isLess = other._isLess;
        delete[] _buffer;
        _buffer = new T[other._buffer_size];
        std::copy(other._buffer, other._buffer + _tail, _buffer);
        return *this;
    }
    ~Heap() { delete[] _buffer; }

    void insert(T a);
    T popMax();
    const bool isEmpty() { return _tail == 0; }
    const std::size_t size() { return _tail; }

    const T& operator[](std::size_t n) { return _buffer[n]; }

private:
    void grow();
    void siftUp(std::size_t n);
    void siftDown(std::size_t n);
    void buildHeap();

    T* _buffer;
    std::size_t _buffer_size;
    std::size_t _tail;
    Compare _isLess;
};

template <class T, class Compare>
void Heap<T, Compare>::grow() {
    if (_buffer_size == 0) {
        _buffer_size = 2;
        _buffer = new T[2];
    }
    else {
    std:size_t new_buffer_size = _buffer_size * 2;
        T* temp = new T[new_buffer_size];
        for (std::size_t i = 0; i < _tail; ++i) {
            temp[i] = _buffer[i];
        }
        delete[] _buffer;
        _buffer = temp;
        _buffer_size = new_buffer_size;
    }
}

template <class T, class Compare>
void Heap<T, Compare>::insert(T a) {
    if (_tail == _buffer_size) {
        grow();
    }
    _buffer[_tail++] = a;
    siftUp(_tail - 1);
}

template <class T, class Compare>
T Heap<T, Compare>::popMax() {
    T temp = _buffer[0];
    _buffer[0] = _buffer[--_tail];
    siftDown(0);
    return temp;
}

template <class T, class Compare>
void Heap<T, Compare>::siftDown(std::size_t n) {
    if (_tail > 2 * n + 1) {
        if (_tail - 1 == 2 * n + 1) {
            if (_isLess(_buffer[n], _buffer[2 * n + 1])) {
                T temp = _buffer[n];
                _buffer[n] = _buffer[2 * n + 1];
                _buffer[2 * n + 1] = temp;
            }
        }
        else {
            if (_isLess(_buffer[n], _buffer[2 * n + 1]) && _isLess(_buffer[2 * n + 2], _buffer[2 * n + 1])) {
                T temp = _buffer[n];
                _buffer[n] = _buffer[2 * n + 1];
                _buffer[2 * n + 1] = temp;
                siftDown(2 * n + 1);
            }
            else if (_isLess(_buffer[n], _buffer[2 * n + 2])) {
                T temp = _buffer[n];
                _buffer[n] = _buffer[2 * n + 2];
                _buffer[2 * n + 2] = temp;
                siftDown(2 * n + 2);
            }
        }
    }
}

template <class T, class Compare>
void Heap<T, Compare>::siftUp(std::size_t n) {
    if (_tail > n - 1 && n != 0) {
        if (_isLess(_buffer[(n - 1) / 2], _buffer[n])) {
            T temp = _buffer[n];
            _buffer[n] = _buffer[(n - 1) / 2];
            _buffer[(n - 1) / 2] = temp;
            siftUp((n - 1) / 2);
        }
    }
}

template <class T, class Compare>
void Heap<T, Compare>::buildHeap() {
    for (int i = _tail / 2 - 1; i >= 0; --i) {
        siftDown(i);
    }
}

int run(std::istream& input, std::ostream& output) {
    std::size_t n = 0;
    input >> n;
    Process* process = new Process[n];
    for (std::size_t i = 0; i < n; ++i) {
        input >> process[i]._P >> process[i]._T;
        process[i].updateWeight();
        // heap.insert(process);
    }
    Heap<Process, decltype(isLessProcess)*> heap(process, n, isLessProcess);
    delete[] process;
    std::size_t counter = 0;
    Process temp;
    while (heap.size() > 0) {
        Process top = heap.popMax();
        top.execute();
        ++counter;
        if (!top.isDone()) {
            heap.insert(top);
        }
    }
    output << counter;
    return 0;
}

int main()
{
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 1 10 1 5 2 5";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "18");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "8 1 10 2 10 1 10 2 5 3 5 1 5 4 11 3 10";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "42");
    }

    run(std::cin, std::cout);
    return 0;
}
