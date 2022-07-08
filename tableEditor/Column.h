#ifndef COLUMN_H
#define COLUMN_H
#include "Node.h"
#include "Type.h"
class Column {
public:
	Column();
	Column(string name, Type t);
	void setName(string name);
	void setType(char t);
	void setNextColumn(Column* column);
	string getName();
	Column* getNextColumn();
	char getType();

private:
	string name;
	Type type;
	Column* nextColumn;
};
#endif 
