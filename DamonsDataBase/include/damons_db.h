
#ifdef DAMONS_DB_USE_AS_DLL

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DAMONS_DB_LIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// QCC_IO_LIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DAMONS_DB_LIB_EXPORTS
#define DAMONS_DB_LIB_API __declspec(dllexport)
#else
#define DAMONS_DB_LIB_API __declspec(dllimport)
#endif

#else 

#define DAMONS_DB_LIB_API

#endif 
