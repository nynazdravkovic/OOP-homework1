#include "TableEditor.h"
#include "Type.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

int tests_public_passed = 0;

// Funkcija koja poredi Vas izlaz sa ocekivanim izlazom za odgovarajuci test primer
void checkOutput(const string& student_output_filename, int test_number) {

	ifstream correct_output("correct_output_" + to_string(test_number) + ".txt");
	stringstream ss_correct;
	ss_correct << correct_output.rdbuf(); correct_output.close();
	string string_correct = ss_correct.str();

	ifstream student_output(student_output_filename);
	stringstream ss_student;
	ss_student << student_output.rdbuf(); student_output.close();
	string string_student = ss_student.str();
	
	if (string_student != "" && string_correct == string_student) {
		cout << "Test " + to_string(test_number) + ": PASSED" << endl;
		tests_public_passed++;
	}
	else {
		cout << "Test " + to_string(test_number) + ": FAILED" << endl;
	}

}

void test(int test_number) {

	fstream student_output("student_output_" + to_string(test_number) + ".txt", ios::out);

	fstream input("input.txt");
	stringstream ss;
	ss << input.rdbuf(); input.close();
	string string_input = ss.str();

	TableEditor* editor = new TableEditor;
	int res = 1;
	switch (test_number) {
	case 1:
		// IMPORT / EXPORT
		res = editor->importTable(string_input);
		cout << "Ucitavanje tabele " << res << endl;
		student_output << editor->exportTable();
		break;
	case 2:
		// IMPORT / INSERT ROW AT THE END / EXPORT
		editor->importTable(string_input);
		editor->insertRow(); // Standardni ulaz: Interstellar 2014 8.6 NE
		student_output << editor->exportTable();
		break;
	case 3:
		// IMPORT / INSERT ROW BEHIND SELECTED ROW / EXPORT
		editor->importTable(string_input);
		editor->selectRow(3);
		editor->insertRow(); // Standardni ulaz: Interstellar 2014 8.6 NE
		student_output << editor->exportTable();
		break;
	case 4:
		// IMPORT / INSERT COLUMN AT THE END / EXPORT
		editor->importTable(string_input);
		editor->insertColumn("trajanje", Type::INT); 
		student_output << editor->exportTable();
		break;
	case 5:
		// IMPORT / INSERT COLUMN BEHIND SELECTED ROW / EXPORT
		editor->importTable(string_input);
		editor->selectColumn("godina");
		editor->insertColumn("opis", Type::TEXT);
		student_output << editor->exportTable();
		break;
	case 6:
		// IMPORT / DELETE ROW / DELETE COLUMN / DELETE CELL / EXPORT
		editor->importTable(string_input);
		editor->selectRow(8); editor->deleteContent(); cout << "red" << endl;
		editor->selectColumn("godina"); editor->deleteContent(); cout << "kolona" << endl;
		editor->selectCell(2, "ocena"); editor->deleteContent(); cout << "celija" << endl;
		student_output << editor->exportTable();
		break;
	case 7:
		// IMPORT / EDIT CELL / EXPORT
		editor->importTable(string_input);
		editor->selectCell(2, "ocena"); editor->editCellValue(); // Ulaz: 6.2
		student_output << editor->exportTable();
		break;
	case 8:
		// IMPORT / ADD VALUE TO CELL / EXPORT
		editor->importTable(string_input);
		editor->selectCell(1, "ocena");
		editor->add(1);
		student_output << editor->exportTable();
		break;
	case 9:
		// IMPORT / MUL COLUMN VALUES / EXPORT
		editor->importTable(string_input);
		editor->selectColumn("ocena");
		editor->mul(0.5);
		student_output << editor->exportTable();
		break;
	case 10:
		// IMPORT / SORT / EXPORT
		editor->importTable(string_input);
		editor->sortByValue("godina", true);
		student_output << editor->exportTable();
		break;
	case 11:
		// IMPORT / FIND FIRST OF / EXPORT
		editor->importTable(string_input);
		student_output << "Prvo pojavljivanje 2021. godine je u redu: " << editor->findFirstOf("2021", "godina");
		break;
	case 12:
		// IMPORT / FIND LAST OF / EXPORT
		editor->importTable(string_input);
		student_output << "Poslednje pojavljivanje 2021. godine je u redu: " << editor->findLastOf("2021", "godina");
		break;
	case 13:
		// IMPORT / COUNT VALUES / EXPORT
		editor->importTable(string_input);
		student_output << "Broj pojavljivanja 2021. godine je: " << editor->countValues("2021", "godina");
		break;
	case 14:
		// IMPORT / COUNT DISTINCT VALUES / EXPORT
		editor->importTable(string_input);
		student_output << "Broj razlicitih godina u tabeli je: " << editor->countDistinctValues("godina");
		break;
	}

	delete editor;
	student_output.close();

	checkOutput("student_output_" + to_string(test_number) + ".txt", test_number);
}

int main() {

	// Izmeniti elemente niza u zavisnosti od testova koje zelite da pokrenete!
	int active_tests[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
	int number_of_tests = sizeof(active_tests) / sizeof(int);

	cout << "============================================" << endl;
	cout << "TESTING TABLE_EDITOR" << endl << endl;
	cout << "Executing tests: ";
	for (int i = 0; i < number_of_tests; i++) cout << active_tests[i] << (i < number_of_tests - 1 ? ", " : "");
	cout << endl;

	for (int i = 0; i < number_of_tests; i++) test(active_tests[i]);

	cout << "============================================" << endl;
	cout << "Tests public tests passed count: " << tests_public_passed << endl;

	return 0;
}