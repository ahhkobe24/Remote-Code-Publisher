/////////////////////////////////////////////////////////////////////
//  DirectoryProcess.cpp - process directory files                 //
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
* None
*
* Maintenance History:
* --------------------
* ver 1.0: Apr 2 2017
* - first released
*/
#include  <direct.h>
#include "DirectoryProcess.h"
#include "../CodePublisher/perFile.h"

//----<analysis directory>----------
DirProcess::DirProcess(std::string path, std::vector<std::string> patterns) :dirPath(path), dirPatterns(patterns)
{
	for (std::string pattern : patterns)
		for (std::string file : analPath(path, pattern))
			filePaths.push_back(file);
}

//----<extract file path from directory>----------
std::vector<std::string> DirProcess::analPath(std::string path, std::string pattern)
{
	std::vector<std::string> files;
	_finddata_t file_info;
	path.append("/");
	std::string current_path = path + pattern;
	std::string all_path = path + "*.*";

	int handle = _findfirst(all_path.c_str(), &file_info);
	if (handle == -1)
		return files;
	do {
		std::string name = file_info.name;
		if (name == "." || name == "..") continue;
		if (file_info.attrib == _A_SUBDIR)
			for (std::string file : analPath(path + name, pattern))
				files.push_back(file);
	} while (!_findnext(handle, &file_info));
	_findclose(handle);

	handle = _findfirst(current_path.c_str(), &file_info);
	if (handle == -1)
		return files;
	do {
		std::string name = file_info.name;
		if (name == "." || name == "..") continue;
		if (file_info.attrib != _A_SUBDIR)
			files.push_back(path + name);
	} while (!_findnext(handle, &file_info));
	_findclose(handle);
	return files;
}

//----<add root page>----------
void DirProcess::processDir(std::string css, std::string js, std::string root_)
{
	std::string root("<html>\n");
	root.append("<head>\n");
	root.append("<style>\n");
	root.append("body {\n");
	root.append("margin:20px; color:black; background-color:#eee;\n");
	root.append("font-family:Consolas; font-weight:600; font-size:110%;\n");
	root.append("}\n");
	root.append(".indent { margin-left:20px; margin-right:20px; }\n");
	root.append("</style>\n");
	root.append("</head>\n");
	root.append("<body>\n");
	root.append("<h3>Source Files:</h3>\n");
	root.append("<hr />\n");
	root.append("<div class=\"indent\">\n");
	for (std::string file : filePaths)
		root.append("<a href=\"").append(file + ".html").append("\">").append(file).append("</a></br>\n");
	root.append("</div>\n");
	root.append("<hr />\n");
	root.append("</body>\n");
	root.append("</html>\n");
	for (std::string file : filePaths) {
		perFile a(file, css, js, root_);
		std::ofstream out(file + ".html");
		if (out.is_open()) {
			out << a.getFileHTMLStr();
			out.close();
		}
	}
	std::ofstream out(root_);
	if (out.is_open()) {
		out << root;
		out.close();
	}
}

//----<Test Stub>----------
/*
* Demonstrate how DirProcess works
*/
#ifdef TEST_detect
int main(int argc, char* argv[])
{
	char buf[_MAX_PATH];
	_getcwd(buf, _MAX_PATH);
	std::string curPath(buf);
	std::string css(curPath + "/myStyle.css");
	std::string js(curPath + "/myScript.js");
	std::vector<std::string> patterns;
	patterns.push_back("*.cpp");
	patterns.push_back("*.h");
	DirProcess dir(argv[1], patterns);
	dir.processDir(css, js, curPath + "/list.html");
	std::string path = "list.html";
	std::string command("start \"\" \"" + path + "\"");
	std::cout << "Requirement 2:" << std::endl;
	std::cout << "I use inlcude as dependence. So I don't use Fawcett's code. I don't use any C language.\n" << std::endl;
	std::cout << "Requirement 3:" << std::endl;
	std::cout << "I have my CodePublisher package, and my program would capture all *.h and *.cpp files.\n" << std::endl;
	std::cout << "Requirenment 4:" << std::endl;
	std::cout << "I wrote javascript file, whose name is myScript.js, to implement this expand and collapse function.\n" << std::endl;
	std::cout << "Requirement 5:" << std::endl;
	std::cout << "I wrote css file, whose name is myStyle.css, to style these pages.\n" << std::endl;
	std::cout << "Requirement 6:" << std::endl;
	std::cout << "I embedded my javascript and css link in <head> section.\n" << std::endl;
	std::cout << "Requirement 7:" << std::endl;
	std::cout << "I embedded dependent files link at the top of each source file page. I use #include to find dependence, so I didn't use project 2.\n" << std::endl;
	std::cout << "Requirement 8:" << std::endl;
	std::cout << "You could type in the directory path in command line. My program would process all *.h and *.cpp files under this path.\n" << std::endl;
	std::cout << "Requirement 9:" << std::endl;
	std::cout << "I already type in my project3's directory path in .bat file. The result is in the browser now.\n" << std::endl;
	std::system(command.c_str());
	system("pause");
}
#endif