


find_path(ZeroMQ_ROOT_DIR
  NAMES include/zmq.h
  )

find_library(ZeroMQ_LIB
    NAMES zmq libzmq
    HINTS ${ZeroMQ_ROOT_DIR}/lib
  )


find_path(ZeroMQ_INCLUDE_DIR
  NAMES zmq.h
  HINTS ${ZeroMQ_ROOT_DIR}/include
  )


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  ZeroMQ
  REQUIRED_VARS ZeroMQ_LIB ZeroMQ_INCLUDE_DIR   
  )

set(ZeroMQ_INCLUDE_DIRS ${ZeroMQ_INCLUDE_DIR})
set(ZeroMQ_LIBRARIES ${ZeroMQ_LIB})
set(ZeroMQ_VERSION ${ZeroMQ_VER})



