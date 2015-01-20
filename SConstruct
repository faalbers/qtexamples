import os

platform = Environment()['PLATFORM']
if platform == 'posix':
    if os.environ['USER'] == 'frank':
        os.environ['QTDIR'] = '/home/frank/QtSDK/Desktop/Qt/474/gcc'
    else:
        os.environ['QTDIR'] = '/pixar/d2/sets/tools-base-02'
elif platform == 'win32':
    os.environ['QTDIR'] = 'C:/Qt/4.7.4'

env = Environment(tools=['default','qt'])

if env['PLATFORM'] == 'posix':
    env['QT_LIB'] = Split("""
        QtCore
        QtGui
    """)
elif env['PLATFORM'] == 'win32':
    env['QT_LIB'] = Split("""
        QtCore4
        QtGui4
    """)

Export('env')

SConscript('SConscript')
