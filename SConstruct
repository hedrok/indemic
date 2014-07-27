import os

avr_env = Environment(
    CC='avr-gcc',
    CXX='avr-g++',
    CPPPATH = ['#/lib'],
    CXXFLAGS = '-std=c++11',
    CCFLAGS = '-Os -Wall',
    ENV = {'PATH' : os.environ['PATH']},
)

cxxtest_env = Environment(
    tools = ['default','cxxtest'],
    CPPPATH = ['#/lib'],
    CXXFLAGS = '-std=c++11 -D_CXXTEST_HAVE_EH',
)
cxxtest_env.ParseConfig("pkg-config simavr --cflags --libs")

class LinkerScriptBuilder:
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
        linkerScript = '';
        with open(path) as verboseOutput:
            if not verboseOutput:
                print("Could not open temp file with linker script information: " + path)
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
        if architecture not in self.filepathByMicro:
            fpm = {}
            dictfile = File('#lib/indemic/ldscripts/' + architecture + '/dict')
            with open(dictfile.abspath) as df:
                if not df:
                    print("Could not opent file: " + dictfile.abspath)
                    Exit(1)
                for line in df:
                    if line[0] == '#':
                        continue
                    if line == '\n':
                        continue
                    parts = line.split()
                    if len(parts) > 2:
                        print("Wrong dict format: '" + line + "' in file " + dictfile.abspath)
                        Exit(1)
                    fpm[parts[0]] = parts[1]
            self.filepathByMicro[architecture] = fpm
        if micro not in self.filepathByMicro[architecture]:
            print("Unknown microcontroller '" + micro + "' for architecture '" + architecture + "'")
            Exit(1)
        filename = self.filepathByMicro[architecture][micro] + '.interrupts'
        intFile = File('#lib/indemic/ldscripts/' + architecture + '/' + filename)
        content = ''
        with open(intFile.abspath) as f:
            if not f:
                print("Could not open file: " + intFile.abspath)
                Exit(1)
            content = f.read(10000)
        return content

    def _patchLinkerScript(self, script, interrupts):
        searchSections = 'SECTIONS\n{\n'
        discardVectors = '  /DISCARD/ : {\n      *(.vectors)\n  }\n'
        script = script.replace(searchSections, searchSections + discardVectors)
        searchVectors = '    *(.vectors)\n    KEEP(*(.vectors))\n'
        interruptsStr = ''
        addr = 0
        interrupts = interrupts.split()
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

    def __call__(self, target, source, env):
        resultpath = target[0].abspath

        parts = target[0].abspath.split('/')
        if len(parts) < 3:
            print("buildLinkerScript requires target to have filename name.micro.arch.x")
            print("                  e.g. avr/at90usb162.x")
            print("Got filename: " + target[0].abspath)
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
            print("Failed to execute: '" + getLinkerScriptCmd + "'") 
            Exit(1)
        linkerScript = self._getLinkerScriptFromOutput(tmpfile.abspath)
        interrupts = self._getInterruptsByMicro(architecture, micro)
        patchedStr = self._patchLinkerScript(linkerScript, interrupts)
        with open(target[0].abspath, 'w') as f:
            if not f:
                print("Could not open file '" + target[0].abspath + "' for writing")
                Exit(1)
            f.write(patchedStr)

    def getLinkerScriptPath(self, architecture, micro):
        if architecture != 'avr':
            print("Currently only avr architecture is supported")
            Exit(1)
        path = File('#_build/' + architecture + '/' + micro + '.x').abspath
        return path

    def getNode(self, architecture, micro):
        path = self.getLinkerScriptPath(architecture, micro)
        if architecture not in self.linkerScripts:
            self.linkerScripts[architecture] = {}
        if micro not in self.linkerScripts[architecture]:
            self.linkerScripts[architecture][micro] = self.env.LinkerScript(path, [])
        return self.linkerScripts[architecture][micro]

class IndemicBoardBuilder:
    linkerBuilder = LinkerScriptBuilder()

    def __init__(self, initialArchitecturesDict):
        self.envByArch = {}
        self.envByMicro = {}
        for envkey in initialArchitecturesDict:
            self.envByArch[envkey] = initialArchitecturesDict[envkey].Clone()
            self.envByMicro[envkey] = {}

    def __call__(self, target, sources):
        parts = target.split('.')
        if len(parts) < 3:
            print("buildIndemicBoard requires target to have filename name.micro.arch.cpp")
            print("                  e.g. some.at90usb162.avr.elf")
            print("Got filename: " + target)
            Exit(1)

        architecture = parts[-2] # e.g. avr
        micro = parts[-3]        # e.g. at90usb162

        if architecture not in self.envByArch:
            print("Architecture " + architecture + " is unknown.")
            Exit(1)
        if micro not in self.envByMicro[architecture]:
            self._initMicro(architecture, micro)
        
        prog = self.envByMicro[architecture][micro].Program(target, sources)
        Depends(prog, self.linkerBuilder.getNode(architecture, micro))
        return prog

    def _initMicro(self, architecture, micro):
        mcustring = ' -mmcu=' + micro
        env = self.envByArch[architecture].Clone()
        env.Append(
            CCFLAGS = mcustring,
            LINKFLAGS = mcustring + ' -T' + self.linkerBuilder.getNode(architecture, micro)[0].abspath
        )
        self.envByMicro[architecture][micro] = env

indemicBoard = IndemicBoardBuilder({'avr': avr_env})

Export('cxxtest_env', 'indemicBoard', 'avr_env', 'IndemicBoardBuilder');

SConscript(['lib/tests/SConscript'], variant_dir='lib/tests/_build', duplicate=0)
SConscript(['apps/SConscript'])
