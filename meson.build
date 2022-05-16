project( 'lumberjack' , 'cpp'
  , version : '0.0.1.-1'
  , license : 'Apache 2.0'
  )


# Make the Hourglass project
hrgls_cmake = import('cmake')
hrgls_proj = hrgls_cmake.subproject('Hourglass', hrgls_cmake_options : ['DCMAKE_CXX_FLAGS=-fpic'])
hrgls_dep = hrgls_proj.get_variable('cm_hrgls_dep')
#hrgls_includes = hrgls_proj.get_variable('cm_hrgls_inc')
hrgls_includes = 'subprojects/Hourglass'

## Make the Hourglass project
schema_cmake = import('cmake')
schema_proj = schema_cmake.subproject('json-schema-validator')

# Define project dependencies
thread_dep = dependency('threads')

# Build libraries
lumberjack_lib = static_library('lumberjack'
  , 'src/lumberjack_basic.cpp'
  , dependencies: [hrgls_dep, thread_dep]
  )

# Build executable
executable( 'simple'
  , 'examples/simple.cpp'
  #, install : true
  #  ,  dependencies : [hrgls_dep, thread_dep, lumberjack_lib]
  , include_directories : ['src', hrgls_includes]
  , link_with : [lumberjack_lib]
  , dependencies : [ thread_dep ]
  )

# Build gtest
gtest_dep = dependency( 'gtest', fallback : ['gtest', 'gtest_dep'])

tests_src = ['tests/LumberjackUnitTests.cpp']
tests = executable('LumberjackUnitTests'
   , sources : tests_src
   , include_directories : ['src', hrgls_includes]
   , dependencies : [gtest_dep]
   , link_with : [lumberjack_lib]
   )

