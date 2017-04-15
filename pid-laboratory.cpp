#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
    int nivel_cantidad = 0;
    int i = 0;
    int status = 0;
    int id_pipe_lectura = 0;
    int id_pipe_escritura1 = 0;
    int id_pipe_escritura2 = 0;

    pid_t pidContador = 0;
    pid_t pidValorActual = 0;
    pid_t suma = 0;  //en suma guardo los valores que se van generando
    pid_t pid1 = 0;     
    pid_t pid2 = 0;    
    
    nivel_cantidad = atoi(argv[1]); 

    for (i = 1; i < nivel_cantidad; i++){
       
        int pipeHijo1Nuevo[2];  //problema de cambio de contexto??
        pipe(pipeHijo1Nuevo);
        
        pid1 = fork();  // Creo hijo1 con pid1
        
        if (pid1 == 0){  // Soy Hijo1 con pid1
           
            // Voy a leer
            // Cierro escritura 
            close(pipeHijo1Nuevo[WRITE]);
            
            //leo
            read(pipeHijo1Nuevo[READ], &suma, sizeof(suma));
            
            //test
            //std::cout << "Soy Hijo1 mi pid es: " << getpid() << " y  recibo  " << suma << " de padre" << std::endl;
            
            //cierro lectura
            close(pipeHijo1Nuevo[READ]);
            
            
            if (i == nivel_cantidad-1){  // Soy el ultimo hijo1 
                pid_t sumaTotal = suma + getpid(); //sumo lo que hay + mi pid
                std::cout << "Soy proceso hoja del árbol con pid: "  << getpid() << ", la suma de los pid es: " << sumaTotal << std::endl;
            }
            
        }else{  // Soy padre con 1 hijo 
           
            suma = suma + getpid();
            
            // Escribo en pipe
            
            //cierro lectura
            close(pipeHijo1Nuevo[READ]);
            
            //escribo
            write(pipeHijo1Nuevo[WRITE], &suma, sizeof(suma));
            
            //test
            //std::cout << "Soy padre mi pid es : " << getpid() << " y escribo: " << suma << std::endl;
            
            //cierro escritura
            close(pipeHijo1Nuevo[WRITE]);
         
            /*********************************************
             * creo hijo 2
             *********************************************/
            
            pid2=fork(); // Creo hijo2
           
            if (pid2 == 0){ // Soy hijo2
             
                //LEO PIPE
                
                //cierro escritura
                close(pipeHijo1Nuevo[WRITE]);
                
                //leo
                read(pipeHijo1Nuevo[READ], &suma, sizeof(suma));
               
                //test
                //std::cout << "Soy hijo2 mi pid es: " << getpid() << " y leo el valor:  " << suma << std::endl;
                
                if (i == nivel_cantidad-1){  // Soy el ultimo hijo1 
                    pid_t sumaTotal = suma + getpid(); //sumo lo que hay + mi pid
                    std::cout << "Soy proceso hoja del árbol con pid: "  << getpid() << ", la suma de los pid es: " << sumaTotal << std::endl;
                }             
            }      
            
            else if ( (pid1 != 0) && (pid2 != 0)){ // Soy padre de 2 hijos
                
                break; // Termino y ya no creo mas hijos
                
            }//Fin soy padre de 2 hijos
        }//Fin if soy padre de 1 hijo
        
       std::cout << "Mi pid: "  << getpid() << ", pid padre: " << getppid() << ", leo y escribo en los descr: " << pipeHijo1Nuevo[READ] << ", "<< pipeHijo1Nuevo[WRITE] << "," << pipeHijo1Nuevo[WRITE] <<std::endl;
        
    }//Fin for
   
    // Esperar
    waitpid(pid1, &status, 0); 
    waitpid(pid2, &status, 0);

    return 0;
}

