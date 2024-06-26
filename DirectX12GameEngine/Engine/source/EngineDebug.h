#pragma once


#define PRINT_N(x) std::cout << x << std::endl;
#define PRINT_FL(file,line, msg) std::cout << "Error at file: " << file << std::endl << "Line: " << line << std::endl << "Message: " << msg << std::endl;
#define WTF(x) if(x != S_OK) { PRINT_FL(__FILE__,__LINE__, "H_RESULT is: "<< x); throw x; }
#define THROWUP_AN_ERROR(x) PRINT_FL(__FILE__,__LINE__,x); throw;

#ifdef NDEBUG

#endif

#ifdef _DEBUG
#define DEBUG_PRINT(x) PRINT_N(x);
#else
#define DEBUG_PRINT(x)
#endif