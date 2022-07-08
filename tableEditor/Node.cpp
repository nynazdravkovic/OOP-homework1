#include "Node.h"
Node::Node(Cell* cell){
	this->cell = cell;
	this->next = nullptr;
}

Node::~Node(){
	delete cell;
}
