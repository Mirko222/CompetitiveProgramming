#include <vector>

///node structure needed by dsu
struct node
{
	int parent, rnk;
};

///returns the parent of 'a'. O(a(N)) -> a(N): inverse of the ackermann function
int fnd(std::vector<node>&p, int a)
{
	if(p[a].parent != p[p[a].parent].parent)
		p[a].parent = fnd(p, p[a].parent); //path compression
	return p[a].parent;
}

///merge set of 'a' with set of 'b'. O(a(N)) -> a(N): inverse of the ackermann function
bool dsu(std::vector<node>&p, int a, int b)
{
	int pa = fnd(p, a);
	int pb = fnd(p, b);

	if(pa == pb)
		return false;

  if(p[pa].rnk == p[pb].rnk)
		p[pa].rnk++;

	if(p[pa].rnk > p[pb].rnk) //merge by rank
		p[pb].parent = pa;
	else
		p[pa].parent = pb;

	return true;
}

///inits the array of nodes (1-based)
void init(std::vector<node>&nodes)
{
  for(int i=1; i<nodes.size(); i++)
	{
		nodes[i].parent = i;
		nodes[i].rnk = 0;
	}
}
