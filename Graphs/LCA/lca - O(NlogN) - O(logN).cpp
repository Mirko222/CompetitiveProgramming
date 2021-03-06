#include <vector>
#include <algorithm>

struct edge ///edge of the tree (add here attributes)
{
  int u;
};

struct node : std::vector<edge> ///node of the tree
{
  std::vector<int> table;
  int depth;
};

///set the parents of the nodes using dfs
void lca_dfs(std::vector<node>&nodes, int act, int from = -1)
{
	for(int i=0; i<nodes[act].size(); i++)
		if(nodes[act][i].u != from)
		{
			nodes[nodes[act][i].u].table[0] = act;
			nodes[nodes[act][i].u].depth = nodes[act].depth + 1;
			lca_dfs(nodes, nodes[act][i].u, act);
		}
}

///preprocessing O(NlogN) to be done to answer queries
void lca_preprocessing(std::vector<node>&nodes, int root)
{
	//init sparse tables
	int logn = 0;
	while((1<<logn) < nodes.size())
		logn++;

  for(int i=1; i<nodes.size(); i++)
	{
		nodes[i].table.resize(logn);
		for(int j=0; j<logn; j++)
			nodes[i].table[j] = -1;
	}

	//init sparse tables with parent
	nodes[root].depth = 0;
	lca_dfs(nodes, root);

	//complete sparse tables with ancestors
  for(int i=1; i<logn; i++)
		for(int j=1; j<nodes.size(); j++)
			nodes[j].table[i] = (nodes[j].table[i-1] != -1) ? nodes[nodes[j].table[i-1]].table[i-1] : -1;
}

///answer an LCA query in O(logN) (more flexible than O(1) implementation)
int lca_query(std::vector<node>&nodes, int a, int b)
{
	int logn = nodes[1].table.size();
	if(nodes[a].depth < nodes[b].depth)
		std::swap(a, b);

	//'a' and 'b' are taken to the same depth O(logN)
  int i = logn-1;
  while(nodes[a].depth != nodes[b].depth)
	{
    while(nodes[a].depth - (1<<i) < nodes[b].depth)
			i--;
    a = nodes[a].table[i];
	}

	//now answer the query
	i = logn-1;
	while(a != b)
	{
		while(i > 0 && nodes[a].table[i] == nodes[b].table[i])
			i--;
		a = nodes[a].table[i];
		b = nodes[b].table[i];
	}

	return a;
}
