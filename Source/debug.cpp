#include "stdafx.h"
#include <assert.h>
#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif
void Exit()
{
	int i = _CrtDumpMemoryLeaks();
	assert(i == 0);
}
int _tmain(int argc, _TCHAR* argv[])
{
	atexit(Exit);
	int* p = new int();
	return 0;
}