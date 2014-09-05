#include <cxxtest/TestSuite.h>
#include <indemic/generic/Register.h>
#include <indemic/generic/RegisterBit.h>
#include <indemic/generic/RegisterValue.h>

static uint8_t pseudoregister;
constexpr uint64_t address = 0x618c89;

class TestMicrocontroller
{
    public:
        typedef uint8_t register_t;
        typedef uint8_t register_value_t;
};

class TCCR0A : public IndeMic::RegisterBase<TestMicrocontroller, address, TCCR0A>
{
    public:
        constexpr static auto COM0A1 = IndeMic::RegisterBit<TCCR0A, 7>();
        constexpr static auto COM0A0 = IndeMic::RegisterBit<TCCR0A, 6>();
        constexpr static auto COM0B1 = IndeMic::RegisterBit<TCCR0A, 5>();
        constexpr static auto COM0B0 = IndeMic::RegisterBit<TCCR0A, 4>();
        constexpr static auto WGM01  = IndeMic::RegisterBit<TCCR0A, 1>();
        constexpr static auto WGM00  = IndeMic::RegisterBit<TCCR0A, 0>();

        constexpr static auto WGM0   = IndeMic::RegisterBit<TCCR0A, 0, 2>();
};

constexpr IndeMic::RegisterBit<TCCR0A, 7> TCCR0A::COM0A1;
constexpr IndeMic::RegisterBit<TCCR0A, 6> TCCR0A::COM0A0;
constexpr IndeMic::RegisterBit<TCCR0A, 5> TCCR0A::COM0B1;
constexpr IndeMic::RegisterBit<TCCR0A, 4> TCCR0A::COM0B0;
constexpr IndeMic::RegisterBit<TCCR0A, 1> TCCR0A::WGM01;
constexpr IndeMic::RegisterBit<TCCR0A, 0> TCCR0A::WGM00;

constexpr IndeMic::RegisterBit<TCCR0A, 0, 2> TCCR0A::WGM0;

class RegisterTestSuite : public CxxTest::TestSuite
{
    public:
        /**
         * testing RegisterBase, RegisterBit and RegisterValue
         */
        void testOutput()
        {
            pseudoregister = 0;
            if (address != reinterpret_cast<uint64_t>(&pseudoregister)) {
                printf("&pseudoregister: %lx, skipping test\n", reinterpret_cast<uint64_t>(&pseudoregister));
                TS_SKIP("Skipped because of hardcoded variable address");
                return;
            }
            constexpr IndeMic::RegisterValue<TCCR0A> v = TCCR0A::COM0A1 | TCCR0A::COM0B0 | TCCR0A::WGM00;
            TCCR0A::set(v);
            TS_ASSERT_EQUALS(pseudoregister, 0x91);
            TCCR0A::set(TCCR0A::WGM01);
            TS_ASSERT_EQUALS(pseudoregister, 0x93);
            TCCR0A::clear(TCCR0A::WGM01);
            TS_ASSERT_EQUALS(pseudoregister, 0x91);
            pseudoregister = 0x93;
            TCCR0A::clear(TCCR0A::WGM0.mask());
            TS_ASSERT_EQUALS(pseudoregister, 0x90);
            pseudoregister = 0x93;
            TCCR0A::clear(TCCR0A::WGM0);
            TS_ASSERT_EQUALS(pseudoregister, 0x90);
            TCCR0A::set(TCCR0A::WGM0.mask<2>());
            TS_ASSERT_EQUALS(pseudoregister, 0x92);
        }
};
