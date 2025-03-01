F77 = gfortran
OPT = -O2 -g
F77FLAGS  = $(OPT) -I. -Wall -fbounds-check -fno-automatic -finit-local-zero -ffixed-line-length-none -fno-align-commons -fPIC
CFLAGS = -fPIC

#EXEC    = hyp_RegGen_main
#SOLIB   = hyp_RegGen.so
FSRC    = hyp_RegGen.f
CSRC    = trapfpe.c
OBJGEN  = hyp_RegGen.o
OBJMAIN = trapfpe.o
OBJFPE  = trapfpe.o
#F77LIBS = `cernlib packlib`
#F77LIBS = -L/usr/local/lib -lcrypt -lnsl -L/usr/lib/x86_64-linux-gnu/libkernlib.so -L/usr/lib/x86_64-linux-gnu/libmathlib.so
F77LIBS = -L/usr/local/lib -lcrypt -L/usr/lib/x86_64-linux-gnu/libkernlib.so -L/usr/lib/x86_64-linux-gnu/libmathlib.so
#F77LIBS = -L/usr/local/lib -lcrypt -static -lmathlib -lpacklib -lkernlib
#F77LIBS = -L/usr/local/lib -static -lmathlib -lpacklib -lkernlib
EXE     = ../../lib/libRegGen.so


.f.o:	
	$(F77) -c $(F77FLAGS) $(FSRC)
.c.o:	
	gcc -c $(CSRC) $(CFLAGS)

all:	$(EXE)

#$(EXEC): $(OBJMAIN) $(EXE)
#	$(F77) -o $(EXEC) $(OBJMAIN) $(OBJGEN) $(F77LIBS)
#	$(F77) -o $(EXEC) $(OBJMAIN) $(SOLIB) $(F77LIBS)

#$(SOLIB): $(OBJGEN)
#	$(F77) -o $(SOLIB) -shared $(OBJGEN)

$(EXE): $(OBJMAIN) $(OBJGEN)
	$(F77) -no-pie -o $(EXE) -shared $(OBJGEN) $(OBJFPE)

clean:	
	rm -f $(EXEC) $(OBJGEN) $(OBJMAIN) $(SOLIB) 
