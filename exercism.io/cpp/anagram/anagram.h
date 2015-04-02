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
namespace anagram {

	map<char,int> main;
	string f;
	map<char,int> create_char_histogram(string word){
		map<char,int> histogram;
		for(int i=0; i<word.size();i++)
		{
			histogram[word[/*tolower*/(i)]]++;
		}
		return	histogram;
	}

	bool isanagram(string second){
		auto s=create_char_histogram(second);
		if(second==f)return false;
		for(auto i=main.begin(),j=s.begin(); i!=main.end();i++,j++){
			 	if(*i!=*j||main.size()!=s.size()){
					return false;
				}
			}
			return true;
	}
	class anagram {
		public:
			anagram(string first){
				main = create_char_histogram(boost::to_lower_copy(first));
				f=first;
			}

			vector<string> matches( vector<string> other) {
				vector<string> anagrams;
				for(string a: other){
					if (isanagram(boost::to_lower_copy(a))){
				 		anagrams.push_back(a);
					}
				} return	anagrams;
			}

	};

}
