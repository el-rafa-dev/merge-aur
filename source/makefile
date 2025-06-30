CC=g++
ONAME=merge #the output name
C_SOURCE= src/main.cc 
H_SOURCE= 
OBJ = $(SRC:.c=.o)

all: $(ONAME)

$(ONAME): $(OBJ)
	$(CC) $(C_SOURCE) $(H_SOURCE) -o $(ONAME) $(OBJ) -lcurl

install:
	sudo mv merge /usr/bin