#ifndef H_ARRAY2D
#define H_ARRAY2D

#include "Vec2.h"

#include <stdexcept>
#include <iostream>

template<class T>
class Array2d
{
public:
    Array2d():
        Array2d(0,0)
    {
    }
    Array2d(int width, int height)
    {
        //Allocate memory for the data
        resize(width, height);
    }
    Array2d(const Array2d& other)
    {
        *this = other;
    }
    ~Array2d()
    {
        if(data == nullptr)
        {
            delete[] data;
        }
    }
    
    Array2d& operator=(const Array2d& other)
    {
        //Resize takes care of deallocation
        this->resize(other.width, other.height);

        //Copy all the data aswell
        for(std::size_t i = 0; i < width * height; ++i)
        {
            this->data[i] = other.data[i];
        }

        return *this;
    }

    void resize(std::size_t width, std::size_t height)
    {
        if(data != nullptr)
        {
            delete[] data;
            data = nullptr;
        }

        this->data = new T[width * height];

        this->width = width;
        this->height = height;
    }

    //Set all the data 
    void initialiseAll(T defaultValue) const
    {
        //for(std::size_t i = 0; i < width * height; ++i)
        //{
        //    data[i] = defaultValue;
        //}
        std::fill_n(data, width*height, defaultValue);
    }
    
    T& at(std::size_t x, std::size_t y)
    {
        if(x < 0 || x >= width || y < 0 || y >= height)
        {
            throw std::out_of_range("Coordinates out of range");
        }
        return data[(y)*width + x];
    }
    T& at(Vec2 pos) const
    {
        return this->at(pos.val[0], pos.val[1]);
    }


    std::size_t getWidth() const
    {
        return width;
    }
    std::size_t getHeight() const
    {
        return height;
    }
private:
    T* data = nullptr;

    std::size_t width, height;
};

#endif
