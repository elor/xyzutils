OBJ+= obj/main.o
OBJ+= obj/convert.o
OBJ+= obj/Atom.o
OBJ+= obj/AtomSet.o
OBJ+= obj/Command.o
OBJ+= obj/Selector.o

OUT= xed

obj/%.o: src/%.cpp
	c++ $(OPT) -I./include -I/home/elor/usr/include -g -c -O3 -Wall $(<:obj/=src/) -o $@

$(OUT): dirs $(OBJ)
	c++ $(OPT) -g $(OBJ) -L/home/elor/usr/lib $(LIB) -o $(OUT)

dirs:
	-mkdir obj
clean:
	-rm -fv $(OBJ) $(OUT)

all:
	@echo stub