EXE = pa3
OBJS_EXE = RGBAPixel.o lodepng.o PNG.o main.o QTree.o var.o

CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic 
LD = clang++
LDFLAGS = -std=c++1y -stdlib=libc++ -lpthread -lm 

all	: pa3

$(EXE) : $(OBJS_EXE)
	$(LD) $(OBJS_EXE) $(LDFLAGS) -o $(EXE)

#object files
RGBAPixel.o : cs221util/RGBAPixel.cpp cs221util/RGBAPixel.h
	$(CXX) $(CXXFLAGS) cs221util/RGBAPixel.cpp -o $@

PNG.o : cs221util/PNG.cpp cs221util/PNG.h cs221util/RGBAPixel.h cs221util/lodepng/lodepng.h
	$(CXX) $(CXXFLAGS) cs221util/PNG.cpp -o $@

lodepng.o : cs221util/lodepng/lodepng.cpp cs221util/lodepng/lodepng.h
	$(CXX) $(CXXFLAGS) cs221util/lodepng/lodepng.cpp -o $@

var.o : var.h var.cpp cs221util/RGBAPixel.h cs221util/PNG.h
	$(CXX) $(CXXFLAGS) var.cpp -o $@

QTree.o : QTree.h QTree.cpp var.h cs221util/PNG.h cs221util/RGBAPixel.h
	$(CXX) $(CXXFLAGS) QTree.cpp -o $@

main.o : main.cpp cs221util/PNG.h cs221util/RGBAPixel.h QTree.h var.h
	$(CXX) $(CXXFLAGS) main.cpp -o main.o

clean :
	-rm -f *.o $(EXE)
