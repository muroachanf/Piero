#include <stdlib.h> 
#include <stdio.h>
#include <math.h>
#include "point.h"

// typedef int(*distance_proc)(point* );

typedef struct _point
{
	int x;
	int y;
	// distance_proc distance;
};

int power(int x){
	return x*x;
}
int point_distance(point *p1,point *p2){
	return sqrt(power(p2->x - p1->x)+power(p2->y - p1->y));
}
point *point_new(int x,int y){
	point *p = (point*)malloc(sizeof(point));
	p->x = x;
	p->y = y;
	// p->distance = point_distance;
	return p;
}
void point_free(point*p){
	if (NULL==p)return ;
	free(p);
}


