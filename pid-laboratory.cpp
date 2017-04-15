#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[]) {
    int nivel_cantidad = 0;
    int i = 0;
    int status = 0;
    int id_pipe_lectura = 0;
    int id_pipe_escritura1 = 0;
    int id_pipe_escritura2 = 0;

    pid_t pidContador = 0;
    pid_t pidValorActual = 0;
    pid_t suma = 0;
    pid_t pid1 = 0;     
    pid_t pid2 = 0;    
    
    nivel_cantidad = atoi(argv[1]); 

    for (i = 1; i < nivel_cantidad; i++) {
       
        int pipeHijo1Nuevo[2];
        pipe(pipeHijo1Nuevo);
        
        pid1 = fork();
        
        if (pid1 == 0) {
            close(pipeHijo1Nuevo[WRITE]);
            read(pipeHijo1Nuevo[READ], &suma, sizeof(suma));
            close(pipeHijo1Nuevo[READ]);
            
            if (i == nivel_cantidad-1) {
                pid_t sumaTotal = suma + getpid();
                std::cout << "Soy proceso hoja del árbol con pid: "  << getpid() << ", la suma de los pid es: " << sumaTotal << std::endl;
            }  
        } else { 
           
            suma = suma + getpid();
            close(pipeHijo1Nuevo[READ]);
            write(pipeHijo1Nuevo[WRITE], &suma, sizeof(suma));
            close(pipeHijo1Nuevo[WRITE]);

            pid2=fork();
           
            if (pid2 == 0) { 
                close(pipeHijo1Nuevo[WRITE]);
                read(pipeHijo1Nuevo[READ], &suma, sizeof(suma));

                if (i == nivel_cantidad-1) {
                    pid_t sumaTotal = suma + getpid();
                    std::cout << "Soy proceso hoja del árbol con pid: "  << getpid() << ", la suma de los pid es: " << sumaTotal << std::endl;
                }             
            }      
            
            else if ( (pid1 != 0) && (pid2 != 0)) { break; }
        }
       std::cout << "Mi pid: "  << getpid() << ", pid padre: " << getppid() << ", leo y escribo en los descr: " << pipeHijo1Nuevo[READ] << ", "<< pipeHijo1Nuevo[WRITE] << "," << pipeHijo1Nuevo[WRITE] <<std::endl;
    }
    waitpid(pid1, &status, 0); 
    waitpid(pid2, &status, 0);
    return 0;
}

