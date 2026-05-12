#pragma once
#include <vector>
using std::vector;

enum ErrorKind {
    TOO_LONG_SHIP,
    NOT_ENOUGH_SHIPS,
    TOO_MANY_SHIPS,
    SHIPS_CONNECTED
};


bool checkCrossing(vector<vector<char>>& field, int x, int y);

struct Coord {
    int x;
    int y;

    bool operator==(const Coord& other) const;
};

struct FieldError {
    ErrorKind kind;
    vector<Coord> coords;
    int len = 0;
    int n = 0;
};

vector<FieldError> validate_field(vector<vector<char>>& field);
