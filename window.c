/*!\file window.c
 *
 * \brief Utilisation de GL4Dummies pour réaliser une démo.
 *
 * Ici est géré l'ouverture de la fenêtre ainsi que l'ordonnancement
 * des animations. Apriori les seuls éléments à modifier ici lors de
 * votre intégration sont le tableau static \ref _animations et le nom
 * du fichier audio à lire.
 *
 * \author Loïc LOKO, lloicloko@gmail.com
 * \author réalisé sous la base de Farès Belhadj amsi@ai.univ-paris8.fr
 * \date February 26, 2017
 */
#include <stdlib.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4dh.h>
#include <GL4D/gl4duw_SDL2.h>
#include "animations.h"
#include <SDL2/SDL_ttf.h>
#include "audioHelper.h"

/* Prototypes des fonctions statiques contenues dans ce fichier C. */
static void init(void);
static void quit(void);
static void resize(int w, int h);
static void keydown(int keycode);

/*!\brief tableau contenant les animations sous la forme de timeline,
 * ce tableau se termine toujours par l'élémént {0, NULL, NULL,
 * NULL} */


static GL4DHanime _animations[] = {

  { 2000,exemple_d_animation_005, NULL,NULL},
  { 2000, exemple_d_animation_07, NULL, NULL },
  { 2000, exemple_d_animation_09, NULL, NULL },
  { 2000, exemple_d_animation_005, NULL, NULL },
  { 2000, exemple_d_animation_07, NULL, NULL },
  { 2000, exemple_d_animation_09, NULL, NULL },

  { 5000, audio_analyzer, NULL, exemple_de_transition_00},
  { 500, audio_analyzer, exemple_d_animation_06, exemple_de_transition_01},
  { 2200, audio_analyzer, NULL, exemple_de_transition_00},
  { 1000, audio_analyzer, exemple_d_animation_061, exemple_de_transition_01},
  { 200, audio_analyzer, NULL, exemple_de_transition_00},
  

  {500, audio_analyzer, animation_flash, exemple_de_transition_01},

  { 1000, exemple_d_animation_06, NULL, NULL },    
  { 5000, exemple_d_animation_05, NULL, NULL },
  { 3000, animation_flash, NULL, NULL },

  { 500, exemple_d_animation_005,exemple_d_animation_07,exemple_de_transition_01},
  { 166.66, exemple_d_animation_09,NULL,exemple_de_transition_01},
  { 500, exemple_d_animation_005,exemple_d_animation_07,exemple_de_transition_01},
  { 166.66, exemple_d_animation_09,NULL,exemple_de_transition_01},
  { 500, exemple_d_animation_005,exemple_d_animation_07,exemple_de_transition_01},
  { 166.66, exemple_d_animation_09,NULL,exemple_de_transition_01},
  { 500, exemple_d_animation_005,exemple_d_animation_07,exemple_de_transition_01},
  { 166.66, exemple_d_animation_09,NULL,exemple_de_transition_01},
  { 5000, exemple_d_animation_03, NULL, NULL },
  { 5000, exemple_d_animation_12,NULL  , NULL },
  { 1000, exemple_d_animation_062,NULL  , NULL },
  { 2000, animation_flash,exemple_d_animation_062, transition_fondu },
  {500, audio_analyzer, animation_flash, transition_fondu},
  { 500, exemple_d_animation_sit,NULL  , exemple_de_transition_01 },
  { 500, exemple_d_animation_sit2,NULL  , exemple_de_transition_01 },
  { 500, exemple_d_animation_sit3,NULL  , exemple_de_transition_01 },
  { 500, exemple_d_animation_sit4,NULL  , exemple_de_transition_01 },
  { 500, exemple_d_animation_sit5,NULL  , exemple_de_transition_01 },
  { 500, exemple_d_animation_sit6,NULL  , exemple_de_transition_01 },
  { 500, exemple_d_animation_sit7,NULL  , exemple_de_transition_01 },
  { 500, exemple_d_animation_sit8,NULL  , exemple_de_transition_01 },
  { 500, exemple_d_animation_sit9,NULL  , exemple_de_transition_01 },
  
  { 1000, exemple_d_animation_062,NULL  , NULL },
  { 500, exemple_d_animation_sit,NULL  , NULL },
  { 500, exemple_d_animation_sit2,NULL  , NULL },
  { 500, exemple_d_animation_sit3,NULL  , NULL },
  { 500, exemple_d_animation_sit4,NULL  , NULL },
  { 500, exemple_d_animation_sit5,NULL  , NULL },
  { 500, exemple_d_animation_sit6,NULL  , NULL },
  { 500, exemple_d_animation_sit7,NULL  , NULL },
  { 500, exemple_d_animation_sit8,NULL  , NULL },
  { 500, exemple_d_animation_sit9,NULL  , transition_fondu },
  {8000, animation_b, NULL, NULL }, // multicub
  {3000, animation_b,NULL, transition_fondu },
  {7000, animation_earth6, NULL, transition_fondu },
  { 25000,animation_earth4,NULL,  },
  { 2800, exemple_d_animation_00, exemple_d_animation_03, exemple_de_transition_01 },
  {12000, exemple_d_animation_03, NULL, transition_fondu },
  { 2800, exemple_d_animation_03, exemple_d_animation_04, exemple_de_transition_01 },
  {12000, exemple_d_animation_04, NULL, NULL },
  { 15000, credits, NULL, NULL },
  {    0, NULL, NULL, NULL } /* Toujours laisser à la fin */
};


/*!\brief dimensions initiales de la fenêtre */
static GLfloat _dim[] = {1024, 768};

/*!\brief fonction principale : initialise la fenêtre, OpenGL, audio
 * et lance la boucle principale (infinie).
 */
int main(int argc, char ** argv) {
  if(!gl4duwCreateWindow(argc, argv, "GL4Dummies DemoHelper", 
			 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			 _dim[0], _dim[1],
			 SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN))
    return 1;
  init();
  atexit(quit);
  gl4duwResizeFunc(resize);
  gl4duwKeyDownFunc(keydown);
  gl4duwDisplayFunc(gl4dhDraw);

  ahInitAudio("XXXTENTACION - Jocelyn Flores (Audio).mp3");
  gl4duwMainLoop();
  return 0;
}

/*!\brief Cette fonction initialise les paramètres et éléments OpenGL
 * ainsi que les divers données et fonctionnalités liées à la gestion
 * des animations.
 */
static void init(void) {
  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
  if(TTF_Init() == -1) {
    fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
    exit(2);
  }
  gl4dhInit(_animations, _dim[0], _dim[1], animationsInit);
  resize(_dim[0], _dim[1]);
}
/*!\brief paramétre la vue (viewPort) OpenGL en fonction des
 * dimensions de la fenêtre.
 * \param w largeur de la fenêtre.
 * \param w hauteur de la fenêtre.
 */
static void resize(int w, int h) {
  _dim[0] = w; _dim[1] = h;
  glViewport(0, 0, _dim[0], _dim[1]);
}

/*!\brief permet de gérer les évènements clavier-down.
 * \param keycode code de la touche pressée.
 */
static void keydown(int keycode) {
  switch(keycode) {
  case SDLK_ESCAPE:
  case 'q':
    exit(0);
  default: break;
  }
}

/*!\brief appelée à la sortie du programme (atexit).
 */
static void quit(void) {
  ahClean();
  gl4duClean(GL4DU_ALL);
}
