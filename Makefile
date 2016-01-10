# Copyright 2015 Henorvell Ge
# This file is a part of CampusExpBot 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
RELEASE_FLAGS = -Werror
WARNINGS = -Wall -Wextra
#ifeq ($(shell sh -c 'uname -o 2>/dev/null || echo not'),Cygwin)
DEBUG = -g
#else
#  DEBUG = -g -D_GLIBCXX_DEBUG
#endif
VERSION = 0.0.1
TARGET = campusbot
BUILD_DIR = $(CURDIR)
SRC_DIR = src
ODIR = obj
DDIR = .deps
OS  = $(shell uname -s)
OS_COMPILER := $(CXX)
OS_LINKER := $(CXX)
CXX = $(OS_COMPILER)
LD = $(OS_LINKER)
#DEFINES += -DLOCALIZE
CXXFLAGS += -ffast-math
LDFLAGS += $(PROFILE)
# enable optimizations. slow to build
ifdef RELEASE
  ifeq ($(NATIVE), osx)
    CXXFLAGS += -O3
  else
    CXXFLAGS += -Os
    LDFLAGS += -s
  endif
  OTHERS += -mmmx -m3dnow -msse -msse2 -msse3 -mfpmath=sse -mtune=native
  # Strip symbols, generates smaller executable.
  OTHERS += $(RELEASE_FLAGS)
#  DEBUG =
#  DEFINES += -DRELEASE
endif
ifdef CLANG
  ifeq ($(NATIVE), osx)
    OTHERS += -stdlib=libc++
  endif
  ifdef CCACHE
    CXX = CCACHE_CPP2=1 ccache $(CROSS)clang++
    LD  = CCACHE_CPP2=1 ccache $(CROSS)clang++
  else
    CXX = $(CROSS)clang++
    LD  = $(CROSS)clang++
  endif
  WARNINGS = -Wall -Wextra -Wno-switch -Wno-sign-compare -Wno-missing-braces -Wno-type-limits -Wno-narrowing
endif

OTHERS += --std=c++11
CXXFLAGS += $(WARNINGS) $(DEBUG) $(PROFILE) $(OTHERS) -MMD
ifeq ($(NATIVE), linux64)
  CXXFLAGS += -m64
  LDFLAGS += -m64
  TARGETSYSTEM=LINUX
else
  # Linux 32-bit
  ifeq ($(NATIVE), linux32)
    CXXFLAGS += -m32
    LDFLAGS += -m32
    TARGETSYSTEM=LINUX
  endif
endif
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
_OBJS = $(SOURCES:$(SRC_DIR)/%.cpp=%.o)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))
ifdef LANGUAGES
  L10N = localization
endif

ifeq ($(TARGETSYSTEM), LINUX)
  ifneq ($(PREFIX),)
    DEFINES += -DPREFIX="$(PREFIX)" -DDATA_DIR_PREFIX
  endif
endif
LDFLAGS += -lrt
LDFLAGS += -ltinyxml
LDFLAGS += -lSQLiteCpp
LDFLAGS += -lsqlite3
LDFLAGS += -lbcm2835
LDFLAGS += -lpthread
LDFLAGS += -lpython2.7
LDFLAGS += -L./lib
all: $(TARGET) $(L10N)
	@
$(TARGET): $(ODIR) $(DDIR) $(OBJS)
	$(LD) $(W32FLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
$(ODIR):
	mkdir -p $(ODIR)

$(DDIR):
	@mkdir $(DDIR)

$(ODIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(DEFINES) $(CXXFLAGS) -c $< -o $@
clean:
	rm -rf $(TARGET)
	rm -rf $(ODIR)
sqlitecpp:
	cd SQLiteCpp;cmake -DSQLITECPP_BUILD_EXAMPLES=OFF -DSQLITECPP_BUILD_TESTS=OFF .;cmake --build .
	cp SQLiteCpp/lib*.a ./lib
