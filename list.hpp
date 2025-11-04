#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T *data;
        node *prev;
        node *next;
        
        // Constructor for sentinel node (no data)
        node() : data(nullptr), prev(nullptr), next(nullptr) {}
        
        // Constructor for data node
        node(const T &value) : data(new T(value)), prev(nullptr), next(nullptr) {}
        
        // Destructor
        ~node() {
            if (data != nullptr) {
                delete data;
                data = nullptr;
            }
        }
    };

protected:
    node *head;  // sentinel node
    node *tail;  // sentinel node
    size_t count;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        pos->prev->next = cur;
        pos->prev = cur;
        return cur;
    }
    
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
        friend class list<T>;
        friend class const_iterator;
    private:
        node *ptr;
        const list<T> *lst;

    public:
        iterator() : ptr(nullptr), lst(nullptr) {}
        iterator(node *p, const list<T> *l) : ptr(p), lst(l) {}
        
        /**
         * iter++
         */
        iterator operator++(int) {
            if (ptr == nullptr || ptr->next == nullptr) {
                throw invalid_iterator();
            }
            iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }
        
        /**
         * ++iter
         */
        iterator & operator++() {
            if (ptr == nullptr || ptr->next == nullptr) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }
        
        /**
         * iter--
         */
        iterator operator--(int) {
            if (ptr == nullptr || ptr->prev == nullptr || ptr->prev->data == nullptr) {
                throw invalid_iterator();
            }
            iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }
        
        /**
         * --iter
         */
        iterator & operator--() {
            if (ptr == nullptr || ptr->prev == nullptr || ptr->prev->data == nullptr) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }
        
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }
        
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return ptr->data;
        }
        
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr && lst == rhs.lst;
        }
        
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr && lst == rhs.lst;
        }
        
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return !(*this == rhs);
        }
        
        bool operator!=(const const_iterator &rhs) const {
            return !(*this == rhs);
        }
    };
    
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
        friend class list<T>;
        friend class iterator;
    private:
        node *ptr;
        const list<T> *lst;

    public:
        const_iterator() : ptr(nullptr), lst(nullptr) {}
        const_iterator(node *p, const list<T> *l) : ptr(p), lst(l) {}
        const_iterator(const iterator &other) : ptr(other.ptr), lst(other.lst) {}
        
        const_iterator operator++(int) {
            if (ptr == nullptr || ptr->next == nullptr) {
                throw invalid_iterator();
            }
            const_iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }
        
        const_iterator & operator++() {
            if (ptr == nullptr || ptr->next == nullptr) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }
        
        const_iterator operator--(int) {
            if (ptr == nullptr || ptr->prev == nullptr || ptr->prev->data == nullptr) {
                throw invalid_iterator();
            }
            const_iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }
        
        const_iterator & operator--() {
            if (ptr == nullptr || ptr->prev == nullptr || ptr->prev->data == nullptr) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }
        
        const T & operator *() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }
        
        const T * operator ->() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return ptr->data;
        }
        
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr && lst == rhs.lst;
        }
        
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr && lst == rhs.lst;
        }
        
        bool operator!=(const iterator &rhs) const {
            return !(*this == rhs);
        }
        
        bool operator!=(const const_iterator &rhs) const {
            return !(*this == rhs);
        }
    };
    
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() : count(0) {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
    }
    
    list(const list &other) : count(0) {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        
        for (node *p = other.head->next; p != other.tail; p = p->next) {
            push_back(*(p->data));
        }
    }
    
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
        delete head;
        delete tail;
    }
    
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) return *this;
        
        clear();
        for (node *p = other.head->next; p != other.tail; p = p->next) {
            push_back(*(p->data));
        }
        return *this;
    }
    
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (count == 0) {
            throw container_is_empty();
        }
        return *(head->next->data);
    }
    
    const T & back() const {
        if (count == 0) {
            throw container_is_empty();
        }
        return *(tail->prev->data);
    }
    
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head->next, this);
    }
    
    const_iterator cbegin() const {
        return const_iterator(head->next, this);
    }
    
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(tail, this);
    }
    
    const_iterator cend() const {
        return const_iterator(tail, this);
    }
    
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return count == 0;
    }
    
    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return count;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        node *p = head->next;
        while (p != tail) {
            node *next = p->next;
            delete p;
            p = next;
        }
        head->next = tail;
        tail->prev = head;
        count = 0;
    }
    
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.lst != this) {
            throw invalid_iterator();
        }
        node *newNode = new node(value);
        insert(pos.ptr, newNode);
        count++;
        return iterator(newNode, this);
    }
    
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (count == 0) {
            throw container_is_empty();
        }
        if (pos.lst != this || pos.ptr == nullptr || pos.ptr->data == nullptr) {
            throw invalid_iterator();
        }
        node *next = pos.ptr->next;
        erase(pos.ptr);
        delete pos.ptr;
        count--;
        return iterator(next, this);
    }
    
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        node *newNode = new node(value);
        insert(tail, newNode);
        count++;
    }
    
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (count == 0) {
            throw container_is_empty();
        }
        node *last = tail->prev;
        erase(last);
        delete last;
        count--;
    }
    
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        node *newNode = new node(value);
        insert(head->next, newNode);
        count++;
    }
    
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (count == 0) {
            throw container_is_empty();
        }
        node *first = head->next;
        erase(first);
        delete first;
        count--;
    }
    
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (count <= 1) return;
        
        // Create array of node pointers
        node **arr = new node*[count];
        size_t idx = 0;
        for (node *p = head->next; p != tail; p = p->next) {
            arr[idx++] = p;
        }
        
        // Sort using provided algorithm (comparing the data in nodes)
        std::function<bool(node* const&, node* const&)> cmp = [](node* const& a, node* const& b) {
            return *(a->data) < *(b->data);
        };
        sjtu::sort<node*>(arr, arr + count, cmp);
        
        // Relink nodes in sorted order
        head->next = arr[0];
        arr[0]->prev = head;
        for (size_t i = 0; i < count - 1; i++) {
            arr[i]->next = arr[i + 1];
            arr[i + 1]->prev = arr[i];
        }
        arr[count - 1]->next = tail;
        tail->prev = arr[count - 1];
        
        delete[] arr;
    }
    
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) return;
        
        node *p1 = head->next;
        node *p2 = other.head->next;
        
        while (p2 != other.tail) {
            // Find insertion point in this list
            while (p1 != tail && !(*(p2->data) < *(p1->data))) {
                p1 = p1->next;
            }
            
            // Move node from other to this
            node *next2 = p2->next;
            other.erase(p2);
            insert(p1, p2);
            p2 = next2;
        }
        
        count += other.count;
        other.count = 0;
    }
    
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (count <= 1) return;
        
        node *p = head->next;
        while (p != tail) {
            node *tmp = p->next;
            p->next = p->prev;
            p->prev = tmp;
            p = tmp;
        }
        
        // Swap head and tail connections
        node *tmp = head->next;
        head->next = tail->prev;
        tail->prev = tmp;
        
        head->next->prev = head;
        tail->prev->next = tail;
    }
    
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (count <= 1) return;
        
        node *p = head->next;
        while (p != tail && p->next != tail) {
            if (*(p->data) == *(p->next->data)) {
                node *dup = p->next;
                erase(dup);
                delete dup;
                count--;
            } else {
                p = p->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
