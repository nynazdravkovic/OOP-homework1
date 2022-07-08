#ifndef CELL_H
#define CELL_H
#include<string>
using namespace std;

class Cell {
public:
	Cell(string cellInf);
	string getInf();
	void setInf(string inf);
	int isStringNumber();
private:
	string inf;
};
#endif
