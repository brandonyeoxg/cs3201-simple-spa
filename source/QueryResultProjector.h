#pragma once
#ifndef QRP_H
#define QRP_H
#include <string>
#include <list>

using namespace std;

class QueryResultProjector
{
public:
  void formatResult();
  string printResult(list<string>);
};

#endif