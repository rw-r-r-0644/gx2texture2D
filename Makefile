BASEDIR	:= $(dir $(firstword $(MAKEFILE_LIST)))
VPATH	:= $(BASEDIR)

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing header files
# ROMFS is a folder to generate app's romfs
#---------------------------------------------------------------------------------
TARGET		:=	gx2TextureShader
SOURCES		:=	src src/matrix
INCLUDES	:=	-Iinclude
ROMFS		:=	romfs

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS		+=	-O2 -Wall -std=c11 $(INCLUDES)
CXXFLAGS	+=	-O2 -Wall $(INCLUDES)
LDFLAGS		+=	$(WUT_NEWLIB_LDFLAGS) $(WUT_STDCPP_LDFLAGS) $(WUT_DEVOPTAB_LDFLAGS) \
				-lm -lcoreinit -lgx2 -lproc_ui -lsysapp -lwhb -lgfd -lromfs-wiiu


#---------------------------------------------------------------------------------
# get a list of objects
#---------------------------------------------------------------------------------
include $(DEVKITPRO)/portlibs/ppc/share/romfs-wiiu.mk
CFILES		:=	$(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(wildcard $(dir)/*.cpp))
SFILES		:=	$(foreach dir,$(SOURCES),$(wildcard $(dir)/*.s))
OBJECTS		:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o) $(ROMFS_TARGET)

#---------------------------------------------------------------------------------
# objectives
#---------------------------------------------------------------------------------
$(TARGET).rpx: $(OBJECTS)

clean:
	@echo CLEAN ...
	@rm -rf $(TARGET).rpx $(TARGET).rpx.elf $(OBJECTS) $(OBJECTS:.o=.d)

#---------------------------------------------------------------------------------
# wut
#---------------------------------------------------------------------------------
include $(WUT_ROOT)/share/wut.mk

#---------------------------------------------------------------------------------
# portlibs
#---------------------------------------------------------------------------------
PORTLIBS	:=	$(DEVKITPRO)/portlibs/ppc
LDFLAGS		+=	-L$(PORTLIBS)/lib
CFLAGS		+=	-I$(PORTLIBS)/include -I$(PORTLIBS)/include/SDL2
CXXFLAGS	+=	-I$(PORTLIBS)/include -I$(PORTLIBS)/include/SDL2