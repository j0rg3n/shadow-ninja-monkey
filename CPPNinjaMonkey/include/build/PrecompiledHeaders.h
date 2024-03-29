#pragma once
#ifndef PRECOMPILED_H_INCLUDED
#define PRECOMPILED_H_INCLUDED

#ifdef USING_PRECOMPILED_HEADERS

#ifdef _WINDOWS
#include <Windows.h>
#include <WindowsX.h>
#endif // _WINDOWS

#include <gl/GL.h>
#include <gl/GLU.h>

#include <cassert>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

#include "boost/scoped_ptr.hpp"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/xml_parser.hpp"

#include "boost/bind.hpp"
#include "boost/function.hpp"

#include "boost/cstdint.hpp"

#include "boost/signals2.hpp"

#include "boost/thread.hpp"

#endif // USING_PRECOMPILED_HEADERS

#endif // PRECOMPILED_H_INCLUDED