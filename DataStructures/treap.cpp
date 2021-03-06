#include <bits/stdc++.h>
using namespace std;

//tutte le operazioni sono O(lg(N)) con alta probabilita'
//operatori == e < devono essere definiti su T
template<class T>
class Treap {
  static const int INF = 2000000000; //piu' alto e' meglio e'
  mt19937 gen;
  uniform_int_distribution<int> distr;

  public:

  struct node {
    node *l, *r;
    int size, prty;
    T elem;
    node(T e, int p) {
      l = r = nullptr;
      size = 1;
      elem = e;
      prty = p;
    }

    int getLeftSize() {
      return l == nullptr ? 0 : l->size;
    }

    int getRightSize() {
      return r == nullptr ? 0 : r->size;
    }
  };
  node *root;

  Treap() : gen(chrono::steady_clock::now().time_since_epoch().count()), distr(0, INF) {
    root = nullptr;
  }

  //calcola dimensione sottoalbero radicato in x
  //tutti gli altri sottoalberi devono avere size calcolata opportunamente
  void computeSize(node*x) {
    int d1 = x->l == nullptr ? 0 : x->l->size;
    int d2 = x->r == nullptr ? 0 : x->r->size;
    x->size = d1 + d2 + 1; //x compreso
  }

  //sistema proprieta' heap, assumento che l'unica violazione
  //sia che un figlio di x sia piu' grande di x (ma non di parent(x))
  node* fixHeap(node*act) {
    if(act->r != nullptr && act->r->prty > act->prty)
      act = leftRot(act);
    if(act->l != nullptr && act->l->prty > act->prty)
      act = rightRot(act);
    return act;
  }

  //rotazione a destra albero radicato in x
  //restituisce nuova radice
  node*rightRot(node*x) {
    if(x->l == nullptr) return x;
    node*t = x->l;
    x->l = t->r;
    t->r = x;
    computeSize(x);
    computeSize(t);
    return t;
  }

  //rotazione a sinistra albero radicato in x
  //restituisce nuova radice
  node*leftRot(node*x) {
    if(x->r == nullptr) return x;
    node*t = x->r;
    x->r = t->l;
    t->l = x;
    computeSize(x);
    computeSize(t);
    return t;
  }

  //trova v in act (serve proprieta' bst)
  node*find(T v, node*act) {
    if(act == nullptr || act->elem == v)
      return act;
    if(act->elem < v)
      return find(v, act->r);
    return find(v, act->l);
  }

  node*find(T v) {
    return find(v, root);
  }

  bool exists(T v) {
    return find(v) != nullptr;
  }

  //inserisce nuovo nodo in sottoalbero radicato in act (restituisce nuova radice)
  node*insert(T v, node*act) {
    if(act == nullptr)
      return new node(v, distr(gen));
    if(act->elem < v)
      act->r = insert(v, act->r);
    else
      act->l = insert(v, act->l);
    computeSize(act);
    act = fixHeap(act);
    return act;
  }

  void insert(T v) {
    root = insert(v, root);
  }

  //rimuove v da sottoalbero radicato in act (restituisce nuova radice)
  node*remove(T v, node*act) {
    if(act == nullptr) return nullptr;
    if(act->elem == v) {
      if(act->l == nullptr && act->r == nullptr) {
        delete act;
        return nullptr;
      }
      int pl = act->l == nullptr ? -1 : act->l->prty;
      int pr = act->r == nullptr ? -1 : act->r->prty;
      if(pl > pr) {
        act = rightRot(act);
        act->r = remove(v, act->r);
      }
      else {
        act = leftRot(act);
        act->l = remove(v, act->l);
      }
    }
    else
      if(act->elem < v)
        act->r = remove(v, act->r);
      else
        act->l = remove(v, act->l);
    computeSize(act);
    return act;
  }

  void remove(T v) {
    root = remove(v, root);
  }

  //cerca nodo a una certa posizione (0-based)
  //se lo si usa quando si ha proprieta' bst,
  //restituisce l'index-piu' piccolo elemento inserito
  node*findAt(int index, node*act) {
    if(act == nullptr) return nullptr;
    int ls = act->l == nullptr ? 0 : act->l->size;
    if(ls == index) return act;
    if(ls < index)
      return findAt(index-ls-1, act->r);
    return findAt(index, act->l);
  }

  node*findAt(int index) {
    return findAt(index, root);
  }

  //inserisce nodo a un certo indice (0-based) (restituisce nuova radice)
  //NB: se index<0 e' considerato 0, se index>size() e' considerato size()
  //dopo quest'inserimento le proprieta' del bst saranno perse
  //tutti gli elementi in [index, end] vengono spostati di 1 a destra, dopo l'inserimento
  node*insert(T v, int index, node*act) {
    if(act == nullptr)
      return new node(v, distr(gen));
    int ls = act->l == nullptr ? 0 : act->l->size;
    if(ls < index)
      act->r = insert(v, index-ls-1, act->r);
    else
      act->l = insert(v, index, act->l);
    computeSize(act);
    act = fixHeap(act);
    return act;
  }

  void insert(T v, int index) {
    root = insert(v, index, root);
  }

  //rimuove nodo a index (0-based)
  //NB: se index<0 e' considerato 0, se index>size() e' considerato size()
  node*removeAt(int index, node*act) {
    if(act == nullptr) return nullptr;
    int ls = act->getLeftSize();
    if(ls == index) {
      if(act->l == nullptr && act->r == nullptr) {
        delete act;
        return nullptr;
      }
      int pl = act->l == nullptr ? -1 : act->l->prty;
      int pr = act->r == nullptr ? -1 : act->r->prty;
      if(pl > pr) {
        act = rightRot(act);
        act->r = removeAt(act->r->getLeftSize(), act->r);
      }
      else {
        act = leftRot(act);
        act->l = removeAt(act->l->getLeftSize(), act->l);
      }
      computeSize(act);
      return act;
    }
    if(ls < index)
      act->r = removeAt(index-ls-1, act->r);
    else
      act->l = removeAt(index, act->l);
    computeSize(act);
    return act;
  }

  void removeAt(int index) {
    root = removeAt(index, root);
  }

  //visita inordine
  void inorder(vector<T>&path, node*act) {
    if(act == nullptr) return;
    inorder(path, act->l);
    path.push_back(act->elem);
    inorder(path, act->r);
  }

  //visia inordine (se c'e' proprieta' bst e' la lista ordinata)
  vector<T> inorder() {
    vector<T> vt;
    inorder(vt, root);
    return vt;
  }

  void print(node*act) {
    if(act == nullptr) return;
    cout<<"(v:"<<act->elem<<" ; p:"<<act->prty<<" ; s:"<<act->size<<")\n";
    print(act->l);
    print(act->r);
  }

  void print() {
    cout<<"-----\n";
    print(root);
    cout<<"-----\n";
  }

  void printInorder() {
    for(T a : inorder())
      cout<<a<<" ";
    cout<<"\n";
  }

  int size() {
    return root == nullptr ? 0 : root->size;
  }

  //elemento massimo (con bst prop.)
  T maxElem() {
    node*ite = root;
    while(ite->r != nullptr)
      ite = ite->r;
    return ite->elem;
  }

  //divide la treap in 2 treap: la prima con tutti elementi <= elem
  //l'altra con tutti elementi > elem (serve BST property)
  //la treap attuale va distrutta
  pair<Treap<T>, Treap<T>> split(T elem, node*act) {
    if(act == nullptr)
      return {Treap<T>(), Treap<T>()};
    pair<Treap<T>, Treap<T>> ret;
    if(act->elem <= elem) {
      ret = split(elem, act->r);
      act->r = ret.first.root;
      computeSize(act);
      ret.first.root = act;
    } else {
      ret = split(elem, act->l);
      act->l = ret.second.root;
      computeSize(act);
      ret.second.root = act;
    }
    return ret;
  }

  pair<Treap<T>, Treap<T>> split(T elem) {
    return split(elem, root);
  }

  //unisce questa treap con l'altra
  //il massimo elemento di una treap, deve essere <= del minimo
  //elemento dell'altra treap (serve BST property)
  //other va distrutta
  void merge(Treap<T>&other) {
    if(other.root == nullptr) return;
    if(root == nullptr) {
      root = other.root;
      return;
    }
    node*n;
    if(root->elem < other.root->elem) {
      //this < other
      n = new node(maxElem(), INF);
      n->l = root;
      n->r = other.root;
    } else {
      n = new node(other.maxElem(), INF);
      n->l = other.root;
      n->r = root;
    }
    root = n;
    computeSize(root);
    remove(root->elem);
  }
};
