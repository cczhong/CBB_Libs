#include "GraphEdgeType.h"

using namespace std;

int main()  {

    GraphEdgeType a;
    cout << "init test: " << a.GetOverlap() << "    " << a.IsVisited() << endl;

    a.SetOverlap(10);
    cout << "SetOverlap test: " << a.GetOverlap() << endl;

    GraphEdgeType b;
    b = a;

    cout << "assignment test: " << b.GetOverlap() << "    " << b.IsVisited() << endl;

    b.SetVisited();
    cout << "SetVisited test: " << b.GetOverlap() << "    " << b.IsVisited() << endl;

    b.SetUnvisited();
    cout << "SetUnvisited test: " << b.GetOverlap() << "    " << b.IsVisited() << endl;

    return 0;
}