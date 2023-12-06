#ifndef CPP_DLINKED_LIST
#define CPP_DLINKED_LIST

#include <stdint.h>

#include <functional>

#include "common/log/log.h"

// To avoid conflicts with std::list, llist us used.
namespace llist
{
namespace singly_linked
{
template <typename T>
struct Node {
    T data;
    Node *next;

    Node(T d, Node *n)
        : data(d)
        , next(n)
    {
    }
    Node(T d)
        : Node(d, nullptr)
    {
    }
};

template <typename T>
class SlinkedList
{
public:
    SlinkedList()
        : size_(0)
        , head_(nullptr)
        , tail_(nullptr)
    {
    }
    ~SlinkedList() {}

    bool InsertSart(T &&data);
    bool InsertSart(const T &data);
    bool InsertSart(Node<T> *node);

    bool InsertEnd(T &&data);
    bool InsertEnd(const T &data);
    bool InsertEnd(Node<T> *node);

    bool InsertNodeBefore(Node<T> *next, Node<T> *node);
    bool InsertNodeAfter(Node<T> *prev, Node<T> *node);

    Node<T> *FindPrev(Node<T> *node);

    bool MoveStart(Node<T> *node);

    bool MoveEnd(Node<T> *node);

    Node<T> *Remove(Node<T> *node);

    void ForwardTraverse(std::function<int(Node<T> *node)> cb);

private:
    uint32_t size_;
    Node<T> *head_;
    Node<T> *tail_;
};

template <typename T>
bool SlinkedList<T>::InsertSart(T &&data)
{
    Node<T> *node = new (std::nothrow) Node<T>(data);
    if (node == nullptr) {
        return false;
    }
    InsertSart(node);
    return true;
}

template <typename T>
bool SlinkedList<T>::InsertSart(const T &data)
{
    Node<T> *node = new (std::nothrow) Node<T>(data);
    if (node == nullptr) {
        return false;
    }
    InsertSart(node);
    return true;
}

template <typename T>
bool SlinkedList<T>::InsertSart(Node<T> *node)
{
    if (node == nullptr) {
        return false;
    }

    if (head_ == nullptr) {
        head_ = node;
        tail_ = node;
    } else {
        InsertNodeBefore(head_, node);
    }
    return true;
}

template <typename T>
bool SlinkedList<T>::InsertEnd(T &&data)
{
    Node<T> *node = new (std::nothrow) Node<T>(data);
    if (node == nullptr) {
        return false;
    }
    InsertEnd(node);
    return true;
}

template <typename T>
bool SlinkedList<T>::InsertEnd(const T &data)
{
    Node<T> *node = new (std::nothrow) Node<T>(data);
    if (node == nullptr) {
        return false;
    }
    InsertEnd(node);
    return true;
}

template <typename T>
bool SlinkedList<T>::InsertEnd(Node<T> *node)
{
    if (node == nullptr) {
        return false;
    }

    node->next = nullptr;
    if (tail_ == nullptr) {
        head_ = node;
        tail_ = node;
    } else {
        InsertNodeAfter(tail_, node);
    }
    return true;
}

template <typename T>
bool SlinkedList<T>::InsertNodeBefore(Node<T> *next, Node<T> *node)
{
    Node<T> *prev;
    if (next == nullptr || node == nullptr) {
        return false;
    }

    if (next == head_) {
        node->next = head_;
        head_ = node;
    } else {
        prev = FindPrev(next);
        if (prev == nullptr) {
            LOG_ERROR("Can't find the node in the singly-linked list!");
            return false;
        }
        node->next = prev->next;
        prev->next = node;
    }
    size_++;
}

template <typename T>
bool SlinkedList<T>::InsertNodeAfter(Node<T> *prev, Node<T> *node)
{
    if (prev == nullptr || node == nullptr) {
        return false;
    }

    if (tail_ == prev) {
        prev->next = node;
        tail_ = node;
    } else {
        node->next = prev->next;
        prev->next = node;
    }
    size_++;
    return true;
}

template <typename T>
Node<T> *SlinkedList<T>::FindPrev(Node<T> *node)
{
    bool found = false;
    Node<T> *found_node = NULL;

    if (node == nullptr) {
        return nullptr;
    }

    for (found_node = head_; found_node != NULL; found_node = found_node->next) {
        if (found_node->next == node) {
            found = true;
            break;
        }
    }
    return found ? found_node : nullptr;
}

template <typename T>
bool SlinkedList<T>::MoveStart(Node<T> *node)
{
    Node<T> *prev;
    if (node == nullptr || head_ == nullptr) {
        return false;
    }
    if (head_ == node) {
        return true;
    }

    prev = FindPrev(node);
    if (prev == nullptr) {
        LOG_ERROR("Can't find the node in the singly-linked list!");
        return false;
    }

    if (tail_ == node) {
        tail_ = prev;
    }

    prev->next = node->next;
    node->next = head_;
    head_ = node;

    return true;
}

template <typename T>
bool SlinkedList<T>::MoveEnd(Node<T> *node)
{
    Node<T> *prev;
    if (node == nullptr || tail_ == nullptr) {
        return false;
    }
    if (tail_ == node) {
        return true;
    }

    if (head_ == node) {
        head_ = node->next;
    } else {
        prev = FindPrev(node);
        if (prev == nullptr) {
            LOG_ERROR("Can't find the node in the singly-linked list!");
            return false;
        }

        prev->next = node->next;
        tail_->next = node;
        tail_ = node;
        node->next = nullptr;
    }

    return true;
}

template <typename T>
Node<T> *SlinkedList<T>::Remove(Node<T> *node)
{
    Node<T> *prev;
    if (node == nullptr) {
        return nullptr;
    }

    if (head_ == node) {
        head_ = node->next;
    } else {
        prev = FindPrev(node);
        if (prev == nullptr) {
            LOG_ERROR("Can't find the node in the singly-linked list!");
            return false;
        }

        prev->next = node->next;
        if (tail_ == node) {
            tail_ = prev;
        }
    }
    size_--;

    return node;
}

template <typename T>
void SlinkedList<T>::ForwardTraverse(std::function<int(Node<T> *node)> cb)
{

    Node<T> *node = head_;
    while (node) {
        if (cb(node) == 1) {
            break;
        }
        node = node->next;
    }
}
} // namespace singly_linked
} // namespace llist

#endif /* CPP_DLINKED_LIST */
