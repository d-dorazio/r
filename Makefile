.PHONY: card clean

r: main.cpp minimize.py
	./minimize.py
	$(CXX) -o $@ -O3 -Wall -Wextra r.cpp

card: r
	./r > card.ppm

clean:
	rm r
