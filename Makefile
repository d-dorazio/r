.PHONY: card clean

r: main.cpp
	$(CXX) -o $@ -O3 -Wall -Wextra main.cpp

card: r
	./r > card.ppm

clean:
	rm r
