# Design Pattern in C++

`
Common architectural approaches.
`


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

# Creational Patterns
<strong> [Builder](#builder) </strong>

<strong> [Factories](#factories) </strong>

<strong> [Prototype](#prototype) </strong>

<strong> [Singleton](#singleton) </strong>

## Builder


<strong>[Back to Creational Patterns](#creational-patterns)</strong>
<br>

***

## Factories


<strong>[Back to Creational Patterns](#creational-patterns)</strong>
<br>

***

## Prototype


<strong>[Back to Creational Patterns](#creational-patterns)</strong>
<br>


***

## Singleton


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

## Adapter


<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>


***

## Bridge


<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>


***

## Composite


<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>


***

## Decorator


<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>


***

## Facade


<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>


***

## Flyweight


<strong>[Back to Structural Patterns](#structural-patterns)</strong>
<br>

***

## Proxy


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


## Chain of Responsibility


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***


## Command


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

## Interpreter


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

## Iterator


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

## Mediator


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

## Memento


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

## Observer


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

## State


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

## Stategy


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

## Template Method

<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***

## Visitor


<strong>[Back to Behavioral Patterns](#behavioral-patterns)</strong>
<br>

***


## Reference Credits

[Course Link](https://www.udemy.com/patterns-cplusplus/)

[References resources - Deviq](https://deviq.com/solid/)

