#include "TableEditor.h"
#include "Row.h"
#include <iostream>
#include "Column.h"
TableEditor::TableEditor(){
	this->firstRow = this->lastRow= nullptr;
	this->header = nullptr;
}

int TableEditor::addRow(string inf) {
	Row* row = new Row();
	int cnt = 0;
	cnt = row->makeRow(inf, ',');
	if (!firstRow) {
		this->firstRow = row;
		this->lastRow = row;
	}
	else {
		row->setPreviousRow(lastRow);
		this->lastRow->setNextRow(row);
		this->lastRow = row;
	}
	return cnt;
}

int TableEditor::loadHeader(string table) {
	int pos = 0;
	int startName;
	this-> numberOfCol = 0;
	//heder sredjujem 
	while (pos < table.size() && table[pos] != '\n') {
		Column* column = new Column();
		this->numberOfCol++;
		//prvo citam tip, ova petlja ce se samo jednom okrenuti
		while (pos < table.size() && table[pos] != ':') {
			if ((table[pos] != 'i') && (table[pos] != 'd') && (table[pos] != 't')) { return -1; }
			else {
				column->setType(table[pos]);
				pos++;
			}
		}
		startName = ++pos;
		while (table[pos] != ',' && table[pos] != '\n') {
			pos++;
		}

		column->setName(table.substr(startName, pos - startName));
		if (!this->header) {
			this->header = column;
			this->headerLast = column;
		}
		else {
			this->headerLast->setNextColumn(column);
			this->headerLast = column;
		}
		if (table[pos] == '\n') break;
		pos++;
	}
	pos++;
	return pos;
}

int TableEditor::importTable(string table) {
	int pos = loadHeader(table);//vraca mi gde krece sledeci red
	if (pos == -1) {
		return -1;
	}
	//posle hedera
	int whichRow = -1;
	while(pos < table.size()){
		//prvo trazim duzinu reda
		int cnt = 0;
		while (pos < table.size() && table[pos] !='\n') {
			pos++;
			cnt++;
		}
		int duzinaReda = cnt;
		string rowString = table.substr(pos-duzinaReda, duzinaReda); //uzimam string koji je taj red
		whichRow++;
		int numberOfColsRow = addRow(rowString);
		if (this->numberOfCol != numberOfColsRow) return whichRow;
		pos++; // \n
	}
	return -2;
}

//izbaci parametar ovde ako zelis
string TableEditor::headerToString(Column* header){
	string headerString = "";
	Column* cur = this->header;
	while (cur) {
		headerString.append(1,cur->getType());
		headerString.append(":");
		headerString.append(cur->getName());
		if (cur != this->headerLast) headerString.append(",");
		cur = cur->getNextColumn();
	}
	headerString.append("\n");
	return headerString;
}

string TableEditor::exportTable(){
	string stringTable = "";
	Row* cur = this->firstRow;
	stringTable.append(headerToString(this->header));
	while (cur) {
		stringTable.append(cur->rowToString());
		cur = cur->getNextRow();
	}
	return stringTable;
}

void TableEditor::deselect(){
	this->selectedColumn = -1;
	this->selectedRow = nullptr;
	this->selectedCell = nullptr;
}
int TableEditor::nothingIsSelected() {
	if ((this->selectedColumn == -1) && (this->selectedCell == nullptr) && (this->selectedRow == nullptr)) {
		return 1;
	}
	else {
		return 0;
	}
}
Row* TableEditor::findRow(int rp) {
	Row* pom = this->firstRow;
	for (int i = 0; pom && i < rp ; i++) {
		pom = pom->getNextRow();
	}
	return pom;
}

void TableEditor::selectRow(int row_position){
	if (nothingIsSelected()) {
		this->selectedRow = findRow(row_position);
	}
}

int TableEditor::findColumn(string col_name) {
	Column* pom = header;
	int temp = 0;
	while (pom) {
		if (pom->getName() == col_name)
			return temp;
		pom = pom->getNextColumn();
		temp++;
	}
	return -1;//ovde je problem ako se ovaj if nikada ne desi
}

void TableEditor::selectColumn(string col_name) {
	if (nothingIsSelected()) {
		this->selectedColumn = findColumn(col_name);
	}
}

void TableEditor::selectCell(int row_position, string col_name){
	if (nothingIsSelected()) {
		Row* pomRow = findRow(row_position);
		int pomCol = findColumn(col_name);
		this->selectedCell = pomRow->getColumnAtPosition(pomCol);
	}
}
void  TableEditor::insertRow() {
	Row* newRow = new Row();
	//dodajem celije
	cout << "Unesite vrednosti celija redom kojim su navedene" << endl;
	string row;
	getline(std::cin, row);
	newRow->makeRow(row, ' ');
	//prepovezujem ih
	if (!selectedRow) {
		newRow->setPreviousRow(this->lastRow);
		this->lastRow->setNextRow(newRow);
		this->lastRow = newRow;
	}
	else if (selectedRow == firstRow) {
		firstRow->setPreviousRow(newRow);
		newRow->setNextRow(firstRow);
		firstRow = newRow;

	}
	else{
		newRow->setNextRow(selectedRow);
		newRow->setPreviousRow(selectedRow->getPreviousRow());
		Row* pom = selectedRow->getPreviousRow();
		pom->setNextRow(newRow);
		selectedRow->setPreviousRow(newRow);
	}
	deselect();
}
void TableEditor::deleteSelectedRow() {
	Row* p = this->selectedRow->getPreviousRow();
	Row* n = this->selectedRow->getNextRow();
	if (selectedRow == firstRow) {
		Row* old = firstRow;
		old->setNextRow(nullptr);
		firstRow = n;
		firstRow->setPreviousRow(nullptr);
	}
	else {
		p->setNextRow(n);
		if (n) {
			n->setPreviousRow(p);
		}
		else {
			lastRow = p;
		}
	}
	delete selectedRow;
}

void TableEditor::deleteSelectedColumn() {
	//prvo brisem header
	Column* pom = this->header;
	Column* prev = this->header;
	for (int i = 0; i < (this->selectedColumn-1); i++) {
		prev = prev->getNextColumn();
	}
	if (this->selectedColumn==0) { 
		Column* old = header;
		header = pom->getNextColumn();
		if (header == nullptr) headerLast = nullptr;
		delete old;
	}
	else {
		pom = prev->getNextColumn();
		prev->setNextColumn(pom->getNextColumn());
		delete pom;
	}
	//onda brisem ostatak tabele
	Row* temp = this->firstRow;
	while (temp) {
		temp->deleteCellFromRow(this->selectedColumn);
		temp = temp->getNextRow();
	}
}

void TableEditor::deleteContent(){
	if (selectedCell!=nullptr) {
		selectedCell->cell->setInf("");
	}
	else if (selectedRow != nullptr) {
		deleteSelectedRow();
	}
	else if (selectedColumn > -1) {
		deleteSelectedColumn();
	}
	deselect();
}
void TableEditor::editCellValue(){
	cout << "unesite vrednost celije" << endl;
	string in;
	cin >> in;
	this->selectedCell->cell->setInf(in);
}

void TableEditor::insertColumn(string col_name, Type type) {
	//prvo dodajem header
	Column* col = new Column(col_name, type);
	if (selectedColumn == 0) {
		col->setNextColumn(header);
		header = col;
		//if (!headerLast) {//ako je header bio prazan pre
		//	headerLast = col;
		//}	
	}
	else if (selectedColumn == -1) {
		if (this->header)
		{
			this->headerLast->setNextColumn(col);
			this->headerLast = col;
		}
		else
		{
			this->header = this->headerLast = col;
		}
	}
	else {
		Column* prev = header;
		for (int i = 0; i < this->selectedColumn - 1; i++) {//idem do prethodnog
			prev = prev->getNextColumn();
		}
		col->setNextColumn(prev->getNextColumn());
		prev->setNextColumn(col);
	}
	//dodajem u svaki red
	Row* temp = firstRow;
	//prepovezujem ih
	string cellInf;

	if (type == INT || type == DECIMAL) {
		cellInf = "0";
	}
	else {
		cellInf = "";
	}
	while (temp) {
		Cell* newCell = new Cell(cellInf);
		if (selectedColumn == 0) {
			Node* newNode = new Node(newCell);
			temp->addFirst(newNode);			
		}
		else if (selectedColumn == -1)	{
			temp->addCellToEndOfRow(newCell);
		}
		else {
			Node* newNode = new Node(newCell);
			Node* a = temp->getColumnAtPosition(this->selectedColumn - 1);
			newNode->next = a->next;
			a->next = newNode;
		}
		temp = temp->getNextRow();
	}

	deselect();
}
string TableEditor::roundString(string s){
	string new_string = "";
	int i = 0;
	for (i = 0; i < s.size() && s[i] != '.'; i++) new_string += s[i];
	if (i < s.size())
	{
		new_string += s[i]; i++;
		new_string += s[i];
	}
	return new_string;
}
void TableEditor::add(double value) {
	if (selectedCell) {
		if (selectedCell->cell->isStringNumber()) {
			double s = stod(selectedCell->cell->getInf());
			this->selectedCell->cell->setInf(roundString(to_string(value + s)));
		}
	}
	else if(this->selectedColumn>-1){
		Row* temp = this->firstRow;
		while (temp) {
			temp->addToCell(value, this->selectedColumn);
			temp = temp->getNextRow();
		}
	}
}

void TableEditor::mul(double value) {
	Row* temp = this->firstRow;
	if (selectedCell) {
		if (selectedCell->cell->isStringNumber()) {
			double s = stod(selectedCell->cell->getInf());
			this->selectedCell->cell->setInf(roundString(to_string(value * s)));
		}		
	}
	else if (this->selectedColumn > -1) {
		while (temp) {
			temp->mulCell(value, this->selectedColumn);
			temp = temp->getNextRow();
		}
	}
}

int TableEditor::findFirstOf(string value, string col_name){
	Row* temp = this->firstRow;
	int count = 0;
	int col = findColumn(col_name);
	if (col == -1) { return -2; }
	while (temp) {
		if (temp->returnStringAtPos(col) == value) {
			//cout << count << endl;
			return count;
		}
		temp = temp->getNextRow();
		count++;
		
	}
	return -2;
}

int TableEditor::findLastOf(string value, string col_name) {
	Row* temp = this->firstRow;
	int nublerOfaperances = 0;
	int count = 0;
	int col = findColumn(col_name);
	if (col == -1) { return -2; }
	int numberOfRows = 0;
	while (temp) {
		temp = temp->getNextRow();
		numberOfRows++;
	}
	temp = this->lastRow;
	while (temp) {
		if (temp->returnStringAtPos(col) == value) {
			nublerOfaperances++;
			break;
		}
		temp = temp->getPreviousRow();
		count++;
	}
	if (nublerOfaperances) { return numberOfRows - count - 1; }
	else{ return -2; }
}
int TableEditor::countValues(string value, string col_name) {
	Row* temp = this->firstRow;
	int count = 0;
	int col = findColumn(col_name);
	while (temp) {
		if (temp->returnStringAtPos(col) == value) {
			count++;
		}
		temp = temp->getNextRow();
	}
	return count;
}

int TableEditor::countDistinctValues(string col_name) {
	Row* temp = this->firstRow;
	Row* temp1 = nullptr;
	int count = 0;
	int pom;
	int col = findColumn(col_name);
	while (temp) {
		if (temp == firstRow) {
			count++;
			temp = temp->getNextRow();
			continue;
		}
		temp1 = temp->getPreviousRow();
		pom = 0;
		while (temp1) {
			if (temp1->returnStringAtPos(col) == temp->returnStringAtPos(col)) {
				pom++;
			}
			temp1 = temp1->getPreviousRow();
		}
		if (pom == 0) count++;
		temp = temp->getNextRow();
	}
	return count;
}

void TableEditor::swap(Row* r1, Row* r2) {
	Node* firstTemp = r1->getFirst();
	Node* lastTemp = r1->getLast();
	r1->setFirst(r2->getFirst());
	r1->setLast(r2->getLast());
	r2->setFirst(firstTemp);
	r2->setLast(lastTemp);
}

void TableEditor::sortByValue(string col_name, bool asc){
	int column = findColumn(col_name);
	Column* a = this->header;
	int count = 0;
	while (count<column) {
		a = a->getNextColumn();
		count++;
	}
	char type = a->getType();
	bool comparingNumbers = (type == 'i') || (type == 'd');
	for (Row* i = this->firstRow; i; i = i->getNextRow()){
		for (Row* j = i->getNextRow(); j; j = j->getNextRow()){
			string value1 = i->getColumnAtPosition(column)->cell->getInf();
			string value2 = j->getColumnAtPosition(column)->cell->getInf();
			if (comparingNumbers) {
				double valNumber1 = std::stod(value1);
				double valNumber2 = std::stod(value2);
				if (asc) {
					if (valNumber2 < valNumber1)	{
						swap(i, j);
					}
				}
				else {
					if (valNumber2 > valNumber1) {
						swap(i, j);
					}
				}
			}
			else {
				if (asc) {
					if (value1 > value2) {
						swap(i, j);
					}
				}
				else {
					if (value1 < value2) {
						swap(i, j);
					}
				}
			}
		}
	}
}


TableEditor::~TableEditor()
{
	Row* curr = firstRow;
	while (curr)
	{
		Row* old = curr;
		curr = curr->getNextRow();
		delete old;
	}

	Column* currColumn = header;
	while (currColumn)
	{
		Column* oldColumn = currColumn;
		currColumn = currColumn->getNextColumn();
		delete oldColumn;
	}
}

