/////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - WPF gui written in C#                      //
//   - makes calls into C++\CLI shim dll                           //
//   - Shim is bound to a MockChannel static library               //
// Note:                                                           //
//   - Application did not execute correctly when MockChannel was  //
//     was built as a dll.  I think the run time wasn't able to    //
//     find the dll to load.                                       //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;

namespace WPFgui
{
  public partial class MainWindow : Window
  {
    Shim shim = new Shim();
    Thread tRcv = null;

    public MainWindow()
    {
      InitializeComponent();
    }

    private int msgCount = 0;
    private void Button_Click(object sender, RoutedEventArgs e)
    {
      String msg = "request";
      SendList.Items.Insert(0, msg);
      shim.PostMessage(msg);
    }

    void Add(String msg)
    {
      RecvList.Items.Insert(0, msg);
    }

    private void Window_Loaded(object sender, RoutedEventArgs e)
    {
      tRcv = new Thread(
        () =>
        {
          while(true)
          {
            String msg = shim.GetMessage();
            Action<String> act = new Action<string>(Add);
            Object[] args = { msg };
            Dispatcher.Invoke(act, args);
          }
        }
      );
      tRcv.Start();
    }
  }
}
