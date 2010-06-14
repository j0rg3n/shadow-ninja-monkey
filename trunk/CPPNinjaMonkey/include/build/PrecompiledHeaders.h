#pragma once
#ifndef PRECOMPILED_H_INCLUDED
#define PRECOMPILED_H_INCLUDED

#ifdef USING_PRECOMPILED_HEADERS

#ifdef _WINDOWS
#include <windows.h>
#endif // _WINDOWS

#include <GL.h>
#include <GLU.h>

#include <vector>
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

#include "boost/signal.hpp"

#endif // USING_PRECOMPILED_HEADERS

#endif // PRECOMPILED_H_INCLUDED