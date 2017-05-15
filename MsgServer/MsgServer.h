#pragma once

////////////////////////////////////////////////////////////////////
// MsgServer.h - Define server class to send and recieve message  //
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
*  ClientHandler:
*  - provides all the functionality to handle clients
*  - created by SocketListener after accepting a request
*  - usually passed to a client handling thread
*  SendHandler:
*  - to response message recieved from client.
*  - hold by ClientHandler
*
*  Required Files:
*  ---------------
   Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
   ttpMessage.h.ttpMessage.cpp
*  Sockets.h, Sockets.cpp,
FileSystem.h, FileSystem.cpp
*  Logger.h, Logger.cpp,
*  Utilities.h, Utililties.cpp,
*  DirectoryProcess.h, DirectoryProcess.cpp
*  WindowsHelpers.h, WindowsHelpers.cpp
*/
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../CodePublisher/DirectoryProcess.h"
#include <string>
#include <iostream>
#include <thread>

using Show = StaticLogger<1>;


class SendHandler
{
public:
	using EndPoint = std::string;
	void execute(const size_t TimeBetweenMessages, Socket& si);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket);
	void setcommand(std::string temp, std::string parameter);
	std::string getFileListAndDirs(std::string dirstr);
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
private:

	std::string para = "11";
	std::string command = "1";


};

class ClientHandler
{
public:
	ClientHandler(BlockingQueue<HttpMessage>& msgQ, DirProcess& dirpro) : msgQ_(msgQ), dir(dirpro) {

	}
	void operator()(Socket socket);
private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
	SendHandler sh;
	DirProcess dir;
};