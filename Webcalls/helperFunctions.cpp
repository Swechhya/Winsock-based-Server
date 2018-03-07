
#include"helperFunctions.h"

//Function to read the specified HTMl file and create the required http response.
bool readHTMLFile(const char*fileName, char** buffer)
{
	FILE* toReadHTML = NULL;
	size_t size;
	char * fileContent;
	const char html[] = "HTTP/1.1 200 OK\r\n"
		"Connection: close\r\n"
		"Content-type: text/html\r\n"
		"\r\n";
	try {
		toReadHTML = fopen(fileName, "r");

		//if unable to read the file return false
		if (toReadHTML == NULL)
		{
			return false;
		}
		fseek(toReadHTML, 0, SEEK_END);
		size = ftell(toReadHTML);
		fileContent = new char[size];
		(*buffer) = new char[size + strlen(html) - 1];
		rewind(toReadHTML);
		fflush(toReadHTML);
		fread(fileContent, sizeof(char), size - (size / 10), toReadHTML);
		strcpy(*buffer, html);
		strcat(*buffer, fileContent);

		delete[] fileContent;
		return true;
	}
	catch (...)
	{
		//If any error occurs return false
		return false;
	}

}

//Function to parse the request and get the specified filename
void parseRequest(char* buffer, char *fileName)
{
	char *getReq;

	getReq = strtok(buffer, "\r\n");
	getReq[strlen(getReq) - 9] = '\0';

	if (strlen(getReq) > 6)
	{
		strcpy(getReq, &getReq[5]);
		strcpy(fileName, "..//pages//");
		strcat(fileName, getReq);
	}
	else
	{
		strcpy(fileName, "\0");
	}

}