PROJECT_NAME := Engine
SRC_DIR := src
INCLUDE_DIR := include
EXTERNAL_DIR := external
BUILD_DIR := build

GLFW_DIR := $(EXTERNAL_DIR)/glfw
GLFW_INCLUDE_DIR := $(GLFW_DIR)/include
GLFW_LIB_DIR := $(GLFW_DIR)/lib

CC := cc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion \
          -Wdouble-promotion -Wnull-dereference -Wformat=2 \
          -DGL_SILENCE_DEPRECATION \
          -I$(SRC_DIR) -I$(INCLUDE_DIR) -I$(EXTERNAL_DIR) -I$(GLFW_INCLUDE_DIR)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    CFLAGS  += -DGL_SILENCE_DEPRECATION
    LDFLAGS := -L$(GLFW_LIB_DIR) -lglfw3 -ldl \
               -framework OpenGL -framework Cocoa \
               -framework IOKit -framework CoreVideo
else
    # Linux
    LDFLAGS := -L$(GLFW_LIB_DIR) -lglfw -lGL -ldl -lm -lpthread
endif

SRCS := $(shell find $(SRC_DIR) $(EXTERNAL_DIR)/glad -name '*.c')
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

TEST_DIR := tests
TEST_SRCS := $(shell find $(TEST_DIR) -name '*.c')
TEST_OBJS := $(TEST_SRCS:%.c=$(BUILD_DIR)/%.o)
ENGINE_OBJS := $(filter-out $(BUILD_DIR)/$(SRC_DIR)/main.o, $(OBJS))

all: $(BUILD_DIR)/$(PROJECT_NAME)

$(BUILD_DIR)/$(PROJECT_NAME): $(OBJS)
	$(CC) $(OBJS) -o $(BUILD_DIR)/$(PROJECT_NAME) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/tests_runner: $(TEST_OBJS) $(ENGINE_OBJS)
	$(CC) $(TEST_OBJS) $(ENGINE_OBJS) -o $(BUILD_DIR)/tests_runner $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

exec: $(BUILD_DIR)/$(PROJECT_NAME)
	./$(BUILD_DIR)/$(PROJECT_NAME)

test: $(BUILD_DIR)/tests_runner
	./$(BUILD_DIR)/tests_runner

.PHONY: all clean exec test
