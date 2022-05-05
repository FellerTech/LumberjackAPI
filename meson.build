project( 'lumberjack' , 'cpp'
  , version : '0.0.1.-1'
  , license : 'Apache 2.0'
  )


# Make the Hourglass project
cmake = import('cmake')
hrgls_proj = cmake.subproject('Hourglass')
hrgls_dep = hrgls_proj.get_variable('cm_hrgls_dep')

# Define project dependencies
thread_dep = dependency('threads')

# Build executable
executable( 'simple'
  , 'examples/simple.cpp'
  #, include_directories : hourglass_includes
  #, install : true
  ,  dependencies: [hrgls_dep, thread_dep]
  )

