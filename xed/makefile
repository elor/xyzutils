OBJ+= main.o convert.o

OUT= xed

%.o: %.cpp
	c++ $(OPT) -I. -I/home/elor/usr/include -g -c -O3 -Wall $(<:obj/=src/) -o $@

$(OUT): $(OBJ)
	c++ $(OPT) -L/home/elor/usr/lib $(LIB) -g $(OBJ) -o $(OUT)

clean:
	-rm -fv $(OBJ) $(OUT) *.txt test.xyz

