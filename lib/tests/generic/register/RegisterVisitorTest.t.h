#include <cxxtest/TestSuite.h>
#include <indemic/generic/Register.h>
#include <indemic/generic/RegisterBit.h>
#include <indemic/generic/RegisterVisitor.h>

static uint8_t pseudoregister1;
static uint8_t pseudoregister2;
constexpr uint64_t address1 = 0x611e38;

class RegisterVisitorTestSuite : public CxxTest::TestSuite
{
    public:
        /**
         * testing RegisterBase, RegisterBit and RegisterValue
         */
        void testVisitor()
        {
            pseudoregister1 = 0;
            pseudoregister2 = 0;
            if (address1 != reinterpret_cast<uint64_t>(&pseudoregister1)) {
                printf("&pseudoregister1: %lx, skipping test\n", reinterpret_cast<uint64_t>(&pseudoregister1));
                TS_SKIP("Skipped because of hardcoded variable address");
                return;
            }
            IndeMic::RegisterVisitor::set<TCCR0A::COM0A1, TCCR0B::CS02, TCCR0A::WGM01, TCCR0B::WGM02>();
            TS_ASSERT_EQUALS(pseudoregister1, (1 << 7) | (1 << 1));
            TS_ASSERT_EQUALS(pseudoregister2, (1 << 2) | (1 << 3));
            IndeMic::RegisterVisitor::clear<TCCR0B::CS0<>, TCCR0A::WGM01>();
            TS_ASSERT_EQUALS(pseudoregister1, (1 << 7));
            TS_ASSERT_EQUALS(pseudoregister2, (1 << 3));
            pseudoregister2 = 0;
            IndeMic::RegisterVisitor::set<TCCR0B::CS0<6>>();
            TS_ASSERT_EQUALS(pseudoregister2, 6);
        }
        class TestMicrocontroller
        {
            public:
                typedef uint8_t register_t;
                typedef uint8_t register_value_t;
        };

        class TCCR0A : public IndeMic::RegisterBase<TestMicrocontroller, address1, TCCR0A>
        {
            public:
                typedef IndeMic::RegisterBit<TCCR0A, 7> COM0A1;
                typedef IndeMic::RegisterBit<TCCR0A, 6> COM0A0;
                typedef IndeMic::RegisterBit<TCCR0A, 5> COM0B1;
                typedef IndeMic::RegisterBit<TCCR0A, 4> COM0B0;
                typedef IndeMic::RegisterBit<TCCR0A, 1> WGM01 ;
                typedef IndeMic::RegisterBit<TCCR0A, 0> WGM00 ;
        };

        class TCCR0B : public IndeMic::RegisterBase<TestMicrocontroller, address1 + 1, TCCR0B>
        {
            public:
                typedef IndeMic::RegisterBit<TCCR0B, 7> FOC0A;
                typedef IndeMic::RegisterBit<TCCR0B, 6> FOC0B;
                typedef IndeMic::RegisterBit<TCCR0B, 3> WGM02;
                typedef IndeMic::RegisterBit<TCCR0B, 2> CS02 ;
                typedef IndeMic::RegisterBit<TCCR0B, 1> CS01 ;
                typedef IndeMic::RegisterBit<TCCR0B, 0> CS00 ;

                template<uint64_t value = ((1 << 3) - 1)>
                using CS0 = IndeMic::RegisterBit<TCCR0B, 0, 3, value>;
        };

};
