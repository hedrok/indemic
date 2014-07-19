#include <cstdint>
#include <indemic/generic/Register.h>
#include <indemic/generic/RegisterBit.h>
#include <indemic/generic/RegisterValue.h>

namespace
{

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
        constexpr static auto WGM00  = IndeMic::RegisterBit<TCCR0A, 0>();
};
constexpr IndeMic::RegisterBit<TCCR0A, 7> TCCR0A::COM0A1;
constexpr IndeMic::RegisterBit<TCCR0A, 0> TCCR0A::WGM00;

class TCCR0B : public IndeMic::RegisterBase<TestMicrocontroller, 257, TCCR0B> {};

#ifdef FAIL_COMPILATION
// Should not compile: Assigining TCCR0A bits to TCCR0B
typedef TCCR0B tRegister;
#else
// Should compile: Assigining TCCR0A bits to TCCR0A, all ok
typedef TCCR0A tRegister;
#endif

void test()
{
    tRegister::set(TCCR0A::COM0A1 | TCCR0A::WGM00);
}

}
