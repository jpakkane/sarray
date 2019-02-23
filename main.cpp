/**
Copyright 2019 Jussi Pakkanen

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifdef NDEBUG
#undef NDEBUG
#endif

#include "sarray.hpp"
#include "cassert"
#include<algorithm>

int main() {
    s::array<int, 20> x;
    assert(x[0]);
    assert(!x[-1]);
    assert(!x[20]);
    assert(!x[100000000]);
    assert(*x[0] == 0);
    *x[1] = 5;
    assert(*x[1] == 5);
    assert(*x[19] == 0);


    for(int i=0; i<20; ++i) {
        *x[i] = 20-i;
    }

    auto m1 = x[1];
    auto m2 = x[2];
    std::swap(m1, m2);
    assert(*x[1] == 18);
    assert(*x[2] == 19);

    std::sort(x.begin(), x.end());
    assert(*x[0] == 1);
    assert(*x[1] == 2);

    std::sort(x.unsafe().begin(), x.unsafe().end(), std::greater<int>());
    assert(*x[19] == 1);
    assert(*x[18] == 2);
    int ten = 10;
    int sixsix = 66;
    assert(x[0].value_or(ten) == 20);
    assert(x[100].value_or(sixsix) == 66);

    return 0;
}
