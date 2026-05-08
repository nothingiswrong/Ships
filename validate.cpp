#include "validate.h"
#include <algorithm>
#include <qdebug.h>
#include <unordered_set>

using std::find;
using std::unordered_set;
using std::vector;

struct Ship {
    vector<Coord> coords;
    int id;
};

bool checkCrossing(vector<vector<char>>& field, int x, int y) {
    int n = field.size();
    int m = field[0].size();
    int c = 0;

    if (x > 0 && field[y][x - 1] == '*') c++;
    if (x + 1 < m && field[y][x + 1] == '*') c++;
    if (y > 0 && field[y - 1][x] == '*')c++;
    if (y + 1 < n && field[y + 1][x] == '*') c++;

    return c <= 1;
}

bool Coord::operator==(const Coord& other) const {
    return x == other.x && y == other.y;
}


vector<Coord> get_near_coords(Coord coord, int m, int n) {
    vector<Coord> coords;
    int dx[] = {0, 0, -1, 1, 1, -1, -1, 1};
    int dy[] = {-1, 1, 0, 0, 1, -1, 1, -1};
    for (int i = 0; i < 8; i++) {
        if (coord.x + dx[i] < m && coord.x + dx[i] >= 0 && coord.y + dy[i] < n && coord.y + dy[i] >= 0)
            coords.push_back(Coord{coord.x + dx[i], coord.y + dy[i]});
    }
    return coords;
}

void collect_cells_vert(vector<vector<char>>& field, vector<vector<bool>>& visited, Ship& ship, int y, int x) {
    while (y < field.size() && field[y][x] == '*' && !visited[y][x]) {
        ship.coords.push_back(Coord{x, y});
        visited[y][x] = true;
        y++;
    }
}

void collect_cells_hor(vector<vector<char>>& field, vector<vector<bool>>& visited, Ship& ship, int y, int x) {
    while (x < field[0].size() && field[y][x] == '*' && !visited[y][x]) {
        ship.coords.push_back(Coord{x, y});
        visited[y][x] = true;
        x++;
    }
}

vector<FieldError> validate_field(vector<vector<char>>& field) {
    vector<Ship> ships;
    int id = 0;
    vector<int> count(5, 0);
    int n = field.size();
    int m = field[0].size();

    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<FieldError> errors;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (field[i][j] == '*' && !visited[i][j]) {
                Ship ship;
                if (j + 1 < m && field[i][j + 1] == '*' && !visited[i][j + 1]) {
                    collect_cells_hor(field, visited, ship, i, j);
                } else {
                    collect_cells_vert(field, visited, ship, i, j);
                }
                ship.id = id++;
                ships.push_back(ship);
            }
        }
    }

    unordered_set<int> problemShipIds;
    for (const Ship& ship : ships) {
        for (const Coord& coord : ship.coords) {
            auto near_coords = get_near_coords(coord, m, n);
            for (const Coord& near_coord : near_coords) {
                if (visited[near_coord.y][near_coord.x] && find(ship.coords.begin(), ship.coords.end(), near_coord) == ship.coords.end()) {
                    problemShipIds.insert(ship.id);
                }
            }
        }
    }

    for (Ship& s : ships) {
        if (problemShipIds.count(s.id)) {
            errors.push_back(FieldError{SHIPS_CONNECTED, s.coords, 0});
        }
    }

    for (Ship& ship : ships) {
        if (problemShipIds.count(ship.id)) {
             continue;
        }

        if (ship.coords.size() > 4) {
            errors.push_back(FieldError{TOO_LONG_SHIP, ship.coords});
        } else {
            count[ship.coords.size()]++;
        }
    }

    for (int i = 1; i < 5; i++) {
        int expected = 5 - i;
        if (count[i] < expected) {
            errors.push_back(FieldError{NOT_ENOUGH_SHIPS, vector<Coord>(), i, expected - count[i]});
        } else if (count[i] > expected) {
            errors.push_back(FieldError{TOO_MANY_SHIPS, vector<Coord>(), i, count[i]});
        }
    }

    return errors;
}
