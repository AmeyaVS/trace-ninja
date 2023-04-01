TARGET_EXEC ?= dlopen-example

CC := gcc
CXX := g++
AS := as

BUILD_DIR ?= build
SRC_DIRS ?=  src
INC_DIRS :=  include

SRCS_C := $(SRC_DIRS)/dlopen-example.c $(SRC_DIRS)/fibo.c $(SRC_DIRS)/fact.c
SRCS_CXX :=
OBJS := $(SRCS_C:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS ?= -fPIC -Wall -Wextra
CXXFLAGS ?= -fPIC -Wall -Wextra
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

all: $(BUILD_DIR)/$(TARGET_EXEC)\
     $(BUILD_DIR)/libfibo.so\
     $(BUILD_DIR)/libfact.so
	./$(BUILD_DIR)/$(TARGET_EXEC)

# Assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# C Source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# C++ Source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -rfv $(BUILD_DIR)/$(TARGET_EXEC) $(BUILD_DIR)/libfibo.so $(BUILD_DIR)/libfact.so $(OBJS) $(DEPS) map.*.log

-include $(DEPS)

MKDIR_P ?= mkdir -p

$(BUILD_DIR)/$(TARGET_EXEC): $(BUILD_DIR)/src/dlopen-example.c.o
	$(CC) $< -o $@ $(LDFLAGS) -ldl

$(BUILD_DIR)/libfibo.so: $(BUILD_DIR)/src/fibo.c.o
	$(CC) -shared -o $@ $<

$(BUILD_DIR)/libfact.so: $(BUILD_DIR)/src/fact.c.o
	$(CC) -shared -o $@ $<


