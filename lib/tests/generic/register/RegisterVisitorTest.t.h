#include <cxxtest/TestSuite.h>
#include <indemic/generic/Register.h>
#include <indemic/generic/RegisterBit.h>
#include <indemic/generic/RegisterVisitor.h>

class RegisterVisitorTestSuite : public CxxTest::TestSuite
{
    public:
        /**
         * testing RegisterVisitor set/clear functionality
         */
        void testSetClear()
        {
            uint8_t &pseudoregister1 = TCCR0A::value.value;
            uint8_t &pseudoregister2 = TCCR0B::value.value;
            pseudoregister1 = pseudoregister2 = 0;
            IndeMic::RegisterVisitor::set<TCCR0A::COM0A1, TCCR0B::CS02, TCCR0A::WGM01, TCCR0B::WGM02>();
            TS_ASSERT_EQUALS(pseudoregister1, (1 << 7) | (1 << 1));
            TS_ASSERT_EQUALS(pseudoregister2, (1 << 2) | (1 << 3));
            IndeMic::RegisterVisitor::clear<TCCR0B::CS0, TCCR0A::WGM01>();
            TS_ASSERT_EQUALS(pseudoregister1, (1 << 7));
            TS_ASSERT_EQUALS(pseudoregister2, (1 << 3));
            pseudoregister2 = 0;
            IndeMic::RegisterVisitor::set<TCCR0B::CS0::Value<6>>();
            TS_ASSERT_EQUALS(pseudoregister2, 6);
        }

        /**
         * testing assign of RegisterVisitor
         */
        void testAssign()
        {
            // TCCR0B:
            // Initial value: 01010110 (0x56)
            // Assign:        10XXX010
            // Result:        10010010 (0x92)
            // Expected operations:
            // &= 10111010 (0xba)
            // |= 10000010 (0x82)
            //
            // TestReg:
            // Initial value: 01111111 (0xff)
            // Assign:        10001000 (0x88)
            // Result:        10001000
            // Expected operations:
            // = 10001000 (0x88)
            using OpLog = OperationsLogger<uint8_t>;
            OpLog &pseudoregister2 = TCCR0B::value;
            OpLog &pseudoregister3 = TestReg::value;
            pseudoregister2.value = 0x56;
            pseudoregister3.value = 0x7f;
            pseudoregister2.lst.clear();
            pseudoregister3.lst.clear();

            IndeMic::RegisterVisitor::assign<
                TCCR0B::FOC0A,
                TCCR0B::FOC0B::Value<0>,
                TCCR0B::CS0::Value<2>,
                TestReg::Bit7,
                TestReg::Bits0_6::Value<8>
            >();
            TS_ASSERT_EQUALS(pseudoregister2.value, 0x92);
            TS_ASSERT_EQUALS(pseudoregister3.value, 0x88);
            std::list<OpLog::OperationData> lst;
            lst.push_back(OpLog::OperationData(OpLog::Operation::And, 0xba));
            lst.push_back(OpLog::OperationData(OpLog::Operation::Or, 0x82));
            assertListsEqual(pseudoregister2.lst, lst);
            lst.clear();
            lst.push_back(OpLog::OperationData(OpLog::Operation::Assign, 0x88));
            assertListsEqual(pseudoregister3.lst, lst);
        }

        template<typename T>
        void assertListsEqual(const std::list<T> &l1, const std::list<T> &l2)
        {
            TS_ASSERT_EQUALS(l1.size(), l2.size());
            if (l1.size() != l2.size()) {
                return;
            }
            auto l1iter = l1.cbegin();
            auto l2iter = l2.cbegin();
            while (l1iter != l1.cend()) {
                TS_ASSERT_EQUALS(*l1iter, *l2iter);
                l1iter++;
                l2iter++;
            }
        }

        class TestMicrocontroller
        {
            public:
                typedef uint8_t register_t;
                typedef uint8_t register_value_t;
        };

        template<typename T>
        class OperationsLogger
        {
            public:
                enum class Operation
                {
                    Or,
                    And,
                    Assign
                };
                struct OperationData
                {
                    OperationData(Operation _operation, T _operand)
                        : operation(_operation)
                        , operand(_operand)
                    {}
                    Operation operation;
                    T operand;
                    bool operator==(const OperationData &other)
                    {
                        return operation == other.operation && operand == other.operand;
                    }
                };
                OperationsLogger() : value(0) {}
                T operator|=(T o)
                {
                    lst.push_back(OperationData(Operation::Or, o));
                    return value |= o;
                }
                T operator&=(T o)
                {
                    lst.push_back(OperationData(Operation::And, o));
                    return value &= o;
                }
                T operator=(T o)
                {
                    lst.push_back(OperationData(Operation::Assign, o));
                    return value = o;
                }
                T value;
                std::list<OperationData> lst;
        };

        class TCCR0A : public IndeMic::RegisterBase<TestMicrocontroller, 0, TCCR0A>
        {
            public:
                typedef IndeMic::RegisterBit<TCCR0A, 7> COM0A1;
                typedef IndeMic::RegisterBit<TCCR0A, 6> COM0A0;
                typedef IndeMic::RegisterBit<TCCR0A, 5> COM0B1;
                typedef IndeMic::RegisterBit<TCCR0A, 4> COM0B0;
                typedef IndeMic::RegisterBit<TCCR0A, 1> WGM01 ;
                typedef IndeMic::RegisterBit<TCCR0A, 0> WGM00 ;

                static inline OperationsLogger<uint8_t>& reg()
                {
                    return value;
                }
                static OperationsLogger<uint8_t> value;
        };

        class TCCR0B : public IndeMic::RegisterBase<TestMicrocontroller, 0, TCCR0B>
        {
            public:
                typedef IndeMic::RegisterBit<TCCR0B, 7> FOC0A;
                typedef IndeMic::RegisterBit<TCCR0B, 6> FOC0B;
                typedef IndeMic::RegisterBit<TCCR0B, 3> WGM02;
                typedef IndeMic::RegisterBit<TCCR0B, 2> CS02 ;
                typedef IndeMic::RegisterBit<TCCR0B, 1> CS01 ;
                typedef IndeMic::RegisterBit<TCCR0B, 0> CS00 ;

                typedef IndeMic::RegisterBit<TCCR0B, 0, 3> CS0;

                static inline OperationsLogger<uint8_t>& reg()
                {
                    return value;
                }
                static OperationsLogger<uint8_t> value;
        };
        
        class TestReg : public IndeMic::RegisterBase<TestMicrocontroller, 0, TestReg>
        {
            public:
                typedef IndeMic::RegisterBit<TestReg, 7> Bit7;
                typedef IndeMic::RegisterBit<TestReg, 0, 7> Bits0_6;

                static inline OperationsLogger<uint8_t>& reg()
                {
                    return value;
                }
                static OperationsLogger<uint8_t> value;
        };

};
RegisterVisitorTestSuite::OperationsLogger<uint8_t> RegisterVisitorTestSuite::TCCR0A::value;
RegisterVisitorTestSuite::OperationsLogger<uint8_t> RegisterVisitorTestSuite::TCCR0B::value;
RegisterVisitorTestSuite::OperationsLogger<uint8_t> RegisterVisitorTestSuite::TestReg::value;
