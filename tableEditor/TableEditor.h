#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H
#include<string>
using namespace std;
#include "Row.h"
#include "Column.h"
#include "Node.h"

class TableEditor {
public:
	TableEditor();
	int importTable(string tabel);
	string exportTable();
	void deselect();
	void selectRow(int row_position);
	void selectColumn(string col_name);
	void selectCell(int row_position, string col_name);
	void insertRow();
	void insertColumn(string col_name, Type type);//nemam
	void deleteContent();
	void editCellValue();
	void add(double value);
	void mul(double value);
	int findFirstOf(string value, string col_name);
	int findLastOf(string value, string col_name);
	int countValues(string value, string col_name);
	int countDistinctValues(string col_name);
	void sortByValue(string col_name, bool asc);
	~TableEditor();

private:
	Column* header;
	Column* headerLast;
	Row* firstRow;
	Row* lastRow;
	int selectedColumn = -1;
	Row* selectedRow = nullptr;
	Node* selectedCell = nullptr;
	int numberOfCol = 0;
	int loadHeader(string head);
	int addRow(string inf);
	string headerToString(Column* header);
	Row* findRow(int rp);
	//string roundString(string s);
	int findColumn(string col_name);
	int nothingIsSelected();
	void deleteSelectedRow();
	void deleteSelectedColumn();
	int findColInHead(string col_name);
	void sortNumbers(int col);
	void swap(Row* r1, Row* r2);
	string roundString(string s);
};

#endif // !TABLEEDITOR_H
