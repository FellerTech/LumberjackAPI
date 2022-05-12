project( 'lumberjack' , 'cpp'
  , version : '0.0.1.-1'
  , license : 'Apache 2.0'
  )


# Make the Hourglass project
hrgls_cmake = import('cmake')
hrgls_proj = hrgls_cmake.subproject('Hourglass', hrgls_cmake_options : ['DCMAKE_CXX_FLAGS=-fpic'])
hrgls_dep = hrgls_proj.get_variable('cm_hrgls_dep')

## Make the Hourglass project
schema_cmake = import('cmake')
schema_proj = schema_cmake.subproject('json-schema-validator')
##schema_proj = schema_cmake.subproject('json_schema_validator', schema_cmake_optons : ['DCMAKE_CXX_FLAGS=-fpic'])
##schema_dep = schema_proj.get_variable('cm_schema_dep')

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
  , link_with : [lumberjack_lib]
  , dependencies : [ thread_dep ]
  )


#cm = import('cmake')
#subp = cm.subproject('capnproto-c++-0.7.0', cmake_options : ['-DBUILD_TESTING=OFF', '-DCMAKE_CXX_FLAGS=-fpic'])
#dep = subp.dependency('capnp')

#executable('main', ['main.cpp'], dependencies: [dep])
