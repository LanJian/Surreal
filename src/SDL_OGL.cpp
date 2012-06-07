/* ***************************************************************
 *
 * File : SDL_OGL.cpp
 *
 * Author : Tiberiu Popa
 * 	    J. Alexander Clarke
 * Date   : June 18th, 2002
 *
 * Modified:
 *
 * Purpose: Implementation file for SDl with OpenGL framework
 *
 * Notes: Based on an SDL tutorial from SDL's web site
 *
 * ****************************************************************/
#include "SDL_OGL.h"
#include <stdio.h>
#include <iostream>



GLfloat lightPosition[] = {20, 20, -110, 1};

void ReSizeGLScene(GLsizei width, GLsizei height){
    
    // Reset The Current Viewport
    glViewport(0,0,width,height);	

    glMatrixMode(GL_PROJECTION);					
    glLoadIdentity();									

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(40.0, (GLfloat)width/(GLfloat)height, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);							
    glLoadIdentity();								

}


int InitGLLights(){
    //GLfloat light1[] = {0.8, 0.8, 0.8, 1};
    //GLfloat light2[] = {0.8, 0.8, .8, 1.0};
    //GLfloat light3[] = {0.2, 0.2, .2, 1.0};
    //GLfloat zero[] = {0, 0, 0 , 0};

    // setup 
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    //glLightf(GL_LIGHT0, GL_SHININESS, 25);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, light2);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, light1);
    //glLightfv(GL_LIGHT0, GL_AMBIENT, light3);
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

}

// Initialization
int InitGL(){
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	// Black Background
	glClearColor(0.00f, 0.00f, 0.00f, 0.0f);			
	
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	// initialize the lighs
	InitGLLights();


  //Use the color as the ambient and diffuse material
  //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  //glEnable(GL_COLOR_MATERIAL);
  
  //White specular material color, shininess 16
  //glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  //glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);

	//Calculate & save matrices
	//glPushMatrix();
	
	//glLoadIdentity();
	//gluPerspective(45.0f, (float)windowWidth/windowHeight, 1.0f, 100.0f);
	//glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);
	
	//glLoadIdentity();
	//gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
				//0.0f, 0.0f, 0.0f,
				//0.0f, 1.0f, 0.0f);
	//glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix);
	
	//glLoadIdentity();
	//gluPerspective(45.0f, 1.0f, 2.0f, 8.0f);
	//glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);
	
	//glLoadIdentity();
	//gluLookAt(	lightPosition.x, lightPosition.y, lightPosition.z,
				//0.0f, 0.0f, 0.0f,
				//0.0f, 1.0f, 0.0f);
	//glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);
	
	//glPopMatrix();

    return 1;
}


void KillGLWindow(){
	SDL_Quit();
}


int CreateGLWindow(char* title, int width, int height, int bits, int fullscreenflag)
{
	Uint32 flags;
	int size;

	/* Initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) {
		fprintf(stderr, "Couldn't init SDL: %s\n", SDL_GetError());
		return 0;
	}

	flags = SDL_OPENGL;
	if ( fullscreenflag ) {
		flags |= SDL_FULLSCREEN;
	}

	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 1 );
	if ( SDL_SetVideoMode(width, height, 0, flags) == NULL ) {
		return 0;
	}

	SDL_GL_GetAttribute( SDL_GL_STENCIL_SIZE, &size);

	ReSizeGLScene(width, height);						


	if (!InitGL()){
		KillGLWindow();
		return 0;		
	}

	return 1;
}


GLuint loadTextureBMP(char* filename){

  GLuint texture;
  SDL_Surface *surface;	// This surface will tell us the details of the image
  GLenum texture_format;
  GLint  nOfColors;
   
  if ( (surface = IMG_Load(filename)) ) { 
   
    // Check that the image's width is a power of 2
    if ( (surface->w & (surface->w - 1)) != 0 ) {
      printf("warning: image.bmp's width is not a power of 2\n");
    }

   
    // Also check if the height is a power of 2
    if ( (surface->h & (surface->h - 1)) != 0 ) {
      printf("warning: image.bmp's height is not a power of 2\n");
    }
   
          // get the number of channels in the SDL surface
          nOfColors = surface->format->BytesPerPixel;
          if (nOfColors == 4)     // contains an alpha channel
          {
                  if (surface->format->Rmask == 0x000000ff)
                          texture_format = GL_RGBA;
                  else
                          texture_format = GL_BGRA;
          } else if (nOfColors == 3)     // no alpha channel
          {
                  if (surface->format->Rmask == 0x000000ff)
                          texture_format = GL_RGB;
                  else
                          texture_format = GL_BGR;
          } else {
                  printf("warning: the image is not truecolor..  this will probably break\n");
                  // this error should not go unhandled
          }
   
    // Have OpenGL generate a texture object handle for us
    glGenTextures( 1, &texture );
   
    // Bind the texture object
    glBindTexture( GL_TEXTURE_2D, texture );

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   
    // Set the texture's stretching properties
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                        texture_format, GL_UNSIGNED_BYTE, surface->pixels );
  } 
   
  // Free the SDL_Surface only if it was successfully created
  if ( surface ) { 
    SDL_FreeSurface( surface );
  }

  return texture;
}
void setFullScreen(int width, int height, bool fs){
	Uint32 flags = SDL_OPENGL;
	if ( fs ) {
		flags |= SDL_FULLSCREEN;
	}

	SDL_SetVideoMode(width, height, 0, flags) == NULL;
}

Colour getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return Colour(p[0], p[1], p[2]);
        else
            return Colour(p[2], p[1], p[0]);
        break;
    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

