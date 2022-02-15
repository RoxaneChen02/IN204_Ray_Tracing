#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdint.h>
#include <string.h>
#include "vec4.hpp"
#include "vec3.hpp"
class mat4x4 {
    public : 
    vec4 data[4];

    mat4x4();
    mat4x4(vec4 v1, vec4 v2, vec4 v3, vec4 v4);
    mat4x4(float array[4][4]);
    mat4x4(const mat4x4& m);

    vec4& operator[](int index);
};

inline mat4x4::mat4x4() {
    data[0] = vec4(0.0, 0.0, 0.0, 0.0);
    data[1] = vec4(0.0, 0.0, 0.0, 0.0);
    data[2] = vec4(0.0, 0.0, 0.0, 0.0);
    data[3] = vec4(0.0, 0.0, 0.0, 0.0);
}

inline mat4x4::mat4x4(vec4 v1, vec4 v2, vec4 v3, vec4 v4) {
    data[0] = v1;
    data[1] = v2;
    data[2] = v3;
    data[3] = v4;
}



inline mat4x4::mat4x4(const mat4x4& m) {
    data[0] = m.data[0];
    data[1] = m.data[1];
    data[2] = m.data[2];
    data[3] = m.data[3];
}

inline vec4& mat4x4::operator[](int index) {
    return data[index];
}

inline vec4 operator*(mat4x4 left, vec4 right) {
    vec4 result;
    result.e[0] = dot4(left[0], right);
    result.e[1]= dot4(left[1], right);
    result.e[2] = dot4(left[2], right);
    result.e[3] = dot4(left[3], right);

    return result;
}

inline mat4x4 operator*(mat4x4 left, mat4x4 right) {
    mat4x4 result;
    for (uint32_t row = 0; row < 4; ++row) {
        vec4
 rowVector = left[row];
        for (uint32_t column = 0; column < 4; ++column) {
            vec4
     columnVector = vec4 (right[0][column], right[1][column], right[2][column], right[3][column]);
            float value = dot4(rowVector, columnVector);

            result[row][column] = value;
        }
    }

    return result;
}




static const mat4x4 IdentityMatrix(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 0.0, 1.0));

inline mat4x4 ScaleMatrix(vec3 scaleVector) {
    mat4x4 result = IdentityMatrix;
    result[0][0] *= scaleVector.x();
    result[1][1] *= scaleVector.y();
    result[2][2] *= scaleVector.z();
    return result;
}

mat4x4 TranslateMatrix(vec3 translateVector) {
    mat4x4 result = IdentityMatrix;
    result[0][3] += translateVector.x();
    result[1][3] += translateVector.y();
    result[2][3] += translateVector.z();
    return result;
}



#endif