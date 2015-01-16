
#arm
#prefix := $(shell ../../../../build/ARM/buildroot-2011.11/output/target/usr/xenomai/usr/bin/xeno-config --prefix)
#i386
prefix := $(shell ../../../../build/i386/buildroot-2011.11/output/target/usr/xenomai/usr/bin/xeno-config --prefix)

#prefix := $(shell xeno-config --prefix)

ifeq ($(prefix),)
$(error Please add <xeno-install>/bin to your PATH variable)
endif

OBJS = socket_functions.o statcom.o

#ARM
#XENO=../../../../build/ARM/buildroot-2011.11/output/target/usr/xenomai/usr
#i386
XENO=../../../../build/i386/buildroot-2011.11/output/target/usr/xenomai/usr


LXRT_CFLAGS= -Wcast-align -g -W -Wall -isystem $(XENO)/include/xenomai -isystem $(XENO)/include/xenomai/rtdm -D_GNU_SOURCE -D_REENTRANT -pipe -D__XENO__
#LXRT_LDFLAGS= -lnative -L../../../../build/ARM/buildroot-2011.11/output/target/usr/lib -lxenomai -lpthread -lrt
LXRT_LDFLAGS= -lnative -L../../../../build/i386/buildroot-2011.11/output/target/usr/lib -lxenomai -lpthread -lrt

#ARM
#CC = ../../../../build/ARM/buildroot-2011.11/output/host/usr/bin/arm-unknown-linux-uclibcgnueabi-gcc #$(shell xeno-config --cc)
#i386
CC = ../../../../build/i386/buildroot-2011.11/output/host/usr/bin/i686-unknown-linux-uclibc-gcc #$(shell xeno-config --cc)

##Syslog path for logd 

SYSROOT = ../../../../build/i386/buildroot-2011.11/output/host/usr/i686-unknown-linux-uclibc/sysroot/
#SYSROOT = ../../../../build/arm/buildroot-2011.11/output/host/usr/arm-unknown-linux-uclibcgnueabi/sysroot/

#-I$(XENO)/include/xenomai/native

#CC = $(shell xeno-config --cc)
#LXRT_CFLAGS = $(shell xeno-config --skin=native --cflags)
#LXRT_LDFLAGS = $(shell xeno-config --skin=native --ldflags)
#CLFAGS_NOT_USED =
CFLAGS = -Wall -pedantic -Wold-style-definition -Wextra  -Wbad-function-cast -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wconversion -Wfloat-equal -Wformat=2 -Winit-self -Winline -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wstack-protector -Wswitch-default -Wswitch-enum -Winvalid-pch -Wstrict-aliasing=2 -Wunknown-pragmas -Wundef -Wunsafe-loop-optimizations -Wpointer-arith -Wcast-qual -Wwrite-strings -Winline -Wvolatile-register-var -Wno-unused-parameter -Wno-variadic-macros


all: engstatcom 
default: engstatcom
log : log1 log2
test: Xenomai_test1 Xenomai_test2 Xenomai_test3 Xenomai_test4 Xenomai_Semaphore Xenomai_Priority_Sceduling Xenomai_Priority_Sceduling_1 Xenomai_Round_Robbin SysLog_Ex3 bufp-label rt_printf xenomai-test-client xenomai-test-rtd satch Controller_time rt_time Clock_sync
timer: timer
log1: liblog.o 
log2 : log_client.o 
log3 : logd
logd : logd
liblog : liblog.so log_rt_client logd
pr:pr
engstatcom: engstatcom.o statcom.o $(OBJS)
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o engstatcom engstatcom.o $(OBJS) -rdynamic -lcrypto -lssl

engstatcom.o: engstatcom.c
	$(CC) $(LXRT_CFLAGS) $(CFLAGS) -c $<

statcom.o: statcom.c
	$(CC) $(LXRT_CFLAGS) $(CFLAGS) -c $<

socket_functions.o: socket_functions.c
	$(CC) $(LXRT_CFLAGS) $(CFLAGS) -c $<

trivial-periodic.o: trivial-periodic.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<
	
pr: trivial-periodic.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl
		
timer.o: timer.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<
		
timer: timer.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl
			
Xenomai_test1.o: Xenomai_test1.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<

Xenomai_test1: Xenomai_test1.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl
	
Xenomai_test2.o: Xenomai_test2.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<

Xenomai_test2: Xenomai_test2.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl	

Xenomai_test3.o: Xenomai_test3.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<

Xenomai_test3: Xenomai_test3.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl	

Xenomai_test4.o: Xenomai_test4.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<

Xenomai_test4: Xenomai_test4.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl	

Xenomai_Semaphore.o: Xenomai_Semaphore.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<
		
Xenomai_Semaphore: Xenomai_Semaphore.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl	
Xenomai_Priority_Sceduling.o: Xenomai_Priority_Sceduling.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<
		
Xenomai_Priority_Sceduling: Xenomai_Priority_Sceduling.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl	

Xenomai_Semaphore_1: Xenomai_Semaphore_1.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl	
Xenomai_Priority_Sceduling_1.o: Xenomai_Priority_Sceduling_1.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<
		
Xenomai_Priority_Sceduling_1: Xenomai_Priority_Sceduling_1.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl	
	

Xenomai_Round_Robbin.o: Xenomai_Round_Robbin.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<
		
Xenomai_Round_Robbin: Xenomai_Round_Robbin.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl
SysLog_Ex3.o:SysLog_Ex3.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<
		
SysLog_Ex3: SysLog_Ex3.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl
	
bufp-label.o:bufp-label.c	
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<

bufp-label:	 bufp-label.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl

rt_printf.o : rt_printf.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<
	
rt_printf:	 rt_printf.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl	
	
	
xenomai-test-rtd.o : xenomai-test-rtd.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<	

xenomai-test-rtd:	 xenomai-test-rtd.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o server $<  -rdynamic -lcrypto -lssl		
	
xenomai-test-client.o : xenomai-test-client.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<	
	
xenomai-test-client:	 xenomai-test-client.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o client $<  -rdynamic -lcrypto -lssl			


satch.o : satch.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<	
	
satch:	 satch.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl	
	

Controller_time.o : Controller_time.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<	

Controller_time : Controller_time.o	
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl
	
rt_time.o : rt_time.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<	

rt_time : rt_time.o	
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl	
	
Clock_sync.o : Clock_sync.c
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -c $<	

Clock_sync : Clock_sync.o	
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@ $<  -rdynamic -lcrypto -lssl		
	
log_rt.o : log_rt.c
	$(CC) $(LXRT_CFLAGS) $(CFLAGS)  -o $@  -c $<  --sysroot=$(SYSROOT)
	
log_linux.o : log_linux.c
	$(CC) $(LXRT_CFLAGS) $(CFLAGS)  -o $@  -c $<  --sysroot=$(SYSROOT)
	
liblog.so:log_linux.o log_rt.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -shared -o $@ log_rt.o log_linux.o -rdynamic --sysroot=$(SYSROOT)
	
log_client.o:log_client.c log_rt_client.c
	cp liblog.so ../../../../build/i386/buildroot-2011.11/output/target/usr/lib 
	$(CC) $(LXRT_CFLAGS) $(CFLAGS) -c log_client.c --sysroot=$(SYSROOT)
	$(CC) $(LXRT_CFLAGS) $(CFLAGS) -c log_rt_client.c --sysroot=$(SYSROOT)
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o log_client log_client.o -llog -rdynamic  --sysroot=$(SYSROOT)
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o log_rt_client log_rt_client.o -llog -rdynamic  --sysroot=$(SYSROOT)

log_rt_client.o: log_rt_client.c
	cp liblog.so ../../../../build/i386/buildroot-2011.11/output/target/usr/lib 
	$(CC) $(LXRT_CFLAGS) $(CFLAGS)  -o $@  -c $<  --sysroot=$(SYSROOT)

log_rt_client:log_rt_client.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o $@  $< -llog -rdynamic  --sysroot=$(SYSROOT)		
	
logd: liblog.so logd.c
	cp liblog.so ../../../../build/i386/buildroot-2011.11/output/target/usr/lib
	$(CC) $(LXRT_CFLAGS) $(CFLAGS) -c logd.c -o logd.o --sysroot=$(SYSROOT)
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) -o logd logd.o -llog -rdynamic --sysroot=$(SYSROOT)
		
	
#log_client: log_client.o
#	gcc -o log_client log_client.o -L. -l_mylib
		

clean:
clean:
	rm -f *.o *.a *.so engstatcom Xenomai_test1 timer pr Xenomai_test3 Xenomai_test2 Xenomai_test4 Xenomai_Priority_Sceduling Xenomai_Semaphore Xenomai_Round_Robbin Xenomai_Priority_Sceduling_1 log_client log logd Xenomai_test1 Xenomai_test2 Xenomai_test3 Xenomai_test4 Xenomai_Semaphore Xenomai_Priority_Sceduling Xenomai_Priority_Sceduling_1 Xenomai_Round_Robbin SysLog_Ex3 bufp-label rt_printf satch rt_time Clock_sync log_rt_client
.PHONY: clean



#gcc -L/home/merom/freedcs/freedcs-code1/Controller/src/Controller/ -Wl,-rpath=/home/merom/freedcs/freedcs-code1/Controller/src/Controller/ -Wall -o log_client log_client.c -llog
# Working Log make 
#lib_mylib.o :log.c
#	$(CC) -c $< -o $@
#	ar rcs lib_mylib.a $@
#	
#log_client.o:log_client.c
#	$(CC) -c $< -o $@
#		
#log_client: log_client.o
#	gcc -o $@ $< -L. -l_mylib
#	
#	
#	log: lib_mylib.o log_client
#
#   gcc -L/usr/lib/ -Wl,-rpath=/usr/lib -Wall -o log_client log_client.c -llog
# gcc -L/usr/lib -Wall -o log_client log_client.c -llog
#  gcc -L/usr/lib -Wall -o log_client log_client.c -llog

#    gcc -L/home/merom/freedcs/build/i386/buildroot-2011.11/output/target/usr/lib -Wall -o log_client log_client.c -llog





#log.o:log.c
#	gcc -c log.c -o lib_mylib.o 
#	ar rcs lib_mylib.a lib_mylib.o
#	
#log_client.o:log_client.c
#	gcc -c log_client.c -o log_client.o
#		
#log_client: log_client.o
#	gcc -o log_client log_client.o -L. -l_mylib
#
#




