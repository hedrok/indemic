#include <cstdint>
#include <indemic/generic/Register.h>
#include <indemic/generic/RegisterBit.h>
#include <indemic/generic/RegisterValue.h>
#include <indemic/generic/RegisterVisitor.h>

namespace
{

#ifdef FAIL_COMPILATION
// Should fail because 17 >= (2 << 3)
constexpr uint8_t checkvalue = 17;
#else
// Should compile ok because 7 < (2 << 3)
constexpr uint8_t checkvalue = 7;
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
        class Cs : public IndeMic::RegisterBit<TCCR0A, 0, 3> {};
};

void test()
{
    IndeMic::RegisterVisitor::set<TCCR0A::Cs::Value<checkvalue> >();
}

}
