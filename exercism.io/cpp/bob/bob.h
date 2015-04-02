#include <iostream>
#include <string> 
#include <locale>         
#include <algorithm>
#define EXERCISM_RUN_ALL_TESTS
using namespace std;


class bob{
	public:
	bob(void){};
	
	string hey(string say);
	
};
	
string bob::hey(string say){
	
	if(say.length()==0||std::all_of( std::begin( say ), std::end( say ), []( char c ) { return isblank(c)||isspace(c); } ))
		return "Fine. Be that way!";
	if((std::all_of( std::begin( say ), std::end( say ), []( char c ) { return ( isblank(c)||isdigit(c)||ispunct(c)); } ))&&say.back()!='?')
			return  "Whatever.";

	if(say.back()=='?')	
		{if(!std::any_of( std::begin( say ), std::end( say )-1, []( char c ) { return ( islower(c)||isdigit(c)); } ))
			return "Whoa, chill out!";
		else if(std::all_of( std::begin( say ), std::end( say )-1, []( char c ) { return ( islower(c)||isdigit(c)||ispunct(c)); } ))
			return "Sure.";
		else return "Sure.";}
	if(!std::any_of( std::begin( say ), std::end( say ), []( char c ) { return ( islower(c)); } ))
			return  "Whoa, chill out!";

	return "Whatever.";
}
	
