//
// Created by weichien on 30/09/2018.
//

using namespace std;
using namespace boost;

struct Document;

struct IMachine
{
    virtual void print(Document &doc) = 0;
    virtual void scan(Document &doc) = 0;
    virtual void fax(Document &doc) = 0;
};

struct MFP : IMachine
{
    void print(Document &doc) override {
        // DO
    }

    void scan(Document &doc) override {

    }

    void fax(Document &doc) override {

    }
};

struct Scanner : IMachine
{
    void print(Document &doc) override {

    }

    void scan(Document &doc) override {
        // DO
    }

    void fax(Document &doc) override {

    }
};

int main()
{
    return 0;
}