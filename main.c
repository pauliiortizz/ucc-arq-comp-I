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
//#include "assembly_functions.h"

#define CLEAR "clear"   // Comando para limpiar la pantalla en la terminal.

#define PASSWORD "12345"
#define MAX_INTENTOS 3

#define TECLA_ENTER 13   // Código ASCII para la tecla ENTER.
#define TECLA_BACKSPACE 8
#define LONGITUD_CLAVE 5    // Longitud máxima para la clave de entrada.

// Declaración de funciones.
void mostrarMenu();

int kbhit(void);

void delay(int *);

void delay(int *a){   // Función para crear un retardo.
 int i;
    unsigned int j = 0x2fffff; //valor hexadecimal muy grande
    for (i = 0; i < *a; i++) {
        for (j = 0; j < 70000; ++j) {  //consume tiempo iterando hasta completar los 70000 ciclos.
        
        }
        //while (j) j--;
    }
}

void ocultarEntrada(char *clave, int longitud);

void disp_binary(int);

int leds(int num);  // Función para controlar los LEDs.

const char led[] = {7, 8, 25, 24, 23, 18, 15, 14};   // Array con los pines de GPIO para los LEDs.

/*int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    
    while ((ch = getchar()) != EOF) {
        if (ch != 100 && ch != 117) {
            ungetc(ch, stdin);
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            fcntl(STDIN_FILENO, F_SETFL, oldf);
            return 1;
        }
    }

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}*/

int leds(int num) {   // Función para activar los LEDs basada en el valor binario de num.
    int i, numval;
    for (i = 0; i < 8; i++) {
        numval = (num >> i) & 0x01;
        digitalWrite(led[i], numval);
    }

    delayMillis(300);   // Retardo después de cambiar el estado de los LEDs.

    return 0;
}

/*void delay(unsigned long int *velocidad) {

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
}*/

void AutoFantastico(void) {
    unsigned char output;
    int t, j = 1;
    static int tiempo=2000;

    initscr(); // Iniciar ncurses
    clear();  // Limpiar pantalla
    noecho(); // No mostrar teclas presionadas
    keypad(stdscr, TRUE); // Habilitar teclas especiales

    printw("Secuencia de auto fantastico ... Oprima 'e' para finalizar\n");

    do {
        output = 0x80;
        for (t = 0; t < 8; t++) {
            keypad(stdscr, TRUE);
                nodelay(stdscr, TRUE);   // No bloquear mientras espera la entrada del teclado.
                switch (getch()) {
                case 'e':
                endwin();
                    return;
                case KEY_UP:
                tiempo = tiempo - 1000;
                
                break;
                case KEY_DOWN:
                tiempo = tiempo + 1000;
                
                break;
                }
            disp_binary(output);   // Mostrar el estado actual en los LEDs.
            delay(&tiempo);
            output = output >> 1;  // Desplazar el bit a la derecha (CORRIMIENTO).
        }
     // Similar al bucle anterior, pero ahora moviendo la luz en la dirección opuesta.
        output = 0x02;
        for (t = 0; t < 6; t++) {
            keypad(stdscr, TRUE);
                nodelay(stdscr, TRUE);
                switch (getch()) {
                case 'e':
                endwin();
                    return;
                case KEY_UP:
                tiempo = tiempo - 1000;
                
                break;
                case KEY_DOWN:
                tiempo = tiempo + 1000;
                
                break;
                }
            disp_binary(output);
            delay(&tiempo);
            
            output = output << 1;  // Desplazar el bit a la izquierda (CORRIMIENTO)
        }
    } while (1);
    
    endwin(); // Finalizar ncurses
}


void choque(void) {
    static int tiempo=2000;
    initscr(); // Iniciar ncurses
    clear();
    noecho(); // No mostrar teclas presionadas
    keypad(stdscr, TRUE); // Habilitar teclas especiales

    unsigned char tabla[7] = {0x81, 0x42, 0x24, 0x18, 0x24, 0x42, 0x81};  // Patrón de choque en los LEDs.

    printw("Secuencia de choque ... Oprima 'e' para finalizar\n");

    while (1) {
        for (int i = 0; i < 7; i++) { // Iterar a través de cada estado del patrón de choque.
            keypad(stdscr, TRUE);
                nodelay(stdscr, TRUE);
                switch (getch()) {
                case 'e':
                endwin();
                    return;
                case KEY_UP:
                tiempo = tiempo - 1000;
                
                break;
                case KEY_DOWN:
                tiempo = tiempo + 1000;
                
                break;
                }
            disp_binary(tabla[i]);
            delay(&tiempo);
        }
    }
 // Repetir indefinidamente hasta que se presione 'e'.
    endwin(); // Finalizar ncurses
}

void parpadeoAlternado(void) {
    unsigned int output;
    static int tiempo=2000;  // Variable estática para mantener el tiempo entre parpadeos.

    initscr(); // Iniciar ncurses
    clear();
    noecho(); // No mostrar teclas presionadas
    keypad(stdscr, TRUE); // Habilitar teclas especiales

    printw("Secuencia de parpadeo alternado ... Oprima 'e' para finalizar\n");

    do {
        // Mostrar posiciones impares (10101010)
        output = 0xAA; // 10101010 en binario, representa las posiciones impares
        keypad(stdscr, TRUE);
                nodelay(stdscr, TRUE);  // No bloquear el programa esperando entrada del usuario.
                switch (getch()) {
                case 'e':
                endwin();
                    return;
                case KEY_UP:
                tiempo = tiempo - 1000;
                
                break;
                case KEY_DOWN:
                tiempo = tiempo + 1000;
                
                break;
                }
        disp_binary(output);
        delay(&tiempo);

        // Mostrar posiciones pares (01010101)
        output = 0x55; // 01010101 en binario, representa las posiciones pares
        keypad(stdscr, TRUE);
                nodelay(stdscr, TRUE);
                switch (getch()) {
                case 'e':
                endwin();
                    return;
                case KEY_UP:
                tiempo = tiempo - 1000;
                
                break;
                case KEY_DOWN:
                tiempo = tiempo + 1000;
                
                break;
                }
        disp_binary(output);
        delay(&tiempo);
        
    } while (1); // Repetir indefinidamente hasta que se presione 'e'.

    endwin(); // Finalizar ncurses
}

void olaOceanica(void) {
    static int tiempo=2000;
    
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
        for (int i = 0; i < sizeof(tabla); i++) {   // Iterar a través de cada estado de la "ola".
            keypad(stdscr, TRUE);
                nodelay(stdscr, TRUE);
                switch (getch()) {
                case 'e':
                endwin();
                    return;
                case KEY_UP:
                tiempo = tiempo - 1000;
                
                break;
                case KEY_DOWN:
                tiempo = tiempo + 1000;
                
                break;
                }
            disp_binary(tabla[i]);
            delay(&tiempo);
        }
    } while (1);   // Repetir indefinidamente hasta que se presione 'e'.

    endwin(); // Finalizar ncurses
}

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

    pioInit();   // Inicializar GPIO para controlar los pines de la Raspberry Pi.

    for (int i = 0; i < 8; i++) {  // Configurar cada pin como salida.
        pinMode(led[i], OUTPUT);
    }

    leds(0xFF);  // Encender todos los LEDs al inicio.

    // Contrasenia unica guardada en memoria
    char claveGuardada[] = "11111";

    do {
        system(CLEAR);
        printf("\n\tCLAVE: ");
        ocultarEntrada(clave, LONGITUD_CLAVE);  // Pedir y ocultar la entrada de la clave.

        if (strcmp(clave, claveGuardada) == 0) {   // Comparar la clave ingresada con la guardada.
            ingresa = 1;
        } else {
            printf("\n\n\tClave incorrecta\n");
            intento++;
            getchar(); // Espera a que el usuario presione Enter
        }
    } while (intento < MAX_INTENTOS && ingresa == 0);   // Repetir mientras no se excedan los intentos y no se ingrese correctamente.

    if (ingresa == 1) {     // Si la entrada es correcta, mostrar el menú y permitir la selección de animaciones.
        printf("\n\n\tBienvenido al Sistema\n");
        int opcion;

        do {
            mostrarMenu();
            printf("\n\tSeleccione una opcion: ");
            scanf("%d", &opcion);  // Leer la opción del usuario

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

            system(CLEAR); // Limpiar la pantalla antes de mostrar el menu nuevamente
            /*printf("\n\n\tPresione Enter para continuar...");
            getchar(); // Espera a que el usuario presione Enter*/

            if (opcion != 1 && opcion!=2 && opcion!=3 && opcion!=4) {
                printf("\n\tSaliendo...\n");  // Salir del bucle si el usuario selecciona la opción de salir
                break;
            }

            system(CLEAR); // Limpiar la pantalla antes de mostrar el menu nuevamente
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
    tcgetattr(STDIN_FILENO, &oldt);   // Obtener la configuración actual del terminal.
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);  // Desactivar eco y modo canónico.
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Establecer la nueva configuración.

    while (1) {
        caracter = getchar();   // Leer un carácter.

        if (caracter == '\n' || caracter == '\r') {   // Si es Enter, terminar.
            clave[i] = '\0';
            break;
        } else if (caracter == 127 || caracter == TECLA_BACKSPACE) { // Si es retroceso, borrar el último carácter.
            if (i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
        } else {
            if (i < longitud) {   // Si no se ha alcanzado la longitud máxima, agregar el carácter a la clave.
                printf("*");
                fflush(stdout);
                clave[i] = caracter;
                i++;
            }
        }
    }

    newt.c_lflag |= (ECHO | ICANON);   // Restaurar la configuración del terminal.
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // Aplicar la configuración restaurada.
}

void disp_binary(int i) {
    int t;
    for (t = 128; t > 0; t = t / 2)
        if (i & t) printw("*");
        else printw("_");
    printw("\n");
    leds(i);
}
