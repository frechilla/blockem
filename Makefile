# this is a note to create the fancy ./configure && make && make install:
#     http://markuskimius.wikidot.com/programming:tut:autotools/
#         autoscan: configure.scan (mv configure.scan configure.ac)
#         autoconf: configure
#         autoheader: config.h.in
#
# package: tar -zcf blockem.20091130.tar.gz `find blockem/ -type f -not -iwholename "*.svn*"`
#
CC=g++
RM=rm -rf
TARGET=blockem
TEST_TARGET=blockem_test
# -g3: 
#     References to functions in the code
# -O0: 
#     No optimisation. Change to -O3 for release
# -fmessage-length=n
#     Try to format error messages so that they fit on lines of about n 
#     characters. The default is 72 characters for g++ and 0 for the rest 
#     of the front ends supported by GCC. If n is zero, then no line-wrapping 
#     will be done; each error message will appear on a single line. 
# -pg:
#     Add this flag to CCPARAM and LDPARAM to work with gprof
CCPARAM := -Wall -fmessage-length=0
#CCPARAM += -O0 -g3 -DDEBUG  # DEBUG
CCPARAM += -O3 -g3 -DNDEBUG # RELEASE
LDPARAM :=
INCLUDE_DIRS := -I"src/lib" `pkg-config gtkmm-2.4 libglademm-2.4 sigc++-2.0 --cflags`
EXTERNAL_LIBS := `pkg-config gtkmm-2.4 libglademm-2.4 sigc++-2.0 --libs`

# blockem library
SOURCE_DIRS_LIB = \
src/lib \

# blokem gui files
SOURCE_DIRS_GUI = \
src/gui

# test files
SOURCE_DIRS_TEST = \
src/lib/test

# Makes a list of the source (.cpp) files
SOURCE_LIB = $(foreach DIR,$(SOURCE_DIRS_LIB),$(wildcard $(DIR)/*.cpp))
SOURCE_GUI = $(foreach DIR,$(SOURCE_DIRS_GUI),$(wildcard $(DIR)/*.cpp))
SOURCE_TEST = $(foreach DIR,$(SOURCE_DIRS_TEST),$(wildcard $(DIR)/*.cpp))

# Makes a list of the .o (object) files.
OBJS_LIB = $(SOURCE_LIB:%.cpp=%.o)
OBJS_GUI = $(SOURCE_GUI:%.cpp=%.o)
OBJS_TEST = $(SOURCE_TEST:%.cpp=%.o)

# Makes a list of the .d (dependancy) files.
# Example to save objs in a different directory:
#     $(addprefix $(BIN_DIR)/,$(SOURCE:.cpp=.d))
DFILES_LIB = $(SOURCE_LIB:.cpp=.d)
DFILES_GUI = $(SOURCE_GUI:.cpp=.d)
DFILES_TEST = $(SOURCE_TEST:.cpp=.d)

# glade object. Created to create the binary with the glade properties embedded
GLADE_OBJ = src/gui/gui.glade.obj


all: $(TARGET) 

test: $(TEST_TARGET)

$(TARGET): $(OBJS_LIB) $(OBJS_GUI) $(GLADE_OBJ)
	@echo 'Building target: $@'
	$(CC) -o$@ $(OBJS_LIB) $(OBJS_GUI) $(GLADE_OBJ) $(EXTERNAL_LIBS) $(LDPARAM)
	@echo ' '

%.o: %.cpp
	@echo 'Building file: $<'
	$(CC) $(INCLUDE_DIRS) $(CCPARAM) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo ' '

$(GLADE_OBJ): $(GLADE_OBJ:%.obj=%)
	@echo 'turn the xml gui file into an object file to be embedded into the final binary'
	ld -r -b binary -o $@ $<
	objcopy --rename-section .data=.rodata,alloc,load,readonly,data,contents $@ $@
	@echo ' '

$(TEST_TARGET): $(OBJS_LIB) $(OBJS_TEST)
	@echo 'Building target: $@'
	$(CC) -o$@ $(OBJS_LIB) $(OBJS_TEST) $(LDPARAM)
	@echo ' '

package_src:
	cd package/ && ./do_package_src.sh && cd ../
	@echo ' '

clean:
	-$(RM) $(OBJS_LIB) $(DFILES_LIB)
	-$(RM) $(OBJS_GUI) $(DFILES_GUI) $(GLADE_OBJ)
	-$(RM) $(OBJS_TEST) $(DFILES_TEST)
	
clean_all: clean 
	-$(RM) $(TARGET) $(TEST_TARGET)

# Includes the .d files so it knows the exact dependencies for every source
-include $(DFILES_LIB)
-include $(DFILES_GUI)
-include $(DFILES_TEST)

