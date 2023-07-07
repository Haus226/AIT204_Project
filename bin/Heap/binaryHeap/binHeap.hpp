#pragma once
#include <algorithm>
#include <vector>

template <typename T, typename Cmp = std::less<T>>
class binHeap {
    private:
        std::vector<T> arr;
        Cmp cmp;
        void siftUp(long long begin, long long end);
        void siftDown(long long begin, long long end);
        void heapify();
    public:
        binHeap(const T& x) {
            arr.push_back(x);
        } 
        binHeap(const std::vector<T> &arr);
        binHeap(Cmp cmp=Cmp()) {};
        binHeap(const std::vector<T> &arr, Cmp cmp);
        std::vector<T> getArr() {return arr;}
        bool empty() {return !arr.size();}
        size_t size() {return arr.size();}
        void push(T x);
        T pop();
        void meld(binHeap &other);
        void update(long long idx, T val);
        void sort();
        void print();
};

#include "binHeap.tpp"