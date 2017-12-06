// copyv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>
#include <Shlwapi.h>

#include <stdlib.h>
#include <string>
#include <iostream>

#pragma comment(lib,"Shlwapi.lib")


#include "../../lsMisc/stdwin32/stdwin32.h"
#include "../../lsMisc/GetLastErrorString.h"
#include "../../lsMisc/CompareFileTime.h"

using namespace stdwin32;
using namespace Ambiesoft;

using namespace std;



int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 3)
	{
		perror("arguments too few");
		exit(1);
	}
	if (argc != 3)
	{
		perror("argument too many");
		exit(2);
	}

	// "C:\Cygwin\home\fjUnc\gitdev\Windows10ClearIconCache\History.txt" "C:\Cygwin\home\fjUnc\gitdev\Windows10ClearIconCache\Windows10ClearIconCache\bin\Release\"
	wstring src = argv[1];
	wstring dst = argv[2];


	src = stdwin32::trim(src);
	src = stdwin32::trim(src, L"\"");
	dst = stdwin32::trim(dst);
	dst = stdwin32::trim(dst, L"\"");

	if (!PathFileExists(src.c_str()))
	{
		wcerr << L'"' << src << L'"' << " does not exist." << endl;
		return 4;
	}
	wstring srcfilename = stdGetFileName(src);

	if (PathIsDirectory(dst.c_str()))
	{
		dst = stdCombinePath(dst, srcfilename);
	}
	else
	{
		if (hasEnding(dst, L"/") || hasEnding(dst, L"\\"))
		{
			if (!CreateDirectory(dst.c_str(), NULL))
			{
				wcerr << L"CreateDirectory failed: " << dst << endl;
				return 5;
			}
		}
		else
		{
			// treat as file
		}
	}


	if (compareFileTime(src.c_str(), dst.c_str()) < 0)
	{
		wcerr << L"An old file can not overwrite the new file." << " " << src << " <<<<< " << dst << endl;
		return 1;
	}
	if (!CopyFile(src.c_str(), dst.c_str(), FALSE))
	{
		DWORD dwLE = GetLastError();
		wstring error = GetLastErrorString(dwLE);
		_wperror(error.c_str());
		return dwLE;
	}
	wcout << L"copied: ";
	wcout << L'"' << src << L'"';
	wcout << L" -> ";
	wcout << L'"' << dst << L'"';
	wcout << endl;
	return 0;
}

