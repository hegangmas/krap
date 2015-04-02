#include <map>
#include <string>
#include <algorithm>
#include <cctype>
#include <iterator>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <stdexcept>
#include <exception>
#define EXERCISM_RUN_ALL_TESTS

using namespace std;
namespace hamming
{
	int compute(string a, string b)
	{
		int counter=0;
		if(a.length()!=b.length())
			throw std::domain_error("args must be same length to calculate hamming distance");
		else
		{
			for(int i = 0; i<a.length(); i++)
			{
				if(a[i]!=b[i])
				{
					counter++;
				}
			}

		}
		return counter;
	}
}
