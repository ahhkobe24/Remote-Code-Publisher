#pragma once
////////////////////////////////////////////////////////////////////
// Test.h - auto test and display requirement                   //
// Ver 1.0                                                       //
// Application: CSE687 Pr#4, Spring 2017                         //
// Platform:    Dell XPS 2720, Win 10 Pro, Visual Studio 2015   //
// Author:      Xia Hua                                         //
//              xhua02@syr.edu                                  //
/////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  Provides two classes :
*  Test:
*  - provides all the functionality to test server and client
*  
*
*  Required Files:
*  ---------------
MsgClient.h, MsgClient.cpp
*/
#include "../MsgClient/MsgClient.h"
#include <iostream>

class Test
{
public:
	Test(BlockingQueue <HttpMessage> &msgQ) :client(msgQ) {}
	void execute();
	void downLoadcssjs();
	void getFileList();
	void openDir();
	void downloadFile();
	void upload();
private:
	MsgClient client;
};