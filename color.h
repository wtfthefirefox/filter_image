#pragma once

struct Color {
public:
    float red;
    float green;
    float blue;

    Color() : red(0), green(0), blue(0) {};
    Color(float r, float g, float b) : red(r), green(g), blue(b) {};

    ~Color() {};
};