# Design Pattern in C++ Learning Notes

[Course Link](https://www.udemy.com/patterns-cplusplus/)
[References resources - Deviq](https://deviq.com/solid/)

Common architectural approaches.


## SOLID Design Principle

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

### Open-Closed Principle
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





### Interface Segregation Principle
Clients should not be forced to depend on methods that they do not use.

Interfaces should belong to clients, not to libraries or hierarchies. Application developers should favor thin, focused interfaces to “fat” interfaces that offer more functionality than a particular class or method needs.

Another benefit of smaller interfaces is that they are easier to implement fully, and thus less likely to break the Liskov Substitution Principle by being only partially implemented. 




### Dependency Inversion Principle


## Creational 
- Builder
- Factories: Abstract Factory, Factory Method
- Prototype
- Singleton

## Structural 
- Adapter
- Bridge
- Composite
- Decorator
- Facade
- Flyweight
- Proxy

## Behavioral
- Chain of Responsibility
- Command
- Interpreter
- Iterator
- Mediator
- Memento
- Observer
- State
- Stategy
- Template Method
- Visitor

***


