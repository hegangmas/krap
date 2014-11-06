SHELL = /bin/sh
CC    = mpicc
CXX   = mpiCC
MAKE  = make
LIBS  = -lm -g
MPIRUN= mpirun
NP_ARG= -np 
HF_ARG= -hostfile
VALL  = valgrind -leak-check=full
GDB   = xterm -e gdb
DDD   =  xterm -e ddd
all: all-redirect


PROGRAMS = \
# colors \
# connectivity_c \
# cpi \
# dlock \
# gameoflifeparalel \
# gameoflifeparalelf \
# hello_cxx \
# masterservant \
# mdlock \
# newsendrecv \
# nodlock \
# pdlock \
# hello_c \
# test \
# qsortnavadni \
# npqs \
# paralelmergesort \
# sendrecv \
# nsendrecv \
# ring_cxx \
# ring_c \
# wait \
paralelsorte \
#pqs

SOURCES  = \
# colors.c \
# connectivity_c.c \
# cpi.c \
# dlock.c \
# gameoflifeparalel.c \
# gameoflifeparalelf.c \
# hello_cxx.cc \
# masterservant.c \
# mdlock.c \
# newsendrecv.c \
# nodlock.c \
# hello_c.c \
# test.c \
# qsortnavadni.c \
# npqs.c \
# paralelmergesort.c \
# sendrecv.c \
# nsendrecv.c \
# ring_cxx.cc \
# ring_c.c \
# wait.c \
paralelsorte.c \
#pqs.c


all-redirect: ${PROGRAMS}


clean:
	rm -rf *.o ${PROGRAMS} *~


.SUFFIXES: .c .cc
.c:
	${CC} ${CFLAGS} -o $* $< ${LIBS} 
.cc:
	${CXX} ${CXXFLAGS} -o $* $< ${LIBS} 


run: ${PROGRAMS}
#	${MPIRUN} ${HF_ARG} hostfile ${NP_ARG} 4 ${VALL} full nsendrecv
#	${MPIRUN} ${HF_ARG} hostfile ${NP_ARG} 4 ${VALL} full hello_c     
#	${MPIRUN} ${HF_ARG} hostfile ${NP_ARG} 2 ${VALL} full test
#	${MPIRUN} ${HF_ARG} hostfile ${NP_ARG} 2 ${VALL} full qsortnavadni
#	${MPIRUN} ${HF_ARG} hostfile ${NP_ARG} 4 ${VALL} full paralelmergesort
#	${MPIRUN}  ${HF_ARG} hostfile ${NP_ARG} 4  npqs
	${MPIRUN}  ${HF_ARG} hf ${NP_ARG} 4 paralelsorte
#	${MPIRUN}  ${HF_ARG} hf ${NP_ARG} 4 pqs




sources:
	@echo ${SOURCES}
