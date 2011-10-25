#ifndef LOCATION_H_
#define LOCATION_H_

/*
    struct for representing locations in the grid.
*/
struct Location
{
    int row, col;

    Location()
    {
        row = col = 0;
    };

    Location(int r, int c)
    {
        row = r;
        col = c;
    };

    void set(int r, int c)
    {
        row = r;
        col = c;
    }

    Location operator+(Location &l)
    {
        return Location(this->row+l.row, this->col+l.col);
    }

    Location operator-(Location &l)
    {
        return Location(this->row-l.row, this->col-l.col);
    }
};

#endif //LOCATION_H_
