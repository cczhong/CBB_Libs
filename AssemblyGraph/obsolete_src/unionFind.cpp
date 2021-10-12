# include "unionFind.hpp"
void DisjointSet::insert(int x)
{
  s.resize(2*x, -1);
}

void DisjointSet::unionNode( int x, int y )
{
  if(x >= s.size()) insert(x);
  if(y >= s.size()) insert(y);
  int root1 = find(x);
  int root2 = find(y);

  if(root1 != root2)
  {
    if( s[ root2 ] < s[ root1 ] ) // size of root2 > size of root1
    {
      s[ root2 ] += s[ root1 ];
      s[ root1 ] = root2;
    }
    else
    {
      s[ root1 ] += s[ root2 ];
      s[ root2 ] = root1;
    }
  }
}

int DisjointSet::find( int x ) const
{
  if( s[ x ] < 0 )  return x;
  else              return find( s[ x ] );
}

int DisjointSet::find( int x )
{
  if( s[ x ] < 0 )  return x;
  else              return s[ x ] = find( s[ x ] );
}
