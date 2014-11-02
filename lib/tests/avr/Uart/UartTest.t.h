#include <vector>

#include <cxxtest/TestSuite.h>

#include <simavr/avr_uart.h>
#include <simavr/sim_elf.h>

class UartTestSuite : public CxxTest::TestSuite
{
    public:
        /**
         * Run test that checks Uart - each sent symbol should
         * be increased by one
         * @param fname Filename of avr firmware
         */
        void runTest(const char* fname)
        {
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
            _testByte = 'a';
	        avr_irq_t * uart_output = avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('1'), UART_IRQ_OUTPUT);
	        avr_irq_t * uart_xon = avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('1'), UART_IRQ_OUT_XON);
            avr_irq_register_notify(
                uart_output,
                [](struct avr_irq_t * irq, uint32_t value, void *param) {
                    UartTestSuite * _this = static_cast<UartTestSuite*>(param);
                    _this->_callnum++;
                    TS_ASSERT_EQUALS(value, _this->_testByte + 1);
                },
                static_cast<void*>(this)
            );
            avr_irq_register_notify(
                uart_xon,
                [](struct avr_irq_t * irq, uint32_t value, void *param) {
                    static bool called = false;
                    if (called) {
                        return;
                    }
                    called = true;
                    UartTestSuite * _this = static_cast<UartTestSuite*>(param);
	                avr_irq_t * uart_input = avr_io_getirq(_this->avr, AVR_IOCTL_UART_GETIRQ('1'), UART_IRQ_INPUT);
		            avr_raise_irq(uart_input, _this->_testByte);
                },
                static_cast<void*>(this)
            );

            // For reaction to input symbol
            for (avr_cycle_count_t i = 0; i < 200; i++) {
                avr_run(avr);
            }
            // Check that Uart character was sent
            TS_ASSERT_EQUALS(this->_callnum, 1);
        }

        /**
         * testing Uart of AT90USB
         */
        void testUart()
        {
            runTest("Uart/uart.at90usb162.avr.elf");
        }

    private:
        avr_t * avr;
        int _callnum;
        uint8_t _testByte;
};
