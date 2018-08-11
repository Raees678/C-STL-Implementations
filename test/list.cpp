// Copyright 2018 <raeesrajwani>

#include <iostream>
#include <memory>

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

  SECTION("list from copy construction") {
    custom::list<int> l1{1, 2, 3};
    custom::list<int> l = l1;
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

    l.back() = 22;
    REQUIRE(l1.back() == 3);
    REQUIRE(l.back() == 22);
  }

  SECTION("list from copy initialization") {
    custom::list<int> l1{1, 2, 3};
    custom::list<int> l;
    l = l1;
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

    l.back() = 22;
    REQUIRE(l1.back() == 3);
    REQUIRE(l.back() == 22);
  }

  SECTION("list from move construction") {
    custom::list<int> l1{1, 2, 3};
    custom::list<int>::iterator prevIt = l1.begin();
    custom::list<int> l(std::move(l1));
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

  SECTION("list from move assignment") {
    custom::list<int> l1{1, 2, 3};
    custom::list<int>::iterator prevIt = l1.begin();
    custom::list<int> l;
    l = std::move(l1);
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

  SECTION("list from move assignment") {
    custom::list<int> l;
    l = {1, 2, 3};
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
  l2.erase(first, last);

  it = l2.begin();
  x = 1;
  while (it != l.end()) {
    REQUIRE(*it == x);
    ++it;
  }
}

TEST_CASE("list can be pushed_back", "[list]") {
  custom::list<int> l;
  l.push_back(5);
  REQUIRE(l.size() == 1);
  REQUIRE(l.back() == 5);
  l.push_back(6);
  REQUIRE(l.size() == 2);
  REQUIRE(l.back() == 6);
  int x = 7;
  l.push_back(x);
  REQUIRE(l.size() == 3);
  REQUIRE(l.back() == 7);
}

TEST_CASE("list can be emplaced_back", "[list]") {
  custom::list<std::unique_ptr<int>> l;
  custom::list<std::unique_ptr<int>>::reference ref =
      l.emplace_back(new int(6));
  REQUIRE(*ref == 6);
}

TEST_CASE("list can be popped_back", "[list]") {
  custom::list<int> l{1, 2};
  l.pop_back();
  REQUIRE(l.size() == 1);
  REQUIRE(l.front() == 1);
  REQUIRE(l.back() == 1);
}

TEST_CASE("list can be pushed_front", "[list]") {
  custom::list<int> l;
  l.push_front(5);
  REQUIRE(l.size() == 1);
  REQUIRE(l.front() == 5);
  l.push_front(6);
  REQUIRE(l.size() == 2);
  REQUIRE(l.front() == 6);
  int x = 7;
  l.push_front(x);
  REQUIRE(l.size() == 3);
  REQUIRE(l.front() == 7);
}

TEST_CASE("list can be emplaced_front", "[list]") {
  custom::list<std::unique_ptr<int>> l;
  custom::list<std::unique_ptr<int>>::reference ref =
      l.emplace_front(new int(6));
  REQUIRE(*ref == 6);
}

TEST_CASE("list can be popped_front", "[list]") {
  custom::list<int> l{1, 2};
  l.pop_front();
  REQUIRE(l.size() == 1);
  REQUIRE(l.front() == 2);
  REQUIRE(l.back() == 2);
}

TEST_CASE("list can be resized", "[list]") {
  custom::list<int> l{1, 2};
  l.resize(1);
  REQUIRE(l.size() == 1);
  l.resize(2);
  REQUIRE(l.size() == 2);
  REQUIRE(l.back() == 0);
  l.resize(4, 17);
  REQUIRE(l.size() == 4);
  REQUIRE(l.back() == 17);
  custom::list<int>::iterator it = l.begin();
  for (int i = 0; i < 3; ++i) {
    ++it;
  }
  REQUIRE(*it == 17);
}

TEST_CASE("list can be swapped", "[list]") {
  custom::list<int> l1{1, 2};
  custom::list<int>::iterator it = l1.begin();
  custom::list<int> l2{3, 4, 5};
  l1.swap(l2);
  REQUIRE(l2.front() == 1);
 
  REQUIRE(l2.back() == 2);
  REQUIRE(l1.front() == 3);
  REQUIRE(*(++l1.begin()) == 4);
  REQUIRE(l1.back() == 5);
  REQUIRE(it != l1.begin());
  REQUIRE(it == l2.begin());
}

TEST_CASE("list can be merged", "[list]") {
  custom::list<int> l1{3, 4, 3330, 5549};
  custom::list<int> l2{1, 5, 7, 99292, 443939};

  custom::list<int> l3{3, 4, 3330, 5549};

  custom::list<int>::iterator it = l2.begin();
  l1.merge(l2);
  l3.merge(custom::list<int>{1, 5, 7, 99292, 443939});

  REQUIRE(l1.size() == 9);
  REQUIRE(l2.size() == 0);
  REQUIRE(l1.begin() == it);

  int prevElement = -1;
  for(const int& element: l1) {
    REQUIRE(prevElement <= element);
    prevElement = element;
  }

  REQUIRE(l3.size() == 9);

  prevElement = -1;
  for(const int& element: l3) {
    REQUIRE(prevElement <= element);
    prevElement = element;
  }
}
