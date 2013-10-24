#include <windows.h>
#include <stdio.h>

class def {
private:
	int v;
public:
	void foo(){
		printf("d ");
	}
	void foov(){
		printf("%d",v);
	}
	void base(){}
};

class abc {
private:
	int v;
public:
	void foo(){
		printf("d ");
	}
	void foov(){
		printf("%d",v);
	}
	virtual void base(){}
};
void m(){
	abc a ;
	a.foo();
}
void m1(){
	((abc*)(NULL))->foo();
}

void m2(){
	((abc*)(NULL))->foov();
}

def* create_def(){
	return (def*)(NULL);
}
void m3(){
	abc *a = (abc*)create_def();
	a->base();
}

int PASCAL WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmdline, int show)
{
	//succed
	m();
	//succed ，只要没有访问成员，即使NULL，也可以调用
	m1();
	// bang ,because member required THIS 
	// m2();
	// bang ! 即使虚拟函数没有访问任何成员变量，也需要this指针，调用的瞬间就crash了。
	m3();
}
