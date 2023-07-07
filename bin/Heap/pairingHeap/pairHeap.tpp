#pragma once
#include <algorithm>
#include "pairHeap.hpp"

template<typename T, typename Cmp>
void pairHeap<T, Cmp>::swap(Node *&x, Node *&y) {
    Node *temp = x;
    x = y;
    y = temp;
}

template<typename T, typename Cmp>
Node<T>* pairHeap<T, Cmp>::meld_(Node *x, Node *y) {
    if (x == nullptr) return y;
    if (y == nullptr) return x;
    // The compare function should return true when "smaller" on the
    // left hand side.

    if (cmp(y->v, x->v)) swap(x, y);

    // if (x->child != nullptr) x->child->parent = y;
    y->sibling = x->child;
    // y->parent = x;
    x->child = y;
    return x;
}

template<typename T, typename Cmp>
Node<T>* pairHeap<T, Cmp>::merge(Node *x)  {
    // Call recursive version if less elements and mute the parent pointer
    // to increase performance
    // Or increase the stack limit

//    if (x == nullptr) return nullptr;
//    x->parent = nullptr;
//    if (x->sibling == nullptr) return x;
    if (x == nullptr || x->sibling == nullptr) return x;
    Node *y = x->sibling, *c = y->sibling;
    // y->parent = nullptr;
    x->sibling = y->sibling = nullptr;
    return meld_(merge(c), meld_(x, y));

    // Iterative version to handle large amount of elements but
    // sloooooooooow
    // if (!x)
    //     return nullptr;

    // Node* root = nullptr;
    // std::vector<Node*> pairs;

    // // Push the nodes to a vector
    // while (x) {
    //     pairs.push_back(x);
    //     x = x->sibling;
    // }

    // // Pairwise merging
    // while (pairs.size() > 1) {
    //     Node* merged = meld(pairs[0], pairs[1]);
    //     pairs.erase(pairs.begin(), pairs.begin() + 2);
    //     pairs.push_back(merged);
    // }

    // // The last node in the vector is the root
    // if (!pairs.empty())
    //     root = pairs[0];

    // return root;
}


// template<typename T, typename Cmp>
// Node<T>* pairHeap<T, Cmp>::decrease(Node *root, Node *x, T v) {
//     x->v = v;
//     if (x == root) return x;
//     if (x->parent->child == x) x->parent->child = x->sibling;
//     else x->parent->sibling = x->sibling;
//     if (x->sibling) x->sibling->parent = x->parent;
//     x->sibling = nullptr;
//     x->parent = nullptr;
//     return meld(root, x);
// }


template<typename T, typename Cmp>
Node<T>* pairHeap<T, Cmp>::pop_(Node *x) {
    // Node *t = merge(x->child);
    // If remove the "delete" line, the memory that holds the
    // original x is not release, we can still access the v stored
    // at that memory or else we will just get some garbage value after deleting
    // delete x;
    // return t;

    return merge(x->child);
}

