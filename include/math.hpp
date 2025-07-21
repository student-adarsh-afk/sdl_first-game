#pragma once
#include<iostream>

struct vector2f{
    vector2f()
    :x(0.0f), y(0.0f){}
    vector2f(float p_x, float p_y)
    :x(p_x), y(p_y){}

    void print(){
        std::cout << "x: " << x << " y: " << y << std::endl;
    }
    float x, y;
};