using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using MyInterop;

namespace TestThreading
{
   public class VM : INotifyPropertyChanged
   {
      private readonly MyInterop.Interop _interop = new MyInterop.Interop();
      public VM()
      {
         TestCommand = new RelayCommand(TestStuff );
         _interop.TranscriptionProgress += OnTranscriptionProgress;
      }

      private void OnTranscriptionProgress( object sender, TranscriptionProgressEventArgs e )
      {
         Percentage = e.Progress;
      }

      private void TestStuff()
      {
         _interop.StartTranscription();
      }

      public string PercentageString => Percentage.ToString();

      private int _percentage = 0;
      public int Percentage
      {
         get => _percentage;
         set
         {
            if( _percentage != value )
            {
               _percentage = value;
               PropertyChanged?.Invoke( this, new PropertyChangedEventArgs( nameof( Percentage ) ) );
               PropertyChanged?.Invoke( this, new PropertyChangedEventArgs( nameof( PercentageString ) ) );
            }
         }
      }

      public ICommand TestCommand { get; }

      public event PropertyChangedEventHandler PropertyChanged;
   }
}
