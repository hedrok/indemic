#include <cstdint>
#include <indemic/generic/Register.h>
#include <indemic/generic/RegisterBit.h>
#include <indemic/generic/RegisterValue.h>

class TestMicrocontroller
{
    public:
        typedef uint8_t register_t;
};

class TCCR0A : public IndeMic::RegisterBase<TestMicrocontroller, 256, TCCR0A>
{
    public:
        constexpr static auto COM0A1 = IndeMic::RegisterBit<TCCR0A, 7>();
        constexpr static auto COM0A0 = IndeMic::RegisterBit<TCCR0A, 6>();
        constexpr static auto COM0B1 = IndeMic::RegisterBit<TCCR0A, 5>();
        constexpr static auto COM0B0 = IndeMic::RegisterBit<TCCR0A, 4>();
        constexpr static auto WGM01  = IndeMic::RegisterBit<TCCR0A, 1>();
        constexpr static auto WGM00  = IndeMic::RegisterBit<TCCR0A, 0>();
};
constexpr IndeMic::RegisterBit<TCCR0A, 7> TCCR0A::COM0A1;
constexpr IndeMic::RegisterBit<TCCR0A, 6> TCCR0A::COM0A0;
constexpr IndeMic::RegisterBit<TCCR0A, 5> TCCR0A::COM0B1;
constexpr IndeMic::RegisterBit<TCCR0A, 4> TCCR0A::COM0B0;
constexpr IndeMic::RegisterBit<TCCR0A, 1> TCCR0A::WGM01;
constexpr IndeMic::RegisterBit<TCCR0A, 0> TCCR0A::WGM00;

int main()
{
    // Compilation error: COM0A1 is one-bit wide
    TCCR0A::set(TCCR0A::COM0A1(1));
    return 0;
}
