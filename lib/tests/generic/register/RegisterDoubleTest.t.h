#include <cxxtest/TestSuite.h>
#include <indemic/generic/Register.h>

class DoubleTestMicrocontroller
{
    public:
        typedef uint8_t register_t;
        typedef uint8_t register_value_t;
        typedef uint16_t register_double_t;
        typedef uint16_t register_double_value_t;
};

class TestRegister : public IndeMic::RegisterDoubleSettable<DoubleTestMicrocontroller, 0, TestRegister>
{
    public:
        static inline DoubleTestMicrocontroller::register_double_t& reg()
        {
            return value;
        }
        static uint16_t value;
};
uint16_t TestRegister::value;

class RegisterDoubleTestSuite : public CxxTest::TestSuite
{
    public:
        /**
         * testing RegisterDoubleValue
         */
        void testDoubleRegister()
        {
            TestRegister::assign(0xf00f);
            TS_ASSERT_EQUALS(TestRegister::value, 0xf00f);
        }
};
