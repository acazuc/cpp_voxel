NAME = cpp_voxel

CXX = g++
#CXX = x86_64-w64-mingw32-g++

CXXFLAGS = -Wall -Wextra -Ofast -g -march=native
#CXXFLAGS+= -fsanitize=address
#CXXFLAGS+= -fsanitize=thread
#CXXFLAGS+= -fsanitize=leak
#CXXFLAGS+= -fsanitize=undefined
#CXXLFAGS+= -Wl,-subsystem,windows

LDFLAGS = -flto=6

INCLUDES = -I include
INCLUDES+= -I lib/glfw/include
INCLUDES+= -I lib/glad/include
INCLUDES+= -I lib/librender/include
INCLUDES+= -I lib/libformat/include
INCLUDES+= -I lib/libgzip/include
INCLUDES+= -I lib/freetype/include
INCLUDES+= -I lib

SRCS_PATH = src/

SRCS_NAME = Main.cpp \
	    Frustum.cpp \
	    Clouds.cpp \
	    Skybox.cpp \
	    AABB.cpp \
	    TickManager.cpp \
	    Noise/SimplexNoise.cpp \
	    Noise/SimplexNoiseOctave.cpp \
	    Noise/WorleyNoise.cpp \
	    Utils/readfile.cpp \
	    Utils/System.cpp \
	    Player/Player.cpp \
	    Player/PlayerRaycast.cpp \
	    Player/PlayerInventory.cpp \
	    Player/PlayerInventorySlot.cpp \
	    Blocks/Blocks.cpp \
	    Blocks/Block.cpp \
	    Blocks/BlockBase.cpp \
	    Blocks/BlockSapling.cpp \
	    Blocks/BlockRail.cpp \
	    Blocks/BlockRedstone.cpp \
	    Blocks/BlockLiquid.cpp \
	    Blocks/BlockWater.cpp \
	    Blocks/BlockLava.cpp \
	    Blocks/BlockSlab.cpp \
	    Blocks/BlockDoubleSlab.cpp \
	    Blocks/BlockTorch.cpp \
	    Blocks/BlockGrass.cpp \
	    Blocks/BlockLeaves.cpp \
	    Shaders/BlocksShader.cpp \
	    Shaders/CloudsShader.cpp \
	    Shaders/SkyboxShader.cpp \
	    Shaders/FocusedShader.cpp \
	    Shaders/EntityShader.cpp \
	    Shaders/ParticlesShader.cpp \
	    Shaders/BreakShader.cpp \
	    Shaders/SunMoonShader.cpp \
	    Shaders/GuiShader.cpp \
	    Shaders/DroppedShader.cpp \
	    Shaders/GuiTextShader.cpp \
	    Shaders/Shader.cpp \
	    Entities/BodyPart.cpp \
	    Entities/Human.cpp \
	    Entities/Creeper.cpp \
	    Entities/Zombie.cpp \
	    Entities/Pig.cpp \
	    Entities/Pigman.cpp \
	    Entities/Pigzombie.cpp \
	    Entities/Sheep.cpp \
	    Entities/Skeleton.cpp \
	    Entities/Slime.cpp \
	    Entities/Cow.cpp \
	    Entities/Entity.cpp \
	    Entities/EntitiesManager.cpp \
	    Entities/DroppedBlock.cpp \
	    World/World.cpp \
	    World/ChunkBlock.cpp \
	    World/Chunk.cpp \
	    World/Region.cpp \
	    World/ChunkLoader.cpp \
	    World/ChunkUpdater.cpp \
	    World/ChunkStorage.cpp \
	    World/Level.cpp \
	    World/Biomes/Biome.cpp \
	    World/Biomes/Biomes.cpp \
	    World/Generators/GeneratorTree.cpp \
	    World/Generators/GeneratorMinable.cpp \
	    Particles/Particle.cpp \
	    Particles/ParticlesManager.cpp \
	    Gui/Gui.cpp \
	    Gui/GuiCross.cpp \
	    Gui/GuiBar.cpp \
	    Gui/GuiHearts.cpp \
	    Gui/GuiInventory.cpp \
	    Gui/GuiButton.cpp \
	    Gui/GuiLabel.cpp \
	    Gui/GuiLagometer.cpp \
	    Gui/GuiTitle.cpp \
	    Gui/GuiSingleplayer.cpp \
	    Gui/GuiSingleplayerEntry.cpp \
	    Gui/GuiMultiplayer.cpp \
	    Gui/GuiMultiplayerEntry.cpp \
	    Gui/GuiSlider.cpp \
	    Gui/Options/GuiOptions.cpp \
	    Gui/Options/GuiOptionsChat.cpp \
	    Gui/Options/GuiOptionsControls.cpp \
	    Gui/Options/GuiOptionsSkin.cpp \
	    Gui/Options/GuiOptionsSound.cpp \
	    Gui/Options/GuiOptionsVideo.cpp \
	    WorldScreen/WorldScreen.cpp \
	    WorldScreen/WorldScreenGui.cpp \
	    TitleScreen/TitleScreen.cpp \
	    NBT/NBTFile.cpp \
	    NBT/NBTTag.cpp \
	    NBT/NBTTagEnd.cpp \
	    NBT/NBTTagByte.cpp \
	    NBT/NBTTagShort.cpp \
	    NBT/NBTTagInt.cpp \
	    NBT/NBTTagLong.cpp \
	    NBT/NBTTagFloat.cpp \
	    NBT/NBTTagDouble.cpp \
	    NBT/NBTTagByteArray.cpp \
	    NBT/NBTTagString.cpp \
	    NBT/NBTTagList.cpp \
	    NBT/NBTTagCompound.cpp \
	    NBT/NBTTagIntArray.cpp \
	    NBT/NBTTagLongArray.cpp \
	    NBT/NBTBuffer.cpp \
	    NBT/NBTStream.cpp \
	    NBT/NBTGZipFile.cpp \
	    NBT/NBTMgr.cpp \
	    NBT/NBTCompoundSanitizer.cpp \

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_PATH = obj/

OBJS_NAME = $(SRCS_NAME:.cpp=.o)

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

LIBRARIES = -L lib/glfw -l:libglfw3.a
LIBRARIES+= -L lib/glad -l:libglad.a
LIBRARIES+= -L lib/librender -l:librender.a
LIBRARIES+= -L lib/libformat -l:libformat.a
LIBRARIES+= -L lib/libgzip -l:libgzip.a
LIBRARIES+= -L lib/freetype -l:libfreetype.a
LIBRARIES+= -L lib/libpng -l:libpng16.a
LIBRARIES+= -L lib/zlib -l:libz.a
LIBRARIES+= -lGL -lX11 -lXrandr -lXinerama -lXcursor -lXxf86vm
LIBRARIES+= -ldl -lrt -lm -lpthread
#LIBRARIES+= -lws2_32 -lwsock32 -lwinmm -lgdi32 -static-libstdc++ -static-libgcc -lopengl32 -lm

all: odir $(NAME)

size:
	@wc `find $(SRCS_PATH) -type f`

$(NAME): $(OBJS)
	@echo "LD $(NAME)"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(NAME) $^ $(LIBRARIES)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp
	@echo "CXX $<"
	@$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCLUDES)

odir:
	@mkdir -p $(OBJS_PATH)
	@mkdir -p $(OBJS_PATH)/Noise
	@mkdir -p $(OBJS_PATH)/Utils
	@mkdir -p $(OBJS_PATH)/Player
	@mkdir -p $(OBJS_PATH)/Blocks
	@mkdir -p $(OBJS_PATH)/Shaders
	@mkdir -p $(OBJS_PATH)/Entities
	@mkdir -p $(OBJS_PATH)/World
	@mkdir -p $(OBJS_PATH)/World/Biomes
	@mkdir -p $(OBJS_PATH)/World/Generators
	@mkdir -p $(OBJS_PATH)/Particles
	@mkdir -p $(OBJS_PATH)/Gui
	@mkdir -p $(OBJS_PATH)/Gui/Options
	@mkdir -p $(OBJS_PATH)/WorldScreen
	@mkdir -p $(OBJS_PATH)/TitleScreen
	@mkdir -p $(OBJS_PATH)/NBT

clean:
	@rm -f $(OBJS)
	@rm -f $(NAME)

re: clean all

.PHONY: clean re odir
