#include <bits/stdc++.h>
using namespace std;

template <class T>
class SparseTable {
  vector<vector<T>> table;
  vector<int> logs;
  //puoi sostituirla con altre operazioni idempotenti
  //es: max, gcd
  T min(T a, T b) {
    return a < b ? a : b;
  }
  public:
  void build(vector<T> vt) { //build sparse table in NlogN
    //precompute logs
    logs.resize(vt.size()+1);
    int act = 0;
    for(int i=0; i<=vt.size(); i++) {
      while(1<<(act+1) <= i)
        act++;
      logs[i] = act;
    }
    //build sparse table
    table.resize(logs[vt.size()] + 1);
    for(int i=0; i<table.size(); i++)
      table[i].resize(vt.size());
    for(int i=0; i<vt.size(); i++)
      table[0][i] = vt[i];
    for(int i=1; i<table.size(); i++)
      for(int j=0; j<=vt.size() - (1<<i); j++)
        table[i][j] = min(table[i-1][j], table[i-1][j + (1<<(i-1))]);
  }
  //answer RMQ in [l, r] (0-based). O(1)
  T query(int l, int r) {
    int x = logs[r - l + 1];
    return min(table[x][l], table[x][r - (1<<x) + 1]);
  }
};
