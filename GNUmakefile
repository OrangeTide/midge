#
################################################################################
all ::
clean ::
clean-all : clean ; $(RM) settings.mk
.PHONY : all clean clean-all help configure
################################################################################
CONFIG_CHOICES = $(patsubst settings-%.mk.template,%,$(notdir $(wildcard configs/settings-*.mk.template)))

# generate shell commands to display config helps on config-XXXXX
define echo_config_choices
$(foreach i,$1,
@echo configure-$i
@echo "  Configure for $i."
)
endef
################################################################################
configure :
	@echo Select one of
	@echo -------------
	$(call echo_config_choices,$(CONFIG_CHOICES))

configure-% :
	cp configs/settings-$*.mk.template settings.mk

help :
	@echo Build Help
	@echo ----------
	@echo usage: make [goals...]
	@echo
	@echo make all
	@echo "  Builds everything. (default)"
	@echo make clean
	@echo "  Cleans all objects and executables."
	@echo make help
	@echo "  This help."
	@echo make configure
	@echo "  Show configuration help."
	$(call echo_config_choices,$(CONFIG_CHOICES))

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
SRCS_windows :=
OBJS :=
EXEC :=
USES :=
USES_$(OS) :=
USES_unix :=
USES_windows :=
CFLAGS :=
CFLAGS_$(OS) :=
CPPFLAGS :=
CPPFLAGS_$(OS) :=
CXXFLAGS :=
CXXFLAGS_$(OS) :=
LDFLAGS :=
LDFLAGS_$(OS) :=
LDLIBS :=
LDLIBS_$(OS) :=
PROVIDE_CFLAGS :=
PROVIDE_CFLAGS_$(OS) :=
PROVIDE_CPPFLAGS :=
PROVIDE_CPPFLAGS_$(OS) :=
PROVIDE_CXXFLAGS :=
PROVIDE_CXXFLAGS_$(OS) :=
PROVIDE_LDFLAGS :=
PROVIDE_LDFLAGS_$(OS) :=
PROVIDE_LDLIBS :=
PROVIDE_LDLIBS_$(OS) :=
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
$(info Using project: $1)
include $1
OBJS += $$(SRCS:.c=.o) $$(SRCS_$(OS):.c=.o) $(if $(UNIX),$$(SRCS_unix:.c=.o))
OBJS := $$(addprefix $$(CUR),$$(OBJS))
EXEC := $$(addprefix $$(CUR),$$(EXEC))
SRCS := $$(addprefix $$(CUR),$$(SRCS))
ifneq ($$(filter lib%,$$(TYPE)),)
# are we building a static library?
TYPE.$$(TAG) := library
PROVIDE_CFLAGS.$$(TAG) := $$(PROVIDE_CFLAGS) $$(PROVIDE_CFLAGS_$(OS))
PROVIDE_CXXFLAGS.$$(TAG) := $$(PROVIDE_CXXFLAGS) $$(PROVIDE_CXXFLAGS_$(OS))
PROVIDE_CPPFLAGS.$$(TAG) := $$(PROVIDE_CPPFLAGS) $$(PROVIDE_CPPFLAGS_$(OS))
PROVIDE_LDFLAGS.$$(TAG) := $$(PROVIDE_LDFLAGS) $$(PROVIDE_LDFLAGS_$(OS))
PROVIDE_LDLIBS.$$(TAG) := $$(PROVIDE_LDLIBS) $$(PROVIDE_LDLIBS_$(OS))
EXEC := $(addsuffix .a,$$(EXEC))
else
# we must be building a regular executable
TYPE.$$(TAG) := executable
EXEC := $(addsuffix $(EXT),$$(EXEC))
endif
# Copy all settings into our database
USES.$$(TAG) := $$(USES) $$(USES_$(OS))
EXEC.$$(TAG) := $$(EXEC)
OBJS.$$(TAG) := $$(OBJS)
SRCS.$$(TAG) := $$(SRCS) $$(SRCS_$(OS))
CFLAGS.$$(TAG) := $$(CFLAGS)
CXXFLAGS.$$(TAG) := $$(CXXFLAGS)
CPPFLAGS.$$(TAG) := $$(CPPFLAGS)
LDFLAGS.$$(TAG) := $$(LDFLAGS) $$(LDFLAGS_$(OS))
LDLIBS.$$(TAG) := $$(LDLIBS) $$(LDLIBS_$(OS))
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
$(foreach t,$(TARGET_TAGS),$(call genrule,$t) $(call clean,$$(EXEC.$t) $$(OBJS.$t)))
