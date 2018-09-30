//
// Created by weichien on 29/09/2018.
//

enum class Color {red, green, blue};
enum class Size {small, medium, large};

struct Product
{
    string name;
    Color color;
    Size size;
};

struct ProductFilter
{
    vector<Product*> by_color(vector<Product*> items, Color color)
    {
        vector<Product*> result;
        for(auto& i : items)
            if(i->color == color)
                result.push_back(i);
        return result;
    }
};

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



/* If manager asked you to make sure products could be filtered by size */
/* Then you need to modify the name of mehtod and also the arguments.  */
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

    auto spec = ColorSpecification(Color::green)
                && SizeSpecification(Size::large);

    for(auto & item : bf.filter(items, spec))
        count << item -> name << " is green and large." << endl;


    return 0;
}
