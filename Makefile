CPPFLAGS += -g

main: main.o fraction.o
	g++ $(CPPFLAGS) -o $@ $+
