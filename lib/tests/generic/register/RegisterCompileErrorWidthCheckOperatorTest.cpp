#include <cstdint>
#include <indemic/generic/Register.h>
#include <indemic/generic/RegisterBit.h>
#include <indemic/generic/RegisterValue.h>

namespace
{

#ifdef FAIL_COMPILATION
// Should fail COM0A1 is one-bit-wide
constexpr uint8_t checkvalue = 2;
#else
// Should compile ok
constexpr uint8_t checkvalue = 1;
#endif


class TestMicrocontroller
{
    public:
        typedef uint8_t register_t;
        typedef uint8_t register_value_t;
};

class TCCR0A : public IndeMic::RegisterBase<TestMicrocontroller, 256, TCCR0A>
{
    public:
        constexpr static auto COM0A1 = IndeMic::RegisterBit<TCCR0A, 7>();
};
constexpr IndeMic::RegisterBit<TCCR0A, 7> TCCR0A::COM0A1;

void test()
{
    TCCR0A::set(TCCR0A::COM0A1.mask<checkvalue>());
}

}
