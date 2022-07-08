#ifndef NODE_H
#define NODE_H
#include "Cell.h"
class Node {
public:
	Node(Cell* cell);
	~Node();
	Cell* cell = nullptr;
	Node* next = nullptr;
};
#endif 

