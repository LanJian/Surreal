/* ***************************************************************
 *
 * File :  main.cpp
 *
 * Author : Tiberiu Popa
 *          J. Alexander Clarke
 * Date   : June 18th, 2002
 *
 * Modified:
 *
 * Purpose: Simple OpenGL program to illustrate the use of SDL with OpenGL
 *
 * Notes: Based on an SDL tutorial from SDL's web site
 *
 * ****************************************************************/

#include "glee/GLee.h"
#include "SDL_OGL.h" 
#include "glm.h"
#include "SoundManager.h"
#include "particle.hpp"
#include "algebra.hpp"
#include "skydome.hpp"
#include "scene.hpp"
#include "a2.hpp"
#include "world.hpp"
#define MICKEY 1
#define SCENE 2
#define SKYDOME 3
float xangle = 0, yangle = 18;
int oldx = 100, oldy = 100;
int curTime = 0;

bool scenicView = false;
int fullscreen = 0;

Point3D scenicPos(-190, 310, -600);
Point3D scenicLookAt(0, 0, 0);

Point3D cameraPos(0,0,0);
Point3D cameraLookAt(0,0,-150);

SkyDome skydome(50000, 100,100);

World* world;
ParticleSystem* particles;

GLMmodel* venusModel;

int music_on = 0;

//Size of shadow map
const int shadowMapSize=7000;

//Textures
GLuint shadowMapTexture;
GLuint texture;
GLuint groundTexture;

//FBO
GLuint fboId;

void makeScene() {
  //GLUquadricObj* qsphere = gluNewQuadric();
  //glNewList(SCENE, GL_COMPILE);

  //gluQuadricDrawStyle(qsphere, GLU_FILL);
  //gluQuadricNormals(qsphere, GLU_SMOOTH);
  //gluQuadricOrientation(qsphere, GLU_OUTSIDE);
  //gluQuadricTexture(qsphere, GL_FALSE);

  //glColor3f(1,1,0);
  //glPushMatrix();
  //glTranslatef(0,-10,0);
  //gluSphere(qsphere, 13, 20, 20);
  //glPopMatrix();

  //glEndList();
  //gluDeleteQuadric(qsphere);

  //root = new SceneNode("root");

  //SceneNode* sphere1 = new GeometryNode("s1", new Sphere());
  //sphere1->translate(Vector3D(0,3,-300));
  //sphere1->scale(Vector3D(13,13,13));
  //sphere1->rotate('x', 90);

  //SceneNode* sphere2 = new GeometryNode("s2", new Sphere());
  //sphere2->translate(Vector3D(-30,3,-300));
  //sphere2->scale(Vector3D(13,13,13));
  //sphere2->rotate('x', 90);


  //root->add_child(sphere1);
  //root->add_child(sphere2);

  venusModel = glmReadOBJ("models/venusl.obj");
  glmUnitize(venusModel);

  particles = new ParticleSystem(500, Point3D(0,0,0), curTime, 0);

  // skydome
  skydome.setDawn(Colour(0.84,0.43,0.22), Colour(0.43,0.47,0.62));
  skydome.setDay(Colour(0.65,0.81,0.83), Colour(0.05,0.36,0.68));
  skydome.loadGradient("images/skyMap.bmp");

  texture = loadTextureBMP("glow1.tga");
  groundTexture = loadTextureBMP("images/bedrock1.bmp");
}


void drawScene(int pass, bool doOffset = false, bool applyTex = false){

    //glActiveTexture(GL_TEXTURE1);
    //glDisable(GL_TEXTURE_2D);
    //glActiveTexture(GL_TEXTURE0);
  
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, texture);

    //glColor4f(1,1,1,1);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);

      //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //// test
    //glPushMatrix();
    //glTranslatef(0,0,-100);
    //glRotatef(90, 1, 0, 0);
    //glBegin(GL_POLYGON);
    //glTexCoord2f(0,0);
    //glVertex3f(-1,0,-1);
    //glTexCoord2f(0,1);
    //glVertex3f(-1,0,1);
    //glTexCoord2f(1,1);
    //glVertex3f(1,0,1);
    //glTexCoord2f(1,0);
    //glVertex3f(1,0,-1);
    //glEnd();
    //glPopMatrix();

    //glDisable(GL_BLEND);
    //glDisable(GL_TEXTURE_2D);

    //glActiveTexture(GL_TEXTURE1);
    //glEnable(GL_TEXTURE_2D);
    //glActiveTexture(GL_TEXTURE0);

    glPushMatrix();
    //glScalef(1.5,1.5,1.5);
    glTranslatef(-175, 240, -470);
    glScalef(10,10,10);
    particles->draw();
    glPopMatrix();
    
    glColor3f(0.8,0.8,0.9);
    glPushMatrix();
    glTranslatef(-200,140,-450);
    glScaled(150,150,150);
    if(pass > 1)
      glScaled(0.993, 0.993, 0.993);
    glmDraw(venusModel, GLM_SMOOTH);
    glPopMatrix();


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, groundTexture);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // ground
    //glColor3f(0.80,0.56,0.1);
    if(pass == 2)
      glColor3f(0.3,0.3,0.3);
    else{
      double sina = sin(skydome.getT(curTime, 0.1, 0.55)*M_PI);
      Colour groundColor = Colour(0.3,0.3,0.3)*(1-sina) + Colour(1,1,1)*sina;
      glColor3f(groundColor.R(), groundColor.G(), groundColor.B());
    }
    glPushMatrix();
    glTranslatef(0,-5,0);
    glScalef(100000,0,100000);
    glBegin(GL_POLYGON);

    glTexCoord2f(0,0);
    glVertex3f(-1,0,-1);
    glTexCoord2f(0,5000);
    glVertex3f(-1,0,1);
    glTexCoord2f(5000,5000);
    glVertex3f(1,0,1);
    glTexCoord2f(5000,0);
    glVertex3f(1,0,-1);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

      //glEnable(GL_TEXTURE_2D);
      //glBindTexture(GL_TEXTURE_2D, groundTexture);
      //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glPushMatrix();
    world->draw(cameraPos, 500, doOffset);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


}

void drawSkyDome(int time){
  glPushMatrix();
  glTranslatef(0, -500, 0);

  skydome.draw(time);

  glPopMatrix();
}

void initShadowMap(){
	//Load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Shading states
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Depth states
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	//We use glScale when drawing the scene
	glEnable(GL_NORMALIZE);

  glActiveTexture(GL_TEXTURE0);
	//Create the shadow map texture
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0,
					GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	// create a framebuffer object
	glGenFramebuffersEXT(1, &fboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
	
	// Instruct openGL that we won't bind a color texture with the currently binded FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	// attach the texture to FBO depth attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, shadowMapTexture, 0);
	
	// check FBO status
	GLenum FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");
	
	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);


	//Use the color as the ambient and diffuse material
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
  GLfloat white[] = {1,1,1,1};
	//White specular material color, shininess 16
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);


  // fog
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogf(GL_FOG_DENSITY, 0.35f);              // How Dense Will The Fog Be
  glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
  glFogf(GL_FOG_START, 1.0f);             // Fog Start Depth
  glFogf(GL_FOG_END, 500.0f);               // Fog End Depth

}

void render(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
 // glLoadIdentity(); // Reset the view
 //
 

  Matrix4x4 lightProjectionMatrix, lightViewMatrix;
  Matrix4x4 cameraProjectionMatrix, cameraViewMatrix;
  GLfloat lightPosition[4];

  bool renderShadows = true;

  Point3D* sunPos = skydome.getSunPos(curTime);
  if(sunPos && (*sunPos)[1] > -100){
    lightPosition = {(*sunPos)[0],(*sunPos)[1],(*sunPos)[2],1};
  } else {
    renderShadows = false;
  }

  // get matrices
  glPushMatrix();

  glLoadIdentity();
  // TODO temp values
  gluPerspective(45.0, (GLfloat)1280/(GLfloat)720, 0.1, 100000.0);
  glGetDoublev(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);

  glLoadIdentity();
  // temp camera
  Point3D c = cameraPos;
  Point3D l = cameraLookAt;
  if(scenicView){
    c = scenicPos;
    l = scenicLookAt;
  }
  gluLookAt(c[0],c[1],c[2],l[0],l[1],l[2],0,1,0);
  glGetDoublev(GL_MODELVIEW_MATRIX, cameraViewMatrix);
  
  
    glLoadIdentity();
    glOrtho(-750, 750, -750, 750, 1, 100000);
    //gluPerspective(45.0f, 1.0f, 1.0f, 100000.0f);
    glGetDoublev(GL_MODELVIEW_MATRIX, lightProjectionMatrix);
    
    glLoadIdentity();
    gluLookAt(	lightPosition[0], lightPosition[1], lightPosition[2],
          0,0,0,
          0.0f, 1.0f, 0.0f);
    glGetDoublev(GL_MODELVIEW_MATRIX, lightViewMatrix);
    
    glPopMatrix();

    glDisable(GL_LIGHTING);
    //First pass - from light's point of view
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(lightProjectionMatrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(lightViewMatrix);


    glBindTexture(GL_TEXTURE_2D, 0);
    //First step: Render from the light POV to a FBO, story depth values only
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboId);	//Rendering offscreen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Use viewport the same size as the shadow map
    glViewport(0, 0, shadowMapSize, shadowMapSize);

    //Draw back faces into the shadow map
    glCullFace(GL_FRONT);

    //Disable color writes, and use flat shading for speed
    glShadeModel(GL_FLAT);
    glColorMask(0, 0, 0, 0);
    
    glPushMatrix();
    //glScaled(0.997, 0.997, 0.997);
    //Draw the scene
    drawScene(1);
    glPopMatrix();

    //Read the depth buffer into the shadow map texture
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    //glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);
    //glActiveTexture(GL_TEXTURE0);

    //restore states
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);
    glColorMask(1, 1, 1, 1);



  Light* lights = skydome.getLight(curTime);
    

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);	

  GLfloat white[] = {1,1,1,1};
  GLfloat black[] = {0,0,0,0};
  //
  Colour f = skydome.getFogColor(curTime);
  GLfloat fogcolor[] = {f.R(), f.G(), f.B(),1};
  glFogfv(GL_FOG_COLOR, fogcolor);            // Set Fog Color
  if(scenicView){
    glFogf(GL_FOG_START, 200);
    glFogf(GL_FOG_END, 1000);
  }
  else{
    glFogf(GL_FOG_END, 1);
    glFogf(GL_FOG_END, 500);
  }
  glEnable(GL_FOG);                   // Enables GL_FOG

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
    
  glViewport(0,0,1280,720);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixd(cameraProjectionMatrix);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixd(cameraViewMatrix);
  glClear(GL_DEPTH_BUFFER_BIT);

    // get matrices

    //2nd pass - Draw from camera's point of view


  Light shadowLight = lights[0];
  GLfloat dimAmbient[] = {shadowLight.ambient.R(), shadowLight.ambient.G(), shadowLight.ambient.B(), 1.0};
  GLfloat dimDiffuse[] = {shadowLight.diffuse.R(), shadowLight.diffuse.G(), shadowLight.diffuse.B(), 1.0};
  GLfloat dimSpecular[] = {shadowLight.specular.R(), shadowLight.specular.G(), shadowLight.specular.B(), 1.0};


    //Use dim light to represent shadowed areas
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, dimAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dimDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, dimSpecular);


    // hack
    glPushMatrix();
    //glScaled(1.005,1.005,1.005);
    drawScene(2,true, true);
    glPopMatrix();
    


  if(renderShadows){
  //3rd pass

  Light brightLight = lights[1];
  GLfloat litAmbient[] = {brightLight.ambient.R(), brightLight.ambient.G(), brightLight.ambient.B(), 1.0};
  GLfloat litDiffuse[] = {brightLight.diffuse.R(), brightLight.diffuse.G(), brightLight.diffuse.B(), 1.0};
  GLfloat litSpecular[] = {brightLight.specular.R(), brightLight.specular.G(), brightLight.specular.B(), 1.0};

  glLightf(GL_LIGHT0, GL_SHININESS, 25);
  glLightfv(GL_LIGHT0, GL_AMBIENT, litAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, litDiffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, litSpecular);

  //Calculate texture matrix for projection
  //This matrix takes us from eye space to the light's clip space
  //It is postmultiplied by the inverse of the current view matrix when specifying texgen
  static Matrix4x4 biasMatrix(Vector4D(0.5, 0.0, 0.0, 0.0),
                Vector4D(0.0, 0.5, 0.0, 0.0),
                Vector4D(0.0, 0.0, 0.5, 0.0),
                Vector4D(0.5, 0.5, 0.5, 1.0));	//bias from [-1, 1] to [0, 1]
  Matrix4x4 textureMatrix=biasMatrix.transpose()*lightProjectionMatrix.transpose()*lightViewMatrix.transpose();

  glActiveTexture(GL_TEXTURE1);
  //Set up texture coordinate generation.
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGendv(GL_S, GL_EYE_PLANE, textureMatrix.getRow(0));
  glEnable(GL_TEXTURE_GEN_S);

  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGendv(GL_T, GL_EYE_PLANE, textureMatrix.getRow(1));
  glEnable(GL_TEXTURE_GEN_T);

  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGendv(GL_R, GL_EYE_PLANE, textureMatrix.getRow(2));
  glEnable(GL_TEXTURE_GEN_R);

  glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGendv(GL_Q, GL_EYE_PLANE, textureMatrix.getRow(3));
  glEnable(GL_TEXTURE_GEN_Q);

  //Bind & enable shadow map texture
  glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
  glEnable(GL_TEXTURE_2D);

  //Enable shadow comparison
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);

  //Shadow comparison should be true (ie not in shadow) if r<=texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

  //Shadow comparison should generate an INTENSITY result
  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

  //Set alpha test to discard false comparisons
  glAlphaFunc(GL_GEQUAL, 0.99);
  glEnable(GL_ALPHA_TEST);

  glActiveTexture(GL_TEXTURE0);

  // hacky way to reduce moire artifacts
  glPushMatrix();
  //glScaled(1.005,1.005,1.005);
  drawScene(3,true, true);
  glPopMatrix();
  }

  glActiveTexture(GL_TEXTURE1);
  //Disable textures and texgen
  glDisable(GL_TEXTURE_2D);

  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glDisable(GL_TEXTURE_GEN_Q);

  glActiveTexture(GL_TEXTURE0);

  //Restore other states
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_FOG);

  //draw skydome
  glDisable(GL_LIGHTING);

  drawSkyDome(curTime);
  glEnable(GL_LIGHTING);


	glFinish();

  SDL_GL_SwapBuffers(); // Swap the buffers
}


void handleKey(SDL_KeyboardEvent key) {
  switch(key.keysym.sym) {
  case SDLK_f:
    if(key.state == SDL_PRESSED) {
      setFullScreen(1280,720,fullscreen);
      fullscreen = (fullscreen+1)%2;
    }
  }// switch

}

void handleMouseMotion(SDL_MouseMotionEvent motion){
    
  // hack for weird mouse position bug
  if(motion.y < 5)
    return;
  if(motion.y > 700)
    return;


  xangle += (float)(motion.x-400)/10.0f;
  yangle += -(float)(motion.y-400)/10.0f;
  //std::cerr << xangle << " " << yangle << std::endl;

  // TODO yangle clamp is buggy
  if(yangle > 90)
    yangle = 90;
  else if(yangle < -90)
    yangle = -90;

  Vector3D l(0,0,-150);
  double angleX = -toRadian(xangle); 
  double angleY = toRadian(yangle);
  l = rotation(angleX, 'y') * rotation(angleY, 'x') * l;
  cameraLookAt = cameraPos + l;

  SDL_WarpMouse(400,400);
  oldx = motion.x; oldy = motion.y;
}

void handleMouseButtons(SDL_MouseButtonEvent button){
    
    if(button.type == SDL_MOUSEBUTTONDOWN){
      scenicView = !scenicView;
    }

}

void move(int dir){
  Vector3D l = cameraLookAt - cameraPos;
  l[1] = 0;
  l.normalize();

  switch(dir){
    // forward
    case 0:
      break;
    // backward
    case 1:
      l = -l;
    //std::cerr << "shadow"
      break;
    // left
    case 2:
      l = -l.cross(Vector3D(0,1,0));
      break;
    // right
    case 3:
      l = l.cross(Vector3D(0,1,0));
      break;
  }

  if(world->intersectPlayer(cameraPos+l))
    return;

  Point3D c = cameraPos+l;
  Point3D d = cameraLookAt+l;
  // venus collision
  if(c[0] > -239 && c[0] < -157 &&
     c[2] > -495 && c[2] < -427)
    return;
  
  cameraPos = c;
  cameraLookAt = d;
  //std::cerr << cameraPos << " " << cameraLookAt << std::endl;
}

int main(int argc, char *argv[])
{
  

  // random seed
  srand(time(NULL));

  Uint8* keys; 
  int done=0; 
    //std::cerr << "shadow"

  //SM.LoadSound("card.wav");
  //SM.LoadSound("OBS.wav");
  //SM.LoadSound("ghost.wav");
  SM.LoadMusic("ambience.mp3");

  SM.PlayMusic(0);
  //SM.PlayMusic(2);

  world = new World(-750, -750, 1500, 1500, NULL);
  

  // Create a new OpenGL window with the title "Cone3D Basecode" at
  // 640x480x32, fullscreen and check for errors along the way
  if(CreateGLWindow("SDL & OpenGL", 1280, 720, 16, fullscreen) == 0){
      printf("Could not initalize OpenGL :(\n\n");
      KillGLWindow();
      return 0;
  }

  world->genWorld();


  initShadowMap();
  std::cerr << "init shadow map" << std::endl;

  char * v = (char *)glGetString(GL_VERSION);
  std::cerr << v << std::endl;

  // Hide the mouse cursor
  SDL_ShowCursor(0);

  // This is the main loop for the entire program and it will run until done==TRUE
  int changeit = 1;
  int reset = 1;
            xangle = 0;
            yangle = 18;
  std::cerr << xangle << " " << yangle << std::endl;

  makeScene();
  while(!done){
    // Get the state of the keyboard keys
    keys = SDL_GetKeyState(NULL);
    if(keys[SDLK_w]) {
      move(0);
      changeit = 1;
    }
    if(keys[SDLK_s]) {
      move(1);
      changeit = 1;
    }
    if(keys[SDLK_a]) {
      move(2);
      changeit = 1;
    }
    if(keys[SDLK_d]) {
      move(3);
      changeit = 1;
    }

    // And poll for events
    SDL_Event event;
    while ( SDL_PollEvent(&event) ) {
      switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
          handleKey(event.key);
          break;
        case SDL_MOUSEMOTION:
          SDL_PeepEvents (&event,9,SDL_GETEVENT,SDL_MOUSEMOTION);
          handleMouseMotion(event.motion);
          if (reset == 1)
          {
            xangle = 0;
            yangle = 18;
            reset = 0;
          }
          //this affects the screen.
          changeit = 1;
          break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          handleMouseButtons(event.button);
          break;
        case SDL_QUIT:
          // then we're done and we'll end this program
          done=1;
          break;
        default:
          break;
      }// switch

    }// while 


    // and check if ESCAPE has been pressed. If so then quit
    if(keys[SDLK_ESCAPE]) done=1;

    particles->update(curTime);

    // Draw the scene
      render();
      changeit = 0;

    curTime += 25;
    SDL_Delay(25);
  }// while done

  // Kill the GL & SDL screens
  
  KillGLWindow();
  // And quit
  return 0;
}
