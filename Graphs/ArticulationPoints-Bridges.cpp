#include <vector>
#include <algorithm>

struct edge
{
  int u, v;
  bool bridge;
  edge(int x, int y): u(x), v(y), bridge(false){}
};

struct node : std::vector<int>
{
  int disc, low;
  bool used;
  bool artPoint;
  node(): artPoint(false), used(false){}
};

//finds bridges and articulation points
void findArtPointsBridges(std::vector<node>&nodes, std::vector<edge>&edges, int act, int&ind, int root, int from = -1)
{
  nodes[act].disc = nodes[act].low = ind++;
  nodes[act].used = true;
  int children = 0;

  for(int i=0; i<nodes[act].size(); i++)
  {
    int neigh = edges[nodes[act][i]].u == act ? edges[nodes[act][i]].v : edges[nodes[act][i]].u;
    if(neigh != from)
    {
      if(!nodes[neigh].used)
      {
        children++;
        findArtPointsBridges(nodes, edges, neigh, ind, root, act);
        nodes[act].low = std::min(nodes[act].low, nodes[neigh].low);

        if(nodes[act].disc < nodes[neigh].low)
          edges[nodes[act][i]].bridge = true;
        if((act == root && children > 1) || (act != root && nodes[act].disc <= nodes[neigh].low))
          nodes[act].artPoint = true;
      }
      else
        nodes[act].low = std::min(nodes[act].low, nodes[neigh].disc);
    }
  }
}
