//
// Created by Nikita Mastinen on 20.12.2020.
//

#ifndef DEQUE_DEQUE_H
#define DEQUE_DEQUE_H

#endif //DEQUE_DEQUE_H

#include <vector>
#include <deque>

using std::cerr;
using std::cout;
using std::endl;
using std::vector;

template <typename T>
class Deque {
  vector<T> buffer;
  size_t l = 0;
  size_t r = 0;
  int move = 0;

  void resize() {
    size_t shift = buffer.size() - r + l / 2 + 1;
    buffer.resize((buffer.size() + 1) * 2);
    for (size_t i = l; i < r; i++) {
      buffer[r + l - 1 - i + shift] = buffer[r + l - 1 - i];
      buffer[r + l - 1 - i] = T();
    }
    l += shift;
    r += shift;
  }
public:
  Deque() = default;

  Deque(int length, T value) {
    l = 0;
    r = length;
    buffer = vector(length, value);
  }

  explicit Deque(int length): Deque(length, T()) {}

  size_t size() const {
    return r - l;
  }

  T& operator[](size_t position) {
    return buffer[l + position];
  }

  const T& operator[](size_t position) const {
    return buffer[l + position];
  }

  T& at(size_t position) {
    if (position >= r - l) throw std::out_of_range("index out of range");
    return buffer[l + position];
  }

  const T& at(size_t position) const {
    if (position >= r - l) throw std::out_of_range("index out of range");
    return buffer[l + position];
  }

  void push_back(T value) {
    if (r + 1 >= buffer.size()) resize();
    buffer[r++] = value;
  }

  void push_front(T value) {
    if (l == 0) resize();
    buffer[--l] = value;
    ++move;
  }

  void pop_front() {
    buffer[l++] = T();
    --move;
  }

  void pop_back() {
    buffer[--r] = T();
  }

  friend class iterator;
  friend class const_iterator;

  class iterator {
  protected:
    int it = 0;
    const Deque<T>* parent;
    int shift = 0;
  public:
    friend class Deque<T>;

    iterator() = default;

    iterator& operator--() {
      --it;
     // cout << it;
      return *this;
    }

    iterator& operator++() {
      ++it;
      return *this;
    }

    iterator& operator+=(int value) {
      it += value;
      return *this;
    }

    iterator& operator-=(int value) {
      it -= value;
      return *this;
    }

    iterator operator--(int) {
      iterator copy = *this;
      --it;
      return copy;
    }

    iterator operator++(int) {
      iterator copy = *this;
      ++it;
      return copy;
    }

    iterator operator+(int value) {
      iterator copy = *this;
      copy += value;
      return copy;
    }

    iterator operator-(int value) {
      iterator copy = *this;
      copy -= value;
      return copy;
    }

    int operator-(iterator value) {//TODO
      iterator copy = *this;
      copy.it -= value.it;
      return copy.it;
    }

    T& operator*() {
      return parent->buffer[parent->l + it + parent->move - shift];
    }

    T* operator->() {
      return &parent->buffer[parent->l + it + parent->move - shift];
    }

    bool operator<(const iterator& y) {
      return it - shift < y.it - y.shift;
    }

    bool operator>(const iterator& y) {
      return it - shift > y.it - y.shift;
    }

    bool operator>=(const iterator& y) {
      return it - shift >= y.it - y.shift;
    }

    bool operator<=(const iterator& y) {
      return it - shift <= y.it - y.shift;
    }

    bool operator==(const iterator& y) {
      return it - shift == y.it - y.shift && parent == y.parent;
    }

    bool operator!=(const iterator& y) {
      return parent != y.parent || it - shift != y.it - y.shift;
    }
  };

  class const_iterator: public Deque<T>::iterator {
  public:
    const T& operator*() const {
      return iterator::parent->buffer[iterator::parent->l +
          iterator::it + iterator::parent->move - iterator::shift];
    }

    const T* operator->() const {
      return &iterator::parent->buffer[iterator::parent->l + iterator::it +
          iterator::parent->move - iterator::shift];
    }
  };

  iterator begin() {
    iterator iterator;
    iterator.parent = this;
    iterator.it = 0;
    iterator.shift = move;
    return iterator;
  }

  iterator end() {
    iterator iterator;
    iterator.parent = this;
    iterator.it = r - l;
    iterator.shift = move;
    return iterator;
  }

  const_iterator cbegin() const {
    const_iterator iterator;
    iterator.parent = this;
    iterator.it = 0;
    iterator.shift = move;
    return iterator;
  }

  const_iterator cend() const {
    const_iterator iterator;
    iterator.parent = this;
    iterator.it = r - l;
    iterator.shift = move;
    return iterator;
  }

  iterator begin() const {
    return cbegin();
  }

  iterator end() const {
    return cend();
  }

  void insert(iterator it, T a) {
    push_back(a);
    for (auto i = --end(); i > it; --i) {
      std::swap(*i, *(i - 1));
    }
  }

  void erase(iterator it) {
    for (auto i = it; i < --end(); ++i) {
      std::swap(*i, *(i + 1));
    }
    pop_back();
  }

  void print() const {
    for (int i = l; i < r; i++) {
      cout << buffer[i] << ' ';
    }
    cout << endl;
  }
};

