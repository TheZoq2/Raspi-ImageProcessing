#ifndef H_ARRAY2D
#define H_ARRAY2D

#include "Vec2.h"

template<class T>
class Array2d
{
public:
    Array2d(int width, int height)
    {
        //Allocate memory for the data
        this->data = new T[int(width * height)];
    }

    //Set all the data 
    void initialiseAll(T defaultValue) const
    {
        for(std::size_t i = 0; i < width * height; i++)
        {
            data[i] = defaultValue;
        }
    }
    
    T& at(int x, int y)
    {
        return data[x*width + y];
    }
    T& at(Vec2 pos)
    {
        return this->at(pos.val[0], pos.val[1]);
    }


    
private:
    T* data;

    std::size_t width, height;
};

#endif
