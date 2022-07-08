#include "Column.h"
Column::Column(){
	this->name = "";
	this->nextColumn = nullptr;
}
Column::Column(string name, Type t) {
	this->type = t;
	this->name = name;
	this->nextColumn = nullptr;
}

void Column::setName(string name){
	this->name = name;
}

void Column::setType(char t){
	this->type = (Type)t;
}

void Column::setNextColumn(Column* next){
	this->nextColumn = next;
}

string Column::getName(){
	return this->name;
}

Column* Column::getNextColumn(){
	return this->nextColumn;
}

char Column::getType(){
	return (char)this->type;
}

