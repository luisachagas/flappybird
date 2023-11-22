#include "screen.h"
#include <stdlib.h>

void screenInit() {
  screenClear();
  screenHideCursor();
}

void screenDestroy() {
  screenShowCursor();
  screenClear();
}

void screenUpdate() {
  // Aqui você pode adicionar qualquer lógica necessária para atualizar a tela
  // Por exemplo, se precisar atualizar mais elementos na tela, faça isso aqui.
  // Se não for necessário por enquanto, deixe vazia.
}