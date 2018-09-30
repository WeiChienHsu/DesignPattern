//
// Created by weichien on 30/09/2018.
//

class Rectangle
{
protected:
    int width, height;
public:
    Rectangle(int width, int height) : width(width), height(height){}

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    virtual void setWidth(int width) {
        Rectangle::width = width;
    }

    virtual void setHeight(int height) {
        Rectangle::height = height;
    }

    int area() const {return height * width;}
};

class Square : public Rectangle
{
public:
    Square(int size) : Rectangle(size, size) {}

    void setWidth(int width) override {
        this -> width = this -> height = width;
    }

    void setHeight(int height) override
    {
        this -> width = this -> height = height;
    }
};

/* Valid the case in Square */
void process(Rectangle& r)
{
    int w = r.getWidth();
    r.setHeight(10); /* Also set Width as 10 in Square */

    cout << "excepted area = " << (w*10)
        << " , got " << r.area() << endl;
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
