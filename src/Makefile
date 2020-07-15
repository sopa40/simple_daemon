#/*-----------------------------------------------------------------------
#A simple daemon project that monitors the system information and logs some information.
#Copyright (C) 2020  Nazar Sopiha

#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <https://www.gnu.org/licenses/>.
#------------------------------------------------------------------------*/

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
