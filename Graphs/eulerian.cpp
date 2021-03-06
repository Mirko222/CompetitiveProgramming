//EULER-WALK
struct edge {
  int u, v;
  bool used;
  edge(int a, int b): u(a), v(b), used(false){}
};
//nella lista d'adiacenza dei nodi mettiamo l'INDICE degli archi
struct node : vector<int> {
  int index;
  node(): index(-1){}
};
//gli archi sono indicizzati da 0 (puoi indicizzare i nodi come vuoi)
vector<int> walk;
void eulerWalk(vector<node>&nodes,vector<edge>&edges, int actNode, int edgeFrom = -1) {
  ++nodes[actNode].index; //utilizzare un indice esterno velocizza parecchio le cose
  for(; nodes[actNode].index<nodes[actNode].size(); nodes[actNode].index++) {
    int adj = nodes[actNode][nodes[actNode].index];
    if(!edges[adj].used) {
      int neigh = edges[adj].u == actNode ? edges[adj].v : edges[adj].u;
      edges[adj].used = true;
      eulerWalk(nodes, edges, neigh, adj);
    }
  }
  //se vuoi la lista degli archi invece che dei nodi:
  //if(edgeFrom != -1) walk.push_back(edgeFrom);
  walk.push_back(actNode);
}
