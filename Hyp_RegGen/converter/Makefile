F77     = gfortran -O -fno-automatic -fPIC
C++     = g++ -g

.f.o:
	$(F77) -c -Wall $*.f -o $*.o

.c.o:
	$(C++) -c -Wall $*.c -o $*.o

.C.o:
	$(C++) -c -Wall -Weffc++ -Wuninitialized $*.C -o $*.o

EXE  = ./exe

OBJS = main.o writeHyp.o

$(EXE):	$(OBJS)
	$(C++) -o $(EXE)             \
	$(OBJS)                      \
	-lz
#	/usr/lib64/libz.so
#        /usr/lib/x86_64-linux-gnu/libz.so
#       /cern/pro/lib/libgeant321.a  \
#	/cern/pro/lib/libpacklib.a   \
#	/cern/pro/lib/libmathlib.a   \
#	/cern/pro/lib/libkernlib.a   \

clear:
	rm $(EXE) $(OBJS)
# DO NOT DELETE
