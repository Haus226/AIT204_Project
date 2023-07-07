#pragma once
#include <algorithm>
#include <cassert>
#include <vector>

// Comment the part involves parent pointer so that it can perform
// more recursions
template <typename T>
struct Node {
    T v;
    Node *child = nullptr, *sibling = nullptr;
    // Node *parent = nullptr;

    Node(const T &val) : v(val) {}
    // Node(T &&val) : v(std::move(val)) {}
};

template <typename T, typename Cmp=std::less<T>>
class pairHeap {

    using Node = Node<T>;

    private:

        Node *root_ = nullptr;
        size_t size_ = 0;
        Cmp cmp;

        void swap(Node *&x, Node *&y);

        Node* meld_(Node *x, Node *y) ;

        Node* merge(Node *x);

        // Node* decrease(Node *root, Node *x, T v);

        Node* pop_(Node *x);
    public:
        // pairHeap() : size_(0), root_(nullptr) {};

        pairHeap(Cmp cmp=Cmp()) : cmp(cmp), size_(0), root_(nullptr) {
        }

        pairHeap(const std::vector<T> &arr, Cmp cmp=Cmp()) : cmp(cmp), size_(arr.size()), root_(nullptr) {
            for (const T &e : arr) {
                Node *p = new Node(e);
                root_ = meld_(root_, p);
            }
        }

        pairHeap(const T& x) : size_(1)  {
            root_ = meld_(root_, new Node(x));
        } 

        ~pairHeap() {
            delete root_;
        }

        void meld (pairHeap other) {
            root_ = meld_(root_, other.root_);
            size_ += other.size_;
        }

        void push(Node *other) {
            root_ = meld_(root_, other);
            size_++;
        }

        void push(T x) {
            root_ = meld_(root_, new Node(x));
            size_++;
        }


        // void decrease(Node *x, T v) {
        //     root_ = decrease(root_, x, v);
        // }

        T pop() {
            assert(size_ && "The heap is empty...");
            T t = root_->v;
            root_ = pop_(root_);
            size_--;
            return t;

        }

        [[nodiscard]] bool empty() {
            return !size_;
        }

        [[nodiscard]] size_t size() const {
            return size_;
        }

        [[nodiscard]] T top() const {
            return root_->v;
        }
};

#include "pairHeap.tpp"
