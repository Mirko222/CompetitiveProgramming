#include <unordered_map>
#include <queue>
#include <vector>
#define ll long long
#define u first
#define w second
#define INF 2999999999
using namespace std;

struct node
{
  unordered_map<int, ll> forw, back; //forward edges and backword edges
  int from;
  ll cap;
};

//function to add an edge to the graph
void addEdge(vector<node>&nodes, int a, int b, ll c, bool direct = true)
{
  if(a != b)
  {
    nodes[a].forw[b] += c;
    nodes[b].back[a] = 0;
    if(!direct)
    {
      nodes[b].forw[a] += c;
      nodes[a].back[b] = 0;
    }
  }
}

//function to relax edges in bfs
void relax(vector<node>&nodes, queue<int>&Q, int act, int u, ll w)
{
  if(nodes[u].from == -1 && w > 0)
  {
    nodes[u].from = act;
    Q.push(u);
    nodes[u].cap = min(w, nodes[act].cap);
  }
}

//computes max flow of the graph using edmondsKarp O(NM^2)
ll edmondsKarp(vector<node>&nodes, int so, int si)
{
  queue<int> Q;
  ll maxFlow = 0;
  bool aug = true; //can we find an augmenting path?

  while(aug) //while we can find an augmenting path
  {
    for(int i=1; i<nodes.size(); i++)
      nodes[i].from = nodes[i].cap = -1;

    nodes[so].from = 0;
    nodes[so].cap = INF;
    Q.push(so);

    while(!Q.empty()) //finds the shortest path from source to sink
    {
      int act = Q.front();
      Q.pop();
      for(pair<int, ll> adj : nodes[act].forw)
        relax(nodes, Q, act, adj.u, adj.w);
      for(pair<int, ll> adj : nodes[act].back)
        relax(nodes, Q, act, adj.u, adj.w);
    }

    if(nodes[si].cap <= 0) //can't find augmenting path
      aug = false;
    else //path found
    {
      maxFlow += nodes[si].cap; //update max flow
      int act = si;
      while(act != 0) //update all the edges on the path
      {
        int from = nodes[act].from;
        if(nodes[from].forw.find(act) != nodes[from].forw.end() && nodes[from].forw[act] >= nodes[si].cap) //forward edge used
        {
          nodes[from].forw[act] -= nodes[si].cap;
          nodes[act].back[from] += nodes[si].cap;
        }
        else //backword edge used
        {
          nodes[from].back[act] -= nodes[si].cap;
          nodes[act].forw[from] += nodes[si].cap;
        }
        act = from;
      }
    }
  }

  return maxFlow;
}
