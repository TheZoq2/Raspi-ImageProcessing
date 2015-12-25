#ifndef H_ARRAY2D
#define H_ARRAY2D

#include "Vec2.h"

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
            delete[] data;
    }
    
    Array2d& operator=(const Array2d& other)
    {
        //Resize takes care of deallocation
        resize(other.width, other.height);

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
            data = nullptr;
            delete[] data;

        this->data = new T[int(width * height)];

        this->width = width;
        this->height = height;
    }

    //Set all the data 
    void initialiseAll(T defaultValue) const
    {
        for(std::size_t i = 0; i < width * height; ++i)
        {
            data[i] = defaultValue;
        }
    }
    
    T& at(int x, int y) const
    {
        return data[x*width + y];
    }
    T& at(Vec2 pos) const
    {
        return this->at(pos.val[0], pos.val[1]);
    }


    std::size_t getWidth()
    {
        return width;
    }
    std::size_t getHeight()
    {
        return height;
    }
private:
    T* data = nullptr;

    std::size_t width, height;
};

#endif
