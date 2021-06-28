SDK_DIR := ../

BUILD := build

BIN := thermal-raw

SRC_DIR := .

SOURCES := $(SRC_DIR)/main.cpp \
			$(SRC_DIR)/SinkVideo.cpp \
			$(SRC_DIR)/SinkAudio.cpp \
			$(SRC_DIR)/AuthorizationHandler.cpp \
			$(SRC_DIR)/NotificationListener.cpp

OBJS := $(addprefix $(BUILD)/, $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(patsubst sample/%,%,$(SOURCES)))))

include ../toolchain.cfg

INCLUDE_DIR := $(SDK_DIR)/include

LIB_DIR := $(SDK_DIR)/lib/$(OS)
LIB_EV_CLIENT := eventstreamclient

CXXFLAGS += -I$(INCLUDE_DIR) -std=c++11 -Wall -ggdb 

all: $(BIN)

clean:
	rm -rf $(BUILD) 

$(BIN): $(OBJS)
	@echo '##Building file: $<'
	$(CXX) $(CXXFLAGS) -o $(BUILD)/$(BIN) $(OBJS) -L$(LIB_DIR) -l$(LIB_EV_CLIENT)
	@echo '##Finished building: $<'

$(BUILD):
	@(if [ ! -d $(BUILD) ]; then \
		mkdir -p $(BUILD); \
	fi)

$(BUILD)/%.o: $(SRC_DIR)/%.cpp | $(BUILD)
	@echo '##Building file: $<'
	@echo '##Invoking: GCC C++ Compiler'
	$(CXX) $(CXXFLAGS) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo '##Finished building: $<'
	@echo ' '	
