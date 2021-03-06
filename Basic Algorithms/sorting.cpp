#include <vector>
#include <random>
#include <ctime>

///sorts array using mergeSort (in non-decreasing order)
template <class T>
std::vector<T> mergeSort(std::vector<T>&v, int l, int r)
{
  std::vector<T> newV;
  if(l == r)
    newV.push_back(v[l]);
  else
  {
    std::vector<T>lx = mergeSort(v, l, (l+r)/2);
    std::vector<T>rx = mergeSort(v, (l+r)/2+1, r);

    int ind1 = 0, ind2 = 0;
    while(ind1 < lx.size() && ind2 != rx.size())
      if(lx[ind1] < rx[ind2])
        newV.push_back(lx[ind1++]);
      else
        newV.push_back(rx[ind2++]);

    while(ind1 < lx.size()) newV.push_back(lx[ind1++]);
    while(ind2 < rx.size()) newV.push_back(rx[ind2++]);
  }
  return newV;
}

///sorts array using mergeSort (in non-decreasing order)
template<class T>
void mergeSort(std::vector<T>&v)
{
  v = mergeSort(v, 0, v.size()-1);
}

///sorts array in O(maxVal). each element must be an integer in [0, maxVal]
void countingSort(std::vector<int>&v, int maxVal)
{
  std::vector<int>frq;
  frq.resize(maxVal+1);
  std::fill(frq.begin(), frq.end(), 0);
  for(int a : v)
    frq[a]++;

  int ind = 0;
  for(int i=0; i<=maxVal; i++)
    for(int j=0; j<frq[i]; j++)
      v[ind++] = i;
}

///sorts array made up by <key, value>. keys must be intergers in [0, maxVal]
template <class T>
void countingSort(std::vector<std::pair<int, T>>&v, int maxVal)
{
  std::vector<std::vector<T>>frq;
  frq.resize(maxVal+1);
  for(std::pair<int, T> a : v)
    frq[a.first].push_back(a.second);

  int ind = 0;
  for(int i=0; i<=maxVal; i++)
    for(T t : frq[i])
      v[ind++] = std::make_pair(i, t);
}

///sorts array using quickSort (in non-decreasing order)
template <class T>
void quickSort(std::vector<T>&v, int l, int r, std::uniform_int_distribution<unsigned int>&dis, std::mt19937&gen)
{
  if(l >= r) return;

  int pivot = v[dis(gen)%(r - l + 1) + l]; //random pivot

  int l1 = l, r1 = r;
  while(l1 < r1) //move the elements smaller than the pivot to the left and the others to the right
  {
    while(l1 <= r1 && v[l1] <= pivot) l1++;
    while(l1 <= r1 && v[r1] > pivot) r1--;
    if(l1 < r1)
    {
      T tmp = v[l1];
      v[l1] = v[r1];
      v[r1] = tmp;
    }
  }

  bool sorted = true;
  for(int i=l; i<r; i++)
    if(v[i] > v[i+1])
      sorted = false;

  if(!sorted)
  {
    quickSort(v, l, l1-1, dis, gen);
    quickSort(v, l1, r, dis, gen);
  }
}

///sorts array using quickSort (in non-decreasing order)
template<class T>
void quickSort(std::vector<T>&v)
{
  std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));
  std::uniform_int_distribution<unsigned int> dis(0, v.size()-1);
  quickSort(v, 0, v.size()-1, dis, gen);
}
