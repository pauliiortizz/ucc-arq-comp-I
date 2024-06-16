#include "EasyPIO.h"
#include <stdio.h>
#include <termios.h>
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

int main();

void mostrarMenu();

int my_getch(void);

int kbhit(void);

void delay(unsigned long int *);

void ocultarEntrada(char *clave, int longitud);

void disp_binary(int);

int leds(int num);

const char led[] = {7, 8, 25, 24, 23, 18, 15, 14};

int leds(int num) {
    int i, numval;
    for (i = 0; i < 8; i++) {
        numval = (num >> i) & 0x01;
        digitalWrite(led[i], numval);
    }

    delayMillis(300);

    return 0;
}

void delay(unsigned long int *velocidad) {

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE); // No espera por la entrada del usuario
    keypad(stdscr, TRUE);

    int ch = getch();

    if (ch == KEY_DOWN) {
        *velocidad += 50;
    }if (*velocidad > 50 && ch == KEY_UP) {
        *velocidad -= 50;
    }

    usleep(*velocidad * 1000);

    endwin();
        /*
        while (*velocidad > 0) {
            if(kbhit()){
                int ch =my_getch();
                if (ch == KEY_DOWN & 0x8000) {
                    *velocidad -= SPEED_INCREMENT;
                } else if (ch == KEY_UP & 0x8000) {
                    *velocidad += SPEED_INCREMENT;
                } else if (ch=='e'){
                    endwin();
                    system(CLEAR);
                    main();
                    return;
                }else{
                    break;
                }
            }
        }*/
    }

    int my_getch() {
        int ch;
        nodelay(stdscr, TRUE); // No bloquear getch
        ch = getch();
        nodelay(stdscr, FALSE); // Restaurar el bloqueo de getch
        return ch;
    }

    void AutoFantastico(void) {
        unsigned long int speed1 = 500;
        unsigned char output;
        int t, j = 1;
        int on_time = 1;

        initscr(); // Iniciar ncurses
        noecho(); // No mostrar teclas presionadas
        keypad(stdscr, TRUE); // Habilitar teclas especiales

        printw("Secuencia en accion ... Oprima 'e' para finalizar\n");
        refresh();
        do {
            output = 0x80;
            for (t = 0; t < 8; t++) {
                disp_binary(output);
                leds(output);
                delay(&speed1);
                output = output >> 1;
                refresh();
            }
            output = 0x01;
            for (t = 0; t < 6; t++) {
                disp_binary(output);
                leds(output);
                delay(&speed1);
                output = output << 1;
                refresh();
            }
        } while (1);
        output = 0x00;
        disp_binary(output);
        printw("Chau...\n");
        refresh();
        endwin(); // Finalizar ncurses
    }


    void choque(void) {
        unsigned long int speed2 = 500;
        initscr(); // Iniciar ncurses
        noecho(); // No mostrar teclas presionadas
        keypad(stdscr, TRUE); // Habilitar teclas especiales

        printw("Mostrando Choque: \n");

        unsigned char tabla[7] = {0x81, 0x42, 0x24, 0x18, 0x24, 0x42, 0x81};

        while (1) {
            for (int i = 0; i < 7; i++) {
                disp_binary(tabla[i]);
                leds(~tabla[i]);
                delay(&speed2);
                refresh();
                //usleep(1000 * 500); // Convertir a microsegundos (500 ms)
            }
        }

        printw("Chau...\n");
        refresh();
        endwin(); // Finalizar ncurses
    }

    void parpadeoAlternado(void) {
        unsigned long int speed3 = 500;
        unsigned int output;
        int on_time = 1;

        initscr(); // Iniciar ncurses
        noecho(); // No mostrar teclas presionadas
        keypad(stdscr, TRUE); // Habilitar teclas especiales

        printw("Secuencia de parpadeo alternado ... Oprima 'e' para finalizar\n");
        refresh();

        do {
            // Mostrar posiciones impares
            output = 0xAA; // 10101010 en binario, representa las posiciones impares
            disp_binary(output);
            leds(output);
            delay(&speed3);
            refresh();

            // Mostrar posiciones pares
            output = 0x55; // 01010101 en binario, representa las posiciones pares
            disp_binary(output);
            leds(output);
            delay(&speed3);
            refresh();

            if (kbhit()) {
                char c = my_getch();
                if (c == 'e') {
                    break;
                }
            }

        } while (1); // Repite hasta que se toque una tecla

        output = 0x00;
        disp_binary(output);
        leds(output);
        printw("Chau...\n");
        refresh();
        endwin(); // Finalizar ncurses
    }

    void olaOceanica(void) {
        unsigned long int speed4 = 500;
        initscr(); // Iniciar ncurses
        noecho(); // No mostrar teclas presionadas
        keypad(stdscr, TRUE); // Habilitar teclas especiales

        printw("Mostrando Ola Oceánica: \n");

        unsigned char tabla[] = {
                0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, // Ola moviéndose de izquierda a derecha
                0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00  // Ola moviéndose de derecha a izquierda
        };

        do {
            for (int i = 0; i < sizeof(tabla); i++) {
                disp_binary(tabla[i]);
                delay(&speed4);
                refresh();
                usleep(500000); // 0.5 segundos

                if (kbhit()) {
                    char c = my_getch();
                    if (c == 'e') {
                        endwin();
                        return;
                    }
                }
            }
        } while (1); // Repite hasta que se toque una tecla

        endwin(); // Finalizar ncurses
    }

    void mostrarMenu() {
        printf("\n\n\tMENU\n");
        printf("\t----\n");
        printf("\t1. El Auto Fantastico\n");
        printf("\t2. El Choque\n");
        printf("\t3. Parpadeo Alternado\n");
        printf("\t4. Ola oceÃ¡nica\n");
        printf("\t5. Salir\n");
    }

    int main() {

        char clave[LONGITUD_CLAVE + 1];
        int intento = 0;
        int ingresa = 0;

        pioInit(); // Inicializar GPIO

        for (int i = 0; i < 8; i++) {
            pinMode(led[i], OUTPUT);
        }

        leds(0xFF);

        // Contrasenia unica guardada en memoria
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

                if (opcion == 5) // Salir del bucle si el usuario selecciona la opciÃ³n de salir
                    break;

                system(CLEAR); // Limpiar la pantalla antes de mostrar el menu nuevamente
            } while (1);
        } else {
            printf("\n\n\tHa sobrepasado el nÃºmero mÃ¡ximo de intentos permitidos\n");
        }

        return 0;
    }

    int kbhit(void) {
        struct timeval tv = {0L, 0L};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        return select(1, &fds, NULL, NULL, &tv) > 0;
    }

    void ocultarEntrada(char *clave, int longitud) {
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

    void disp_binary(int i) {
        int t;
        for (t = 128; t > 0; t = t / 2)
            if (i & t) printw("*");
            else printw("_");
        printw("\n");
        leds(i);
    }

