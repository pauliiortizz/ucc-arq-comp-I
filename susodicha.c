#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>
#include "EasyPIO.h"


void retardo(unsigned long int); // da tiempo entre las luces
void simular_led(unsigned char);
int fvelocidad(unsigned long int); // aumenta o disminuye velocidad

void auto_fantastico(unsigned long int);
void choque(unsigned long int);
void bateriaDescargandose(unsigned long int);
void chocaLos5(unsigned long int);

int main()
{
    char correct_password[] = "12345";
    char password[6];
    int intentos = 0;
    int acceso = 0;

    while (intentos < 3 && !acceso) {
        int index = 0;
        char ch;

        printf("Ingrese su password de 5 digitos: ");

        // leer la contraseÃ±a
        while (index < 5) {
            ch = _getch();

            //ignorar el enter
            if (ch == '\r') {
                continue;
            }
                //hacer que borre el caracter con el backspace y que no lo tome como parte de la pw
            else if (ch == 8) {
                if (index > 0) {
                    index--;
                    printf("\b \b");  //mueve el cursor para escribir 1 index atrÃ¡s
                }
            }
                //
            else if (index < 5) {  //para que finalice al ingresar 5 caracteres
                password[index++] = ch;
                putchar('*');
            }
        }

        password[index] = '\0';  // agrega caracter nulo para terminar la cadena
        printf("\n");

        // compara la pw ingresada con la guardada
        if (strcmp(password, correct_password) == 0) {
            system("cls");
            printf("Bienvenid@ al Sistema!\n");
            acceso = 1;
        } else {
            printf("Password no valida, intente de nuevo\n");
            intentos++;
        }
    }

    if (!acceso) {
        printf("Has excedido el numero maximo de intentos. El programa abortara.\n");
        return;
    }

    int opcion = 0;
    unsigned long int velocidadAux = 145000000;
    unsigned long int velocidad = velocidadAux;

    do
    {

        printf("\n1- Auto fantastico\n");
        printf("2- El choque\n");
        printf("3- Bateria descargandose\n");
        printf("4- Choca los 5\n");
        printf("5- Salir\n");

        printf("\nIngrese la opcion que desee visualizar: ");
        scanf("%d", &opcion);
        while (opcion < 1 || opcion > 6)
        {
            printf("Valor erroneo, ingrese nuevamente: ");
            scanf("%d", &opcion);
        }

        switch (opcion)
        {
            case 1:
                system("cls");
                auto_fantastico(velocidad);
                opcion = 0;
                velocidad = velocidadAux;
                break;
            case 2:
                system("cls");
                choque(velocidad);
                opcion = 0;
                velocidad = velocidadAux;
                break;
            case 3:
                system("cls");
                bateriaDescargandose(velocidad);
                opcion = 0;
                velocidad = velocidadAux;
                break;
            case 4:
                system("cls");
                chocaLos5(velocidad);
                opcion = 0;
                velocidad = velocidadAux;
                break;
            case 5:
                system("cls");
                printf("Sistema apagado");
                break;
            default:
                system("cls");
                printf("No existe secuencia para ese numero\n");
                opcion = 0;

        }
    } while (opcion != 6);

    return 0;
}

void retardo(unsigned long int a)
{
    while (a){
        a--;
    }

}

void simular_led(unsigned char dato)
{
    for (unsigned char mascara = 128; mascara > 0; mascara >>= 1)
    {
        if (dato & mascara)
        {
            printf("*");
        }
        else
        {
            printf("_");
        }
    }
}

//incluir esta funcion dentro del retardo
int fvelocidad(unsigned long int velocidad)
{
    if ((velocidad - 4800000) > 9600000)
    {
        if (GetAsyncKeyState(VK_UP) & 0x0001)
        {
            velocidad -= 4800000;
        }
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x0001)
    {
        velocidad += 4800000;
    }
    return velocidad;
}

void auto_fantastico(unsigned long int velocidad)
{
    //por algoritmo
    unsigned int dato;
    while (1)
    {
        dato = 0x80;
        for (int t = 0; t < 8; t++)
        {
            printf("Presione ESC para regresar al menu principal\n");
            printf("Pulse la flecha para arriba para incrementar la velocidad, o para abajo para disminuirla\n");
            printf("Demora: %lu\n", velocidad);
            simular_led(dato);
            retardo(velocidad);
            system("cls");
            dato = dato >> 1;
            //cambiar de lugar el aceleracion/desaceleracion
            velocidad = fvelocidad(velocidad);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
            {
                return;
            }
        }
        dato = 0x01;
        for (int i = 0; i < 6; i++)
        {
            printf("Presione ESC para regresar al menu principal\n");
            printf("Pulse la flecha para arriba para incrementar la velocidad, o para abajo para disminuirla\n");
            printf("Demora: %lu\n", velocidad);
            dato = dato << 1;
            simular_led(dato);
            retardo(velocidad);
            system("cls");
            velocidad = fvelocidad(velocidad);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
            {
                return;
            }
        }
    }
}

void choque(unsigned long int velocidad)
{
    //por tabla
    unsigned char tabla[] = {
            0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42};

    while (1)
    {
        for (int i = 0; i < 7; ++i)
        {
            printf("Presione ESC para volver al menu principal\n");
            printf("Pulse la flecha para arriba para incrementar la velocidad, o para abajo para disminuirla\n");
            printf("Demora: %lu\n", velocidad);
            simular_led(tabla[i]);
            retardo(velocidad);
            system("cls");

            velocidad = fvelocidad(velocidad);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
            {
                return;
            }
        }
    }
}

void bateriaDescargandose(unsigned long int velocidad)
{
    //por algoritmo
    unsigned int dato = 0xFF;
    while (1)
    {
        for (int i = 0; i < 9; ++i)
        {
            printf("Presione ESC para regresar al menu principal\n");
            printf("Demora: %lu\n", velocidad);
            simular_led(dato);
            dato <<= 1;
            retardo(velocidad);
            system("cls");

            velocidad = fvelocidad(velocidad);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
            {
                return;
            }
        }
        dato = 0xFF;
    }
}

void chocaLos5(unsigned long int velocidad)
{
    //por algoritmo
    unsigned char tabla[] = {
            0x00, 0x81, 0xC3, 0xE7, 0xFF, 0xE7, 0xC3, 0x81};
    while (1)
    {
        for (int i = 0; i < 8; i++)
        {
            printf("Presione ESC para regresar al menu principal\n");
            printf("Pulse la flecha para arriba para incrementar la velocidad, o para abajo para disminuirla\n");
            printf("Demora: %lu\n", velocidad);
            simular_led(tabla[i]);
            retardo(velocidad);
            system("cls");

            velocidad = fvelocidad(velocidad);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
            {
                return;
            }
        }
    }
}
