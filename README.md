# Design Pattern in C++ Learning Notes

[Course Link](https://www.udemy.com/patterns-cplusplus/)

Common architectural approaches.


## SOLID Design Principle

### Single Responsibility Principle 
A class should have single reason to change or take only one responsibility.

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
Your assistance should be open for extension.
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


