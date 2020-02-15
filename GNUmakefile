#
################################################################################
all ::
clean ::
.PHONY : all clean
################################################################################
include settings.mk
################################################################################
HOSTCC ?= $(CC)
HOSTLD ?= $(LD)
HOSTAS ?= $(AS)
################################################################################
ARFLAGS = rvU
GLOBAL_CFLAGS = $(CFLAGS)
GLOBAL_CPPFLAGS = $(CPPFLAGS)
GLOBAL_CXXFLAGS = $(CXXFLAGS)
################################################################################
# Detect OS if not set
ifeq ($(OS),)
OS := $(shell uname -s | tr A-Z a-z)
endif
ifeq ($(OS),Windows_NT)
OS := windows
UNIX :=
endif
ifeq ($(OS),linux)
UNIX = unix
endif
ifeq ($(OS),freebsd)
UNIX = unix
endif
################################################################################

# reset our variables between runs
define wipe
$(eval
SRCS :=
SRCS_$(OS) :=
SRCS_unix :=
OBJS :=
EXEC :=
LIBS :=
USES :=
CFLAGS :=
CPPFLAGS :=
CXXFLAGS :=
LDFLAGS :=
LDLIBS :=
PROVIDE_CFLAGS :=
PROVIDE_CPPFLAGS :=
PROVIDE_CXXFLAGS :=
PROVIDE_LDFLAGS :=
PROVIDE_LDLIBS :=
TYPE :=
TAG :=
CUR :=
)
endef

# create rules if extension is set
ifneq ($(EXT),)
%$(EXT) : %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

%$(EXT) : %.c
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@
endif

# use an extra expansion to create a clean rule
clean = $(eval clean :: ; $$(RM) $1)

# load config
# $1 = path to project.cfg
define loadconfig
$(eval
# find the directory
CUR := $$(dir $1)
TAG := $$(patsubst %/,%,$$(CUR))
ifeq ($$(TAG),)
TAG := none
endif
$(info Using config: $1)
include $1
OBJS += $$(SRCS:.c=.o) $$(SRCS_$(OS):.c=.o) $(if $(UNIX),$$(SRCS_unix:.c=.o))
OBJS := $$(addprefix $$(CUR),$$(OBJS))
EXEC := $$(addprefix $$(CUR),$$(EXEC))
SRCS := $$(addprefix $$(CUR),$$(SRCS))
ifneq ($$(filter lib%,$$(TYPE)),)
# are we building a static library?
TYPE.$$(TAG) := library
PROVIDE_CFLAGS.$$(TAG) := $$(PROVIDE_CFLAGS)
PROVIDE_CXXFLAGS.$$(TAG) := $$(PROVIDE_CXXFLAGS)
PROVIDE_CPPFLAGS.$$(TAG) := $$(PROVIDE_CPPFLAGS)
PROVIDE_LDFLAGS.$$(TAG) := $$(PROVIDE_LDFLAGS)
PROVIDE_LDLIBS.$$(TAG) := $$(PROVIDE_LDLIBS)
EXEC := $(addsuffix .a,$$(EXEC))
else
# we must be building a regular executable
TYPE.$$(TAG) := executable
EXEC := $(addsuffix $(EXT),$$(EXEC))
endif
# Copy all settings into our database
USES.$$(TAG) := $$(USES)
EXEC.$$(TAG) := $$(EXEC)
OBJS.$$(TAG) := $$(OBJS)
SRCS.$$(TAG) := $$(SRCS)
CFLAGS.$$(TAG) := $$(CFLAGS)
CXXFLAGS.$$(TAG) := $$(CXXFLAGS)
CPPFLAGS.$$(TAG) := $$(CPPFLAGS)
LDFLAGS.$$(TAG) := $$(LDFLAGS)
LDLIBS.$$(TAG) := $$(LDLIBS)
)
endef
# generate rules to build an executable or library
# $1 = tag
define genrule
$(eval
all :: $$(EXEC.$1)
ifneq ($$(filter lib%,$$(TYPE.$1)),)
# are we building a static library?
$$(EXEC.$1) :: $$(EXEC.$1)($$(OBJS.$1))
else
# we must be building a regular executable
$$(EXEC.$1) :: $$(OBJS.$1) $$(foreach u,$$(USES.$1),$$(EXEC.$$u))
endif
$$(EXEC.$1) :: CFLAGS := $$(GLOBAL_CFLAGS) $$(foreach u,$$(USES.$1),$$(PROVIDE_CFLAGS.$$u)) $$(CFLAGS.$1)
$$(EXEC.$1) :: CXXFLAGS := $$(GLOBAL_CXXFLAGS) $$(foreach u,$$(USES.$1),$$(PROVIDE_CXXFLAGS.$$u)) $$(CXXFLAGS.$1)
$$(EXEC.$1) :: CPPFLAGS := $$(GLOBAL_CPPFLAGS) $$(foreach u,$$(USES.$1),$$(PROVIDE_CPPFLAGS.$$u)) -I. $$(CPPFLAGS.$1)
$$(EXEC.$1) :: LDFLAGS := $$(foreach u,$$(USES.$1),$$(PROVIDE_LDFLAGS.$$u)) $$(LDFLAGS.$1)
$$(EXEC.$1) :: LDLIBS := $$(foreach u,$$(USES.$1),$$(PROVIDE_LDLIBS.$$u)) $$(LDLIBS.$1)
)
endef
################################################################################
PROJECTS := $(wildcard */project.cfg project.cfg)

$(call wipe) # make sure nothing stale slips in...
# First phase: pull in config files
$(foreach d,$(PROJECTS),$(call loadconfig,$d) $(eval TARGET_TAGS += $(TAG)) $(call wipe))
# Second phase: generate rules
$(foreach t,$(TARGET_TAGS),$(call genrule,$t) $(call clean,$$(EXEC.$t) $$(OBJS.$1)))
