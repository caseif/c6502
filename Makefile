rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

LD = $(CC)

TESTDIR = test

SRCDIR = src
INCDIR = include
TEST_SRCDIR = $(TESTDIR)/src
TEST_INCDIR = $(TESTDIR)/include

OUTDIR = build
LIBNAME = libc6502.a

INSTALLINCDIR = /usr/include/
INSTALLLIBDIR = /usr/lib/

TEST_BINNAME = c6502_test

SRCEXT = c
HEXT   = h
OBJEXT = o

INCLUDES = ./include
TEST_INCLUDES = ./test/include
LIBS = c
CFLAGS := -g -Wall -std=c11

INCFLAGS := $(foreach d, $(INCLUDES), -I$d)
TEST_INCFLAGS := $(foreach d, $(TEST_INCLUDES), -I$d)
LIBFLAGS := $(foreach d, $(LIBS), -l$d)
CFLAGS += $(INCFLAGS)
TEST_CFLAGS := $(TEST_INCFLAGS) $(CFLAGS)
LDFLAGS := $(LIBFLAGS)

SRCFILES = $(call rwildcard, $(SRCDIR), *.$(SRCEXT))
HFILES = $(call rwildcard, $(INCDIR), *.$(HEXT))
TEST_SRCFILES = $(call rwildcard, $(TEST_SRCDIR), *.$(SRCEXT))
TEST_HFILES = $(call rwildcard, $(TEST_INCDIR), *.$(HEXT))

OBJFILES = $(patsubst %, $(OUTDIR)/%, $(patsubst $(SRCDIR)/%.$(SRCEXT), %.$(OBJEXT), $(SRCFILES)))

TEST_OBJFILES := $(patsubst %, $(OUTDIR)/$(TESTDIR)/%, $(patsubst $(TEST_SRCDIR)/%.$(SRCEXT), %.$(OBJEXT), $(TEST_SRCFILES)))
TEST_OBJFILES += $(filter-out build/main.o,$(OBJFILES))

MKDIR_P = @mkdir -p

.PHONY: apply_license_headers clean all test

all: apply_license_headers $(OUTDIR)/$(LIBNAME)

test: $(OUTDIR)/$(TEST_BINNAME)
	./build/$(TEST_BINNAME)

apply_license_headers:
	./scripts/apply_license_headers.sh

$(OUTDIR)/$(LIBNAME): $(OBJFILES)
	$(AR) rs $@ $^

$(OUTDIR)/$(TEST_BINNAME): $(TEST_OBJFILES)
	$(LD) -o $(OUTDIR)/$(TEST_BINNAME) $(TEST_OBJFILES) $(LDFLAGS)

.SECONDEXPANSION:
$(OUTDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	$(MKDIR_P) $(@D)
	$(CC) $(CFLAGS) -MD -c -o $@ $<
	@cp $(OUTDIR)/$*.d $(OUTDIR)/$*.P; \
			sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
			-e '/^$$/ d' -e 's/$$/ :/' < $(OUTDIR)/$*.d >> $(OUTDIR)/$*.P; \
			rm -f $(OUTDIR)/$*.d

.SECONDEXPANSION:
$(OUTDIR)/$(TESTDIR)/%.$(OBJEXT): $(TEST_SRCDIR)/%.$(SRCEXT)
	$(MKDIR_P) $(@D)
	$(CC) $(TEST_CFLAGS) -MD -c -o $@ $<
	@cp $(OUTDIR)/$(TESTDIR)/$*.d $(OUTDIR)/$(TESTDIR)/$*.P; \
			sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
			-e '/^$$/ d' -e 's/$$/ :/' < $(OUTDIR)/$(TESTDIR)/$*.d >> $(OUTDIR)/$(TESTDIR)/$*.P; \
			rm -f $(OUTDIR)/$(TESTDIR)/$*.d

-include *.P

install: $(OUTDIR)/$(LIBNAME)
	@if ! [ "$(shell id -u)" = 0 ]; then echo "This target must be run as root." ; exit 1 ; fi
	cp -pr $(INCDIR)/* $(INSTALLINCDIR)
	cp -p $(OUTDIR)/$(LIBNAME) $(INSTALLLIBDIR)

clean:
	rm -rf $(OUTDIR)

${OUTDIR}:
	${MKDIR_P} ${OUTDIR}
