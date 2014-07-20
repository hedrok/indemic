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

indemic_architectures = {
    'avr' : avr_env,
}
def indemicBoard(target, sourcepath):
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
    # TODO: select linker script based on mcu
    specific_env.Append(
        CCFLAGS = mcustring,
        LINKFLAGS = mcustring + ' -T' + linkerscriptpath,
    )
    specific_env.Program(target, sourcepath)
    return None

Export('at90usb_env', 'cxxtest_env', 'indemicBoard');

SConscript(['lib/tests/SConscript'], variant_dir='lib/tests/_build', duplicate=0)
SConscript(['apps/SConscript'])
