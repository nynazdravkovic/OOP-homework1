#include "Cell.h"
Cell::Cell(string cellInf){
	this->inf = cellInf;
}
string Cell::getInf()
{
	return this->inf;
}
void Cell::setInf(string inf){
	this->inf = inf;
}

int Cell::isStringNumber() {
	string s = this->inf;
	char pom = s[0];
	int i = 0;
	while (pom) {
		if (isalpha(pom)) {
			return 0;
		}
		i++;
		pom = s[i];
	}
return 1;
}
