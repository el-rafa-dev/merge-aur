CC=g++
ONAME=merge #the output name
C_SOURCE= src/main.cc
OBJ = $(SRC:.c=.o)

all: $(ONAME)
.PHONY: clean

$(ONAME): $(OBJ)
	$(CC) $(C_SOURCE) $(H_SOURCE) -o $(ONAME) $(OBJ) -lcurl

clean:
	rm -rf merge

install:
	sudo mv merge /usr/bin