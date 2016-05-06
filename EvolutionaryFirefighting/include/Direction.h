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
        Direction() {direction = STAY;}
        Direction(int dir)
        {
            direction = STAY;
            if(-1 <= dir && dir <= 7)
                direction = dir;
        }
        virtual ~Direction() {}

        inline bool operator == (Direction dir) {return direction == dir.direction;}
        inline bool operator != (Direction dir) {return direction != dir.direction;}

        void nextCW() {if(direction != -1) direction = (direction+1)%8;}
        void nextCCW() {if(direction != -1) direction = (direction+7)%8;}
        void setTo(int dir) {if(0 <= dir && dir <= 8) direction = dir;}
        int horizontalDirection()
        {
            if(1 <= direction && direction <= 3)
                return 1;
            if(5 <= direction && direction <= 7)
                return -1;
            return 0;
        }
        int verticalDirection()
        {
            if(direction == 0 || direction == 1 || direction == 7)
                return 1;
            if(3 <= direction && direction <= 5)
                return -1;
            return 0;
        }

    protected:

    private:
        int direction;
};

#endif // DIRECTION_H
