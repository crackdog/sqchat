CC = g++ 
CFLAGS = #-O2 #-Wall -Wextra
PROG = sqserver
SRC = main.cpp encryption.cpp
HDR = sqserver.h encryption.h #$(SRC:%.cpp=%.h)
OBJ = $(SRC:%.cpp=%.o)
GIT = $(SRC) $(HDR) Makefile info.txt README.md runscript.sh
DEP = .dep.mk
LOGDIR = logs/


$(PROG): $(OBJ) $(DEP)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJ)
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<
	
dep: $(DEP)
	
.dep.mk: $(SRC)
	$(CC) -MM $(SRC) > $(DEP)
	
-include $(DEP)

.PHONY: addgit
addgit:
	git add $(GIT)
	
.PHONY: clearlog
clearlog:
	rm -rf $(LOGDIR)

.PHONY: clean
clean:
	rm -rf $(OBJ) $(PROG) $(DEP) $(LOGDIR) 

