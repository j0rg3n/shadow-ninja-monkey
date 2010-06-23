#pragma once
#ifndef TRACE_H_INCLUDED
#define TRACE_H_INCLUDED


// -----------------------------------------------------------------------------


#define TRACE(pszFormat, ...) TraceMessage(pszFormat, __FILE__, __LINE__, __VA_ARGS__)


// -----------------------------------------------------------------------------


extern void TraceMessage(const char* pszFormat, const char* pszFile, int nLine, ...);


#endif // TRACE_H_INCLUDED