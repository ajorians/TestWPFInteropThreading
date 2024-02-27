using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace TestThreading
{
   /// <summary>
   /// A command whose sole purpose is to 
   /// relay its functionality to other
   /// objects by invoking delegates. The
   /// default return value for the CanExecute
   /// method is 'true'.
   /// </summary>
   public class RelayCommand<T> : ICommand
   {
      #region Fields

      internal readonly Action<T> _execute = null;
      internal readonly Predicate<T> _canExecute = null;

      #endregion // Fields

      #region Constructors

      public RelayCommand( Action<T> execute )
            : this( execute, null )
      {
      }

      /// <summary>
      /// Creates a new command.
      /// </summary>
      /// <param name="execute">The execution logic.</param>
      /// <param name="canExecute">The execution status logic.</param>
      public RelayCommand( Action<T> execute, Predicate<T> canExecute )
      {
         _execute = execute ?? throw new ArgumentNullException( nameof( execute ) );
         _canExecute = canExecute;
      }

      #endregion // Constructors

      #region ICommand Members

      [DebuggerStepThrough]
      public bool CanExecute( object parameter )
      {
         if ( _canExecute is null )
         {
            return true;
         }
         if ( parameter is null && typeof( T ).IsValueType )
         {
            return _canExecute( default );
         }
         if ( parameter is string && typeof( T ) != typeof( string ) && !( parameter is T ) )
         {
            var typeConverter = TypeDescriptor.GetConverter( typeof( T ) );
            try
            {
               var convertedObject = (T) typeConverter.ConvertFromString( (string) parameter );
               return _canExecute( convertedObject );
            }
            catch ( Exception ex ) when ( ex is FormatException || ex is InvalidOperationException )
            {
               return false;
            }
         }

         return _canExecute( (T)parameter );
      }

      public event EventHandler CanExecuteChanged
      {
         add
         {
            if ( _canExecute != null )
               CommandManager.RequerySuggested += value;
         }
         remove
         {
            if ( _canExecute != null )
               CommandManager.RequerySuggested -= value;
         }
      }

      public void Execute( object parameter )
      {
         if ( parameter is string && typeof( T ) != typeof( string ) && !( parameter is T ) )
         {
            var typeConverter = TypeDescriptor.GetConverter( typeof( T ) );
            var convertedObject = (T) typeConverter.ConvertFromString( (string) parameter );
            _execute( convertedObject );
         }
         else
         {
            if ( parameter is null )
            {
               parameter = default( T );
            }

            _execute( (T)parameter );
         }
      }

      #endregion // ICommand Members
   }

   /// <summary>
   /// A command whose sole purpose is to 
   /// relay its functionality to other
   /// objects by invoking delegates. The
   /// default return value for the CanExecute
   /// method is 'true'.
   /// </summary>
   public class RelayCommand : ICommand
   {
      #region Fields

      internal readonly Action _execute;
      internal readonly Func<bool> _canExecute;

      #endregion // Fields

      #region Constructors

      /// <summary>
      /// Creates a new command that can always execute.
      /// </summary>
      /// <param name="execute">The execution logic.</param>
      public RelayCommand( Action execute )
         : this( execute, null )
      {
      }

      /// <summary>
      /// Creates a new command.
      /// </summary>
      /// <param name="execute">The execution logic.</param>
      /// <param name="canExecute">The execution status logic.</param>
      /// <exception cref="ArgumentNullException"><c>execute</c> is null.</exception>
      public RelayCommand( Action execute, Func<bool> canExecute )
      {
         _execute = execute ?? throw new ArgumentNullException( nameof( execute ) );
         _canExecute = canExecute;
      }

      #endregion // Constructors

      #region ICommand Members

      [DebuggerStepThrough]
      public bool CanExecute( object parameter )
      {
         return _canExecute?.Invoke() ?? true;
      }

      public event EventHandler CanExecuteChanged
      {
         add
         {
            if ( _canExecute != null )
               CommandManager.RequerySuggested += value;
         }
         remove
         {
            if ( _canExecute != null )
               CommandManager.RequerySuggested -= value;
         }
      }

      public void Execute( object parameter )
      {
         _execute();
      }

      #endregion // ICommand Members
   }
}
