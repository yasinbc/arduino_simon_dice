/*
    Autor: Yasin Boulahya Chairi
    Asignatura: Tecnología de Computadores - UNIR
    Actividad 2 Simon Dice en Arduino
    Descripcion: Juego de Simon Dice en Arduino con 4 botones asociado a 4 leds
*/

//Definimos las notas en el sistema internacional y sus frecuencias:
#define NOTA_B0  31
#define NOTA_C1  33
#define NOTA_CS1 35
#define NOTA_D1  37
#define NOTA_DS1 39
#define NOTA_E1  41
#define NOTA_F1  44
#define NOTA_FS1 46
#define NOTA_G1  49
#define NOTA_GS1 52
#define NOTA_A1  55
#define NOTA_AS1 58
#define NOTA_B1  62
#define NOTA_C2  65
#define NOTA_CS2 69
#define NOTA_D2  73
#define NOTA_DS2 78
#define NOTA_E2  82
#define NOTA_F2  87
#define NOTA_FS2 93
#define NOTA_G2  98
#define NOTA_GS2 104
#define NOTA_A2  110
#define NOTA_AS2 117
#define NOTA_B2  123
#define NOTA_C3  131
#define NOTA_CS3 139
#define NOTA_D3  147
#define NOTA_DS3 156
#define NOTA_E3  165
#define NOTA_F3  175
#define NOTA_FS3 185
#define NOTA_G3  196
#define NOTA_GS3 208
#define NOTA_A3  220
#define NOTA_AS3 233
#define NOTA_B3  247
#define NOTA_C4  262
#define NOTA_CS4 277
#define NOTA_D4  294
#define NOTA_DS4 311
#define NOTA_E4  330
#define NOTA_F4  349
#define NOTA_FS4 370
#define NOTA_G4  392
#define NOTA_GS4 415
#define NOTA_A4  440
#define NOTA_AS4 466
#define NOTA_B4  494
#define NOTA_C5  523
#define NOTA_CS5 554
#define NOTA_D5  587
#define NOTA_DS5 622
#define NOTA_E5  659
#define NOTA_F5  698
#define NOTA_FS5 740
#define NOTA_G5  784
#define NOTA_GS5 831
#define NOTA_A5  880
#define NOTA_AS5 932
#define NOTA_B5  988
#define NOTA_C6  1047
#define NOTA_CS6 1109
#define NOTA_D6  1175
#define NOTA_DS6 1245
#define NOTA_E6  1319
#define NOTA_F6  1397
#define NOTA_FS6 1480
#define NOTA_G6  1568
#define NOTA_GS6 1661
#define NOTA_A6  1760
#define NOTA_AS6 1865
#define NOTA_B6  1976
#define NOTA_C7  2093
#define NOTA_CS7 2217
#define NOTA_D7  2349
#define NOTA_DS7 2489
#define NOTA_E7  2637
#define NOTA_F7  2794
#define NOTA_FS7 2960
#define NOTA_G7  3136
#define NOTA_GS7 3322
#define NOTA_A7  3520
#define NOTA_AS7 3729
#define NOTA_B7  3951
#define NOTA_C8  4186
#define NOTA_CS8 4435
#define NOTA_D8  4699
#define NOTA_DS8 4978

//CONSTANTES - define numero de pines para los LEDs, botones, BUZZER (altavoz) y tonos del juego
const byte ledPines[] = {9, 10, 11, 12};
const byte botonPines[] = {2, 3, 4, 5};
#define BUZZER_PIN 8 //numero del pin del BUZZER

#define MAX_LONGITUD 100 //Hasta donde llega la secuencia

const int tonoJuego[] = { NOTA_G3, NOTA_C4, NOTA_E4, NOTA_G5}; //Tono inicial del juego

// Variables globales - Almacenan el estado del juego
byte secuenciaJuego[MAX_LONGITUD] = {0}; //Secuencia del juego
byte indexJuego = 0; //Indice por el que inicia el juego (se va sumando)

//Inicializa la comunicacion en serie de la protoboard con Arduino
void setup(){
    Serial.begin(9600);
    for (byte i = 0; i < 4; i++) {
        pinMode(ledPines[i], OUTPUT);
        pinMode(botonPines[i], INPUT_PULLUP);
    }

    pinMode(BUZZER_PIN, OUTPUT);
    // Generador de numero aleatorios y asume que el PIN A0 esta desconectado
    randomSeed(analogRead(A0));
}

//Enciendo el LED asignado y reproduce el tono requerido
void luzLedTonoJuego(byte ledIndex){
    digitalWrite(ledPines[ledIndex], HIGH);
    tone(BUZZER, tonoJuego[ledIndex]);
    delay(300);
    digitalWrite(ledPines[ledIndex], LOW);
    noTone(BUZZER);
}

//Reproduce la secuencia actual de notas que el jugador tiene que repetir
void reproduceSecuencia() {
    for (int i = 0; i <  indexJuego; i++) {
        byte ledActual = secuenciaJuego[i];
        luzLedTonoJuego(ledActual);
        delay(50);
    }
}

//Espera hasta que el jugador presiona uno de los botones y devuelve el índice de ese boton
byte leeBotones() {
    while (true) {
    for(byte i = 0; i < 4; i++) {
        byte botonPin = botonPines[i];
        if (digitalRead(botonPin) == LOW) {
            return i;
        }
    }
    delay(1);
    }
}


//Reproduce las notas de GAME OVER y reporta la puntuacion del juego
void gameOver() {
    Serial.print("Game over! Tu puntuacion ha sido: ");
    Serial.println(indexJuego - 1);
    indexJuego = 0;
    delay(200);

    //Reproduce el sonido Wah-Wah-Wah-Wah cuando no aciertas
    tone(BUZZER_PIN, NOTE_DS5);
    delay(300);
    tone(BUZZER_PIN, NOTE_D5);
    delay(300);
    tone(BUZZER_PIN, NOTE_CS5);
    delay(300);
    for (byte i = 0; i < 10; i++) {
        for (int pitch = -10; pitch <= 10; pitch++) {
        tone(BUZZER_PIN, NOTA_C5 + pitch);
        delay(5);
        }
    }
    noTone(BUZZER_PIN);
    delay(500);
}

//Obtiene el input del jugador y lo compara la secuencia generada
bool compruebaSecuencia() {
    for (int i = 0; i < indexJuego; i++) {
        byte botonEsperado = secuenciaJuego[i];
        byte botonActual =  leeBotones();
        luzLedTonoJuego(botonActual);
        if (botonEsperado != botonActual) {
        return false;
        }
    }

    return true;
}

//Reproduce sonido GANAR cada vez que el jugador finaliza una secuencia/nivel
void reproduceSonidoFinalNivel() {
    tone(BUZZER_PIN, NOTA_E4);
    delay(150);
    tone(BUZZER_PIN, NOTA_G4);
    delay(150);
    tone(BUZZER_PIN, NOTA_E5);
    delay(150);
    tone(BUZZER_PIN, NOTA_C5);
    delay(150);
    tone(BUZZER_PIN, NOTA_D5);
    delay(150);
    tone(BUZZER_PIN, NOTA_G5);
    delay(150);
    noTone(BUZZER_PIN);
}


//Funcion principal
void loop(){
    //Agrega un color aleatorio al final de la secuencia
    secuenciaJuego[indexJuego] = random(0, 4);
    indexJuego++;
    if (indexJuego >= MAX_LONGITUD) {
        indexJuego = MAX_LONGITUD - 1;
    }

    reproduceSecuencia();
    if (!compruebaSecuencia()) {
        gameOver();
    }

    delay(300);

    if(indexJuego > 0) {
        reproduceSonidoFinalNivel();
        delay(300);
    }
}