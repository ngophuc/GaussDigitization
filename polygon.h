#ifndef POLYGON_H
#define POLYGON_H
#include "point.h"
#include "functions.h"

template < typename TPoint >
class Polygon
{
public:
    Polygon() {}
    //Polygon(const Polygon& rhs) : elements_(rhs.elements_) {}
    virtual ~Polygon() {}
    Polygon & operator=(const Polygon& rhs) { return *this; }
    
    TPoint& operator[](const size_t index)
    {
        return elements_.at(index);
    }
    
    TPoint& at(const size_t index)
    {
        return elements_.at(index);
    }
    
    TPoint& front()
    {
        return elements_.at(0);
    }
    
    TPoint& back()
    {
        return elements_.at(elements_.size()-1);
    }
    
    size_t size()
    {
        return elements_.size();
    }
    
    Polygon& push_back(const TPoint& p){
        elements_.push_back(p);
        return (*this);
    }
    
private:
    std::vector<TPoint> elements_;
};

template<typename T>
class Polygon2D : public Polygon<Point2D<T> >
{
public:
    typedef  Point2D<T> TPoint;
    Polygon2D() { }
    Polygon2D(const Polygon2D& rhs) : Polygon<TPoint>(rhs) { }
    
    std::pair<TPoint, TPoint > getBoundingBox() {
        T minX = this->at(0)[0];
        T maxX = this->at(0)[0];
        T minY = this->at(0)[1];
        T maxY = this->at(0)[1];
        
        for(size_t it=1; it<this->size(); it++) {
            T x = this->at(it)[0];
            T y = this->at(it)[1];
            if(minX > x) minX = x;
            if(maxX < x) maxX = x;
            if(minY > y) minY = y;
            if(maxY < y) maxY = y;
        }
        Point2D<T> tl (minX,minY);
        Point2D<T> rb (maxX,maxY);
        return std::make_pair(tl,rb);
    }
    
    friend std::ostream& operator<<(std::ostream& out, Polygon2D & poly)
    {
        std::string s = "Polygon: ";
        for(int i = 0; i < poly.size(); i++)
            s += "(" + std::to_string(poly.at(i).x())  + "," + std::to_string(poly.at(i).y()) + ") ";
        return out << s;
    }
    
    bool isInside(Point2D<T> p) {
        return isInsidePolygon<T>(*this, p);
    }
    
private:
};

template<typename T>
class Mesh3D
{
public:
    typedef  Point3D<T> TPoint;
    Mesh3D() { }
    Mesh3D(const Mesh3D& rhs) {
        this->vertices_ = rhs.getVertices();
        this->faces_ = rhs.getFaces();
    }
    Mesh3D(const std::vector<TPoint>& vertices, const std::vector<std::vector<unsigned int> >& faces) {
        this->vertices_ = vertices;
        this->faces_ = faces;
    }
    
    std::vector<TPoint> getVertices() const {return this->vertices_;}
    std::vector<std::vector<unsigned int> > getFaces() const {return this->faces_;}
    void addVertex(const TPoint& v) {this->vertices_.push_back(v);}
    void addFace(const std::vector<unsigned int>& f) {this->faces_.push_back(f);}
    
    std::pair<TPoint, TPoint > getBoundingBox() {
        T minX = this->vertices_.at(0)[0];
        T maxX = this->vertices_.at(0)[0];
        T minY = this->vertices_.at(0)[1];
        T maxY = this->vertices_.at(0)[1];
        T minZ = this->vertices_.at(0)[2];
        T maxZ = this->vertices_.at(0)[2];
        
        for(size_t it=1; it<this->vertices_.size(); it++) {
            T x = this->vertices_.at(it)[0];
            T y = this->vertices_.at(it)[1];
            T z = this->vertices_.at(it)[2];
            if(minX > x) minX = x;
            if(maxX < x) maxX = x;
            if(minY > y) minY = y;
            if(maxY < y) maxY = y;
            if(minZ > z) minZ = z;
            if(maxZ < z) maxZ = z;
        }
        Point3D<T> tl (minX,minY,minZ);
        Point3D<T> rb (maxX,maxY,maxZ);
        return std::make_pair(tl,rb);
    }
    
    friend std::ostream& operator<<(std::ostream& out, Mesh3D & mesh)
    {
        std::string s = "Mesh: ";
        for(int i = 0; i < mesh.size(); i++)
            s += "(" + std::to_string(mesh.at(i).x())  + "," + std::to_string(mesh.at(i).y())  + "," + std::to_string(mesh.at(i).z())  + ") ";
        return out << s;
    }
    
    bool isInside(Point3D<T> p) {
        return isInsideMesh<T>(*this, p);
    }
private:
    std::vector<TPoint> vertices_;
    std::vector<std::vector<unsigned int> > faces_;
};

typedef Polygon2D< int > Polygon2Di;
typedef Mesh3D< int > Mesh3Di;
typedef Polygon2D< double > Polygon2Dd;
typedef Mesh3D< double > Mesh3Dd;

#endif // POLYGON_H
