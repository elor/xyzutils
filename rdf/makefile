OBJ+= main.o

OUT= run

%.o: %.cpp
	c++ -g -c -O3 -Wall $(<:obj/=src/) -o $@

$(OUT): $(OBJ)
	c++ -g $(OBJ) -o $(OUT)

clean:
	-rm -fv $(OBJ) $(OUT) *.txt

