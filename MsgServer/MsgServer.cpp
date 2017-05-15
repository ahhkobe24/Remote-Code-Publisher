/////////////////////////////////////////////////////////////////////
//  MsgServer.cpp - inplement functions and start server to listen //
 //                 and provide services                           //
//  ver 1.0                                                        //
//  Language:	Visual C++ 2015                                    //
//  Author:		Xia Hua                                      //
/////////////////////////////////////////////////////////////////////
/*
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
*
*/
/*
* Required Files:
*  MsgServer.h
*/


#include "MsgServer.h"
using namespace Utilities;
//----< get files and directories in a specified directory >---------
std::string SendHandler::getFileListAndDirs(std::string dirstr)
{
	static FileSystem::Directory dire;
	std::string old = dire.getCurrentDirectory();
	dire.setCurrentDirectory(dirstr);
	std::vector<std::string> filevec = dire.getFiles();
	std::vector<std::string> dirvec = dire.getDirectories();

	std::string filelist = "";
	for (size_t i = 0; i < dirvec.size(); i++)
	{
		if(dirvec[i]!="."&&dirvec[i]!="..")
		filelist += dirstr +dirvec[i] + "&";
	}
	for (size_t i = 0; i < filevec.size(); i++)
	{
		filelist += dirstr +filevec[i] + "&";
	}

	dire.setCurrentDirectory(old);
	return filelist;

}

//----< set command >---------
void SendHandler::setcommand(std::string temp,std::string parameter)
{
	command = temp;
	para = parameter;
}

//----< send requested files to client >---------
bool SendHandler::sendFile(const std::string& filename, Socket& socket)
{
	std::string fqname = filename;
	if (fqname.find(".html") > 300 && fqname.find(".js")>300 && fqname.find(".css")>300)fqname += ".html";
    	
	std::string filechar = fqname.substr(fqname.find_last_of("/") + 1);
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
	{
		std::cout<<"\n\n  can't open file " +  fqname;
		return false;
	}
	HttpMessage msg = makeMessage(1, "", "localhost::8080");
	msg.addAttribute(HttpMessage::Attribute("file", filechar));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	sendMessage(msg, socket);

	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		socket.send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();
	std::cout
		<< "\n\n **************Sned sent File Block***********\n" + msg.toIndentedString() + "\n" + "**********************************************\n";
	return true;
}

//----< send message to client >---------
void SendHandler::sendMessage(HttpMessage& msg, Socket& socket)
{
	std::string msgString = msg.toString();
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}

//----< start a thread to reponse to messages from client based on command parameter >---------
void SendHandler::execute(const size_t TimeBetweenMessages, Socket& si)
{
		try
		{	
			while (command != "quit")
			{
				if (command == "request") {
					HttpMessage msg;std::string msgBody = getFileListAndDirs("../Repository/");msg = makeMessage(1, msgBody, "localhost");
					sendMessage(msg, si);
					std::cout<< "\n\n **************Server send message Block***********\n" + msg.toIndentedString() + "\n" + "**********************************************\n";
					::Sleep(TimeBetweenMessages);
					command = ""; para = "";
					continue;
				}
				if (command=="open dir") {
					HttpMessage msg;
					std::string msgBody = getFileListAndDirs(para+"/");
					msg = makeMessage(1, msgBody, "localhost");
					sendMessage(msg, si);
					std::cout<< "\n\n **************Server send message Block***********\n" + msg.toIndentedString() + "\n" + "**********************************************\n";
					::Sleep(TimeBetweenMessages);
					command = ""; para = "";
					continue;
				}
				if (command=="download") {
					HttpMessage msg;
					std::vector<std::string> patterns;
					patterns.push_back("*.cpp");
					patterns.push_back("*.h");
					DirProcess dir("../Repository", patterns);
					std::string css("../myStyle.css");
					std::string js("../myScript.js");
					dir.processDir(css, js, "../list.html");
					::Sleep(200);
					std::string msgBody = para + " download failed";
					if (!sendFile(para, si))
					{
						HttpMessage msg;
						std::string msgBody = para + " download failed";
						sendMessage(msg, si);
						::Sleep(TimeBetweenMessages);
						command = ""; para = "";
						continue;
					}
					::Sleep(TimeBetweenMessages);
					command = ""; para = "";
				}
			}
			}
		catch (std::exception& exc)
		{
			Show::write("\n  Exeception caught: ");
			std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
			Show::write(exMsg);
		}
}

//----< construct http message >---------
HttpMessage SendHandler::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:Server";  // ToDo: make this a member of the sender
											 // given to its constructor.
	switch (n)
	{
	case 1:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("POST", "Message"));
		msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
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


//----< this defines processing to frame messages >------------------

HttpMessage ClientHandler::readMessage(Socket& socket)
{
  connectionClosed_ = false; HttpMessage msg;
  while (true)
  {
    std::string attribString = socket.recvString('\n');
    if (attribString.size() > 1)
    {
      HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);msg.addAttribute(attrib);
    }
    else{ break;}
  }
  // If client is done, connection breaks and recvString returns empty string
  if (msg.attributes().size() == 0)
  {
   /*connectionClosed_ = true;*/return msg;
  }
  // read body if POST - all messages in this demo are POSTs
  if (msg.attributes()[0].first == "POST")
  {
    std::string filename = msg.findValue("file");std::string command = msg.findValue("command");std::string para = msg.findValue("parameter");sh.setcommand(command,para);
    if (filename != "")
    {
      size_t contentSize;std::string sizeString = msg.findValue("content-length");
      if (sizeString != "")contentSize = Converter<size_t>::toValue(sizeString);
      else
        return msg;
      readFile(filename, contentSize, socket);HttpMessage msg; std::string msgBody = filename + "from client has been recieved";
	  msg = sh.makeMessage(1, msgBody, "localhost");sh.sendMessage(msg, socket);
    }
    if (filename != "")
    {
      msg.removeAttribute("content-length");std::string bodyString = "<file>" + filename + "</file>";
      std::string sizeString = Converter<size_t>::toString(bodyString.size());
      msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));msg.addBody(bodyString);
    }
    else
    {
      size_t numBytes = 0;size_t pos = msg.findAttribute("content-length");
      if (pos < msg.attributes().size())
      {
        numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
        Socket::byte* buffer = new Socket::byte[numBytes + 1];socket.recv(numBytes, buffer);buffer[numBytes] = '\0';
        std::string msgBody(buffer);msg.addBody(msgBody);delete[] buffer;
      }
    }
  }
  return msg;
}
//----< read a binary file from socket and save >--------------------
/*
 * This function expects the sender to have already send a file message, 
 * and when this function is running, continuosly send bytes until
 * fileSize bytes have been sent.
* generate file recieved to html file;
 */
bool ClientHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{
	std::string fqname ;
	
		if (filename.find(".h")<60)fqname = "../Repository/headfile/" + filename;
		if (filename.find(".cpp")<60)fqname = "../Repository/cppfile/" + filename;
		
  FileSystem::File file(fqname);
  file.open(FileSystem::File::out, FileSystem::File::binary);
  if (!file.isGood())
  {
		  Show::write("\n\n  can't open file " + fqname);
		  return false;
	 /* }*/
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
 

  return true;
}
//----< receiver functionality is defined by this function,start the response thread >---------

void ClientHandler::operator()(Socket socket)
{
	std::thread t1([&]() {sh.execute(100, socket); });
	t1.detach();
	while (true)
  {
    HttpMessage msg = readMessage(socket);
    if (connectionClosed_ || msg.bodyString() == "quit")
    {
      Show::write("\n\n  clienthandler thread is terminating");
      break;
    }
	if(msg.bodyLength()!=0)
	std::cout
		<< "\n\n **************Server recieve message Block***********\n" + msg.toIndentedString() + "\n" + "**********************************************\n";
    msgQ_.enQ(msg);
  }
}

//----< test stub >--------------------------------------------------

int main()
{
  ::SetConsoleTitle(L"HttpMessage Server - Runs Forever");

  Show::attach(&std::cout);
  Show::start();
  Show::title("\n  HttpMessage Server started");

  BlockingQueue<HttpMessage> msgQ;

  try
  {
    SocketSystem ss;
    SocketListener sl(8080, Socket::IP6);
 
	std::vector<std::string> patterns;
	patterns.push_back("*.cpp");
	patterns.push_back("*.h");
	DirProcess dir("../Repository", patterns);
	ClientHandler cp(msgQ, dir);
    sl.start(cp);
    while (true)
    {
      HttpMessage msg = msgQ.deQ();
	  
    }
  }
  catch (std::exception& exc)
  {
    Show::write("\n  Exeception caught: ");
    std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
    Show::write(exMsg);
  }
}