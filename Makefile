# SPDX-License-Identifier: GPL-2.0
# Author: xunicatt
# Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

NAME    := mono
VERSION := 0
PATCH   := 1

TARGET := $(NAME)-v$(VERSION).$(PATCH).ihx
BUILD  := build

CC    := sdcc
AS    := sdasstm8
FLASH := stm8flash
DCE   := stm8dce

CSTD    := --std=c99
CFLAGS  ?=
CFLAGS  += --Werror
CFLAGS  += -mstm8 --opt-code-size
CFLAGS  += $(CSTD) -I.
CFLAGS  += --disable-warning 126 # unreachable code
CFLAGS  += --disable-warning 190 # empty translation unit
LDFLAGS ?=
LDFLAGS += --out-fmt-ihx

VERBOSE ?= no
Q = @

ifeq ($(VERBOSE),yes)
	Q =
endif

include core/core.mk
include drivers/drivers.mk
include init/init.mk

ASMDIR := $(BUILD)/asm
DCEDIR := $(BUILD)/dce
OBJDIR := $(BUILD)/obj

CFILES := $(SRCS)
ASMFILES := $(patsubst %.c, $(ASMDIR)/%.asm, $(CFILES))
OBJFILES := $(patsubst %.c, $(OBJDIR)/%.rel, $(notdir $(CFILES)))

.PHONY: all clean

all: $(BUILD)/$(TARGET)

# Stage 1: Compiles (C -> ASM)
$(ASMDIR)/%.asm: %.c
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -S -o $@ $<
	@echo "CC  : $@"

# Stage 2: Removes dead codes (ASMs -> ASMs)
dce: $(ASMFILES)
	@echo "DCE : Optimizing"
	$(Q)mkdir -p $(DCEDIR)
	$(Q)$(DCE) -o $(DCEDIR) $^

# Stage 3: Assembly to Object file (ASM -> REL)
$(OBJDIR)/%.rel: $(DCEDIR)/%.asm
	$(Q)mkdir -p $(OBJDIR)
	$(Q)$(AS) -plosgffw $@ $<
	@echo "ASM  : $@"

# Stage 4: Final firmware generation (RELs -> IHX)
$(BUILD)/$(TARGET): dce $(OBJFILES)
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJFILES)
	@echo "LD  : $@"

clean:
	$(Q)rm -rf $(BUILD)

flash: $(BUILD)/$(TARGET)
	$(Q)$(FLASH) -c stlinkv2 -p stm8s103?3 -w $<
