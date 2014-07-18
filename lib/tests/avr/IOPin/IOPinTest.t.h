#include <vector>

#include <cxxtest/TestSuite.h>

#include "sim_avr.h"
#include "avr_ioport.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"

class IOPinTestSuite : public CxxTest::TestSuite
{
    public:
        void setUp()
        {
            this->portState = 0;
            this->portChangesNum = 0;
            this->portExpected.clear();

            this->ddrState = 0;
            this->ddrChangesNum = 0;
            this->ddrExpected.clear();
        }

        void runTest(const char* fname, int cycles)
        {
            elf_firmware_t f;
            elf_read_firmware(fname, &f);

            avr_t * avr = NULL;

            avr = avr_make_mcu_by_name(f.mmcu);
            if (!avr) {
                TS_FAIL("avr_make_mcu failed");
                return;
            }

            avr_init(avr);
            avr_load_firmware(avr, &f);

            for (int i = 0; i < 8; i++) {
                avr_irq_register_notify(
                    avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), i),
                    [](struct avr_irq_t * irq, uint32_t value, void *param) {
                        IOPinTestSuite *p = static_cast<IOPinTestSuite*>(param);
                        if (value) {
                            p->portState |= (1 << irq->irq);
                        } else {
                            p->portState &= ~(1 << irq->irq);
                        }
                        if (p->portChangesNum < p->portExpected.size()) {
                            TS_ASSERT_EQUALS(
                                p->portState,
                                p->portExpected[p->portChangesNum]
                            );
                        }
                        p->portChangesNum++;
                    },
                    static_cast<void*>(this)
                );
            }
            avr_irq_register_notify(
                avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), IOPORT_IRQ_DIRECTION_ALL),
                [](struct avr_irq_t * irq, uint32_t value, void *param) {
                    IOPinTestSuite *p = static_cast<IOPinTestSuite*>(param);
                    p->ddrState = value;
                    if (p->ddrChangesNum < p->ddrExpected.size()) {
                        TS_ASSERT_EQUALS(
                            p->ddrState,
                            p->ddrExpected[p->ddrChangesNum]
                        );
                    }
                    p->ddrChangesNum++;
                },
                static_cast<void*>(this)
            );

            for (int i = 0; i < cycles; i++) {
		        avr_run(avr);
            }

            TS_ASSERT_EQUALS(portChangesNum, this->portExpected.size());
            TS_ASSERT_EQUALS(ddrChangesNum, this->ddrExpected.size());
        }

        /**
         * testing IOPin::setHigh,
         *         IOPin::setLow,
         *         IOPin::setValue
         */
        void testOutput()
        {
            // Set pins 0 and 1 output
            this->ddrExpected.push_back(0x3);
            // Pin 0 and 1 initialized low
            this->portExpected.push_back(0x0);
            this->portExpected.push_back(0x0);

            // First set 0 to high
            this->portExpected.push_back(0x1);
            // Set 1 to high
            this->portExpected.push_back(0x3);
            // Set 0 to low
            this->portExpected.push_back(0x2);
            // Set 1 to low
            this->portExpected.push_back(0x0);
            // Testing setValue function:
            // setting 1 to high using setValue
            this->portExpected.push_back(0x2);
            // setting 0 to high using setValue
            this->portExpected.push_back(0x3);
            // setting 0 to low using setValue
            this->portExpected.push_back(0x2);
            // setting 1 to low using setValue
            this->portExpected.push_back(0x0);

            runTest("avr/IOPin/avr-output.elf", 100);
        }

        /**
         * testing IOPin::setOutput,
         *         IOPin::setInput,
         */
        void testDdr()
        {
            // When setting pins 0 and 1 as output
            // their value changes to zero
            this->portExpected.push_back(0x0);
            this->portExpected.push_back(0x0);

            // First set 0 to output
            this->ddrExpected.push_back(0x1);
            // Then set 1 to output
            this->ddrExpected.push_back(0x3);
            // First set 0 to input
            this->ddrExpected.push_back(0x2);
            // Then set 1 to input
            this->ddrExpected.push_back(0x0);

            runTest("avr/IOPin/avr-ddr.elf", 100);
        }

    private:
        uint8_t portState;
        size_t portChangesNum;
        std::vector<uint8_t> portExpected;

        uint8_t ddrState;
        size_t ddrChangesNum;
        std::vector<uint8_t> ddrExpected;
};
