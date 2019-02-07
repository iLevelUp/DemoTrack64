/*!\file animations.c
 *
 * \brief Votre espace de liberté : c'est ici que vous pouvez ajouter
 * vos fonctions de transition et d'animation avant de les faire
 * référencées dans le tableau _animations du fichier \ref window.c
 *
 * Des squelettes d'animations et de transitions sont fournis pour
 * comprendre le fonctionnement de la bibliothèque. En bonus des
 * exemples dont un fondu en GLSL.
 *
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date May 05, 2014
 */

#include <GL4D/gl4dh.h>
#include "audioHelper.h"
#include <assert.h>
#include <stdlib.h>
#include <GL4D/gl4dg.h>
#include <SDL_image.h>

/*!\brief identifiant de la géométrie QUAD GL4Dummies */
static GLuint _quadId = 0;

void audio_analyzer(int state) {
 /* INITIALISEZ VOS VARIABLES */ 
static GLuint screen = 0;
 int l, i;
 Sint16 * s;
 static Sint16 t[256];
 static int w, h; 
switch(state) { 
case GL4DH_INIT: /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */ 
screen = gl4dpInitScreen(); 
w = gl4dpGetWidth(); 
h = gl4dpGetHeight(); 
return; 
case GL4DH_FREE: /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */ 
if(screen) { 
gl4dpSetScreen(screen); 
gl4dpDeleteScreen(); 
screen = 0; }
 return; 
case GL4DH_UPDATE_WITH_AUDIO: /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
 s = (Sint16 *)ahGetAudioStream();
 l = ahGetAudioStreamLength();
 if(l >= 2 * 256)
 for(i = 0; i < 256; i++) t[i] = h / 2 + (h / 2) * s[i] / ((1 << 15) - 1.0); 
return; default: /* GL4DH_DRAW */ /* JOUER L'ANIMATION */ 
gl4dpSetScreen(screen); 
gl4dpClearScreen(); 
for(i = 0; i < 256 - 1; i++) { 
  int x0, y0, x1, y1;
   x0 = (i * (w - 1)) / (256 - 1); 
  y0 = t[i]; 
  x1 = ((i + 1) * (w - 1)) / (256 - 1); 
  y1 = t[i + 1]; 
  gl4dpSetColor(rand());
  gl4dpLine(x0, y0, x1, y1);
 } 
gl4dpUpdateScreen(NULL); 



return; 
} 
}

void exemple_de_transition_00(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state) {
  /* INITIALISEZ VOS VARIABLES */
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE TRANSITION (SES VARIABLES <STATIC>s) */
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR LES DEUX ANIMATIONS EN FONCTION DU SON */
    if(a0) a0(state);
    if(a1) a1(state);
    return;
  default: /* GL4DH_DRAW */
    /* JOUER LES DEUX ANIMATIONS */
    if(a0) a0(state);
    if(a1) a1(state);
    /* MIXER LES DEUX ANIMATIONS */
    return;
  }
}

void exemple_de_transition_01(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state) {
  /* INITIALISEZ VOS VARIABLES */
  int vp[4], i;
  GLint tId;
  static GLuint tex[2], pId;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE TRANSITION (SES VARIABLES <STATIC>s) */
    glGetIntegerv(GL_VIEWPORT, vp);
    glGenTextures(2, tex);
    for(i = 0; i < 2; i++) {
      glBindTexture(GL_TEXTURE_2D, tex[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vp[2], vp[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/mix.fs", NULL);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    if(tex[0]) {
      glDeleteTextures(2, tex);
      tex[0] = tex[1] = 0;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR LES DEUX ANIMATIONS EN FONCTION DU SON */
    if(a0) a0(state);
    if(a1) a1(state);
    return;
  default: /* GL4DH_DRAW */
    /* RECUPERER L'ID DE LA DERNIERE TEXTURE ATTACHEE AU FRAMEBUFFER */
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &tId);
    /* JOUER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[0],  0);
    if(a0) a0(state);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[1],  0);
    if(a1) a1(state);
    /* MIXER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tId,  0);
    glDisable(GL_DEPTH);
    glUseProgram(pId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    if(et / (GLfloat)t > 1) {
      fprintf(stderr, "%d-%d -- %f\n", et, t, et / (GLfloat)t);
      exit(0);
    }
    glUniform1f(glGetUniformLocation(pId, "dt"), et / (GLfloat)t);
    glUniform1i(glGetUniformLocation(pId, "tex0"), 0);
    glUniform1i(glGetUniformLocation(pId, "tex1"), 1);
    gl4dgDraw(_quadId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    return;
  }
}
void exemple_de_transition_02(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state) {
  /* INITIALISEZ VOS VARIABLES */
  int vp[4], i;
  GLint tId;
  static GLuint tex[2], pId;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE TRANSITION (SES VARIABLES <STATIC>s) */
    glGetIntegerv(GL_VIEWPORT, vp);
    glGenTextures(2, tex);
    for(i = 0; i < 2; i++) {
      glBindTexture(GL_TEXTURE_2D, tex[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vp[2], vp[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/titrage.fs", NULL);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    if(tex[0]) {
      glDeleteTextures(2, tex);
      tex[0] = tex[1] = 0;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR LES DEUX ANIMATIONS EN FONCTION DU SON */
    if(a0) a0(state);
    if(a1) a1(state);
    return;
  default: /* GL4DH_DRAW */
    /* RECUPERER L'ID DE LA DERNIERE TEXTURE ATTACHEE AU FRAMEBUFFER */
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &tId);
    /* JOUER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[0],  0);
    if(a0) a0(state);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[1],  0);
    if(a1) a1(state);
    /* MIXER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tId,  0);
    glDisable(GL_DEPTH);
    glUseProgram(pId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    if(et / (GLfloat)t > 1) {
      fprintf(stderr, "%d-%d -- %f\n", et, t, et / (GLfloat)t);
      exit(0);
    }
    glUniform1f(glGetUniformLocation(pId, "dt"), et / (GLfloat)t);
    glUniform1i(glGetUniformLocation(pId, "tex0"), 0);
    glUniform1i(glGetUniformLocation(pId, "tex1"), 1);
    gl4dgDraw(_quadId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    return;
  }
}
static void loadTexture(GLuint id, const char * filename) {
  SDL_Surface * t;
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if( (t = IMG_Load(filename)) != NULL ) {
#ifdef __APPLE__
    int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
#else
    int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
#endif       
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, mode, GL_UNSIGNED_BYTE, t->pixels);
    SDL_FreeSurface(t);
  } else {
    fprintf(stderr, "can't open file %s : %s\n", filename, SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  }
}
void transition_fondu(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state) {
  //* INITIALISEZ VOS VARIABLES */
  int vp[4], i;
  GLint tId;
  static GLuint tex[3], pId;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE TRANSITION (SES VARIABLES <STATIC>s) */
    glGetIntegerv(GL_VIEWPORT, vp);
    glGenTextures(3, tex);
    for(i = 0; i < 3; i++) {
      glBindTexture(GL_TEXTURE_2D, tex[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vp[2], vp[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    loadTexture(tex[2], "image/corps.png");
    pId = gl4duCreateProgram("<vs>shaders/basic.vs", "<fs>shaders/mixi.fs", NULL);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    if(tex[0]) {
      glDeleteTextures(3, tex);
      tex[0] = tex[1] = 0;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR LES DEUX ANIMATIONS EN FONCTION DU SON */
    if(a0) a0(state);
    if(a1) a1(state);
    return;
  default: /* GL4DH_DRAW */
    /* RECUPERER L'ID DE LA DERNIERE TEXTURE ATTACHEE AU FRAMEBUFFER */
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &tId);
    /* JOUER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[0],  0);
    if(a0) a0(state);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tex[1],  0);
    if(a1) a1(state);
    /* MIXER LES DEUX ANIMATIONS */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,  tId,  0);
    glDisable(GL_DEPTH);
    glUseProgram(pId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex[2]);
    if(et / (GLfloat)t > 1) {
      fprintf(stderr, "%d-%d -- %f\n", et, t, et / (GLfloat)t);
      exit(0);
    }
    glUniform1f(glGetUniformLocation(pId, "dt"), et / (GLfloat)t);
    glUniform1i(glGetUniformLocation(pId, "tex0"), 0);
    glUniform1i(glGetUniformLocation(pId, "tex1"), 1);
    glUniform1i(glGetUniformLocation(pId, "tex2"), 2);
    gl4dgDraw(_quadId);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    return;
  }
}

void animation_flash(int state) {
  /* INITIALISEZ VOS VARIABLES */
  int l, i;
  Uint16 * s;
  static GLfloat c[4] = {0, 0, 0, 0};
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    s = (Uint16 *)ahGetAudioStream();
    l = ahGetAudioStreamLength();
    if(l >= 8)
      for(i = 0; i < 4; i++)
  c[i] = s[i] / ((1 << 16) - 1.0);
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    glClearColor(c[0], c[1], c[2], c[3]);
    glClear(GL_COLOR_BUFFER_BIT);
    return;
  }
}



void exemple_d_animation_03(int state) {
  /* INITIALISEZ VOS VARIABLES */
  int l, i;
  Sint16 * s;
  GLint rect[4], tr, mr;
  static int r = 0, or = 0;
  static GLuint screen_id;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    screen_id = gl4dpInitScreen();
    gl4dpUpdateScreen(NULL);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    s = (Sint16 *)ahGetAudioStream();
    l = ahGetAudioStreamLength();
    for(i = 0, tr = 0; i < l >> 1; i++)
      tr += abs(s[i]);
    tr /= l >> 1;
    r = 100 + (tr >> 6);
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    tr = r;
    mr = MAX(tr, or) + 1;
    gl4dpSetScreen(screen_id);
    rect[0] = MAX((gl4dpGetWidth()  >> 1) - mr, 0);
    rect[1] = MAX((gl4dpGetHeight() >> 1) - mr, 0);
    rect[2] = MIN(mr << 1,  gl4dpGetWidth() - rect[0]);
    rect[3] = MIN(mr << 1, gl4dpGetHeight() - rect[1]);
    gl4dpSetColor(RGB(0, 0, 0));
    gl4dpRect(rect);
    gl4dpSetColor(RGB(255, 255, 255));
    gl4dpFilledCircle(gl4dpGetWidth() >> 1, gl4dpGetHeight() >> 1, tr);
    gl4dpUpdateScreen(rect);
    or = tr;
    return;
  }
}

void exemple_d_animation_04(int state) {
  /* INITIALISEZ VOS VARIABLES */
  int l, i;
  Sint16 * s;
  GLint tr;
  static GLint r = 0;
  static GLuint s1, s2;
  static GLfloat r1[] = {0.0, 0.0, 1.0, 1.0}, r2[] = {0, 0, 1, 1}, rf = 0.0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    s1 = gl4dpInitScreen();
    s2 = gl4dpInitScreen();
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    s = (Sint16 *)ahGetAudioStream();
    l = ahGetAudioStreamLength();
    for(i = 0, tr = 0; i < l >> 1; i++)
      tr += abs(s[i]);
    tr /= l >> 1;
    r = 100 + (tr >> 5);
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    tr = r;
    gl4dpSetScreen(s2);
    gl4dpClearScreen();
    for(i = 0; i < tr; i++) {
      Uint8 r = rand()&255, g = rand()&255, b = rand()&255;
      gl4dpSetColor(RGB(r, g, b));
      gl4dpCircle(gl4dpGetWidth() >> 2, gl4dpGetHeight() >> 1, i);
    }
    gl4dpSetColor(RGB(255, 255, 255));
    gl4dpFilledCircle(gl4dpGetWidth() * 3 / 4, gl4dpGetHeight() >> 1, i);
    gl4dpUpdateScreen(NULL);
    rf += 0.05;
    gl4dpSetScreen(s1);
    gl4dpClearScreen();
    gl4dpMap(s1, s2, r1, r2, rf);
    gl4dpUpdateScreen(NULL);
    return;
  }
}

/* exemple simpliste de recopie depuis une surface sdl vers le screen en cours */
static void copyFromSurface(SDL_Surface * s, int x0, int y0) {
  Uint32 * p, coul, ocoul;
  Uint8 rmix, gmix, bmix;
  double f1, f2;
  int x, y, maxx, maxy;
  assert(s->format->BytesPerPixel == 4); /* pour simplifier, on ne gère que le format RGBA */
  p = s->pixels;
  maxx = MIN(x0 + s->w, gl4dpGetWidth());
  maxy = MIN(y0 + s->h, gl4dpGetHeight());
  for(y = y0; y < maxy; y++) {
    for(x = x0; x < maxx; x++) {
      ocoul = gl4dpGetPixel(x, y);
      coul = p[(s->h - 1 - (y - y0)) * s->w + x - x0]; /* axe y à l'envers */
      f1 = ALPHA(coul) / 255.0; f2 = 1.0 - f1;
      rmix = f1 *   RED(coul) + f2 *   RED(ocoul);
      gmix = f1 * GREEN(coul) + f2 * GREEN(ocoul);
      bmix = f1 *  BLUE(coul) + f2 *  BLUE(ocoul);
      gl4dpSetColor(RGB(rmix, gmix, bmix));
      gl4dpPutPixel(x, y);
    }
  }
}

void exemple_d_animation_00(int state) {
  /* INITIALISEZ VOS VARIABLES */
  int l, i;
  Uint16 * s;
  static GLfloat c[4] = {0, 0, 0, 0};
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    s = (Uint16 *)ahGetAudioStream();
    l = ahGetAudioStreamLength();
    if(l >= 8)
      for(i = 0; i < 4; i++)
  c[i] = s[i] / ((1 << 16) - 1.0);
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    glClearColor(c[0], c[1], c[2], c[3]);
    glClear(GL_COLOR_BUFFER_BIT);
    return;
  }
}
void exemple_d_animation_05(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/drugs.jpg");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFFFFFFFF);
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite, rand() % gl4dpGetWidth(),rand() % gl4dpGetHeight());
    gl4dpUpdateScreen(NULL);
    return;
  }
}

void exemple_d_animation_005(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/larme.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);//0xFF0000FF 0xFF000080
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_07(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/larme3.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0x00FF00FF);
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_09(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/larmeM.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();//0x551A8B
    gl4dpClearScreenWith(0xFFB394);
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}

void exemple_d_animation_06(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {0.0, 0.0, 0.8, 0.3}, r2[] = {0, 0, 1, 1}, r = 0.0;
  static SDL_Surface * sprite = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    sprite = IMG_Load("image/car1.png");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFFFFFFFF);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(sprite, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    ///r1[0] += 0.01;
    r1[2] += 0.01;
    //r -= 0.02;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}
void exemple_d_animation_061(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/lang9.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();//0x551A8B
    gl4dpClearScreenWith(0xFFB394);
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/14,gl4dpGetHeight()/4);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_062(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {0.0, 0.0, 0.8, 0.3}, r2[] = {0, 0, 1, 1}, r = 0.0;
  static SDL_Surface * sprite = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    sprite = IMG_Load("image/car.png");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(sprite, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */

    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);
    r1[0] += 0.01;
    r -= 0.02;
    gl4dpMap(s1, s2, r1, r2, r);
    return;
  }
}


void animation_b(int state) {
  switch(state) {
  case GL4DH_INIT:
    cube_init();
    return;
  case GL4DH_FREE:
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    return;
  default: /* GL4DH_DRAW */
    cube_draw();
    return;
  }
}
void exemple_d_animation_sit(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/Sit.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);//0xFF0000FF 0xFF000080
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_sit2(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/Sit2.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);//0xFF0000FF 0xFF000080
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_sit3(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/Sit3.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);//0xFF0000FF 0xFF000080
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_sit4(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/Sit4.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);//0xFF0000FF 0xFF000080
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_sit5(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/Sit5.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);//0xFF0000FF 0xFF000080
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_sit6(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/Sit6.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);//0xFF0000FF 0xFF000080
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_sit7(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/Sit7.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);//0xFF0000FF 0xFF000080
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_sit8(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/Sit8.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);//0xFF0000FF 0xFF000080
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_sit9(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/Sit10.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);//0xFF0000FF 0xFF000080
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_flower(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/hand.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);//0xFF0000FF 0xFF000080
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_flower2(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/hand2.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFF000000);//0xFF0000FF 0xFF000080
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite,gl4dpGetWidth()/2.8,gl4dpGetHeight()/3.5);
    gl4dpUpdateScreen(NULL);
    return;
  }
}



void exemple_d_animation_006(int state) {
  /* INITIALISEZ VOS VARIABLES */
  const GLfloat s[2] = {1.0, -1.0}, t[2] = {0.0, 1.0};
  static GLfloat r1[] = {0.0, 0.2, 0.25, 0.75}, r2[] = {0, 0, 1, 1}, r = 0.0;
  static SDL_Surface * sprite = NULL;
  static GLuint s1 = 0, s2 = 0;
  switch(state) {
  case GL4DH_INIT:
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    sprite = IMG_Load("image/larme.png");
    s1 = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFFFFFFFF);
    s2 = gl4dpInitScreen();
    gl4dpCopyFromSDLSurfaceWithTransforms(sprite, s, t);
    gl4dpMap(s1, s2, r1, r2, 0.0);
    return;
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(s1);
    gl4dpDeleteScreen();
    gl4dpSetScreen(s2);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(s1);
    gl4dpUpdateScreen(NULL);



    gl4dpMap(s1, s2, r1, r2, r);
//}
    return;
  }
}


void animation_earth4(int state) {
  switch(state) {
  case GL4DH_INIT:
    earth4_init();
    return;
  case GL4DH_FREE:
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    return;
  default: /* GL4DH_DRAW */
    earth4_draw();
    return;
  }
}


void animation_earth6(int state) {
  switch(state) {
  case GL4DH_INIT:
    earth6_init();
    return;
  case GL4DH_FREE:
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    return;
  default: /* GL4DH_DRAW */
    earth6_draw();
    return;
  }
}


void exemple_d_animation_12(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("image/Fem2.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFFFFFFFF);
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite, rand()% gl4dpGetWidth(), rand()% gl4dpGetHeight());
    gl4dpUpdateScreen(NULL);
    return;
  }
}


void exemple_d_animation_13(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("shooting.jpeg");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFFFFFFFF);
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite, rand()% gl4dpGetWidth(), rand()% gl4dpGetHeight());
    gl4dpUpdateScreen(NULL);
    return;
  }
}

void exemple_d_animation_14(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("ddd_duel.jpg");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFFFFFFFF);
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite, rand()% gl4dpGetWidth(), rand()% gl4dpGetHeight());
    gl4dpUpdateScreen(NULL);
    return;
  }
}
void exemple_d_animation_15(int state) {
  /* INITIALISEZ VOS VARIABLES */
  static SDL_Surface * sprite = NULL;
  static GLuint screen_id = 0;
  switch(state) {
  case GL4DH_INIT: {
    /* INITIALISEZ VOTRE ANIMATION (SES VARIABLES <STATIC>s) */
    SDL_Surface * s = IMG_Load("duel.png");
    sprite = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(s);
    screen_id = gl4dpInitScreen();
    gl4dpClearScreenWith(0xFFFFFFFF);
    return;
  }
  case GL4DH_FREE:
    /* LIBERER LA MEMOIRE UTILISEE PAR LES <STATIC>s */
    gl4dpSetScreen(screen_id);
    gl4dpDeleteScreen();
    if(sprite) {
      SDL_FreeSurface(sprite);
      sprite = NULL;
    }
    return;
  case GL4DH_UPDATE_WITH_AUDIO:
    /* METTRE A JOUR VOTRE ANIMATION EN FONCTION DU SON */
    return;
  default: /* GL4DH_DRAW */
    /* JOUER L'ANIMATION */
    gl4dpSetScreen(screen_id);
    copyFromSurface(sprite, rand()% gl4dpGetWidth(), rand()% gl4dpGetHeight());
    gl4dpUpdateScreen(NULL);
    return;
  }
}


void animationsInit(void) {
  if(!_quadId)
    _quadId = gl4dgGenQuadf();
}
