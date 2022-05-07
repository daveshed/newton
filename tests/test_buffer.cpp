// stdlib
#include <cstdint>
// test dependencies
#include "CppUTest/TestHarness.h"
// code under test
#include "buffer.h"

TEST_GROUP(BufferTestGroup)
{
    Newton::FifoBuffer<uint8_t> buffer;

    void fill()
    {
        for (uint8_t i = 0; i < Newton::FifoBuffer<uint8_t>::max_size; ++i)
        {
            buffer.push(i);
        }
    }

    void check_contents()
    {
        for (uint8_t i = 0; i < Newton::FifoBuffer<uint8_t>::max_size; ++i)
        {
            uint8_t value;
            CHECK_EQUAL(0, buffer.pop(&value));
            CHECK_EQUAL(i, value);
        }
    }

    void setup()
    {
        buffer.clear();
    }
};

TEST(BufferTestGroup, TestClearedBufferReportsEmpty)
{
    CHECK(buffer.empty());
    CHECK(!buffer.full());
    CHECK_EQUAL(0U, buffer.size());
}

TEST(BufferTestGroup, TestFilledBufferReportsFull)
{
    fill();
    CHECK(buffer.full());
    CHECK_EQUAL(Newton::FifoBuffer<uint8_t>::max_size, buffer.size());
}

TEST(BufferTestGroup, TestFillingDrainingBufferDataConsistent)
{
    fill();
    check_contents();
    CHECK(buffer.empty());
}

TEST(BufferTestGroup, TestPoppingElementsMakesSpaceForMore)
{
    fill();
    buffer.pop(nullptr);
    buffer.pop(nullptr);
    buffer.pop(nullptr);
    CHECK(!buffer.full());
    // put the same amount of data back in
    CHECK_EQUAL(0, buffer.push(0x00));
    CHECK_EQUAL(0, buffer.push(0x01));
    CHECK_EQUAL(0, buffer.push(0x02));
    CHECK(buffer.full());
}

TEST(BufferTestGroup, TestCannotPopFromEmptyBuffer)
{
    CHECK(buffer.empty());
    CHECK_EQUAL(-1, buffer.pop(nullptr));
}

TEST(BufferTestGroup, TestCannotPushToFullBuffer)
{
    fill();
    CHECK(buffer.full());
    CHECK_EQUAL(-1, buffer.push(0x00));
}

TEST(BufferTestGroup, TestPoppingArrayFromFilledBufferReturnsExpected)
{
    fill();
    const size_t initial_size = buffer.size();
    const size_t n_elements = 10;
    uint8_t actual[n_elements] = {0U};
    buffer.pop(actual, n_elements);
    uint8_t expected[n_elements] = {0U};
    for (uint8_t i = 0; i < n_elements; i++)
    {
        expected[i] = i;
    }
    MEMCMP_EQUAL(expected, actual, n_elements);
    CHECK_EQUAL(initial_size - n_elements, buffer.size());
}

TEST(BufferTestGroup, TestPushingArrayIntoEmptyBufferContainsExpected)
{
    const size_t max_size = Newton::FifoBuffer<uint8_t>::max_size;
    uint8_t data[max_size];
    for (uint8_t i = 0; i < max_size; ++i)
    {
        data[i] = i;
    }
    buffer.push(data, max_size);
    check_contents();
}