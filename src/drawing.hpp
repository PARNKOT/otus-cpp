#pragma once
#include "shapes.hpp"

template <typename ICanDraw>
void draw(ICanDraw gui, shapes::PointVector points) {
    gui.draw(points);
}