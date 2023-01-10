//matt rushton
#include <iostream.h>
#include <hc_string.h>
#include <logic.h>

inline String getline(istream &i) 
{
    char buf[10240], nl;
    i.get(buf,10240, '\n');
    if (i) i.get(nl);
    assert(!i || nl == '\n');
    return buf;
} 
