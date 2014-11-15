#include <cxxtest/TestSuite.h>
#include <indemic/generic/RegisterMultiSetter.h>

template<uint64_t taddress>
class RegisterBase
{
    public:
        enum { address = taddress };
};
template<class R, uint64_t v, uint64_t m>
class RegisterBit
{
    public:
        typedef R Register;
        enum { value = v };
        enum { mask = m };
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
                TS_ASSERT_EQUALS(2, C::value);
                TS_ASSERT_EQUALS(1, C::valueZeroes);
                assertedReg1++;
            }
            if (std::is_same<typename C::Register, Reg27>()) {
                TS_ASSERT_EQUALS(1+4+16, C::value);
                TS_ASSERT_EQUALS(8, C::valueZeroes);
                assertedReg27++;
            }
        }
        static int assertedReg1;
        static int assertedReg27;
};
int Functor::assertedReg1 = 0;
int Functor::assertedReg27 = 0;

class Reg1Bit1 : public RegisterBit<Reg1, 0, 1> {};
class Reg1Bit2 : public RegisterBit<Reg1, 2, 2> {};
class Reg27Bit1 : public RegisterBit<Reg27, 1, 1> {};
// Bits 3-5, 4th is unset
class Reg27Bits3till5 : public RegisterBit<Reg27, 4+16, 4+8+16> {};

class RegisterMultiSetterTestSuite : public CxxTest::TestSuite
{
    public:
        void setUp()
        {
            Functor::assertedReg1 = 0;
            Functor::assertedReg27 = 0;
        }
        /**
         * testing RegisterMultiSetter collecting bits
         */
        void testCollectingBits()
        {
            IndeMic::RegisterMultiSetter<Functor, Reg27Bits3till5, Reg1Bit2, Reg1Bit1, Reg27Bit1>::work();
            TS_ASSERT_EQUALS(Functor::assertedReg1, 1);
            TS_ASSERT_EQUALS(Functor::assertedReg27, 1);
        }

        template<typename... Bits>
        using Bundle = IndeMic::RegisterBitBundle<Bits...>;
        /**
         * testing RegisterMultiSetter with bit bundles
         * - same test, but with bundles
         */
        void testBitBundle()
        {
            IndeMic::RegisterMultiSetter<
                Functor,
                Bundle<
                    Reg27Bits3till5,
                    Reg1Bit2
                >,
                Reg27Bit1,
                Bundle<
                    Bundle<
                        Bundle<
                            Reg1Bit1
                        >
                    >
                >
            >::work();
            TS_ASSERT_EQUALS(Functor::assertedReg1, 1);
            TS_ASSERT_EQUALS(Functor::assertedReg27, 1);
        }
};
