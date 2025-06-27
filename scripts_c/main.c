#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite/sqlite3.h"

typedef struct{
    int numero;
    int apariciones;
} Aparicion_num;

int Mediana(sqlite3 *db,sqlite3_stmt *stmt,char *consulta);
int Media(sqlite3 *db,sqlite3_stmt *stmt,char *consulta);
int Moda(sqlite3 *db,sqlite3_stmt *stmt,char *consulta);

void algoritmo(int*numeros,int longitud);

int main(){
    //conectando la base de datos
    sqlite3 *db = NULL;
    const char *ruta = "../DATA/Database.db";

    if(sqlite3_open(ruta,&db) != SQLITE_OK){
            fprintf(stderr,"ERROR AL ABRIR LA DB :%s",sqlite3_errmsg(db));
    }else{
        printf("se conecto exitosamente a la DB\n");
    }
    
    //obteniendo la consulta
    sqlite3_stmt *stmt;
    char *consulta = (char*)malloc((strlen("SELECT ") + 1)*sizeof(char));
    consulta = "SELECT ";

    //obteniendo el nombre de la columna
    char *dato = (char*)calloc(25,sizeof(char));
    printf("escribe el nombre de la columna:");
    fgets(dato,25,stdin);
    dato[strcspn(dato,"\n")] = '\0';

    //uniendo a la consulta la columna
    consulta = (char*)realloc(consulta,(strlen(consulta) + strlen(dato) + 1) * sizeof(char));
    strcat(consulta,dato);
    char *temp = " FROM Houses";
    consulta = (char*)realloc(consulta,(strlen(consulta) + strlen(temp) + 1) * sizeof(char));
    consulta[strlen(consulta)] = '\0';

    //FUCIONES

    //liberando los recursos
    sqlite3_close(db);
    return 0;
}



int Mediana(sqlite3 *db,sqlite3_stmt *stmt,char *consulta){
    //preparando la consulta
    if(sqlite3_prepare_v2(db,consulta,-1,&stmt,NULL) != SQLITE_OK){
        fprintf(stderr,"hubo un error al preparar la consulta :%s",sqlite3_errmsg(db));
        return 1;
    }

    //lanzando la consulta
    int *Datos_cosulta = NULL;
    int numero,indice = 0;

    while(sqlite3_step(stmt) == SQLITE_ROW){
        numero = sqlite3_column_int(stmt,0);
        Datos_cosulta = (int*)realloc(Datos_cosulta,(indice+1) * sizeof(int));
        Datos_cosulta[indice] = numero;
        indice++;
    }

    //REALIZANDO LA MEDIANA
    
    //algoritmo para ordenar

    int resultado;
    sqlite3_finalize(stmt);
    return resultado;
}



int Media(sqlite3 *db,sqlite3_stmt *stmt,char *consulta){
    //preparando la consulta
    if(sqlite3_prepare_v2(db,consulta,-1,&stmt,NULL) != SQLITE_OK){
        fprintf(stderr,"hubo un error al preparar la consulta :%s",sqlite3_errmsg(db));
        return 1;
    }

    //lanzando la consulta
    int *Datos_cosulta = NULL;
    int numero,indice = 0;

    while(sqlite3_step(stmt) == SQLITE_ROW){
        numero = sqlite3_column_int(stmt,0);
        Datos_cosulta = (int*)realloc(Datos_cosulta,(indice+1) * sizeof(int));
        Datos_cosulta[indice] = numero;
        indice++;
    }

    //sacando la media
    int sumatoria = 0;

    for(int i = 0; i<indice+1; i++){
        sumatoria+=Datos_cosulta[i];
    }

    int resultado = sumatoria/indice+1;

    return resultado;
}



int Moda(sqlite3 *db,sqlite3_stmt *stmt,char *consulta){
    //preparando la consulta
    if(sqlite3_prepare_v2(db,consulta,-1,&stmt,NULL) != SQLITE_OK){
        fprintf(stderr,"hubo un error al preparar la consulta :%s",sqlite3_errmsg(db));
        return 1;
    }

    //lanzando la consulta
    int *Datos_cosulta = NULL;
    int numero,indice = 0;

    while(sqlite3_step(stmt) == SQLITE_ROW){
        numero = sqlite3_column_int(stmt,0);
        Datos_cosulta = (int*)realloc(Datos_cosulta,(indice+1) * sizeof(int));
        Datos_cosulta[indice] = numero;
        indice++;
    }

    //Sacando la moda


    int resultado;
    return resultado;
}
