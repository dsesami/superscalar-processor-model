# template Makefile

CC = g++
OPT = -g  
WARN = -Wall
CFLAGS = $(OPT) $(WARN) 

# .cc files
SIM_SRC = main.cc fetch.cc decode.cc rename.cc regRead.cc dispatch.cc issue.cc execute.cc writeback.cc retire.cc

# .o files
SIM_OBJ = main.o fetch.o decode.o rename.o regRead.o dispatch.o issue.o execute.o writeback.o retire.o



  
#################################

# default rule

all: sim_ds
		@echo "my work is done here..."


# rule for making sim_cache
sim_ds: $(SIM_OBJ)
		$(CC) -o sim_ds $(CFLAGS) $(SIM_OBJ) -lm
		@echo "-----------DONE WITH SIM_DS -----------"


# generic rule for converting any .cc file to any .o file
.cc.o:
		$(CC) $(CFLAGS)  -c $*.cc


# type "make clean" to remove all .o files plus the sim_ds binary
clean:
		rm -f *.o sim_ds


# type "make clobber" to remove all .o files (leaves sim_ds binary)
clobber:
		rm -f *.o

