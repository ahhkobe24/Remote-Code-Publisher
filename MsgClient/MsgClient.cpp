/////////////////////////////////////////////////////////////////////
//  MsgClient.cpp - inplement functions and start client to connect //
//                 server and provide services                      //
//  ver 1.0                                                        //
//  Language:	Visual C++ 2015                                    //
//  Author:		Xia Hua                                            //
/////////////////////////////////////////////////////////////////////
/*
* This package implements a client that sends HTTP style messages and
* files to a server that simply displays messages and stores files.
*

/*
* Required Files:
*  MsgServer.h
*/

#include "MsgClient.h"
#include <string>
#include <iostream>
#include <thread>


using namespace Utilities;
using Utils = StringHelper;

/////////////////////////////////////////////////////////////////////
// ClientCounter creates a sequential number for each client
//

//----< read a binary file from socket and save >--------------------
/*
* This function expects the sender to have already send a file message,
* and when this function is running, continuosly send bytes until
* fileSize bytes have been sent.
*/

bool ServerHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{
	std::string fqname = "../Client/html/" + filename;
	if(filename.find(".js")<50 || filename.find(".css")<50)fqname = "../Client/" + filename;
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{

		std::cout<<"\n\n  can't open file " + fqname;
		return false;
	}

	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];

	size_t bytesToRead;
	while (true)
	{
		if (fileSize > BlockSize)
			bytesToRead = BlockSize;
		else
			bytesToRead = fileSize;

		socket.recv(bytesToRead, buffer);

		FileSystem::Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);

		file.putBlock(blk);
		if (fileSize < BlockSize)
			break;
		fileSize -= BlockSize;
	}
	file.close();
	if (filename.find(".js") > 100 && filename.find(".css") > 100)
	{
		std::string path = "..\\Client\\html\\" + filename;
		std::string command("start \"\" \"" + path + "\""); // the command is: [ start "" "..\test files\page.html" ] // using [ start ..\text files\page.html ] will fail to start // using [ start "..\test files\page.html" ] will only start new command window with the current directory 
		std::system(command.c_str());
	}
	return true;
}

//----< this defines processing to frame messages >------------------
HttpMessage ServerHandler::readMessage(Socket& socket)
{
	connectionClosed_ = false;HttpMessage msg;
	while (true)
	{
		std::string attribString = socket.recvString('\n');
		if (attribString.size() > 1)
		{
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);msg.addAttribute(attrib);
		}
		else
		{break;}
	}
	if (msg.attributes().size() == 0)
	{
		connectionClosed_ = true;return msg;
	}
	// read body if POST - all messages in this demo are POSTs
	if (msg.attributes()[0].first == "POST")
	{
		std::string filename = msg.findValue("file");
		if (filename != "")
		{
			size_t contentSize;std::string sizeString = msg.findValue("content-length");
			if (sizeString != "")
				contentSize = Converter<size_t>::toValue(sizeString);
			else
				return msg;
			readFile(filename, contentSize, socket);
		}
		if (filename != "")
		{
			msg.removeAttribute("content-length");std::string bodyString = "<file>" + filename + "</file>";
			std::string sizeString = Converter<size_t>::toString(bodyString.size());
			msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));msg.addBody(bodyString);
		}
		else
		{
			// read message body
			size_t numBytes = 0;size_t pos = msg.findAttribute("content-length");
			if (pos < msg.attributes().size())
			{numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
				Socket::byte* buffer = new Socket::byte[numBytes + 1];socket.recv(numBytes, buffer);
				buffer[numBytes] = '\0';std::string msgBody(buffer);msg.addBody(msgBody);delete[] buffer;
			}
		}
	}
	return msg;
}
//----< receiver functionality is defined by this function >---------
void ServerHandler::readTH(Socket& socket)
{
	while (true)
	{
		HttpMessage msg = readMessage(socket);
		if (connectionClosed_ || msg.bodyString() == "quit")
		{
			std::cout << "\n\n **************Server is terminating***********\n";
			break;
		}
		std::cout << "\n\n **************client recieve message Block***********\n" + msg.toIndentedString() + "\n" + "**********************************************\n";
		msgQ_.enQ(msg);
	}
}

class ClientCounter
{
public:
  ClientCounter() { ++clientCount; }
  size_t count() { return clientCount; }
private:
  static size_t clientCount;
};

size_t ClientCounter::clientCount = 0;

/////////////////////////////////////////////////////////////////////
// MsgClient class
// - was created as a class so more than one instance could be 
//   run on child thread
//

//----< factory for creating messages >------------------------------
/*
 * This function only creates one type of message for this demo.
 * - To do that the first argument is 1, e.g., index for the type of message to create.
 * - The body may be an empty string.
 * - EndPoints are strings of the form ip:port, e.g., localhost:8081. This argument
 *   expects the receiver EndPoint for the toAddr attribute.
 */
 //----< get the instance of ServerHandler >---------
ServerHandler& MsgClient::getSh()
{
	return sh;
}
//----< construct http message >---------
HttpMessage MsgClient::makeMessage(size_t n, const std::string& body, const EndPoint& ep,std::string command,std::string para)
{
  HttpMessage msg;
  HttpMessage::Attribute attrib;
  EndPoint myEndPoint = "localhost:client";  // ToDo: make this a member of the sender
                                           // given to its constructor.
  switch (n)
  {
  case 1:
    msg.clear();
    msg.addAttribute(HttpMessage::attribute("POST", "Message"));
    msg.addAttribute(HttpMessage::Attribute("command", command));
	msg.addAttribute(HttpMessage::Attribute("parameter", para));
    msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
    msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

    msg.addBody(body);
    if (body.size() > 0)
    {
      attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
      msg.addAttribute(attrib);
    }
    break;
  default:
    msg.clear();
    msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
  }
  return msg;
}
//----< send message using socket >----------------------------------

void MsgClient::sendStream(HttpMessage& msg)
{
  std::string msgString = msg.toString();
  si.send(msgString.size(), (Socket::byte*)msgString.c_str());
}
//----< send file using socket >-------------------------------------
/*
 * - Sends a message to tell receiver a file is coming.
 * - Then sends a stream of bytes until the entire file
 *   has been sent.
 * - Sends in binary mode which works for either text or binary.
 */

 //----< upload files to Server >---------
bool MsgClient::sendFile(const std::string& filepath)
{
  // assumes that socket is connected

  std::string fqname = filepath;
  std::string filename = filepath.substr(filepath.find_last_of('\\') + 1);
  FileSystem::FileInfo fi(fqname);
  size_t fileSize = fi.size();
  std::string sizeString = Converter<size_t>::toString(fileSize);
  FileSystem::File file(fqname);
  file.open(FileSystem::File::in, FileSystem::File::binary);
  if (!file.isGood())
    return false;
  
  HttpMessage msg = makeMessage(1, "", "localhost::8080","uplaod", filename);
  msg.addAttribute(HttpMessage::Attribute("file", filename));
  msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
  sendStream(msg);
  const size_t BlockSize = 2048;
  Socket::byte buffer[BlockSize];
  while (true)
  {
    FileSystem::Block blk = file.getBlock(BlockSize);
    if (blk.size() == 0)
      break;
    for (size_t i = 0; i < blk.size(); ++i)
      buffer[i] = blk[i];
    si.send(blk.size(), buffer);
    if (!file.isGood())
      break;
  }
  file.close();
  std::cout
	  << "\n\n **************client sent File Block***********\n" + msg.toIndentedString() + "\n" + "**********************************************\n";
  return true;
}
//----< send message to server >--------------------

void MsgClient::sendMessage(std::string str,std::string command,std::string para)
{
	ClientCounter counter;
	size_t myCount = counter.count();
	std::string myCountString = Utilities::Converter<size_t>::toString(myCount);
	std::string temp = "";
	std::string msgBody =
		"<msg>Message from client to "+ temp+"</msg> :"+ str;
	HttpMessage msg = makeMessage(1, msgBody, "localhost:8080", command, para);
	sendStream(msg);
	std::cout
		<<"\n\n **************client sent message Block***********\n"+ msg.toIndentedString()+"\n"+ "**********************************************\n";
	::Sleep(100);
}

//----< start connection and start the read message thread >---------
void MsgClient::execute()
{
	try
	{

		while (!si.connect("localhost", 8080))
		{
			//Show::write("\n client waiting to connect");
			::Sleep(100);
		}
	}
	catch (std::exception& exc)
	{
		//Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		//Show::write(exMsg);
	}
	std::thread t1([&]() {sh.readTH(si); });
	t1.detach();

}
  
