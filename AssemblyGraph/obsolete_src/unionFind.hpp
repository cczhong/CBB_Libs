#ifndef UNIONFIND_H_
#define UNIONFIND_H_
# include <vector>
# include <iostream>
class DisjointSet
{
private:
  std::vector<int> s = std::vector<int>(10, -1);
public:
  int find( int x ) const;
  int find( int x );
  void unionNode( int x, int y );
  void insert(int x);
};
#endif
