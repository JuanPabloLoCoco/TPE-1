#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include "getnum.h"
#define VERDADERO 1
#define FALSO 0
#define MovValido(i,j,x,y,dim) ((((i+(x))<0) || ((i+(x))>=dim) || ((j+(y))<0) || ((j+(y))>=dim) )? FALSO:VERDADERO)
#define AgrandarTabla(cont,bloque) (((cont%bloque)==0)?VERDADERO:FALSO)
#define direccionBuena(df,dc) ((!(df==dc && dc==0) && !(df==0 && dc==-1))?VERDADERO:FALSO)
#define MAX_DIM 30
#define bloque 10

typedef struct{
    int turno;
    int ptsjug1;
    int ptsjug2;
    char **tablero;
    int dim;
    int jugadores;
}tPartida;

typedef struct{
    int F1;
    int C1;
    int F2;
    int C2;
}tMovimiento;

//Verifico si en una posicion de la matriz, con un desplazamiento sobre una posicion inicial es cero
int Escero(char **matriz,int F1,int C1,int df,int dc,int salto)
{
  if (matriz[F1][C1]=='0')
    return VERDADERO;
  else
    return FALSO;
}

//Verifico si dos direciones de una matriz,caracterizados por un desplazamiento son iguales
int EsIgual(char **matriz,int F1,int C1,int df,int dc,int salto)
{
  if (matriz[F1][C1]==matriz[F1+df*salto][C1+dc*salto])
    return VERDADERO;
  else
    return FALSO;
}

//Se ingresa una matriz, las coordenadas de una posicion inicial, y un desplazamiento sobre x e y.
// Se busca mediante saltos desde una posicon inicial sobre una direccion a donde se encuentra el de mismo valor.
int Buscarmismo(char ** matriz,int F1, int C1, int df, int dc, int dim)
{
  int salto;
  //Por cada iteracion se verifica si esta dentro de la matriz, es cero o es igual a posicion inicial.
  for (salto=1;MovValido(F1,C1,df*salto,dc*salto,dim) && ((Escero(matriz,F1,C1,df,dc,salto)) || (EsIgual(matriz,F1,C1,df,dc,salto)));salto++)
  {
    // Si en una iteracion encuentra el mismo valor de la posicion incial, retorna la cantidad de saltos hasta ese valor.
     if (EsIgual(matriz,F1,C1,df,dc,salto))
        return salto;
  }
  //Encuentra un char diferente a la posicon inicial o diferente a 0, retorna 0;
  return 0;

}

//Se ingresa una matriz, coordenadas iniciales, un desplazamiento y punteros a posiciones finales.
//Se busca mediante saltos, hasta donde se encuentra el mas proximo distinto de la posicon inicial.
int contarsaltos(char ** matriz,int F1,int C1, int df, int dc, int dim, int * posfx, int * posfy )
{
  //botones es la cantidad de botones encontrados en una linea
  int botones=0, salto;

  //Por cada iteracion se verifica si esta dentro de la matriz, es cero o es igual a posicion inicial.
  for (salto=1; MovValido(F1,C1,df*salto,dc*salto,dim) && ((Escero(matriz,F1,C1,df,dc,salto)) || (EsIgual(matriz,F1,C1,df,dc,salto)));salto++)
  {
    //Si en una iteracion encuentra uno igual, suma a la lista de botones y guarda en los punteros su ubicacion final.
    if (EsIgual(matriz,F1,C1,df,dc,salto))
    {
      botones++;
      *posfx=(F1+df*salto);
      *posfy=(C1+dc*salto);
    }
  }
  /*retorna 0 si no hay movimiento en esa linea*/
  /*retorna el numero de botones hasta que sea diferente o se vaya de los limites*/

  return botones;
  }

//Dada un tMovimiento vector, un indice, y coordenadas finales e inciales.
//se guardan en el vector de estructura tmovimiento en la posicion del indice el movimiento.
void asignacionTabla(tMovimiento * tabla,int pos,int F1,int C1,int F2,int C2)
{
  tabla[pos].F1=F1;
  tabla[pos].C1=C1;
  tabla[pos].F2=F2;
  tabla[pos].C2=C2;
}


//Se ingresan una estructura tPartida con los datos de la partida y un puntero de movimientos
//En el arregglo de tmovimientos se guardaran los posibles movimientos, con la mayor cantidad de botones cortados.
int Maximo (tPartida * partida,tMovimiento * tabla)
{
  int F1,C1,df,dc,max=2,contmax=0,cont=0, posfx, posfy;
  //reservo memoria para posiciones de COls Final
  //  posfx y posfy son punteros a las direcciones finales
  //  contmax es el contado del listado de maximos
  /*Cont, cuenta el numero de botones en una linea*/
  /*max, señala la cantidad maxima de botones. Inicializa en 1, porque seguro que hay movimiento*/
  for (F1=0;F1<(partida->dim); F1++)//Recorro todas las filas
  {
    for (C1=0; C1<(partida->dim); C1++)//Recorro todas las columnas
    {
      if (!Escero(partida->tablero,F1,C1,0,0,0))//Sigo si en esa direccion no es 0
      {
        for (df=0; df<2; df++) //Tomo la L inferior derecha. voy de 0 a 1
        {
          for (dc=-1;dc<2;dc++) //Tomo todos los valores. de -1 a 1
          {
            if (direccionBuena(df,dc))//verifico que este dentro de la L
            {
              cont= contarsaltos(partida->tablero,F1,C1,df,dc,partida->dim,&posfx,&posfy)+1 ;//cuento cantidad de botones maximas en esa direccion desde esa posicion + 1 del boton del que sale
              if (cont==max)//Si es igual al maximo, sumo a la tabla de valores
              {
                //verifico que el vector de estructuras tabla tenga espacio suficiente
                if (AgrandarTabla(contmax,bloque))
                {
                  tabla=realloc(tabla,(contmax+bloque)*sizeof(tMovimiento));
                }
                asignacionTabla(tabla,contmax,F1,C1,posfx,posfy);
                contmax++;
              }
              //si supera al maximo. Hay un nuevo maximo
              //En ese caso rearmo el vector de posibles posiciones;
              if (cont>max)
              {
                max=cont;
                contmax=0;
                tabla=realloc(tabla,(contmax+bloque)*sizeof (tMovimiento));
                asignacionTabla(tabla,0,F1,C1,posfx,posfy);
                contmax++;
              }
            }
          }
        }
      }
    }
  }
  tabla=realloc(tabla,(contmax)*sizeof(tMovimiento));
  return contmax;
}


//Se ingresan una estructura tPartida con los datos de la partida y un puntero de movimientos
//En el arregglo de tmovimientos se guardaran los posibles movimientos, con la menor cantidad de botones cortados.
int Minimo (tPartida * partida,tMovimiento * tabla)
{
  int F1,C1,df,dc,cont=0,salto=0;
  for (F1=0;F1<(partida->dim); F1++)//Recorre todas las filas
  {
    for (C1=0; C1<(partida->dim); C1++)//recorre todas las columnas
    {
      if ((!Escero(partida->tablero,F1,C1,0,0,0)))//verifica que la posicion no sea 0
      {
        for (df=0; df<2; df++)//Tomo la L inferior derecha. voy de 0 a 1
        {
          for (dc=-1;dc<2;dc++)//Tomo todos los valores. de -1 a 1
          {
            if ((direccionBuena(df,dc)))//verifico que este dentro de la L
            {
              salto=Buscarmismo(partida->tablero,F1,C1,df,dc,partida->dim) ;/*devuleve la cantidad de saltos que realizo hasta encontrar uno igual.*/
              if (salto!=0) /*Si salto es distinto de 0, significa que encontro en una cierta cantidad de saltos*/
              {
                if (AgrandarTabla(cont,bloque)) //si no hay espacio en el vector, lo agrando
                {
                 tabla=realloc(tabla,(cont+bloque)*sizeof (tMovimiento));
                }
                asignacionTabla(tabla,cont,F1,C1,(F1+df*salto),(C1+dc*salto));
                cont++;
              }
            }
          }
	      }
      }
    }
  }
  tabla=realloc(tabla,cont*sizeof(tMovimiento));
  return cont;
}

//Movpc recibe un puntero a Tpartida, y un puntero a Movimiento.
void MovPc(tPartida *partida, tMovimiento * movimiento)
{
    //opciones va a guardar la dimension del arreglo con los posibles movimientos
    //op va a ser una variable booleana que decida si se busca la menor cantidad de botones o la mayor
    //elegida es un aleatorio entre 0 y la dimension del vector-1 que determina la opcion elegida
    int opciones=0,elegida=0,op=0;
    //en tabla se guardan los posibles movimientos a realizar
    tMovimiento * tabla;
    tabla=malloc(sizeof(tMovimiento));
    /*se sortea maximo o minimo*/
    op=aleatorio(0,1);
    if (op==0)
    {
      opciones=Minimo(partida,tabla);
    }
    else
    {
      opciones=Maximo(partida,tabla);
    }

    elegida= aleatorio(0,(opciones-1));

    //Una vez elegida la opcion de movimiento, seleccion de los posibles valores el valor en la posicion elegida.
    movimiento->F1=tabla[elegida].F1;
    movimiento->C1=tabla[elegida].C1;
    movimiento->F2=tabla[elegida].F2;
    movimiento->C2=tabla[elegida].C2;

}
