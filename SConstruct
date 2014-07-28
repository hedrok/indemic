import os

cxxtest_env = Environment(
    tools = ['default','cxxtest'],
    CPPPATH = ['#/lib'],
    CXXFLAGS = '-std=c++11 -D_CXXTEST_HAVE_EH',
)
cxxtest_env.ParseConfig("pkg-config simavr --cflags --libs")

indemic_env = Environment(
    tools = ['indemic'],
)

Export('cxxtest_env', 'indemic_env')

SConscript(['lib/tests/SConscript'], variant_dir='lib/tests/_build', duplicate=0)
SConscript(['apps/SConscript'])
