#include <string>
#include <vector>

/**
compute failure function of str.
ff[i] is the largest suffix of str[0, i] which is also a prefix of str[0, i]
*/
void failureFunc(std::string&str, int*ff)
{
	ff[0] = 0;
  int ind = 0;
  for(int i=1; i<str.length(); i++)
	{
		while(ind != 0 && str[i] != str[ind])
			ind = ff[ind - 1];

		if(str[i] == str[ind])
			ind++;

		ff[i] = ind;
	}
}

///finds the number of occurrences of the pattern into the string, gives the positions too (0-based)
int kmp(std::string&str, std::string&pattern, std::vector<int>*positions)
{
  int ff[pattern.length()];
  failureFunc(pattern, ff);

	int ind = 0, answ = 0;
	for(int i=0; i<str.length(); i++)
	{
		while(ind != 0 && str[i] != pattern[ind])
			ind = ff[ind-1];

    if(str[i] == pattern[ind])
			ind++;

    if(ind == pattern.length())
		{
			if(positions != nullptr)
				positions->push_back(i - pattern.length() + 1);
			answ++;
			ind = ff[ind-1];
		}
	}

	return answ;
}
