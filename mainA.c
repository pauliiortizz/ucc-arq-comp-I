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
#include "assembly_functions.h"

#define CLEAR "clear"

#define PASSWORD "12345"
#define MAX_INTENTOS 3

#define TECLA_ENTER 13
#define TECLA_BACKSPACE 8
#define LONGITUD_CLAVE 5

//extern void *parpadeoAlternado();
//extern void *olaOceanica(void);
void mostrarMenu();

int my_getch(void);

int kbhit(void);

void delay(unsigned long int *velocidad);

void ocultarEntrada(char *clave, int longitud);

void disp_binary();

int leds(int num);

const char led[] = {7, 8, 25, 24, 23, 18, 15, 14};

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

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

    if (ch == KEY_DOWN) {     // ASCII de la 'd'
        *velocidad += 200;
    }
    if (*velocidad > 200 && ch == KEY_UP) {    // ASCII de la 'u'
        *velocidad -= 200;
    }

    usleep(*velocidad * 1000);   // Retardo ajustado

    endwin();
}


///////////////// Delay para las funciones en assembly ///////////////////
int delayAssembly(int tiempo){
 int i;
 int input1, input2;
 int resultado = tiempo;
 
    unsigned int j = 0x3fffff; //raspberry 0x3fffff;
    for (i = 0; i < tiempo; i++) {
        for (j = 0; j < 65535; ++j) {
            if (kbhit()) {
                input1 = getch();

                if (KEY_UP == 224){

                input2 = getch();

                if (input2 != 72 && input2 != 80) {
                    return resultado = 0 ;
                }

                if (input2 == 72) { // si input es up key 
                    resultado = resultado - 500;
                }

                if (input2 == 80) { // si input es down key
                    resultado = resultado + 500;
                }  

                } else {
                    return resultado = 0;
                } 
               
                }
        }
        while (j) j--;
                keypad(stdscr, TRUE);
                nodelay(stdscr, TRUE);
                switch (getch()) {
                case 'a':
                    return 0;
                case KEY_UP:
                resultado = tiempo - 500;
                break;
                case KEY_DOWN:
                resultado = tiempo + 500;
                break;
                }
                

    return resultado;
}
}

void AutoFantastico(void) {
    unsigned long int speed1 = 500;
    unsigned char output;
    int t, j = 1;

    initscr(); // Iniciar ncurses
    clear();
    noecho(); // No mostrar teclas presionadas
    keypad(stdscr, TRUE); // Habilitar teclas especiales

    printw("Secuencia de auto fantastico ... Oprima 'e' para finalizar\n");

    do {
        output = 0x80;
        for (t = 0; t < 8; t++) {
            disp_binary(output);
            leds(output);
            delay(&speed1);
            if (kbhit() && getchar() == 'e') { // 'e' en ASCII
                endwin();
                return;
            }
            output = output >> 1;
            refresh();
        }
        output = 0x02;
        for (t = 0; t < 6; t++) {
            disp_binary(output);
            leds(output);
            delay(&speed1);
            if (kbhit() && getchar() == 'e') { // 'e' en ASCII
                endwin();
                return;
            }
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
    clear();
    noecho(); // No mostrar teclas presionadas
    keypad(stdscr, TRUE); // Habilitar teclas especiales

    unsigned char tabla[7] = {0x81, 0x42, 0x24, 0x18, 0x24, 0x42, 0x81};

    printw("Secuencia de choque ... Oprima 'e' para finalizar\n");

    while (1) {
        for (int i = 0; i < 7; i++) {
            disp_binary(tabla[i]);
            leds(tabla[i]);
            delay(&speed2);
            refresh();
            if (kbhit() && getchar() == 'e') {
                endwin();
                return;
            }
        }
    }
    printw("Chau...\n");
    refresh();
    endwin(); // Finalizar ncurses
}

/* void parpadeoAlternado(void) {
    unsigned long int speed3 = 500;
    unsigned int output;

    initscr(); // Iniciar ncurses
    clear();
    noecho(); // No mostrar teclas presionadas
    keypad(stdscr, TRUE); // Habilitar teclas especiales

    printw("Secuencia de parpadeo alternado ... Oprima 'e' para finalizar\n");

    do {
        // Mostrar posiciones impares
        output = 0xAA; // 10101010 en binario, representa las posiciones impares
        disp_binary(output);
        leds(output);
        delay(&speed3);
        refresh();

        if (kbhit() && getchar() == 'e') { // 'e' en ASCII
            endwin();
            return;
        }

        // Mostrar posiciones pares
        output = 0x55; // 01010101 en binario, representa las posiciones pares
        disp_binary(output);
        leds(output);
        delay(&speed3);
        refresh();

        if (kbhit() && getchar() == 'e') { // 'e' en ASCII
            endwin();
            return;
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
    clear();
    noecho(); // No mostrar teclas presionadas
    keypad(stdscr, TRUE); // Habilitar teclas especiales

    unsigned char tabla[] = {
            0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, // Ola moviéndose de izquierda a derecha
            0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00  // Ola moviéndose de derecha a izquierda
    };

    printw("Secuencia de ola oceanica ... Oprima 'e' para finalizar\n");

    do {
        for (int i = 0; i < sizeof(tabla); i++) {
            disp_binary(tabla[i]);
            delay(&speed4);
            refresh();
            //usleep(500000); // 0.5 segundos

            if (kbhit() && getchar() == 'e') { // 'e' en ASCII
                endwin();
                return;
            }
        }
    } while (1); // Repite hasta que se toque una tecla

    endwin(); // Finalizar ncurses
}
*/

void mostrarMenu() {
    printf("\n\n\tMENU\n");
    printf("\t----\n");
    printf("\t1. El Auto Fantastico\n");
    printf("\t2. El Choque\n");
    printf("\t3. Parpadeo Alternado\n");
    printf("\t4. Ola oceanica\n");
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
    char claveGuardada[] = "11111";

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
                    break;
                default:
                    break;
            }

            //system(CLEAR); // Limpiar la pantalla antes de mostrar el menu nuevamente
            printf("\n\n\tPresione Enter para continuar...");
            getchar(); // Espera a que el usuario presione Enter

            if (opcion != 1 && opcion!=2 && opcion!=3 && opcion!=4) {
                printf("\n\tSaliendo...\n");  // Salir del bucle si el usuario selecciona la opción de salir
                break;
            }

            //system(CLEAR); // Limpiar la pantalla antes de mostrar el menu nuevamente
        } while (1);
    } else {
        printf("\n\n\tHa sobrepasado el número máximo de intentos permitidos\n");
    }

    return 0;
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


void disp_binaryAssembly(int i) {
    int t;
    int n = 0;
    const char led[] = {7, 8, 25, 24, 23, 18, 15, 14};
    //system("cls");
    printf("\033[2J");
    unsigned int k = 10;
    while (k--)
    printf("\033[F");
    
    for (t = 128; t > 0; t = t/2) {

        if ( i & t) {
             printf("*");
            digitalWrite(led[n], 1);
        } else {
             printf("_");
            digitalWrite(led[n], 0);
        }
        printw("\n");
        leds(i);

        //n++;
    }
    
    printf("\n\r");
    //printf("\033[1;30mPresione la tecla E para salir\033[0m\n\r");
} 
