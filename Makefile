CC = g++ 
CFLAGS = -Wall -Wextra #-g #-O2
PROG = sqserver
SRC = main.cpp encryption.cpp connection.cpp daemon.cpp
HDR = sqserver.h encryption.h connection.h daemon.h #$(SRC:%.cpp=%.h)
OBJ = $(SRC:%.cpp=%.o) 
LOG = *.log
DEP = .dep.mk


$(PROG): $(OBJ) $(DEP)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJ)
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<
	
dep: $(DEP)
	
.dep.mk: $(SRC)
	$(CC) -MM $(SRC) > $(DEP)
	
-include $(DEP)

.PHONY: clean
clean:
	rm -rf $(OBJ) $(PROG) $(DEP) $(LOG)

