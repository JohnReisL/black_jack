PRJNAME=test

#################

COMPCXX=g++

CXXFLAGS=-ggdb3 -Wall 

CINCLUDES=

CXXSTD=-std=c++17

#################

SRC_PATH=src

OBJ_PATH=obj

INCLUDED_FILE=$(SRC_PATH)/blackjack.h

BUILD_PATH=build

CCSRC=main.cc

CCSRCS=$(addprefix $(SRC_PATH)/,$(CCSRC))

CCOBJS=$(addprefix $(OBJ_PATH)/,$(notdir $(CCSRCS:.cc=.o)))

TARGETS=$(addprefix $(BUILD_PATH)/$(PRJNAME)_,$(notdir $(CCSRCS:.cc=)))

################

all: $(TARGETS)

run: $(TARGETS)
	@for i in $(TARGETS); do \
		echo "Запускаем" $$i ; \
		echo; \
		./$$i; \
		echo; \
	done


$(BUILD_PATH)/$(PRJNAME)_%: $(OBJ_PATH)/%.o
	@if [ ! -d $(BUILD_PATH) ]; then mkdir $(BUILD_PATH); fi
	$(COMPCXX) $(LDLIBS) -o $@ $<

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cc
	@if [ ! -d $(OBJ_PATH) ]; then mkdir $(OBJ_PATH); fi
	$(COMPCXX) $(CXXFLAGS) $(CINCLUDES) $(CXXSTD) -c $< -o $@

####################

.PHONY: clean

.PRECIOUS: $(CCOBJS)

clean:
	@rm -rvf $(OBJ_PATH) $(BUILD_PATH) .*~ *~ \#*\# .d **/*~ **/*#
