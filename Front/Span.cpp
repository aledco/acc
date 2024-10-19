#include "Span.hpp"

/**
 * Gets the string representation of the point.
 */
std::string Point::to_string() const
{
    return std::to_string(line) + ":" + std::to_string(col);
}

/**
 * Adds two points.
 */
Point operator+(const Point& a, const Point& b) 
{
     return Point(a.line + b.line, a.col + b.col);
}

/**
 * Determines if a is less than b.
 */
bool operator<(const Point& a, const Point& b)
{
    return a.line < b.line || (a.line == b.line) && a.col < b.col;
}

/**
 * Determines if a is less than or equal to b.
 */
bool operator<=(const Point& a, const Point& b)
{
    return a.line < b.line || (a.line == b.line) && a.col <= b.col;
}

/**
 * Determines if a is greater than b.
 */
bool operator>(const Point& a, const Point& b)
{
    return a.line > b.line || (a.line == b.line) && a.col > b.col;
}

/**
 * Determines if a is greater than or equal to b.
 */
bool operator>=(const Point& a, const Point& b)
{
    return a.line > b.line || (a.line == b.line) && a.col >= b.col;
}

/**
 * Determines if a is equal to b.
 */
bool operator==(const Point& a, const Point& b)
{
    return a.line == b.line && a.col == b.col;
}

/**
 * Determines if a is not equal to b.
 */
bool operator!=(const Point& a, const Point& b)
{
    return a.line != b.line || a.col != b.col;
}

/**
 * Adds two spans.
 */
Span operator+(const Span& a, const Span& b)
{
    const Point start = a.start < b.start ? a.start : b.start;
    const Point end = a.end > b.end ? a.end : b.end;
    return Span(start, end);
}

/**
 * Adds a span.
 */
void Span::operator+=(const Span& s)
{
   start = start < s.start ? start : s.start;
   end = end > s.end ? end : s.end;
}
