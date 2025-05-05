

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
 
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
 
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = BikeGame
 
# Create obj directory if it doesn't exist
$(shell mkdir -p $(OBJ_DIR))
 
all: $(TARGET)
 
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)
 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@
 
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
 
.PHONY: all clean