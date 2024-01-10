TARGET=ring_queue_test

#编译中间生成文件路径
OBJ_DIR=build/obj
#编译输出文件路径
OUT_DIR=build/out

#设置创建文件夹指命令
MKDIR=mkdir -p
#设置删除命令
RM=rm -rf

#0:Release
#1:Debug
DEBUG=1

#设置编译器
CC=gcc

#优化等级
ifeq ($(DEBUG), 0)
	OPT=-O3
else
	OPT=-O0 -g
endif

#C编译参数
C_FLAGS=$(C_STD) $(OPT) -Wall -MMD -MP -fdiagnostics-color=always

#C全局宏定义
C_DEFINES=
C_DEFINE_FLAGS:=$(addprefix -D, $(C_DEFINES))

#头文件路径
INCLUDE_DIRS=\
	test \
	ring_queue \

INCLUDE_FLAGS=$(addprefix -I, $(INCLUDE_DIRS))

#源文件
C_SRCS=\
	main.c \
	test/ring_queue_test.c \
	ring_queue/ring_queue.c \
	
OBJS=$(addprefix $(OBJ_DIR)/, $(notdir $(C_SRCS:.c=.o)))

vpath %.c $(dir $(C_SRCS))

all:$(OUT_DIR)/$(TARGET)

$(OUT_DIR)/$(TARGET):$(OBJS) Makefile | $(OUT_DIR)
	$(CC) $(OBJS) -o $@

$(OBJ_DIR)/%.o:%.c Makefile | $(OBJ_DIR)
	$(CC) -c $< $(C_DEFINE_FLAGS) $(INCLUDE_FLAGS) $(C_FLAGS) -o $@

$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)

$(OUT_DIR):
	$(MKDIR) $(OUT_DIR)

-include $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.d)))

.PHONY:clean print

clean:
	$(RM) $(OBJ_DIR)
	$(RM) $(OUT_DIR)

print:
	@echo C_SRCS = $(C_SRCS)
	@echo OBJS = $(OBJS)
	@echo INCLUDE_DIRS = $(INCLUDE_DIRS)
	@echo INCLUDE_FLAGS = $(INCLUDE_FLAGS)