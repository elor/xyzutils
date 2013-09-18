OBJ+= splitxyz.o

OUT= splitxyz

%.o: %.cpp
	c++ -fopenmp -I/home/elor/usr/include -g -c -O3 -Wall $(<:obj/=src/) -o $@

all: $(OUT)

$(OUT): $(OBJ)
	c++ -fopenmp -g $(OBJ) -L/home/elor/usr/lib -largumentparser -o $(OUT)

clean:
	-rm -fv $(OBJ) $(OUT) *.txt
