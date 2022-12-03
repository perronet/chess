#pragma once

enum Player { White, Black, None};

class Position {
    public:
        int i;
        int j;

    bool operator==(const Position& rhs) const { 
        return i == rhs.i && j == rhs.j; 
    }

    bool operator!=(const Position& rhs) const { 
        return i == rhs.i && j == rhs.j; 
    }
};