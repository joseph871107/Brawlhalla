/* File description:
This is the header file 'Vector2.h'. This is the API for the class 'Vector2'. */

namespace game_framework
{
#ifndef VECTOR2_H
#define VECTOR2_H
class Vector2
{
    public:
        virtual ~Vector2();
        //Destructor

        Vector2();
        //Default constructor

        Vector2(const Vector2& objectValue);
        //Copy constructor

        Vector2(const int& xValue, const int& yValue);
        //Full constructor

        Vector2& operator=(const Vector2& rightObject);
        //Operator overloading function of the assignment operator

        const double GetLength() const;

        const int& GetX() const;

        const int& GetY() const;

        void SetXY(const int& x1, const int& y1, const int& x2, const int& y2);

    private:
        int _x, _y;

};
#endif
}