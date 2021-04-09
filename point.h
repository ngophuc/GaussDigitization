#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <vector>

template < size_t dim = 2, typename T = double >
class Point
{
public:
    Point() { std::fill(elements_, elements_+dim, 0); }
    //Point(const Point& rhs) : elements_(rhs.elements_) {}
    virtual ~Point() {}

    Point & operator=(const Point& rhs)
    {
        for(int i = 0; i < dim; i++)
            elements_[i] = rhs.elements_[i];
        return *this;
    }

    Point operator+(const Point& p)
    {
        Point ret;
        for(int i = 0; i < dim; i++)
            ret[i] += elements_[i] + p.elements_[i];
        return ret;
    }

    Point & operator+=( const Point& p)
    {
        for(int i = 0; i < dim; i++)
            elements_[i] += p.elements_[i];
        return *this;
    }

    Point & operator-=( const Point& p)
    {
        for(int i = 0; i < dim; i++)
            elements_[i] -= p.elements_[i];
        return *this;
    }

    bool operator==(Point& p)
    {
        for(int i = 0; i < dim; i++)
            if (p[i] != elements_[i])
                return false;
        return true;
    }
    
    //const T & operator[](const size_t index) const
    T & operator[](const size_t index)
    {
        return elements_[index];
    }
    
    double distance(const Point& p) {
        double d=0;
        for(int i = 0; i < dim; i++) {
            double v = elements_[i]*p.elements_[i];
            d += v*v;
        }
        return sqrt(d);
    }
    
    friend std::ostream& operator<<(std::ostream& out, Point & p)
    {
        std::string s = "Point ( ";
        for(int i = 0; i < dim; i++)
            s += std::to_string(p[i])  +" ";
        return out << s << ") ";
    }
private:
    T elements_[dim];
};

template<typename T>
class Point2D : public Point<2, T>
{
public:

    Point2D() { }

    Point2D(const Point2D& rhs) : Point<2, T>(rhs) { }

    Point2D(const T x, const T y)
    {
        (*this)[0] = x;
        (*this)[1] = y;
    }
    
    T x() {return (*this)[0];}
    T y() {return (*this)[1];}
};

template<typename T>
class Point3D : public Point<3, T>
{
public:

    Point3D() { }

    Point3D(const Point3D& rhs) : Point<3, T>(rhs) { }

    Point3D(const T x, const T y, const T z)
    {
        (*this)[0] = x;
        (*this)[1] = y;
        (*this)[2] = z;
    }
    
    T x() {return (*this)[0];}
    T y() {return (*this)[1];}
    T z() {return (*this)[2];}
};

typedef Point2D< int > Point2Di;
typedef Point3D< int > Point3Di;
typedef Point2D< double > Point2Dd;
typedef Point3D< double > Point3Dd;

#endif // POINT_H
