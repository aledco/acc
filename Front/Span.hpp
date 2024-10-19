#pragma once

/**
 * Represents a point in the source code.
 */
struct Point
{
    int line;
    int col;
    Point(int line, int col): line(line), col(col) {}
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
    Span(const Point start, const Point end): start(start), end(end) {}
    friend Span operator+(Span& a, Span& b);
};