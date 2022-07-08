#ifndef ROW_H
#define ROW_H
#include "Node.h"
#include "Type.h"

class Row {
public:
	Row();
	~Row();
	int makeRow(string row, char c);
	void addCellToRow(string cellInf);
	void addCellToEndOfRow(Cell* c);
	Row* getNextRow();
	void setNextRow(Row* newRow);
	Row* getPreviousRow();
	void setPreviousRow(Row* row);
	string rowToString();
	Node* getFirst();
	void addFirst(Node* newCell);
	Node* getColumnAtPosition(int col);
	void deleteCellFromRow(int col);
	void mulCell(double value, int col);
	void addToCell(double value, int col);
	string returnStringAtPos(int col);
	Node* getLast();
	void setLast(Node* n);
	void setFirst(Node* n);
	string roundString(string s);
	void deleteNodes();
	//~Row();

private:
	Node* first;
	Node* last;
	Row* nextRow;
	Row* previousRow;
};
#endif // !ROW_H
