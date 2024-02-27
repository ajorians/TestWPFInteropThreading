#pragma once

#include <functional>

namespace InteropHelper
{
   namespace Details
   {
      template<typename StdFunction>
      struct StdFunctionSignature;

      template<typename ReturnType, typename... Arguments>
      struct StdFunctionSignature<std::function<ReturnType( Arguments... )>>
      {
         typedef ReturnType( *func )( Arguments... );
      };
   }

   template<typename TFunctionCallback>
   TFunctionCallback DelegateToStdFunction( ::System::Delegate^ delegate )
   {
      ::System::IntPtr ptr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate( delegate );

      auto functionPtr = static_cast<Details::StdFunctionSignature< TFunctionCallback >::func>( ptr.ToPointer() );

      return TFunctionCallback{ functionPtr };
   }

} // namespace InteropHelper
