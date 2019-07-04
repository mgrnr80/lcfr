#pragma once

#include "lcfr/crypto/fp.h"

namespace lcfr {

template <unsigned NB, class W = uint32_t>
struct ec_point
{
    ui<NB, W> x;
    ui<NB, W> y;

    static const unsigned WB = 8 * sizeof(W);
    static const unsigned NW = (NB + WB - 1) / WB;

public:
    ec_point()
        : x(W(-1)),
          y(W(-1))
    {}

    ec_point(const ui<NB, W>& x_, const ui<NB, W>& y_)
        : x(x_),
          y(y_)
    {}

    ec_point(const ec_point& p)
        : x(p.x),
          y(p.y)
    {}

    bool is_zero() const
    {
        return lcfr::eq(x, ui<NB, W>(W(-1)), NW);
    }

    ec_point& operator = (const ec_point& other)
    {
        lcfr::set(x, other.x, NW);
        lcfr::set(y, other.y, NW);
        return *this;
    }
};

template <unsigned NB, class W = uint32_t>
struct ec_point_p
{
    ui<NB, W> x;
    ui<NB, W> y;
    ui<NB, W> z;

    static const unsigned WB = 8 * sizeof(W);
    static const unsigned NW = NB / WB;

public:
    ec_point_p()
        : x(W(0)),
          y(W(0)),
          z(W(0))
    {}

    ec_point_p(const ui<NB, W>& x_, const ui<NB, W>& y_)
        : x(x_),
          y(y_),
          z(W(1))
    {}

    ec_point_p(const ui<NB, W>& x_, const ui<NB, W>& y_, const ui<NB, W>& z_)
        : x(x_),
          y(y_),
          z(z_)
    {}

    ec_point_p(const ec_point_p& p)
        : x(p.x),
          y(p.y),
          z(p.z)
    {}

    bool is_zero() const
    {
        return lcfr::eq(z, ui<NB, W>::ZERO, NW);
    }

    ec_point_p& operator = (const ec_point_p& other)
    {
        lcfr::set(x, other.x, NW);
        lcfr::set(y, other.y, NW);
        lcfr::set(z, other.z, NW);
        return *this;
    }
};

}
