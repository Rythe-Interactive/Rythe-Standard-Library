
#include "concepts"
#include "delegate"
#include "hash"
#include "primitives"
#include "type_util"
#include "utilities"

namespace rsl {
    void test() {
        delegate<void()> del;

        del = &test;

        del();
    }
}
