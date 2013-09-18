OBJ+= splitxyz.o

OUT= splitxyz

%.o: %.cpp
	c++ -fopenmp -g -c -O3 -Wall $(<:obj/=src/) -o $@

all: $(OUT)

$(OUT): $(OBJ)
	c++ -fopenmp -g $(OBJ) -o $(OUT)

clean:
	-rm -fv $(OBJ) $(OUT) *.txt
