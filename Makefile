NAME = cpp_voxel

CC = g++

CFLAGS = -Wall -Wextra -Werror -O2 -g -march=native -flto=8

INCLUDES_PATH = include -I lib/glfw/include -I lib/glad/include -I lib/librender/include -I lib/libformat/include

SRCS_PATH = src/

SRCS_NAME = Main.cpp \
	    World.cpp \
	    ChunkBlock.cpp \
	    Chunk.cpp \
	    Player.cpp \
	    Frustum.cpp \
	    ChunkLoader.cpp \
	    Clouds.cpp \
	    Skybox.cpp \
	    Entity.cpp \
	    AABB.cpp \
	    Region.cpp \
	    EntitiesManager.cpp \
	    TickManager.cpp \
	    Particle.cpp \
	    ParticlesManager.cpp \
	    Noise/SimplexNoise.cpp \
	    Noise/SimplexNoiseOctave.cpp \
	    Noise/WorleyNoise.cpp \
	    Utils/readfile.cpp \
	    Player/PlayerRaycast.cpp \
	    Blocks/Blocks.cpp \
	    Blocks/Block.cpp \
	    Shaders/BlocksShader.cpp \
	    Shaders/CloudsShader.cpp \
	    Shaders/SkyboxShader.cpp \
	    Shaders/FocusedShader.cpp \
	    Shaders/EntityShader.cpp \
	    Shaders/ParticlesShader.cpp \
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

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_PATH = obj/

OBJS_NAME = $(SRCS_NAME:.cpp=.o)

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

LIBRARY = -L lib/glfw/src -l:libglfw3.a -L lib/glad -l:libglad.a -L lib/librender -l:librender.a -L lib/libformat -l:libformat.a -L lib/libpng -l:libpng16.a -L lib/zlib -l:libz.a -lGL -lX11 -lXrandr -lXinerama -lXcursor -lXxf86vm -ldl -lrt -lm -lpthread

all: odir $(NAME)

$(NAME): $(OBJS)
	@echo " - Making $(NAME)"
	@$(CC) $(CFLAGS) -o $(NAME) $^ $(LIBRARY)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.cpp
	@echo " - Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $< -I$(INCLUDES_PATH)

odir:
	@mkdir -p $(OBJS_PATH)
	@mkdir -p $(OBJS_PATH)/Noise
	@mkdir -p $(OBJS_PATH)/Utils
	@mkdir -p $(OBJS_PATH)/Player
	@mkdir -p $(OBJS_PATH)/Blocks
	@mkdir -p $(OBJS_PATH)/Shaders
	@mkdir -p $(OBJS_PATH)/Entities

clean:
	@echo " - Cleaning objs"
	@rm -f $(OBJS)

fclean: clean
	@echo " - Cleaning executable"
	@rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re odir
