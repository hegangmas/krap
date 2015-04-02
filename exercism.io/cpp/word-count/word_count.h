#include <map>
#include <string>
#include <algorithm>
#include <cctype>
#include <iterator>
#include <vector>
#include <boost/algorithm/string.hpp>
#define EXERCISM_RUN_ALL_TESTS

using namespace std;
namespace
{

    string normalize(string const& text)
    {
        string normalized;
        transform(text.begin(), text.end(), back_inserter(normalized),[](const char c){return (isalnum(c) || c == '\'') ? tolower(c) : ' ';});
        return normalized;
    }

    string trim_word(string const& word)
    {
        return boost::trim_copy_if(word, boost::is_any_of("' "));
    }

    vector<string> extract_words(string const& text)
    {
        //make vector of words
        vector<string> words;
        //split text to words based on spaces and tabs
        boost::split(words, text, boost::is_any_of(" \t"));
        //take words from vector and remove spaces and apostrofs(trimm it)
        transform(words.begin(), words.end(), words.begin(), trim_word);
        return words;
    }
}

namespace word_count
{
    //our database for counting words
    std::map<std::string, int> words(std::string const& text)
    {
        //make map for counting words
        map<string, int> word_counter;
        //look at all words and build map acordingly
        for (auto const& word : extract_words(normalize(text)))
            if(!word.empty())word_counter[word]++;
        return word_counter;
    }
}
