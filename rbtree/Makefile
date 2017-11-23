all:main test
main:main.cpp
	g++ -o $@ $^ -std=c++11 -g

test:test_performance.cpp
	g++ -o $@ $^ -std=c++11 -g

clean:
	rm -rf main test

.PHONY:clean

