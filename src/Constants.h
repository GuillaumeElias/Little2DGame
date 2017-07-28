#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    static const bool LINUX = false;
#else
    static const bool LINUX = true;
#endif

static const int SCREEN_WIDTH = 880;
static const int MARGIN_BOTTOM = 35;
static const int SCREEN_HEIGHT = 600 + MARGIN_BOTTOM;
static const int FRAME_TIME = 35;

static const double WORLD_FALL_ACC = 0.5; //fall velocity

static const int DIALOG_LEFT_MARGIN = 60;
static const long DIALOG_TIME_LETTER = /*50*/50;
static const long DIALOG_TIME_SENTENCE = /*2000*/500;
static const int DIALOG_CHARACTER_TOP_MARGIN = SCREEN_HEIGHT / 3 + 10;
static const int DIALOG_CHARACTER_RIGHT_MARGIN = 20;

static const int CHARACTER_NB_SPRITES = 2;
static const long CHARACTER_MOVE_TIME = 1000;

static const int WALKING_ANIMATION_FRAMES = 4;
static const int END_OBJECT_ANIMATION_FRAMES = 2;
static const int END_OBJECT_ANIMATION_FRAME_TIME = 300;

static const int TRIGGER_WIDTH = 10;
static const int TRIGGER_DEFAULT_HEIGHT = 100;
static const int TRIGGER_DIALOG_PARAM_MASK = 100;
static const int TRIGGER_PAZOOK_PARAM_MASK = 200;

static const int PLAYER_INIT_SPEED = 10;
static const int PLAYER_WALK_FRAME_TIME = 80;
static const int PLAYER_INIT_SPEED_IN_JUMP = 7;
static const int PLAYER_INIT_HEALTH = 3;
static const int PLAYER_WIDTH = 30;
static const int PLAYER_HEIGHT = 55;
static const int PLAYER_CLIMB_STEP = 1;
static const double PLAYER_JUMP_VEL = 8.5;
static const double PLAYER_SUPER_JUMP_VEL = 12;
static const int PLAYER_FIRE_INTERVAL = 300;
static const int PLAYER_FIRE_ANIM_TIME = 150;
static const int PLAYER_DIE_TIME = 800;

static const int BLOB_JUMP_HEIGHT = 35;
static const int BLOB_LIFE = 2;

static const int ZOMBIE_LIFE = 4;
static const int ZOMBIE_WIDTH = 28;

static const int BEAST_LIFE = 5;
static const int BEAST_WIDTH = 42;
static const int BEAST_DISPLACEMENT = 5;
static const int BEAST_ANIM_FRAME_TIME = 300;
static const int BEAST_ANIM_NB_FRAMES = 2;

static const int BULLET_DISPLACEMENT = 20;
static const int BULLET_TOP_MARGIN = 18;

static const int ITEM_IN_INVENTORY_WIDTH = 50;
static const int ITEM_IN_INVENTORY_MARGIN_TOP = 5;
static const int ITEM_IN_INVENTORY_MARGIN_LEFT = 430;

static const int BLOB_TOUCH_RESET = 1000;
static const double ELEVATOR_DEFAULT_ALTITUDE = 100;
static const double BOTTOM_BAR_RED_TIME = 200;

static const int PAZOOK_NUMBER_MAX = 3;
static const int PAZOOK_CHARACTER_TOP_MARGIN = 60;
static const int PAZOOK_DIALOG_TOP_MARGIN = 80;
static const int PAZOOK_CARDS_MARGIN_TOP = 250;
static const int PAZOOK_CARD1_X = 100;
static const int PAZOOK_CARD2_X = 320;
static const int PAZOOK_CARD3_X = 540;
static const int PAZOOK_WINNING_POINTS = 10;
static const int PAZOOK_NB_WINNING_NEEDED = 2;

static const int ZOMBIE_SPAWNER_ANIMATION_FRAMES = 3;
static const int ZOMBIE_SPAWNER_ANIMATION_TIME = 200;
static const int ZOMBIE_SPAWNER_RATE = 10000;
static const int ZOMBIE_SPAWNER_MAX_LIVE_SPAWNED_ZOMBIES = 3;


struct Position{
    int x, y;
};

enum Direction {LEFT, RIGHT};
enum BulletType{PLAYER_BULLET_1};

struct PlayerPosition : Position{
};

#endif // CONSTANTS_H_INCLUDED
