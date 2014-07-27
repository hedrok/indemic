import os

avr_env = Environment(
    CC='avr-gcc',
    CXX='avr-g++',
    CPPPATH = ['#/lib'],
    CXXFLAGS = '-std=c++11',
    CCFLAGS = '-Os -Wall',
    ENV = {'PATH' : os.environ['PATH']},
)

linkerscriptpath = File('#lib/indemic/avr/avr35.x').abspath
at90usb_env = avr_env.Clone();
at90usb_env.Append(
    CCFLAGS = ' -mmcu=at90usb162',
    LINKFLAGS = (' -mmcu=at90usb162 -T' + linkerscriptpath),
)
# We need only includes (not libs) of simavr for avr-mcu.c
at90usb_env.ParseConfig("pkg-config simavr --cflags")

cxxtest_env = Environment(
    tools = ['default','cxxtest'],
    CPPPATH = ['#/lib'],
    CXXFLAGS = '-std=c++11 -D_CXXTEST_HAVE_EH',
)
cxxtest_env.ParseConfig("pkg-config simavr --cflags --libs")

def getLinkerScriptFromOutput(path):
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

def getInterruptsByMicro(architecture, micro):
    if not hasattr(getInterruptsByMicro, 'filepathByMicro'):
        getInterruptsByMicro.filepathByMicro = {}
    if architecture not in getInterruptsByMicro.filepathByMicro:
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
        getInterruptsByMicro.filepathByMicro[architecture] = fpm
    if micro not in getInterruptsByMicro.filepathByMicro[architecture]:
        print("Unknown microcontroller '" + micro + "' for architecture '" + architecture + "'")
        Exit(1)
    filename = getInterruptsByMicro.filepathByMicro[architecture][micro] + '.interrupts'
    intFile = File('#lib/indemic/ldscripts/' + architecture + '/' + filename)
    content = ''
    with open(intFile.abspath) as f:
        if not f:
            print("Could not open file: " + intFile.abspath)
            Exit(1)
        content = f.read(10000)
    return content

def patchLinkerScript(script, interrupts):
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

def buildLinkerScript(target, source, env):
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
    linkerScript = getLinkerScriptFromOutput(tmpfile.abspath)
    interrupts = getInterruptsByMicro(architecture, micro)
    patchedStr = patchLinkerScript(linkerScript, interrupts)
    with open(target[0].abspath, 'w') as f:
        if not f:
            print("Could not open file '" + target[0].abspath + "' for writing")
            Exit(1)
        f.write(patchedStr)

linker_env = Environment(
    BUILDERS = {
        'LinkerScript': Builder(
            action = buildLinkerScript
        )
    },
    tools = ['textfile'],
    ENV = {'PATH' : os.environ['PATH']},
)
indemic_architectures = {
    'avr' : avr_env,
}
def getLinkerScriptPath(architecture, micro):
    if architecture != 'avr':
        print("Currently only avr architecture is supported")
        Exit(1)
    path = File('#_build/' + architecture + '/' + micro + '.x').abspath
    linker_env.LinkerScript(path, [])
    return path

def indemicBoard(target, sourcepath, appenddict = {}):
    indemicBoard.linkerSkripts = {}
    #Source: /home/hedrok/projects/indemic/apps/led/boards/cnc.at90usb162.avr.cpp
    parts = sourcepath.split('.')
    if len(parts) < 3:
        print("buildIndemicBoard requires source to have filename name.micro.arch.cpp")
        print("                  e.g. some.at90usb162.avr.cpp")
        print("Got filename: " + sourcepath)
        Exit(1)

    architecture = parts[-2] # e.g. avr
    micro = parts[-3]        # e.g. at90usb162

    if architecture not in indemic_architectures:
        print("Architecture " + architecture + " is unknown.")
        Exit(1)

    mcustring = ' -mmcu=' + micro

    specific_env = indemic_architectures[architecture].Clone()

    if micro not in indemicBoard.linkerSkripts:
        indemicBoard.linkerSkripts[micro] = getLinkerScriptPath(architecture, micro)

    specific_env.Append(
        CCFLAGS = mcustring,
        LINKFLAGS = mcustring + ' -T' + indemicBoard.linkerSkripts[micro],
    )
    specific_env.Append(**appenddict)
    prog = specific_env.Program(target, sourcepath)
    
    Depends(prog, indemicBoard.linkerSkripts[micro])
    return None

Export('at90usb_env', 'cxxtest_env', 'indemicBoard');

SConscript(['lib/tests/SConscript'], variant_dir='lib/tests/_build', duplicate=0)
SConscript(['apps/SConscript'])
