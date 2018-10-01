//
// Created by weichien on 30/09/2018.
//

using namespace std;


struct Document;

/* Having Interfaces segregated */

struct IPrinter
{
    virtual void print(Document &doc) = 0;
};

struct IScanner
{
    virtual void scan(Document &doc) = 0;
};

struct IFax
{
    virtual void fax(Document &doc) = 0;
};

struct Printer : IPrinter {};

struct Scanner : IScanner {};

struct IMachine : IPrinter, IScanner {};

struct Machine : IMachine
{
    IPrinter& printer;
    IScanner& scanner;

    Machine(IPrinter &printer, IScanner &scanner) :
            printer(printer), scanner(scanner) {}

    void print(Document &doc) override {
        printer.print(doc);
    }

    void scan(Document &doc) override {
        scanner.scan(doc);
    }
};


int main()
{
    return 0;
}

//struct IMachine
//{
//    virtual void print(Document &doc) = 0;
//    virtual void scan(Document &doc) = 0;
//    virtual void fax(Document &doc) = 0;
//};
//
//struct MFP : IMachine
//{
//    void print(Document &doc) override {
//        // DO
//    }
//
//    void scan(Document &doc) override {
//
//    }
//
//    void fax(Document &doc) override {
//
//    }
//};
//
//struct Scanner : IMachine
//{
//    void print(Document &doc) override {
//
//    }
//
//    void scan(Document &doc) override {
//        // DO
//    }
//
//    void fax(Document &doc) override {
//
//    }
//};
