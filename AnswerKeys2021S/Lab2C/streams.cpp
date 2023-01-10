#include "streams.h"

#include <fstream>  /* needed for ofstream below */
std::ostream &trace  = (getenv("TRACE_OUTPUT") ? (*(new std::ofstream(getenv("TRACE_OUTPUT")))) : std::cerr);
std::ostream &debug  = (getenv("DEBUG_OUTPUT") ? (*(new std::ofstream(getenv("DEBUG_OUTPUT")))) : std::cerr);
