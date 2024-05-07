#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t tenedor[ 5 ];
sem_t mesa;

unsigned int cuentaComida[] = {0, 0, 0, 0, 0};

void pensar(int i){
    unsigned long k = 0;
    while( k < 1000000)// gasta un poco de tiempo
        k++;
}

void comer( int i ){
    printf("el filosofo %d come\n", i);
    cuentaComida[ i ]++;
    sleep( 2 );
}

void *cuanto_ha_comido( void * ){
    while( 1 ){
        sleep( 1 );
        printf( "%d %d %d %d %d\n", cuentaComida[ 0 ], cuentaComida[ 1 ],
        cuentaComida[ 2 ], cuentaComida[ 3 ], cuentaComida[ 4 ] );
    }
}


void *filosofos( void *p ){
    int i = *(int *)p;
    while( 1 ) {
        pensar( i );

        sem_wait( &mesa );

        sem_wait( &tenedor[ i ] );
        sem_wait( &tenedor[ (i+1)%5 ] );

        comer( i );

        sem_post( &tenedor[ (i+1)%5 ] );  
        //sem_post( &tenedor[ i ] );   //cuando no liberamos el recurso compartido(los tenedores) existe interbloqueo y los procesos se van a dormir

        sem_post( &mesa );

    }
}

int main(){
    pthread_t pensador[ 5 ], comidas;
    int num_pensador[ ] = {0, 1, 2, 3, 4};
    int i;

    for( i = 0; i < 5; i++ ){
        sem_init( &tenedor[ i ], 0, 1 );
    }

    sem_init( &mesa, 0, 4 );

    pthread_create( &comidas, NULL, cuanto_ha_comido, NULL );

    for( i = 0; i < 5; i++ ){
        pthread_create( &pensador[ i ], NULL, filosofos,
                        (void *)&num_pensador[ i ] ) ;
    }

    for( i = 0; i < 5; i++ ){
        pthread_join( pensador[ i ], NULL ) ;
    }

    pthread_join( comidas, NULL );

}
