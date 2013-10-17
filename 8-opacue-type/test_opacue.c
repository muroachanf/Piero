#include <stdio.h>
#include <math.h>
#include "point.h"

int main(){
	point *p1 = point_new(3,4);
	point *p2 = point_new(6,9);
	// printf("\n");
	printf("%d\n", point_distance(p1,p2));
	point_free(p1);
	point_free(p2);
	
}