#ifndef _USERS_RAEESRAJWANI_DOCUMENTS_HOBBY_DATA_STRUCTURES_CPP_LIST_LIST_HPP_
#define _USERS_RAEESRAJWANI_DOCUMENTS_HOBBY_DATA_STRUCTURES_CPP_LIST_LIST_HPP_

// Copyright 2018 <raeesrajwani>

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <type_traits>

#include "../iterator/iterator.hpp"
#include "boost/type_index.hpp"

namespace custom {

template <class T, class Allocator = std::allocator<T>>
class list;

template <class T, class Allocator>
class list {
 private:
  template <class U>
  struct listNode {
    typedef U value_type;

    value_type value_;
    listNode<value_type>* nextPtr_;
    listNode<value_type>* prevPtr_;

    explicit listNode(const value_type& value,
                      listNode<value_type>* nextPtr = nullptr,
                      listNode<value_type>* prevPtr = nullptr)
        : value_(value), nextPtr_(nextPtr), prevPtr_(prevPtr) {}

    explicit listNode(value_type&& value,
                      listNode<value_type>* nextPtr = nullptr,
                      listNode<value_type>* prevPtr = nullptr)
        : value_(std::move(value)), nextPtr_(nextPtr), prevPtr_(prevPtr) {}
  };

  template <class U>
  class list_iterator {
    U* listNodePtr_;
    template <class, class>
    friend class list;
    template <class>
    friend class const_list_iterator;

   public:
    typedef std::ptrdiff_t difference_type;
    typedef U value_type;
    typedef U* pointer;
    typedef typename U::value_type& reference;
    typedef std::bidirectional_iterator_tag iterator_category;

    explicit list_iterator(pointer listNodePtr) : listNodePtr_(listNodePtr) {}

    list_iterator(const list_iterator<U>& other)
        : listNodePtr_(other.listNodePtr_) {}

    list_iterator<U> operator=(const list_iterator<U>& other) {
      this->listNodePtr_ = other.listNodePtr_;
      return *this;
    }

    list_iterator<value_type>& operator--() {
      listNodePtr_ = listNodePtr_->prevPtr_;
      return *this;
    }

    list_iterator<value_type> operator--(int) {
      pointer temp = listNodePtr_;
      --(*this);
      return list_iterator<value_type>(temp);
    }

    list_iterator<value_type>& operator++() {
      listNodePtr_ = listNodePtr_->nextPtr_;
      return *this;
    }

    list_iterator<value_type> operator++(int) {
      pointer temp = listNodePtr_;
      ++(*this);
      return list_iterator<value_type>(temp);
    }

    reference operator*() { return listNodePtr_->value_; }

    pointer operator->() { return listNodePtr_; }

    bool is_null() const { return !static_cast<bool>(listNodePtr_); }

    template <class V>
    bool operator==(const list_iterator<V>& other) const {
      return this->listNodePtr_ == other.listNodePtr_;
    }

    template <class V>
    bool operator!=(const list_iterator<V>& other) const {
      return !(*this == other);
    }
  };

  template <class U>
  class const_list_iterator {
    const U* listNodePtr_;
    template <class, class>
    friend class list;
    template <class>
    friend class list_iterator;

   public:
    typedef std::ptrdiff_t difference_type;
    typedef U value_type;
    typedef const U* pointer;
    typedef const typename U::value_type& reference;
    typedef std::bidirectional_iterator_tag iterator_category;

    explicit const_list_iterator(pointer listNodePtr)
        : listNodePtr_(listNodePtr) {}

    explicit const_list_iterator(const list_iterator<U>& listIterator)
        : listNodePtr_(const_cast<pointer>(listIterator.listNodePtr_)) {}

    const_list_iterator(const const_list_iterator<U>& other)
        : listNodePtr_(other.listNodePtr_) {}

    const_list_iterator<value_type>& operator--() {
      listNodePtr_ = listNodePtr_->prevPtr_;
      return *this;
    }

    const_list_iterator<value_type> operator--(int) {
      pointer temp = listNodePtr_;
      --(*this);
      return const_list_iterator<value_type>(temp);
    }

    const_list_iterator<value_type>& operator++() {
      listNodePtr_ = listNodePtr_->nextPtr_;
      return *this;
    }

    const_list_iterator<value_type> operator++(int) {
      pointer temp = listNodePtr_;
      ++(*this);
      return const_list_iterator<value_type>(temp);
    }

    reference operator*() const { return listNodePtr_->value_; }

    pointer operator->() const { return listNodePtr_; }

    bool is_null() const { return !static_cast<bool>(listNodePtr_); }

    template <class V>
    bool operator==(const const_list_iterator<V>& other) const {
      return this->listNodePtr_ == other.listNodePtr_;
    }

    template <class V>
    bool operator!=(const const_list_iterator<V>& other) const {
      return !(*this == other);
    }
  };

 public:
  typedef T value_type;
  typedef Allocator allocator_type;
  static_assert(
      (std::is_same<value_type, typename allocator_type::value_type>::value),
      "Invalid allocator::value_type");
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef typename std::allocator_traits<Allocator>::pointer pointer;
  typedef
      typename std::allocator_traits<Allocator>::const_pointer const_pointer;
  typedef list_iterator<listNode<T>> iterator;
  typedef const_list_iterator<listNode<T>> const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef typename allocator_type::template rebind<listNode<T>>::other
      rebound_allocator_type;

  //    *** CONSTRUCTORS ***
  explicit list(const Allocator& alloc = Allocator())
      : alloc_(alloc), head_(nullptr), tail_(nullptr), size_(0) {
    if constexpr (!std::is_same<Allocator, std::allocator<T>>::value) {
      std::cout << "Constructing list with custom allocator ";
      std::cout << boost::typeindex::type_id_with_cvr<decltype(alloc_)>()
                       .pretty_name()
                << std::endl;
    }
  }

  list(size_type size, const value_type& value,
       const Allocator& alloc = Allocator())
      : alloc_(alloc), head_(nullptr), tail_(nullptr), size_(0) {
    (*this).insert(const_iterator(head_), size, value);
  }

  explicit list(size_type size, const Allocator& alloc = Allocator())
      : list(size, value_type(), alloc) {}

  template <class InputIt>
  list(InputIt first, InputIt last, const Allocator& alloc = Allocator(),
       typename std::enable_if<is_input_iterator<InputIt>::value>::type* =
           nullptr)
      : alloc_(alloc), head_(nullptr), tail_(nullptr), size_(0) {
    (*this).insert(const_iterator(tail_), first, last);
  }

  list(std::initializer_list<T> init, const Allocator& alloc = Allocator())
      : alloc_(alloc), head_(nullptr), tail_(nullptr), size_(0) {
    (*this).insert(const_iterator(tail_), init);
  }

  list(const list& other)
      : list(other.begin(), other.end(),
             std::allocator_traits<allocator_type>::
                 select_on_container_copy_construction(other.get_allocator())) {
  }

  list(const list& other, const Allocator& alloc)
      : list(other.begin(), other.end(), alloc) {}

  list(list&& other)
      : alloc_(std::move(other.get_allocator())),
        head_(std::move(other.head_)),
        tail_(std::move(other.tail_)),
        size_(std::move(other.size())) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  list(list&& other, const Allocator& alloc)
      : alloc_(alloc),
        head_(std::move(other.head_)),
        tail_(std::move(other.tail_)),
        size_(std::move(other.size())) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  ~list() { clear(); }

  list& operator=(const list& other) {
    // if allocators can be copied AND are not equal
    if constexpr (std::allocator_traits<allocator_type>::
                      propagate_on_container_copy_assignment::value &&
                  !std::allocator_traits<
                      allocator_type>::is_always_equal::value) {
      // deallocate using current allocator
      clear();
      // copy new allocator for future reallocations
      alloc_ = other.alloc_;
    }
    // else allocators are equal OR allocator cannot be copied
    // use current/copied allocator to allocate copies 'value_type' values in
    // other
    assign(other.begin(), other.end());
    return *this;
  }

  list& operator=(list&& other) {
    // deallocate usint current allocator
    clear();

    // if allocators can be moved
    if constexpr (std::allocator_traits<allocator_type>::
                      propagate_on_container_move_assignment::value) {
      // move new allocator for future reallocations
      alloc_(std::move(other.alloc_));
      head_(std::move(other.head_));
      tail_(std::move(other.tail_));
      size_(std::move(other.size()));
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    } else {
      // allocators cant be moved but are equal
      if (std::allocator_traits<allocator_type>::is_always_equal::value) {
        // simply move new memory resources in
        head_(std::move(other.head_));
        tail_(std::move(other.tail_));
        size_(std::move(other.size()));
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
      } else {
        // allocators cant be moved AND are not equal
        // move elements individually using current allocator
        for (value_type&& element : other) {
          push_back(std::move(element));
        }
      }
    }
    return *this;
  }

  void assign(size_type count, const T& value) {
  clear();
  insert(cbegin(), count, value);
}

template <class InputIt>
void assign(InputIt first, InputIt last,
            typename std::enable_if<is_input_iterator<InputIt>::value>::type* =
                nullptr) {
  clear();
  insert(cbegin(), first, last);
}

void assign(std::initializer_list<T> ilist) {
  clear();
  insert(cbegin(), ilist);
}

allocator_type get_allocator() const { return alloc_; }

/**** ELEMENT ACCESS ****/
reference front() { return head_->value_; }

const_reference front() const { return head_->value_; }

reference back() { return tail_->value_; }

const_reference back() const { return tail_->value_; }

/***** CAPACITY *****/
bool empty() const noexcept { return !size_; }

size_type size() const noexcept { return size_; }

size_type max_size() const noexcept { return size(); }

/**** ITERATORS ****/
iterator begin() noexcept { return iterator(head_); }

const_iterator begin() const noexcept { return const_iterator(head_); }

const_iterator cbegin() const noexcept { return const_iterator(head_); }

iterator end() noexcept {
  if (size()) {
    return iterator(tail_->nextPtr_);
  }
  return iterator(tail_);
}

const_iterator end() const noexcept {
  if (size()) {
    return const_iterator(tail_->nextPtr_);
  }
  return const_iterator(tail_);
}

const_iterator cend() const noexcept {
  if (size()) {
    return const_iterator(tail_->nextPtr_);
  }
  return const_iterator(tail_);
}

iterator rbegin() noexcept {
  if (size()) {
    return reverse_iterator(tail_->nextPtr_);
  }
  return reverse_iterator(tail_);
}

const_iterator rbegin() const noexcept {
  if (size()) {
    return const_reverse_iterator(tail_->nextPtr_);
  }
  return const_reverse_iterator(tail_);
}

const_iterator crbegin() const noexcept {
  if (size()) {
    return const_reverse_iterator(tail_->nextPtr_);
  }
  return const_reverse_iterator(tail_);
}

iterator rend() noexcept { return reverse_iterator(head_); }

const_iterator rend() const noexcept { return const_reverse_iterator(head_); }

const_iterator crend() const noexcept { return const_reverse_iterator(head_); }

/**** MODIFIERS ****/
void clear() noexcept { erase(cbegin(), cend()); }

iterator insert(const_iterator pos, const_reference value) {
  listNode<T>* newNode =
      std::allocator_traits<rebound_allocator_type>::allocate(alloc_, 1);
  std::allocator_traits<rebound_allocator_type>::construct(alloc_, newNode,
                                                           value);
  // inserting into empty list
  if (size() == 0) {
    listNode<T>* prevHead = head_;
    head_ = newNode;
    tail_ = head_;
    newNode->nextPtr_ = prevHead;
    newNode->prevPtr_ = nullptr;
  } else {
    // inserting into front of list
    if (pos == cbegin()) {
      newNode->nextPtr_ = head_;
      head_->prevPtr_ = newNode;
      head_ = newNode;
      // inserting into back of list
    } else if (pos == cend()) {
      newNode->prevPtr_ = tail_;
      tail_->nextPtr_ = newNode;
      tail_ = newNode;
      // inserting into middle of list
    } else {
      listNode<T>* prevNode = pos->prevPtr_;
      newNode->nextPtr_ =
          const_cast<typename iterator::pointer>(pos.listNodePtr_);
      newNode->prevPtr_ = prevNode;
      prevNode->nextPtr_ = newNode;
      newNode->nextPtr_->prevPtr_ = newNode;
    }
  }

  ++size_;
  iterator newNodeIterator(newNode);
  return newNodeIterator;
}

iterator insert(const_iterator pos, T&& value) {
  listNode<T>* newNode =
      std::allocator_traits<rebound_allocator_type>::allocate(alloc_, 1);
  std::allocator_traits<rebound_allocator_type>::construct(alloc_, newNode,
                                                           std::move(value));
  // inserting into empty list
  if (size() == 0) {
    listNode<T>* prevHead = head_;
    head_ = newNode;
    tail_ = head_;
    newNode->nextPtr_ = prevHead;
    newNode->prevPtr_ = nullptr;
  } else {
    // inserting into front of list
    if (pos == cbegin()) {
      newNode->nextPtr_ = head_;
      head_->prevPtr_ = newNode;
      head_ = newNode;
      // inserting into back of list
    } else if (pos == cend()) {
      newNode->prevPtr_ = tail_;
      tail_->nextPtr_ = newNode;
      tail_ = newNode;
      // inserting into middle of list
    } else {
      listNode<T>* prevNode = pos->prevPtr_;
      newNode->nextPtr_ =
          const_cast<typename iterator::pointer>(pos.listNodePtr_);
      newNode->prevPtr_ = prevNode;
      prevNode->nextPtr_ = newNode;
      newNode->nextPtr_->prevPtr_ = newNode;
    }
  }

  ++size_;
  iterator newNodeIterator(newNode);
  return newNodeIterator;
}

iterator insert(const_iterator pos, size_type count, const T& value) {
  iterator newPos(const_cast<typename iterator::pointer>(pos.listNodePtr_));
  while (count) {
    newPos = insert(const_iterator(newPos), value);
    --count;
  }
  return newPos;
}

template <class InputIt>
iterator insert(
    const_iterator pos, InputIt first, InputIt last,
    typename std::enable_if<is_input_iterator<InputIt>::value>::type* =
        nullptr) {
  InputIt it = first;
  iterator newPos(const_cast<typename iterator::pointer>(pos.listNodePtr_));
  while (it != last) {
    (*this).insert(pos, *it);
    ++it;
  }
  return newPos;
}

iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) {
  typename std::initializer_list<value_type>::iterator it = ilist.begin();
  iterator newPos(const_cast<typename iterator::pointer>(pos.listNodePtr_));
  while (it != ilist.end()) {
    (*this).insert(pos, *it);
    ++it;
  }
  return newPos;
}

iterator erase(const_iterator pos) {
  listNode<value_type>* ptrToDelete =
      const_cast<typename iterator::pointer>(pos.listNodePtr_);
  listNode<value_type>* nextNode = ptrToDelete->nextPtr_;
  listNode<value_type>* prevNode = ptrToDelete->prevPtr_;

  if (ptrToDelete == head_) {
    if (head_ == tail_) {
      tail_ = nextNode;
    } else {
      nextNode->prevPtr_ = nullptr;
    }
    head_ = nextNode;
  } else if (ptrToDelete == tail_) {
    prevNode->nextPtr_ = nullptr;
    tail_ = prevNode;
  } else {
    prevNode->nextPtr_ = nextNode;
    nextNode->prevPtr_ = prevNode;
  }

  std::allocator_traits<rebound_allocator_type>::destroy(alloc_, ptrToDelete);
  std::allocator_traits<rebound_allocator_type>::deallocate(alloc_, ptrToDelete,
                                                            1);
  --size_;
  return iterator(nextNode);
}

iterator erase(const_iterator first, const_iterator last) {
  const_iterator it = first;
  iterator retVal =
      iterator(const_cast<typename iterator::pointer>(first.listNodePtr_));
  while (it != last) {
    retVal = erase(it);
    ++it;
  }
  return retVal;
}

void push_back(const T& value) { insert(cend(), value); }

void push_back(T&& value) { insert(cend(), std::move(value)); }

template <class... Args>
reference emplace_back(Args&&... args) {
  value_type value = value_type(std::move(args)...);
  iterator it = insert(cend(), std::move(value));
  return *it;
}

void pop_back() {
  const_iterator lastElementIt = const_iterator(tail_);
  erase(lastElementIt);
}

void push_front(const T& value) { insert(cbegin(), value); }

void push_front(T&& value) { insert(cbegin(), std::move(value)); }

template <class... Args>
reference emplace_front(Args&&... args) {
  value_type value = value_type(std::move(args)...);
  iterator it = insert(cbegin(), std::move(value));
  return *it;
}

void pop_front() {
  const_iterator firstElementIt = const_iterator(head_);
  erase(firstElementIt);
}

void resize(size_type count) {
  while (count < size()) {
    pop_back();
  }

  while (count > size()) {
    push_back(value_type());
  }
}

void resize(size_type count, const value_type& value) {
  while (count < size()) {
    pop_back();
  }

  while (count > size()) {
    push_back(value);
  }
}

void swap(list& other) noexcept(
    std::allocator_traits<allocator_type>::is_always_equal::value) {
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
  if constexpr (std::allocator_traits<
                    allocator_type>::propagate_on_container_swap::value) {
    std::swap(alloc_, other.alloc_);
  }
}

private:
rebound_allocator_type alloc_;
listNode<value_type>* head_;
listNode<value_type>* tail_;
size_type size_;
};  // namespace custom
}  //  namespace custom

#endif  // _USERS_RAEESRAJWANI_DOCUMENTS_HOBBY_DATA_STRUCTURES_CPP_LIST_LIST_HPP_
