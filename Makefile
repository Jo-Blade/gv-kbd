#########################
# customise these
CFILES := main.c input.c config.c graphics.c 2d-engine.c free-selection-keyboard.c
PROG := prog
CFLAGS := -Wall -Wextra -g
LDFLAGS := -lSDL2
########################

# -MMD generates dependencies while compiling
CFLAGS += -MMD
CC := gcc

OBJFILES := $(CFILES:.c=.o)
DEPFILES := $(CFILES:.c=.d)

$(PROG) : $(OBJFILES)
	$(LINK.o) -o $@ $^ $(LDFLAGS)

clean :
	rm -f $(PROG) $(OBJFILES) $(DEPFILES)

-include $(DEPFILES)
