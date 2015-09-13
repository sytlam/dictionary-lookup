// Dictionary.cpp

#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <algorithm>  // for swap, sort, and is_permutation
#include <vector>
using namespace std;

const int TABLE_SIZE = 50000; // 50000 buckets in the hash table

void removeNonLetters(string& s);

class DictionaryImpl
{
  public:
    DictionaryImpl() {}
    ~DictionaryImpl() {}
    void insert(string word);
    void lookup(string letters, void callback(string)) const;
  private:
    unsigned long hashFunc(string& name) const;
    struct Node
    {
        string orderedWord; // holds the word rearranged in alphabetical order (ex dog becomes dgo)
        vector<string> permutations; // holds all the permutations of orderedWord
    };
    vector<Node> table[TABLE_SIZE]; // hash table with 50000 buckets
};

unsigned long DictionaryImpl::hashFunc(string& name) const
{
    int total = 0;
    for (int i = 0; i < name.length(); i++)
    {
        total = total+(i+1)*name[i]; // create a uniform distribution of words into buckets
    }
    total = total % TABLE_SIZE;
    return total;
}

void DictionaryImpl::insert(string word) // inserts a word into the dictionary
{
    removeNonLetters(word);
    string orderedWord = word;
    sort(orderedWord.begin(), orderedWord.end()); // sort the word in ascending order so all permutations of the word are put in the same bucket
    
    if ( ! word.empty())
    {
        unsigned long key = hashFunc(orderedWord); // get the location of where to put the word using hash function
        for (int i = 0; i < table[key].size(); i++) // go through all the nodes in that bucket in the hash table
        {
            if (table[key].at(i).orderedWord == orderedWord) // orderedWord has already been encountered
            {
                table[key].at(i).permutations.push_back(word); // add the permutation to the list that has the same orderedWord
                return;
            }
        }
        
        // orderedWord has not been encountered before, so make a new node for it and add the word to the list of permutations
        Node newNode = Node();
        newNode.orderedWord = orderedWord;
        newNode.permutations.push_back(word);
        table[key].push_back(newNode);
    }
}

void DictionaryImpl::lookup(string letters, void callback(string)) const // find all the permutations of letters in the dictionary
{
    if (callback == nullptr)
        return;

    removeNonLetters(letters);
    if (letters.empty())
        return;
    
    string orderedWord = letters;
    sort(orderedWord.begin(), orderedWord.end()); // sort the word in ascending order so all permutations of the word are put in the same bucket
    unsigned long key = hashFunc(orderedWord); // get which bucket the word is in

    for (vector<Node>::const_iterator i = table[key].begin(); i != table[key].end(); i++)
    {
        if (i->orderedWord == orderedWord) // found the word rearranged in alphabetical order
        {
            for (int j = 0; j < i->permutations.size(); j++) // call callback on all its permutations
            {
                callback(i->permutations.at(j));
            }
            return;
        }
    }
}

void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop off everything from "to" to end.
}

//******************** Dictionary functions **********************************

// These functions simply delegate to DictionaryImpl's functions.
// You probably don't want to change any of this code.

Dictionary::Dictionary()
{
    m_impl = new DictionaryImpl;
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters, callback);
}
