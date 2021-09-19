#include <cstdio>

#define NANOSVG_IMPLEMENTATION
extern "C"
{
#include <nanosvg.h>
}
#undef NANOSVG_IMPLEMENTATION

#define NANOSVGRAST_IMPLEMENTATION
extern "C"
{
#include <nanosvgrast.h>
}
#undef NANOSVGRAST_IMPLEMENTATION
