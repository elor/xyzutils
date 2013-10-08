OUT= xyzeval

OBJ+= obj/main.o
OBJ+= obj/Bond.o
OBJ+= obj/BondMap.o
OBJ+= obj/Type.o

OPT+= -fopenmp

BINDIR=/home/elor/usr/bin/
LIBDIR=/home/elor/usr/lib/
INCDIR=/home/elor/usr/include/

.PHONY: dirs clean install uninstall

obj/%.o: src/%.cpp
	c++ $(OPT) -I./include $(OPT) -I$(INCDIR) -g -c -O3 -Wall $(<:obj/=src/) -o $@

$(OUT): dirs $(OBJ)
	c++ $(OPT) -L$(LIBDIR) -g $(OBJ) -largumentparser -o $@

dirs:
	-mkdir obj

install:
	install $(OUT) $(BINDIR)
	
uninstall:
	rm $(BINDIR)/$(OUT)

clean:
	-rm -rfv obj/
	-rm -fv $(OUT)

all: $(OUT)
