/*!\file animations.h
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
#ifndef _ANIMATIONS_H

#define _ANIMATIONS_H

#ifdef __cplusplus
extern "C" {
#endif
  extern void animation_earth(int state);
extern void animation_earth2(int state);
extern void animation_earth3(int state);
extern void animation_earth4(int state);
extern void animation_earth5(int state);
extern void animation_earth6(int state);
  extern void exemple_de_transition_00(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);
  extern void exemple_de_transition_01(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);
  extern void exemple_de_transition_02(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);

  extern void transition_fondu(void (* a0)(int), void (* a1)(int), Uint32 t, Uint32 et, int state);
  extern void exemple_d_animation_00(int state);
  extern void exemple_d_animation_01(int state);
  extern void exemple_d_animation_02(int state);
  extern void exemple_d_animation_03(int state);
  extern void exemple_d_animation_04(int state);
  extern void exemple_d_animation_05(int state);
  extern void exemple_d_animation_07(int state);
  extern void exemple_d_animation_09(int state);
  extern void exemple_d_animation_7(int state);
extern void exemple_d_animation_8(int state);
extern void exemple_d_animation_9(int state);
extern void exemple_d_animation_10(int state);
extern void exemple_d_animation_11(int state);
extern void exemple_d_animation_12(int state);
extern void exemple_d_animation_13(int state);
extern void exemple_d_animation_14(int state);
extern void exemple_d_animation_15(int state);
  extern void exemple_d_animation_06(int state);
  extern void exemple_d_animation_061(int state);
 extern void exemple_d_animation_062(int state);
  extern void exemple_d_animation_005(int state);
  extern void exemple_d_animation_006(int state);
  extern void exemple_d_animation_sit(int state);
  extern void exemple_d_animation_sit2(int state);
  extern void exemple_d_animation_sit3(int state);
  extern void exemple_d_animation_sit4(int state);
  extern void exemple_d_animation_sit5(int state);
  extern void exemple_d_animation_sit6(int state);
  extern void exemple_d_animation_sit7(int state);
  extern void exemple_d_animation_sit8(int state);
  extern void exemple_d_animation_sit9(int state);
  extern void animation_b(int state);
  extern void animation_ear(int state);
  extern void animation_flash(int state);
  extern void audio_analyzer(int state);  
  /* credits.c */
  extern void credits(int state);
  extern void animationsInit(void);

#ifdef __cplusplus
}
#endif

#endif
