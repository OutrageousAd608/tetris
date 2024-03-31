#ifndef TETRIS_H
#define TETRIS_H

#define WHITE   "\x1b[0m"       /* White*/
#define BLUE    "\x1b[34m"      /* Blue */
#define GREEN   "\x1b[32m"      /* Green */
#define PURPLE  "\x1b[35m"      /* Purple */
#define RED     "\x1b[31m"      /* Red */
#define BLACK   "\x1b[30m"      /* Black */
#define YELLOW  "\x1b[33m"      /* Yellow */
#define CYAN    "\x1b[36m"      /* Cyan */

enum class color{
    blue = 0,
    green,
    purple,
    red,
    black,
    yellow,
    cyan,
    white,
};

enum class shape{
    i = 0,
    j,
    l,
    o,
    s,
    t,
    z,
};

class block{
    public: 
        int x; //for x axis
        int y; //for y axis
        color blockColor;

        block(int _x, int _y, color _blockColor){
            x = _x;
            y = _y;
            blockColor = _blockColor;
        }

        ~block(){
            
        }
};

class figure{
    public:
        block* first;
        block* second; //center of block
        block* third;
        block* fourth;
        shape figureShape;
        color figureColor;
        figure* shadow;

        figure(){
            first = new block(0, 0, (color)0);
            second = new block(0, 0, (color)0);
            third = new block(0, 0, (color)0);
            fourth = new block(0, 0, (color)0);
            figureShape = (shape)0;
            figureColor = (color)0;
            shadow = nullptr;
        }

        ~figure(){
            delete first;
            delete second;
            delete third;
            delete fourth;
        }

};

#endif
