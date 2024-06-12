#include <stdio.h>
//#include "EasyPIO.h"
#include <stdint.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h> // for usleep
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#define CLEAR "clear"

#define PASSWORD "12345"
#define SPEED_INCREMENT 100000
#define MAX_INTENTOS 3

#define TECLA_ENTER 13
#define TECLA_BACKSPACE 8
#define LONGITUD_CLAVE 5

int speed = 500000;

char my_getch(void) {
    int ch;
    nodelay(stdscr, TRUE); // No bloquear getch
    ch = getch();
    nodelay(stdscr, FALSE); // Restaurar el bloqueo de getch
    return ch;
}

void delay (unsigned long int a) {
    unsigned int velocidad=a;

            while(a>0){
               if(my_getch()==KEY_DOWN){
                   velocidad+=5000;
                   a+=5000;
               }
               if((velocidad-5000)>=50000 && (a-5000)>=50000){
                   if(my_getch()==KEY_UP){
                       velocidad-=5000;
                       a-=5000;
                   }
               }else{
                   break;
               }
               a--;
            }
            //return velocidad;
}

int kbhit(void) {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

void ocultarEntrada(char* clave, int longitud) {
    int i = 0;
    char caracter;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (1) {
        caracter = getchar();

        if (caracter == '\n' || caracter == '\r') {
            clave[i] = '\0';
            break;
        } else if (caracter == 127 || caracter == TECLA_BACKSPACE) {
            if (i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
        } else {
            if (i < longitud) {
                printf("*");
                fflush(stdout);
                clave[i] = caracter;
                i++;
            }
        }
    }

    newt.c_lflag |= (ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void disp_binary(int i){
    int t;
    for(t=128;t>0;t=t/2)
        if(i&t) printf("*");
        else printf("_");
        printf("\n");
}

void AutoFantastico(void){
    char output;
    char t, j=1;
    int on_time=1;

    initscr(); // Iniciar ncurses
    noecho(); // No mostrar teclas presionadas
    keypad(stdscr, TRUE); // Habilitar teclas especiales

    printf("Secuencia en accion ... Oprima 'e' para finalizar\n");
    do{
        output=0x80;
        for(t=0; t<8; t++){
            disp_binary(output);
            sleep(on_time);
            output=output>>1;
        }
        output=0x01;
        for(t=0;t<6;t++){
            output=output<<1;
            disp_binary(output);
            sleep(on_time);
        }

        if (kbhit()) {
            char c = my_getch();
            if (c == 'e') {
                break;
            }
        }

    }while(1); //repite hasta que se toca una tecla
    output=0x00;
    disp_binary(output);
    printf("Chau...\n");
    endwin(); // Finalizar ncurses
}

void Choque(void) {
    printf("Mostranco Choque: \n");
    char a;
    unsigned char tabla[7] = {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42};
    for (int i = 0; i < 7; i++) {
        disp_binary(tabla[i]);
        sleep(1000); // Convertir a milisegundos
    }
}

void mostrarMenu() {
    printf("\n\n\tMENU\n");
    printf("\t----\n");
    printf("\t1. El Auto Fantastico\n");
    printf("\t2. El Choque\n");
    printf("\t3. Parpadeo Alternado\n");
    printf("\t4. Ola oceánica\n");
    printf("\t5. Salir\n");
}

int main() {
    char clave[LONGITUD_CLAVE + 1];
    int intento = 0;
    int ingresa = 0;

    // Contraseña única guardada en memoria
    char claveGuardada[] = "12345";

    do {
        system(CLEAR);
        printf("\n\tCLAVE: ");
        ocultarEntrada(clave, LONGITUD_CLAVE);

        if (strcmp(clave, claveGuardada) == 0) {
            ingresa = 1;
        } else {
            printf("\n\n\tClave incorrecta\n");
            intento++;
            getchar(); // Espera a que el usuario presione Enter
        }
    } while (intento < MAX_INTENTOS && ingresa == 0);

    if (ingresa == 1) {
        printf("\n\n\tBienvenido al Sistema\n");
        int opcion;

        do {
            mostrarMenu();
            printf("\n\tSeleccione una opcion: ");
            scanf("%d", &opcion);

            switch (opcion) {
                case 1:
                    AutoFantastico();
                    break;
                case 2:
                    Choque();
                    break;
                case 3:
                   //parpadeoAlternado();
                    break;
                case 4:
                    //olaOceanica();
                    break;
                case 5:
                    printf("\n\tSaliendo...\n");
                    break;
                default:
                    printf("\n\tOpcion no valida\n");
            }

            getchar(); // Limpiar el buffer de entrada
            printf("\n\n\tPresione Enter para continuar...");
            getchar(); // Espera a que el usuario presione Enter

            if (opcion == 5) // Salir del bucle si el usuario selecciona la opción de salir
                break;

            system(CLEAR); // Limpiar la pantalla antes de mostrar el menú nuevamente
        } while (1);
    } else {
        printf("\n\n\tHa sobrepasado el número máximo de intentos permitidos\n");
    }

    return 0;
}

/* #include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#define CLEAR "cls"
void delay(unsigned long int milliseconds) {
    Sleep(milliseconds);
}
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#define CLEAR "clear"
void delay(unsigned long int microseconds) {
    usleep(microseconds);
}
#endif

#define TECLA_ENTER 13
#define TECLA_BACKSPACE 8
#define LONGITUD_CLAVE 5
#define MAX_INTENTOS 3
#define SPEED_INCREMENT 100000

int speed = 500000; // 500ms

#ifndef _WIN32
int kbhit(void) {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

char getch(void) {
    char c;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}
#endif

void ocultarEntrada(char* clave, int longitud) {
    int i = 0;
    char caracter;

#ifdef _WIN32
    while (1) {
        caracter = getch();

        if (caracter == TECLA_ENTER) {
            clave[i] = '\0';
            break;
        }
        else if (caracter == TECLA_BACKSPACE) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        }
        else {
            if (i < longitud) {
                printf("*");
                clave[i] = caracter;
                i++;
            }
        }
    }
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (1) {
        caracter = getchar();

        if (caracter == '\n' || caracter == '\r') {
            clave[i] = '\0';
            break;
        }
        else if (caracter == 127 || caracter == TECLA_BACKSPACE) {
            if (i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
        }
        else {
            if (i < longitud) {
                printf("*");
                fflush(stdout);
                clave[i] = caracter;
                i++;
            }
        }
    }

    newt.c_lflag |= (ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
#endif
}

void autoFantastico() {
    int leds[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
    int num_leds = sizeof(leds) / sizeof(leds[0]);

    printf("Auto fantástico en ejecución. Presiona 'q' para salir.\n");
    while (1) {
        for (int i = 0; i < num_leds; i++) {
            for (int j = 0; j < num_leds; j++) {
                if (j == i) {
                    printf("* ");
                } else {
                    printf("- ");
                }
            }
            printf("\n");
            fflush(stdout);
#ifdef _WIN32
            delay(speed / 1000);  // Convert microseconds to milliseconds for Windows
#else
            delay(speed);
#endif

            if (kbhit()) {
                char c = getch();
                if (c == 'q') {
                    printf("\nSaliendo de Auto fantástico...\n");
                    return;
                } else if (c == 'u' && speed > SPEED_INCREMENT) {
                    speed -= SPEED_INCREMENT;
                } else if (c == 'd') {
                    speed += SPEED_INCREMENT;
                }
            }
        }
        for (int i = num_leds - 2; i > 0; i--) {
            for (int j = 0; j < num_leds; j++) {
                if (j == i) {
                    printf("* ");
                } else {
                    printf("- ");
                }
            }
            printf("\n");
            fflush(stdout);
#ifdef _WIN32
            delay(speed / 1000);  // Convert microseconds to milliseconds for Windows
#else
            delay(speed);
#endif

            if (kbhit()) {
                char c = getch();
                if (c == 'q') {
                    printf("\nSaliendo de Auto fantástico...\n");
                    return;
                } else if (c == 'u' && speed > SPEED_INCREMENT) {
                    speed -= SPEED_INCREMENT;
                } else if (c == 'd') {
                    speed += SPEED_INCREMENT;
                }
            }
        }
    }
}

void choque() {
    const char *tabla_choque[] = {
            "* - - - - - - *",
            " * - - - - - * ",
            "- * - - - - * -",
            "- -* - - - *- -",
            "- - * - - *- - ",
            "- - -* - *- - -",
            "- - - * *- - - ",
            "- - - -* - - - ",
            "- - - * *- - - ",
            "- - -* - *- - -",
            "- -* - - * - -",
            "- * - - - *- -",
            " * - - - - * -",
            "* - - - - - * ",
            "* - - - - - - *"
    };

    int num_secuencias = sizeof(tabla_choque) / sizeof(tabla_choque[0]);

    printf("Secuencia de Choque. Presiona 'q' para salir.\n");

    while (1) {
        for (int i = 0; i < num_secuencias; i++) {
            printf("%s\n", tabla_choque[i]);
            fflush(stdout);
#ifdef _WIN32
            delay(speed / 1000);
#else
            delay(speed);
#endif

            if (kbhit()) {
                char c = getch();
                if (c == 'q') {
                    printf("Saliendo de Secuencia de Choque...\n");
                    return;
                } else if (c == 'u' && speed > SPEED_INCREMENT) {
                    speed -= SPEED_INCREMENT;
                } else if (c == 'd') {
                    speed += SPEED_INCREMENT;
                }
            }
        }
    }
}


void parpadeoAlternado() {
    int leds[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
    int num_leds = sizeof(leds) / sizeof(leds[0]);

    printf("Parpadeo alternado en ejecución. Presiona 'q' para salir.\n");
    while (1) {
        // Patrón 1: LED 1, 3, 5, 7 encendidos; LED 2, 4, 6, 8 apagados
        for (int i = 0; i < num_leds; i++) {
            if (i % 2 == 0) {
                printf("* ");
            } else {
                printf("- ");
            }
        }
        printf("\n");
        fflush(stdout);
#ifdef _WIN32
        delay(speed / 1000);  // Convert microseconds to milliseconds for Windows
#else
        delay(speed);
#endif

        if (kbhit()) {
            char c = getch();
            if (c == 'q') {
                printf("\nSaliendo de Parpadeo alternado...\n");
                return;
            } else if (c == 'u' && speed > SPEED_INCREMENT) {
                speed -= SPEED_INCREMENT;
            } else if (c == 'd') {
                speed += SPEED_INCREMENT;
            }
        }

        // Patrón 2: LED 2, 4, 6, 8 encendidos; LED 1, 3, 5, 7 apagados
        for (int i = 0; i < num_leds; i++) {
            if (i % 2 == 1) {
                printf("* ");
            } else {
                printf("- ");
            }
        }
        printf("\n");
        fflush(stdout);
#ifdef _WIN32
        delay(speed / 1000);  // Convert microseconds to milliseconds for Windows
#else
        delay(speed);
#endif

        if (kbhit()) {
            char c = getch();
            if (c == 'q') {
                printf("\nSaliendo de Parpadeo alternado...\n");
                return;
            } else if (c == 'u' && speed > SPEED_INCREMENT) {
                speed -= SPEED_INCREMENT;
            } else if (c == 'd') {
                speed += SPEED_INCREMENT;
            }
        }
    }
}

void olaOceanica() {
    // Tabla de datos para la secuencia de ola
    const char *tabla_secuencia[] = {
        "* - - - - - - -",
        "* * - - - - - -",
        "* * * - - - - -",
        "* * * * - - - -",
        "* * * * * - - -",
        "* * * * * * - -",
        "* * * * * * * -",
        "* * * * * * * *",
        "- * * * * * * *",
        "- - * * * * * *",
        "- - - * * * * *",
        "- - - - * * * *",
        "- - - - - * * *",
        "- - - - - - * *",
        "- - - - - - - *",
        "- - - - - - - -"
    };

    // Variables locales
    int num_secuencias = sizeof(tabla_secuencia) / sizeof(tabla_secuencia[0]);
    int speed = 500000; // 0.5 segundos

    printf("Secuencia de Ola. Presiona 'q' para salir.\n");

    while (1) {
        for (int i = 0; i < num_secuencias; i++) {
            printf("%s\n", tabla_secuencia[i]);
            fflush(stdout);
            //usleep(speed);  para MacOS
           // _sleep(speed); // para Windows
#ifdef _WIN32
            delay(speed / 1000);
#else
            delay(speed);
#endif

            if (kbhit()) {
                char c = getch();
                if (c == 'q') {
                    printf("Saliendo de Secuencia de Ola...\n");
                    return;
                } else if (c == 'u' && speed > SPEED_INCREMENT) {
                    speed -= SPEED_INCREMENT;
                } else if (c == 'd') {
                    speed += SPEED_INCREMENT;
                }
            }
        }
    }
}

void mostrarMenu() {
    printf("\n\n\tMENU\n");
    printf("\t----\n");
    printf("\t1. El Auto Fantastico\n");
    printf("\t2. El Choque\n");
    printf("\t3. Parpadeo Alternado\n");
    printf("\t4. Ola oceánica\n");
    printf("\t5. Salir\n");
}

int main() {
    char clave[LONGITUD_CLAVE + 1];
    int intento = 0;
    int ingresa = 0;

    // Contraseña única guardada en memoria
    char claveGuardada[] = "12345";

    do {
        system(CLEAR);
        printf("\n\tCLAVE: ");
        ocultarEntrada(clave, LONGITUD_CLAVE);

        if (strcmp(clave, claveGuardada) == 0) {
            ingresa = 1;
        } else {
            printf("\n\n\tClave incorrecta\n");
            intento++;
            getchar(); // Espera a que el usuario presione Enter
        }
    } while (intento < MAX_INTENTOS && ingresa == 0);

    if (ingresa == 1) {
        printf("\n\n\tBienvenido al Sistema\n");
        int opcion;

        do {
            mostrarMenu();
            printf("\n\tSeleccione una opcion: ");
            scanf("%d", &opcion);

            switch (opcion) {
                case 1:
                    autoFantastico();
                    break;
                case 2:
                    choque();
                    break;
                case 3:
                    parpadeoAlternado();
                    break;
                case 4:
                    olaOceanica();
                    break;
                case 5:
                    printf("\n\tSaliendo...\n");
                    break;
                default:
                    printf("\n\tOpcion no valida\n");
            }

            getchar(); // Limpiar el buffer de entrada
            printf("\n\n\tPresione Enter para continuar...");
            getchar(); // Espera a que el usuario presione Enter

            if (opcion == 5) // Salir del bucle si el usuario selecciona la opción de salir
                break;

            system(CLEAR); // Limpiar la pantalla antes de mostrar el menú nuevamente
        } while (1);
    } else {
        printf("\n\n\tHa sobrepasado el número máximo de intentos permitidos\n");
    }

    return 0;
}
*/