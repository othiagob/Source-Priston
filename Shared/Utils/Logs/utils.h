#pragma once

#include "utils_filesystem.h"
#include "utils_logging.h"
#include "utils_exception.h"
#include "utils_testing.h"

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete [] (x); (x) = NULL; }

