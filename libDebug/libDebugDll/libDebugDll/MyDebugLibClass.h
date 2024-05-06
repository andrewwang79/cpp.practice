#pragma once
#ifdef LIBDEBUGDLL_EXPORTS
#define LIBDEBUGDLL_API __declspec(dllexport)
#else
#define LIBDEBUGDLL_API __declspec(dllimport)
#endif

class LIBDEBUGDLL_API MyDebugLibClass {
 public:
  MyDebugLibClass();
  int add(int a, int b);
};

extern "C" LIBDEBUGDLL_API int Add(int a, int b);
