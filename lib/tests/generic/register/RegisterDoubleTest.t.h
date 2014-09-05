#include <cxxtest/TestSuite.h>
#include <indemic/generic/Register.h>

namespace
{
static uint8_t doublepseudoregister1;
static uint8_t doublepseudoregister2;
constexpr uint64_t daddress1 = 0x618ec9;
constexpr uint64_t daddress2 = daddress1 + 1;
}

class DoubleTestMicrocontroller
{
    public:
        typedef uint8_t register_t;
        typedef uint8_t register_value_t;
        typedef uint16_t register_double_t;
        typedef uint16_t register_double_value_t;
};

class TestRegister : public IndeMic::RegisterDoubleSettable<DoubleTestMicrocontroller, daddress1, TestRegister>
{
};

class RegisterDoubleTestSuite : public CxxTest::TestSuite
{
    public:
        /**
         * testing RegisterDoubleValue
         */
        void testDoubleRegister()
        {
            doublepseudoregister1 = 0;
            doublepseudoregister2 = 0;
            if (daddress1 != reinterpret_cast<uint64_t>(&doublepseudoregister1)) {
                printf("&doublepseudoregister1: %lx, skipping test\n", reinterpret_cast<uint64_t>(&doublepseudoregister1));
                TS_SKIP("Skipped because of hardcoded variable daddress");
                return;
            }
            if (daddress2 != reinterpret_cast<uint64_t>(&doublepseudoregister2)) {
                printf("&doublepseudoregister2: %lx, skipping test\n", reinterpret_cast<uint64_t>(&doublepseudoregister2));
                TS_SKIP("Skipped because of hardcoded variable daddress");
                return;
            }
            TestRegister::assign(0xf00f);
            TS_ASSERT_EQUALS(doublepseudoregister1, 0x0f);
            TS_ASSERT_EQUALS(doublepseudoregister2, 0xf0);
        }
};
