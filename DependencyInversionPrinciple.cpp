//
// Created by weichien on 30/09/2018.
//

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <tuple>
using namespace std;


enum class Relationship
{
    parent,
    child,
    sibling
};

struct Person
{
    string name;
};

struct RelationshipBrowser
{
    virtual vector<Person> find_all_children_of(const string& name) = 0;
};

struct Relationships : RelationshipBrowser /* Low Level Modules */
{
    vector<tuple<Person, Relationship, Person>> relations;
    void add_parent_and_child(const Person& parent, const Person& child)
    {
        relations.push_back({parent, Relationship :: parent, child});
        relations.push_back({child, Relationship :: child, parent});
    }

    /* Implement the RelationshipBrowser */
    vector<Person> find_all_children_of(const string &name) override {
        vector<Person> result;
        for(auto &&[first, rel, second] : relations)
        {
            if(first.name == name && rel == Relationship::parent)
            {
                result.push_back(second);
            }
        }
        return result;
    }
};


struct Research /* Have dependency on abstraction RelationshipBrowser */
{
    Research(RelationshipBrowser& browser)
    {
        for(auto& child : browser.find_all_children_of("John"))
        {
            cout << "John has a child called " << child.name << endl;
        }
    }
};

//struct Research /* High Level Modules */
//{
//  Research(Relationships& relationships)
//  {
//      auto& relations = relationships.relations;
//      for(auto &&[first, rel, second] : relations)
//      {
//          if(first.name == "John" && rel == Relationship::parent)
//          {
//            cout << "John has a child called" << second.name << endl;
//          }
//      }
//  }
//};

int main() {
    Person parent{"John"};
    Person child1{"Chris"}, child2{"Matt"};
    Relationships rs;
    rs.add_parent_and_child(parent, child1);
    rs.add_parent_and_child(parent, child2);

    Research _(rs);
    return 0;
}