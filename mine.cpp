/* Σπαθάρης Χρήστος - 1923 */
/* Μάμαλης Γεώργιος - 1880 */
/* compile: g++ -o mine mine.cpp -lGL -lglut -lGLU */



#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>  
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <unistd.h>
#include <string.h>

#define N 64

int sunmoves = 0;							
int reserve = 0;							
int xpos = N/2, ypos = N/2, zpos = 1;					
int randcolor[64][64][65];					
int xcoord,ycoord;							
int fallcounter = 0;							
int points = 50;
int lights = 0;
int lightpower = 0;
int cameracounter = 0;							
float camx = 0.0 , camy = 0.0, camz = 5.0;			
float camroll = 0.0 , camyaxis = 0.0;					
bool mouse = false;							
bool space = false;
bool gameover = false;
bool exist[N][N][N];
bool light[N][N][N];
GLfloat emissiveLight[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat emissivePosition[] = {0.0f,0.0f,0.0f,1.0f};
GLfloat ambientLight[] = {0.5f,0.5f,0.5f,1.0f};
GLfloat diffuseLight[] = {0.8f,0.8f,0.8f,0.1f};
GLfloat diffuseLight2[] = {0.3f,0.3f,0.3f,0.1f};
GLfloat lightposition[] = {1.0f,1.0f,65.0f,1.0f};
GLfloat difusePosition[] = {1.0f,1.0f,65.0f,0.1f};
GLfloat materialAmbientColour[] = { 0.2125f, 0.1275f, 0.054f, 1.0f };
GLfloat materialDiffuseColour[] = { 0.714f, 0.4284f, 0.18144f, 1.0f };
GLfloat materialEmissiveColour[] = { 0.1f, 0.1f, 0.1f, 1.0f }; 

void quad(int e1, int e2, int e3, int e4,int col,int side){
	int i,j;
	float r,g,b;
	float normalx,normaly,normalz;
	float ver[8][3] = 
	{
	    {-0.5,-0.5,0.5},
	    {-0.5,0.5,0.5},
	    {0.5,0.5,0.5},
	    {0.5,-0.5,0.5},
	    {-0.5,-0.5,-0.5},
	    {-0.5,0.5,-0.5},
	    {0.5,0.5,-0.5},
	    {0.5,-0.5,-0.5}
	};
		
			

	if(col == 0){
		r = 1.0;
		g = 0.0;
		b = 0.0;
	}
	else if(col == 1){
		r = 0.0;
		g = 1.0;
		b = 0.0;
	}
	else if(col == 2){
		r = 0.0;
		g = 0.0;
		b = 1.0;
	}
	else if(col == 3){
		r = 1.0;
		g = 1.0;
		b = 0.0;
	}
	else if(col == 4){
		r = 1.0;
		g = 0.0;
		b = 1.0;
	}
	
	GLfloat color[8][3] = 
	{
		{r,g,b},
		{r,g,b},
		{r,g,b},
		{r,g,b},
		{r,g,b},
		{r,g,b},
		{r,g,b},
		{r,g,b}
	};
	if(side == 1){
		normalx = 0.0;
		normaly = 0.0;
		normalz = -1.0;
	}
	else if(side == 2){
		normalx = 1.0;
		normaly = 0.0;
		normalz = 0.0;
	}
	else if(side == 3){
		normalx = 0.0;
		normaly = -1.0;
		normalz = 0.0;
	}
	else if(side == 4){
		normalx = 0.0;
		normaly = 1.0;
		normalz = 0.0;
	}
	else if(side == 5){
		normalx = 0.0;
		normaly = 0.0;
		normalz = 1.0;
	}
	else if(side == 6){
		normalx = -1.0;
		normaly = 0.0;
		normalz = 0.0;
	}

	glBegin(GL_QUADS);
	    glNormal3f(normalx,normaly,normalz);
	    glColor3fv(color[e1]);
	    glVertex3fv(ver[e1]);

	    glNormal3f(normalx,normaly,normalz);
	    glColor3fv(color[e2]);
	    glVertex3fv(ver[e2]);

	    glNormal3f(normalx,normaly,normalz);
	    glColor3fv(color[e3]);
	    glVertex3fv(ver[e3]);

	    glNormal3f(normalx,normaly,normalz);
	    glColor3fv(color[e4]);
	    glVertex3fv(ver[e4]);
	glEnd();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.0,0.0,0.0);
	glBegin (GL_LINE_LOOP);	
		glVertex3fv(ver[e1]);
		glVertex3fv(ver[e2]);
		glVertex3fv(ver[e3]);
		glVertex3fv(ver[e4]);
	glEnd ( );	
}

void lockCamera(){		
	if(camyaxis > 90.0){
		camyaxis = 90.0;
	}
	if(camyaxis < -90.0){
		camyaxis = -90.0;
	}
	if(camroll < 0.0){
		camroll += 360.0;
	}
	if(camroll > 360.0){
		camroll -= 360.0;
	}
}

void updateCamera(float distance, float direction){
	float rad,rad2;
	rad = (camroll + direction) * M_PI / 180;
	camx -= sin(rad) * distance;
	camz -= cos(rad) * distance;
}

void drawText(char *text, int len, int x, int y, int z){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
  	glColor3f( 1.0, 0.0, 0.0 );
  	glRasterPos3f(x, y, z);
  	for (int i = 0; i < len; i++) {
    		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
  	}
	glPopMatrix();
	
}

void checkIfGameOver(){
	int i,j,k,lighter=0;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			for(k=0;k<N;k++){
				if(light[i][j][k] == true){
					lighter = 1;
				}
			}
		}
	}
						
	if(points <= 0 || (sunmoves >= N && lighter == 0)){
		gameover = true;
	}
}


void fallToAbyss(){		
	if(exist[xpos][ypos][0] == false){
		points= points -20;
		camx = 0.0;
		camz = 5.0;
		xpos = N/2;
		ypos = N/2;
		zpos = 1;
	}
	checkIfGameOver();
}
	
void camera(float mousespeed , bool mouse){
	int w = glutGet( GLUT_WINDOW_WIDTH );
    	int h = glutGet( GLUT_WINDOW_HEIGHT );
	float midx,midy;
	float difx,dify;
	if(mouse){
		glutSetCursor(GLUT_CURSOR_NONE);
		midx = w/2;
		midy = h/2;
		difx = (midx - xcoord);
		dify = (midy - ycoord);
		camroll += mousespeed*(midx - xcoord);
		camyaxis += mousespeed*(midy - ycoord);
		lockCamera();
		fallToAbyss();
		glutWarpPointer(w/2,h/2);
	}
	glRotatef(-camyaxis,1.0,0.0,0.0);
	glRotatef(-camroll,0.0,1.0,0.0);
	glRotatef(0.0 , 0.0, 0.0, 1.0);
}

void initlight(){			
	glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
	glEnable(GL_LIGHT2);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight); 
    glShadeModel (GL_SMOOTH);	
	glEnable(GL_COLOR_MATERIAL);	
}

void renderScene(void) {
	int i,j,z;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();	
	int w = glutGet( GLUT_WINDOW_WIDTH );
    int h = glutGet( GLUT_WINDOW_HEIGHT );
	gluPerspective( 45, w / h, 1.0,1000.0 ); 
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glLightfv (GL_LIGHT0, GL_POSITION, lightposition);
	initlight();
 
	camera(0.1,mouse);
	
	glTranslatef(-camx,-camy,-camz);			
	if(cameracounter == 0){
		gluLookAt(	28.5f,2.0f, 28.5f,		
				5.0f, 2.0f, 5.0f,
				0.0f, 2.0f, 0.0f);
	}
	else if(cameracounter == 1){
		gluLookAt(	1.0f,65.0f, 1.0f,
				50.0f, 65.0f, 50.0f,
				0.0f, 65.0f, 0.0f);
	}
	else if(cameracounter == 2){
		gluLookAt(	1.0f,65.0f, 64.0f,
				1.0f, 65.0f, 5.0f,
				0.0f, 65.0f, 0.0f);
	}
	else if(cameracounter == 3){
		gluLookAt(	64.0f,65.0f, 1.0f,
				64.0f, 65.0f, 5.0f,
				0.0f, 65.0f, 0.0f);
	}
	else if(cameracounter == 4){
		gluLookAt(	64.0f,65.0f, 64.0f,
				64.0f, 65.0f, 5.0f,
				0.0f, 65.0f, 0.0f);
		cameracounter = -1;
	}
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			for(z=0;z<N;z++){
				if(exist[i][j][z] == true){
					glPushMatrix();	
					glTranslatef(i,z,j);
					quad(0,3,2,1,randcolor[i][j][z],1);
					quad(2,3,7,6,randcolor[i][j][z],2);
					quad(0,4,7,3,randcolor[i][j][z],3);
					quad(1,2,6,5,randcolor[i][j][z],4);
					quad(4,5,6,7,randcolor[i][j][z],5);
					quad(0,1,5,4,randcolor[i][j][z],6);
					glPopMatrix();
				}
				if(light[i][j][z] == true){
					GLfloat diffusePosition[] = {(float)xpos,(float)ypos,(float)zpos,1.0f};
					glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,materialEmissiveColour);
					glEnable (GL_LIGHT1);
					glLightfv (GL_LIGHT1, GL_DIFFUSE, diffuseLight2);
					glLightfv (GL_LIGHT1, GL_POSITION, diffusePosition);
					glPushMatrix();
					glTranslatef(i,z,j);
					glColor3d(1.0, 0.65, 0.0);
	    				glutSolidSphere(0.5, 256, 256);
					glPopMatrix();
				}
			}
		}
	}
	if(sunmoves < N){
		GLfloat sunPosition[] = {(float)sunmoves,(float)N,(float)sunmoves,1.0f};
		glTranslatef(sunmoves,N,sunmoves);
		glColor3d(1.0, 1.0, 0.0);
	    	glutSolidSphere(5, 512, 512);
		glLightfv (GL_LIGHT2, GL_DIFFUSE, diffuseLight);
		glLightfv (GL_LIGHT2, GL_POSITION, sunPosition);
	}
	if(sunmoves == N){
		glDisable(GL_LIGHT2);	
	}
	if(space == true){ 
		char text[16];
		sprintf(text, "Reserve : %d", reserve);
		drawText(text,strlen(text),0.0,0.0,0.0);
	}
	if(gameover == true){
		char over[20] = "GAME OVER!!!";
		drawText(over,strlen(over),0.8,0.8,0.8);

	}
	glutSwapBuffers();
}

void initrand(){
	int i,j,z;
	srand (time(NULL));
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			for(z=0;z<N;z++){
				randcolor[i][j][z] = rand() % 4;
				if(j == round(N/2) && i == round(N/2) && z == 0){
					randcolor[i][j][z] = 4;
				}
			}	
		}
	}
}

void initblocks(){
	int i,j,z;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			exist[i][j][0] = true;
			light[i][j][0] = false;
			for(z=1;z<N;z++){
				exist[i][j][z] = false;
				light[i][j][z] = false;
				
			}
		}		
	}
}

void changeSize(int w, int h) {
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45,ratio,1,1000);

	glMatrixMode(GL_MODELVIEW);
}

void MousePassiveMotion(int x , int y){		
	xcoord = x;
	ycoord = y;
}

void MouseEntry(int state){			
	if(state == GLUT_ENTERED){
		mouse = true;
	}
	else{
		mouse = false;
	}
}

int boxLevel(int x, int y, int z){		
	int i,levelcounter=0;
	for(i=0;i<N;i++){
		if(exist[x][y][i] == true){
			levelcounter++;
		}
	}
	return levelcounter;
}

bool checkKickOutOfBounds(int x,int y, int z){		
	if(x>=N || y>=N || x<0 || y<0){
		return false;
	}
	return true;
}

bool checkUpperBound(int level){
	if(level > N){
		return false;
	}
	return true;
}

void mouseClick(int button, int state, int x, int y){
	int boxlevel;
	if(button ==  GLUT_LEFT_BUTTON ){				
		if(state == GLUT_DOWN){
			if(reserve > 0){				
				points = points + 5;
				reserve --;
				if(camroll >= 0 && camroll <=90){
					boxlevel = boxLevel(xpos-1,ypos,zpos);
					if(checkUpperBound(boxlevel)){
						exist[xpos-1][ypos][boxlevel] = true;
					}		
				}
				if(camroll > 90 && camroll <=180){
					boxlevel = boxLevel(xpos,ypos+1,zpos);
					if(checkUpperBound(boxlevel)){
						exist[xpos][ypos+1][boxlevel] = true;
					}
				}
				if(camroll > 180 && camroll <=270){
					boxlevel = boxLevel(xpos+1,ypos,zpos);
					if(checkUpperBound(boxlevel)){
						exist[xpos+1][ypos][boxlevel] = true;
					}
				}
				if(camroll > 270 && camroll <=360){
					boxlevel = boxLevel(xpos,ypos-1,zpos);
					if(checkUpperBound(boxlevel)){
						exist[xpos][ypos-1][boxlevel] = true;
					}
				}
			}
		}
	}
	if(button == GLUT_RIGHT_BUTTON){			
		int newx,newy,newz;
		if(state == GLUT_DOWN){
			if(camroll >= 0 && camroll <=90){
				int c = 0;
				if(exist[xpos-1][ypos][zpos] == true){
					c++;
					while(exist[xpos-c-1][ypos][zpos] == true && xpos-c-1 >= 0){
						c++;
					}
					for(int i = c ; i > 0 ; i--){
						if(checkKickOutOfBounds(xpos - i - 1,ypos,zpos)){
							exist[xpos-i-1][ypos][zpos] = true;
							exist[xpos-i][ypos][zpos] = false;
							float temp = randcolor[xpos-i-1][ypos][zpos];
							randcolor[xpos-i-1][ypos][zpos] = randcolor[xpos-i][ypos][zpos];
							randcolor[xpos-i][ypos][zpos] = temp;				
						}
						else{
							exist[xpos-i][ypos][zpos] = false;
						}
					}
				}
			}
			if(camroll > 90 && camroll <=180){
				int c = 0;
				if(exist[xpos][ypos+1][zpos] == true){
					c++;
					while(exist[xpos][ypos+c+1][zpos] == true && ypos+c+1 < N){
						c++;
					}
					for(int i = c ; i > 0 ; i--){
						if(checkKickOutOfBounds(xpos,ypos+i+1,zpos)){
							exist[xpos][ypos+i+1][zpos] = true;
							exist[xpos][ypos+i][zpos] = false;
							float temp = randcolor[xpos][ypos+i+1][zpos];
							randcolor[xpos][ypos+i+1][zpos] = randcolor[xpos][ypos+i][zpos];
							randcolor[xpos][ypos+i][zpos] = temp;
						}
						else{
							exist[xpos][ypos+i][zpos] = false;
						}
					}
				}
			}
			if(camroll > 180 && camroll <=270){
				int c = 0;
				if(exist[xpos+1][ypos][zpos] == true){
					c++;
					while(exist[xpos+c+1][ypos][zpos] == true && xpos+c+1 < N){
						c++;
					}
					for(int i = c ; i > 0 ; i--){
						if(checkKickOutOfBounds(xpos+i+1,ypos,zpos)){
							exist[xpos+i+1][ypos][zpos] = true;
							exist[xpos+i][ypos][zpos] = false;
							float temp = randcolor[xpos+i+1][ypos][zpos];
							randcolor[xpos+i+1][ypos][zpos] = randcolor[xpos+i][ypos][zpos];
							randcolor[xpos+i][ypos][zpos] = temp;						
						}
						else{
							exist[xpos+i][ypos][zpos] = false;
						}
					}
				}
			}
			if(camroll > 270 && camroll <=360){
				int c = 0;
				if(exist[xpos][ypos-1][zpos] == true){
					c++;
					while(exist[xpos][ypos-c-1][zpos] == true && xpos-c-1 >= 0){
						c++;
					}
					for(int i = c ; i > 0 ; i--){
						if(checkKickOutOfBounds(xpos,ypos-i-1,zpos)){
							exist[xpos][ypos-i-1][zpos] = true;
							exist[xpos][ypos-i][zpos] = false;
							float temp = randcolor[xpos][ypos-i-1][zpos];
							randcolor[xpos][ypos-i-1][zpos] = randcolor[xpos][ypos-i][zpos];
							randcolor[xpos][ypos-i][zpos] = temp;
						}
						else{
							exist[xpos][ypos-i][zpos] = false;
						}
					}
				}
			}
		}
	}
}

void changeLevel(){
	if(exist[xpos][ypos][zpos] == true && exist[xpos][ypos][zpos+1] == false){
		zpos = zpos+1;
		camy = camy + 1.0;
		points = points + 5;
	}
	if(exist[xpos][ypos][zpos-1] == false){
		if(zpos > 1){
			zpos = zpos-1;
			camy = camy - 1.0;
			fallcounter++;
			if(fallcounter > 1){
				points = points - 5;
			}
			changeLevel();
		}
	}
	fallcounter=0;	
}

void checkBoxUnder(){			
	int i,j,k,co=0;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			for(k=2;k<N;k++){
				if(exist[i][j][k-1] == false && exist[i][j][k] == true){
					randcolor[i][j][k-1] = randcolor[i][j][k];
					exist[i][j][k] = false;
					exist[i][j][k-1] =true;
					renderScene();
					usleep(400000);
				}
			}
		}
	}
}

bool checkIfCanMove(int choice){		
	if(choice == 1){
		if(exist[xpos-1][ypos][zpos] == true && exist[xpos-1][ypos][zpos+1]==true){
			return false;
		}
		return true;
	}
	if(choice == 2){
		if(exist[xpos][ypos+1][zpos] == true && exist[xpos][ypos+1][zpos+1]==true){
			return false;
		}
		return true;
	}
	if(choice == 3){
		if(exist[xpos+1][ypos][zpos] == true && exist[xpos+1][ypos][zpos+1]==true){
			return false;
		}
		return true;
	}
	if(choice == 4){
		if(exist[xpos][ypos-1][zpos] == true && exist[xpos][ypos-1][zpos+1]==true){
			return false;
		}
		return true;
	}
}

bool checkIfOutOfBounds(int choice){		
	if(choice == 1){
		if(xpos - 1 < 0){
			return false;
		}
		return true;
	}
	if(choice == 2){
		if(ypos + 1 >= N){
			return false;
		}
		return true;
	}
	if(choice == 3){
		if(xpos + 1 >= N){
			return false;
		}
		return true;
	}
	if(choice == 4){
		if(ypos - 1 < 0){
			return false;
		}
		return true;
	}
}

void createLight(){
	if(lights > 0){			
		light[xpos][ypos][zpos] = true;
		lightpower = 0;
		lights --;
	}
}

void checkColor(){			
	if(randcolor[xpos][ypos][zpos-1] == 0){
		reserve = reserve + 2;
	}
	if(randcolor[xpos][ypos][zpos-1] == 1){
		reserve = reserve + 3;
	}
	if(randcolor[xpos][ypos][zpos-1] == 3){
		reserve = reserve + 1;
	}
	points = points - 5;
}	

void keys(unsigned char key, int x, int y){
	if(key == 'w'){
		if(camroll >= 0 && camroll <=90){			
			if(checkIfOutOfBounds(1)){				
				if(checkIfCanMove(1)){				
					updateCamera(1.0,0.0);
					xpos --;
					sunmoves++;
					lightpower++;
				}
			}
		}
		if(camroll >90 && camroll <=180){			
			if(checkIfOutOfBounds(2)){			
				if(checkIfCanMove(2)){				
					updateCamera(1.0,0.0);		
					ypos ++;
					sunmoves++;
					lightpower++;
				}
			}
		}
		if(camroll > 180 && camroll <=270){		
			if(checkIfOutOfBounds(3)){			
				if(checkIfCanMove(3)){
					updateCamera(1.0,0.0);
					xpos ++;
					sunmoves++;
					lightpower++;
				}
			}
		}
		if(camroll > 270 && camroll <=360){		
			if(checkIfOutOfBounds(4)){
				if(checkIfCanMove(4)){					
					updateCamera(1.0,0.0);			
					ypos --;
					sunmoves++;
					lightpower++;
				}
			}		
		}
		changeLevel();
	}
	if(key == 's'){
		if(camroll >= 0 && camroll <=90){
			if(checkIfOutOfBounds(3)){
				if(checkIfCanMove(3)){
					updateCamera(1.0,180.0);	
					xpos ++;
					sunmoves++;
					lightpower++;
				}
			}	
		}
		if(camroll >90 && camroll <=180){
			if(checkIfOutOfBounds(4)){
				if(checkIfCanMove(4)){			
					updateCamera(1.0,180.0);
					ypos --;
					sunmoves++;
					lightpower++;
				}
			}	
		}
		if(camroll > 180 && camroll <=270){
			if(checkIfOutOfBounds(1)){
				if(checkIfCanMove(1)){
					updateCamera(1.0,180.0);
					xpos --;
					sunmoves++;
					lightpower++;
				}
			}	
		}
		if(camroll > 270 && camroll <=360){
			if(checkIfOutOfBounds(2)){
				if(checkIfCanMove(2)){
					updateCamera(1.0,180.0);
					ypos ++;
					sunmoves++;
					lightpower++;
				}
			}
		}
		changeLevel();
	}
	if(key == 'a'){
		if(camroll >= 0 && camroll <=90){
			if(checkIfOutOfBounds(2)){
				if(checkIfCanMove(2)){
					updateCamera(1.0,90.0);
					ypos++;
					sunmoves++;
					lightpower++;
				}
			}
		}
		if(camroll >90 && camroll <=180){	
			if(checkIfOutOfBounds(3)){
				if(checkIfCanMove(3)){
					updateCamera(1.0,90.0);		
					xpos++;
					sunmoves++;
					lightpower++;
				}
			}
		}
		if(camroll > 180 && camroll <=270){
			if(checkIfOutOfBounds(4)){
				if(checkIfCanMove(4)){
					updateCamera(1.0,90.0);
					ypos--;
					sunmoves++;
					lightpower++;
				}
			}
		}
		if(camroll > 270 && camroll <=360){
			if(checkIfOutOfBounds(1)){
				if(checkIfCanMove(1)){
					updateCamera(1.0,90.0);		
					xpos--;
					sunmoves++;
					lightpower++;
				}
			}
		}
		changeLevel();
	}
	if(key == 'd'){
		if(camroll >= 0 && camroll <=90){
			if(checkIfOutOfBounds(4)){
				if(checkIfCanMove(4)){
					updateCamera(1.0,270.0);
					ypos--;
					sunmoves++;
					lightpower++;
				}
			}
		}
		if(camroll >90 && camroll <=180){
			if(checkIfOutOfBounds(1)){
				if(checkIfCanMove(1)){
					updateCamera(1.0,270.0);			
					xpos--;
					sunmoves++;
					lightpower++;
				}
			}
		}
		if(camroll > 180 && camroll <=270){
			if(checkIfOutOfBounds(2)){
				if(checkIfCanMove(2)){
					updateCamera(1.0,270.0);
					ypos++;
					sunmoves++;
					lightpower++;
				}
			}
		}
		if(camroll > 270 && camroll <=360){
			if(checkIfOutOfBounds(3)){
				if(checkIfCanMove(3)){
					updateCamera(1.0,270.0);		
					xpos++;
					sunmoves++;
					lightpower++;
				}
			}
		}
		if(lightpower >= 30){
			glDisable (GL_LIGHT1);
		}
		changeLevel();	
	}
	if(key == 'q'){
		if(camroll >= 0 && camroll <=90){
			exist[xpos-1][ypos][zpos] = false;		
		}
		if(camroll > 90 && camroll <=180){
			exist[xpos][ypos+1][zpos] = false;
		}
		if(camroll > 180 && camroll <=270){
			exist[xpos+1][ypos][zpos] = false;
		}
		if(camroll > 270 && camroll <=360){
			exist[xpos][ypos-1][zpos] = false;
		}
	}
	if(key == 'e'){
		int z;
		points = points-5;
		lights++;
		for(z=0;z<N;z++){
			if(camroll >= 0 && camroll <=90){
				exist[xpos-1][ypos][z] = false;		
			}
			if(camroll > 90 && camroll <=180){
				exist[xpos][ypos+1][z] = false;
			}
			if(camroll > 180 && camroll <=270){
				exist[xpos+1][ypos][z] = false;
			}
			if(camroll > 270 && camroll <=360){
				exist[xpos][ypos-1][z] = false;
			}
		}
	}	
	if(key == 'r'){
		points = points + 50;
		checkBoxUnder();
	}
	if(key == 'v'){
		cameracounter++;
	}
	if(key == 'l'){
		createLight();
	}
	if(key == 27){
		exit(1);
	}
	space = false;
	if(key==32){
		space = true;
		checkColor();
		if(xpos != N/2 || ypos !=N/2){
			randcolor[xpos][ypos][zpos-1] = 2;
		}
	}
}
	

int main(int argc,char** argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("MINECRAFT 3D");
	initblocks();
	initrand();

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutKeyboardFunc(keys);

	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutEntryFunc(MouseEntry);
		
	glutMainLoop();

	return 1;
}
