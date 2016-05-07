#ifndef DIRECTION_H
#define DIRECTION_H

#include <string>

#define STAY -1
#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7

class Direction
{
    public:
        Direction() {_direction = STAY;}
        Direction(int dir)
        {
            _direction = STAY;
            if(-1 <= dir && dir <= 7)
                _direction = dir;
        }
        virtual ~Direction() {}

        inline bool operator == (Direction dir) {return _direction == dir._direction;}
        inline bool operator != (Direction dir) {return _direction != dir._direction;}

        void nextCW() {if(_direction != -1) _direction = (_direction+1)%8;}
        void nextCCW() {if(_direction != -1) _direction = (_direction+7)%8;}
        void setTo(int dir) {if(0 <= dir && dir <= 8) _direction = dir;}
        int horizontalDirection()
        {
            if(1 <= _direction && _direction <= 3)
                return 1;
            if(5 <= _direction && _direction <= 7)
                return -1;
            return 0;
        }
        int verticalDirection()
        {
            if(_direction == 0 || _direction == 1 || _direction == 7)
                return 1;
            if(3 <= _direction && _direction <= 5)
                return -1;
            return 0;
        }

    protected:

    private:
        int _direction;
};

#endif // DIRECTION_H
