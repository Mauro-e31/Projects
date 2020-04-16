# makefile

all: dataserver client

common.o: common.h common.cpp
	g++-8 -g -w -std=c++2a -c common.cpp

FIFOreqchannel.o: RequestChannel.h FIFOreqchannel.h FIFOreqchannel.cpp
	g++-8 -g -w -std=c++2a -c FIFOreqchannel.cpp

MQRequestChannel.o: RequestChannel.h MQRequestChannel.h MQRequestChannel.cpp
	g++-8 -g -w -std=c++2a -c MQRequestChannel.cpp

KernelSemaphore.o: KernelSemaphore.h KernelSemaphore.cpp
	g++-8 -g -w -std=c++2a -c KernelSemaphore.cpp

SHMRequestChannel.o: RequestChannel.h SHMRequestChannel.h KernelSemaphore.o SHMRequestChannel.cpp
	g++-8 -g -w -std=c++2a -c SHMRequestChannel.cpp

client: client.cpp FIFOreqchannel.o common.o KernelSemaphore.o  MQRequestChannel.o SHMRequestChannel.o
	g++-8 -g -w -std=c++2a -o client client.cpp FIFOreqchannel.o common.o KernelSemaphore.o MQRequestChannel.o SHMRequestChannel.o -lpthread -lrt

#SHMreqchannel.o SHMBoundedBuffer.o KernelSemaphore.o
dataserver: dataserver.cpp  FIFOreqchannel.o common.o KernelSemaphore.o MQRequestChannel.o SHMRequestChannel.o
	g++-8 -g -w -std=c++2a -o dataserver dataserver.cpp FIFOreqchannel.o common.o KernelSemaphore.o MQRequestChannel.o SHMRequestChannel.o -lpthread -lrt

clean:
	rm -rf *.o *.csv fifo* dataserver client data*_*
