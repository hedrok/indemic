#include <cxxtest/TestSuite.h>
#include <indemic/generic/RegisterMultiSetter.h>

template<uint64_t taddress>
class RegisterBase
{
    public:
        enum { address = taddress };
};
template<class R, uint64_t v>
class RegisterBit
{
    public:
        typedef R Register;
        enum { value = v };
};

class Reg1 : public RegisterBase<1> {};
class Reg27 : public RegisterBase<27> {};

class Functor
{
    public:
        template<typename C>
        static void processRegister()
        {
            if (std::is_same<typename C::Register, Reg1>()) {
                TS_ASSERT_EQUALS(3, C::value);
                assertedReg1++;
            }
            if (std::is_same<typename C::Register, Reg27>()) {
                TS_ASSERT_EQUALS(13, C::value);
                assertedReg27++;
            }
        }
        static int assertedReg1;
        static int assertedReg27;
};
int Functor::assertedReg1 = 0;
int Functor::assertedReg27 = 0;

class Reg1Bit1 : public RegisterBit<Reg1, 1> {};
class Reg1Bit2 : public RegisterBit<Reg1, 2> {};
class Reg27Bit1 : public RegisterBit<Reg27, 1> {};
class Reg27Bits3and4 : public RegisterBit<Reg27, 12> {};

class RegisterMultiSetterTestSuite : public CxxTest::TestSuite
{
    public:
        /**
         * testing RegisterMultiSetter
         */
        void testOutput()
        {
            IndeMic::RegisterMultiSetter<Functor, Reg27Bits3and4, Reg1Bit2, Reg1Bit1, Reg27Bit1>::work();
            TS_ASSERT_EQUALS(Functor::assertedReg1, 1);
            TS_ASSERT_EQUALS(Functor::assertedReg27, 1);
        }
};
