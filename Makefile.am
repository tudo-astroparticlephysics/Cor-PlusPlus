srcdir = @srcdir@
VPATH = @srcdir@

AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CXXFLAGS = -I$(top_srcdir) -pthread -std=c++14 -Wall -Wextra -Wnarrowing -pedantic @BAACK_CPPFLAGS@
 



# docu, if doxygen, CORSIKA so far never with doxygen
if HAVE_DOXYGEN
BAACK_Documentation:
	$(DOXYGEN) $(top_srcdir)/docs 
endif






nobase_include_HEADERS = 


## Dynstack
lib_LIBRARIES = libDynstack.a libRemotecontrol.a libBaack.a

libDynstack_a_SOURCES = dynstack/export.cpp 
libDynstack_a_CPPFLAGS = -O3 -g @BAACK_CPPFLAGS@

## Remotecontrol
libRemotecontrol_a_SOURCES = remote_control/communication/packet.cpp \
  remote_control/communication/message_queue.cpp \
  remote_control/export.cpp \
  remote_control/control/periodic_task.cpp \
  remote_control/control/main_control.cpp \
  remote_control/control/recv_cache.cpp
libRemotecontrol_a_CPPFLAGS = -O3 -g @BAACK_CPPFLAGS@ 


## Dependency library for Dynstack and Remotecontrol
libBaack_a_SOURCES = lib/data/corsika/event_header.cpp \
  lib/data/corsika/event_end.cpp \
  lib/data/corsika/steering_card.cpp \
  lib/data/corsika/run_end.cpp \
  lib/data/corsika/run_header.cpp \
  lib/io/network/server.cpp \
  lib/io/network/client.cpp \
  lib/io/network/dns_lookup.cpp \
  lib/io/network/win_socket.cpp \
  lib/io/network/unix_socket.cpp \
  lib/io/file.cpp \
  basic/header_manager.cpp \
  basic/header_set.cpp \
  basic/export.cpp \
  basic/basic.cpp 

libBaack_a_CPPFLAGS = -O3 -g @BAACK_CPPFLAGS@ 

if BAACK_WITHTEST

bin_PROGRAMS = dynstack_test remotecontrol_test library_test dynstack_benchmark

dynstack_test_SOURCES = test/dynstack/storage_test.cpp \
  test/dynstack/advanced_test.cpp \
  test/dynstack/unit_test.cpp \
  test/dynstack/wrapper_test.cpp 
dynstack_test_LDADD = libDynstack.a
dynstack_test_CPPFLAGS = -Og -g


remotecontrol_test_SOURCES = test/remote_control/message_queue_test.cpp \
  test/remote_control/protocol_test.cpp \
  test/remote_control/unit_test.cpp
remotecontrol_test_LDADD = libRemotecontrol.a 
remotecontrol_test_CPPFLAGS = -Og -g

library_test_SOURCES = test/lib/data/particle_test.cpp \
  test/lib/io/server_test.cpp \
  test/lib/io/client_test.cpp \
  test/lib/io/socket_test.cpp \
  test/lib/io/dns_lookup_test.cpp \
  test/lib/unit_test.cpp \
  test/lib/meta/tuple_test.cpp \
  test/lib/meta/list_test.cpp \
  test/lib/meta/is_callable_test.cpp
library_test_LDADD = libBaack.a
library_test_CPPFLAGS = -Og -g

#basic_test_SOURCES =
#basic_test_LDADD = libBaack.a 
#basic_test_CPPFLAGS = -Og -g

endif

if BAACK_WITHBENCH

dynstack_benchmark_SOURCES = benchmark/benchmark.cpp \
  benchmark/stack/real.cpp
dynstack_benchmark_LDADD = libDynstack.a
dynstack_benchmark_CPPFLAGS = -O3 -g

endif

EXTRA_DIST = LICENSE README


distclean-local:
	find -name "*.o" -exec -f rm {} \;
	rm -f *.a ; rm -f *.la ; rm -f *.so* ; 









