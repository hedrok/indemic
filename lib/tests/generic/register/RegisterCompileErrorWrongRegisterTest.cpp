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

class TCCR0B : public IndeMic::RegisterBase<TestMicrocontroller, 257, TCCR0B>
{
    public:
        constexpr static auto FOC0A = IndeMic::RegisterBit<TCCR0B, 7>();
        constexpr static auto FOC0B = IndeMic::RegisterBit<TCCR0B, 6>();
        constexpr static auto WGM02 = IndeMic::RegisterBit<TCCR0B, 3>();
        constexpr static auto CS02  = IndeMic::RegisterBit<TCCR0B, 2>();
        constexpr static auto CS01  = IndeMic::RegisterBit<TCCR0B, 1>();
        constexpr static auto CS00  = IndeMic::RegisterBit<TCCR0B, 0>();
};
constexpr IndeMic::RegisterBit<TCCR0B, 7> TCCR0B::FOC0A;
constexpr IndeMic::RegisterBit<TCCR0B, 6> TCCR0B::FOC0B;
constexpr IndeMic::RegisterBit<TCCR0B, 3> TCCR0B::WGM02;
constexpr IndeMic::RegisterBit<TCCR0B, 2> TCCR0B::CS02;
constexpr IndeMic::RegisterBit<TCCR0B, 1> TCCR0B::CS01;
constexpr IndeMic::RegisterBit<TCCR0B, 0> TCCR0B::CS00;

int main()
{
    // Compilation error: cannot set TCCR0A value to TCCR0B
    TCCR0B::set(TCCR0A::COM0A1 | TCCR0A::WGM00);
    return 0;
}
