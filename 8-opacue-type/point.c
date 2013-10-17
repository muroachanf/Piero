#include <stdlib.h> 
#include <stdio.h>
#include <math.h>
#include "point.h"
typedef int(*distance_proc)(point* );

typedef struct _point
{
	int x;
	int y;
	distance_proc distance;
};



int point_distance(point *self){
	return sqrt(pow(self->x,2)+pow(self->y,2));
}
point *point_new(int x,int y){
	point *p = (point*)malloc(sizeof(point));
	p->x = x;
	p->y = y;
	p->distance = point_distance;
	return p;
}
void point_free(point*p){
	if (NULL==p)return ;
	free(p);
}
