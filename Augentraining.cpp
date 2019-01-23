#include <iostream>
#include <stdlib.h>
#include <Windows.h>

using namespace std;

#define TEMP_SIZE 4

static int ins_retval = 0;

void m_instruction(FILE*&);

int main() {
	// open file
	FILE* adsl_file = nullptr;
	ins_retval = fopen_s(&adsl_file, "durations.txt", "r");
	// error check
	if (ins_retval != 0) {
		ins_retval = fopen_s(&adsl_file, "durations.txt", "w");
		if (ins_retval != 0) {
			cout << "Die Datei durations.txt konnte weder gefunden noch angelegt werden." << endl;
			cout << "Druecken Sie eine beliebige Taste zum Fortfahren." << endl;
			system("pause");
			return 1;
		}
		cout << "Die Datei durations.txt wurde neu angelegt." << endl << "Sie koennen diese nun fuellen." << endl;
		m_instruction(adsl_file);
		return 1;
	}

	// read information from file
	fseek(adsl_file, 0L, SEEK_END);
	int inl_size = ftell(adsl_file);
	if (inl_size == 0) {
		cout << "Die Datei durations.txt muss erst mit Informationen \nueber Ihre Trainingseinheiten gefuellt werden." << endl;
		m_instruction(adsl_file);
		return 1;
	}
	rewind(adsl_file);
	char* achl_buf = new char[inl_size];
	ins_retval = fread(achl_buf, sizeof(char), inl_size, adsl_file);
	if (ins_retval == 0) {
		cout << "Fehler beim Einlesen der Datei." << endl;
		delete[] achl_buf;
		achl_buf = nullptr;
		m_instruction(adsl_file);
		return 0;
	}

	// get the amount of numbers inside the buffer to allocate a number buffer
	int inl_numbers = 0;
	bool bol_is_number = false;
	for (int inl_i = 0; inl_i < inl_size; inl_i++) {
		if (achl_buf[inl_i] >= '0' && achl_buf[inl_i] <= '9') {
			if (!bol_is_number) {
				inl_numbers++;
			}
			bol_is_number = true;
		}
		else {
			bol_is_number = false;
		}
	}

	// allocate buffer and fill it with numbers from file
	int* ainl_number_stor = new int[inl_numbers];
	char chrl_temp[TEMP_SIZE] = { 0 };
	int inl_index = 0;
	for (int inl_i = 0; inl_i < inl_size; inl_i++) {
		if (achl_buf[inl_i] >= '0' && achl_buf[inl_i] <= '9') {
			for (int inl_j = 0; ; inl_j++, inl_i++) {
				if (achl_buf[inl_i] >= '0' && achl_buf[inl_i] <= '9') {
					chrl_temp[inl_j] = achl_buf[inl_i];
				}
				else {
					ainl_number_stor[inl_index] = strtol(chrl_temp, NULL, 10);
					inl_index++;
					memset(chrl_temp, 0, TEMP_SIZE);
					break;
				}
			}
		}
	}

	// give the complete duration of the sequence
	int inl_time_sum = 0;
	for (int inl_i = 0; inl_i < inl_numbers; inl_i++) {
		inl_time_sum += ainl_number_stor[inl_i];
	}
	cout << "Augentraining - gesamte Dauer: " << inl_time_sum/60 << " min ";
	if (inl_time_sum % 60 != 0) {
		cout << inl_time_sum % 60 << " s";
	}
	cout << endl;
	// the actual training sequence
	for (int inl_i = 0; inl_i < inl_numbers; inl_i++) {
		cout << ainl_number_stor[inl_i];
		Sleep(ainl_number_stor[inl_i] * 1000);
		// make a sound
		cout << '\a' << " fertig" << endl;
		if ((inl_i + 1)% 10 == 0) {
			cout << endl;
		}
	}
	Sleep(1000);

	// cleanup
	fclose(adsl_file);
	adsl_file = nullptr;
	delete[] achl_buf;
	achl_buf = nullptr;
	delete[] ainl_number_stor;
	ainl_number_stor = nullptr;
	return 0;
}

void m_instruction(FILE* &adsp_pointer) {
	fclose(adsp_pointer);
	adsp_pointer = nullptr;
	cout << "Dazu muessen Sie die Zeiten einzelner Einheiten in Sekunden eintippen." << endl;
	cout << "Beispiel: \n15\n15\n15\n15\n60\n60\n60\n60\n";
	cout << "Oder: \n15,15,15,15,60,60,60,60" << endl;
	//cout << "Druecken Sie eine beliebige Taste zum Fortfahren." << endl;
	system("pause");
	return;
}