CC = gcc
CXX = g++

FLAGS = -std=c++11 -O3

ROOT_DIR = .
TARGET = $(ROOT_DIR)/a.out

# BOOST
BOOST_ROOT = C:/boost_1_72_0/
BOOST_INC = $(BOOST_ROOT)

SRC_DIR = $(ROOT_DIR)/src
INC_DIR = $(ROOT_DIR)/src
SRC := $(foreach dir, $(SRC_DIR), $(filter-out %main.cpp, $(wildcard $(dir)/*.cpp)))
SRC_CC := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cc))
OBJ := $(patsubst %.cpp,%.o, $(SRC))
OBJ += $(patsubst %.cc, %.o, $(SRC_CC))

SVR_OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(OBJ))

INC_PATHS = $(INC_DIR) $(BOOST_INC)
LIB_PATHS = 

.PHONY: all

all: $(TARGET)

$(TARGET): $(SVR_OBJS)
	$(CXX) $(FLAGS) $(SVR_OBJS) -o $(TARGET) $(addprefix -I, $(INC_PATHS))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo -e "\033[1;33m\nCompiling $< ==> $@\033[0m"
	$(CC) $(FLAGS) -c -o $@ $(addprefix -I, $(INC_PATHS)) $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo -e "\033[1;33m\nCompiling $< ==> $@\033[0m"
	$(CXX) $(FLAGS) -c -o $@ $(addprefix -I, $(INC_PATHS)) $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@echo -e "\033[1;33m\nCompiling $< ==> $@\033[0m"
	$(CXX) $(FLAGS) -c -o $@ $(addprefix -I, $(INC_PATHS)) $<

clean:
	rm -f $(SVR_OBJS) $(TARGET)