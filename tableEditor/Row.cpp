#include "Row.h"
#include <iostream>
Row::Row(){
	this->first = this->last = nullptr;
	this->nextRow = nullptr;
	this->previousRow = nullptr;
}

int Row::makeRow(string row, char c){
	//std::cout << "row " << row << std::endl;
	int pos = 0;
	int wordLenght;
	int cnt=0;
	while (pos < row.size()) {
		wordLenght = 0;
		if (row[pos] == '"') {
			wordLenght++;
			pos++;
			while (pos < row.size() && row[pos] != '"') {
				pos++;
				wordLenght++;
			}
			pos++;
			wordLenght++;
			string string_test = row.substr(pos - wordLenght, wordLenght);
			addCellToRow(string_test);
			cnt++;
			pos++;
		}
		else {
			while (pos < row.size() && row[pos] != c) {
				pos++;
				wordLenght++;
			}
			string string_test = row.substr(pos - wordLenght, wordLenght);
			addCellToRow(string_test);
			cnt++;
			pos++;
		}
	}
	return cnt;
}

void Row::addCellToRow(string cellInf){
	Cell* cell = new Cell(cellInf);
	addCellToEndOfRow(cell);
}
void Row::addCellToEndOfRow(Cell* cell) {
	Node* node = new Node(cell);
	if (!first) {
		this->first = node;
		this->last = node;
	}
	else {
		this->last->next = node;
		this->last = node;		
	}
}

Row* Row::getNextRow(){
	return this->nextRow;
}
void Row::setNextRow(Row* newRow){
	nextRow = newRow;
}
Row* Row::getPreviousRow() {
	return this->previousRow;
}
void Row::setPreviousRow(Row* row) {
	this->previousRow = row;
}

string Row::rowToString(){
	Node* cur = this->first;
	string rowString="";
	while (cur) {
		rowString.append(cur->cell->getInf());
		if(cur != this->last) rowString.append(",");
		cur = cur->next;
	}
	rowString.append("\n");
	return rowString;
}

Node* Row::getFirst(){
	return this->first;
}

void Row::addFirst(Node* newCell){
	newCell->next = this->first;
	this->first = newCell;
}

Node* Row::getColumnAtPosition(int col){
	if (col == -1) return nullptr;
	Node* pom = this->first;
	for (int i = 0; i < col; i++) {
		pom = pom->next;
	}
	return pom;
}
string Row::returnStringAtPos(int col) {
	Node* node = getColumnAtPosition(col);
	return node->cell->getInf();
}

Node* Row::getLast(){
	return this->last;
}

void Row::setLast(Node* n){
	this->last = n;
}

void Row::setFirst(Node* n){
	this->first = n;
}

void Row::deleteCellFromRow(int col) {
	Node* prev = getColumnAtPosition(col-1);//dolazim do prethodnog noda 
	Node* temp = nullptr;
	if (prev == nullptr) { 
		temp = this->first;
		this->first = this->first->next;
		delete temp;
		if (this->first == nullptr) this->last = nullptr;
	}
	else {
		temp = prev->next;
		prev->next = temp->next;
		if (temp == this->last) {
			last = prev;
		}
		delete temp;
	}
}
string Row::roundString(string s) {
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



void Row::mulCell(double value, int col){
	Node* temp = getColumnAtPosition(col);
	string s = temp->cell->getInf();
	if (s.empty()) {
		s = s;
	}
	else if(temp->cell->isStringNumber()){
		double s = stod(temp->cell->getInf());
		temp->cell->setInf(roundString(to_string(value * s)));
	}
}

void Row::addToCell(double value, int col){
	Node* temp = getColumnAtPosition(col);
	string s = temp->cell->getInf();
	if (s.empty()) {
		s = s;
	}
	else if (temp->cell->isStringNumber()) {
		double s = stod(temp->cell->getInf());
		temp->cell->setInf(roundString(to_string(value + s)));
	}
}

Row::~Row(){
	deleteNodes();
}
void Row::deleteNodes(){
	Node* curr = first;
	while (curr)
	{
		Node* old = curr;
		curr = curr->next;
		delete old;
	}
}