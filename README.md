# Design Pattern in C++

`
Common architectural approaches.
`


<strong>[SOLID Design Principle](#solid-design-principle)</strong>


<strong>[Creational Patterns](#creational-patterns)</strong>


<strong>[Structural Patterns](#structural-patterns)</strong>


<strong>[Behavioral Patterns](#behavioral-patterns)</strong>

<strong>[Reference Credits](#reference-credits)</strong>

***


## SOLID Design Principle

<strong>[Single Responsibility Principle](#single-responsibility-principle)</strong>

- A class should only have one reason to change.
- Separation of concerns - different classes handling different, independent tasks/problems.


<strong>[Open Closed Principle](#open-closed-principle)</strong>

- Classes should be open for extension but closed for modification.


<strong>[Liskov Substitution Principle](#liskov-substitution-principle)</strong>

- You should be able to substitute a base type for a subtype.


<strong>[Interface Segregation Principle](#interface-segregation-principle)</strong>

- Don't put too much into an interface; split into separate interfaces
- YAGNI - You Ain't Going to Need It

<strong>[Dependency Inversion Principle](#dependency-inversion-principle)</strong>

- High-Level modules should not depend upon low-level ones; use abstraction.


### Single Responsibility Principle 
A class should have single reason to change or take only one responsibility.

All of the contents of a single class are tightly coupled together, since the class itself is a single unit that must either be entirely used or not at all (discounting static methods and data for the moment). When other classes make use of a particular class, and that class changes, the depending classes must be tested to ensure they continue to function correctly with the new behavior of the class. 

We define each responsibility of a class as a reason for change.  If you can think of more than one motivation for changing a class, it probably has more than one responsibility.  When these axes of change occur, the class will probably need to have different aspects of its behavior changed, at different times and for different reasons.



#### Some examples of responsibilities to consider that may need to be separated include:

- Persistence
- Validation
- Notification
- Error Handling
- Logging
- Class Selection / Instantiation
- Formatting
- Parsing
- Mapping

```c++
struct PersistenceManager
{
    static void save(const Journal& j, const string& filename)
    {
        ofstream ofs(filename);
        for(auto &e : j.entries) {
            ofs << e << endl;
        }
    }
};
```

<strong>[Back to SOLID Principle](#solid-design-principle)</strong>
<br>

### Open Closed Principle
Software entities (classes, modules, methods, etc.)  should be open for extension, but closed for modification.
Avoid to jump into the code you have already writen.

```c++
/* Specification Interface */
template <typename T> struct Specification
{
    virtual bool is_satisfied(T* item) = 0;
    /* Should be defined through the inheritance */

    AndSpecification<T> operator&&(Specification<T> && other)
    {
        return AndSpecification<T>(*this, other);
    }
};

/* Filter Interface */
template <typename T> struct Filter
{
    /* Pure Virtual Interface */
    virtual vector<T*> filter(vector<T*> items,
                              Specification<T> &spec) = 0;
};

struct BetterFilter : Filter<Product>
{
    vector<Product *> filter(vector<Product*> items, Specification<Product> &spec)
    override {
        vector<Product*> result;
        for(auto &item : items)
            if(spec.is_satisfied(item))
                    result.push_back(item);
        return result;
    }
};

struct ColorSpecification : Specification<Product>
{
    Color color;
    ColorSpecification(Color color) : color(color) {}

    bool is_satisfied(Product *item) override {
        return item -> color == color;
    }
};

struct SizeSpecification : Specification<Product>
{
    Size size;
    explicit SizeSpecification(const Size size)
    : size { size }
    }

    bool is_satisfied(Product *item) override {
        return item -> size == size;
    }
};

template <typename T> struct AndSpecification : Specification<T>
{
    Specification<T> &first;
    Specification<T> &second;

    AndSpecification(Specification<T> &first,
                     Specification<T> &second) : first(first), second(second) {}

    bool is_satisfied(T *item) override {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }
};
```

```c++
int main()
{
    Product apple{"Apple", Color::green, Size::small};
    Product tree{"Tree", Color::green, Size::large};
    Product house{"House", Color::blue, Size::large};

    vector<Product*> items {&apple, &tree, &house};

    /* Poor method to write a product filter */
    ProductFilter pf;
    auto green_things = pf.by_color(items, Color::green);
    for(auto & item : greem_things)
        cout << item->name << " is " << item->color << endl;

    /* By implementing Specification and Filter Interfaces */
    BetterFilter bf;
    ColorSpecification green(Color::green);

    for(auto & item : bf.filter(items, green))
        count << item -> name << " is " << item -> color << endl;

    SizeSpecification large(Size::large);
    AndSpecification<Product> green_and_large(green, large);

    /* Same method of combining both SizeSpecification and ColorSpecification */
    /* By overriding the operator "&&" in the Specification class */
    auto spec = ColorSpecification(Color::green)
                && SizeSpecification(Size::large);

    for(auto & item : bf.filter(items, spec))
        count << item -> name << " is green and large." << endl;


    return 0;
}

```

<strong>[Back to SOLID Principle](#solid-design-principle)</strong>
<br>

### Liskov Substitution Principle

Subtypes must be substitutable for their base types.

When this principle is violated, it tends to result in a lot of extra conditional logic scattered throughout the application, checking to see the specific type of an object. This duplicate, scattered code becomes a breeding ground for bugs as the application grows.

```
Inheritance:

One object can be designed to inherit from another if it always has an 
            “IS-SUBSTITUTABLE-FOR” relationship with the inherited object.
```

A common code smell that frequently indicates an LSP violation is the presence of type checking code within a code block that should be polymorphic. For instance, if you have a foreach loop over a collection of objects of type Foo, and within this loop there is a check to see if Foo is in fact Bar (subtype of Foo), then this is almost certainly an LSP violation. If instead you ensure Bar is in all ways substitutable for Foo, there should be no need to include such a check.

```c++
/* Valid the case in Square */
void process(Rectangle& r)
{
    int w = r.getWidth();
    r.setHeight(10); /* Also set Width as 10 in Square */

    cout << "excepted area = " << (w*10)
        << " , got " << r.area() << endl;
}

int main() {
    Rectangle r{3, 4};
    process(r); /* Excepted 40, got 40 */

    Square sq{5};
    process(sq); /* Excepted 50, got 100 */


    return 0;
}
```

<strong>[Back to SOLID Principle](#solid-design-principle)</strong>
<br>

### Interface Segregation Principle
Clients should not be forced to depend on methods that they do not use.

Interfaces should belong to clients, not to libraries or hierarchies. Application developers should favor thin, focused interfaces to “fat” interfaces that offer more functionality than a particular class or method needs.

Another benefit of smaller interfaces is that they are easier to implement fully, and thus less likely to break the Liskov Substitution Principle by being only partially implemented. 

#### Bad Implementation: We gave client an interface that client could directly implement the method they need it.

```c++
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
        // NULL VALUE
    }

    void fax(Document &doc) override {
        // NULL VALUE
    }
};

struct Scanner : IMachine
{
    void print(Document &doc) override {
        // NULL VALUE
    }

    void scan(Document &doc) override {
        // DO
    }

    void fax(Document &doc) override {
        // NULL VALUE
    }
};
```

#### Make Iterfaces smaller
```c++
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
```

<strong>[Back to SOLID Principle](#solid-design-principle)</strong>
<br>

### Dependency Inversion Principle

- High-Level Modules should not depend on Low-Level Modules. Both should depend on abstractions.

- Abstractions should not depend on details. Details should depend on abstractions.

- Should not depend on details of some body else's implementation.

#### If the low-level module Relationships would like to make vector of relations becomes private or other type of data, then the high-level module Research would be crushed.

```c++
struct Relationships /* Low Level Modules */
{
    vector<tuple<Person, Relationship, Person>> relations;
    void add_parent_and_child(const Person& parent, const Person& child)
    {
        relations.push_back({parent, Relationship :: parent, child});
        relations.push_back({child, Relationship :: child, parent});
    }
};
    
struct Research /* High Level Modules */
{
  Research(Relationships& relationships)
  {
      auto& relations = relationships.relations;
      for(auto &&[first, rel, second] : relations)
      {
          if(first.name == "John" && rel == Relationship::parent)
          {
            cout << "John has a child called" << second.name << endl;
          }
      }
  }
};

int main() {
    Person parent{"John"};
    Person child1{"Chris"}, child2{"Matt"};
    Relationships rs;
    rs.add_parent_and_child(parent, child1);
    rs.add_parent_and_child(parent, child2);

    Research _(rs);
    return 0;
}
```
#### Have dependency on abstraction RelationshipBrowser

```c++
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
```

<strong>[Back to SOLID Principle](#solid-design-principle)</strong>
<br>

***

## Creational Patterns
<strong> [Builder](#builder) </strong>

<strong> [Abstract Factories](#abstract-factories) </strong>

<strong> [Factories](#factories) </strong>

<strong> [Prototype](#prototype) </strong>

<strong> [Singleton](#singleton) </strong>

### Builder

將一個複雜的構建過程與其具表示細節相分離，使得同樣的構建過程可以創建不同的表示。

- Some objects are simple and can be created in a single constructor call.
- Other objects require a lot of ceremony to create.
- Having an object with 10 constructor arguments is not productive.
- Instead, opt for piecewise construction.
- Builder provides an API for constructing an object step-by-step


```c++
struct HtmlBuilder
{
    HtmlElement root;

    HtmlBuilder(string root_name)
    {
        root.name = root_name;
    }

    /* Fluent Interface : Return a Reference */
    HtmlBuilder& add_child(string child_name, string child_text)
    {
        HtmlElement e{ child_name, child_text };
        root.elements.emplace_back(e);
        return *this;
    }

    /* Fluent Interface : Return a Pointer */
    HtmlBuilder* add_child_2(string child_name, string child_text)
    {
        HtmlElement e{ child_name, child_text };
        root.elements.emplace_back(e);
        return this;
    }

    string str() const {return root.str();}

};
```

#### Builder Coding Exercise

```c++
#include <string>
#include <vector>
#include <ostream>
using namespace std;

struct Field
{
    string name, type;

    Field(const string& name, const string& type)
            : name{name},
              type{type}
    {
    }


    friend ostream& operator<<(ostream& os, const Field& obj)
    {
        return os << obj.type << " " << obj.name << ";";
    }
};

struct Class
{
    string name;
    vector<Field> fields;

    friend ostream& operator<<(ostream& os, const Class& obj)
    {
        os << "class " << obj.name << "\n{\n";
        for (auto&& field : obj.fields)
        {
            os << "  " << field << "\n";
        }
        return os << "};\n";
    }
};


class CodeBuilder
{
    Class the_class;

public:
    CodeBuilder(const string& class_name)
    {
        the_class.name = class_name;
    }

    CodeBuilder& add_field(const string& name, const string& type)
    {
        the_class.fields.emplace_back(Field{ name, type });
        return *this;
    }

    friend ostream& operator<<(ostream& os, const CodeBuilder& obj)
    {
        return os << obj.the_class;
    }
};

int main()
{
    auto cb = CodeBuilder{"Person"}.add_field("name", "string").add_field("age", "int");
    ostringstream oss;
    oss << cb;
    auto printed = oss.str();
}
```






<strong>[Back to Creational Patterns](#creational-patterns)</strong>
<br>

***

### Factories

不同條件下創建不同實例。

A component responsibile solely for the wholesale (not piecewise) creation of objects.

- Object creation logic becomes too convoluted.
- Constructor is not descriptive. (Cannot overload with smae sets of arguments with different names)
- Object creation(non-piecewise, unlike Builder) can be outsourced to.
- A separate funciton (Factory Method)
- That may exist in a separate class (Factory)


#### Dont allow to redeclare the constructor with same types of arguments.

```c++
struct Point
{
    float x, y;
    Point(float x, float y) : x(x), y(y) {}
    Point(float rho, float theta) {
    }
};
```

#### Instead we use anotehr ENUM class to represent PointType

```c++
struct Point
{
    float x, y;

    //! 
    //! \param a this is either x or tho
    //! \param b this is either y or theta
    //! \param type
    Point(float a, float b, PointType type = PointType::cartesian)
    {
        if(type == PointType::cartesian)
        {
            x = a;
            y = b;
        } else {
            x = a * cos(b);
            y = b * sin(b);
        }
    }
};
```

#### We still can opt to make our users know much clearly about the Interfaces

```c++
struct Point
{

    Point(float x, float y) : x(x), y(y) {}
public:
    float x, y;

    static Point NewCartesian(float x, float y)
    {
        return {x, y};
    }

    static Point NewPolar(float r, float theta)
    {
        return {r*cos(theta), r*sin(theta)};
    }

    friend ostream &operator<<(ostream &os, const Point &point) {
        os << "x: " << point.x << " y: " << point.y;
        return os;
    }

};
```

#### Still can improve by add a Factory but inteads of adding PointFactory as a friend class into Point (no valid in Open-Close Principle), we can make all variable and mehtods in Point Public

```c++

struct Point
{
public:
    float x, y;

    Point(float x, float y) : x(x), y(y) {}

    friend ostream &operator<<(ostream &os, const Point &point) {
        os << "x: " << point.x << " y: " << point.y;
        return os;
    }

};

struct PointFactory
{
    static Point NewCartesian(float x, float y)
    {
        return {x, y};
    }

    static Point NewPolar(float r, float theta)
    {
        return {r*cos(theta), r*sin(theta)};
    }
};
```

#### Inner Factory : Make user have clear API to know what they should do

```c++
class Point
{
    Point(float x, float y) : x(x), y(y) {}

    class PointFactory
    {
        PointFactory() {}

    public:
        static Point NewCartesian(float x, float y)
        {
            return { x,y };
        }
        static Point NewPolar(float r, float theta)
        {
            return{ r*cos(theta), r*sin(theta) };
        }
    };

public:
    float x, y;
    static PointFactory Factory;

    friend ostream &operator<<(ostream &os, const Point &point) {
        os << "x: " << point.x << " y: " << point.y;
        return os;
    }
};

int main()
{
    auto p = Point::Factory.NewCartesian(2, 3);
    cout << p << endl;
    return 0;
}
```


<strong>[Back to Creational Patterns](#creational-patterns)</strong>
<br>

***

### Abstract Factories





<strong>[Back to Creational Patterns](#creational-patterns)</strong>
<br>

***

### Prototype

通過拷貝原型創建新的對象。


<strong>[Back to Creational Patterns](#creational-patterns)</strong>
<br>


***

### Singleton

保證一個類僅有一個實例。

<strong>[Back to Creational Patterns](#creational-patterns)</strong>
<br>

***


## Structural Patterns

<strong> [Adapter](#adapter) </strong>

<strong> [Bridge](#bridge) </strong>

<strong> [Composite](#composite) </strong>

<strong> [Decorator](#decorator) </strong>

<strong> [Facade](#facade) </strong>

<strong> [Flyweight](#flyweight) </strong>

<strong> [Proxy](#proxy) </strong>

***

### Adapter

使得原本由於接口不兼容而不能一起工作的那些類可以一起工作。


<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>


***

### Bridge

兩個維度獨立變化，依賴方式實現抽象與實現分離：需要一個作為橋接的接口/抽象類，多個角度的實現類依賴注入到抽象類，使它們在抽象層建立一個關聯關係。

<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>


***

### Composite

用戶對單個對象和組合對象的使用具有一致性的統一接口。

<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>


***

### Decorator

保持接口，增強性能：修飾類繼承被修飾對象的抽象父類，依賴被修飾對象的實例（被修飾對象依賴注入），以實現接口擴展。

<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>


***

### Facade

在客戶端和複雜系統之間再加一層，這一次將調用順序、依賴關係等處理好。即封裝底層實現，隱藏系統的複雜性，並向客戶端提供了一個客戶端可以訪問系統的高層接口


<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>


***

### Flyweight

享元工廠類控制；HashMap實現緩衝池重用現有的同類對象，如果未找到匹配的對象，則創建新對象

<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>

***

### Proxy

為其他對象提供一種代理以控制對這個對象的訪問：增加中間層（代理層），代理類與底層實現類實現共同接口，並創建底層實現類對象（底層實現類對象依賴注入代理類），以便向外界提供功能接口。

<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>

***

## Behavioral Patterns

<strong> [Chain of Responsibility](#chain-of-responsibility) </strong>

<strong> [Command](#command) </strong>

<strong> [Interpreter](#interpreter) </strong>

<strong> [Iterator](#iterator) </strong>

<strong> [Mediator](#mediator) </strong>

<strong> [Memento](#memento) </strong>

<strong> [Observer](#observer) </strong>

<strong> [State](#state) </strong>

<strong> [Stategy](#stategy) </strong>

<strong> [Template Method](#template-method) </strong>

<strong> [Visitor](#visitor) </strong>


### Chain of Responsibility

攔截的類都實現統一接口，每個接收者都包含對下一個接收者的引用。將這些對象連接成一條鏈，並且沿著這條鏈傳遞請求，直到有對象處理它為止。

<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***


### Command

將"行為請求者"與"行為實現者"解耦：調用者依賴命令，命令依賴接收者，調用者Invoker→命令Command→接收者Receiver。


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

### Interpreter

給定一個語言，定義它的文法表示，並定義一個解釋器，這個解釋器使用該標識來解釋語言中的句子。

<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

### Iterator

集合中含有迭代器：分離了集合對象的遍歷行為，抽象出一個迭代器類來負責，無須暴露該對象的內部表示。

<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

### Mediator

對象與對象之間存在大量的關聯關係，將對象之間的通信關聯關係封裝到一個中介類中單獨處理，從而使其耦合鬆散，可以獨立地改變它們之間的交互。


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

### Memento

通過一個備忘錄類專門存儲對象狀態。客戶通過備忘錄管理類管理備忘錄類。


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

### Observer

一對多的依賴關係，在觀察目標類里有一個 ArrayList 存放觀察者們。當觀察目標對象的狀態發生改變，所有依賴於它的觀察者都將得到通知，使這些觀察者能夠自動更新。（即使用推送方式）


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

### State

狀態對象依賴注入到context對象，context對象根據它的狀態改變而改變它的相關行為(可通過調用內部的狀態對象實現相應的具體行為)


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

### Stategy

策略對象依賴注入到context對象，context對象根據它的策略改變而改變它的相關行為(可通過調用內部的策略對象實現相應的具體策略行為)

<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

### Template Method

將這些通用算法抽象出來，在一個抽象類中公開定義了執行它的方法的方式/模板。它的子類可以按需要重寫方法實現，但調用將以抽象類中定義的方式進行。

<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

### Visitor


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***


## Reference Credits

[Course Link](https://www.udemy.com/patterns-cplusplus/)

[References resources - Deviq](https://deviq.com/solid/)

