//
// Created by weichien on 30/09/2018.
//

#include <iostream>

class Rectangle
{
protected:
    int width, height;
public:
    Rectangle(const int width, const int height)
            : width{width}, height{height} { }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    void setWidth(int width) {
        Rectangle::width = width;
    }

    void setHeight(int height) {
        Rectangle::height = height;
    }

    int area() const { return width * height; }
};

class Square : public Rectangle
{
public:
    Square(int size): Rectangle(size,size) {}
    void set_width(const int width) override {
        this->width = height = width;
    }
    void set_height(const int height) override {
        this->height = width = height;
    }
};



/* Valid the case in Square */
void process(Rectangle& r)
{
    int w = r.getWidth();
    r.setWidth(10);

    std::cout << "expected area = " << (w * 10)
              << ", got " << r.area() << std::endl;
}

/* Factory : Should make child be substitable to any parent class */
struct RectangleFactory
{
    static Rectangle create_rectangle(int w, int h);
    static Rectangle create_square(int size);
};

int main() {
    Rectangle r{3, 4};
    process(r); /* Excepted 40, got 40 */

    Square sq{5};
    process(sq); /* Excepted 50, got 100 */


    return 0;
}
