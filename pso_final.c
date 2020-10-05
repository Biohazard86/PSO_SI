// Algoritmo PSO universal
// DAVID BARRIOS PORTALES 
// davidbarrios@usal.es


/*
PARAMETROS:

e1 y e2 son valores aleatorios [0,1]
w es la inercia
f1 y f2 son los valores cognitivos y sociales respectivamente
T es el numero de iteracciones
h es una CTE positiva

*/

/*
COMPILAR:

gcc -o pso pso_final.c -lm  (-lm es necesario por incluir la libreria math.h)

*/

/*
EJEMPLO DE EJECUCION

./pso 1 0.729 2.05 2.05  (modo ejecucion 1 esfera y 2 sumacuadrado, W, F1 y F2)


*/

// pseudocodigo:
//	 REPETIR
//		Evaluar el firness de cada partícula
//		Actualizar la mejor solución de cada partícula
//		Actualizar la mejor solución globar
//		Actualizar la velocidad y posición de cada partícula
// 	HASTA (condición de parada)





/*
	ALGORITMO
		Intenta simular comportamientos solciales de animales

		Combina exploracion y explotacion
			/Exploracion de nuevas soluciones 
			/Explotacion de la informacion

		Aplica una busqueda local y busqueda global para buscar un equilibrio entre la explotacion y la exploracion.

		Se considera un grupo de particulas P. Cada particula tiene asociado un estado que se modifica con el tiempo.
			Variables de cada particula>>> Posicion, velocidad y mejor posicion personal.
		La solucion al problema viene dada por la mejor posicino encontrada por el enjambre.



		A - Inicializar la poblacion de particulas.
			Se inicializa la posicion y velocidad de todas ellas.


		B - Evaluar el fitness de cada particula
			Para calcularlo se aplica la funcion objetivo del problema a la posicion de cada particula. 
			El fitness asociado a la posicion de cada particula determina la calidad de la solcion que dicha posicion representa.


		C - Actualizar la mejor solucion personal de cada particula. 
			Si la nueva posicion de una particula es mejor que la mejor personal almacenada para ella hasta el momento, actualizamos esta ultima con el valor de la primera.

		D - Actualizar la mejor solucion global.
			Si se ha encontrado una solucion que mejora a la considerada mejor hasta el moment, guardarla como nueva mejor solucion global.

		E - Actualizar velocidad y posicion.
			En primer lugar se actualiza la velocidad y despues se actualiza la posicion.

				E-1 Actualizamos la velocidad >>   vi(t+1)= w vi(t) + f1e1[bi(t)-xi(t)] + f2e2[g(t)-xi(t)]
					DONDE>>>  	e1, e2: valores aleatorios en [0, 1]
								w: inercia		Una gran inercia hara una exploracion global y una pequenia, una local. (Es buena idea que decrezca con las iteracciones)
								f1: parámetro cognitivo
								f2: parámetro social

				E-2 Actualizar posicion
					Se calcula con:   xi(t+1) = xi(t) + vi(t+1)
					La nueva posición depende de la actual y de la nueva velocidad que acabamos de calcular.


*/








//Librerias ------------
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>		// Para los numero aleatorios.


//Constantes -----------
#define MAX 10				// Numero de interacciones del algoritmo ( T )
#define P 5 				// Numero de partículas del enjambre 
#define r 2 				// Numero de dimensiones del espacio de solucion  (tamaño de los vectores)
#define FUN_ESFERA 1		//Numero para la funcion funcion_esfera
#define FUN_CUADRADO 2  	//Numero para la funcion funcion_sumacuadrado
#define FUN_USO 2			//La funcion que vamos a usar. (para cambiar el uso se debe cambiar el valor de la CTE.)
#define MIN_PARAMETROS 4	//Numero minimo de parametros para que el programa funcione. ( "./pso 2" --> 1 parametro)
#define V1 -2				//Velocidad minima
#define V2 2				//velocidad maxima
#define P1 -5				//Posicion minima
#define P2 5 				//posicion maxima 

#define INFINITO 99999


// PROTOTIPOS
void mostrar_vector(double vector[], int tam);



// FUNCIONES:
//----------------------------------------------------------------------------------------------------------------------------------------
double funcion_esfera(double vector[]){		//Funcion esfera
	int i;
double auxiliar=0;  //OJO

	for(i=0;i<r;i++){
		auxiliar=auxiliar+(pow(vector[i],2));  
	}

	return auxiliar;
}

//----------------------------------------------------------------------------------------------------------------------------------------
double funcion_sumacuadrado(double vector[]){
	int i;
double valor=0,potencia; //OJO

	for(i=0;i<r;i++){
		potencia=(i+1)*pow(vector[i],2);
		valor=valor+potencia;
	}

	return valor;
}
//----------------------------------------------------------------------------------------------------------------------------------------

//Con esta funcion elegimos el metodo por el que calcular, por ejemplo, aplicar funcion_sumacuadrado o funcion_esfera mediante la variablo "opcion"
double calcular_fitness(double vector[], int opcion){
	double valor;

	//Se pueden implementar facilmente añadiendo más if's o con el case comentado abajo
    if(opcion==1){
        return ( funcion_esfera( vector ) );
    }

    if (opcion==2){
        return (funcion_sumacuadrado( vector ));
    }
    else{
    	printf("\n No se ha introducido un numero valido o el metodo no esta implementado. \n");
    	printf("Recuerde;\n");
    	printf("\t1 - FUNCION ESFERA\n");
    	printf("\t2 - FUNCION SUMA CUADRADO\n");
    	exit(1);
    }

/*
	switch(opcion){
		case 0:printf("\nNo se han introducido opciones en la llamada\n");
               exit(2); // salimos ya que no se ha introdcido ninguna opcion en la llamada del programa.

		case '1':return funcion_esfera(vector);
			x[P][r]
		case '2':return funcion_sumacuadrado(vector);

		default:printf("\nOpciones no implementadas\n");
			exit(1);
	}
*/
return 0;
}



/*-----------------------------------------------------------------------------
PARÁMETROS:
   	itera: iteración del algoritmo general en la que llamo a esta función
        0 => no tengo valores guardados en g, por lo que no debo calcular su fitnes
        distinto de 0 => ya tengo algún valor en g
----------------------------------------------------------- */
void actualizar_mejor_global(double g[r], double b[P][r], int opcion, int itera){		// Con esta función actualizamos la mejor posición global
	double fitness_g,fitness_b;
	int i,j;


	if (itera == 0)
		fitness_g =INFINITO;
	else
		fitness_g = calcular_fitness(g,opcion);

	for(i=0;i<P;i++){
		fitness_b=calcular_fitness(b[i],opcion);
		if(fitness_b < fitness_g){ //OJO

			fitness_g=fitness_b;//porque sabemos que en la siguiente interaccion será igual.
			printf("\n >>>>> Nuevo mejor FITNESS encontrado:  %lf", fitness_g);
			for(j=0;j<r;j++){
				g[j]=b[i][j];
			}

		}
	}

   printf("\nMEJOR GLOBAL ->");
   mostrar_vector(g, r);

}

//----------------------------------------------------------------------------------------------------------------------------------------


//Funcion que actualiza la mejor marca de cada partícula, comparando b (matriz de mejores posiciones para cada particula) con x (posicion actual de cada partícula)
void actualizar_mejor_personal(double b[P][r], double x[P][r], int opcion){
	int i,j;
	//printf("\n >>>>>>>>> actualizar_mejor_personal");
	if(opcion==1){
		for(i=0;i<P;i++){   // recorremos  
		//printf("\n\t<<<<  1 b: %lf    x: %lf",funcion_esfera(b[i]), funcion_esfera(x[i]));

			/*if(opcion==1)
				fitness= funcion_esfera(b[i]);
			else
				fitness= funcion_sumacuadrado(b[i]); */
			//fitness =calcular_fitness(b[i], opcion);

			if(funcion_esfera(b[i])>funcion_esfera(x[i])){
				
				// Si el fitness de B es peor que el fitness de X elegimos el valor de X

				//Si es mejor opcion (estamos minimizando, OJO) copiamos el vector x de R elementos en b 
				for(j=0;j<r;j++){
					b[i][j]=x[i][j];
				}
				//printf("\n\t\t|| X: %lf  B: %lf ||", funcion_esfera(x[i]), funcion_esfera(b[i]));
			}
		}
	}
	if(opcion==2){
		for(i=0;i<P;i++){   // recorremos  
			if(funcion_sumacuadrado(b[i])>funcion_sumacuadrado(x[i])){

				// Si el fitness de B es peor que el fitness de X elegimos el valor de X (Estamos minimizando)

				//Si es mejor opcion (estamos minimizando, OJO) copiamos el vector x de R elementos en b 
				for(j=0;j<r;j++){
					b[i][j]=x[i][j];
				}
				
			}
	}

}

   printf("\nMEJOR LOCAL ->");
   for(i=0;i<P;i++)
   {
	printf("\n  [parti.%d]-> ", i+1);
	mostrar_vector(b[i], r);
   }

}



//----------------------------------------------------------------------------------------------------------------------------------------
// Con esta funcióna actualizamos la nueva velocidad y la nueva posicion de cada partícula.
void actualizar_posicion_velocidad(double x[P][r],double v[P][r],double b[P][r], double g[], double w, double f1, double f2){
	double e1,e2;
	int i,j;
	//creamos las componentes aleatorias de e1 y e2
	//srand(time(NULL));
	e1=(  1.0+ (rand()%100)   )/100.0;  //OJO rand()%2;
	e2=(  1.0+ (rand()%100)   )/100.0;  //OJO rand()%2;
	printf("\n e1: %f\n e2: %f \n", e1, e2);

// calculamos la nueva velocidad
	for(i=0;i<P;i++){
		for(j=0;j<r;j++){
			v[i+1][r]=(w*v[i][j]  +  f1*e1*(b[i][j]-x[i][j])  +  f2*e2*(g[j]-x[i][j]));
		}
	}
//actualizamos la nueva posicion
	for(i=0;i<P;i++){
		for(j=0;j<r;j++){
			x[i+1][r]=(x[i][r])+(v[i+1][r]);
		}
	}

// FIN DE LA FUNCION.
}




// ------------------------------------------------------------------------------------

//Con esta función inicializamos las matrices de las particulas en las cuales se encuentra almacenada la informacion de posición, velocidad, y mejor posición. 

void inicializacion_particulas(double x[P][r], double v[P][r], double b[P][r]){

int i,j;

srand(time(NULL)); 		//Para que cada interaccion del bucle for se genere un numero aleatorio diferente. 

for(i=0;i<P;i++){
	for(j=0;j<r;j++){
		v[i][j]=rand()%(V2-V1+1)+V1;	// numero = rand () % (N-M+1) + M;   // Este está entre M y N

	}
}


for(i=0;i<P;i++){
	for(j=0;j<r;j++){
		x[i][j]=rand()%(P2-P1+P2);
		b[i][j]=x[i][j];  //inicializamos la b=x para que al menos la primera iteraccion sea igual que la actual.
	}
}

}




// ----------------------------------------------------------------------------------------

//Función que muestra un vector de tamaño "tam"
void mostrar_vector(double vector[], int tam)
{
int i;

	printf( " (");
	
	for(i=0;i<tam;i++)
		printf("%lf ", vector[i]);
	
	printf(" ) ");
}


//Funcion para mostrar las posiciones del vector g de un tamaño r (cte)

void mostrar_solucion (double g[]){
	int i;


// Mostramos las posiciones del vector g[r]  (r es una constante)
	printf("\n --- SOLUCION AL PROBLEMA ENCONTRADA POR PSO ---\n\t");
	for(i=0;i<r;i++){
		printf(" %lf   ", g[i]);

	}
	printf("\n ------------------------------------------------\n");


	//printf("\n");
	printf("\nValores de los diferentes parametros");
	printf("\n Particulas    %d", P);
	printf("\n Dimensiones   %d", r);
	printf("\n");


}




// MAIN ------------------------------------------------------------------------------------------------------------------

int main (int argc, char *argv[]){
	//VARIABLES 
	int i, opcion;
	double w,f1,f2;


	time_t t;      // para iniciar el generador de números aleatorios

	 // se inicia el generador de números aleatorios en función del tiempo
    srand((unsigned) time(&t));



	// Pasamos el caracter a int
	if(argc > MIN_PARAMETROS){
		opcion=atoi(argv[1]);
		w=atof(argv[2]);
		f1=atof(argv[3]);
		f2=atof(argv[4]);
		printf("\n Usted ha introducido la opcion:                 %d \n", opcion);
		printf(" Con una inercia (w) de:                           %f\n", w);
		printf(" Con un componente de parametro cognitivo f1 de:   %f\n", f1);
		printf(" Con un componente de parametro social f2 de:      %f\n", f2);
	}
	else{
		printf("\n\n No se han introducido los parametros necesarios.\n Recuerde, primer parametro para el modo de ejecucion. \n (1 para funcion esfera 2 para sumacuadrado)\n Segundo parametro para W (double) \n Tercer y cuarto parametro para f1 y f2 respectivamente (double) \n\n Saliendo del programa\n");
		printf("\nUn ejemplo de ejecución:\n");
		printf("\n Inercia 0.729\n");
		printf("\n Componentes cognitiva y social 2,1 (su suma >=4\n");
		printf("\n ./pso 1 0.729 2.05 2.05\n");
		exit(1);
	}



	//Mejor solucion del enjambre, la solución al problema.
	double g[r];

	//Posicion actual de cada una de las P partículas
	double x[P][r];  //x[0]      -> representa la posición de la primera partícula  
		       		 // x[P-1]   -> representa la posición de la última partícula (P-1)
	
	//Velocidad actual de cada una de las P partículas 
	double v[P][r];  //x[0]      -> representa la velocidad de la primera partícula  
					 // x[P-1]   -> representa la velocidad de la última partícula (P-1)
	
	//Mejor posición encotrada por cada una de las P partículas
	double b[P][r];   //x[0]      -> representa la mejor posición de la primera partícula  
					 // x[P-1]   -> representa la mejor posición de la última partícula (P-1)

	//MAX ES UNA CONSTANTE


	//inicializamos la matriz de posicion (x[P][r]) y la de velocidad (v[P][r])
	inicializacion_particulas(x,v,b);
	//Ya tenemos unos valores iniciales de posicion y de velocidad con los que trabajar.


	// for para realizar las MAX iteracciones (MAX es un constante definida arriba, en este caso 10)
	for(i=0;i<MAX;i++){
	//		Evaluar el firness de cada partícula
		// calcular_fitness(x[P][r], opcion);  //se hace dentro de actualizar_mejor_...	
	//		Actualizar la mejor solución de cada partícula
		actualizar_mejor_personal(b,x,opcion);
	//		Actualizar la mejor solución globar
		actualizar_mejor_global(g,b,opcion, i);
	//		Actualizar la velocidad y posición de cada partícula
		actualizar_posicion_velocidad(x, v, b, g, w, f1, f2);



	}



//		MOSTRAR MEJOR VALOR ENCONTRADO
	//mostamos el valor de g(t), que es el mejor valor global encontrado
	mostrar_solucion( g ); //Le pasamos el vector entero
	return 1;
}







