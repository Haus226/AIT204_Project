#include "binHeap.hpp"
#include <cassert>
#include <iostream>

template<typename T, typename Cmp>
binHeap<T, Cmp>::binHeap(const std::vector<T> &arr) : arr(arr) {
    heapify();  
}

template<typename T, typename Cmp>
binHeap<T, Cmp>::binHeap(const std::vector<T> &arr, Cmp cmp) : arr(arr), cmp(cmp) {
    heapify();  
}

template<typename T, typename Cmp>
void binHeap<T, Cmp>::siftUp(long long begin, long long end) {
    while ((begin - 1) / 2 >= end && cmp(arr[begin], arr[(begin - 1) / 2])) {
        std::swap(arr[begin], arr[(begin - 1) / 2]);
        begin = (begin - 1) / 2;
    }
}

template<typename T, typename Cmp>
void binHeap<T, Cmp>::siftDown(long long begin, long long end) {
    long long childIdx = 2 * begin + 1;
    while (childIdx < end) {
        if (childIdx + 1 < end && cmp(arr[childIdx + 1], arr[childIdx])) childIdx++;
        if (cmp(arr[begin], arr[childIdx])) break;
        std::swap(arr[begin], arr[childIdx]);
        begin = childIdx;
        childIdx = 2 * begin + 1;
    }
}

template <typename T, typename Cmp>
void binHeap<T, Cmp>::heapify() {
    for (long long idx = arr.size() - 1; idx >= 0; idx--) siftDown(idx, arr.size());
}

template <typename T, typename Cmp>
void binHeap<T, Cmp>::sort() {
    for (long long idx = arr.size() - 1; idx > 0; idx--) {
        std::swap(arr[0], arr[idx]);
        siftDown(0, idx);
    }
}

template <typename T, typename Cmp>
void binHeap<T, Cmp>::push(T x) {
    arr.push_back(x);
    siftUp(arr.size() - 1, 0);
}

template <typename T, typename Cmp>
T binHeap<T, Cmp>::pop() {
    assert(arr.size() && "The heap is empty...");
    T e = arr[0];
    std::swap(arr[0], arr[arr.size() - 1]);
    arr.pop_back();
    siftDown(0, arr.size());
    return e;
}

template <typename T, typename Cmp>
void binHeap<T, Cmp>::meld(binHeap &other) {
    for (T &e : other.getArr()) push(e);
}


template <typename T, typename Cmp>
void binHeap<T, Cmp>::update(long long idx, T val) {
    if (cmp(arr[idx], val)) {
        arr[idx] = val;
        siftDown(idx, arr.size());
    } else {
        arr[idx] = val;
        siftUp(idx, 0);
    }
}

template <typename T, typename Cmp>
void binHeap<T, Cmp>::print() {
    for (const T &e : arr) std::cout << e << " ";
    std::cout << std::endl;
}






