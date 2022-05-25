#############################################
# Lumberjack API meson build file
#############################################
project( 'lumberjack' 
  , 'cpp'
  , version : '0.0.1.-1'
  , license : 'Apache 2.0'
  , default_options : ['c_std=c11', 'cpp_std=c++11']
  )

r = run_command('git', 'rev-parse', 'HEAD', check: true )
git_hash = r.stdout().strip()

#############################################
# Make the Hourglass project
#############################################
hrgls_cmake = import('cmake')
hrgls_proj = hrgls_cmake.subproject('Hourglass'
  , cmake_options : ['DCMAKE_CXX_FLAGS=-fpic']
  )
hrgls_dep = hrgls_proj.get_variable('cm_hrgls_dep')
hrgls_lib = hrgls_proj.dependency('hrgls')

hrgls_includes = [ 'subprojects/Hourglass',
  'subprojects/Hourglass/__CMake_build/' 
  ]

#############################################
## Make the json-schema-validator project
#############################################
#schema_cmake = import('cmake')
#schema_proj = schema_cmake.subproject('pboettch-json-schema-validator')


#############################################
#
#############################################
fttimer_proj = subproject('fttimer')
fttimer_dep = fttimer_proj.get_variable( 'fttimer_dep')
fttimer_inc = fttimer_proj.get_variable( 'fttimer_inc')

# Define project dependencies
thread_dep = dependency('threads')

# Build libraries
lumberjack_basic_lib = static_library( 'lumberjack'
  , 'src/lumberjack_basic.cpp'
  , include_directories : ['src', hrgls_includes, fttimer_inc]
  , dependencies: [hrgls_lib, thread_dep, fttimer_dep ]
  , cpp_args : [
      '-DLJ_VERSION="@0@"'.format(meson.project_version())
      , '-DLJ_HASH="@0@"'.format(git_hash)
    ]
  )

#############################################
# Build executable
#############################################
executable( 'simple'
  , 'examples/simple.cpp'
  #, install : true
  #  ,  dependencies : [hrgls_lib, thread_dep, lumberjack_lib]
  ,  dependencies : [hrgls_lib, thread_dep]
  , include_directories : ['src', hrgls_includes]
  , link_with : [lumberjack_basic_lib ]
  )

# Build gtest
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')
#gtest_dep = dependency( 'gtest', fallback : ['gtest', 'gtest_main_dep'])
#gmock_dep = dependency( 'gmock')


#############################################
# Run unit tests (meson test)
#############################################
tests_src = ['tests/LumberjackBasicUnitTests.cpp']
tests = executable('LumberjackBasicUnitTests'
   , sources : tests_src
   , include_directories : ['src', hrgls_includes]
   , dependencies : [gtest_dep, gmock_dep, fttimer_dep]
   , link_with : [lumberjack_basic_lib]
   )

test('Basic unit tests', tests)

#############################################
# Build documentation
#############################################
doxygen = find_program('doxygen', required: false)
if doxygen.found()
  #Set 
  cdata = configuration_data()
  cdata.set('VERSION', meson.project_version())
  cdata.set('GIT_HASH', git_hash )

  doxyfile = configure_file( input : 'Doxyfile.in'
    , output : 'Doxyfile'
    , configuration : cdata
  )

  docs = custom_target('doc'
    , build_by_default : true
    , command : [ doxygen, doxyfile ]
    , output : ['html']
  )
else
   warning('doxygen not found. Documentation disabled')
endif
