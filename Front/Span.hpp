#pragma once

#include <string>

/**
 * Represents a point in the source code.
 */
struct Point
{
    int line;
    int col;
    Point(int line, int col): line(line), col(col) {}

    std::string to_string() const;

    friend Point operator+(const Point& a, const Point& b);
    friend bool operator<(const Point& a, const Point& b);
    friend bool operator<=(const Point& a, const Point& b);
    friend bool operator>(const Point& a, const Point& b);
    friend bool operator>=(const Point& a, const Point& b);
    friend bool operator==(const Point& a, const Point& b);
    friend bool operator!=(const Point& a, const Point& b);
};

/** 
 * Represents a span of points in the source code.
 */
struct Span
{
    Point start;
    Point end;
    Span(): start(Point(0, 0)), end(Point(0, 0)) {}
    Span(const Point start, const Point end): start(start), end(end) {}
    friend Span operator+(const Span& a, const Span& b);
    void operator+=(const Span& s);
};