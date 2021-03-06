#include <vector>

struct edge
{
	int u;
};

struct node : std::vector<edge>
{
	bool visitedTS;
	node(): visitedTS(false){}
};

///utility function for toposort
void topoDfs(std::vector<node>&nodes, int act, std::vector<int>&tsort)
{
  nodes[act].visitedTS = true;
  for(int i=0; i<nodes[act].size(); i++)
		if(!nodes[nodes[act][i].u].visitedTS)
			topoDfs(nodes, nodes[act][i].u, tsort);
	tsort.push_back(act);
}

///finds the topological sorting of the DAG (tsort will be inverted)
void toposort(std::vector<node>&nodes, std::vector<int>&tsort)
{
  for(int i=1; i<nodes.size(); i++)
		nodes[i].visitedTS = false;

	for(int i=1; i<nodes.size(); i++)
		if(!nodes[i].visitedTS)
			topoDfs(nodes, i, tsort);
}
