#ifndef UTILS_H
#define UTILS_H

/**
 * GLSL-like three dimensional vector
 */
typedef struct vec3
{
    float x;
    float y;
    float z;
} vec3;

/**
 * Color with RGB components
 */
typedef struct Color
{
    float red;
    float green;
    float blue;
} Color;

/**
 * Material
 */
typedef struct Material
{
    struct Color ambient;
    struct Color diffuse;
    struct Color specular;
    float shininess;
} Material;

/**
 * Calculates radian from degree.
 */
double degree_to_radian(double degree);

/**
 * Calculates the length of a vector.
 * @param v Vector
 */
float length(const vec3* v);

/**
 * Normalize the vector.
 * @param v Vector
 */
vec3 normalized(const vec3* v);

/**
 * Add two vectors.
 * @param v1 Vector 1
 * @param v2 Vector 2
 */
vec3 add(const vec3* v1, const vec3* v2);

/**
 * Subtract two vectors.
 * @param v1 Vector 1
 * @param v2 Vector 2
 */
vec3 subtract(const vec3* v1, const vec3* v2);

/**
 * Multiply a vector by a scalar.
 * @param v   Vector
 * @param scalar  Scalar
 * @return vec3  Resulting vector
 */
vec3 multiply(const vec3* v, float scalar);

#endif /* UTILS_H */
