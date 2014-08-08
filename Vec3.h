#include <string>

#ifndef H_VEC3
#define H_VEC3

class Vec3
{
public:
    Vec3();
    Vec3(float x, float y, float z);

    std::string getString();

    float x();
    float y();
    float z();

    float val[3];

    Vec3& operator=(Vec3 arg);
};

#endif
