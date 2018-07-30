//
//  custom_alloc.hpp
//  override
//
//  Created by Raees Rajwani on 25/02/2018.
//  Copyright © 2018 Raees Rajwani. All rights reserved.
//

// allocator adapted from
// http://www.drdobbs.com/the-standard-librarian-what-are-allocato/184403759

#ifndef custom_alloc_h
#define custom_alloc_h

#include <cstdlib>
#include <iostream>

template <typename T>
class allocator {
 public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  allocator(){};
  ~allocator(){};

  template <typename U>
  allocator(const allocator<U>&){};

  template <typename U>
  void operator=(const allocator<U>&){};

  template <class U>
  struct rebind {
    typedef allocator<U> other;
  };

  pointer address(reference x) { return &x; }

  const_pointer address(const_reference x) const { return &x; }

  pointer allocate(size_type n, const_pointer = 0) {
    void* p = malloc(n * sizeof(T));
    if (!p) {
      throw std::bad_alloc();
    }
    return static_cast<pointer>(p);
  }

  void deallocate(pointer p, size_type) { free(p); }

  size_type max_size() const {
    return static_cast<size_type>(-1) / sizeof(value_type);
  }

  void construct(pointer p, const value_type& x) { new (p) value_type(x); }

  void destroy(pointer p) { p->~value_type(); }
};

template <class T>
bool operator==(const allocator<T>&, const allocator<T>&) {
  return true;
}

template <class T>
bool operator!=(const allocator<T>&, const allocator<T>&) {
  return false;
}

template <>
class allocator<void> {
  typedef void value_type;
  typedef void* pointer;
  typedef const void* const_pointer;

  template <class U>
  struct rebind {
    typedef allocator<U> other;
  };
};

#endif /* custom_alloc_hpp */
