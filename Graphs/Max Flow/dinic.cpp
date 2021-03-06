#include <bits/stdc++.h>
using namespace std;

//T e' il tipo per il peso degli archi (e del maxflow)
//tipicamente int/long long
template<class T>
class Dinic {
  public:

  struct edge {
    int to;
    T cap, flow;
  };

  vector<edge> edges;
  int eind;

  //per ogni nodo salviamo la lista degli archi incidenti
  vector<vector<int>> nodes;
  vector<int> dists, ptr;

  Dinic(int N, int M) {
    //ridimensionare gli archi migliora performance
    //se non sappiamo M dobbiamo usare push_back in addEdge
    edges.resize(2*M);
    eind = 0;
    //i nodi possono essere indicizzati sia da 1 che da 0
    //se sono indicizzati da 1, passa |V(G)|+1 come N
    nodes.resize(N);
    dists.resize(N);
    ptr.resize(N);
  }

  //arco diretto u->v (aggiungine 2 per non-diretto)
  void addEdge(int u, int v, T cap) {
    edge d, r;
    d.to = v; d.cap = cap; d.flow = 0;
    r.to = u; r.cap = cap; r.flow = cap;
    edges[eind] = d;
    edges[eind+1] = r;
    nodes[u].push_back(eind); //i diretti sono su indici pari
    nodes[v].push_back(eind+1); //i reverse su indici dispari
    eind+=2;
  }

  bool bfs(int s, int t) {
    fill(dists.begin(), dists.end(), -1);
    dists[s] = 0;
    queue<int> q;
    q.push(s);
    while(!q.empty() && dists[t] == -1) {
      int act = q.front();
      q.pop();
      for(int ind : nodes[act]) {
        int to = edges[ind].to;
        if(dists[to] == -1 && edges[ind].cap - edges[ind].flow > 0) {
          dists[to] = dists[act] + 1;
          q.push(to);
        }
      }
    }
    return dists[t] != -1;
  }

  T dfs(int act, int t, T pushed = -1) {
    if(act == t || pushed == 0)
      return pushed;

    for(; ptr[act]<nodes[act].size(); ptr[act]++) {
      int id = nodes[act][ptr[act]];
      int to = edges[id].to;
      T topush = edges[id].cap - edges[id].flow;
      if(topush > pushed && pushed != -1) topush = pushed;
      if(topush > 0 && dists[to] == dists[act] + 1) {
        T ret = dfs(to, t, topush);
        if(ret > 0) {
          edges[id].flow += ret;
          edges[id^1].flow -= ret;
          return ret;
        }
      }
    }
    return 0;
  }

  //gira in O(N^2 * M) in generale
  //se tutti gli archi hanno peso 1, gira in O(M * sqrt(M))
  //se lo usiamo per max-bipartite matching con pesi unitari, gira in O(M * sqrt(N))
  T maxflow(int s, int t) {
    T flow = 0;
    while(bfs(s, t)) {
      T pushed;
      fill(ptr.begin(), ptr.end(), 0);
      while( (pushed = dfs(s, t)) > 0 )
        flow += pushed;
    }
    return flow;
  }
};

int main() {

  int N, M;
  scanf("%d%d", &N, &M);
  map<pair<int, int>, long long> mp;

  for(int i=0, a, b, c; i<M; i++) {
    scanf("%d%d%d", &a, &b, &c);
    if(a == b) continue;
    if(a > b) swap(a, b);
    mp[{a, b}] += c;
  }

  Dinic<long long> dinic(N+1, 2*mp.size());
  for(pair<pair<int, int>, long long> p : mp) {
    dinic.addEdge(p.first.first, p.first.second, p.second);
    dinic.addEdge(p.first.second, p.first.first, p.second);
  }
  printf("%lld", dinic.maxflow(1, N));

  return 0;
}
