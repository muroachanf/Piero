#include <iostream>
using namespace std;

class Win {
	public :
		void *operator new(size_t size);
		void operator delete(void *ptr);
		void sth(){cout<<"sth here\n";}
};

void* Win::operator new(size_t size)
{ 	
	cout << "operator new \n";
	return (Win*)new char[size];
} 

void Win::operator delete(void* ptr)
{ 
	// assert(((WPWin*)ptr)->deletable);
	cout << "operator delete\n";
	delete [] (char*)ptr;
}

int main() 
{
	cout << "new!\n";
	// new and delete
	Win *win = new Win;
	win->sth();
	delete win;
	// not new and delete 
	cout << "-------------\n";
	cout << "stack new!\n";
	Win win2 ;
	win2.sth();
	return 0; 
}
