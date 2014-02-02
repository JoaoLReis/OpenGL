#include "SDL/SDL.h"
#include "SDL/SDL_image.h" 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GL/glew.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define VERTICES 0
#define COLORS 1
#define NORMALS 2
#define TEXTURE 3

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

#define ROTATE 0
#define TRANSLATE 1
#define SCALE 2

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2 

#define tNEAR  1.0f
#define tFAR 4000.0f
#define tFOVY 30.0f
#define PI 3.141f

#define FRAMES_PER_SECOND 60

#define NUMTILESX 30
#define NUMTILESY 20

/*static const glm::vec3 ligthpos = glm::vec3(0.0, 0.0, 5.0);

static const glm::vec2 LightAttenuation = glm::vec2(0.0, 0.005);

static const glm::vec3 AmbientLightColor = glm::vec3( 0.3, 0.3, 0.3);
static const glm::vec3 LightDiffuseColor = glm::vec3( 0.9, 0.9, 0.9);
static const glm::vec3 LightSpecularColor = glm::vec3( 0.9, 0.9, 0.9);

static const glm::vec3 MaterialAmbientColor = glm::vec3(0.8, 0.8, 0.8);
static const glm::vec3 MaterialDiffuseColor = glm::vec3(0.9, 0.9, 0.9);
static const glm::vec3 MaterialSpecularColor = glm::vec3(0.9, 0.9 ,0.9);

static const float MaterialShininess = 64.0f;*/

using namespace std;
