import os

linkerscriptpath = Dir('.').abspath + '/lib/indemic/avr/avr35.x'
at90usb_env = Environment(
    CC='avr-gcc',
    CXX='avr-g++',
    CPPPATH = ['#/lib'],
    CXXFLAGS = '-std=c++11',
    CCFLAGS = '-Os -mmcu=at90usb162 -Wall',
    ENV = {'PATH' : os.environ['PATH']},
    LINKFLAGS = ('-mmcu=at90usb162 -T' + linkerscriptpath),
)
# We need only includes (not libs) of simavr for avr-mcu.c
at90usb_env.ParseConfig("pkg-config simavr --cflags")

cxxtest_env = Environment(
    tools = ['default','cxxtest'],
    CXXTEST_CPPPATH = ['#/lib'],
    CXXFLAGS = '-std=c++11',
    LINKFLAGS = ('-std=c++11 -I' + Dir('.').abspath + '/lib'),
)
cxxtest_env.ParseConfig("pkg-config simavr --cflags --libs")

Export('at90usb_env', 'cxxtest_env');

SConscript(['lib/tests/SConscript'], variant_dir='lib/tests/_build', duplicate=0)
