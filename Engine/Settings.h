// SFML Settings
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_TITLE "Owaria!"
#define TARGET_FPS 60.0f
#define TARGET_UPS 60.0f
#define TARGET_FPS_TIME 1.0f / TARGET_FPS
#define TARGET_UPS_TIME 1.0f / TARGET_UPS
#define V_SYNC true

// Box2D Settings
#define WORLD_GRAVITY 10.0f
#define BOX2D_VELOCITY_ITERATIONS 8
#define BOX2D_POSITION_ITERATIONS 3
#define BOX2D_SCALE 30.0f

// Math Settings
#define ENGINE_PI 3.14f

// Camera Settings
#define CAMERA_LERP 0.025f
#define CAMERA_FREE_ROAM_SPEED 1.0f

// Player Settings
#define PLAYER_SPEED 7
#define PLAYER_JUMP_SPEED 10
#define PLAYER_DENSITY 10
#define PLAYER_FRICTION 3
#define PLAYER_TORQUE 10

// Map Settings
#define MAP_FRICTION 3

// Constants
#define LEFT_DIRECTION -1
#define RIGHT_DIRECTION 1
#define TOP_DIRECTION 1
#define DOWN_DIRECTION -1
#define NONE_DIRECTION 0

// Wearables
#define WEARABLE_MINIMUM 0
#define WEARABLE_WEAPON 0
#define WEARABLE_SHIELD 1
#define WEARABLE_HELMET 2
#define WEARABLE_PAULDRON 3
#define WEARABLE_GAUNTLETS 4
#define WEARABLE_BOOTS 5
#define WEARABLE_COUNT 6