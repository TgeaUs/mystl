#pragma once
#ifndef _MY_CONFIG_H_
#define _MY_CONFIG_H_
#include <iostream>
namespace MyConfig
{


# define ISDIALOGUE false

#	define EXCEPTIONS true
#   ifdef EXCEPTIONS
#     define STL_USE_EXCEPTIONS
#   endif

# ifdef STL_USE_EXCEPTIONS
#   define STL_TRY try
#   define STL_CATCH_ALL catch(...)
#   define STL_THROW(x) throw x
#   define STL_RETHROW throw
#   define STL_NOTHROW throw()
#   define STL_UNWIND(action) catch(...) { action; throw; }
# else
#   define __STL_TRY
#   define __STL_CATCH_ALL if (false)
#   define __STL_THROW(x)
#   define __STL_RETHROW
#   define __STL_NOTHROW
#   define __STL_UNWIND(action)# endif

#endif // !MY_CONFIG_H_
}
#endif
