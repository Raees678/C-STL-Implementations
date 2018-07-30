#ifndef _USERS_RAEESRAJWANI_DOCUMENTS_HOBBY_DATA_STRUCTURES_CPP_ITERATOR_ITERATOR_HPP_
#define _USERS_RAEESRAJWANI_DOCUMENTS_HOBBY_DATA_STRUCTURES_CPP_ITERATOR_ITERATOR_HPP_

// Copyright 2018 <raeesrajwani>
#include <iterator>

template<typename T, typename X = void>
struct is_input_iterator {
  static constexpr bool value = false;
};

template<typename T>
struct is_input_iterator<T,
  typename std::enable_if<std::is_same<typename std::iterator_traits<T>::iterator_category, std::input_iterator_tag>::value>::type> {
    static constexpr bool value = true;
};

template<typename T>
struct is_input_iterator<T,
  typename std::enable_if<std::is_same<typename std::iterator_traits<T>::iterator_category, std::output_iterator_tag>::value>::type> {
    static constexpr bool value = true;
};

template<typename T>
struct is_input_iterator<T,
  typename std::enable_if<std::is_same<typename std::iterator_traits<T>::iterator_category, std::forward_iterator_tag>::value>::type> {
    static constexpr bool value = true;
};

template<typename T>
struct is_input_iterator<T,
  typename std::enable_if<std::is_same<typename std::iterator_traits<T>::iterator_category, std::bidirectional_iterator_tag>::value>::type> {
    static constexpr bool value = true;
};
//
template<typename T>
struct is_input_iterator<T,
  typename std::enable_if<std::is_same<typename std::iterator_traits<T>::iterator_category, std::random_access_iterator_tag>::value>::type> {
    static constexpr bool value = true;
};


#endif //_USERS_RAEESRAJWANI_DOCUMENTS_HOBBY_DATA_STRUCTURES_CPP_ITERATOR_ITERATOR_HPP_
