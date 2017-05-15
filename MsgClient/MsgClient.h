#pragma once
////////////////////////////////////////////////////////////////////
// MsgClient.h - Define client class to connect to server and   //
//               send and recieve message                        //
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
*  MsgClient:
*  - provides all the functionality to connect server
*  - created by Socket
*  - send message to server
*  ServerHandler:
*  - to response message recieved from server.
*  - hold by MsgClient
*
*  Required Files:
*  ---------------
Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
httpMessage.h httpMessage.cpp
*  Sockets.h, Sockets.cpp,
FileSystem.h, FileSystem.cpp
*  Logger.h, Logger.cpp,
*  Utilities.h, Utililties.cpp,
*/
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../Utilities/Utilities.h"
#include "../FileSystem/FileSystem.h"
#include <string>
#include <iostream>



class ServerHandler
{
public:
	ServerHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void readTH(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
private:

	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	//bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
};

class MsgClient
{
public:
	MsgClient(BlockingQueue<HttpMessage>& msgQ) :sh(msgQ) {}
	using EndPoint = std::string;
	void execute();
	void sendMessage(std::string str, std::string command, std::string para);
	ServerHandler& getSh();
	bool sendFile(const std::string& filepath);
private:
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep, std::string command, std::string para);
	void sendStream(HttpMessage& msg);
	
	ServerHandler sh;
	SocketSystem ss;
	SocketConnecter si;
};
