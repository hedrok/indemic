/* Default linker script, for normal executables */
OUTPUT_FORMAT("elf32-avr","elf32-avr","elf32-avr")
OUTPUT_ARCH(avr:35)
MEMORY
{
  text   (rx)   : ORIGIN = 0, LENGTH = 64K
  data   (rw!x) : ORIGIN = 0x800060, LENGTH = 0xffa0
  eeprom (rw!x) : ORIGIN = 0x810000, LENGTH = 64K
  fuse      (rw!x) : ORIGIN = 0x820000, LENGTH = 1K
  lock      (rw!x) : ORIGIN = 0x830000, LENGTH = 1K
  signature (rw!x) : ORIGIN = 0x840000, LENGTH = 1K
}
SECTIONS
{
  /* Read-only sections, merged into text segment: */
  .hash          : { *(.hash)		}
  .dynsym        : { *(.dynsym)		}
  .dynstr        : { *(.dynstr)		}
  .gnu.version   : { *(.gnu.version)	}
  .gnu.version_d   : { *(.gnu.version_d)	}
  .gnu.version_r   : { *(.gnu.version_r)	}
  .rel.init      : { *(.rel.init)		}
  .rela.init     : { *(.rela.init)	}
  .rel.text      :
    {
      *(.rel.text)
      *(.rel.text.*)
      *(.rel.gnu.linkonce.t*)
    }
  .rela.text     :
    {
      *(.rela.text)
      *(.rela.text.*)
      *(.rela.gnu.linkonce.t*)
    }
  .rel.fini      : { *(.rel.fini)		}
  .rela.fini     : { *(.rela.fini)	}
  .rel.rodata    :
    {
      *(.rel.rodata)
      *(.rel.rodata.*)
      *(.rel.gnu.linkonce.r*)
    }
  .rela.rodata   :
    {
      *(.rela.rodata)
      *(.rela.rodata.*)
      *(.rela.gnu.linkonce.r*)
    }
  .rel.data      :
    {
      *(.rel.data)
      *(.rel.data.*)
      *(.rel.gnu.linkonce.d*)
    }
  .rela.data     :
    {
      *(.rela.data)
      *(.rela.data.*)
      *(.rela.gnu.linkonce.d*)
    }
  .rel.ctors     : { *(.rel.ctors)	}
  .rela.ctors    : { *(.rela.ctors)	}
  .rel.dtors     : { *(.rel.dtors)	}
  .rela.dtors    : { *(.rela.dtors)	}
  .rel.got       : { *(.rel.got)		}
  .rela.got      : { *(.rela.got)		}
  .rel.bss       : { *(.rel.bss)		}
  .rela.bss      : { *(.rela.bss)		}
  .rel.plt       : { *(.rel.plt)		}
  .rela.plt      : { *(.rela.plt)		}
  /* Internal text space or external memory.  */
  .text   :
  {
      . = 0;
      FILL(0x0c94);
      . = . + 2;
      FILL(0x3a00);
      *(.reset_vector)
      KEEP(*(.reset_vector))

      . = 4;
      FILL(0x0c94);
      . = . + 2;
      FILL(0x3a00);
      *(.int0_vector)
      KEEP(*(.int0_vector))

      . = 8;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00);
      *(.int1_vector)
      KEEP(*(.int1_vector))

      . = 12;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.int2_vector)
      KEEP(*(.int2_vector))

      . = 16;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.int3_vector)
      KEEP(*(.int3_vector))

      . = 20;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.int4_vector)
      KEEP(*(.int4_vector))

      . = 24;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.int5_vector)
      KEEP(*(.int5_vector))

      . = 28;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.int6_vector)
      KEEP(*(.int6_vector))

      . = 32;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.int7_vector)
      KEEP(*(.int7_vector))

      . = 36;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.pcint0_vector)
      KEEP(*(.pcint0_vector))

      . = 40;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.pcint1_vector)
      KEEP(*(.pcint1_vector))

      . = 44;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.usb_general_vector)
      KEEP(*(.usb_general_vector))

      . = 48;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.usb_endpoint_vector)
      KEEP(*(.usb_endpoint_vector))

      . = 52;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.wdt_vector)
      KEEP(*(.wdt_vector))

      . = 56;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.timer1_capt_vector)
      KEEP(*(.timer1_capt_vector))

      . = 60;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.timer1_compa_vector)
      KEEP(*(.timer1_compa_vector))

      . = 64;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.timer1_compb_vector)
      KEEP(*(.timer1_compb_vector))

      . = 68;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.timer1_compc_vector)
      KEEP(*(.timer1_compc_vector))

      . = 72;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.timer1_ovf_vector)
      KEEP(*(.timer1_ovf_vector))

      . = 76;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.timer0_compa_vector)
      KEEP(*(.timer0_compa_vector))

      . = 80;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.timer0_compb_vector)
      KEEP(*(.timer0_compb_vector))

      . = 84;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.timer0_ovf_vector)
      KEEP(*(.timer0_ovf_vector))

      . = 88;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.spi_stc_vector)
      KEEP(*(.spi_stc_vector))

      . = 92;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.usart1_rx_vector)
      KEEP(*(.usart1_rx_vector))

      . = 96;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.usart1_udre_vector)
      KEEP(*(.usart1_udre_vector))

      . = 100;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.usart1_tx_vector)
      KEEP(*(.usart1_tx_vector))

      . = 104;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.analog_comp_vector)
      KEEP(*(.analog_comp_vector))

      . = 108;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.ee_ready_vector)
      KEEP(*(.ee_ready_vector))

      . = 112;
      FILL(0x0c94)
      . = . + 2;
      FILL(0x3a00)
      *(.spm_ready_vector)
      KEEP(*(.spm_ready_vector))

      . = 116;
      FILL(0)

      *(.error_interrupt_handler)
      KEEP(*(.error_interrupt_handler))

      /*
      *(.vectors)
      KEEP(*(.vectors))
      */
    /* For data that needs to reside in the lower 64k of progmem.  */
     *(.progmem.gcc*)
    /* PR 13812: Placing the trampolines here gives a better chance
       that they will be in range of the code that uses them.  */
    . = ALIGN(2);
     __trampolines_start = . ;
    /* The jump trampolines for the 16-bit limited relocs will reside here.  */
    *(.trampolines)
     *(.trampolines*)
     __trampolines_end = . ;
     *(.progmem*)
    . = ALIGN(2);
    /* For future tablejump instruction arrays for 3 byte pc devices.
       We don't relax jump/call instructions within these sections.  */
    *(.jumptables)
     *(.jumptables*)
    /* For code that needs to reside in the lower 128k progmem.  */
    *(.lowtext)
     *(.lowtext*)
     __ctors_start = . ;
     *(.ctors)
     __ctors_end = . ;
     __dtors_start = . ;
     *(.dtors)
     __dtors_end = . ;
    KEEP(SORT(*)(.ctors))
    KEEP(SORT(*)(.dtors))
    /* From this point on, we don't bother about wether the insns are
       below or above the 16 bits boundary.  */
    *(.init0)  /* Start here after reset.  */
    KEEP (*(.init0))
    *(.init1)
    KEEP (*(.init1))
    *(.init2)  /* Clear __zero_reg__, set up stack pointer.  */
    KEEP (*(.init2))
    *(.init3)
    KEEP (*(.init3))
    *(.init4)  /* Initialize data and BSS.  */
    KEEP (*(.init4))
    *(.init5)
    KEEP (*(.init5))
    *(.init6)  /* C++ constructors.  */
    KEEP (*(.init6))
    *(.init7)
    KEEP (*(.init7))
    *(.init8)
    KEEP (*(.init8))
    *(.init9)  /* Call main().  */
    KEEP (*(.init9))
    *(.text)
    . = ALIGN(2);
     *(.text.*)
    . = ALIGN(2);
    *(.fini9)  /* _exit() starts here.  */
    KEEP (*(.fini9))
    *(.fini8)
    KEEP (*(.fini8))
    *(.fini7)
    KEEP (*(.fini7))
    *(.fini6)  /* C++ destructors.  */
    KEEP (*(.fini6))
    *(.fini5)
    KEEP (*(.fini5))
    *(.fini4)
    KEEP (*(.fini4))
    *(.fini3)
    KEEP (*(.fini3))
    *(.fini2)
    KEEP (*(.fini2))
    *(.fini1)
    KEEP (*(.fini1))
    *(.fini0)  /* Infinite loop after program termination.  */
    KEEP (*(.fini0))
     _etext = . ;
  }  > text
  .data	  : AT (ADDR (.text) + SIZEOF (.text))
  {
     PROVIDE (__data_start = .) ;
    /* --gc-sections will delete empty .data. This leads to wrong start
       addresses for subsequent sections because -Tdata= from the command
       line will have no effect, see PR13697.  Thus, keep .data  */
    KEEP (*(.data))
     *(.data*)
    *(.rodata)  /* We need to include .rodata here if gcc is used */
     *(.rodata*) /* with -fdata-sections.  */
    *(.gnu.linkonce.d*)
    . = ALIGN(2);
     _edata = . ;
     PROVIDE (__data_end = .) ;
  }  > data
  .bss   : AT (ADDR (.bss))
  {
     PROVIDE (__bss_start = .) ;
    *(.bss)
     *(.bss*)
    *(COMMON)
     PROVIDE (__bss_end = .) ;
  }  > data
   __data_load_start = LOADADDR(.data);
   __data_load_end = __data_load_start + SIZEOF(.data);
  /* Global data not cleared after reset.  */
  .noinit  :
  {
     PROVIDE (__noinit_start = .) ;
    *(.noinit*)
     PROVIDE (__noinit_end = .) ;
     _end = . ;
     PROVIDE (__heap_start = .) ;
  }  > data
  .eeprom  :
  {
    /* See .data above...  */
    KEEP(*(.eeprom*))
     __eeprom_end = . ;
  }  > eeprom
  .fuse  :
  {
    KEEP(*(.fuse))
    KEEP(*(.lfuse))
    KEEP(*(.hfuse))
    KEEP(*(.efuse))
  }  > fuse
  .lock  :
  {
    KEEP(*(.lock*))
  }  > lock
  .signature  :
  {
    KEEP(*(.signature*))
  }  > signature
  /* Stabs debugging sections.  */
  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }
  .note.gnu.build-id : { *(.note.gnu.build-id) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end ) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /* DWARF 3 */
  .debug_pubtypes 0 : { *(.debug_pubtypes) }
  .debug_ranges   0 : { *(.debug_ranges) }
  /* DWARF Extension.  */
  .debug_macro    0 : { *(.debug_macro) }
}
