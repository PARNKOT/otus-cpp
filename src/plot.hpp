#pragma once
#include <memory>
#include "shapes.hpp"
#include "gui.hpp"
#include "drawing.hpp"


using ShapePointer = std::unique_ptr<shapes::Shape>;
using ShapePointerVector = std::vector<ShapePointer>;


class Plot {
public:
    void draw(ShapePointer shape) {
        auto points = shape->points();

        ::draw(gui_, points);

        shapes_.push_back(std::move(shape));
    }



private:
    ShapePointerVector shapes_;
    std::shared_ptr<GUIStab> gui_;
};