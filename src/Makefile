TARGET ?= simple_daemon
BUILDDIR := .build
SRC := main.c
SRC += logging.c daemonize.c writers.c
#DEFINES := LOG_SYSLOG
CC = gcc
CFLAGS = -O2 -Wall -Wextra
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
