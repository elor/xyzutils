OBJ+= hist.o

OUT= hist

%.o: %.cpp
	c++ -fopenmp -g -c -O3 -Wall $(<:obj/=src/) -o $@

$(OUT): $(OBJ)
	c++ -fopenmp -g $(OBJ) -o $(OUT)

clean:
	-rm -fv $(OBJ) $(OUT) *.txt

