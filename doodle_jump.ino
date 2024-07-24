//TODO: ADD COMMENTS, POWERUPS AND DIFFERENT TYPES OF ENEMIES, ADD A MENU S WITH A DOODLE ICON?, MAYBE MAKE AN ENDLESS MODE AND AND MODE WITH AN ENDING, OR MAYBE TRY TO MAKE THE PLAYER WRAP AROUND THE SCREEN A LITTLE BUT THIS WOULD AFFECT HITBOXES????
//TODO: ADD SOUND EFFECTS, MAKE IT MORE DIFFICULT, MAYBE MAKE MORE GAMES ANE A WHOLE GAME SELECTION SYSTEM

#include <Adafruit_GFX.h>
#include <RGBmatrixPanel.h>
#include <gamma.h>

// define the wiring of the LED screen
const uint8_t CLK = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;

const int BULLET_SPEED = 1;
const int PLAYER_SPEED = 3;
const int SCROLL_LINE = 15;
const int PLATFORM_SPEED = 10;
const int RESET_PLATFORM_TIME = 20;

const int NUM_PLATFORMS = 30;
const int MAX_JUMP = 6;
void drawdoodle(int x, int y);
// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

class Color
{
  public:
    int red;
    int green;
    int blue;
    Color()
    {
        red = 0;
        green = 0;
        blue = 0;
    }
    Color(int r, int g, int b)
    {
        red = r;
        green = g;
        blue = b;
    }
    operator uint16_t() const { return matrix.Color333(red, green, blue); }
};

const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);

class Player {
  public:
    Player() {
      x = 0;
      y = 15;
      falling = true;
      remainingJump = 0;
    }

    Player(int in_x, int in_y) {
      x = in_x;
      y = in_y;
      falling = true;
      remainingJump = 0;
    }

    void set_x(int in_x) {
      x = in_x;
    }

    void set_y(int in_y) {
      y = in_y;
    }

    void set_remainingJump(int in_remainingJump) {
      remainingJump = in_remainingJump;
    }

    void flip() {
      falling = !falling;
    }

    void initialize(int in_x, int in_y) {
      x = in_x;
      y = in_y;
      falling = true;
    }

    int get_x() {
      return x;
    }

    int get_y() {
      return y;
    }

    int get_remainingJump() {
      return remainingJump;
    }

    bool is_falling() {
      return falling;
    }
    bool hit_line(){
      return hitline;
    }

    void draw(Color c) {
      matrix.drawPixel(x, y+1, c);
      matrix.drawPixel(x-1, y, c);
      matrix.drawPixel(x-1, y+1, c);
      matrix.drawPixel(x-1, y+2, c);
      matrix.drawPixel(x-2, y, c);
      matrix.drawPixel(x-2, y+1, c);
      matrix.drawPixel(x-2, y+2, c);
    }

    void up() {
      x++;
    }

    void down() {
      x--;
    }

  private: 
    int x;
    int y;
    int remainingJump;
    bool falling;
    bool hitline;
};

class Enemy {
  public:
    Enemy() {
      x = 0;
      y = 0;
    }
    Enemy(int in_x, int in_y) {
      x = in_x;
      y = in_y;
    }

    void initialize(int in_x, int in_y) {
        alive = true;
        x = in_x;
        y = in_y;
    }

    int get_x() {
      return x;
    }

    int get_y() {
      return y;
    }

    bool is_alive() {
      return alive;
    }

    void die() {
      alive = false;
    }

    void draw(Color c, Color eye) {
      matrix.drawPixel(x, y, eye);
      matrix.drawPixel(x, y+1, c);
      matrix.drawPixel(x, y + 2, eye);
      matrix.drawPixel(x - 1, y, c);
      matrix.drawPixel(x - 1, y + 1, c);
      matrix.drawPixel(x - 1, y + 2, c);
    }

    void up() {
      x++;
    }

    void down() {
      x--;
    }

  private:
    bool alive;
    int x;
    int y;
};

class Platform {
  public:
    Platform() {
      x = 32;
      y = 0;
      type = 0;
      alive = false;
      right = true;
      justFlipped = false;
      time = RESET_PLATFORM_TIME;
    }

    Platform(int in_x, int in_y) {
      x = in_x;
      y = in_y;
      alive = true;
      right = true;
      justFlipped = false;
      time = RESET_PLATFORM_TIME;
    }

    Platform(int in_x, int in_y, int in_type) {
      x = in_x;
      y = in_y;
      type = in_type;
      alive = true;
      justFlipped = false;
      time = RESET_PLATFORM_TIME;
    }

    void initialize(int in_x, int in_y){
      alive = true;
      x = in_x;
      y = in_y;
      type = 0;
      right = true;
      justFlipped = false;
      time = RESET_PLATFORM_TIME;
    }

    void initialize(int in_x, int in_y, int in_type){
      alive = true;
      x = in_x;
      y = in_y;
      type = in_type;
      right = true;
      justFlipped = false;
      time = RESET_PLATFORM_TIME;
    }

    void set_justFlipped(bool in_justFlipped) {
      justFlipped = in_justFlipped;
    }

    bool out_of_time() {
      return time <= 0;
    }

    void decrement_time() {
      time--;
    }

    void respawn() {
      alive = true;
    }

    void reset_time() {
      time = RESET_PLATFORM_TIME;
    }

    int get_x() {
      return x;
    }

    int get_y() {
      return y;
    }

    int get_type() {
      return type;
    }

    int get_time() {
      return time;
    }

    bool is_right() {
      return right;
    }

    void flip() {
      right = !right;
      justFlipped = true;
    }

    void die() {
      alive = false;
    }

    bool hasJustFlipped() {
      return justFlipped;
    }

    bool is_alive() {
      return alive;
    }

    void draw(Color c) {
      matrix.drawPixel(x, y, c);
      matrix.drawPixel(x, y+1, c);
      matrix.drawPixel(x, y+2, c);
    }

    void up() {
      x++;
    }

    void down() {
      x--;
    }

    void move() {
      if(right) y++;
      else y--;
      justFlipped = false;
    }

  private:
    int x;
    int y;
    int type;
    bool alive;
    bool right;
    bool justFlipped;
    int time;
};

class Bullet {
  public:
    Bullet() {
      x = 0;
      y = 0;
      alive = false;
    }
    void initialize(int in_x, int in_y) {
      x = in_x;
      y = in_y;
      alive = true;
    }

    int get_x() {
      return x;
    }

    int get_y() {
      return y;
    }

    void draw(Color c) {
      matrix.drawPixel(x, y, c);
    }

    void die() {
      alive = false;
    }

    bool is_alive() {
      return alive;
    }

    void up() {
      x++;
      if(x > 31) alive = false;
    }

    void down() {
      x--;
    }

  private:
    bool alive;
    int x;
    int y;
};

class Game {
  public:
    Game() {
      time = 0;
      nextPlatform = 0;
      nextEnemy = 0;
      score = 0;
      height = 0;
      playing = true;
    }

    void reset() {
      time = 0;
      nextPlatform = 0;
      nextEnemy = 0;
      score = 0;
      height = 0;
      playing = true;
    }

    void setup_game() {
      time = 0;
      while(!(digitalRead(BUTTON_PIN_NUMBER) == HIGH)) {
        if(time == 2500) {
          time = 0;
        }
        if (time == 0) {
            matrix.setCursor(0, 0);
            matrix.fillScreen(BLACK);
            matrix.setTextColor(WHITE);
            matrix.print("PRESS TO");
        }

        else if (time == 1000) {
            matrix.setCursor(0, 0);
            matrix.fillScreen(BLACK);
            matrix.setTextColor(GREEN);
            matrix.print("START");
        }

        delay(1);
        time++;
      }
      time = 0;
      reset_level();
      delay(250);
      player.set_x(4);
    }

    void drawdoodle(int x, int y){
      //row 1
      matrix.drawPixel(x, y+2, YELLOW);
      matrix.drawPixel(x, y+3, YELLOW);
      matrix.drawPixel(x, y+4, YELLOW);

      //row 2
      matrix.drawPixel(x-1, y+1, YELLOW);
      matrix.drawPixel(x-1, y+2, YELLOW);
      matrix.drawPixel(x-1, y+3, YELLOW);
      matrix.drawPixel(x-1, y+4, YELLOW);
      matrix.drawPixel(x-1, y+5, YELLOW);
      //row 3
      matrix.drawPixel(x-2, y, YELLOW);
      matrix.drawPixel(x-2, y+1, YELLOW);
      matrix.drawPixel(x-2, y+2, YELLOW);
      matrix.drawPixel(x-2, y+3, YELLOW);
      matrix.drawPixel(x-2, y+4, YELLOW);
      matrix.drawPixel(x-2, y+5, YELLOW);
      matrix.drawPixel(x-2, y+6, YELLOW);
      //row 4
      matrix.drawPixel(x-3, y, YELLOW);
      matrix.drawPixel(x-3, y+1, YELLOW);
      matrix.drawPixel(x-3, y+2, YELLOW);
      matrix.drawPixel(x-3, y+3, AQUA);
      matrix.drawPixel(x-3, y+4, YELLOW);
      matrix.drawPixel(x-3, y+5, AQUA);
      matrix.drawPixel(x-3, y+6, YELLOW);
      //row 5
      matrix.drawPixel(x-4, y, YELLOW);
      matrix.drawPixel(x-4, y+1, YELLOW);
      matrix.drawPixel(x-4, y+2, YELLOW);
      matrix.drawPixel(x-4, y+3, YELLOW);
      matrix.drawPixel(x-4, y+4, YELLOW);
      matrix.drawPixel(x-4, y+5, YELLOW);
      matrix.drawPixel(x-4, y+6, YELLOW);
      matrix.drawPixel(x-4, y+7, YELLOW);
      matrix.drawPixel(x-4, y+8, YELLOW);
      matrix.drawPixel(x-4, y+9, YELLOW);
      //row 6
      matrix.drawPixel(x-5, y, YELLOW);
      matrix.drawPixel(x-5, y+1, YELLOW);
      matrix.drawPixel(x-5, y+2, YELLOW);
      matrix.drawPixel(x-5, y+3, YELLOW);
      matrix.drawPixel(x-5, y+4, YELLOW);
      matrix.drawPixel(x-5, y+5, YELLOW);
      matrix.drawPixel(x-5, y+6, YELLOW);
      matrix.drawPixel(x-5, y+7, YELLOW);
      matrix.drawPixel(x-5, y+8, YELLOW);
      matrix.drawPixel(x-5, y+9, YELLOW);
      //row 7
      matrix.drawPixel(x-6, y, YELLOW);
      matrix.drawPixel(x-6, y+1, YELLOW);
      matrix.drawPixel(x-6, y+2, YELLOW);
      matrix.drawPixel(x-6, y+3, YELLOW);
      matrix.drawPixel(x-6, y+4, YELLOW);
      matrix.drawPixel(x-6, y+5, YELLOW);
      matrix.drawPixel(x-6, y+6, YELLOW);
      //row 8
      matrix.drawPixel(x-7, y, YELLOW);
      matrix.drawPixel(x-7, y+1, YELLOW);
      matrix.drawPixel(x-7, y+2, YELLOW);
      matrix.drawPixel(x-7, y+3, YELLOW);
      matrix.drawPixel(x-7, y+4, YELLOW);
      matrix.drawPixel(x-7, y+5, YELLOW);
      matrix.drawPixel(x-7, y+6, YELLOW);
      //row 9
      matrix.drawPixel(x-8, y, GREEN);
      matrix.drawPixel(x-8, y+1, GREEN);
      matrix.drawPixel(x-8, y+2, GREEN);
      matrix.drawPixel(x-8, y+3, GREEN);
      matrix.drawPixel(x-8, y+4, GREEN);
      matrix.drawPixel(x-8, y+5, GREEN);
      matrix.drawPixel(x-8, y+6, GREEN);
      //row 10
      matrix.drawPixel(x-9, y, GREEN);
      matrix.drawPixel(x-9, y+1, GREEN);
      matrix.drawPixel(x-9, y+2, GREEN);
      matrix.drawPixel(x-9, y+3, GREEN);
      matrix.drawPixel(x-9, y+4, GREEN);
      matrix.drawPixel(x-9, y+5, GREEN);
      matrix.drawPixel(x-9, y+6, GREEN);
      //row 11
      matrix.drawPixel(x-10, y, GREEN);
      matrix.drawPixel(x-10, y+1, GREEN);
      matrix.drawPixel(x-10, y+2, GREEN);
      matrix.drawPixel(x-10, y+3, GREEN);
      matrix.drawPixel(x-10, y+4, GREEN);
      matrix.drawPixel(x-10, y+5, GREEN);
      matrix.drawPixel(x-10, y+6, GREEN);
      //row 12
      matrix.drawPixel(x-11, y+1, WHITE);
      matrix.drawPixel(x-11, y+4, WHITE);
      //row 13
      matrix.drawPixel(x-12, y+1, WHITE);
      matrix.drawPixel(x-12, y+2, WHITE);
      matrix.drawPixel(x-12, y+4, WHITE);
      matrix.drawPixel(x-12, y+5, WHITE);
    }
    void update(int potentiometer_value, bool button_pressed) {
      if(!playing) {
        if (time == 0) {
            matrix.fillScreen(BLACK);
            matrix.setCursor(0, 0);
            matrix.setTextColor(RED);
            matrix.print("GAME OVER");
        } else if (time == 1500) {
            matrix.setCursor(0, 0);
            matrix.fillScreen(BLACK);
            matrix.setTextColor(WHITE);
            matrix.print("PRESS TO");
        }

        else if (time == 2500) {
            matrix.setCursor(0, 0);
            matrix.fillScreen(BLACK);
            matrix.setTextColor(WHITE);
            matrix.print("RETRY");
        }

        else if (time == 3500) {
            matrix.fillScreen(BLACK);
            matrix.setCursor(0, 0);
            matrix.setTextColor(RED);
            matrix.print("GAME OVER");
        } else if (time == 5000) {
            matrix.setCursor(0, 0);
            matrix.fillScreen(BLACK);
            matrix.setTextColor(WHITE);
            matrix.print("PRESS TO");
        }

        else if (time == 6000) {
            matrix.setCursor(0, 0);
            matrix.fillScreen(BLACK);
            matrix.setTextColor(WHITE);
            matrix.print("RETRY");
        }

        if (time == 7000) {
            matrix.fillScreen(BLACK);
            matrix.setCursor(0, 0);
            matrix.setTextColor(RED);
            matrix.print("GAME OVER");
        } else if (time == 8500) {
            matrix.setCursor(0, 0);
            matrix.fillScreen(BLACK);
            matrix.setTextColor(WHITE);
            matrix.print("PRESS TO");
        }

        else if (time == 9500) {
            matrix.setCursor(0, 0);
            matrix.fillScreen(BLACK);
            matrix.setTextColor(WHITE);
            matrix.print("RETRY");
        }

        if(time > 10500){
        int xc = 22;
        int yc = 4;
        int xv = 1;
        int yv = 1;
        while(!(digitalRead(BUTTON_PIN_NUMBER) == HIGH)){
          matrix.setCursor(0, 0);
          matrix.fillScreen(BLACK);
          drawdoodle(xc,yc);
          delay(250);
          //x velocity update
          xc += xv;
          if(xv > 0 && xc >= 31){
            xv *= -1;
          }
          else if(xv < 0 && xc <= 12){
            xv *= -1;
          }

          //y velocity update
          yc += yv;
          if(yv > 0 && yc >= 6){
            yv *= -1;
          }
          else if(yv < 0 && yc <= 0){
            yv *= -1;
          }
        }
        }
       delay(1);
       if(button_pressed) {
          reset();
          reset_level();
          delay(250);
       }
       time++;
       return;
      }


      if(player.get_x() < 0) {

        for(int i = 0; i < 5; i++) {
          player.draw(BLACK);
          player.up();
          player.draw(GREEN);
          scroll_up();
          for(int o = 0; o < NUM_PLATFORMS; o++) {
            if(platforms[o].is_alive()) {
              if(platforms[o].get_type() == 1) platforms[o].draw(LIME);
              else if(platforms[o].get_type() == 2) platforms[o].draw(PURPLE);
              else if(platforms[o].get_type() == 3) platforms[o].draw(AQUA);
              else platforms[o].draw(ORANGE);
            }
          }
          if(enemy.is_alive()) enemy.draw(RED, BLUE);
          if(bullet.is_alive()) bullet.draw(WHITE);
          delay(100);
        }

        loss();
        return;
      }

      player.draw(BLACK);
      //Set player lateral position
      player.set_y(potentiometer_value / 75);

      //PLATFORM UPDATES
      if(time % PLATFORM_SPEED == 0) {
        for(int i = 0; i < NUM_PLATFORMS; i++) {
          if(platforms[i].get_type() == 3) {
            platforms[i].draw(BLACK);
            if((platforms[i].get_y() == 13 || platforms[i].get_y() == 0) && !platforms[i].hasJustFlipped()) {
              platforms[i].flip();
              platforms[i].draw(AQUA);
            }
            else if ((platforms[i].get_y() == 13 || platforms[i].get_y() == 0) && platforms[i].hasJustFlipped()) {
              platforms[i].move();
              platforms[i].draw(AQUA);
              platforms[i].set_justFlipped(false);
            }

            else {
              platforms[i].move();
              platforms[i].draw(AQUA);
            }
            if(enemy.is_alive()) enemy.draw(RED, BLUE);
          }

          if(platforms[i].get_type() == 2 && !platforms[i].is_alive()) {
            if (platforms[i].out_of_time()) {
              platforms[i].respawn();
              platforms[i].draw(PURPLE);
              platforms[i].reset_time();
            } else if (platforms[i].get_time() < 8 && platforms[i].get_time() % 2 == 1) {
              platforms[i].draw(YELLOW);
              platforms[i].decrement_time();
            } else if (platforms[i].get_time() < 7 && platforms[i].get_time() % 2 == 0) {
              platforms[i].draw(BLACK);
              platforms[i].decrement_time();
            } else {
              platforms[i].decrement_time();
            }
          }
        }
      }

      //PLAYER UPDATES
      if(time % PLAYER_SPEED == 0) {


        //If the player is currently falling, decrement the vertical position
        if(player.is_falling()) {
          player.down();
        }

        //If the player is not falling, increment the vertical position
        if(!player.is_falling() && player.get_remainingJump() > 0) {
          player.set_remainingJump(player.get_remainingJump()-1);
          if(player.get_x() == SCROLL_LINE) {
            scroll_down();
            //IF WE ARE DUE TO SPAWN A NEW PLATFORM, FIND THE LOWEST INDEX DEAD PLATFORM AND INITALIZE IT
            if(nextPlatform == 0) {
              int i;
              for(i = 0; i < NUM_PLATFORMS; i++) {
                if((!platforms[i].is_alive() && platforms[i].get_type() != 2) || (!platforms[i].is_alive() && platforms[i].get_type() == 2 && platforms[i].get_x() < 0)) break;
              }
              if(random(15) > 13) platforms[i].initialize(31, random(14), 1);
              else if(random(15) > 13) platforms[i].initialize(31, random(13)+1, 3);
              else if(height < 50 && random(20) > 18) platforms[i].initialize(31, random(14), 2);
              else if(height > 100 && random(5) > 3) platforms[i].initialize(31, random(14), 2);
              else if(height > 160 && random(4) > 2) platforms[i].initialize(31, random(14), 2);
              else if(height > 270 && random(3) > 1) platforms[i].initialize(31, random(14), 2);
              else if(height > 380 && random(6) != 1) platforms[i].initialize(31, random(14), 2);
              else if(height > 500) platforms[i].initialize(31, random(14), 2);
              else platforms[i].initialize(31, random(14), 0);
              nextPlatform = random(2) + 2;
            } else {
              nextPlatform--;
            }

            if(nextEnemy == 0) {
              enemy.initialize(31, random(14));
              nextEnemy = random(20) + 32;
            } else {
              nextEnemy--;
            }
            height++;
            score++;
          }
          else {
            player.up();
          }
        }

        //PLAYER ENEMY COLLISION
        if(player.is_falling()
          && enemy.is_alive()
          && (player.get_x()-2 == enemy.get_x())
          && (player.get_y() >= enemy.get_y()-2 && player.get_y() <= enemy.get_y()+2)) {
            player.flip();
            player.set_remainingJump(MAX_JUMP*3);
            player.up();
            player.draw(GREEN);
            delay(150);
            player.draw(BLACK);
            enemy.die();
            enemy.draw(BLACK, BLACK);
            score+=10;
        }
        else {
          if(enemy.is_alive()
            && (player.get_x() == enemy.get_x()-1)
            && (player.get_y()+1 >= enemy.get_y() && player.get_y()+1 <= enemy.get_y()+2)) {
              player.draw(GREEN);
              delay(25);
              loss();
              return;
          }
          if(enemy.is_alive()) {
            for(int i = 1; i < 3; i++) {
              for(int o = 0; o < 2; o++) {
                if((player.get_x()-i == enemy.get_x()-o)
                  && player.get_y() >= enemy.get_y()-2 && player.get_y() <= enemy.get_y()+2) {
                    player.draw(GREEN);
                    delay(25);
                    loss();
                    return;
                } 
              }
            }
          }
        }

        //If the player has collided with the platform, falling is false and move the player two pixels above the playforms
        for(int i = 0; i < NUM_PLATFORMS; i++) {
          if(platforms[i].is_alive()
            && (player.get_x()-2 == platforms[i].get_x()) 
            && (player.get_y() > platforms[i].get_y()-3)
            && (player.get_y() < platforms[i].get_y()+3) 
            && (player.is_falling())) {
            player.flip();
            if(platforms[i].get_type() == 1) {
              player.set_remainingJump(MAX_JUMP*2);
            } else if(platforms[i].get_type() == 2) {
              player.set_remainingJump(MAX_JUMP);
              platforms[i].die(); 
              platforms[i].draw(BLACK);
            } else {
              player.set_remainingJump(MAX_JUMP);
            }
            player.up();
            player.draw(GREEN);
            delay(75);
            player.draw(BLACK);
          }
        }

        if(!player.is_falling() && player.get_remainingJump() == 0) {
          for(int i = 0; i < NUM_PLATFORMS; i++) {
            if(platforms[i].is_alive()) {
              if(platforms[i].get_type() == 1) platforms[i].draw(LIME);
              else if(platforms[i].get_type() == 2) platforms[i].draw(PURPLE);
              else if(platforms[i].get_type() == 3) platforms[i].draw(AQUA);
              else platforms[i].draw(ORANGE);
              if(enemy.is_alive()) enemy.draw(RED, BLUE);
            }
          }
          player.flip();
          player.draw(GREEN);
          delay(75);
        }

        //REDRAW PLATFORMS IN CASE THE PLAYER WENT ON TOP OF ONE OF THEM
        for(int i = 0; i < NUM_PLATFORMS; i++) {
          if(platforms[i].is_alive()) {
            if(platforms[i].get_type() == 1) platforms[i].draw(LIME);
            else if(platforms[i].get_type() == 2) platforms[i].draw(PURPLE);
            else if(platforms[i].get_type() == 3) platforms[i].draw(AQUA);
            else platforms[i].draw(ORANGE);

            if(enemy.is_alive()) enemy.draw(RED, BLUE);
          }
        }
      }
      player.draw(GREEN);

      //BULLET UPDATES
      if(time % BULLET_SPEED == 0) {


        //BULLET-ENEMY Collision
        if(bullet.is_alive()) {
          bullet.draw(BLACK);
          bullet.up();
          if(enemy.is_alive()
            && ((bullet.get_x() == enemy.get_x()-1) || (bullet.get_x() == enemy.get_x()))
            && (bullet.get_y() >= enemy.get_y() && bullet.get_y() < enemy.get_y()+3)) {
              bullet.die();
              bullet.draw(BLACK);
              enemy.die();
              enemy.draw(BLACK, BLACK);
              score+=30;
          } 
          else {
            bullet.draw(WHITE);
          }
        }

        if(button_pressed && !bullet.is_alive()) {
          bullet.initialize(player.get_x()+1, player.get_y()+1);
          bullet.draw(WHITE);
        }
      }
      delay(20);
      time++;
    }


  private:
    Enemy enemy;
    Platform platforms[NUM_PLATFORMS];
    Player player;
    Bullet bullet;
    int time;
    int nextPlatform;
    int nextEnemy;
    int score;
    int height;
    bool playing;
    // REMEMBER - SCROLL ONLY HAPPENS when the player HITS THE LINE
    void scroll_down() {
      if(enemy.is_alive()) {
        enemy.draw(BLACK, BLACK);
        enemy.down();
        enemy.draw(RED, BLUE);
        if(enemy.get_x() < 0) {
          enemy.die();
        }
      }
      if(bullet.is_alive()) {
        bullet.draw(BLACK);
        bullet.down();
      }
      for(int i = 0; i < NUM_PLATFORMS; i++) {
        if(platforms[i].is_alive() || platforms[i].get_type() == 2) {
          platforms[i].draw(BLACK);
          platforms[i].down();
          if(platforms[i].get_x() < 0) {
            platforms[i].die();
         }
        }
      }
    }

    void scroll_up() {
      if(enemy.is_alive()) {
        enemy.draw(BLACK, BLACK);
        enemy.up();
        if(enemy.get_x() > 31) {
          enemy.die();
        }
      }
      if(bullet.is_alive()) {
        bullet.draw(BLACK);
        bullet.up();
      }
      for(int i = 0; i < NUM_PLATFORMS; i++) {
        if(platforms[i].is_alive() || platforms[i].get_type() == 2) {
          platforms[i].draw(BLACK);
          platforms[i].up();
          if(platforms[i].get_x() > 31) {
            platforms[i].die();
         }
        }
      }
    }

    void loss() {
      bool thingsAlive = true;
        while(thingsAlive) {
          thingsAlive = false;
          scroll_up();
          for(int o = 0; o < NUM_PLATFORMS; o++) {
            if(platforms[o].is_alive()) {
              thingsAlive = true;
              if(platforms[o].get_type() == 1) platforms[o].draw(LIME);
              else if(platforms[o].get_type() == 2) platforms[o].draw(PURPLE);
              else if(platforms[o].get_type() == 3) platforms[o].draw(AQUA);
              else platforms[o].draw(ORANGE);
            }
          }
          if(enemy.is_alive()) enemy.draw(RED, BLUE);
          if(bullet.is_alive()) bullet.draw(WHITE);
          player.draw(GREEN);
          delay(70);
        }

        while(player.get_x() < 32) {
          player.draw(BLACK);
          player.up();
          player.draw(GREEN);
          delay(70);
        }
        matrix.setTextColor(WHITE);
        matrix.fillScreen(BLACK);
        matrix.setCursor(0, 0);
        matrix.print("SCORE");
        matrix.setCursor(0, 8);
        matrix.print(score);
        delay(3000);
        matrix.fillScreen(BLACK);
        playing = false;
        time = 0;
    }


    void reset_level() {
      player.set_x(4);
      matrix.fillScreen(BLACK);
      platforms[0].initialize(0, 0);
      platforms[1].initialize(0, 3);
      platforms[2].initialize(0, 6);
      platforms[3].initialize(0, 9);
      platforms[4].initialize(0, 12);
      platforms[5].initialize(0, 15);
      int i, n;
      for(i = 1, n = 0; i < 32; n++) {
        if(n > 5) {
          i += random(2) + 2;
          if(random(10) > 8) {
            platforms[n].initialize(i, random(14), 1);
            platforms[n].draw(LIME);
          }
          else if(random(10) > 8) {
            platforms[n].initialize(i, random(13)+1, 3);
            platforms[n].draw(AQUA);
          }
          else {
            platforms[n].initialize(i, random(14), 0);
            platforms[n].draw(ORANGE);
          }
        } else {
          platforms[n].draw(ORANGE);
        }
      }
      nextPlatform = i - 30;
      nextEnemy = 45;
      enemy.initialize(26, random(13));
      enemy.draw(RED, BLUE);
    }
};

Game g;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
  g.setup_game();
}

void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
  g.update(potentiometer_value, button_pressed);
}