from SCons.Script import *
from SCons.Builder import Builder
import os

env = Environment()

def CheckPKGConfig(context, version):
     context.Message( 'Checking for pkg-config... ' )
     ret = context.TryAction('pkg-config --atleast-pkgconfig-version=%s' % version)[0]
     context.Result( ret )
     return ret

def CheckPKG(context, name):
     context.Message( 'Checking for %s... ' % name )
     ret = context.TryAction('pkg-config --exists \'%s\'' % name)[0]
     context.Result( ret )
     return ret

conf = Configure(env, custom_tests = { 'CheckPKGConfig' : CheckPKGConfig,
                                       'CheckPKG' : CheckPKG })
if not conf.CheckPKGConfig('0.15.0'):
     print 'pkg-config >= 0.15.0 not found.'
     Exit(1)
env = conf.Finish()

cxxtest_env = Environment(
    tools = ['default','cxxtest'],
    CPPPATH = ['#/lib'],
    CXXFLAGS = '-std=c++11 -D_CXXTEST_HAVE_EH',
)

indemic_env = Environment(
    tools = ['indemic'],
)

Export('cxxtest_env', 'indemic_env', 'conf')

if Tool('cxxtest').exists(cxxtest_env):
    SConscript(['lib/tests/SConscript'], variant_dir='lib/tests/_build', duplicate=0)
else:
    print("Skipping tests as no cxxtest is found")

SConscript(['apps/SConscript'])

