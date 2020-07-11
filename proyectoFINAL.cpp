#include <math.h>
#define PI 3.1415926535897932384626433832795
#include <freeglut.h> //o prueba con #include <GL/freeglut.h>
#include <GL/gl.h>

//declaramos variables para la rotacion
double rotacionD = 0, rotacionI = 0, velocidad = 15;

//declaramos variables para la translaccion
double x = 0, delta1;

//declaramos las variables para la pintura de los petalos
float pintarA=0, AzulF, pintarR=0, RojoF, pintarV=0, VerdeF;
float rojo=0, verde=0, azul=0;

//declaramos las variables para la animacion
double delta;
int tiempoActual, tiempoAnterior;

//Creamos variables para la creacion de la circunferencia de la hoja
static float radius = 15;
static double w;


//PARA LAS CURVAS USAMOS hermiteCubico.cpp
// Declaramos variables globales que nos ayudara en la creacion de las curvas	
	
	// Longitud de la línea de flecha.
	static float lengthArrowLine = 2.0; 
	
	// Número de vértices en cúbico.
	static int numVertices = 50; 
	
	// Puntos de Control.
	static float controlPoints[2][3];
	static float controlPoints1[2][3];
	
	// Vectores tangentes en los puntos de control.
	static float tangentVectors[2][3];
	static float tangentVectors1[2][3];
	
	// Longitudes de vectores tangentes.
	static float squareLengthTangent[2];
	static float squareLengthTangent1[2];
	
	// Puntos finales de vectores tangentes.
	static float endPointTangentVectors[2][3];
	static float endPointTangentVectors1[2][3];
	
	// Puntos finales de las líneas de flecha.
	static float endPointArrowLines[4][3];
	static float endPointArrowLines1[4][3];
	
	//variables auxiliares para las operaciones matematicas
	static int z;
	static float u, H0, H1, H2, H3;
// Fin de los globales.



	
//Funcion para el calculo de vectores de tangente de punto final
void CalcularVectorTangente(void){ 
 for (int i = 0; i < 2; i++)
 	for (int j = 0; j < 3; j++)
 		endPointTangentVectors[i][j] = controlPoints[i][j] + tangentVectors[i][j];
}

// Funcion para el calculo de puntos finales de línea de flecha.
void CalcularPuntosDeLaCurva(void){
 for (int i = 0; i < 2; i++){
 	squareLengthTangent[i] = tangentVectors[i][0]*tangentVectors[i][0] + tangentVectors[i][1]*tangentVectors[i][1];

 	if (squareLengthTangent[i] != 0.0){
		endPointArrowLines[2*i][0] = endPointTangentVectors[i][0] -lengthArrowLine * (tangentVectors[i][0] - tangentVectors[i][1]) / sqrt(2.0 * squareLengthTangent[i]);
 		endPointArrowLines[2*i][1] = endPointTangentVectors[i][1] - lengthArrowLine * (tangentVectors[i][0] + tangentVectors[i][1]) / sqrt(2.0 * squareLengthTangent[i]);
 		endPointArrowLines[2*i][2] = 0.0;
 		endPointArrowLines[2*i+1][0] = endPointTangentVectors[i][0] - lengthArrowLine * (tangentVectors[i][0] + tangentVectors[i][1]) / sqrt(2.0 * squareLengthTangent[i]);
 		endPointArrowLines[2*i+1][1] = endPointTangentVectors[i][1] - lengthArrowLine * (-tangentVectors[i][0] + tangentVectors[i][1]) / sqrt(2.0 * squareLengthTangent[i]);
 		endPointArrowLines[2*i+1][2] = 0.0;
 	}
 }
}

//se creo otra funcion iguales a la anterior para la construccion del tallo de la hoja
//porque nos exigia utilizar dos curvas
void CalcularVectorTangente1(void){ 
 for (int i = 0; i < 2; i++)
 	for (int j = 0; j < 3; j++)
 		endPointTangentVectors1[i][j] = controlPoints1[i][j] + tangentVectors1[i][j];
}
void CalcularPuntosDeLaCurva1(void){
 for (int i = 0; i < 2; i++){
 	squareLengthTangent1[i] = tangentVectors1[i][0]*tangentVectors1[i][0] + tangentVectors1[i][1]*tangentVectors1[i][1];

 	if (squareLengthTangent1[i] != 0.0){
		endPointArrowLines1[2*i][0] = endPointTangentVectors1[i][0] -lengthArrowLine * (tangentVectors1[i][0] - tangentVectors1[i][1]) / sqrt(2.0 * squareLengthTangent1[i]);
 		endPointArrowLines1[2*i][1] = endPointTangentVectors1[i][1] - lengthArrowLine * (tangentVectors1[i][0] + tangentVectors1[i][1]) / sqrt(2.0 * squareLengthTangent1[i]);
 		endPointArrowLines1[2*i][2] = 0.0;
 		endPointArrowLines1[2*i+1][0] = endPointTangentVectors1[i][0] - lengthArrowLine * (tangentVectors1[i][0] + tangentVectors1[i][1]) / sqrt(2.0 * squareLengthTangent1[i]);
 		endPointArrowLines1[2*i+1][1] = endPointTangentVectors1[i][1] - lengthArrowLine * (-tangentVectors1[i][0] + tangentVectors1[i][1]) / sqrt(2.0 * squareLengthTangent1[i]);
 		endPointArrowLines1[2*i+1][2] = 0.0;
 	}
 }
}

HermiteCubico(){
			 CalcularVectorTangente(); // Calcular puntos finales de vector tangente.
			 CalcularPuntosDeLaCurva(); // Calcular puntos de la curva
			
			 //imprime los puntos
			 for(int i = 0; i <= numVertices; ++i){
			 	// Operaciones para dibujo de la curva cúbica como una franja de línea.
			 	u = (float)i/numVertices;
				H0 = 2.0*u*u*u - 3*u*u + 1.0;
				H1 = -2.0*u*u*u + 3*u*u;
				H2 = u*u*u - 2.0*u*u + u;
				H3 = u*u*u - u*u;
			 	glVertex3f(H0*controlPoints[0][0] + H1*controlPoints[1][0] + H2*tangentVectors[0][0] + H3*tangentVectors[1][0], H0*controlPoints[0][1] + H1*controlPoints[1][1] + H2*tangentVectors[0][1] + H3*tangentVectors[1][1],0.0);
			 }
}

HermiteCubico1(){
			 CalcularVectorTangente1(); // Calcular puntos finales de vector tangente.
			 CalcularPuntosDeLaCurva1(); // Calcular puntos de la curva
			
			 //imprime los puntos
			 for(int i = 0; i <= numVertices; ++i){
			 	// Operaciones para dibujo de la curva cúbica como una franja de línea.
			 	u = (float)i/numVertices;
				H0 = 2.0*u*u*u - 3*u*u + 1.0;
				H1 = -2.0*u*u*u + 3*u*u;
				H2 = u*u*u - 2.0*u*u + u;
				H3 = u*u*u - u*u;
			 	glVertex3f(H0*controlPoints1[0][0] + H1*controlPoints1[1][0] + H2*tangentVectors1[0][0] + H3*tangentVectors1[1][0], H0*controlPoints1[0][1] + H1*controlPoints1[1][1] + H2*tangentVectors1[0][1] + H3*tangentVectors1[1][1],0.0);
			 }
}

//funcion para modificar la pantalla blanca
void reshape(int width, int height){
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(double)width/(double)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}


//Esta funcion calcula el tiempo que transcurre entre un frame y otro
void calculaTiempo(){
	/* Restamos el tiempo actual del tiempo pasado para
 	* para saber cuanto tiempo transcurrió entre un frame y otro */
 	
 	//devuelve el número de milisegundos que demora cada frame
    tiempoActual = glutGet(GLUT_ELAPSED_TIME);
    
    //el delta es la variacion entre un frame y otro
    delta = (double)(tiempoActual - tiempoAnterior)/1000;
    delta1 = (double)(tiempoActual - tiempoAnterior)/1000;
    tiempoAnterior = tiempoActual;
}


//eesta funcion se encarga de la animacion entre frame
void animacionTiempo(double delta) {
	
	/* Se actualizan los valores de los atributos que
 	* serán animados */
 	
 	//aqui se hacen las variaciones que tendran los colores
	//valores dinamicos que cambian cada frame
    float deltaRojo = delta/3,
          deltaAzul = delta/2,
          deltaVerde = delta/4;

    //aqui se hacen los variaciones de valor del color Azul
    if(azul<1.0f){
    	azul = azul+deltaAzul;
        pintarA=0.0f;
        AzulF=azul-pintarA;
    } else{
    	pintarA+=deltaAzul;
    	if(pintarA-azul<0){
    		azul=0.0f;
    		pintarA=0.0f;
    	}
    	AzulF=azul-pintarA;
    }
    
    //aqui se hacen los variaciones de valor del color Rojo
    if(rojo<1.0f){
    	rojo = rojo+deltaRojo;
        pintarR=0.0f;
        RojoF=rojo-pintarR;
    } else{
    	pintarR+=deltaRojo;
    	if(pintarR-rojo<0){
    		rojo=0.0f;
    		pintarR=0.0f;
    	}
    	RojoF=rojo-pintarR;
    }
    
    //aqui se hacen los variaciones de valor del color Verde
    if(verde<1.0f){
    	verde = verde+deltaVerde;
        pintarV=0.0f;
        VerdeF=verde-pintarV;
    } else{
    	pintarV+=deltaVerde;
    	if(pintarV-verde<0){
    		verde=0.0f;
    		pintarV=0.0f;
    	}
    	VerdeF=verde-pintarV;
    }
    
    
    //aqui se hacen los variaciones de la rotacion, que va de derecha a izquierda
    //PARA LA FLOR
	if(rotacionD>=5){
    	rotacionD -= velocidad * delta;
    	rotacionI = 0;
    }else{
    	rotacionI += velocidad * delta;
    	if(rotacionI+rotacionD>=15){
    		rotacionD=15;
    	}
    }
    
    //aqui se hacen los variaciones de la translacion, que va de derecha a izquierda
    //PARA LAS NUBES
    if(x > 60){
        x = -150;
    }
    else {
        x += 5 * delta1;
    }
    
}

//esta funcion invoca a las funciones anteriores 
void animar(){
    calculaTiempo();
    animacionTiempo(delta);
    glutPostRedisplay();
}

/*
En esta funcion se dibuja el sol
*/
void sol(){
	glPushMatrix();
			 glScalef(0.5,0.5,0);
			 glScalef(0.8,0.8,0);
			 glTranslatef( 50,175, 0 );
			 glColor3f(1.0, 1.0, 0.0);
			 glBegin(GL_POLYGON);
				for(w = 0; w < 2 * PI; w += PI / 10)
				 	glVertex3f(cos(w) * radius, sin(w) * radius, 0.0);
			 glEnd();
	glPopMatrix();
}

/*
En esta funcion se dibuja el pasto de la flor
*/ 
void pasto(){
			 controlPoints[0][0]=3;
			 controlPoints[0][1]=-5;
			 controlPoints[0][2]=0.0;
			 controlPoints[1][0]=2; 
			 controlPoints[1][1]=-40.5;
			 controlPoints[1][2]=0.0;
 
			 tangentVectors[0][0] = 24;
			 tangentVectors[0][1] = -34; 
			 tangentVectors[0][2] = 0.0;
			 tangentVectors[1][0] = -11.5; 
			 tangentVectors[1][1] = -17;
			 tangentVectors[1][2] = 0;
			 
			 glColor3f(0.137255, 0.556863, 0.137255);
			 for( int q=-150;q<=150;q+=15){
			 glPushMatrix();
 			 glScalef(0.5,0.5,0);
 			 glScalef(0.5,0.5,0);
			 glTranslatef( q, 60, 0 );
			 glBegin(GL_LINE_STRIP);
			 HermiteCubico();
			 glEnd();
			 glPopMatrix();
			 
			 glPushMatrix();
 			 glScalef(0.5,0.5,0);
 			 glScalef(0.5,0.5,0);
			 glTranslatef( -q, 63, 0 );
			 glRotatef(180, 0.0f,1.0f,0.0f);
			 glBegin(GL_LINE_STRIP);
			 HermiteCubico();
			 glEnd();
			 glPopMatrix();
			 }
			 
			 for(int q=-150;q<=150;q+=15){
			 glPushMatrix();
 			 glScalef(0.5,0.5,0);
 			 glScalef(0.5,0.5,0);
			 glTranslatef( q+10, 53, 0 );
			 glBegin(GL_LINE_STRIP);
			 HermiteCubico();
			 glEnd();
			 glPopMatrix();
			 
			 glPushMatrix();
 			 glScalef(0.5,0.5,0);
 			 glScalef(0.5,0.5,0);
			 glTranslatef( -q+10, 58, 0 );
			 glRotatef(180, 0.0f,1.0f,0.0f);
			 glBegin(GL_LINE_STRIP);
			 HermiteCubico();
			 glEnd();
			 glPopMatrix();
			 }
}

//funcion para dibujar la nube
void nube(){
			 glBegin(GL_POLYGON);
			 
			 //coordenadas de la primera curva
			 controlPoints1[0][0]=-40;
			 controlPoints1[0][1]=27.5;
			 controlPoints1[0][2]=0.0;
			 controlPoints1[1][0]=-35.5; 
			 controlPoints1[1][1]=36.5;
			 controlPoints1[1][2]=0.0;
			 
			 tangentVectors1[0][0] = -8;
			 tangentVectors1[0][1] = 16.5; 
			 tangentVectors1[0][2] = 0.0;
			 tangentVectors1[1][0] = 4; 
			 tangentVectors1[1][1] = 0;
			 tangentVectors1[1][2] = 0;
			 
			 HermiteCubico1();
			 
			 //coordenadas de la segunda curva
			 controlPoints1[0][0]=-35.5;
			 controlPoints1[0][1]=36.5;
			 controlPoints1[0][2]=0.0;
			 controlPoints1[1][0]=-22; 
			 controlPoints1[1][1]=41.5;
			 controlPoints1[1][2]=0.0;
			 
			 tangentVectors1[0][0] = 4.5;
			 tangentVectors1[0][1] = 20; 
			 tangentVectors1[0][2] = 0.0;
			 tangentVectors1[1][0] = 15.5; 
			 tangentVectors1[1][1] = -17;
			 tangentVectors1[1][2] = 0;
			 
			 HermiteCubico1();
			 
			 //coordenadas de la tercera curva
			 controlPoints1[0][0]=-22;
			 controlPoints1[0][1]=41.5;
			 controlPoints1[0][2]=0.0;
			 controlPoints1[1][0]=-4.5; 
			 controlPoints1[1][1]=41.5;
			 controlPoints1[1][2]=0.0;
			 
			 tangentVectors1[0][0] = 17;
			 tangentVectors1[0][1] = 33; 
			 tangentVectors1[0][2] = 0.0;
			 tangentVectors1[1][0] = 6; 
			 tangentVectors1[1][1] = -16.5;
			 tangentVectors1[1][2] = 0;
			 
			 HermiteCubico1();
			 
			 //coordenadas de la cuarta curva
			 controlPoints1[0][0]=-4.5;
			 controlPoints1[0][1]=41.5;
			 controlPoints1[0][2]=0.0;
			 controlPoints1[1][0]=7.5; 
			 controlPoints1[1][1]=35.5;
			 controlPoints1[1][2]=0.0;
			 
			 tangentVectors1[0][0] = 24.5;
			 tangentVectors1[0][1] = 15.5; 
			 tangentVectors1[0][2] = 0.0;
			 tangentVectors1[1][0] = 2; 
			 tangentVectors1[1][1] = -9.5;
			 tangentVectors1[1][2] = 0;
			 
			 HermiteCubico1();
			 
			 //coordenadas de la quinta curva
			 controlPoints1[0][0]=7.5;
			 controlPoints1[0][1]=35.5;
			 controlPoints1[0][2]=0.0;
			 controlPoints1[1][0]=13; 
			 controlPoints1[1][1]=27.5;
			 controlPoints1[1][2]=0.0;
			 
			 tangentVectors1[0][0] = 16.5;
			 tangentVectors1[0][1] = 1; 
			 tangentVectors1[0][2] = 0.0;
			 tangentVectors1[1][0] = -0.5; 
			 tangentVectors1[1][1] = -5.5;
			 tangentVectors1[1][2] = 0;
			 
			 HermiteCubico1();
			 
			 glEnd();
}

/*
En esta funcion se dibujan las nubes (gracias a la funcion nube)
*/	
void nubes(){
			 glPushMatrix();
			 glScalef(0.5,0.5,0);
			 glTranslatef( x, 100, 0 );
			 	
			 	glPushMatrix();
			 	glTranslatef( 48, 1.8, 0 );
			 	glScalef(0.9,0.8,0);
			 	glColor3f(0.35, 0.8, 1);
			 	nube();
			 	glPopMatrix();
			 	
			 	glPushMatrix();
			 	glTranslatef( 60, 0, 0 );
			 	glScalef(0.9,0.8,0);
			 	glColor3f(0.35, 0.9, 1);
			 	nube();
			 	glPopMatrix();
			 	
			 glPopMatrix();
}

/*
En esta matriz se dibuja el tallo de la flor
*/
void tallo(){
			 
			 //coordenadas de la primera curva
			 controlPoints[0][0]=3;
			 controlPoints[0][1]=-5;
			 controlPoints[0][2]=0.0;
			 controlPoints[1][0]=2; 
			 controlPoints[1][1]=-40.5;
			 controlPoints[1][2]=0.0;
 
			 tangentVectors[0][0] = 24;
			 tangentVectors[0][1] = -34; 
			 tangentVectors[0][2] = 0.0;
			 tangentVectors[1][0] = -11.5; 
			 tangentVectors[1][1] = -17;
			 tangentVectors[1][2] = 0;
			 
			 CalcularVectorTangente(); // Calcular puntos finales de vector tangente.
			 CalcularPuntosDeLaCurva(); // Calcular puntos de la curva
			 
			 //coordenadas de la segunda curva
			 controlPoints1[0][0]=1;
			 controlPoints1[0][1]=-5;
			 controlPoints1[0][2]=0.0;
			 controlPoints1[1][0]=-0.5; 
			 controlPoints1[1][1]=-41;
			 controlPoints1[1][2]=0.0;
 
			 tangentVectors1[0][0] = 29.5;
			 tangentVectors1[0][1] = -38; 
			 tangentVectors1[0][2] = 0.0;
			 tangentVectors1[1][0] = -9.5; 
			 tangentVectors1[1][1] = -12.5;
			 tangentVectors1[1][2] = 0;
			 
			 CalcularVectorTangente1(); // Calcular puntos finales de vector tangente.
			 CalcularPuntosDeLaCurva1(); // Calcular puntos de la curva
			 
			 glPushMatrix();
 			 glScalef(2,2,0);
			 glTranslatef( 0.7, 0, 0 );
			 glColor3f(0.137255, 0.556863, 0.137255);
 
			 glBegin(GL_QUAD_STRIP);
			 for(z = 0; z <= numVertices; z++){
			 	// Operaciones para dibujo de la curva cúbica como una franja de línea.
			 	u = (float)z/numVertices;
				H0 = 2.0*u*u*u - 3*u*u + 1.0;
				H1 = -2.0*u*u*u + 3*u*u;
				H2 = u*u*u - 2.0*u*u + u;
				H3 = u*u*u - u*u;
			 	glVertex3f(H0*controlPoints[0][0] + H1*controlPoints[1][0] + H2*tangentVectors[0][0] + H3*tangentVectors[1][0], H0*controlPoints[0][1] + H1*controlPoints[1][1] + H2*tangentVectors[0][1] + H3*tangentVectors[1][1],0.0);
			 	glVertex3f(H0*controlPoints1[0][0] + H1*controlPoints1[1][0] + H2*tangentVectors1[0][0] + H3*tangentVectors1[1][0], H0*controlPoints1[0][1] + H1*controlPoints1[1][1] + H2*tangentVectors1[0][1] + H3*tangentVectors1[1][1],0.0);
			 }
			 glEnd();
			 glPopMatrix();
}

/*
En esta matriz se dibuja la hoja de la flor
*/
void hoja(){
			 //coordenadas
			 controlPoints[0][0]=7;
			 controlPoints[0][1]=-22.5;
			 controlPoints[0][2]=0.0;
			 controlPoints[1][0]=7;
			 controlPoints[1][1]=-21.5;
			 controlPoints[1][2]=0.0;
			 
			 tangentVectors[0][0] = 74;
			 tangentVectors[0][1] = 5; 
			 tangentVectors[0][2] = 0.0;
			 tangentVectors[1][0] = -25.5; 
			 tangentVectors[1][1] = -27;
			 tangentVectors[1][2] = 0;
			 
			 glPushMatrix();
			 glScalef(2,2,0);
			 glTranslatef( -0.4, -8, 0 );
			 glColor3f(0.137255, 0.556863, 0.137255);
			 glBegin(GL_POLYGON);
			 HermiteCubico();
			 glEnd();
			 glPopMatrix();
}

/*
En esta funcion se dibuja los petalos de la flor
*/
void petalos(){
	
			 //las coordenadas del petalo
			 controlPoints[0][0]=7.5;
			 controlPoints[0][1]=-1;
			 controlPoints[0][2]=0.0;
			 controlPoints[1][0]=-8;
			 controlPoints[1][1]=-1;
			 controlPoints[1][2]=0.0;
		
			 tangentVectors[0][0] = 72;
			 tangentVectors[0][1] = 144.5; 
			 tangentVectors[0][2] = 0.0;
			 tangentVectors[1][0] = 56.5; 
			 tangentVectors[1][1] = -157;
			 tangentVectors[1][2] = 0;
			 
			 /*
			 En esta matriz se dibuja el petalo 5 de la flor
			 */ 
			 glPushMatrix();
			 glTranslatef(-5, -3, 0 );
			 glRotatef(-233.0f, 0.0f, 0.0f, 1.0f);
			 glColor3f(0.35, 0.35+VerdeF, 0.60+AzulF);
			 glBegin(GL_POLYGON);
			 HermiteCubico();
			 glEnd();
			 glPopMatrix();
 
 			 /*
			 En esta matriz se dibuja el petalo 1 de la flor
			 */ 
			 glPushMatrix();
			 glTranslatef(0, 7, 0 );
			 glColor3f(0.2, 0.3+VerdeF, 0.5);  //Some type of blue
			 glBegin(GL_POLYGON);
			 HermiteCubico();
			 glEnd();
			 glPopMatrix();
 
 
 			 /*
			 En esta matriz se dibuja el petalo 6 de la flor
			 */ 
			 glPushMatrix();
			 glTranslatef(-6, 3, 0 );
			 glRotatef(-290.0f, 0.0f, 0.0f, 1.0f);
			 glColor3f(0.30+RojoF, 0.30, 1-AzulF); 
			 glBegin(GL_POLYGON);
			 HermiteCubico();
			 glEnd();
			 glPopMatrix();
 
 
 			 /*
			 En esta matriz se dibuja el petalo 3 de la flor
			 */ 
			 glPushMatrix();
			 glRotatef(-120.0f, 0.0f, 0.0f, 1.0f);
			 glTranslatef(3, 6, 0 );
			 glColor3f(1-RojoF, 0.43, 0.78);
			 glBegin(GL_POLYGON);
			 HermiteCubico();
			 glEnd();
			 glPopMatrix();
 
 
 			 /*
			 En esta matriz se dibuja el petalo 2 de la flor
			 */
			 glPushMatrix();
			 glTranslatef(5.5, 2, 0 );
			 glRotatef(-65.0f, 0.0f, 0.0f, 1.0f);
			 glColor3f(0.82-RojoF, 0.57, 0.46+AzulF);
			 glBegin(GL_POLYGON);
			 HermiteCubico();
			 glEnd();
			 glPopMatrix();
 
 
 			 /*
			 En esta matriz se dibuja el petalo 4 de la flor
			 */
			 glPushMatrix();
			 glTranslatef( 0, -7, 0 );
			 glRotatef(-180.0f, 0.0f, 0.0f, 1.0f);
			 glColor3f(0.36+RojoF, 0.20, 0.09+AzulF); 
			 glBegin(GL_POLYGON);
			 HermiteCubico();
			 glEnd();
			 glPopMatrix();
}

/*
En esta funcion se dibuja el circulo de la flor
*/
void corolla(){
			 glPushMatrix();
			 glColor3f(1.0, 1.0, 0.0);
			 glBegin(GL_POLYGON);
				for(w = 0; w < 2 * PI; w += PI / 10)
				 	glVertex3f(cos(w) * radius, sin(w) * radius, 0.0);
			 glEnd();
			 glPopMatrix();
}
			 

//en esta funcion se encuentra el dibujo de la flor
void dibuja(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 0.0);//color de fondo
    
    /* Se guarda la matriz de transformación para no afectar
     * las siguientes primitivas dibujadas */  
	glPushMatrix();
	glTranslatef( 0, -47.5, 0 );			 
			 sol();
			 nubes();
			 pasto();
	/*
	En esta matriz se hace la rotacion de la flor
	*/ 
    glPushMatrix();
    glRotatef(rotacionD+rotacionI, 0.0f,0.0f,1.0f);
    	/*
		En esta matriz se hace el escalamiento de la flor
		(aumentar o dismunir tamaño)
		*/ 
		glPushMatrix();
 		glScalef(0.5,0.5,0);
 		glScalef(0.5,0.5,0);
 		glTranslatef( 0,122, 0 );
			 tallo();
 			 hoja();
 			 petalos();
 			 corolla();
 		glPopMatrix();
    /* Regresamos la matriz de transformación a su estado
     * anterior para volver a dibujar desde el origen */
    glPopMatrix();
    glPopMatrix();
    /* Se cambia al buffer con el nuevo contenido */
    glutSwapBuffers();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Proyecto Flor");

    glutReshapeFunc(reshape);
    
    //este comando glutIdleFunc nos ayuda a que el programa GLUT 
	//pueda realizar tareas de procesamiento en segundo plano o animación continua 
    glutIdleFunc(animar); 
    
    glutDisplayFunc(dibuja);

    
      /* posición y orientación */
    gluLookAt(  0.0, 0.0, 90.0, /* eye point */
                0.0, 0.0, 0.0,  /*punto de referencia */
                0.0, 1.0, 0.0); /* vector que indica arriba, en este caso Y */
    
    //aqui indicamos que el tiempo empieza en cero
	tiempoActual = tiempoAnterior = 0;
    glutMainLoop();
    return 0;
}
