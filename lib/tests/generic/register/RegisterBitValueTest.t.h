#include <cxxtest/TestSuite.h>
#include <indemic/generic/Register.h>
#include <indemic/generic/RegisterBit.h>

class RegisterBitValueTestSuite : public CxxTest::TestSuite
{
    public:
        /**
         * testing getting value from RegisterBit
         */
        void testBitValue()
        {
            uint8_t &pseudoregister1 = TCCR0A::value;
            TCCR0A::set(1 << 7 | 1 << 5 | 1 << 1);
            TS_ASSERT_EQUALS(TCCR0A::COM0A::getValue(), 2);
            TS_ASSERT_EQUALS(TCCR0A::COM0B1::getValue(), 1);
            TS_ASSERT_EQUALS(TCCR0A::COM0B0::getValue(), 0);
            TS_ASSERT_EQUALS(TCCR0A::WGM01::getValue(), 1);
            TS_ASSERT_EQUALS(TCCR0A::WGM00::getValue(), 0);
            TS_ASSERT_EQUALS(TCCR0A::COM0A::getValueUnshifted(), 1 << 7);
        }
        class TestMicrocontroller
        {
            public:
                typedef uint8_t register_t;
                typedef uint8_t register_value_t;
        };

        class TCCR0A : public IndeMic::RegisterSettable<TestMicrocontroller, 0, TCCR0A>
        {
            public:
                typedef IndeMic::RegisterBit<TCCR0A, 6, 2> COM0A;
                typedef IndeMic::RegisterBit<TCCR0A, 5> COM0B1;
                typedef IndeMic::RegisterBit<TCCR0A, 4> COM0B0;
                typedef IndeMic::RegisterBit<TCCR0A, 1> WGM01 ;
                typedef IndeMic::RegisterBit<TCCR0A, 0> WGM00 ;

                static inline TestMicrocontroller::register_t& reg()
                {
                    return value;
                }
                static uint8_t value;
        };
};
uint8_t RegisterBitValueTestSuite::TCCR0A::value = 0;
