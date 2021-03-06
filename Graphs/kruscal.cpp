#include "dsu.cpp"
#include <vector>
#include <algorithm>

typedef long long w_t; ///type of edges' weight (int, long long, float, ...)

///a single node of the graph
struct edge
{
	int u, v; ///edge u->v and v->u
	w_t w;
	bool inMst; ///is it in the MST?
	edge(): inMst(false){}

	bool operator < (const edge&a) const
	{
		return this->w < a.w;
	}
};

///finds the MST in O(NlogN)
w_t kruscal(std::vector<node>&nodes, std::vector<edge>&edges)
{
	init(nodes);
	std::sort(edges.begin(), edges.end());

	w_t answ = 0;

  for(int i=0; i<edges.size(); i++)
		if(dsu(nodes, edges[i].u, edges[i].v))
		{
			edges[i].inMst = true;
			answ += edges[i].w;
		}

	return answ;
}
