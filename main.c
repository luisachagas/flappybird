#include "keyboard.h"
#include "screen.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_HEIGHT 12
#define MIN_HEIGHT 5

const int MAX_OBSTACLES = 8;
int groundLength = SCREEN_WIDTH;

typedef struct node {
  int birdX;
  int birdY;
  int velocity;
  int gravity;
  int score;
  struct node *next;
} Master;

typedef struct {
  int x;
  int y;
  int width;
  int height;
  int speed;
  int transition; // Nova variável para controlar a transição
} Obstacle;

void desenhar_passaro(int x, int y);
void desenhar_obstaculo(Obstacle *obstacle);
void processar_entrada(Master *head);
void atualizar_passaro(Master *head, Obstacle obstacles[MAX_OBSTACLES]);
int verificar_colisao(Master *head, Obstacle obstacle);
int altura_aleatoria();
void mover_obstaculo(Obstacle *obstacle, int dx, int dy);
void desenhar_chao();
void score(Master *head, Obstacle obstacles[MAX_OBSTACLES]);

int main() { 
  Master *head = NULL;
  Master *temp = (Master *)malloc(sizeof(Master));

  if (temp != NULL) {
    temp->birdX = SCREEN_WIDTH / 4;
    temp->birdY = SCREEN_HEIGHT / 2;
    temp->velocity = 0;
    temp->gravity = 1;
    temp ->score = 0;
    temp->next = NULL;

    head = temp;

    screenInit(1);
    keyboardInit();
    timerInit(50);

    Obstacle obstacles[MAX_OBSTACLES];

    for (int i = 0; i < MAX_OBSTACLES; ++i) {
      obstacles[i].x = SCREEN_WIDTH + i * 10;

      if (rand() % 2 == 0) {
        obstacles[i].y = SCREEN_HEIGHT - altura_aleatoria();
      } else {
        obstacles[i].y = 0;
      }

      obstacles[i].width = 3;
      obstacles[i].height = altura_aleatoria();
      obstacles[i].speed = 1;
      obstacles[i].transition = 0;
    }
    while (1) {
      screenClear();
      desenhar_passaro(head->birdX, head->birdY);
      

      for (int i = 0; i < MAX_OBSTACLES; ++i) {
        mover_obstaculo(&obstacles[i], -1, 0);
        desenhar_obstaculo(&obstacles[i]);
      }

      desenhar_chao();
      score(head, obstacles);
      atualizar_passaro(head, obstacles);
      processar_entrada(head);

      screenUpdate();

      timerSleep(100);
    }
  }
  keyboardDestroy();
  screenDestroy();
  timerDestroy();
  free(temp);
  return 0;
}

void atualizar_passaro(Master *head, Obstacle obstacles[MAX_OBSTACLES]) {
  head->velocity += head->gravity;
  head->birdY += head->velocity;

  if (head->birdY < 0) {
    head->birdY = 0;
    head->velocity = 0;
  }
  if (head->birdY >= SCREEN_HEIGHT) {
    head->birdY = SCREEN_HEIGHT - 1;
    head->velocity = 0;
  }
  for (int i = 0; i < MAX_OBSTACLES; ++i) {
    if (verificar_colisao(head, obstacles[i])) {
      exit(0);
    }
  }
}

int verificar_colisao(Master *head, Obstacle obstacle) {
  if (head->birdX < obstacle.x + obstacle.width &&
      head->birdX + 2 > obstacle.x &&
      head->birdY < obstacle.y + obstacle.height &&
      head->birdY + 1 > obstacle.y) {
    screenClear();
    printf("\n\nColisao com obstaculo!\n\nFim do jogo.");
    printf("\n\nPontuacao final: %d\n", head->score);
    return 1;
  }
    if (head->birdY == MINY){
      screenClear();
      printf("\n\nColisao com o teto!\n\nFim do jogo.\n");
      printf("\n\nPontuacao final: %d\n", head->score);
      return 1;
    }else if (head->birdY < MIN_HEIGHT && head->birdY < MINY){
      //verifica se a posição vertical do pássaro é menor que a altura mínima permitida e menor que a posição mínima permitida
        screenClear(); 
        printf("\n\nColisao com o chao!\n\nFim do jogo.\n");
        printf("\n\nPontuacao final: %d\n", head->score);
        return 1;
    }else if(head->birdY < MIN_HEIGHT && head->birdY >= SCREEN_HEIGHT){ // verifica se a posição vertical do pássaro é menor que a posição vertical inicial do pássaro e maior ou igual à altura da tela
      screenClear();
      printf("\n\nColisao com o chao!\n\nFim do jogo.\n");
      printf("\n\nPontuacao final: %d\n", head->score);
    return 1;
    }
  return 0;
}

void processar_entrada(Master *head) {
  if (keyhit()) {
    char key = readch();
    if (key == ' ') {
      head->velocity = -3;
    }
  }
}

void desenhar_passaro(int x, int y) {
  screenGotoxy(x, y);
  screenSetColor(YELLOW, DEFAULT_COLOR);
  printf("O>");
}

void desenhar_obstaculo(Obstacle *obstacle) {
  int opacity = 255 - obstacle->transition * 20;

  screenSetColor(GREEN, BLACK);

  for (int i = 0; i < obstacle->height; i++) {
    int yPos;

    if (obstacle->y > SCREEN_HEIGHT / 2) {
      yPos = SCREEN_HEIGHT - obstacle->height + i;
    } else {
      yPos = i;
    }

    if (yPos >= 0 && yPos < SCREEN_HEIGHT) {
      screenGotoxy(obstacle->x, yPos);
      for (int j = 0; j < obstacle->width; j++) {
        printf("#");
      }
    }
  }

  screenSetColor(DEFAULT_COLOR, DEFAULT_COLOR);
}

int altura_aleatoria() {
  return rand() % (MAX_HEIGHT - MIN_HEIGHT + 1) + MIN_HEIGHT;
}

void mover_obstaculo(Obstacle *obstacle, int dx, int dy) {
  obstacle->x += dx;
  obstacle->y += dy;
  obstacle->transition += 1;

  if (obstacle->x + obstacle->width <= 0) {
    obstacle->x = SCREEN_WIDTH + rand() % 10;
    obstacle->height = altura_aleatoria();
    obstacle->transition = 0;
  }
}

void desenhar_chao() {
  screenSetColor(BLACK, BLACK);

  for (int i = 0; i < groundLength; i++) {
    screenGotoxy(i, SCREEN_HEIGHT);
    printf("=");
  }

  screenSetColor(DEFAULT_COLOR, DEFAULT_COLOR);
}

void score(Master *head, Obstacle obstacles[MAX_OBSTACLES]) {
    for (int i = 0; i < MAX_OBSTACLES; ++i) {
      if (obstacles[i].x + obstacles[i].width <= head->birdX) {
        // O pássaro passou com sucesso por este obstáculo
        head->score++;
      }
    }

    printf("\n\nPontuacao: %d\n", head->score);
  }
