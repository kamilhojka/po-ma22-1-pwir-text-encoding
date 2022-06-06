// pwir-text-encoding.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>
#include <vector>
#include <omp.h>

#define MAX_TAB_SIZE 1000 

using namespace std;

void ShowIntroInformation(HANDLE hConsole);
void SetSortOption(HANDLE hConsole, int& sort);
void SetText(HANDLE hConsole, char text[MAX_TAB_SIZE]);
void SetKeyValue(HANDLE hConsole, int& key);
void SetDelayValue(HANDLE hConsole, int& delay);
void SetDisplayValue(HANDLE hConsole, int& display);
void SetRepeatValue(HANDLE hConsole, int& repeat);
void RunEncodeTextMethod(HANDLE hConsole, char text[MAX_TAB_SIZE], int key, int delay, int display);
void RunEncodeTextMethodParallel(HANDLE hConsole, char text[MAX_TAB_SIZE], int key, int delay, int display);
void RunEncodeTextMethodParallelOpenMP(HANDLE hConsole, char text[MAX_TAB_SIZE], int key, int delay, int display);
void RunDecodeTextMethod(HANDLE hConsole, char text[MAX_TAB_SIZE], int key, int delay, int display);
void RunDecodeTextMethodParallel(HANDLE hConsole, char text[MAX_TAB_SIZE], int key, int delay, int display);
void RunDecodeTextMethodParallelOpenMP(HANDLE hConsole, char text[MAX_TAB_SIZE], int key, int delay, int display);

int main()
{
	setlocale(LC_CTYPE, "Polish");
	char text[MAX_TAB_SIZE], tmptext[MAX_TAB_SIZE];
	int sort, key, delay, display, repeat;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ShowIntroInformation(hConsole);
	while (true)
	{
		SetSortOption(hConsole, sort);
		cin.ignore(MAX_TAB_SIZE, '\n');
		SetText(hConsole, text);
		SetKeyValue(hConsole, key);
		SetDelayValue(hConsole, delay);
		SetDisplayValue(hConsole, display);
		if (sort == 1)
		{
			memcpy(tmptext, text, MAX_TAB_SIZE);
			RunEncodeTextMethod(hConsole, tmptext, key, delay, display);
			memcpy(tmptext, text, MAX_TAB_SIZE);
			RunEncodeTextMethodParallel(hConsole, tmptext, key, delay, display);
			memcpy(tmptext, text, MAX_TAB_SIZE);
			RunEncodeTextMethodParallelOpenMP(hConsole, tmptext, key, delay, display);
		}
		else if (sort == 2)
		{
			memcpy(tmptext, text, MAX_TAB_SIZE);
			RunDecodeTextMethod(hConsole, tmptext, key, delay, display);
			memcpy(tmptext, text, MAX_TAB_SIZE);
			RunDecodeTextMethodParallel(hConsole, tmptext, key, delay, display);
			memcpy(tmptext, text, MAX_TAB_SIZE);
			RunDecodeTextMethodParallelOpenMP(hConsole, tmptext, key, delay, display);
		}
		SetRepeatValue(hConsole, repeat);
		if (repeat == 0) break;
	}
}

void ShowIntroInformation(HANDLE hConsole)
{
	SetConsoleTextAttribute(hConsole, 11);
	for (int i = 0; i < 70; i++) cout << '*';
	SetConsoleTextAttribute(hConsole, 3);
	cout << "\n\n  PROGRAMOWANIE WSP”£BIEØNE I ROZPROSZONE 21/22L\n  Rozwiπzanie rÛwnania rÛøniczkowego zwyczajnego\n  Autor programu: ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << "Kamil Hojka -- 97632\n\n";
	SetConsoleTextAttribute(hConsole, 11);
	for (int i = 0; i < 70; i++) cout << '*';
	cout << "\n";
	SetConsoleTextAttribute(hConsole, 15);
}

void SetSortOption(HANDLE hConsole, int& sort)
{
	SetConsoleTextAttribute(hConsole, 14);
	cout << "\n -> Opcje:";
	cout << "\n --> [1] Szyfrowanie (Encode)";
	cout << "\n --> [2] Deszyfrowanie (Decode)";
	while (true) {
		SetConsoleTextAttribute(hConsole, 14);
		cout << "\n --> Wybierz spoúrÛd dostÍpnych opcji: ";
		SetConsoleTextAttribute(hConsole, 15);
		cin >> sort;
		if (cin.good() && (sort == 1 || sort == 2)) break;
		SetConsoleTextAttribute(hConsole, 4);
		cout << "    ! WartoúÊ musi byÊ liczbπ rÛwnπ jednej z dostÍpnych opcji\n";
		SetConsoleTextAttribute(hConsole, 15);
		cin.clear();
		cin.ignore();
	}
}

void SetText(HANDLE hConsole, char text[MAX_TAB_SIZE])
{
	while (true) {
		SetConsoleTextAttribute(hConsole, 14);
		cout << "\n -> Wprowadü tekst ";
		SetConsoleTextAttribute(hConsole, 4);
		cout << "[Limit znakÛw: 1000]: ";
		SetConsoleTextAttribute(hConsole, 15);
		cin.getline(text, MAX_TAB_SIZE);
		if (cin.good()) break;
		SetConsoleTextAttribute(hConsole, 4);
		cout << "    ! Wystπpi≥ b≥πd podczas wprowadzenia tekstu, sprÛbuj jeszcze raz\n";
		SetConsoleTextAttribute(hConsole, 15);
		cin.clear();
		cin.ignore();
	}
}

void SetKeyValue(HANDLE hConsole, int& key)
{
	while (true) {
		SetConsoleTextAttribute(hConsole, 14);
		cout << "\n -> Podaj wartoúÊ klucza? [-26, ..., 26]: ";
		SetConsoleTextAttribute(hConsole, 15);
		cin >> key;
		if (cin.good() && (key <= 26 && key >= -26)) break;
		SetConsoleTextAttribute(hConsole, 4);
		cout << "    ! WartoúÊ klucza musi mieúciÊ siÍ w przedziale [-26, ..., 26]\n";
		SetConsoleTextAttribute(hConsole, 15);
		cin.clear();
		cin.ignore();
	}
}

void SetDelayValue(HANDLE hConsole, int& delay)
{
	while (true) {
		SetConsoleTextAttribute(hConsole, 14);
		cout << "\n -> Podaj opÛünienie? [ms]: ";
		SetConsoleTextAttribute(hConsole, 15);
		cin >> delay;
		if (cin.good() && delay >= 0) break;
		SetConsoleTextAttribute(hConsole, 4);
		cout << "    ! WartoúÊ opÛünienia musi byÊ liczbπ naturalnπ {0, 1, 2, ...}\n";
		SetConsoleTextAttribute(hConsole, 15);
		cin.clear();
		cin.ignore();
	}
}

void SetDisplayValue(HANDLE hConsole, int& display)
{
	while (true) {
		SetConsoleTextAttribute(hConsole, 14);
		cout << "\n -> Czy wyúwietliÊ wynik? [1/0]: ";
		SetConsoleTextAttribute(hConsole, 15);
		cin >> display;
		if (cin.good() && (display == 0 || display == 1)) break;
		SetConsoleTextAttribute(hConsole, 4);
		cout << "    ! WartoúÊ musi byÊ liczbπ 0 lub 1\n";
		SetConsoleTextAttribute(hConsole, 15);
		cin.clear();
		cin.ignore();
	}
}

void SetRepeatValue(HANDLE hConsole, int& repeat)
{
	while (true) {
		SetConsoleTextAttribute(hConsole, 14);
		cout << "\n -> Czy powtÛrzyÊ program? [1/0]: ";
		SetConsoleTextAttribute(hConsole, 15);
		cin >> repeat;
		if (cin.good() && (repeat == 0 || repeat == 1)) break;
		SetConsoleTextAttribute(hConsole, 4);
		cout << "    ! WartoúÊ musi byÊ liczbπ 0 lub 1\n";
		SetConsoleTextAttribute(hConsole, 15);
		cin.clear();
		cin.ignore();
	}
}

void EncodeText(char text[MAX_TAB_SIZE], int key, int delay, int i)
{
	bool isGood = false;
	char a, z;
	this_thread::sleep_for(std::chrono::milliseconds(delay));
	if (text[i] >= 'a' && text[i] <= 'z') {
		a = 'a', z = 'z';
		isGood = true;
	}
	else if (text[i] >= 'A' && text[i] <= 'Z') {
		a = 'A', z = 'Z';
		isGood = true;
	}

	if (isGood) {
		if (key >= 0)
		{
			if (text[i] + key <= z) text[i] += key;
			else text[i] = text[i] + key - 26;
		}
		else {
			if (text[i] + key >= a) text[i] += key;
			else text[i] = text[i] + key + 26;
		}
	}
}

void RunEncodeTextMethod(HANDLE hConsole, char text[MAX_TAB_SIZE], int key, int delay, int display)
{
	cout << "\n\n";
	SetConsoleTextAttribute(hConsole, 11);
	for (int i = 0; i < 70; i++) cout << '*';
	SetConsoleTextAttribute(hConsole, 3);
	cout << "\n ---> Sekwencyjne szyfrowanie tekstu - Szyfr Cezara\n";
	SetConsoleTextAttribute(hConsole, 15);
	auto begin = chrono::high_resolution_clock::now();
	for (int i = 0; i < strlen(text); i++)
	{
		EncodeText(text, key, delay, i);
	}
	auto end = chrono::high_resolution_clock::now();
	auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	if (display)
	{
		cout << "\n Zaszyfrowany tekst: ";
		SetConsoleTextAttribute(hConsole, 14);
		cout << text;
		SetConsoleTextAttribute(hConsole, 15);
	}
	cout << "\n\n Zmierzony czas: " << elapsed << " ms\n";
}

void RunEncodeTextMethodParallel(HANDLE hConsole, char text[MAX_TAB_SIZE], int key, int delay, int display)
{
	cout << "\n\n";
	SetConsoleTextAttribute(hConsole, 11);
	for (int i = 0; i < 70; i++) cout << '*';
	SetConsoleTextAttribute(hConsole, 3);
	cout << "\n ---> RÛwnoleg≥e szyfrowanie tekstu za pomocπ thread - Szyfr Cezara\n";
	SetConsoleTextAttribute(hConsole, 15);
	auto begin = chrono::high_resolution_clock::now();
	vector<thread> threads(strlen(text));
	for (int i = 0; i < threads.size(); i++)
	{
		threads[i] = thread(EncodeText, text, key, delay, i);
	}
	for (auto& thread : threads)
	{
		thread.join();
	}
	auto end = chrono::high_resolution_clock::now();
	auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	if (display)
	{
		cout << "\n Zaszyfrowany tekst: ";
		SetConsoleTextAttribute(hConsole, 14);
		cout << text;
		SetConsoleTextAttribute(hConsole, 15);
	}
	cout << "\n\n Zmierzony czas: " << elapsed << " ms\n";
}

void RunEncodeTextMethodParallelOpenMP(HANDLE hConsole, char text[MAX_TAB_SIZE], int key, int delay, int display)
{
	cout << "\n\n";
	SetConsoleTextAttribute(hConsole, 11);
	for (int i = 0; i < 70; i++) cout << '*';
	SetConsoleTextAttribute(hConsole, 3);
	cout << "\n ---> RÛwnoleg≥e szyfrowanie tekstu za pomocπ OpenMP - Szyfr Cezara\n";
	SetConsoleTextAttribute(hConsole, 15);
	auto begin = chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(static, 1)
	for (int i = 0; i < strlen(text); i++)
	{
		EncodeText(text, key, delay, i);
	}
	auto end = chrono::high_resolution_clock::now();
	auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	if (display)
	{
		cout << "\n Zaszyfrowany tekst: ";
		SetConsoleTextAttribute(hConsole, 14);
		cout << text;
		SetConsoleTextAttribute(hConsole, 15);
	}
	cout << "\n\n Zmierzony czas: " << elapsed << " ms\n";
}

void DecodeText(char text[MAX_TAB_SIZE], int key, int delay, int i)
{
	bool isGood = false;
	char a, z;
	this_thread::sleep_for(std::chrono::milliseconds(delay));
	if (text[i] >= 'a' && text[i] <= 'z') {
		a = 'a', z = 'z';
		isGood = true;
	}
	else if (text[i] >= 'A' && text[i] <= 'Z') {
		a = 'A', z = 'Z';
		isGood = true;
	}

	if (isGood) {
		if (key >= 0)
		{
			if (text[i] - key >= a) text[i] -= key;
			else text[i] = text[i] - key + 26;
		}
		else {
			if (text[i] - key <= z) text[i] -= key;
			else text[i] = text[i] - key - 26;
		}
	}
}

void RunDecodeTextMethod(HANDLE hConsole, char text[MAX_TAB_SIZE], int key, int delay, int display)
{
	cout << "\n\n";
	SetConsoleTextAttribute(hConsole, 11);
	for (int i = 0; i < 70; i++) cout << '*';
	SetConsoleTextAttribute(hConsole, 3);
	cout << "\n ---> Sekwencyjne deszyfrowanie tekstu - Szyfr Cezara\n";
	SetConsoleTextAttribute(hConsole, 15);
	auto begin = chrono::high_resolution_clock::now();
	for (int i = 0; i < strlen(text); i++)
	{
		DecodeText(text, key, delay, i);
	}
	auto end = chrono::high_resolution_clock::now();
	auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	if (display)
	{
		cout << "\n Zdeaszyfrowany tekst: ";
		SetConsoleTextAttribute(hConsole, 14);
		cout << text;
		SetConsoleTextAttribute(hConsole, 15);
	}
	cout << "\n\n Zmierzony czas: " << elapsed << " ms\n";
}

void RunDecodeTextMethodParallel(HANDLE hConsole, char text[MAX_TAB_SIZE], int key, int delay, int display)
{
	cout << "\n\n";
	SetConsoleTextAttribute(hConsole, 11);
	for (int i = 0; i < 70; i++) cout << '*';
	SetConsoleTextAttribute(hConsole, 3);
	cout << "\n ---> RÛwnoleg≥e deszyfrowanie tekstu za pomocπ thread - Szyfr Cezara\n";
	SetConsoleTextAttribute(hConsole, 15);
	auto begin = chrono::high_resolution_clock::now();
	vector<thread> threads(strlen(text));
	for (int i = 0; i < threads.size(); i++)
	{
		threads[i] = thread(DecodeText, text, key, delay, i);
	}
	for (auto& thread : threads)
	{
		thread.join();
	}
	auto end = chrono::high_resolution_clock::now();
	auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	if (display)
	{
		cout << "\n Zdeaszyfrowany tekst: ";
		SetConsoleTextAttribute(hConsole, 14);
		cout << text;
		SetConsoleTextAttribute(hConsole, 15);
	}
	cout << "\n\n Zmierzony czas: " << elapsed << " ms\n";
}

void RunDecodeTextMethodParallelOpenMP(HANDLE hConsole, char text[MAX_TAB_SIZE], int key, int delay, int display)
{
	cout << "\n\n";
	SetConsoleTextAttribute(hConsole, 11);
	for (int i = 0; i < 70; i++) cout << '*';
	SetConsoleTextAttribute(hConsole, 3);
	cout << "\n ---> RÛwnoleg≥e deszyfrowanie tekstu za pomocπ OpenMP - Szyfr Cezara\n";
	SetConsoleTextAttribute(hConsole, 15);
	auto begin = chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(static, 1)
	for (int i = 0; i < strlen(text); i++)
	{
		DecodeText(text, key, delay, i);
	}
	auto end = chrono::high_resolution_clock::now();
	auto elapsed = chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	if (display)
	{
		cout << "\n Zdeszyfrowany tekst: ";
		SetConsoleTextAttribute(hConsole, 14);
		cout << text;
		SetConsoleTextAttribute(hConsole, 15);
	}
	cout << "\n\n Zmierzony czas: " << elapsed << " ms\n";
}