/*
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2004-2010 Gabor Horvath <hgabor@rawtherapee.com>
 *
 *  RawTherapee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RawTherapee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RawTherapee.  If not, see <http://www.gnu.org/licenses/>.
 *  2018-2020 Pierre Cabrera <pierre.cab@gmail.com>
 */
#pragma once

#include "multilangmgr.h"

/* ==== BaseSpot ==== */
class BaseSpot
{
public:
    // Spot shape type enumeration
    enum spotType {
        spotEllipse = 0,
        spotRectangle = 1
    };

private:
    // BaseSpot parameters
    Glib::ustring name;
    spotType shape;
    bool isexcluded;
    bool isvisible;

public:
    BaseSpot();

    // Getter/Setter functions
    Glib::ustring getSpotName()
    {
        return this->name;
    };
    void setSpotName(const Glib::ustring& name)
    {
        this->name = name;
    };
    spotType getSpotType()
    {
        return this->shape;
    };
    void setSpotType(const spotType type)
    {
        this->shape = type;
    };
    bool isSpotExcluded()
    {
        return this->isexcluded;
    };
    void setSpotExcluded(const bool excluded)
    {
        this->isexcluded = excluded;
    };
    bool isSpotVisible()
    {
        return this->isvisible;
    };
    void setSpotVisible(const bool visible)
    {
        this->isvisible = visible;
    };

    // TBD

private:
    // TBD
};
