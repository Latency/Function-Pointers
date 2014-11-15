################################################################################
# All rights reserved (R)    Bio-Hazard Industries  (c) 1998-2014  by Latency  #
################################################################################

##############################################################################
# Compiler specifications
# These match the variable names given in /usr/share/lib/make/make.rules
# so that make's generic rules work to compile our files.
# gmake prefers CXX and CXXFLAGS for c++ programs
##############################################################################
# Which compiler should be used (leave commented for default)
#CXX = g++
#CC  = gcc

# What flags should be passed to the compiler
DEBUG_LEVEL     = -g
EXTRA_CCFLAGS   = -Wall -std=c99

# Uncomment for additional testing features
DEBUG = yes

# Output file name (executable)
OUTPUT_FILE = Function-Pointers.out

# Dependancy file name
DEPFILE     = dependancies

# Uncomment for relative directory naming convention
#RELATIVE = yes

# Directories
TOPDIR      = $(shell if [ -n "$(PWD)" ]; then echo $(PWD); else pwd; fi)
define variable-def
  ifndef $1
#   $$(warning $1 is undefined)
    RELEASEDIR  = $(TOPDIR)/Release
    DEBUGDIR    = $(TOPDIR)/Debug
    OBJDIR      = $(TOPDIR)/Objects
    SRCDIR      = $(TOPDIR)/src
  else
#   $$(warning $1 is defined)
    RELEASEDIR  = Release
    DEBUGDIR    = Debug
    OBJDIR      = Objects
    SRCDIR      = src
  endif
endef
DEPDIR = $(OBJDIR)/Dependancies

VERSION = 1
PATCHLEVEL = 2

# Any libraries that might need to be supported..   e.g. -lpthread
LIBS =

# Any library paths that need to be included.   e.g. -L<path>
LDFLAGS =

##############################################################################
# Do Not Modify Anything Below This Line (unless you know what you're doing) #
##############################################################################

CXXFLAGS   = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)
CCFLAGS    = $(CXXFLAGS)

COMPILE.cc = $(CXX) -c $(CXXFLAGS) $(INCLUDES) $(TARGET_ARCH)
COMPILE.c  = $(CC)  -c $(CCFLAGS) $(INCLUDES) $(TARGET_ARCH)
LINK.o     = $(CC) $(LDFLAGS) $(TARGET_ARCH)
OUTPUT_OPTION = -o $@

SUBPATCHLEVEL = $(shell date +%y%m%d)

export DEBUG TOPDIR VERSION PATCHLEVEL
$(eval $(call variable-def,RELATIVE))

DEFS      = -DBINDIR="$(BINDIR)" -DVERSION="v$(VERSION).$(PATCHLEVEL).$(SUBPATCHLEVEL)"
ifdef DEBUG
  BINDIR   = $(DEBUGDIR)
  DEFS   += -D_DEBUG
else
  BINDIR   = $(RELEASEDIR)
  CFLAGS += -O3
endif

#############################################################################
#                        Directory path associations                        #
#############################################################################
# w/ string token delim @ \n
VPATH := $(shell find '$(SRCDIR)/' -type d -exec echo \\"\"{}\\"\" \;)
VPATH := $(subst /\",\",$(VPATH))	# Unifiy all tokens
VPATH_:= $(shell echo -e "$(VPATH)" | sed 's/\([^"]\) /\1\\ /g')

#############################################################################
#                             Build files                                   #
#############################################################################
# Replace space within tokens w/ ?
TMP := $(shell echo -e "$(VPATH)" | sed 's/\([^"]\) /\1?/g' )
SRCFILES := $(foreach d,$(TMP),$(shell find "$d/" -maxdepth 1 -type f -name \
			"*.c" -exec echo \\"\"{}\\"\" \; ))
SRCFILES_:= $(shell echo -e "$(SRCFILES)" | sed 's/\([^"]\) /\1\\ /g' | \
			 tr -d '"' ) # Replace space within tokens w/ '\ '

HDRFILES := $(foreach d,$(TMP),$(shell find "$d/" -maxdepth 1 -type f -name \
			"*.h" -exec echo \\"\"{}\\"\" \; ))

OBJFILES := $(SRCFILES:.c\"=.o\")
# Replace space within tokens w/ ?
TMP := $(shell echo -e "$(OBJFILES)" | sed 's/\([^"]\) /\1?/g' )
TMP := $(foreach d,$(TMP),\\\""$(OBJDIR)"/$(notdir "$d")\\\")
OBJFILES := $(shell echo -e "$(TMP)" | sed 's/?/\\ /g')
OBJFILES_:= $(shell echo -e "$(OBJFILES)" | sed 's/\([^"]\) /\1\\ /g' | \
			 tr -d '"' ) # Replace space within tokens w/ '\ '

OBJSMADE := $(shell [ -d "$(OBJDIR)" ] && find "$(OBJDIR)/" \
			 -maxdepth 1 -type f -name "*.o" -exec echo \\"\"{}\\"\" \; )
OBJSMADE_:= $(shell echo -e "$(OBJSMADE)" | \
			 sed 's/\([^"]\) /\1\\ /g' | tr -d '"' )

#############################################################################
# Auto-dependancy generation files
#############################################################################
DEPFILES := $(SRCFILES:.c\"=.P\")
# Replace space within tokens w/ ?
TMP := $(shell echo -e "$(DEPFILES)" | sed 's/\([^"]\) /\1?/g' )
TMP := $(foreach d,$(TMP),\\\""$(DEPDIR)"/$(notdir "$d")\\\")
DEPFILES := $(shell echo -e "$(TMP)" | sed 's/?/\\ /g')
DEPFILES_:= $(shell echo -e "$(DEPFILES)" | sed 's/\([^"]\) /\1\\ /g' | \
			 tr -d '"' ) # Replace space within tokens w/ '\ '
-include $(DEPFILES_)

#############################################################################
#                               Backup files                                #
#############################################################################
# Add additional paths here to [find . '$(BINDIR)/'] etc.
TMP := $(shell find . '$(BINDIR)/' -maxdepth 0 -type d -exec \
		echo \\"\"{}\\"\" \; 2>/dev/null) # w/ string token delim @ \n
TMP := $(subst /\",\","$(TMP)")	# Unifiy all tokens
# Replace space within tokens w/ ?
TMP := $(shell echo -e "$(TMP)" | sed 's/\([^"]\) /\1?/g' )
BACKUPFILES := $(foreach d,$(TMP),$(shell find "$d/" -maxdepth 1 -type f \
				-name "*~" -exec echo \\"\"{}\\"\" \; ))
# Replace space within tokens w/ ?
BACKUPFILES_:= $(shell echo -e "$(BACKUPFILES)" | sed 's/\([^"]\) /\1?/g' )

#############################################################################
#                          Include directories                              #
#############################################################################
# Replace space within tokens w/ ?
TMP := $(shell echo -e "$(VPATH)" | sed 's/\([^"]\) /\1?/g' )
TMP := $(foreach d,$(TMP),-I\"$d\")
INCLUDES := $(shell echo $(TMP) | sed 's/?/ /g')


define create_directory
  if  [ ! -d "$(1)" ]; then \
	echo "Creating directory for \`$(1)'" ; \
    mkdir -p "$(1)" ; \
   fi
endef

define remove_folder
  if  [ -d "$(1)" ]; then \
    echo "Removing folder \`$(1)'" ; \
    $(RM) -rf "$(1)" ; \
  fi
endef

define create_backup
  if [ -d "$(1)" -a -f "$(2)" ]; then \
    echo "Creating backup \`$(2)' -> \`$(2)~'" ; \
    mv -f "$(2)" "$(2)~" ; \
  fi
endef

define path_parse
  # Equivilent to $(*F) | $(*D) but to include spaces within file/dir names
  $(eval TMP := $(shell echo $(1) | sed 's/\([^"]\) /\1?/g' )) 
  $(eval TMPD:= $(dir $(TMP)))
  $(eval TMPD:= $(shell echo $(TMPD) | sed 's/?/\\ /g' ))
  $(eval TMPF:= $(notdir $(TMP)))
  $(eval TMPF:= $(shell echo $(TMPF) | sed 's/?/\\ /g' ))
endef

define define_compile_rules
OBJDIR_:= $(shell echo -e "$(OBJDIR)" | sed 's/\([^"]\) /\1\\ /g')
    ######################################################
    # Compile and generate dependency info;
    ######################################################
    #   sed:    strip leading spaces
    #   sed:    remove comments
    #   sed:    fill space seperator '?' for directories/files (if applicable)
    #   sed:    strip the target (everything up to and incl. the colon)
    #   sed:    remove any continuation backslashes
    #   sed:    swallow any lines with a colon
    #   fmt -1: list filenames one per line
    #   sed:    add trailing colons
    #   sed:    restore fill space seperator
    #   redirect out to new file (*.P)rocessed
    #   remove (*.d)ependancy file
$(OBJDIR_)/%.o: $(1)/%.c
	@echo "Compiling \`$<'"
	@$(call path_parse,"$<")
	@echo " Creating \`$(DEPDIR)/$(TMPF:.c=.d)'"
	@$(COMPILE.c) -MMD -MF "$(DEPDIR)/$(TMPF:.c=.d)" $(INCLUDES) \
						 $(DEFS) "$<" -o "$(OBJDIR)/$(TMPF:.c=.o)"
	@# Dynamically generate build rules to rebuild when touched
	@# See Chapter 4 of the 'make' manual, --
	@#  "Rules without Commands or Prerequisites"
	@sed -e 's/^ *//' -e 's/#.*//' -e 's/\ /?/g' -e 's/^.*:[ ]*//' \
	  -e 's/[^ ]\\$$//' -e '/^\s*$$/d' < "$(DEPDIR)/$(TMPF:.c=.d)" | \
	  fmt -1 | sed -e 's/$$/:/' -e 's/?/ /g' >> "$(DEPDIR)/$(TMPF:.c=.P)" >> \
	  "$(DEPDIR)/$(TMPF:.c=.d)" ; \
	  mv -f "$(DEPDIR)/$(TMPF:.c=.d)" "$(DEPDIR)/$(TMPF:.c=.P)"
endef

$(foreach directory,$(sort $(dir "$(VPATH_)")),$(eval $(call define_compile_rules,$(directory))))

#############################################################################
#                               Build Rules                                 #
#############################################################################

%.P : %.c
	@echo "Testing"
	@$(call makedepend,$*,$(DEPDIR))
	@sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' < $*.d > $@; \
	 $(RM) $*.d; [ -s $@ ] || $(RM) $@



#############################################################################
#                           Build Targets                                   #
#############################################################################

PHONY    = init_obj init_bin init_dep init_clean
PHONY   += clean test help all
ifdef DEBUG
  PHONY += touch help_touch depend
endif
.PHONY: $(PHONY)


## Default Rule (Build Output File) ##
OTMP := $(shell echo "$(BINDIR)/$(OUTPUT_FILE)" | sed 's/\([^"]\) /\1\\ /g' )
$(OTMP): init_obj init_dep $(OBJFILES_) init_bin
	@$(call create_backup,$(BINDIR),$@)
	@echo "  Linking \`$<'"
	@$(LINK.o) $(LDLIBS) $(OBJSMADE_) -o "$@"


## ALL ##
all: $(OTMP)


init_obj:
	@$(call create_directory,$(OBJDIR))


init_bin:
	@$(call create_directory,$(BINDIR))


init_dep:
	@$(call create_directory,$(DEPDIR))


init_clean:
	@$(call remove_folder,$(OBJDIR))
	@for i in $(BACKUPFILES_) ; do \
	  if [ -f $$i ]; then \
	    echo "Removing backup file \`$$i'" ; \
		$(RM) $$i ; \
	  fi ; \
	 done
	@for i in . "$(BINDIR)" ; do \
	  if [ -f "$$i/core" ]; then \
	    echo "Removing core dump found in folder \`$$i'" ; \
	    $(RM) "$$i/core" ; \
	  fi ; \
 	 done
	@if  [ -d "$(BINDIR)" ]; then \
	  if [ -f "$(BINDIR)/$(OUTPUT_FILE)" ]; then \
	    echo "Removing output file for \`$(BINDIR)/$(OUTPUT_FILE)'" ; \
 	    $(RM) "$(BINDIR)/$(OUTPUT_FILE)" ; \
	  fi ; \
	  if [ -f "$(BINDIR)/$(OUTPUT_FILE)~" ]; then \
	    echo "Removing backup output file for \`$(BINDIR)/$(OUTPUT_FILE)~'" ; \
 	    $(RM) "$(BINDIR)/$(OUTPUT_FILE)~" ; \
	  fi ; \
	 fi
	@echo "Cleanup finished."


## TEST ##
test:
	@echo ---------------- Makefile settings ------------------
	@echo TMP=$(TMP)
	@echo TOPDIR=$(TOPDIR)
	@echo VPATH=$(VPATH)
	@echo VPATH_=$(VPATH_)
	@echo OUTPUT_FILE=$(OUTPUT_FILE)
	@echo RELEASEDIR=$(RELEASEDIR)
	@echo DEBUGDIR=$(DEBUGDIR)
	@echo SRCDIR=$(SRCDIR)
	@echo OBJDIR=$(OBJDIR)
	@echo BINDIR=$(BINDIR)
	@echo SRCFILES=$(SRCFILES)
	@echo SRCFILES_=$(SRCFILES_)
	@echo OBJFILES=$(OBJFILES)
	@echo OBJFILES_=$(OBJFILES_)
	@echo OBJSMADE=$(OBJSMADE)
	@echo OBJSMADE_=$(OBJSMADE_)
	@echo HDRFILES=$(HDRFILES)
	@echo DEPFILES=$(DEPFILES)
	@echo DEPFILES_=$(DEPFILES_)
	@echo BACKUPFILES=$(BACKUPFILES)
	@echo OBJS=$(OBJS)
	@echo INCLUDES=$(INCLUDES)
	@echo DEFS=[ $(DEFS) ]
	@echo CFLAGS=[ $(CFLAGS) ]
	@echo LIBS=[ $(LIBS) ]
	@echo VERSION=\"v$(VERSION).$(PATCHLEVEL).$(SUBPATCHLEVEL)\"
	@echo -----------------------------------------------------


##############################################
#    ----- SPECIFIC RELEASE RULES ----       #
##############################################
ifdef DEBUG
##############################################
# Debug Section
#############################################

## DEPEND ##
DTMP := $(shell echo "$(DEPDIR)/$(DEPFILE)" | sed 's/\([^"]\) /\1\\ /g' )
depend $(DTMP): init_dep
	@if ["$@" != "$(DEPDIR)/$(DEPFILE)" -o ! -e "$(DEPDIR)/$(DEPFILE)"]; then \
	  echo "Generating source code dependency information."; \
	  $(CC) -MM $(SRCFILES_) $(INCLUDES) > "$(DEPDIR)/$(DEPFILE)" ; \
	fi


## TOUCH ##
touch: init_bin init_obj
	@echo "==> Creating backup files"
	touch "./zzz~"
	touch "$(BINDIR)/asdf~"
	touch "$(BINDIR)/dewr~"
	@echo "==> Creating core files"
	touch "./core"
	touch "$(BINDIR)/core"
	@echo "==> Creating output files"
	touch "$(BINDIR)/$(OUTPUT_FILE)"
	touch "$(BINDIR)/$(OUTPUT_FILE)~"
	@echo "==> Creating objects *.o, .depends, etc."
	touch "$(OBJDIR)/asdf.o"


## CLEAN (chain rule for Debug) ##
clean: init_clean
	@$(call remove_folder,$(BINDIR))








##############################################
# End of Debug Section
#############################################
## HELP (Private) ##
help_touch:
	@echo -e \
	"touch          - Creates files to test against clean rule."

## HELP (chain rule for Debug) ##
help: help_touch
else
##############################################
# Release Section
#############################################
## CLEAN (chain rule for Release) ##
clean: init_clean







##############################################
# End of Release Section
##############################################
## DO NOT ADD ANY NEW RULES BELOW THIS LINE ##
##############################################
## HELP ##
help:
endif
	@echo -e \
	"print-settings - Displays all the variables.\n"\
	"clean          - Purge all object files and executables.\n"\
	"depend         - Generates dependancies for pre-compiled headers.\n"\
	"help           - Shows this screen.\n"

