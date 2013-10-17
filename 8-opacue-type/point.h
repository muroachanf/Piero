
struct _point;
typedef struct _point point;


point *point_new(int x,int y);
void point_free(point*p);
int point_distance(point * p1,point *p2);