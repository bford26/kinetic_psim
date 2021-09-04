#ifndef FIELD_H
#define FIELD_H

#include "Utils.hpp"


template <class T>
class Field_
{
public:

    // base constructors
    Field_(int ni, int nj, int nk) : ni(), nj(), nk()
    {
        // allocate memory
        data = new T**[ni];
        for(int i=0; i<ni; i++)
        {
            data[i] = new T*[nj];
            for(int j=0; j<nj; j++)
            {
                data[i][j] = new T[nk];
            }
        }

        clear();
    }
    
    Field_(v3i nn) : Field_(nn[0], nn[1], nn[2]) {};
    
    // copy constructor
    Field_(const Field_ &other) : ni(other.ni), nj(other.nj), nk(other.nk)
    {
        for(int i=0; i<ni; i++)
            for(int j=0; j<nj; j++)
                for(int k=0; k<nk; k++)
                    data[i][j][k] = other(i,j,k);
    }
    
    // move constructor
    Field_(Field_ &&other): ni(other.ni), nj(other.nj), nk(other.nk)
    {
        data = other.data;
        other.data = nullptr;
    }
    
    // move assignment op
    Field_& operator = (Field_ &&f) { data=f.data; f.data=nullptr; return (*this); }
    
    // destructor
    ~Field_()
    {
        if(data == nullptr) return;

        for(int i=0; i<ni; i++)
        {
            for(int j=0; j<nj; j++)
            {
                delete[] data[i][j];
            }
            delete[] data[i];
        }

        delete[] data;
    }

    // gives you ability to index the class object and not the data object in the class container
    T** operator[] (int i) { return data[i]; }
    // basically just indexing, but so that you cant change the data
    T operator() (int i, int j, int k) const { return data[i][j][k]; }

    void operator = (double s)
    {
        for(int i=0; i<ni; i++)
        {
            for(int j=0; j<nj; j++)
            {
                for(int k=0; k<nk; k++)
                {
                    data[i][j][k] = s;
                }
            }
        }

        // for Field_& return type
        // return (*this);
    }

    // element wise divison
    void operator /= (const Field_ &other)
    {

        for(int i=0; i<ni; i++)
        {
            for(int j=0; j<nj; j++)
            {
                for(int k=0; k<nk; k++)
                {
                    if (other.data[i][j][k] != 0)
                        data[i][j][k] /= other.data[i][j][k];
                    else
                        data[i][j][k] = 0;
                }
            }
        }

        // return (*this);
    }

    Field_& operator += (const Field_& other)
    {
        for (int i=0;i<ni;i++)
			for (int j=0;j<nj;j++)
				for (int k=0;k<nk;k++)
					data[i][j][k]+=other(i,j,k);

        return (*this);
    }

    Field_& operator *= (const double s)
    {
        for (int i=0;i<ni;i++)
			for (int j=0;j<nj;j++)
				for (int k=0;k<nk;k++)
                    data[i][j][k] *= s;
        return (*this);
    }

    // Field_& operator *= (const Field_ &other)
    // {
    //     for (int i=0;i<ni;i++)
	// 		for (int j=0;j<nj;j++)
	// 			for (int k=0;k<nk;k++)
	// 				data[i][j][k]*=other[i][j][k];
    //     return (*this);
    // }

    friend Field_<T> operator*(double s, const Field_<T>&f)
    {
        Field_<T> r(f);     // copy the field f
        r *= s;
        return r;
    }
    friend Field_<T> operator*(const Field_<T>&f, double s)
    {
        Field_<T> r(f);
        r*=s;
        return r;
    }


    // mult element wise two fields
    friend Field_<T> operator*(const Field_<T>&f1, const Field_<T>&f2)
    {
        Field_<T> r(f1);
        for(int i=0; i<f1.ni; i++)
            for(int j=0; j<f1.nj; j++)
                for(int k=0; k<f1.nk; k++)
                    r[i][j][k] = f1(i,j,k) * f2(i,j,k);
        return r;
    }

    // mult element wise two fields
    friend Field_<T> operator/(const Field_<T>&f, const Field_<T>&d)
    {
        Field_<T> r(f);
        for(int i=0; i<f.ni; i++)
            for(int j=0; j<f.nj; j++)
                for(int k=0; k<f.nk; k++)

                    if(d(i,j,k) != 0)
                        r[i][j][k] = f(i,j,k) / d(i,j,k);
                    else
                        r[i][j][k] = 0;
        return r;
    }

    int U(int i, int j, int k) {return k*ni*nj+j*ni+i;}

    void clear() { (*this) = 0; }

    void scatter(v3d lc, T value)
    {
        int i = (int) lc[0];
        double di = lc[0] - i;

        int j = (int) lc[1];
        double dj = lc[1] - j;

        int k = (int) lc[2];
        double dk = lc[2] - k;

        data[i][j][k] += (T)value*(1-di)*(1-dj)*(1-dk);		
        data[i+1][j][k] += (T)value*(di)*(1-dj)*(1-dk);
        data[i+1][j+1][k] += (T)value*(di)*(dj)*(1-dk);
        data[i][j+1][k] += (T)value*(1-di)*(dj)*(1-dk);
        data[i][j][k+1] += (T)value*(1-di)*(1-dj)*(dk);
        data[i+1][j][k+1] += (T)value*(di)*(1-dj)*(dk);
        data[i+1][j+1][k+1] += (T)value*(di)*(dj)*(dk);
        data[i][j+1][k+1] += (T)value*(1-di)*(dj)*(dk);
    }

    T gather(v3d lc)
    {
        int i = (int) lc[0];
        double di = lc[0] - i;

        int j = (int) lc[1];
        double dj = lc[1] - j;

        int k = (int) lc[2];
        double dk = lc[2] - k;

        // std::cout << "\t\tef di,dj,dk: " << di << "," << dj << "," << dk << std::endl;

        // std::cout << "\t\tef [i][j][k]: " << data[i][j][k] << std::endl;
        // std::cout << "\t\tef [i+1][j][k]: " << data[i+1][j][k] << std::endl;
        // std::cout << "\t\tef [i+1][j+1][k]: " << data[i+1][j+1][k] << std::endl;
        // std::cout << "\t\tef [i][j+1][k]: " << data[i][j+1][k] << std::endl;
        // std::cout << "\t\tef [i][j][k+1]: " << data[i][j][k+1] << std::endl;
        // std::cout << "\t\tef [i+1][j][k+1]: " << data[i+1][j][k+1] << std::endl;
        // std::cout << "\t\tef [i+1][j+1][k+1]: " << data[i+1][j+1][k+1] << std::endl;
        // std::cout << "\t\tef [i][j+1][k+1]: " << data[i][j+1][k+1] << std::endl;

        /*gather electric field onto particle position*/
		T val = data[i][j][k]*(1-di)*(1-dj)*(1-dk)+
                data[i+1][j][k]*(di)*(1-dj)*(1-dk)+
				data[i+1][j][k]*(di)*(1-dj)*(1-dk)+
				data[i+1][j+1][k]*(di)*(dj)*(1-dk)+
				data[i][j+1][k]*(1-di)*(dj)*(1-dk)+
				data[i][j][k+1]*(1-di)*(1-dj)*(dk)+
				data[i+1][j][k+1]*(di)*(1-dj)*(dk)+
				data[i+1][j+1][k+1]*(di)*(dj)*(dk)+
				data[i][j+1][k+1]*(1-di)*(dj)*(dk);

        // std::cout << "\t\tef val: " << val << std::endl; 
        	
		return val;
    }

    template<class S>
    friend std::ostream& operator<<(std::ostream &out, Field_<S> &f);

protected:
    int ni, nj, nk;
    T ***data;
};

template<class T>
std::ostream& operator<<(std::ostream &out, Field_<T> &f)
{
	for (int k=0;k<f.nk;k++,out<<"\n")
		for (int j=0;j<f.nj;j++)
			for (int i=0;i<f.ni;i++) out<<f.data[i][j][k]<<" ";
	return out;
}


typedef Field_<double> Field;
typedef Field_<v3d> Field3;


#endif