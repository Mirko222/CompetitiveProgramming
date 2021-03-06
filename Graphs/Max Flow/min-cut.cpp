#include "edmonds-karp.cpp"
using namespace std;

ll mcDfs(vector<node>&nodes, vector<bool>&used, int act, vector<pair<int, int>>*cutEdges)
{
  used[act] = true;
  ll answ = 0;
  for(pair<int, ll> adj : nodes[act].forw)
    if(adj.w != 0 && !used[adj.u])
      answ += mcDfs(nodes, used, adj.u, cutEdges);

  for(pair<int, ll> adj : nodes[act].forw)
    if(adj.w == 0 && !used[adj.u])
    {
      answ += (nodes[adj.u].back[act] / 2LL);
      if(cutEdges != nullptr)
        cutEdges->push_back({act, adj.u});
    }
  return answ;
}

ll minCut(vector<node>&nodes, int si, int so, vector<pair<int, int>>*cutEdges)
{
  edmondsKarp(nodes, si, so);
  vector<bool> used;
  used.resize(nodes.size());
  for(int i=0; i<nodes.size(); i++)
    used[i] = false;
  return mcDfs(nodes, used, si, cutEdges);
}
