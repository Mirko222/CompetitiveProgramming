#include <bits/stdc++.h>
using namespace std;

template<class T> //int o long long?
class FenwickTree {
  #define LSOne(S) (S & (-S)) //primo bit a 1 da destra
  vector<T> ft; //USES 1-BASED INDEX!!!
  public: //tutte le funzioni sono O(lg(n))
    FenwickTree(int n) {
      ft.assign(n + 1, 0);
    }
    T rsq(int b) { //returns RSQ[1, b]
      T sum = 0;
      for(; b; b -= LSOne(b))
        sum += ft[b];
      return sum ;
    }
    T rsq(int a, int b) { //returns RSQ[a, b]
      return rsq(b) - (a == 1 ? 0 : rsq(a - 1));
    }
    //adjusts value of the k-th element by v
    void adjust(int k, T v) {//note:n=ft.size()-1
      for(; k < (int)ft.size(); k += LSOne(k))
        ft[k] += v;
    }
    //restituisce il primo indice (in [1, N])
    //tale che rsq(indice) >= v (rsq deve
    //essere non-decrescente)
    //NB: se rsq(i)<v per ogni i, restituisce N+1
    int lower_bound(T v) {
      int lgn = (int)(log(ft.size())/log(2));
      int ind = 0;
      T sum = 0;
      for(int i=lgn; i>=0; i--) {
        int p = ind + (1<<i);
        //metti <= v e diventa upper_bound
        if(p < ft.size() && sum + ft[p] < v) {
          sum += ft[p];
          ind = p;
        }
      }
      return ind+1;
    }
};
// kD: (eg 2D BIT ):
// update (x, y, val): //aggiunge val a elemento x,y
// for(; x<(int)ft.size(); x += LSOne(x))
//   for(y2=y; y2<(int)ft[0].size(); y2 += LSOne(y2))
//      ft[x][y2] += val;
// query analoga (ritorna rsq da 1,1 a x,y)
