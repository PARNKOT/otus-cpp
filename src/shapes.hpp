#pragma once
#include <cstdint>
#include <vector>

namespace shapes {
    struct Point {
        int32_t x;
        int32_t y;
    };

    using PointVector = std::vector<Point>;

    struct ShapeDescription {
        PointVector points;
        int32_t radius = -1;
    };

    class Shape {
    public:
        const PointVector& points() const noexcept {
            return desc_.points;
        }

        decltype(ShapeDescription::radius) radius() {
            return desc_.radius;
        }

        bool is_drawen() {
            return is_drawen_;
        }

    protected:
        ShapeDescription desc_;
        bool is_drawen_ = false;
    };

    class Line : public Shape {
    public:
        Line(Point point1, Point point2) {
            desc_.points.push_back(point1);
            desc_.points.push_back(point2);
        }
    private:
    };

    class Triangle : public Shape {
    public:
        Triangle(Point point1, Point point2, Point point3) {
            desc_.points.push_back(point1);
            desc_.points.push_back(point2);
            desc_.points.push_back(point3);
        }
    private:
    };


    class Circle : public Shape {
    public:
        Circle(Point point1, decltype(ShapeDescription::radius) radius) {
            desc_.points.push_back(point1);
            desc_.radius = radius;
        }
    private:
    };


} // namespace shapes
