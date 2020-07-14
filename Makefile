TARGET ?= myDaemon
DEBUG ?= 0
BUILDDIR := .build
SRC := main.c
SRC += daemonize.c logging.c
#SRC += dlfunc_cos.c
DEFINES := DEBUG=$(DEBUG)
CC = gcc
CFLAGS = -O2 -std=gnu18 -Wall -Wextra -pedantic
LDFLAGS :=

DEFINES := $(addprefix -D,$(DEFINES))

.PHONY: all clean tidy

all: $(TARGET)

$(TARGET): $(BUILDDIR)/$(TARGET)
	cp $< $@

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/$(TARGET): $(addprefix $(BUILDDIR)/,$(SRC:.c=.o))
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILDDIR)/%.o: %.c $(BUILDDIR)
	$(CC) $(CFLAGS) $(DEFINES) -c $< -o $@

# remove build artifacts
clean:
	@echo Cleaning up
	rm -f $(BUILDDIR)/$(SRC:.c=.o)

# remove build artifacts & target
tidy: clean
	rm -f $(BUILDDIR)/$(TARGET)
