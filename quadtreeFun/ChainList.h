#pragma once
#include <stdint.h>
#include <utility>


template <class T>
class Link {
public:
    Link(Link<T> *previous, T &&data):
        previous(previous),
        m_data(data) {
        puts("Init link");
    }
    ~Link() {
        previous = nullptr;
        next = nullptr;
        puts("Del link");
    }
    const T &GetData() const { return m_data; }
    Link<T> *previous;
    Link<T> *next = nullptr;

private:
    const T m_data;
};

template <class T>
class Chain {
public:
    Chain() {}
    ~Chain() { Clear(); }

    const Link<T> *GetFirst() const { return m_first; }
    const Link<T> *GetLast()  const { return m_last; }
    uint32_t Size() const { return this->m_size; }

    const Link<T> *New(T &&data);
    Link<T> *Pop();
    void Clear();

private:
    Link<T> *m_first = nullptr;
    Link<T> *m_last = nullptr;
    uint32_t m_size;
};

template <class T>
const Link<T> *Chain<T>::New(T &&data) {
    auto *const new_link = new Link<T>(m_last, std::move(data));
    switch (m_size) {
    case 0:
        m_first = new_link;
        m_size++;
        return m_first;
    case 1:
        m_first->next = new_link;
        m_last = new_link;
        break;
    default:
        m_last->next = new_link;
        m_last = new_link;
        break;
    }
    m_size++;
    return m_last;
}

template <class T>
Link<T> *Chain<T>::Pop() {
    Link<T> *tmp = nullptr;
    switch (m_size) {
    case 0:
        m_size++;
        break;
    case 1:
        tmp = m_first;
        m_first = nullptr;
        break;
    case 2:
        tmp = m_last;
        m_last = nullptr;
        m_first->next = nullptr;
        tmp->previous = nullptr;
        break;
    default:
        tmp = m_last;
        m_last = m_last->previous;
        m_last->next = nullptr;
        tmp->previous = nullptr;
        break;
    }

    m_size--;
    return tmp;
}

template <class T>
void Chain<T>::Clear() {
    if (m_size > 0) {
        Link<T> *tmp = m_last;
        while (tmp != nullptr) {
            tmp = tmp->previous;
            if (tmp == nullptr) break;
            delete tmp->next;
        }
        delete m_first;
    }
    m_first = nullptr;
    m_last = nullptr;
    m_size = 0;
}
