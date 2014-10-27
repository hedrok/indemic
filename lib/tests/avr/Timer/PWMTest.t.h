#include <cxxtest/TestSuite.h>

#include <simavr/sim_avr.h>
#include <simavr/sim_elf.h>
#include <simavr/avr_timer.h>

class PWMTestSuite : public CxxTest::TestSuite
{
    public:
        /**
         * Run test that asserts that first pin of port D is
         * alternated periodically
         * @param fname Filename of avr firmware
         * @param period Period in AVR cycles
         * @param highDuration Duration of high part of PWM in AVR cycles
         */
        void runTest(const char* fname, avr_cycle_count_t period, avr_cycle_count_t highDuration)
        {
            this->_period = period;
            this->_highDuration = highDuration;
            elf_firmware_t f;
            elf_read_firmware(fname, &f);

            avr = avr_make_mcu_by_name(f.mmcu);
            if (!avr) {
                TS_FAIL("avr_make_mcu failed");
                return;
            }

            avr_init(avr);
            avr_load_firmware(avr, &f);

            _callnum = 0;
            avr_irq_t * i_compb = avr_io_getirq(avr, AVR_IOCTL_TIMER_GETIRQ('0'), TIMER_IRQ_OUT_COMP + AVR_TIMER_COMPB);
            avr_irq_register_notify(
                i_compb,
                [](struct avr_irq_t * irq, uint32_t value, void *param) {
                    PWMTestSuite * _this = static_cast<PWMTestSuite*>(param);
                    static avr_cycle_count_t prev = 0;
                    static uint8_t prevValue = 1;
                    if (_this->_callnum > 2) {
                        TS_ASSERT_EQUALS(prevValue ^ 1, value);
                        prevValue = value;
                        if (value == 1) {
                            TS_ASSERT_EQUALS(_this->avr->cycle - prev, _this->_period);
                        } else {
                            TS_ASSERT_EQUALS(_this->avr->cycle - prev, _this->_highDuration);
                        }
                    }
                    if (value == 1) {
                        _this->_callnum++;
                        prev = _this->avr->cycle;
                    }
                },
                static_cast<void*>(this)
            );

            // 40 should be more than enough to start timer
            for (avr_cycle_count_t i = 0; i < period * 10 + 40; i++) {
                avr_run(avr);
            }
            TS_ASSERT(this->_callnum >= 10);
        }

        /**
         * testing PWM of AT90USB
         */
        void testPWMAT90USB()
        {
            runTest("Timer/pwm.at90usb162.avr.elf", 1000, 400);
        }

    private:
        avr_t * avr;
        avr_cycle_count_t _period;
        avr_cycle_count_t _highDuration;
        int _callnum;
};
