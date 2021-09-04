#ifndef UTILS_8_31
#define UTILS_8_31

#include<iostream>
#include<math.h>

template <class T>
struct v2_gen
{
    v2_gen() : x(0), y(0) {}
    v2_gen(const T _x, const T _y) : x(_x), y(_y) {}
    v2_gen(const T a[2]) : x(a[0]), y(a[1]) {}
    v2_gen(const v2_gen& v) : x(v.x), y(v.y) {}
    v2_gen& operator=(const v2_gen& v) = default;
    v2_gen& operator=(double s) {this->x=s; this->y=s; return (*this);}
    
    T mag() const {return T(std::sqrt(x*x + y*y));}
    T mag2() const {return x*x + y*y;}

    v2_gen norm() const { T r=1/mag(); return v2_gen(x*r, y*r); }
    v2_gen perp() const { return v2_gen(-y, x); }
    v2_gen floor() const { return v2_gen(std::floor(x), std::floor(y)); }
    v2_gen ceil() const {return v2_gen(std::ceil(x), std::ceil(y)); }
    v2_gen max(const v2_gen& v) const { return v2_gen(std::max(x, v.x), std::max(y, v.y)); }
    v2_gen min(const v2_gen& v) const { return v2_gen(std::min(x, v.x), std::min(y, v.y)); }
    v2_gen cart() { return {std::cos(y) * x, std::sin(y) * x}; }
    v2_gen polar() { return { mag(), std::atan2(y,x) }; }

    T dot(const v2_gen& rhs) const {return this->x*rhs.x + this->y*rhs.y;}
    T& operator[](int i) { return (i==0) * x + (i==1) * y; }
    T operator()(int i) const {return (i==0) * x + (i==1) * y;}

    v2_gen operator + (const v2_gen& rhs)   const { return v2_gen(this->x+rhs.x, this->y+rhs.y); }
    v2_gen operator - (const v2_gen& rhs)   const { return v2_gen(this->x-rhs.x, this->y-rhs.y); }

    v2_gen operator * (const T& rhs)        const { return v2_gen(this->x*rhs, this->y*rhs); }
    v2_gen operator * (const v2_gen& rhs)   const { return v2_gen(this->x*rhs.x, this->y*rhs.y); }
    v2_gen operator / (const T& rhs)        const { return v2_gen(this->x/rhs, this->y/rhs); }
    v2_gen operator / (const v2_gen& rhs)   const { return v2_gen(this->x/rhs.x, this->y/rhs.y); }


    v2_gen operator += (const v2_gen& rhs) {this->x+=rhs.x; this->y+=rhs.y; return(*this); }
    v2_gen operator -= (const v2_gen& rhs) {this->x-=rhs.x; this->y-=rhs.y; return(*this); }

    v2_gen operator *= (const T& rhs) {this->x*=rhs; this->y*=rhs; return(*this); }
    v2_gen operator /= (const T& rhs) {this->x/=rhs; this->y/=rhs; return(*this); }

    v2_gen operator *= (const v2_gen& rhs) {this->x*=rhs.x; this->y*=rhs.y; return(*this); }
    v2_gen operator /= (const v2_gen& rhs) {this->x/=rhs.x; this->y/=rhs.y; return(*this); }

    bool operator == (const v2_gen& rhs) const { return (this->x == rhs.x && this->y == rhs.y); }
    bool operator != (const v2_gen& rhs) const { return (this->x != rhs.x || this->y != rhs.y); }

    const std::string str() const {return std::string("(") + std::to_string(this->x) + "," + std::to_string(this->y) + ")";}
    friend std::ostream& operator << (std::ostream& os, const v2_gen& rhs) {os << rhs.str(); return os;}

    operator v2_gen<int32_t>() const { return {static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) }; }
    operator v2_gen<float>() const { return {static_cast<float>(this->x), static_cast<float>(this->y) }; }
    operator v2_gen<double>() const { return {static_cast<double>(this->x), static_cast<double>(this->y) }; }

protected:
    T x = 0;
    T y = 0;
};

template<class T> inline v2_gen<T> operator * (const float& lhs, const v2_gen<T>& rhs)
{ return v2_gen((T) lhs * (float)rhs.x, (T)(lhs * (float)rhs.y)); }
template<class T> inline v2_gen<T> operator * (const double& lhs, const v2_gen<T>& rhs)
{ return v2_gen((T) lhs * (double)rhs.x, (T)(lhs * (double)rhs.y)); }
template<class T> inline v2_gen<T> operator * (const int& lhs, const v2_gen<T>& rhs)
{ return v2_gen((T) lhs * (int)rhs.x, (T)(lhs * (int)rhs.y)); }
template<class T> inline v2_gen<T> operator / (const float& lhs, const v2_gen<T>& rhs)
{ return v2_gen((T) lhs / (float)rhs.x, (T)(lhs / (float)rhs.y)); }
template<class T> inline v2_gen<T> operator / (const double& lhs, const v2_gen<T>& rhs)
{ return v2_gen((T) lhs / (double)rhs.x, (T)(lhs / (double)rhs.y)); }
template<class T> inline v2_gen<T> operator / (const int& lhs, const v2_gen<T>& rhs)
{ return v2_gen((T) lhs / (int)rhs.x, (T)(lhs / (int)rhs.y)); }


// not 100% sure where this can be used yet
template<class T, class U> inline bool operator < (const v2_gen<T>& lhs, const v2_gen<U>& rhs)
{ return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x); }
template<class T, class U> inline bool operator > (const v2_gen<T>& lhs, const v2_gen<U>& rhs)
{ return lhs.y > rhs.y || (lhs.y == rhs.y && lhs.x > rhs.x); }

typedef v2_gen<double> v2d;
typedef v2_gen<unsigned int> v2ui;


// 3D vector struct

template <class T>
struct v3_gen
{
    v3_gen() : x(0), y(0), z(0) {}
    v3_gen(const T _x, const T _y, const T _z) : x(_x), y(_y), z(_z) {}
    v3_gen(const T a[3]) : x(a[0]), y(a[1]), z(a[2]) {}
    v3_gen(const v3_gen& v) : x(v.x), y(v.y), z(v.z) {}
    v3_gen& operator=(const v3_gen& v) {this->x=v.z; this->y=v.z; this->z=v.z; return (*this);};
    v3_gen& operator=(double s) {this->x=s; this->y=s; this->z=s; return (*this);}

    T mag() const {return T(std::sqrt(x*x + y*y));}
    T mag2() const {return x*x + y*y;}

    v3_gen norm() const { T r=1/mag(); return v3_gen(x*r, y*r); }
    v3_gen perp() const { return v3_gen(-y, x); }
    v3_gen floor() const { return v3_gen(std::floor(x), std::floor(y)); }
    v3_gen ceil() const {return v3_gen(std::ceil(x), std::ceil(y)); }
    v3_gen max(const v3_gen& v) const { return v3_gen(std::max(x, v.x), std::max(y, v.y)); }
    v3_gen min(const v3_gen& v) const { return v3_gen(std::min(x, v.x), std::min(y, v.y)); }
    v3_gen cart() { return {std::cos(y) * x, std::sin(y) * x}; }
    v3_gen polar() { return { mag(), std::atan2(y,x) }; }

    T dot(const v3_gen& rhs) const {return this->x*rhs.x + this->y*rhs.y;}
    T& operator[](int i) { if(i==0) return x; else if(i==1) return y; else if(i==2) return z; else std::cerr<<"bad index\n"; return x; }
    T operator()(int i) const {return (i==0) * x + (i==1) * y + (i==2) * z;}

    v3_gen operator + (const v3_gen& rhs)   const { return v3_gen(this->x+rhs.x, this->y+rhs.y, this->z+rhs.z); }
    v3_gen operator - (const v3_gen& rhs)   const { return v3_gen(this->x-rhs.x, this->y-rhs.y, this->z-rhs.z); }

    v3_gen operator + (const T& rhs)        const { return v3_gen(this->x+rhs, this->y+rhs, this->z+rhs); }
    v3_gen operator - (const T& rhs)        const { return v3_gen(this->x-rhs, this->y-rhs, this->z-rhs); }

    v3_gen operator * (const T& rhs)        const { return v3_gen(this->x*rhs, this->y*rhs, this->z*rhs); }
    v3_gen operator * (const v3_gen& rhs)   const { return v3_gen(this->x*rhs.x, this->y*rhs.y, this->z*rhs.z); }
    v3_gen operator / (const T& rhs)        const { return v3_gen(this->x/rhs, this->y/rhs, this->z/rhs); }
    v3_gen operator / (const v3_gen& rhs)   const { return v3_gen(this->x/rhs.x, this->y/rhs.y, this->z/rhs.z); }


    v3_gen operator += (const v3_gen& rhs) {this->x+=rhs.x; this->y+=rhs.y; this->z+=rhs.z; return(*this); }
    v3_gen operator -= (const v3_gen& rhs) {this->x-=rhs.x; this->y-=rhs.y; this->z-=rhs.z; return(*this); }

    v3_gen operator *= (const T& rhs) {this->x*=rhs; this->y*=rhs; this->z*=rhs; return(*this); }
    v3_gen operator /= (const T& rhs) {this->x/=rhs; this->y/=rhs; this->z/=rhs; return(*this); }

    v3_gen operator *= (const v3_gen& rhs) {this->x*=rhs.x; this->y*=rhs.y; this->z*=rhs.z; return(*this); }
    v3_gen operator /= (const v3_gen& rhs) {this->x/=rhs.x; this->y/=rhs.y; this->z/=rhs.z; return(*this); }

    bool operator == (const v3_gen& rhs) const { return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z); }
    bool operator != (const v3_gen& rhs) const { return (this->x != rhs.x || this->y != rhs.y || this->z != rhs.z); }

    const std::string str() const {return std::string("(") + std::to_string(this->x) + "," + std::to_string(this->y) + "," + std::to_string(this->z) + ")";}
    friend std::ostream& operator << (std::ostream& os, const v3_gen& rhs) {os << rhs.str(); return os;}

    operator v3_gen<int32_t>() const { return {static_cast<int32_t>(this->x), static_cast<int32_t>(this->y), static_cast<int32_t>(this->z) }; }
    operator v3_gen<float>() const { return {static_cast<float>(this->x), static_cast<float>(this->y), static_cast<float>(this->z) }; }
    operator v3_gen<double>() const { return {static_cast<double>(this->x), static_cast<double>(this->y), static_cast<double>(this->z) }; }

protected:
    T x = 0;
    T y = 0;
    T z = 0;
};


template<class T> inline v3_gen<T> operator * (const float& lhs, const v3_gen<T>& rhs)
{ return v3_gen((T) lhs * (float)rhs(0), (T)(lhs * (float)rhs(1)), (T)(lhs * (float)rhs(2))); }


template<class T> inline v3_gen<T> operator * (const double& lhs, const v3_gen<T>& rhs)
{ return v3_gen( (T) lhs * (double)rhs(0), (T)(lhs * (double)rhs(1)), (T)(lhs * (double)rhs(2))); }



template<class T> inline v3_gen<T> operator * (const int& lhs, const v3_gen<T>& rhs)
{ return v3_gen((T) lhs * (int)rhs(0), (T)(lhs * (int)rhs(1)), (T)(lhs * (double)rhs(2))); }
template<class T> inline v3_gen<T> operator / (const float& lhs, const v3_gen<T>& rhs)
{ return v3_gen((T) lhs / (float)rhs(0), (T)(lhs / (float)rhs(1)), (T)(lhs / (float)rhs(2))); }


template<class T> inline v3_gen<T> operator / (const double& lhs, const v3_gen<T>& rhs)
{ return v3_gen((T) lhs / (double)rhs(0), (T)(lhs / (double)rhs(1)), (T)(lhs / (double)rhs(2))); }


template<class T> inline v3_gen<T> operator / (const int& lhs, const v3_gen<T>& rhs)
{ return v3_gen((T) lhs / (int)rhs(0), (T)(lhs / (int)rhs(1)), (T)(lhs / (double)rhs(2))); }


// not 100% sure where this can be used yet, (unsure about the 3D implementation)
// template<class T, class U> inline bool operator < (const v3_gen<T>& lhs, const v3_gen<U>& rhs)
// { return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x); }
// template<class T, class U> inline bool operator > (const v3_gen<T>& lhs, const v3_gen<U>& rhs)
// { return lhs.y > rhs.y || (lhs.y == rhs.y && lhs.x > rhs.x); }

typedef v3_gen<double> v3d;
typedef v3_gen<int32_t> v3i;




// Generalized Matrix
template<class T>
struct matrix
{
    matrix(int r, int c) : nrows(r), ncols(c) {d = new T[nrows*ncols]; for(int i=0; i<nrows*ncols;i++) d[i]=(T)0;}
    ~matrix() {delete[] d;}

    // matrix * vector

    bool operator == (const matrix& rhs) const { return (nrows==rhs.nrows && ncols==rhs.ncols && d == rhs.d); }
    bool operator != (const matrix& rhs) const { return (nrows!=rhs.nrows || ncols!=rhs.ncols || d != rhs.d); }

    const std::string str() const { return "[ ... ]"; }
    friend std::ostream& operator << (std::ostream& os, const matrix& rhs) {os << rhs.str(); return os;}

protected:
    int nrows;
    int ncols;
    T* d;
};

typedef matrix<double> dMatrix;
typedef matrix<int32_t> nMatrix;


#endif
