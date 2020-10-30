EXECUTABLE = solver

SRCDIR = src
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

CXX = g++
BITS_OPTION = -m64
CXXFLAGS = -std=c++11 -O3 -fPIC -fexceptions -DNDEBUG -DIL_STD -g3
LDLIBS = -lm

$(EXECUTABLE): $(OBJECTS) 
	@echo  "\033[31m \nLinking all objects files: \033[0m"
	$(CXX) $(BITS_OPTION) $(OBJECTS) -o $@ $(LDLIBS)

-include $(OBJECTS:.o=.d)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo  "\033[31m \nCompiling $<: \033[0m"
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo  "\033[32m \nCreating $< dependency file: \033[0m"
	$(CXX) -std=c++11  -MM $< > $(basename $@).d
	@mv -f $(basename $@).d $(basename $@).d.tmp
	@sed -e 's|.*:|$(basename $@).o:|' < $(basename $@).d.tmp > $(basename $@).d
	@rm -f $(basename $@).d.tmp

clean:
	@echo "\033[31mCleaning obj directory... \033[0m"
	@rm $(EXECUTABLE) -f $(OBJDIR)/*.o $(OBJDIR)/*.d


rebuild: clean $(EXECUTABLE)

	
