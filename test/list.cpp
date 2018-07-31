// Copyright 2018 <raeesrajwani>

#include <iostream>

#include "../list/list.hpp"
#include "boost/type_index.hpp"

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do
                           // this in one cpp file
#include "./catch.hpp"

TEST_CASE("listIterator operations", "[listIterator]") {
  custom::list<int> l(1, 5);
  const custom::list<int> l_const(1, 5);

  SECTION("listIterator can be incremented") {
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::const_iterator it_const = l_const.begin();

    REQUIRE((it++) == l.begin());
    REQUIRE(it == l.end());
    REQUIRE(++(l.begin()) == l.end());

    REQUIRE((it_const++) == l_const.begin());
    REQUIRE(it_const == l_const.end());
    REQUIRE(++(l_const.begin()) == l_const.end());
  }

  SECTION("listIterator can be decremented") {
    custom::list<int> lDec(2, 5);
    const custom::list<int> lDec_const(2, 5);

    custom::list<int>::iterator it = lDec.begin();
    custom::list<int>::const_iterator it_const = lDec_const.begin();

    ++it;
    REQUIRE(--it == lDec.begin());
    REQUIRE(it-- == lDec.begin());
    REQUIRE(it.is_null());

    ++it_const;
    REQUIRE(--it_const == lDec_const.begin());
    REQUIRE(it_const-- == lDec_const.begin());
    REQUIRE(it_const.is_null());
  }

  SECTION("listIterator can be dereferenced") {
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::const_iterator it_const = l_const.begin();
    REQUIRE(*it == 5);
    *it = 6;
    REQUIRE(*it == 6);
    REQUIRE(std::is_const_v<std::remove_reference_t<decltype(*it_const)>>);
  }

  SECTION("listIterator can be member selected via pointer") {
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::const_iterator it_const = l_const.begin();
    REQUIRE(custom::list<int>::iterator(it->nextPtr_) == l.end());
    REQUIRE(custom::list<int>::const_iterator(it_const->nextPtr_) ==
            l_const.end());
    // it_const->nextPtr_ = nullptr; // TRIGGERS COMPILE TIME ERROR
  }

  SECTION("listIterator can be member selected via pointer") {
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::const_iterator it_const = l_const.begin();
    REQUIRE(custom::list<int>::iterator(it->nextPtr_) == l.end());
    REQUIRE(custom::list<int>::const_iterator(it_const->nextPtr_) ==
            l_const.end());
    // it_const->nextPtr_ = nullptr; // TRIGGERS COMPILE TIME ERROR
  }
  SECTION("listIterator is_null() returns true when nullptr") {
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::const_iterator it_const = l_const.begin();
    REQUIRE((--it).is_null());
    REQUIRE((--it_const).is_null());
  }

  SECTION("listIterator equality and inequality works") {
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::const_iterator it_const = l_const.begin();

    REQUIRE((--it).is_null());
    REQUIRE((--it_const).is_null());
  }
}

TEST_CASE("list can be constructed", "[list]") {
  SECTION("empty list creates a list with no elements with begin() == end()") {
    custom::list<int> l;
    REQUIRE(l.size() == 0);
    REQUIRE(l.begin() == l.end());
  }

  SECTION("list with n elements of same value") {
    custom::list<int> l(3, 5);
    REQUIRE(l.size() == 3);
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::iterator beforeBegin = --(l.begin());
    REQUIRE(beforeBegin.is_null() == true);
    while (it != l.end()) {
      REQUIRE(*it == 5);
      ++it;
    }
    REQUIRE(l.end().is_null() == true);
  }

  SECTION("list with n elements of default value") {
    custom::list<int> l(3);
    REQUIRE(l.size() == 3);
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::iterator beforeBegin = --(l.begin());
    REQUIRE(beforeBegin.is_null() == true);
    while (it != l.end()) {
      REQUIRE(*it == 0);
      ++it;
    }
    REQUIRE(l.end().is_null() == true);
  }

  SECTION("list from other list") {
    custom::list<int> l1(3);
    custom::list<int> l(l1.begin(), l1.end());
    REQUIRE(l.size() == 3);
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::iterator beforeBegin = --(l.begin());
    REQUIRE(beforeBegin.is_null() == true);
    while (it != l.end()) {
      REQUIRE(*it == 0);
      ++it;
    }
    REQUIRE(l.end().is_null() == true);
  }

  SECTION("list from other list iterators") {
    custom::list<int> l1(3, 4);
    custom::list<int> l((++l1.begin()), l1.end());
    REQUIRE(l.size() == 2);
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::iterator beforeBegin = --(l.begin());
    REQUIRE(beforeBegin.is_null() == true);
    while (it != l.end()) {
      REQUIRE(*it == 4);
      ++it;
    }
    REQUIRE(l.end().is_null() == true);
  }

  SECTION("list from initializer list") {
    custom::list<int> l{1, 2, 3};
    REQUIRE(l.size() == 3);
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::iterator beforeBegin = --(l.begin());
    REQUIRE(beforeBegin.is_null() == true);
    int val = 1;
    while (it != l.end()) {
      REQUIRE(*it == val);
      ++it;
      ++val;
    }
    REQUIRE(l.end().is_null() == true);
  }

  SECTION("list from copy") {
    custom::list<int> l1{1, 2, 3};
    custom::list l = l1;
    REQUIRE(l.size() == 3);
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::iterator beforeBegin = --(l.begin());
    REQUIRE(beforeBegin.is_null() == true);
    int val = 1;
    while (it != l.end()) {
      REQUIRE(*it == val);
      ++it;
      ++val;
    }
    REQUIRE(l.end().is_null() == true);
  }

  SECTION("list from move") {
    custom::list<int> l1{1, 2, 3};
    custom::list<int>::iterator prevIt = l1.begin();
    custom::list l = std::move(l1);
    REQUIRE(l.size() == 3);
    custom::list<int>::iterator it = l.begin();
    custom::list<int>::iterator beforeBegin = --(l.begin());
    REQUIRE(beforeBegin.is_null() == true);
    int val = 1;
    while (it != l.end()) {
      REQUIRE(*it == val);
      ++it;
      ++val;
    }
    REQUIRE(l.end().is_null() == true);
    REQUIRE(prevIt ==
            l.begin());  // Iterators are not invalidated and point to new list
  }
}

TEST_CASE("list can be cleared", "[list]") {
  custom::list<int> l1{1, 2, 3};
  l1.clear();
  REQUIRE(l1.size() == 0);
  REQUIRE(l1.begin() == l1.end());
  REQUIRE(l1.begin().is_null());
}

TEST_CASE("list can be erased", "[list]") {
  custom::list<int> l{1, 2, 3, 4, 5};

  // erase from beginning
  custom::list<int>::iterator retValBegin = l.erase(l.cbegin());
  REQUIRE(l.size() == 4);
  REQUIRE(*retValBegin == 2);
  custom::list<int>::iterator it = l.begin();
  int x = 2;
  while (it != l.end()) {
    REQUIRE(*it == x);
    ++x;
    ++it;
  }

  it = l.begin();
  for (int i = 0; i < 3; ++i) {
    ++it;
  }

  // erase from end
  custom::list<int>::iterator retValEnd =
      l.erase(custom::list<int>::const_iterator(it));
  REQUIRE(l.size() == 3);
  REQUIRE(retValEnd.is_null());

  it = l.begin();
  x = 2;
  while (it != l.end()) {
    REQUIRE(*it == x);
    ++x;
    ++it;
  }

  it = l.begin();
  ++it;

  // erase from middle
  custom::list<int>::iterator retValMid =
      l.erase(custom::list<int>::const_iterator(it));
  REQUIRE(l.size() == 2);

  it = l.begin();
  x = 2;
  while (it != l.end()) {
    REQUIRE(*it == x);
    x *= 2;
    ++it;
  }

  custom::list<int> l2{1, 3, 3, 3, 1};
  custom::list<int>::const_iterator it2 = l2.cbegin();
  custom::list<int>::const_iterator first = ++it2;
  custom::list<int>::const_iterator last = ++(++(++it2));
  std::cout << "*****" << std::endl;
  l2.erase(first, last);

  it = l2.begin();
  x = 1;
  while (it != l.end()) {
    std::cout << *it << std::endl;
    REQUIRE(*it == x);
    ++it;
  }
}

// int main(void) {
//   // volatile custom::list<int> l1{1, 2, 3};
//   volatile int x = 5;
// }