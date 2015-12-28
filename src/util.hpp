#include <boost/format.hpp>
#include <ctype.h>
#include <stdio.h>

template <typename IterType>
void hexdump(std::ostream& os, IterType begin, IterType end) {
    const size_t width = 16;
    for (auto it = begin;
            it < end;
            it += width)
    {
        std::cout << it - begin;
        for (auto j = it; j-it < width; j++) {
            if (j < end)
                os << boost::format("%1$#02x ") % (unsigned)(*j);
            else
                os << "     ";
        }
        os << " ";
        for (auto j = it; j-it < width; j++) {
            if (j < end)
                os << (isprint(*j) ? *j : '.');
        }
        os << std::endl;
    }
}

