#include "diag/Trace.h"

#include <stdarg.h>

#ifdef _WINDOWS
#include "boost/thread/mutex.hpp"
#include <windows.h> //< OutputDebugString
#endif

// -----------------------------------------------------------------------------


using namespace boost;


// -----------------------------------------------------------------------------


static const size_t TRACE_BUFFER_SIZE = 32768;


// -----------------------------------------------------------------------------


static char g_szTraceBuffer[TRACE_BUFFER_SIZE];

#ifdef _WINDOWS
static mutex g_traceBufferMutex;
#endif


// -----------------------------------------------------------------------------


void TraceMessage(const char* pszFormat, const char* pszFile, int nLine, ...)
{
	va_list pArgs;
	va_start(pArgs, nLine);

	char* pszWritePos = g_szTraceBuffer;
	char* pszWriteEnd = g_szTraceBuffer + TRACE_BUFFER_SIZE - 1;

	{
		lock_guard<mutex> lock(g_traceBufferMutex);

		// TODO: Add time with milliseconds or microseconds.
		int nCharCount = sprintf(pszWritePos, "%s(%d):\n", pszFile, nLine);
		assert(nCharCount != -1);
		pszWritePos += nCharCount;

		nCharCount = vsnprintf(pszWritePos, pszWriteEnd - pszWritePos, pszFormat, pArgs);
		if (nCharCount == -1)
		{
			pszWritePos = pszWriteEnd;
		}
		else
		{
			pszWritePos += nCharCount;
		}

		*pszWritePos = 0;

		puts(g_szTraceBuffer);
#ifdef _WINDOWS
		OutputDebugString(g_szTraceBuffer);
#endif
	}
}
