//maximum-bipartite-matching + minimum (bipartite) vertex cover
//O(NM) (indici nodi sono 0-based)

//ogni nodo ha lista d'adiacenza (indice nodi)
struct node : vector<int> {
  int match; //indice del nodo con cui c'e' il match
  bool set1; //mettila a true se il nodo e' sul set sinistro
  bool inVertexCover; //nodo sulla vertex cover?
  bool used;
  node(): set1(false), match(-1), inVertexCover(false){}
};

//trova un percorso aumentante
bool augmentingPath(vector<node>&nodes, int act) {
  nodes[act].used = true;

  if(!nodes[act].set1) //set destro
    if(nodes[act].match == -1)
      return true;
    else
      return augmentingPath(nodes, nodes[act].match);

  for(int adj : nodes[act]) //set sinistro
    if(adj != nodes[act].match && !nodes[adj].used)
      if(augmentingPath(nodes, adj)) {
        nodes[act].match = adj;
        nodes[adj].match = act;
        //NB: se arriviamo sul set sx, proveniamo dal suo match
        //con il set dx (oppure il match era -1)
        return true;
      }
  return false;
}

//maximum cardinality bipartite matching
int MCBM(vector<node>&nodes) {
  int answ = 0;
  for(int i=0; i<nodes.size(); i++)
    if(nodes[i].set1) {
      for(int j=0; j<nodes.size(); j++)
        nodes[j].used = false;
      if(augmentingPath(nodes, i))
        answ++;
    }
  return answ;
}

//utility per vertex cover, NON PER MATCHING
//consiglio: copia augmenting path e MCBM
//poi modifica: sono molto simili
void alternatingPath(vector<node>&nodes, int act) {
  nodes[act].used = true;
  if(!nodes[act].set1) {
    if(nodes[act].match != -1) {
      nodes[act].inVertexCover = true;
      alternatingPath(nodes, nodes[act].match);
    }
    return;
  }
  for(int adj : nodes[act]) //set sinistro
    if(adj != nodes[act].match && !nodes[adj].used)
      alternatingPath(nodes, adj);
}

//NB: minVertexCover e' O(NM) anche
//senza la chiamata a MCBM, quindi e'
//inutile usare dinic per trovare il matching
//in questo caso
int minVertexCover(vector<node>&nodes) {
  int d = MCBM(nodes);
  for(int i=0; i<nodes.size(); i++)
    if(nodes[i].set1 && nodes[i].match == -1) {
      for(int j=0; j<nodes.size(); j++)
        nodes[j].used = false;
      alternatingPath(nodes, i);
    }
  for(int i=0; i<nodes.size(); i++) {
    int m = nodes[i].match;
    if(nodes[i].set1 && m != -1 && !nodes[m].inVertexCover)
      nodes[i].inVertexCover = true;
  }
  return d;
}
