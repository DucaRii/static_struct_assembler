#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#endif

#include <cstdint>
#include <memoryapi.h>
#include <fstream>
#include <vector>
#include <algorithm>

#include "helper/address.hpp"