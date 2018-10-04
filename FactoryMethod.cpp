#include <cmath>
#include <ostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <tuple>
#include <iostream>
using namespace std;

class Point
{
    // use a factory method
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