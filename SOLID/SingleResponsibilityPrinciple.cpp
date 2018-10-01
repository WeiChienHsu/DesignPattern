//
// Created by weichien on 29/09/2018.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
using namespace std;


struct Journal {
    string title;
    vector<string> entries;

    explicit Journal(const string &title) : title(title) {}

    void add_entry(const string &entry)
    {
        static int count = 1;
        entries.push_back(boost::lexical_cast<string>(count++) + ":" + entry);

    }

    /* Wrong Way to save: Adding a persistent code into file */
    /* If we would like to change the way we save the strings */
    /* we need to go through all methods to change every implementation */
    void save(const string& filename)
    {
        ofstream ofs(filename);
        for(auto &e : entries) {
            ofs << e << endl;
        }
    }
};

/* Separation of concern: to implement persistence */
struct PersistenceManager
{
    /* We just need to refactor persistence and could change */
    /* to way we want to save the contents to database       */
    static void save(const Journal& j, const string& filename)
    {
        ofstream ofs(filename);
        for(auto &e : j.entries) {
            ofs << e << endl;
        }
    }
};


int main() {

    Journal journal{"Dear Diary"};
    journal.add_entry("I ate a bug.");
    journal.add_entry("I cried today");

    journal.save("diary.txt");

    PersistenceManager pm;
    pm.save(journal, "diary.txt");

    return 0;
}

