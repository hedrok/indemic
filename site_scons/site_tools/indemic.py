# coding=UTF-8
#-------------------------------------------------------------------------
# IndeMic: Indepedndent of Microcontroller library
#-------------------------------------------------------------------------
#
# == About ==
#
# This builder selects mcu by filename and generates linker script for that
# mcu (currently only AVR is supported)
#
# == Usage: ==
#
# env.indemicBoard('target.architecture.micro.elf')
# e.g. target.avr.at90usb162.elf
#
# Will create at90usb162.x linker script, and compile
# sources with -mmcu=at90usb162 flag
#
# env.indemicBoardSimAvr will do the same, but will add include flags for
# simavr

from SCons.Script import *
from SCons.Builder import Builder
import os

class BaseFamily:
    def getName(self):
        raise NotImplementedError()
    def getEnv(self):
        return self._env
    def getBuilders(self):
        return self._builders

    def _initEnv(self, toolchain_prefix):
        """
        Initializes self._env by toolchain prefix
        TODO: remove hardcoded path to indemic library
        """
        self._env = Environment(
            CC = toolchain_prefix + '-gcc',
            CXX = toolchain_prefix + '-g++',
            CPPPATH = ['#/lib'],
            CXXFLAGS = '-std=c++11',
            CCFLAGS = '-Os -Wall',
            ENV = {'PATH' : os.environ['PATH']},
            BUILDERS = {
                'Hex' : Builder(
                    action = toolchain_prefix + '-objcopy -j .text -j .data -O ihex $SOURCE $TARGET',
                    suffix = '.hex',
                    src_suffix = '.elf',
                ),
                'Asm' : Builder(
                    action = toolchain_prefix + '-objdump -d $SOURCE > $TARGET',
                    suffix = '.s',
                    src_suffix = '.elf',
                ),
            }
        )

class AVRFamily(BaseFamily):
    """
    AVR family class
    """
    def __init__(self):
        """
        Append builders to environment
        TODO: remove hardcoded path to indemic library
        """
        self._builders = {}
        self._initEnv('avr')

        def CheckPKG(context, name):
             context.Message( 'Checking for %s... ' % name )
             ret = context.TryAction('pkg-config --exists \'%s\'' % name)[0]
             context.Result( ret )
             return ret
        conf = Configure(self._env, custom_tests = {'CheckPKG' : CheckPKG})

        if not conf.CheckCC():
            self._env = False
        if self._env:
            have_simavr = False
            if conf.CheckPKG('simavr'):
                have_simavr = True
            if have_simavr:
                avr_sim_env = self._env.Clone()
                avr_sim_env.ParseConfig("pkg-config simavr --cflags")
                indemicBoardSimAvr = IndemicBoardBuilder({'avr': avr_sim_env})
                self._builders['indemicBoardSimAvr'] = indemicBoardSimAvr
        conf.Finish()

    def getName(self):
        return 'avr'
    def getEnv(self):
        return self._env
    def getBuilders(self):
        return self._builders

libopencm3_root = '/opt/libopencm3'
class STM32Family(BaseFamily):
    def __init__(self):
        self._initEnv('arm-none-eabi')
        self._env.Append(
            CPPPATH = [libopencm3_root + '/arm-none-eabi/include'],
            LIBPATH = [libopencm3_root + '/arm-none-eabi/lib'],
        )

        conf = Configure(self._env)
        if not conf.CheckCC():
            self._env = False
        conf.Finish()

    def getName(self):
        return 'stm32'
    def getEnv(self):
        return self._env
    def getBuilders(self):
        return {}

# A warning class to notify users of problems
class ToolIndeMicWarning(SCons.Warnings.Warning):
    pass

SCons.Warnings.enableWarningClass(ToolIndeMicWarning)

class LinkerScriptBuilder:
    """
    Class for 'building' linker scripts.
    Gets original linker script from GCC and patches it with interrupts
    """
    def __init__(self):
        self.linkerScripts = {}
        self.env = Environment(
            BUILDERS = {
                'LinkerScript': Builder(
                    action = self
                )
            },
            tools = ['textfile'],
            ENV = {'PATH' : os.environ['PATH']},
        )
        self.filepathByMicro = {}

    def _getLinkerScriptFromOutput(self, path):
        """
        Gets linkerscript from GCC verbose output file.
        @param path Path to output of GCC
        @returns
        """
        linkerScript = '';
        with open(path) as verboseOutput:
            if not verboseOutput:
                SCons.Warnings.warn(
                    ToolIndeMicWarning,
                    "Could not open temp file with linker script information: " + path
                )
                Exit(1)
            state = 'before'
            for line in verboseOutput:
                if line == 'using external linker script:\n':
                    state = 'linebefore'
                if line == '==================================================\n':
                    if state == 'linebefore':
                        state = 'script'
                        continue
                    elif state == 'script':
                        state = 'after'
                        break
                if state == 'script':
                    linkerScript += line
        return linkerScript

    def _getInterruptsByMicro(self, architecture, micro):
        """
        Gets interrupt list for microcontroller

        TODO: configure path to interrupts list
        @param architecture e.g. avr
        @param micro e.g. at90usb162
        @returns List of interrupts for microcontroller
        """
        ldscriptspath = '#lib/indemic/ldscripts'

        if architecture not in self.filepathByMicro:
            fpm = {}
            dictfile = File(ldscriptspath + '/' + architecture + '/dict')
            with open(dictfile.abspath) as df:
                if not df:
                    SCons.Warnings.warn(
                            ToolIndeMicWarning,
                            "Could not opent file: " + dictfile.abspath
                    )
                    Exit(1)
                for line in df:
                    if line[0] == '#':
                        continue
                    if line == '\n':
                        continue
                    parts = line.split()
                    if len(parts) > 2:
                        SCons.Warnings.warn(
                            ToolIndeMicWarning,
                            "Wrong dict format: '" + line + "' in file " + dictfile.abspath
                        )
                        Exit(1)
                    fpm[parts[0]] = parts[1]
            self.filepathByMicro[architecture] = fpm
        if micro not in self.filepathByMicro[architecture]:
            SCons.Warnings.warn(
                ToolIndeMicWarning,
                "Unknown microcontroller '" + micro + "' for architecture '" + architecture + "'"
            )
            Exit(1)
        filename = self.filepathByMicro[architecture][micro] + '.interrupts'
        intFile = File(ldscriptspath + '/' + architecture + '/' + filename)
        content = ''
        with open(intFile.abspath) as f:
            if not f:
                SCons.Warnings.warn(
                    ToolIndeMicWarning,
                    "Could not open file: " + intFile.abspath
                )
                Exit(1)
            content = f.read(10000)
        return content.split()

    def _patchLinkerScript(self, script, interrupts):
        """
        Patches linker script. Replaces vectors with list of specific vectors,
        discards vectors sections and add error_interrupt_handler

        @param script Content of initial linker script
        @param interrupts List of interrupts for specific micro
        @return (string) Patched script
        """
        searchSections = 'SECTIONS\n{\n'
        discardVectors = '  /DISCARD/ : {\n      *(.vectors)\n  }\n'
        script = script.replace(searchSections, searchSections + discardVectors)
        searchVectors = '    *(.vectors)\n    KEEP(*(.vectors))\n'
        interruptsStr = ''
        addr = 0
        # Each vector table item takes 4 bytes,
        # But least address item is 2 bytes,
        # so * 4 / 2 => * 2
        errorAddr = len(interrupts) * 2
        errorAddr = hex(errorAddr)
        errorAddr = errorAddr[2:]
        errorAddr = ('0' * (4 - len(errorAddr))) + errorAddr
        errorAddr = errorAddr[2:] + errorAddr[:2]
        for i in interrupts:
            interruptsStr += '    . = ' + str(addr) + ';\n'
            # 0x0c94 - rjmp in avr asm
            interruptsStr += '    FILL(0x0c94);\n'
            interruptsStr += '    . = . + 2;\n'
            # Jump to error_interrupt_handler if there is no
            # handler
            interruptsStr += '    FILL(0x' + errorAddr + ');\n'
            interruptsStr += '    *(.' + i + ')\n';
            interruptsStr += '    KEEP(*(.' + i + '))\n';
            interruptsStr += '\n'
            addr += 4
        interruptsStr += '    . = ' + str(addr) + ';\n'
        interruptsStr += '    FILL(0);\n\n'
        interruptsStr += '    *(.error_interrupt_handler)\n'
        interruptsStr += '    KEEP(*(.error_interrupt_handler))\n\n'
        script = script.replace(searchVectors, interruptsStr)
        return script

    def __call__(self, env, target, source = None):
        """
        Call as builder for scons. 

        @param target List of targets, currently only one target is supported.
                      It should have name like name.micro.arch.x
        @param source Ignored
        @param env    Scons env, should be self.env
        @return Linker script node
        """
        resultpath = target[0].abspath

        parts = target[0].abspath.split('/')
        if len(parts) < 3:
            SCons.Warnings.warn(
                ToolIndeMicWarning,
                  "buildLinkerScript requires target to have filename name.micro.arch.x\n"
                + "                  e.g. avr/at90usb162.x\n"
                + "Got filename: " + target[0].abspath
            )
            Exit(1)

        architecture = parts[-2] # e.g. avr
        micro = parts[-1]        # e.g. at90usb162.x
        micro = micro.split('.') # ['at90usb162', 'x']
        micro = micro[0]
        # Prepare dummy.c file:
        dummy = env.Textfile(target = '#_build/dummy.c', source = ['int main(void){}'])
        tmpfile = File('#_build/' + architecture + '/' + micro + '-verbose.txt')
        # avr-gcc -Wl,--verbose -mmcu=MCU /tmp/dummy.c
        getLinkerScriptCmd = 'avr-gcc -Wl,--verbose -mmcu=' +  micro + ' ' + dummy[0].abspath + ' -o ' + dummy[0].abspath + '.out > ' + tmpfile.abspath
        if env.Execute(getLinkerScriptCmd):
            SCons.Warnings.warn(
                ToolIndeMicWarning,
                "Failed to execute: '" + getLinkerScriptCmd + "'"
            )
            Exit(1)
        linkerScript = self._getLinkerScriptFromOutput(tmpfile.abspath)
        interrupts = self._getInterruptsByMicro(architecture, micro)
        patchedStr = self._patchLinkerScript(linkerScript, interrupts)
        with open(target[0].abspath, 'w') as f:
            if not f:
                SCons.Warnings.warn(
                    ToolIndeMicWarning,
                    "Could not open file '" + target[0].abspath + "' for writing"
                )
                Exit(1)
            f.write(patchedStr)

    def getLinkerScriptPath(self, architecture, micro):
        """
        Gets linker script path by architecture and micro

        @param architecture e.g. avr
        @param micro e.g. at90usb162
        @returns Path to linker script generated for this micro
        """
        if architecture != 'avr':
            SCons.Warnings.warn(
                ToolIndeMicWarning,
                "Currently only avr architecture is supported"
            )
            Exit(1)
        path = File('#_build/' + architecture + '/' + micro + '.x').abspath
        return path

    def getNode(self, architecture, micro):
        """
        Gets node for linkerscript for architecture/micro

        @param architecture e.g. avr
        @param micro e.g. at90usb162
        @returns Scons node of linker script, either calls
                 env.LinkerScript for this linker script
                 or returns cached one
        """
        path = self.getLinkerScriptPath(architecture, micro)
        if architecture not in self.linkerScripts:
            self.linkerScripts[architecture] = {}
        if micro not in self.linkerScripts[architecture]:
            self.linkerScripts[architecture][micro] = self.env.LinkerScript(path, [])
        return self.linkerScripts[architecture][micro]

class IndemicBoardBuilder:
    """
    Builder for indemic boards. Adds options for mcu and linker script
    """

    linkerBuilder = LinkerScriptBuilder()

    def __init__(self, initialArchitecturesDict):
        """ Constructor """
        self.envByArch = {}
        self.envByMicro = {}
        for envkey in initialArchitecturesDict:
            self.envByArch[envkey] = initialArchitecturesDict[envkey]
            if self.envByArch[envkey]:
                self.envByArch[envkey] = self.envByArch[envkey].Clone()
            self.envByMicro[envkey] = {}

    def __call__(self, env, target, sources = None):
        """
        Call as SCons builder. Builds elf from sources, selects
        mcu and architecture by target name

        @param target Resulting elf to build
        @param sources Sources to build from
        @returns Resulting elf node
        """

        target = Flatten(Split(target))
        parts = target[0].split('.')
        if len(parts) < 3:
            SCons.Warnings.warn(
                ToolIndeMicWarning,
                "buildIndemicBoard requires target to have filename name.micro.arch.elf\n"
                "                  e.g. some.at90usb162.avr.elf\n"
                "Got filename: " + target[0]
            )
            Exit(1)

        architecture = parts[-2] # e.g. avr
        micro = parts[-3]        # e.g. at90usb162

        if architecture not in self.envByArch:
            SCons.Warnings.warn(
                ToolIndeMicWarning,
                "Architecture " + architecture + " is unknown."
            )
            Exit(1)

        if not self.envByArch[architecture]:
            SCons.Warnings.warn(
                ToolIndeMicWarning,
                "Skipping target '" + target[0] + "' as AVR architecture is unavailable.\n"
                "Check for avr-gcc in your path\n"
            )
            return None

        if micro not in self.envByMicro[architecture]:
            self._initMicro(architecture, micro)
        
        avrEnv = self.envByMicro[architecture][micro]
        prog = avrEnv.Program(target, sources)
        avrEnv.Hex(prog)
        avrEnv.Asm(prog)
        if architecture == 'avr':
            Depends(prog, self.linkerBuilder.getNode(architecture, micro))

        return prog

    def _initMicro(self, architecture, micro):
        """
        Initializes internal fields for new microcontroller
        Creates new environment with -mmcu flag, adds -T flag with
        linker script

        @param architecture e.g. avr
        @param micro e.g. at90usb162
        """
        cflags = ''
        ldflags = ''
        libs = []
        if architecture == 'stm32':
            if micro[:7] == 'stm32f4':
                cflags = '-DSTM32F4 -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16'
                ldflags = cflags + '-l'
                libs = ['opencm3_stm32f4']
            else:
                SCons.Warnings.warn(
                    ToolIndeMicWarning,
                    "Unknown micro: " + micro
                )
                Exit(1)
            ldflags = cflags
        elif architecture == 'avr':
            cflags = '-mmcu=' + micro
            ldflags = cflags + ' -T' + self.linkerBuilder.getNode(architecture, micro)[0].abspath

        env = self.envByArch[architecture]
        if not env:
            return None
        env = env.Clone()
        env.Append(
            CCFLAGS = ' ' + cflags,
            LINKFLAGS = ' ' + ldflags,
            LIBS = libs,
        )
        self.envByMicro[architecture][micro] = env

def generate(env, **kwargs):
    d = {}
    builders = {}
    families = [AVRFamily(), STM32Family()]

    for f in families:
        d[f.getName()] = f.getEnv()
        builders.update(f.getBuilders())

    indemicBoard = IndemicBoardBuilder(d)
    builders['indemicBoard'] = indemicBoard
    env.Append(
        BUILDERS = builders,
    )

def exists(env):
    return True
