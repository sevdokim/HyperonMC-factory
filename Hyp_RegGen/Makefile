PACKAGE   = HyperonMC_RegGen

ifeq ($(PLATFORM),)
PLATFORM = $(shell root-config --arch)
endif

TOPDIR  = $(shell pwd)/..
BINDIR  = $(TOPDIR)/tmp/tgt_$(PLATFORM)/$(PACKAGE)
LIBDIR  = $(TOPDIR)/lib

ifeq ($(ROOTSYS),)
ROOT_INCDIR = $(shell root-config --incdir)
ROOT_BINDIR = $(shell root-config --prefix)/bin
ROOT_ETCDIR = $(shell root-config --etcdir)/vmc
else
ROOT_INCDIR = $(ROOTSYS)/include
ROOT_BINDIR = $(ROOTSYS)/bin
ROOT_ETCDIR = $(ROOTSYS)/etc/vmc
endif

include $(ROOT_ETCDIR)/Makefile.$(PLATFORM)

############################### Sources #######################################

# Rules
#
include $(ROOT_ETCDIR)/MakeRules

# Dictionary
#
GDICT    := $(BINDIR)/$(PACKAGE)Cint.cxx
GDICTH   := $(GDICT:.cxx=.h)
GDICTO   := $(patsubst %.cxx,%.o,$(GDICT))

# Sources
#
CXXSRC	:= $(wildcard src/*.cxx) 
SRCS	:= $(CXXSRC)

# C++ Headers
#
HDRS    := $(wildcard include/*.h)
HDRS    := $(filter-out include/$(PACKAGE)LinkDef.h,$(HDRS))
HDRS    += include/$(PACKAGE)LinkDef.h

# Objects
#
CXXOBJ	:= $(patsubst %.cxx,$(BINDIR)/%.o,$(CXXSRC))
OBJS	:= $(CXXOBJ) $(GDICTO)

# Make include list
#
INCLUDES := -Iinclude

# C++ compilation flags
#
CXXFLAGS := $(CXXOPTS) -I. $(INCLUDES) -I$(ROOT_INCDIR) 

DEPINC 		+= -I. -I$(ROOT_INCDIR) $(INCLUDES) 

############################### Targets #######################################


SLIBRARY	= $(LIBDIR)/lib$(PACKAGE).$(SL)
ALIBRARY	= $(LIBDIR)/lib$(PACKAGE).a

default:	depend $(SLIBRARY)


$(LIBDIR)/lib$(PACKAGE).$(SL):  $(OBJS)
$(LIBDIR)/lib$(PACKAGE).a:      $(OBJS)

DICT:=	 	$(GDICT)

$(DICT): 	$(HDRS)

depend:		$(SRCS)

TOCLEAN		= $(BINDIR) $(LIBDIR)/lib$(PACKAGE).$(SL) $(LIBDIR)/lib$(PACKAGE).a

include $(ROOT_ETCDIR)/MakeMacros

############################### Dependencies ##################################

-include $(BINDIR)/Make-depend
