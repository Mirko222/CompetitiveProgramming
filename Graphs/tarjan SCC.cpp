#include <vector>
#include <stack>

struct edge
{
	int u;
};

struct node : std::vector<edge>
{
  int disc, low; ///variables used to compute SCCs
  int sccId; ///id of the scc to which this node belongs (in range [1, N])
  bool inStack;
  node(): sccId(-1), disc(-1){}
};

///finds SCCs on a connected components
void tarjanDfs(std::vector<node>&nodes, int act, int&dId, std::stack<int>&st)
{
  nodes[act].disc = nodes[act].low = dId++;
  nodes[act].inStack = true;
  st.push(act);

  for(int i=0; i<nodes[act].size(); i++)
		if(nodes[nodes[act][i].u].disc == -1)
		{
			tarjanDfs(nodes, nodes[act][i].u, dId, st);
      nodes[act].low = std::min(nodes[act].low, nodes[nodes[act][i].u].low);
		}
		else
			if(nodes[nodes[act][i].u].inStack)
				nodes[act].low = std::min(nodes[act].low, nodes[nodes[act][i].u].disc);

	if(nodes[act].disc == nodes[act].low)
	{
		int last = -1;
    while(last != act)
		{
			last = st.top();
			nodes[last].inStack = false;
			st.pop();
      nodes[last].sccId = act;
		}
	}
}

void tarjan(std::vector<node>&nodes) ///finds SCCs over all the graph
{
	for(int i=1; i<nodes.size(); i++)
	{
		nodes[i].disc = nodes[i].sccId = -1;
		nodes[i].inStack = false;
	}

	int dId = 0;
	std::stack<int> st;
	for(int i=1; i<nodes.size(); i++)
		if(nodes[i].disc == -1)
			tarjanDfs(nodes, i, dId, st);
}

///creates the dag of the strongly connected components (call tarjan first)
void buildSCCDAG(std::vector<node>&nodes, std::vector<node>&dag)
{
  for(int i=1; i<nodes.size(); i++)
		for(int j=0; j<nodes[i].size(); j++)
			if(nodes[i].sccId != nodes[nodes[i][j].u].sccId)
				dag[nodes[i].sccId].push_back({nodes[nodes[i][j].u].sccId});
}
