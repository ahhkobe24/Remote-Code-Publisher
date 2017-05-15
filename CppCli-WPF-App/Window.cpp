///////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                //
//          - Demo for CSE 687 Project #4                                //
// ver 2.0                                                               //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015             //
///////////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "Window.h"
using namespace CppCliWindows;

WPFCppCliDemo::WPFCppCliDemo()
{
  // set up channel

  ObjectFactory* pObjFact = new ObjectFactory;
  pSendr_ = pObjFact->createSendr();
  pRecvr_ = pObjFact->createRecvr();
  pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
  pChann_->start();
  delete pObjFact;

  // client's receive thread

  recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
  recvThread->Start();

  // set event handlers

  this->Loaded += 
    gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
  this->Closing += 
    gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
  hSendButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendMessage);
  hClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
  hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
  hShowItemsButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::getItemsFromList);
  hOpenDir->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::openDirfromServer);

  // set Window properties

  this->Title = "WPF C++/CLI Demo";
  this->Width = 800;
  this->Height = 600;

  // attach dock panel to Window

  this->Content = hDockPanel;
  hDockPanel->Children->Add(hStatusBar);
  hDockPanel->SetDock(hStatusBar, Dock::Bottom);
  hDockPanel->Children->Add(hGrid);

  // setup Window controls and views

  setUpTabControl();
  setUpStatusBar();
  setUpSendMessageView();
  setUpFileListView();
  setUpConnectionView();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
  delete pChann_;
  delete pSendr_;
  delete pRecvr_;
}

void WPFCppCliDemo::setUpStatusBar()
{
  hStatusBar->Items->Add(hStatusBarItem);
  hStatus->Text = "Conneted to server and files have been downloaded";
  //hstatus->FontWeight = FontWeights::Bold;
 // hStatus->text
  hStatusBarItem->Content = hStatus;
  hStatusBar->Padding = Thickness(20, 5, 20, 5);
}

void WPFCppCliDemo::setUpTabControl()
{
  hGrid->Children->Add(hTabControl);
  hSendMessageTab->Header = "Get file list";
  hFileListTab->Header = "Upload File";
  hConnectTab->Header = "Connect";
  hTabControl->Items->Add(hSendMessageTab);
  hTabControl->Items->Add(hFileListTab);
  hTabControl->Items->Add(hConnectTab);
}

void WPFCppCliDemo::setTextBlockProperties()
{
 /* RowDefinition^ hRow1Def = gcnew RowDefinition();
  hSendMessageGrid->RowDefinitions->Add(hRow1Def);
  Border^ hBorder1 = gcnew Border();
  hBorder1->BorderThickness = Thickness(1);
  hBorder1->BorderBrush = Brushes::Black;
  hBorder1->Child = SListBox;
  hTextBlock1->Padding = Thickness(15);
  hTextBlock1->Text = "";
  hTextBlock1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
  hTextBlock1->FontWeight = FontWeights::Bold;
  hTextBlock1->FontSize = 16;
  hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
  hScrollViewer1->Content = hBorder1;
  hSendMessageGrid->SetRow(hScrollViewer1, 0);
  hSendMessageGrid->Children->Add(hScrollViewer1);*/
	Console::Write("\n  setting up FileList view");
	hSendMessageGrid->Margin = Thickness(20);
	hSendMessageTab->Content = hSendMessageGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	//hRow1Def->Height = GridLength(75);
	hSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	SListBox->SelectionMode = SelectionMode::Multiple;
	hBorder1->Child = SListBox;
	hSendMessageGrid->SetRow(hBorder1, 0);
	hSendMessageGrid->Children->Add(hBorder1);
}

void WPFCppCliDemo::setButtonsProperties()
{
  RowDefinition^ hRow2Def = gcnew RowDefinition();
  hRow2Def->Height = GridLength(75);
  hSendMessageGrid->RowDefinitions->Add(hRow2Def);
  Border^ hBorder2 = gcnew Border();
  hBorder2->Width = 120;
  hBorder2->Height = 30;
  hBorder2->BorderThickness = Thickness(1);
  hBorder2->BorderBrush = Brushes::Black;
  hSendButton->Content = "Get file list";
  Border^ hBorder4 = gcnew Border();
  hBorder4->Width = 120;
  hBorder4->Height = 30;
  hBorder4->BorderThickness = Thickness(1);
  hBorder4->BorderBrush = Brushes::Black;
  hOpenDir->Content = "open dir";
  hBorder4->Child = hOpenDir;
  hBorder2->Child = hSendButton;
  Border^ hBorder3 = gcnew Border();
  hBorder3->Width = 120;
  hBorder3->Height = 30;
  hBorder3->BorderThickness = Thickness(1);
  hBorder3->BorderBrush = Brushes::Black;
  hClearButton->Content = "Download";
  hBorder3->Child = hClearButton;
  hStackPanel1->Children->Add(hBorder2);
  TextBlock^ hSpacer = gcnew TextBlock();
  hSpacer->Width = 10;
  TextBlock^ hSpacer2 = gcnew TextBlock();
  hSpacer2->Width = 10;
  hStackPanel1->Children->Add(hSpacer);
  hStackPanel1->Children->Add(hBorder3);
  hStackPanel1->Children->Add(hSpacer2);
  hStackPanel1->Children->Add(hBorder4);
  hStackPanel1->Orientation = Orientation::Horizontal;
  hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
  hSendMessageGrid->SetRow(hStackPanel1, 1);
  hSendMessageGrid->Children->Add(hStackPanel1);
}

void WPFCppCliDemo::setUpSendMessageView()
{
  Console::Write("\n  setting up sendMessage view");
  hSendMessageGrid->Margin = Thickness(20);
  hSendMessageTab->Content = hSendMessageGrid;

  setTextBlockProperties();
  setButtonsProperties();
}

std::string WPFCppCliDemo::toStdString(String^ pStr)
{
  std::string dst;
  for (int i = 0; i < pStr->Length; ++i)
    dst += (char)pStr[i];
  return dst;
}

void WPFCppCliDemo::sendMessage(Object^ obj, RoutedEventArgs^ args)
{
	SListBox->Items->Clear();
  pSendr_->postMessage(toStdString("request list"));
  /*std::string message = "message from client";
  pChann_->sendMessage(message, "request");*/
  //String^ sMsg = toSystemString(message);
  //array<String^>^ args = gcnew array<String^>(1);
  //args[0] = sMsg;
  //Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
  //Dispatcher->Invoke(act, args);  // must call addText on main UI thread
  Console::Write("\n  sent message");
  hStatus->Text = "Sent message";
}

String^ WPFCppCliDemo::toSystemString(std::string& str)
{
  StringBuilder^ pStr = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    pStr->Append((Char)str[i]);
  return pStr->ToString();
}

void WPFCppCliDemo::addText(String^ msg)
{
	SListBox->Items->Add(msg);
}

void WPFCppCliDemo::getMessage()
{
  // recvThread runs this function

  while (true)
  {
    std::cout << "\n  receive thread calling getMessage()";
    std::string msg = pRecvr_->getMessage();
	std::string filename = "";
	while (msg.find('&') < 300)
	{
		filename = msg.substr(0, msg.find('&'));
		msg = msg.substr(msg.find('&') + 1);
		String^ sMsg = toSystemString(filename);
		array<String^>^ args = gcnew array<String^>(1);
		args[0] = sMsg;
		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
		Dispatcher->Invoke(act, args);  // must call addText on main UI thread
	}
  }
}

void WPFCppCliDemo::openDirfromServer(Object^ sender, RoutedEventArgs^ args)
{
	
	String^ dirname;
	int index = 0;
	int count = SListBox->SelectedItems->Count;
	hStatus->Text = "open dir";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count != 1)
	{
		hStatus->Text = "Please select one item";
		return;
	}
		for each (String^ item in SListBox->SelectedItems)
		{
			dirname = item+"<>";
		}
		if (toStdString(dirname).find_last_of('.') > 10)
		{
			hStatus->Text = "Please select a dir";
			return;
		}
		SListBox->Items->Clear();
		pSendr_->postMessage(toStdString(dirname));
	}



void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	String^ filesname;
	int index = 0;
	int count = SListBox->SelectedItems->Count;
	hStatus->Text = "Download";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in SListBox->SelectedItems)
		{
			items[index++] = item;
			filesname += item + "@";
		}
		pSendr_->postMessage(toStdString(filesname));
	}

	SListBox->Items->Clear();
	if (count > 0) {
		for each (String^ item in items)
		{
			SListBox->Items->Add(item);
		}
	}
}


void WPFCppCliDemo::getItemsFromList(Object^ sender, RoutedEventArgs^ args)
{
	String^ filesname;
	int index = 0;
	int count = hListBox->SelectedItems->Count;
	hStatus->Text = "Upload files";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox->SelectedItems)
		{
			/*if ((toStdString(filesname).length()-toStdString(filesname).find_last_of(".h"))>5 && (toStdString(filesname).length()-toStdString(filesname).find_last_of(".cpp")) > 100)
			{
				hStatus->Text = "please upload a .h file or .cpp file";
				return;
			}*/
			items[index++] = item;
			filesname += item + "&";
		}
		pSendr_->postMessage(toStdString(filesname));
		
	}
	
	hListBox->Items->Clear();
	if (count > 0) {
		for each (String^ item in items)
		{
			hListBox->Items->Add(item);
		}
	}
}



void WPFCppCliDemo::setUpFileListView()
{
  Console::Write("\n  setting up FileList view");
  hFileListGrid->Margin = Thickness(20);
  hFileListTab->Content = hFileListGrid;
  RowDefinition^ hRow1Def = gcnew RowDefinition();
  //hRow1Def->Height = GridLength(75);
  hFileListGrid->RowDefinitions->Add(hRow1Def);
  Border^ hBorder1 = gcnew Border();
  hBorder1->BorderThickness = Thickness(1);
  hBorder1->BorderBrush = Brushes::Black;
  hListBox->SelectionMode = SelectionMode::Multiple;
  hBorder1->Child = hListBox;
  hFileListGrid->SetRow(hBorder1, 0);
  hFileListGrid->Children->Add(hBorder1);

  RowDefinition^ hRow2Def = gcnew RowDefinition();
  hRow2Def->Height = GridLength(75);
  RowDefinition^ hRow2Def2 = gcnew RowDefinition();
  hRow2Def2->Height = GridLength(75);
  hFileListGrid->RowDefinitions->Add(hRow2Def);
  hFileListGrid->RowDefinitions->Add(hRow2Def2);
  hFolderBrowseButton->Content = "Select Directory";
  hFolderBrowseButton->Height = 30;
  hFolderBrowseButton->Width = 120;
  hFolderBrowseButton->BorderThickness = Thickness(2);
  hFolderBrowseButton->BorderBrush = Brushes::Black;
  hFileListGrid->SetRow(hFolderBrowseButton, 1);
  hFileListGrid->Children->Add(hFolderBrowseButton);

  // Show selected items button.
  hShowItemsButton->Content = "Upload file";
  hShowItemsButton->Height = 30;
  hShowItemsButton->Width = 120;
  hShowItemsButton->BorderThickness = Thickness(2);
  hShowItemsButton->BorderBrush = Brushes::Black;
  hFileListGrid->SetRow(hShowItemsButton, 2);
  hFileListGrid->Children->Add(hShowItemsButton);

  hFolderBrowserDialog->ShowNewFolderButton = false;
  hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
  std::cout << "\n  Browsing for folder";
  hListBox->Items->Clear();
  System::Windows::Forms::DialogResult result;
  result = hFolderBrowserDialog->ShowDialog();
  if (result == System::Windows::Forms::DialogResult::OK)
  {
    String^ path = hFolderBrowserDialog->SelectedPath;
    std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
    array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
    for (int i = 0; i < files->Length; ++i)
      hListBox->Items->Add(files[i]);
    array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
    for (int i = 0; i < dirs->Length; ++i)
      hListBox->Items->Add(L"<> " + dirs[i]);
  }
}
void WPFCppCliDemo::setUpConnectionView()
{
  Console::Write("\n  setting up Connection view");
}

void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
  Console::Write("\n  Window closing");
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
  Console::WriteLine(L"\n Starting WPFCppCliDemo");

  Application^ app = gcnew Application();
  app->Run(gcnew WPFCppCliDemo());
  Console::WriteLine(L"\n\n");
}