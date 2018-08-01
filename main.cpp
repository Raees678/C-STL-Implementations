// Copyright 2018 <raeesrajwani>
#include <iostream>
#include <list>
#include <memory>
#include "./list/list.hpp"

int main(int argc, char* argv[]) {
  custom::list<std::unique_ptr<int>> l;
  auto& x = l.emplace_back(new int(6));
}
