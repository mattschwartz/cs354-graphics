#ifndef _DRAWPLANT_H_
#define _DRAWPLANT_H_

/* Functions implemented in drawplant.cpp */

enum {
    RIGHT = 1,
    LEFT = 2,
    CENTER = 4,
    TERM = 8
};

enum {
    SUMMER,
    AUTUMN,
    WINTER,
    SPRING
};

enum {
    SPRUCE,
    YEW,
    WILLOW,
    REDWOOD
};

#define TRUE 1
#define FALSE 0

void setBranchAngle(int ang);
void drawTree(int depth);
void setSeason(int season);
void setTree(int tree);
void seedRandomColor(void);

#endif	/* _DRAWPLANT_H_ */
