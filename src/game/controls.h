#pragma once

struct Controls
{
    bool left = 0;
    bool right = 0;
    bool shoot = 0;

    Controls(bool l = 0, bool r = 0, bool s = 0): left(l), right(r), shoot(s) {};
};