#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite/sqlite3.h"

char c;
#define BUFFER_CLEANER(c)\
    while((c = getchar()) != '\n' && c != EOF)

#define ERROR_FUNTION(resultado,funcion)\
    if((resultado = funcion) == -1){fprintf(stderr,"ERROR CON LA FUNCION\n");return 0;}

#define BUFFER_MAX 60

typedef struct{
    long long int numero;
    int apariciones;
} Aparicion_num;

double Mediana(sqlite3 *db,char *consulta);
double Media(sqlite3 *db,char *consulta);
long long int Moda(sqlite3 *db,char *consulta);

void Quick_Sort(long long int*numeros,int inicio,int final);
int Particion(long long int*numeros,int inicio,int final);


int main(){
    //conectando la base de datos
    sqlite3 *db = NULL;
    const char *ruta = "../DATA/Database.db";

    if(sqlite3_open(ruta,&db) != SQLITE_OK){
            fprintf(stderr,"ERROR AL ABRIR LA DB :%s\n",sqlite3_errmsg(db));
    }else{
        printf("se conecto exitosamente a la DB\n");
    }
    
    //obteniendo la consulta
    char *consulta = calloc(BUFFER_MAX,sizeof(char));
    //obteniendo el nombre de la columna
    printf("ESCRIBE LA CONSULTA COMPLETA:");
    fgets(consulta,BUFFER_MAX,stdin);
    if ((int)strcspn(consulta,"\n") == (int)strlen(consulta)){
        BUFFER_CLEANER(c);
        consulta[strcspn(consulta,"\n")] = '\0';
    }else{
        consulta[strcspn(consulta,"\n")] = '\0';
    }

    //FUCIONES
    double resultado_Media;
    double resultado_Mediana;
    long long int resultado_Moda;
    
    ERROR_FUNTION(resultado_Media,Media(db,consulta));
    ERROR_FUNTION(resultado_Mediana,Mediana(db,consulta));
    ERROR_FUNTION(resultado_Moda,Moda(db,consulta));

    printf("La media es de %.4lf\n",resultado_Media);
    printf("la mediana es de %.4lf\n",resultado_Mediana);
    printf("la moda es %lli\n",resultado_Moda);
    //liberando los recursos
    free(consulta);
    sqlite3_close(db);
    return 0;
}



double Mediana(sqlite3 *db,char *consulta){
    //preparando la consulta
    sqlite3_stmt *stmt;

    if(sqlite3_prepare_v2(db,consulta,-1,&stmt,NULL) != SQLITE_OK){
        fprintf(stderr,"hubo un error al preparar la consulta :%s\n",sqlite3_errmsg(db));
        return -1;
    }

    //lanzando la consulta
    long long int *Datos_consulta = NULL;
    int numero;
    int indice = 0;

    while(sqlite3_step(stmt) == SQLITE_ROW){
        numero = sqlite3_column_int64(stmt,0);
        Datos_consulta = (long long int*)realloc(Datos_consulta,(indice+1) * sizeof(long long int));
        Datos_consulta[indice] = numero;
        indice++;
    }

    //REALIZANDO LA MEDIANA
    double resultado;

    //algoritmo para ordenar
    Quick_Sort(Datos_consulta,0,indice);

    //verificando si es par o inpar
    int prueba = (indice+1)/2;
    float prueba_2 = (indice + 1)/2.0;

    if ((float)prueba - (float)prueba_2 == 0.0){
        //PAR
        prueba_2+=0.5;
        long long int numero1 = Datos_consulta[prueba];
        long long int numero2 = Datos_consulta[(int)prueba_2];
        resultado = (numero1+numero2)/2.0;
    }else{
        //IMPAR

        resultado = Datos_consulta[prueba];
    }

    free(Datos_consulta);
    sqlite3_finalize(stmt);
    return resultado;
}



double Media(sqlite3 *db,char *consulta){
    //preparando la consulta
    sqlite3_stmt *stmt;

    if(sqlite3_prepare_v2(db,consulta,-1,&stmt,NULL) != SQLITE_OK){
        fprintf(stderr,"hubo un error al preparar la consulta :%s\n",sqlite3_errmsg(db));
        return -1;
    }

    //lanzando la consulta
    long long int *Datos_consulta = NULL;
    int numero,indice = 0;

    while(sqlite3_step(stmt) == SQLITE_ROW){
        numero = sqlite3_column_int64(stmt,0);
        Datos_consulta = (long long int*)realloc(Datos_consulta,(indice+1) * sizeof(long long int));
        Datos_consulta[indice] = numero;
        indice++;
    }

    //sacando la media
    long long int sumatoria = 0.0;

    for(int i = 0; i<indice+1; i++){
        sumatoria+=Datos_consulta[i];
    }

    double resultado = sumatoria/(indice+1+0.0);

    sqlite3_finalize(stmt);
    free(Datos_consulta);
    return resultado;
}



long long int Moda(sqlite3 *db,char *consulta){
    //preparando la consulta
    sqlite3_stmt *stmt;

    if(sqlite3_prepare_v2(db,consulta,-1,&stmt,NULL) != SQLITE_OK){
        fprintf(stderr,"hubo un error al preparar la consulta :%s\n",sqlite3_errmsg(db));
        return -1;
    }

    //lanzando la consulta
    long long int *Datos_consulta = NULL;
    int indice = 0;

    while(sqlite3_step(stmt) == SQLITE_ROW){
        long long numero;
        numero = sqlite3_column_int64(stmt,0);
        Datos_consulta = (long long int*)realloc(Datos_consulta,(indice+1) * sizeof(long long int));
        Datos_consulta[indice] = numero;
        indice++;
    }

    //Sacando la moda
    Aparicion_num *apariciones_numeros = malloc(1*sizeof(Aparicion_num));
    int cantidad_array = 1;
    int indice_apariciones = 0;
    apariciones_numeros[indice_apariciones].apariciones= 1;
    apariciones_numeros[indice_apariciones].numero = Datos_consulta[0];

    Quick_Sort(Datos_consulta,0,indice);
    //rellanando el array de struct
    for (int i = 1; i<indice+1;i++){
        if(apariciones_numeros[indice_apariciones].numero == Datos_consulta[i]){
            apariciones_numeros[indice_apariciones].apariciones += 1;
        }else{
            cantidad_array++;
            indice_apariciones++;
            apariciones_numeros = realloc(apariciones_numeros,cantidad_array*sizeof(Aparicion_num));
            //colocando los nuevos datos
            apariciones_numeros[indice_apariciones].numero = Datos_consulta[i];
            apariciones_numeros[indice_apariciones].apariciones = 1;
        }
    }

    free(Datos_consulta);

    long long int resultado = apariciones_numeros[0].numero;
    int mayor_aparicion = apariciones_numeros[0].apariciones;
    //Obteniendo el numero con mayor aparicion
    for (int i = 1; i<cantidad_array;i++){
        if(mayor_aparicion < apariciones_numeros[i].apariciones){
            resultado = apariciones_numeros[i].numero;
            mayor_aparicion = apariciones_numeros[i].apariciones;
        }
    }

    sqlite3_finalize(stmt);
    free(apariciones_numeros);
    return resultado;
}


void Quick_Sort(long long int*numeros,int inicio, int final){

    if(final<=inicio){return;}

    int pivote = Particion(numeros,inicio,final);

    //particion izquierda
    Quick_Sort(numeros,inicio,pivote-1);
    //partcion derecha
    Quick_Sort(numeros,pivote+1,final);
}

int Particion(long long int*numeros,int inicio, int final){
    long long int pivote = numeros[final];
    int i = inicio-1;

    for(int j = inicio; j<=final; j++){
        if(numeros[j]<pivote){
            int temp;
            i++;
            temp = numeros[i];
            numeros[i] = numeros[j];
            numeros[j] = temp;
        }
    }
    int temp;
    i++;
    temp = numeros[i];
    numeros[i] = numeros[final];
    numeros[final] = temp;
    //retorna la posicion final del pivote
    return i;
}
