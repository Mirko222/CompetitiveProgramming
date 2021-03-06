class SuffixArray {
  public:
  vector<int> SA, LCP;
  string str;

  struct SANode {
    int ind;
    int rnk[2];
  };

  static bool cmp(const SANode&a, const SANode&b) {
    if(a.rnk[0] == b.rnk[0])
     return a.rnk[1] < b.rnk[1];
    return a.rnk[0] < b.rnk[0];
  }

  //O(n lg^2(n))
  void buildSA(string&s) {
    str = s;
    int N = s.length();
    SA.resize(N);
    vector<SANode> sa;
    sa.resize(N);
    vector<int> inv;
    inv.resize(N);
    for(int i=0; i<N; i++) {
      sa[i].ind = i;
      inv[i] = i;
      sa[i].rnk[0] = s[i];
      sa[i].rnk[1] = i+1 == N ? -1 : s[i+1];
    }
    sort(sa.begin(), sa.end(), cmp);
    //usa radix-sort invece di sort e diventa O(nlgn) [piu' lento in pratica]
    for(int t=2; t<N; t*=2) {
      int r1 = 0;
      int pr1 = -2, pr2 = -2;
      for(int i=0; i<N; i++) {
        int tmp = sa[i].rnk[0];
        sa[i].rnk[0] = (sa[i].rnk[0] != pr1 || sa[i].rnk[1] != pr2) ? ++r1 : r1;
        pr1 = tmp;
        pr2 = sa[i].rnk[1];
        inv[sa[i].ind] = i;
      }
      if(r1 == N) break;
      for(int i=0; i<N; i++)
        sa[i].rnk[1] = sa[i].ind + t >= N ? -1 : sa[inv[sa[i].ind + t]].rnk[0];
      sort(sa.begin(), sa.end(), cmp);
    }
    for(int i=0; i<N; i++)
     SA[i] = sa[i].ind;
    //INV[i] = posizione i-esimo suffisso nel suffix array
    //for(int i=0; i<N; i++)
    // INV[sa[i].ind] = i;
  }

  void buildLCP() { //call buildSA first
    LCP.resize(SA.size());
    int L = 0;
    int N = str.length();
    vector<int> p, PLCP;
    p.resize(N);
    PLCP.resize(N);
    p[SA[N-1]] = -1;
    for(int i=0; i<N-1; i++)
      p[SA[i]] = SA[i+1];
    for(int i=0; i<N; i++) {
      if(p[i] == -1) {
        PLCP[i] = -1;
        continue;
      }
      while(i + L < N && p[i] + L < N && str[i + L] == str[p[i] + L])
        L++;
      PLCP[i] = L;
      L = max(L-1, 0);
    }
    for(int i=0; i<N; i++)
      LCP[i] = PLCP[SA[i]];
  }
  
  void debugPrint() {
    cout<<"----SA----\n";
    for(int i=0; i<SA.size(); i++) {
      for(int j=SA[i]; j<str.length(); j++)
        cout<<str[j];
      if(LCP.size() > 0) cout<<" "<<LCP[i];
      cout<<"\n";
    }
    cout<<"-----\n";
  }
};
