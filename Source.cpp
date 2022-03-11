#include <Windows.h>
#include <consoleapi.h>
#include <string>
#include <utility>
#include <math.h>
#include <iostream>
#include <sstream>
#include <ostream>
#include <istream>
#include <fstream>
#include "Lib/aes_transformations.h"
#include "Lib/aes128.h"

bool g_Shutdown = false;
HANDLE g_consoleHandle;
aes128 g_cryptEngine;

void printOptions() {
	std::cout << "F1 - Encrypt file" << std::endl << "F3 - Decrypt file" << std::endl;
}
int getOption() {
	if (GetAsyncKeyState(VK_F1)) {
		return 1;
  } else if(GetAsyncKeyState(VK_F3)) {
	  return 2;
  }
  return 0;
}
std::string getEncryptedContents(std::string key, std::string contents) {
	std::cout << "key: " << key << std::endl;
	return g_cryptEngine.encrypt(contents, key);
}
std::string getDecryptedContents(std::string key, std::string contents) {
	std::cout << "key: " << key << std::endl;
	return  g_cryptEngine.decrypt(contents, key);
}
std::string getPath() {
	std::cout << "File path:";
	std::string temp;
	std::getline(std::cin,temp);
	std::cout << std::endl;
	return temp;
}
std::string getKey() {
	std::cout << "Encryption key:";
	std::string temp;
	std::getline(std::cin,temp);
	std::cout << std::endl;
	return temp;
}
std::string getFileContentsFromPath(std::string path) {
	std::string contents = "", currentLine = "";
	std::ifstream file(path);
	while (std::getline(file, currentLine)) {
		contents.append(currentLine);
	}
	file.close();
	return contents;
}
void writeNewContents(std::string path, std::string newContents) {
	std::ofstream file(path, std::ios::trunc);
	file << newContents;
	file.close();
}
void clearConsole() {
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;
	GetConsoleScreenBufferInfo(g_consoleHandle, &screen);
	FillConsoleOutputCharacterA(g_consoleHandle, ' ', screen.dwSize.X * screen.dwSize.Y, { 0, 0 }, &written);
	FillConsoleOutputAttribute(g_consoleHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,screen.dwSize.X * screen.dwSize.Y, { 0, 0 }, &written);
	SetConsoleCursorPosition(g_consoleHandle, { 0, 0 });
}
int main() {
	g_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	printOptions();
	while (!g_Shutdown) {
		int option = getOption();
		if (option) {
			std::string path = getPath();
			std::string contents = getFileContentsFromPath(path);
			std::cout << contents << std::endl;
			std::string key = getKey();
			switch (option) {
				// Encrypt
			case 1: {
				std::string newContents = getEncryptedContents(key, contents);
				std::cout << "new: " << newContents << std::endl;
				writeNewContents(path, newContents);
				break;
			}
			   // Decrypt
			case 2: {
				std::string newContents = getDecryptedContents(key, contents);
				std::cout << "new: " << newContents << std::endl;
				writeNewContents(path, newContents);
				break;
			}
			}
			Sleep(2000);
			clearConsole();
			printOptions();
		}
	}
}