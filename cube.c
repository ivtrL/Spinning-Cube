#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
float A, B, C;

float cuboLargura = 10;
int largura = 160, altura = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCII = ' ';
int distanciaCamera = 100;
float K1 = 40;

float incrementarVelocidade = 1;

float x, y, z;
float ooz;
int xp, yp;
int idx;

float calcularX(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) + 
    j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calcularY(int i, int j, int k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) - 
    j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) - 
    i * cos(B) * sin(C);
}

float calcularZ(int i, int j, int k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + 
    i * sin(B);
}

void calcularSuperficie(float cuboX, float cuboY, float cuboZ, int ch) {
    x = calcularX(cuboX, cuboY, cuboZ);
    y = calcularY(cuboX, cuboY, cuboZ);
    z = calcularZ(cuboX, cuboY, cuboZ) + distanciaCamera;

    ooz = 1 / z;

    xp = (int)(largura/2 - 2 * cuboLargura + K1 * x * ooz * 2);
    yp = (int)(altura/2 + K1 * y * ooz);

    idx = xp + yp * largura;
    if (idx >= 0 && idx < largura * altura) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() { 
    printf("\x1b[2J");
    while (1) {
        memset(buffer, backgroundASCII, largura * altura);
        memset(zBuffer, 0, largura * altura * 4);
        for(float cuboX = -cuboLargura; cuboX < cuboLargura; cuboX += incrementarVelocidade) {
            for (float cuboY = -cuboLargura; cuboY < cuboLargura; cuboY += incrementarVelocidade) {
                calcularSuperficie(cuboX, cuboY, -cuboLargura, '.') ;
                calcularSuperficie(cuboLargura, cuboY, cuboX, '$') ;
                calcularSuperficie(-cuboLargura, cuboY, -cuboX, '~') ;
                calcularSuperficie(-cuboX, cuboY, cuboLargura, '#') ;
                calcularSuperficie(cuboX, -cuboLargura, -cuboY, ';') ;
                calcularSuperficie(cuboX, cuboLargura, cuboY, '+') ;
            }
        }
        printf("\x1b[H");
        for ( int k = 0; k < largura * altura; k++) {
            putchar(k % largura ? buffer[k] : 10);
        }

        A += 0.005;
        B += 0.005;
        usleep(500);
    }
    return 0; 
}