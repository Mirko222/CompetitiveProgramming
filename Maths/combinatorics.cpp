#include <vector>
#define ll long long
#define INF 9223372036854775807

/*C(N, K) : number of ways to take K elements from a set of N (without repetitions)
* IL VETTORE DELLA DP DEVE ESSERE INIZIALIZZATO A  -1 */
ll binomial(ll N, ll K, std::vector<std::vector<ll>>&dp, ll MOD = INF)
{
  if(K == 0 || N == K) return 1LL;
  if(dp[N][K] == -1)
    dp[N][K] = (binomial(N-1, K, dp, MOD) + binomial(N-1, K-1, dp, MOD))%MOD;
  return dp[N][K];
}
