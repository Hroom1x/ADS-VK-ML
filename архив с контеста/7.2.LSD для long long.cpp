// 7.2 
// Дан массив неотрицательных целых 64 - битных чисел.Количество чисел не больше 1000000.
// Отсортировать массив методом поразрядной сортировки LSD по байтам.

#include <cassert>
#include <iostream>
#include <sstream>
#include <cstring>

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>


void countingSort(unsigned long long* a, int n, int byte) {
    if (n <= 1 || byte >= 8)
        return;
    // Отсчёт байтов идёт справа налево начиная с нуля
    // Т.е. 7...6...5...4...3...2...1...0...
    unsigned long long mask = 255;
    mask = mask << (byte * 8);

    int c[256];  // Массив для подсчёта байтов
    std::memset(c, 0, 256 * sizeof(int));
    for (std::size_t i = 0; i < n; ++i) {
        ++c[(a[i] & mask) >> (byte * 8)];
    }
    // Модифицируем массив для границ элементов
    for (std::size_t i = 1; i < 256; ++i) {
        c[i] += c[i - 1];
    }
    int c_old[256];  // Сохраняем количество элементов в группе
    std::memcpy(c_old, c, sizeof(int) * 256);

    unsigned long long* tmp = new unsigned long long[n];
    std::memset(tmp, 0, n * sizeof(unsigned long long));
    // Перебор исходного массива
    for (int i = n - 1; i >= 0; --i) {
        tmp[--c[(a[i] & mask) >> (byte * 8)]] = a[i];
    }
    std::memcpy(a, tmp, sizeof(unsigned long long) * n);
    delete[] tmp;
}

void LSDSort(unsigned long long* a, int n) {
    int segment_size = 0;
    countingSort(a, n, 0);
    for (int i = 1; i < 8; ++i) {
        countingSort(a, n, i);
    }
}

int run(std::istream& input, std::ostream& output) {
    int n = 0;
    input >> n;
    assert(n > 0);
    unsigned long long* a = new unsigned long long[n];
    for (std::size_t i = 0; i < n; ++i) {
        input >> a[i];
    }

    LSDSort(a, n);

    output << a[0];
    for (std::size_t i = 1; i < n; ++i) {
        output << " " << a[i];
    }

    delete[] a;
    return 0;
}

int main()
{
    /*{
        std::stringstream input;
        std::stringstream output;
        input << "20 194 109 62 80 245 61 153 156 119 205 54 208 156 204 185 58 162 58 124 206";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "54 58 58 61 62 80 109 119 124 153 156 156 162 185 194 204 205 206 208 245");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "20 15104 21248 27904 9728 2048 16896 29184 6144 32000 53504 50432 30720 1792 21504 64000 35072 27136 64512 28416 49408";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "1792 2048 6144 9728 15104 16896 21248 21504 27136 27904 28416 29184 30720 32000 35072 49408 50432 53504 64000 64512");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "8 3077 2561 3109 3073 257 769 3585 2817";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "257 769 2561 2817 3073 3077 3109 3585");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "20 8587 29070 60970 35100 19507 3638 58136 34605 28135 45671 56599 9240 55267 48270 30905 1198 36327 41499 45043 24425";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "1198 3638 8587 9240 19507 24425 28135 29070 30905 34605 35100 36327 41499 45043 45671 48270 55267 56599 58136 60970");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "3 4 1000000 7";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "4 7 1000000");
    }*/

    run(std::cin, std::cout);
    //_CrtDumpMemoryLeaks();
    return 0;
}
