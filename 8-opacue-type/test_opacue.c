#include <stdio.h>
#include <math.h>
#include "point.h"

int main(){
	point *pp = point_new(3,4);
	printf("%d", point_distance(pp));
	point_free(pp);
	
}