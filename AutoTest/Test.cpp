/////////////////////////////////////////////////////////////////////
//  Test.cpp - auto test and display requirement                   //
//  ver 1.0                                                        //
//  Language:	Visual C++ 2015                                    //
//  Author:		Xia Hua                                      //
/////////////////////////////////////////////////////////////////////
/*
*
*/
/*
* Required Files:
*  Test.h
*/


#include "Test.h"


//----------<start the client and connect to server>-----
void Test::execute()
{
	client.execute();
}

//----------<start the download of css and js files after connection>-----
void Test::downLoadcssjs()
{
	client.sendMessage("initial start download js and css", "download", "../Repository/myStyle.css");
	::Sleep(300);
	client.sendMessage("initial start download js and css", "download", "../Repository/myScript.js");
	std::cout << "\n---------- js and css files have been downloaded from server-----------" << "\n";
}

//----------<get the file list from server>-----
void Test::getFileList()
{
	
	std::cout << "\n\n----------now display file list from server----------" << "\n\n";
	client.sendMessage("request file list", "request", "11");
}
//----------<open directory on  server>-----
void Test::openDir()
{
	std::cout << "\n\n----------now display open the dir :../Repository/headfile ---------" << "\n\n";
	client.sendMessage("open dir : ../Repository/headfile", "open dir", "../Repository/headfile");
}
//----------<download files from server>-----
void Test::downloadFile()
{
	std::cout << "\n\n----------now display download file  :../Repository/headfile/Sockets.h.html, it will open automatically after download" << "\n\n";
	client.sendMessage("download:../Repository/headfile/Sockets.h.html", "download", "../Repository/headfile/Sockets.h.html");
}
//----------<upload files to server>-----
void Test::upload()
{
	std::cout << "\n\n----------now display upload file  :C:\\SU\\course\\CIS687\\proj4\\CommPrototype1\\Client\\Sockets.h" << "\n\n";
	::Sleep(200);
	//client.sendMessage("upload file  :C:\\SU\\course\\CIS687\\proj4\\CommPrototype1\\Client\\Sockets.h", "upload", "C:\\SU\\course\\CIS687\\proj4\\CommPrototype1\\Client\\Sockets.h");
	client.sendFile("..\\Client\\Sockets.h");
	::Sleep(200);
	client.sendFile("..\\Client\\Test1.h");
	::Sleep(200);
	client.sendFile("..\\Client\\Test1.cpp");
	::Sleep(200);
	client.sendFile("..\\Client\\Test2.cpp");
	::Sleep(200);
}


int main()
{
	BlockingQueue <HttpMessage> msgQ;
	Test test(msgQ);
	test.execute();
	test.downLoadcssjs();
	::Sleep(100);
	test.getFileList();
	::Sleep(100);
	test.openDir();
	::Sleep(100);
	test.upload();
	::Sleep(100);
	test.downloadFile();
	char a;
	std::cin >> a;
	::Sleep(100);
}
	


	

	

	
	

