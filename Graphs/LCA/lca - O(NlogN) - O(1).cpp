#include <vector>

struct edge ///edge of the tree (add here attributes)
{
  int u;
};

struct node : std::vector<edge> ///node of the tree
{
	int index, depth;
};

class LCA_1 ///class for LCA
{
	std::vector<int> dfs_order;
	std::vector<int> logs;
	std::vector<std::vector<int>> sparse_table;

	public:

	void dfs(std::vector<node>&nodes, int act, int from = -1)
	{
		for(int i=0; i<nodes[act].size(); i++)
			if(nodes[act][i].u != from)
			{
				dfs_order.push_back(act);
				nodes[nodes[act][i].u].depth = nodes[act].depth + 1;
				dfs(nodes, nodes[act][i].u, act);
			}
		dfs_order.push_back(act);
	}

	///preprocess O(NlogN)
	void preprocessing(std::vector<node>&nodes, int root)
	{
		sparse_table.clear();
		logs.clear();
		dfs_order.clear();
		for(int i=0; i<nodes.size(); i++) nodes[i].index = -1;

		//build the dfs-order
		nodes[root].depth = 0;
		dfs(nodes, root);
		for(int i=0; i<dfs_order.size(); i++)
			if(nodes[dfs_order[i]].index == -1)
				nodes[dfs_order[i]].index = i;

		//precompute logs
		int logn = 0;
		logs.resize(dfs_order.size()+1);
		logs[0] = 0;
    for(int i=1; i<logs.size(); i++)
		{
			logs[i] = logs[i-1];
			while((1<<logn) < i)
			{
				logs[i] = logn;
				logn++;
			}
		}

		//build min-query sparse table over the dfs-order
    sparse_table.resize(dfs_order.size());
    for(int i=0; i<sparse_table.size(); i++)
			sparse_table[i].resize(logn+1);

		for(int i=0; i<sparse_table.size(); i++)
			sparse_table[i][0] = dfs_order[i];

		for(int i=1; i<logn; i++)
			for(int j=0; j<sparse_table.size() - (1<<i) + 1; j++)
			{
        int hl = (1<<i) / 2;
        if(nodes[sparse_table[j][i-1]].depth < nodes[sparse_table[j + hl][i-1]].depth)
					sparse_table[j][i] = sparse_table[j][i-1];
				else
					sparse_table[j][i] = sparse_table[j + hl][i-1];
			}
	}

	///lca query O(1)
	int query(std::vector<node>&nodes, int a, int b)
	{
    a = nodes[a].index;
    b = nodes[b].index;
    if(a > b) std::swap(a, b);

    int logn = logs[b - a + 1];
    int l = (1<<logn) - 1;
    if(nodes[sparse_table[a][logn]].depth < nodes[sparse_table[b - l][logn]].depth)
			return sparse_table[a][logn];
    return sparse_table[b - l][logn];
	}
};
