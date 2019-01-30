# Usage of this module as follows:
#
#     find_package(ZeroMQ)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  ZeroMQ_ROOT_DIR  Set this variable to the root installation of
#                            ZeroMQ if the module has problems finding
#                            the proper installation path.
#
# Variables defined by this module:
#
#  ZeroMQ_FOUND              System has ZeroMQ libs/headers
#  ZeroMQ_LIBRARIES          The ZeroMQ libraries
#  ZeroMQ_INCLUDE_DIR        The location of ZeroMQ headers
#  ZeroMQ_VERSION            The version of ZeroMQ

find_path(ZeroMQ_ROOT_DIR
  NAMES include/zmq.h
  )

find_library(ZeroMQ_LIBRARY
    NAMES zmq libzmq
    HINTS ${ZeroMQ_ROOT_DIR}/lib
  )


find_path(ZeroMQ_INCLUDE_DIR
  NAMES zmq.h
  HINTS ${ZeroMQ_ROOT_DIR}/include
  )

#function(extract_version_value value_name file_name value)
#  file(STRINGS ${file_name} val REGEX "${value_name} .")
#  string(FIND ${val} " " last REVERSE)
#  string(SUBSTRING ${val} ${last} -1 val)
#  string(STRIP ${val} val)
#  set(${value} ${val} PARENT_SCOPE)
#endfunction(extract_version_value)

#extract_version_value("ZMQ_VERSION_MAJOR" ${ZeroMQ_INCLUDE_DIR}/zmq.h MAJOR)
#extract_version_value("ZMQ_VERSION_MINOR" ${ZeroMQ_INCLUDE_DIR}/zmq.h MINOR)
#extract_version_value("ZMQ_VERSION_PATCH" ${ZeroMQ_INCLUDE_DIR}/zmq.h PATCH)

#set(ZeroMQ_VER "${MAJOR}.${MINOR}.${PATCH}")


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  ZeroMQ
  REQUIRED_VARS ZeroMQ_LIBRARY ZeroMQ_INCLUDE_DIR
  VERSION_VAR ZeroMQ_VER
  )

set(ZeroMQ_FOUND ${ZEROMQ_FOUND})
set(ZeroMQ_INCLUDE_DIRS ${ZeroMQ_INCLUDE_DIR})
set(ZeroMQ_LIBRARIES ${ZeroMQ_LIBRARY})
set(ZeroMQ_VERSION ${ZeroMQ_VER})



