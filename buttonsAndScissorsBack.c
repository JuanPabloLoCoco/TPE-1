#include "buttonsAndScissorsBack.h"


void asignacionTabla(tMovimiento * tabla,int pos,int F1,int C1,int F2,int C2);
int Buscarmismo(char ** matriz,int F1, int C1, int df, int dc, int dim);
char** crearMatriz(int n);
int contarsaltos(char ** matriz,int F1,int C1, int df, int dc, int dim, int * posfx, int * posfy );
int Escero(char **matriz,int F1,int C1,int df,int dc,int salto);
int EsIgual(char **matriz,int F1,int C1,int df,int dc,int salto);
int hacerjugada(tPartida *partida,tMovimiento * movimiento )
int HayJugada(TPartida * partida)	
int ExisteTablero(const char* filename);
int ExisteArchivo(const char* filename);
int Maximo (tPartida * partida,tMovimiento * tabla);
int Minimo (tPartida * partida,tMovimiento * tabla);



int AbrirTablero(tPartida* partida)
{
//Asigno suficiente memoria para que entre el nombre del archivo.

  char* filename;
  if (partida->dim < 10)
    filename=malloc(10*sizeof(char));
  else
    filename=malloc(12*sizeof(char));
  sprintf(filename, "./%dx%d.txt", partida->dim, partida->dim);

//Veo si ese archivo existe
  if (!ExisteTablero(filename))
    return 0;
  FILE* archivo = fopen(filename, "r");
//Elijo un tablero al azar
  int opciones;

  opciones = (fgetc(archivo)-'0');
  int eleccion = aleatorio(1, opciones);


  char c=fgetc(archivo);
//Me paro sobre el primer tablero y busco el seleccionado
  for (int i = 0; i != (eleccion-1)*(2 + partida->dim*(partida->dim+1)); ++i)
    c=fgetc(archivo);

//Creo la matriz
  partida->tablero=crearMatriz(partida->dim);
  for (int i = 0; i < (partida->dim); ++i)
  {
    for (int j = 0; j < (partida->dim); ++j)
    {
      c=fgetc(archivo);
      if (c==' ')
        c='0';
      partida->tablero[i][j]=c;

      //Verifico que este pasando valores adecuados.
      //Si c fuese un espacio o un ENTER, fallaria el programa.
      if (c!='A' && c!='B' && c!='C' && c!='D' && c!='E' && c!='0')
      {
        return 0;
      }
    }
    c=fgetc(archivo);
  }
  return 1;
}

int aleatorio(int izq, int der)
{
    int num;
    srand(time(NULL));
    num = izq+(rand()%(der-izq+1));
    return num;
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

int CargarPartida(const char* filename, tPartida *partida)
{
  if (!(ExisteArchivo(filename)))
    return 0;
  char ubicacion[strlen(filename)+3];
  snprintf(ubicacion, sizeof(ubicacion), "./%s", filename);
  FILE* archivo = fopen(ubicacion, "rb");

  fread(&partida->jugadores, sizeof(partida->jugadores), 1, archivo);
  fread(&partida->turno, sizeof(partida->turno), 1, archivo);
  fread(&partida->dim, sizeof(partida->dim), 1, archivo);

  partida->tablero=crearMatriz(partida->dim);
  for (int i = 0; i < (partida->dim); ++i)
    for (int j = 0; j < (partida->dim); ++j)
      partida->tablero[i][j]=fgetc(archivo);
  fclose(archivo);
  return 1;
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

char** crearMatriz(int n)
{
    int flag=1;
    char** tablero=NULL;

    //Crear tantos arreglos como dimension
    tablero=malloc(sizeof(*tablero)*n);
    if(tablero!=NULL)
    {
        for (int i = 0; i < n  &&  flag; ++i)
        {
            //Para cada elemento del arreglo, darle otro arreglo de la misma dimension
            tablero[i]=malloc(sizeof(**tablero)*n);

            //Si no hay memoria, cancelar todo el proceso.
            if (tablero[i] == NULL)
            {
                flag=0;
                for (int j = 0; j >= 0; j--)
                    free(tablero[j]);
                free(tablero);
            }
        }
    }
    return tablero;
}

int Escero(char **matriz,int F1,int C1,int df,int dc,int salto)
{
  if (matriz[F1+df*salto][C1+dc*salto]=='0')
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

int ExisteArchivo(const char* filename)
{
    char ubicacion[strlen(filename)+3];
    snprintf(ubicacion, sizeof(ubicacion), "./%s", filename);
    FILE* archivo;

    if ((archivo = fopen(ubicacion, "rb")) != NULL)
    {
     fclose(archivo);
     return 1;
    }
    return 0;
}

int ExisteTablero(const char* filename)
{
    FILE* archivo;
    archivo = fopen(filename, "r");
    if (archivo != NULL)
    {
     fclose(archivo);
     return 1;
    }
    return 0;
}

void GuardarPartida( const char* filename, tPartida* partida)
{
  //Sumo 3 para asegurarme de que entren el "./" y el 0
  char ubicacion[strlen(filename)+3];
  snprintf(ubicacion, sizeof(ubicacion), "./%s", filename);
  FILE* archivo = fopen(ubicacion, "wb");

  // 0 = 2 players
  // 1 = CPU
  fwrite(&partida->jugadores, sizeof(partida->jugadores), 1, archivo);
  // Modo
  fwrite(&partida->turno, sizeof(partida->turno), 1, archivo);
  // Dimension del tablero
  fwrite(&partida->dim, sizeof(partida->dim), 1, archivo);
  // Tablero
  for (int i = 0; i < (partida->dim); ++i)
    for (int j = 0; j < (partida->dim); ++j)
      fwrite((&partida->tablero[i][j]), sizeof(char), 1, archivo);
  fclose(archivo);
}

//Dada una direccion de movimiento correcta, previamente verificada
int hacerjugada(tPartida *partida,tMovimiento * movimiento )
{
  int dx,dy,x=movimiento->F1,y=movimiento->C1, cont=0 ;
  if ((movimiento->F2-movimiento->F1)==0) //Si la resta de las posiciones en filas es 0, la pendiente en x es 0; 
    dx=0;
  else
    dx= (movimiento->F2) > (movimiento->F1) ? 1 : -1;// si no es 0, la pendiente es -1 o 1 dependiedno de los valores.
  if ((movimiento->C2-movimiento->C1)==0) //Si la resta de las posiciones en columna es 0, la pendiente en x es 0;
    dy=0;
  else
    dy = (movimiento->C2) > (movimiento->C1) ? 1 : -1; // si no es 0, la pendiente es -1 o 1 dependiedno de los valores.
  for (; x!=(movimiento->F2) || y!=movimiento->C2; x+= dx, y+=dy)//Mediante iteraciones, se va haciendo el corte colocando 0 en la linea de corte. 
  {
    if ( partida->tablero[x][y]!='0')
    {
      partida->tablero[x][y]='0';
      cont++;
    }
  }
  partida->tablero[movimiento->F2][movimiento->C2]='0';//Se hace 0 en la primera fila de corte. 
  cont++;
  return cont;
}

// Hay jugada indica si hay un movimiento de tan solo dos botones posibles. Una vez encontrado, devuelve verdadero.
int HayJugada(TPartida * partida)
{
  int F1,C1,df,dc,mov;
  for (F1 = 0; F1 < (partida->dim); F1++) //Recorre todas las filas
  {
    for (C1 = 0; C1 < (partida->dim); C1++)//Recorre todas las columnas
    {
      if ((!Escero(partida->tablero,F1,C1,0,0,0))//Verifica que la posicion no sea 0
      {
        for (df=0; df<2;df++)//Barre Las filas adyascentes
        {
          for (dc=-1; dc<2;dc++)//Barre las columnas adyascentes
          {
            if ((direccionBuena(df,dc))//Verifica que las posiciones del barrido sea de la L inferior
            {
              mov=Buscarmismo(partida->tablero,F1,C1,df,dc,partida->dim);//Manda a buscar ficha desde la posicion F1, C1 en direccion df dc
              if (mov>0)//Si es mayor a 0, significa que hay ficha
              {
                return 0;//hay jugada
              }
            }
          }
        }
      }
    }
  }
  return 1;//Si llego aca, es poque no encontro ficha. NO hay jugada
}

int InvalidMove(tPartida* partida, tMovimiento* coordenadas)
{
    //ERROR 1      Posicion inicial invalida
    //ERROR 2      Posicion Final invalida
    //ERROR 3      Necesito botones especificos
    //ERROR 4      No vale cortar uno solo
    //ERROR 5      No es una diagonal
    //ERROR 6      Hay botones bloqueando el camino

    //Dominio Original
    if (coordenadas->F1 >= partida->dim || coordenadas->F1 < 0 || coordenadas->C1 >= partida->dim || coordenadas->C1 < 0)
     return 1;


    //Dominio Final
    if (coordenadas->F2 >= partida->dim || coordenadas->F2 < 0 || coordenadas->C2 >= partida->dim || coordenadas->C2 < 0)
     return 2;


    if (partida->tablero[coordenadas->F1][coordenadas->C1] == ' ' || partida->tablero[coordenadas->F2][coordenadas->C2] == ' ')
     return 3;


    //Direccion
     //Se queda fijo
     if (coordenadas->F1 == coordenadas->F2 && coordenadas->C1 == coordenadas->C2)
       return 4;

     double my = (double)(coordenadas->C2-coordenadas->C1);
     double mx = (double)(coordenadas->F2-coordenadas->F1);
     //No es diagonal
     if (mx != 0     &&      my != 0     &&      abs(my/mx) != 1)
       return 5;

    //Blocked
     if (partida->tablero[coordenadas->F1][coordenadas->C1] != partida->tablero[coordenadas->F2][coordenadas->C2])
       return 6;

     int x;
     int y;
     int dx = (mx < 0)? -1 : (mx > 0);
     int dy = (my < 0)? -1 : (my > 0);

     for (x = coordenadas->F1, y = coordenadas->C1;    (x != (coordenadas->F2)) || (y != (coordenadas->C2))     ;     x += dx, y += dy   )
         {
         if (partida->tablero[coordenadas->F1][coordenadas->C1] != partida->tablero[x][y]   &&  partida->tablero[x][y] != '0')
           return 6;
         }
    return 0;
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
