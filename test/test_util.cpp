//
// Created by bokket on 2021/5/28.
//

#include "../bokket/thread/util.h"
#include "../bokket/Log/Log.h"
#include "../bokket/thread/Assert.h"


bokket::Logger::ptr g_logger=BOKKET_LOG_ROOT();

void test_assert() {
    BOKKET_LOG_INFO(g_logger)<<bokket::backTraceToString(10);
    ASSERT_MSG(1==1,"abcdef xx");
}

int main() {
    test_assert();
}