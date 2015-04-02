#include <iostream>
#include <string>         // std::string
#include <locale>         // std::locale, std::isupper, std::tolower
#include <algorithm>
#include "bob.h"
int main(int avgc, char** avgv)
{
	bob b;
	//cout<<b.hey("Tom-ay-to, tom-aaaah-to.")<<endl;

    cout<<"Whatever."<<b.hey("Tom-ay-to, tom-aaaah-to.")<<endl;

    cout<<"Whoa, chill out!"<<b.hey("WATCH OUT!")<<endl;

    cout<<"Sure."<<b.hey("Does this cryogenic chamber make me look fat?")<<endl;

    cout<<"Whatever."<<b.hey("Let's go make out behind the gym!")<<endl;

    cout<<"Whatever."<<b.hey("It's OK if you don't want to go to the DMV.")<<endl;

    cout<<"Whoa, chill out!"<<b.hey("WHAT THE HELL WERE YOU THINKING?")<<endl;

   cout<<"Whoa, chill out!"<<b.hey("1, 2, 3 GO!")<<endl;

    cout<<"Whatever."<<b.hey("1, 2, 3")<<endl;

    cout<<"Sure."<<b.hey("4?")<<endl;

    cout<<"Whoa, chill out!"<<b.hey("ZOMG THE %^*@#$(*^ ZOMBIES ARE COMING!!11!!1!")<<endl;
    cout<<"Whoa, chill out!"<<b.hey("I HATE YOU")<<endl;

    cout<<"Whatever."<<b.hey("Ending with a ? means a question.")<<endl;

    cout<<"Sure."<<b.hey("Wait! Hang on.  Are you going to be OK?")<<endl;

    cout<<"Fine. Be that way!"<<b.hey("")<<endl;

    cout<<"Fine. Be that way!"<<b.hey("   ")<<endl;
    
 	return 0;
} 
