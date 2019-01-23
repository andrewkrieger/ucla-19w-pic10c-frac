CPPFLAGS += -g

main: main.o fraction.o
	g++ $(CPPFLAGS) -o $@ $+

main.o: main.cpp fraction.h boost_hash_combine.h
