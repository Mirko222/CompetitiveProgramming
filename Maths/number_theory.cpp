#define ll long long
#define ld long double
#include <vector>
#include <cmath>
#include <algorithm>

///computes greatest common divisor O(log10(N))
ll gcd(ll a, ll b)
{
  return b == 0 ? a : gcd(b, a%b);
}

///computes least common multiple O(log10(N))
ll lcm(ll a, ll b)
{
  return a / gcd(a, b) * b;
}

///check if there are solutions to ax + by = c (integer numbers)
bool checkDiophantine(ll a, ll b, ll c)
{
  return (c % gcd(a, b)) == 0;
}

//finds integer x and y such that ax + by = gcd(a,b)
void extendedEuclid(ll a, ll b, ll&x0, ll&y0)
{
  if(b == 0)
  {
    x0 = 1;
    y0 = 0;
    return;
  }
  extendedEuclid(b, a%b, x0, y0);
  ll ty = x0 - (a / b) * y0;
  x0 = y0;
  y0 = ty;
}

///solve ax + by = c (integers)
bool solveDiophantine(ll a, ll b, ll c, ll&x0, ll&y0)
{
  if(!checkDiophantine(a, b, c)) return false;

  ll d = gcd(a, b);
  extendedEuclid(a, b, x0, y0);
  x0 *= (c / d);
  y0 *= (c / d);
  return true;
}

///returns the n-th solution of the diophantine equation
bool nthDiophantine(ll a, ll b, ll c, ll n, ll&x, ll&y)
{
  if(!solveDiophantine(a, b, c, x, y))
    return false;
  ll d = gcd(a, b);
  x += (b / d) * n;
  y -= (a / d) * n;
  return true;
}

///computes the primes in [2, N] using the sieve of eratosthenes O(N logN loglogN)
void sieve(const int N, std::vector<int>*primes, std::vector<bool>&bs)
{
  bs.resize(N+10);
  std::fill(bs.begin(), bs.end(), true);
  bs[0] = bs[1] = false;

  for(ll i=2; i<=N; i++)
    if(bs[i])
    {
      for(ll j=i*i; j<=N; j+=i)
        bs[j] = false;

      if(primes != nullptr)
        primes->push_back(i);
    }
}

///check if a number is prime (trying only odd numbers)
bool isPrime(ll N)
{
  if(N == 2) return true;
  if(N%2 == 0 || N == 1) return false;
  for(ll i=3; i*i<=N; i+=2)
    if(N%i == 0)
      return false;
  return true;
}

///check if a number is prime (trying only prime numbers)
bool isPrime(ll N, std::vector<int>&primes)
{
  if(N == 1) return false;
  for(int i=0; ((ll)primes[i]) * ((ll)primes[i]) <= N; i++)
    if(N%primes[i] == 0)
      return false;
  return true;
}

///computes A^B mod M in O(logB)
ll fastModExp(ll a, ll b, ll m)
{
  if(b == 1) return a%m;

  ll am = (b%2 == 1) ? a%m : 1;

  ll h = fastModExp(a, b/2, m);
  return (am * ((h * h) % m)) % m;
}
