// 6.1
// Дано множество целых чисел из[0..10 ^ 9] размера n.
// Используя алгоритм поиска k - ой порядковой статистики, требуется найти следующие параметры множества :
// 1) 10 % перцентиль
// 2) медиана
// 3) 90 % перцентиль
//
// 6.1)  Реализуйте стратегию выбора опорного элемента “медиана трёх”.Функцию Partition реализуйте
// методом прохода двумя итераторами от начала массива к концу.

#include <cassert>
#include <iostream>
#include <sstream>

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>


template <class T>
bool isLessDefault(const T& l, const T& r) {
    return l < r;
}

void print(int* a, int n) {
    std::cout << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;
}

template <class T>
int findPivot(T* a, int n, bool (*isLess)(const T& l, const T& r) = isLessDefault) {
    if (n <= 2) {
        return 0;
    }
    if (isLess(a[n / 2], a[0])) {
        if (isLess(a[0], a[n - 1]))
            return 0;
        return isLess(a[n - 1], a[n / 2]) ? n / 2 : n - 1;
    }
    if (isLess(a[n / 2], a[n - 1]))
        return n / 2;
    return isLess(a[n - 1], a[0]) ? 0 : n - 1;
}

// Возвращает индекс, на который встанет пивот после разделения.
template <class T>
int partition(T* a, int n, bool (*isLess)(const T& l, const T& r) = isLessDefault) {
    if (n <= 1) {
        return 0;
    }

    int pivot = findPivot(a, n);
    std::swap(a[pivot], a[n - 1]);

    int i = 0;
    int j = 0;
    while (j < n - 1) {
        if (isLess(a[n - 1], a[j]))
            j++;
        else
            std::swap(a[i++], a[j++]);
    }
    std::swap(a[i], a[n - 1]);
    return i;
}

template <class T>
int findKStat(T* a, int n, const T& k, bool (*isLess)(const T& l, const T& r) = isLessDefault) {
    int pivot = partition(a, n, isLess);
    if (k == pivot)
        return pivot;
    // int left = 0;       // Левая граница
    // int right = n - 1;  // Правая граница
    if (isLess(k, pivot)) {
        return findKStat(a, pivot, k, isLess);
    }
    else {
        return findKStat(a + pivot + 1, n - pivot - 1, k - pivot - 1, isLess) + pivot + 1;
    }
    //while (pivot != k) {
    //    if (isLess(k, pivot)) {
    //        right = pivot - 1;  // Смещаем правую границу
    //        pivot = left + partition(a + left, right - left + 1, isLess);
    //    }
    //    else {
    //        left = pivot + 1;  // Смещаем левую границу
    //        pivot = left + partition(a + left, right - left + 1, isLess);
    //    }
    //}
    // return pivot;
}

int run(std::istream& input, std::ostream& output) {
    int n = 0;
    input >> n;
    assert(n >= 3);
    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
        input >> arr[i];
    }

    int percentile_10 = arr[findKStat(arr, n, (n % 10 == 0 ? n / 10 : n / 10 + 1))];
    int median = arr[findKStat(arr, n, (n % 2 == 0 ? n / 2 : n / 2 + 1))];
    int percentile_90 = arr[findKStat(arr, n, (n % 10 == 0 ? n - n / 10 : n - n / 10 - 1))];

    output << percentile_10 << std::endl << median << std::endl << percentile_90;
    delete[] arr;
    return 0;
}

int main()
{
    /*{
        std::stringstream input;
        std::stringstream output;
        input << "10 0 1 1 1 1 0 1 1 1 1";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "0\n1\n1");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "9 0 1 1 1 1 0 0 0 0";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "0\n1\n1");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "10 1 1 0 0 0 1 0 1 0 0";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "0\n0\n1");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "8 1 0 0 1 1 0 1 0";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "0\n1\n1");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "10 0 0 0 0 0 0 0 0 0 1";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "0\n0\n1");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "10 1 2 3 4 5 6 7 8 9 10 ";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "2\n6\n10");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "10 10 9 8 7 6 5 4 3 2 1";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "2\n6\n10");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "4 1 2 3 4";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "2\n3\n4");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "3 1 2 3";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "2\n3\n3");
    }

    {
        std::stringstream input;
        std::stringstream output;
        input << "1000 990 273 361 888 461 797 476 623 147 257 57 619 53 492 789 629 148 426 213 398 345 32 878 123 451 836 111 706 117 17 580 951 977 238 996 77 581 124 604 196 8 559 994 102 459 284 429 625 490 812 5 732 831 696 1000 704 411 7 278 733 103 937 224 382 540 714 921 632 322 197 364 703 602 199 939 186 522 601 991 371 247 403 349 889 300 176 693 353 721 729 971 583 3 285 690 646 775 727 660 319 379 228 956 777 23 318 588 923 988 500 463 828 943 22 534 575 542 510 430 841 759 749 865 49 162 191 177 208 938 792 126 301 316 190 869 989 161 501 650 585 16 517 636 173 229 924 645 811 479 790 314 69 628 656 670 882 762 268 279 12 925 201 999 582 593 245 27 827 154 295 942 959 29 530 610 644 63 220 210 886 84 483 679 685 406 859 58 664 236 854 929 918 780 250 341 41 574 469 187 415 653 833 860 506 673 122 825 913 420 332 689 178 100 142 369 251 225 527 537 311 774 731 347 607 242 138 362 635 826 952 312 119 658 710 10 360 634 289 633 137 283 800 930 298 829 281 478 907 159 241 926 862 325 409 586 850 315 839 258 771 215 770 356 494 106 613 666 200 521 363 730 182 75 76 843 957 4 863 246 192 83 263 90 36 15 166 165 855 712 334 101 262 189 529 327 737 578 292 66 678 489 541 480 376 234 170 203 134 460 507 217 425 313 594 48 232 354 348 110 282 864 195 21 899 43 455 980 910 498 468 719 627 906 465 338 589 372 188 761 519 605 449 473 808 81 252 194 456 380 724 815 709 798 987 152 853 958 74 849 477 901 387 164 266 277 85 571 402 680 261 772 767 70 997 743 267 970 914 554 505 577 713 975 2 940 776 487 31 149 508 169 60 584 526 598 33 454 514 396 950 568 381 384 47 525 346 416 995 821 56 576 172 949 1 497 928 59 114 981 648 179 212 675 14 531 564 92 280 97 871 639 890 481 702 146 896 93 255 389 168 269 440 947 801 452 433 230 260 858 874 499 373 608 202 464 95 992 769 290 845 112 558 86 374 231 274 539 969 520 193 895 557 462 982 299 78 450 806 993 909 441 317 983 787 359 145 524 515 936 902 98 903 567 40 758 397 659 688 340 183 566 802 443 941 496 401 366 125 136 18 782 669 39 720 894 555 595 55 830 804 683 756 726 143 131 306 308 391 44 121 592 302 211 846 599 805 916 611 606 700 691 820 735 513 556 793 113 486 405 511 788 572 813 493 109 838 115 879 668 892 822 108 915 272 358 467 244 412 538 393 946 781 654 622 248 94 310 323 444 25 911 562 342 662 624 705 671 407 61 219 785 965 67 19 799 404 151 73 920 641 399 954 218 968 297 107 550 286 385 485 974 410 72 745 291 206 842 51 569 747 932 484 413 816 807 643 167 223 293 964 684 99 34 908 637 809 276 392 728 786 840 533 912 235 160 256 437 931 35 742 62 495 422 579 824 663 326 87 876 418 175 139 711 439 590 692 898 30 305 740 141 491 155 336 350 779 818 642 333 784 482 810 904 739 717 681 953 395 978 390 204 370 665 548 303 877 962 104 96 984 503 37 471 447 421 960 919 866 24 881 470 547 423 754 885 198 887 725 694 948 523 42 158 967 180 763 26 621 791 275 216 544 682 68 736 630 755 457 707 64 893 436 686 851 738 616 388 966 474 417 488 54 819 400 6 156 233 383 446 428 600 249 458 868 79 13 287 140 560 596 46 565 532 795 794 153 832 89 343 328 368 11 351 344 330 884 647 768 612 661 237 961 378 105 337 549 424 512 873 757 955 427 835 518 222 438 748 445 998 935 603 386 917 834 135 38 783 88 695 207 171 309 976 752 221 847 900 803 408 651 796 573 502 778 823 377 561 442 288 367 431 945 91 130 543 320 718 331 475 687 185 985 45 65 270 9 880 674 28 226 434 116 870 80 304 723 535 655 339 127 239 294 891 120 597 626 335 591 365 746 214 875 227 979 631 259 240 972 638 553 254 453 857 697 163 944 927 128 922 435 618 545 587 552 472 844 848 852 432 355 264 652 466 132 375 184 209 144 118 872 765 764 563 734 296 963 741 973 205 157 82 867 52 243 861 883 174 352 620 329 20 150 615 933 753 837 129 773 133 640 986 672 448 536 715 414 667 897 181 504 708 766 856 271 760 676 419 701 516 934 699 609 50 722 546 744 321 71 528 698 751 657 509 324 814 551 614 649 357 570 307 716 253 817 617 905 750 394 265 677";
        run(input, output);
        std::cout << output.str() << std::endl;
        assert(output.str() == "101\n501\n901");
    }*/

    run(std::cin, std::cout);
    //_CrtDumpMemoryLeaks();
    return 0;
}