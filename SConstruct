import os

# Create base environment
baseEnv = Environment()
#...further customization of base env
#baseEnv['TOOLSPATH'] = '/pixar/d2/sets/tools-base-02'
#baseEnv.Append(LIBPATH = [toolspath+'/lib'])

# platform and user specific
if baseEnv['PLATFORM'] == 'posix':
    baseEnv.Append(CPPFLAGS = [
        '-fPIC'
        ,'-std=c++0x'
        ])
    #PIXAR_FEDORA:qt5dir = '/home/afrank/QT521/5.2.1/gcc_64'
    qt5dir = '/home/frank/Qt5.4.0/5.4/gcc_64'
elif baseEnv['PLATFORM'] == 'win32':
    baseEnv.Append(CPPFLAGS = [
        #'-D_WIN32'
        #,'/W4'
        #,'/Wall'
        #,'/WX'
        #,'/Za'
        #,'/EHsc'
        ])
    baseEnv.Append(ENV = {'PATH' : os.environ['PATH']})
    baseEnv.Append(ENV = {'LIB' : os.environ.get('LIB')})
    baseEnv.Append(ENV = {'INCLUDE' : os.environ.get('INCLUDE')})
    qt5dir = 'c:/Qt/Qt5.4.0/5.4/msvc2013_64_opengl'

# Clone Qt environment
qtEnv = baseEnv.Clone()
# Set QT5DIR and PKG_CONFIG_PATH
qtEnv['ENV']['PKG_CONFIG_PATH'] = os.path.join(qt5dir, 'lib/pkgconfig')
qtEnv['QT5DIR'] = qt5dir
# Add qt5 tool
qtEnv.Tool('qt5')
#...further customization of qt env

# Export environments
Export('baseEnv qtEnv')

# Your other stuff...
# ...including the call to your SConscripts

SConscript('SConscript')

