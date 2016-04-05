//#############################################################################
//
//  This file is part of ImagePlay.
//
//  ImagePlay is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ImagePlay is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ImagePlay.  If not, see <http://www.gnu.org/licenses/>.
//
//#############################################################################

#include "IPLPoint.h"

IPLPoint::IPLPoint() : IPLData(IPL_POINT)
{
}

IPLPoint::IPLPoint(double x, double y) : IPLPoint()
{
    _x = x;
    _y = y;
}

IPLPoint::IPLPoint(const IPLPoint &other):
    IPLData(other)
{
    _x = other._x;
    _y = other._y;
}

IPLPoint &IPLPoint::operator=(const IPLPoint &other)
{
    _x = other._x;
    _y = other._y;
    return *this;
}

IPLPoint::~IPLPoint()
{

}

std::string IPLPoint::toString(int x, int y)
{
    std::stringstream s;
    s << "X: " << x << " Y: " << y;

    return s.str();
}
