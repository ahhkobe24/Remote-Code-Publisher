#pragma once
/////////////////////////////////////////////////////////////////////
//  DirectoryProcess.h - process directory files                   //
//  ver 1.0                                                        //
//  Language:	Visual C++ 2015                                    //
//  Author:		Xia Hua                                      //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is designed to process directory files
* process each file by perFile class
*
* Public Interface:
* -----------------
* DirProcess(path, patterns):	enter directory path and patterns;
* analPath(path, patterns):		extract required files under the path;
* processDir(css, js, root):	process each file in the directory, add css and javascript part;
*
* Maintenance History:
* --------------------
* ver 1.0: Apr 2 2017
* - first released
*/
#ifndef DIRECTORYPROCESS_H
#define DIRECTORYPROCESS_H

#include "../CodePublisher/perFile.h"
#include <io.h>

////////////////////////////////////////////////////////////////////////////
// DirProcess class process directory files
// add a root html page, and each file html page
//
class DirProcess
{
public:
	DirProcess(std::string path, std::vector<std::string> patterns);
	std::vector<std::string> analPath(std::string path, std::string pattern);
	inline std::vector<std::string> getFilePaths() { return filePaths; };
	void processDir(std::string css, std::string js, std::string root);
private:
	std::string dirPath;
	std::vector<std::string> filePaths;
	std::vector<std::string> dirPatterns;
	std::vector<perFile> dirFile;
};
#endif // !DIRECTORYPROCESS_H
