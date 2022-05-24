# LumberjackAPI
General Logging API

# Meson Build
## configure meson with the specified build directory.
meson ../build/LumberjackAPI/

## If there are changes in the meson.build file, you may need to reconfigure
meson --reconfigure ../build/LumberjackAPI/


## Go to the build directory
cd ../build/LumberjackAPI

## Build with ninja
ninja

## Run unit tests
meson test
