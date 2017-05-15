#pragma once
/////////////////////////////////////////////////////////////////////
//  perFile.h - process each source code                           //
//  ver 1.0                                                        //
//  Language:   Visual C++ 2015                                    //
//  Author:     Xia Hua                                      //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is designed to process each .cpp file or .h file.
* Add CSS part and JavaScript part.
*
* Public Interface:
* -----------------
* perFile(in, css, js, root):	process required file, add css, javascript, and root part;
* cssPart(cssPath):				add css part;
* dep_Root(depPaths, root):		add dependence part, and root part.
*
* Maintenance History:
* --------------------
* ver 1.0: Apr 2 2017
* - first released
*/
#ifndef PERFILE_H
#define PERFILE_H

#include<stdio.h>
#include<iostream>
#include<string>
#include<vector>
#include<fstream>

////////////////////////////////////////////////////////////////////////////
// perFile class process one .cpp or .h file, convert it to .html file
// add required css and javascript part.
//
class perFile
{
public:
	perFile(std::string in, std::string css, std::string js, std::string root);
	std::string CSSPart(std::string cssPath);
	std::string JSPart(std::string jsPath);
	std::string insertDep_Root(std::string dep_root);
	inline std::string getFileStr() { return fileStr; };
	inline std::string getFileHTMLStr() { return fileHTMLStr; };
	//std::string dep_Root(std::vector<std::string> depPaths, std::string root);
	int eachLine(std::string& line, int no, int flag);//process {}
private:
	std::string fileStr;
	std::string fileHTMLStr;
	std::string filePath;
	std::vector<std::string> headerPaths;
	std::vector<std::string> depFilePaths;
};

#endif // !PERFILE_H
