/*!\file base.c
 * \brief géométries lumière diffuse et transformations de base en GL4Dummies
 * \author Farès BELHADJ, amsi@ai.univ-paris8.fr
 * \date April 15 2016 (fichier de base étant introuvable, je l'ai obtenu au sein d'un L3)*/
#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>

/*!\brief identifiant du programme GLSL */
static GLuint _pId = 0;
/*!\brief quelques objets géométriques */
static GLuint _sphere = 0, _cube = 0, _quad = 0;

void cube_init(void) {
  int vp[4], w, h;
  glGetIntegerv(GL_VIEWPORT, vp);
  w = vp[2] - vp[0];
  h = vp[3] - vp[1];
  _pId  = gl4duCreateProgram("<vs>shaders/base.vs", "<fs>shaders/base.fs", NULL);
  gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
  gl4duBindMatrix("projectionMatrix");
  gl4duLoadIdentityf();
  gl4duFrustumf(-0.5, 0.5, -0.5 * h / w, 0.5 * h / w, 0.4, 1000.0);
  gl4duBindMatrix("modelViewMatrix");
  _sphere = gl4dgGenSpheref(30, 30);
  _cube = gl4dgGenCubef();
  _quad = gl4dgGenQuadf();
}

void cube_draw(void) {
  static GLfloat a = 0;
  GLfloat color1[] = {0, 1, 1, 1}, color2[] = {0, 1, 0, 1}, color3[] = {0, 0, 1, 0.2};

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gl4duBindMatrix("modelViewMatrix");
  gl4duLoadIdentityf();
  glUseProgram(_pId);
  gl4duTranslatef(0, 0, -50.0);


  gl4duRotatef(a++ , 0, 1, 12);
  gl4duTranslatef(0.07, 0.15, 0.22);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color3);
  gl4dgDraw(_sphere);
  



  gl4duRotatef( a++, 10, 1, 5);
  gl4duTranslatef(0.30, 0.37, 0.45);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1,color1);
  gl4dgDraw(_quad);
  


  gl4duRotatef(a++, 0, 1, 14);
  gl4duTranslatef(0.52, 0.60, 0.66);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1,color2);
  gl4dgDraw(_cube);
  


  gl4duRotatef(a++, 0, 1, 6);
  gl4duTranslatef(0.72, 0.80, 0.88);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color3);
  gl4dgDraw(_sphere);
  

  gl4duRotatef(a++, 0, 1, 3);
  gl4duTranslatef(0.97, 1.08, 1.16);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color1);
  gl4dgDraw(_cube);
  
  gl4duRotatef(a++, 0, 1, 17);
  gl4duTranslatef(1.22, 1.30, 1.37);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color2);
  gl4dgDraw(_quad);
  
  gl4duRotatef( a++, 0, 1, 10);
  gl4duTranslatef(1.45, 1.52, 1.60);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color3);
  gl4dgDraw(_sphere);
  
  gl4duRotatef( a++, 0, 1, 9);
  gl4duTranslatef(1.69, 1.75, 1.88);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color1);
  gl4dgDraw(_cube);
 /////////////////



  gl4duRotatef(a++*50, 1, 0, 6);
  gl4duTranslatef(0.72, 0.80, 0.88);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color3);
  gl4dgDraw(_sphere);
  

  gl4duRotatef(a++, 1, 0, 3);
  gl4duTranslatef(0.97, 1.08, 1.16);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color1);
  gl4dgDraw(_cube);
  
  gl4duRotatef(a++, 1, 0, 17);
  gl4duTranslatef(1.22, 1.30, 1.37);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color2);
  gl4dgDraw(_quad);
  
  gl4duRotatef( a++, 1, 0, 10);
  gl4duTranslatef(1.45, 1.52, 1.60);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color3);
  gl4dgDraw(_sphere);
  
  gl4duRotatef( a++, 0, 4, 9);
  gl4duTranslatef(1.69, 1.75, 1.88);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color1);
  gl4dgDraw(_cube);
 
  gl4duRotatef(a++, 1, 0, 6);
  gl4duTranslatef(0.72, 0.80, 0.88);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color3);
  gl4dgDraw(_sphere);
  

  gl4duRotatef(a++, 1, 0, 3);
  gl4duTranslatef(0.97, 1.08, 1.16);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color1);
  gl4dgDraw(_cube);
  
  gl4duRotatef(a++, 1, 0, 17);
  gl4duTranslatef(1.22, 1.30, 1.37);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color2);
  gl4dgDraw(_quad);
  
  ///////////////////////////

gl4duRotatef(a++, 0, 1, 6);
  gl4duTranslatef(0.72, 0.80, 0.88);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color3);
  gl4dgDraw(_sphere);
  

  gl4duRotatef(a++, 0, 1, 3);
  gl4duTranslatef(0.97, 1.08, 1.16);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color1);
  gl4dgDraw(_cube);
  
  gl4duRotatef(a++, 0, 1, 17);
  gl4duTranslatef(1.22, 1.30, 1.37);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color2);
  gl4dgDraw(_quad);
  
  gl4duRotatef( a++, 0, 1, 10);
  gl4duTranslatef(1.45, 1.52, 1.60);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color3);
  gl4dgDraw(_sphere);
  
  gl4duRotatef( a++, 0, 1, 9);
  gl4duTranslatef(1.69, 1.75, 1.88);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color1);
  gl4dgDraw(_cube);
 
  gl4duRotatef(a++, 0, 1, 6);
  gl4duTranslatef(0.72, 0.80, 0.88);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color3);
  gl4dgDraw(_sphere);
  

  gl4duRotatef(a++, 0, 1, 3);
  gl4duTranslatef(0.97, 1.08, 1.16);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color1);
  gl4dgDraw(_cube);
  
  gl4duRotatef(a++, 0, 1, 17);
  gl4duTranslatef(1.22, 1.30, 1.37);
  gl4duSendMatrices();
  glUniform4fv(glGetUniformLocation(_pId, "couleur"), 1, color2);
  gl4dgDraw(_quad);
  







  gl4dfBlur(0, 0, 5, 1, 0, GL_FALSE);
  gl4dfSobelSetMixMode(GL4DF_SOBEL_MIX_MULT);
  gl4dfSobel(0, 1, GL_FALSE);
}
