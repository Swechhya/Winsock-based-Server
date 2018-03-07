#pragma once
#include<iostream>
bool readHTMLFile(const char*fileName, char** buffer);
void parseRequest(char* buffer, char *fileName);
