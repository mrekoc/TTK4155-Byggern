extern int PAGE;

#ifndef GLOBAL_33
#define GLOBAL_33

//Defining macros
#define F_CPU 4915200    // Frequency at crystal, main frequency atmega162
#define FOSC 4915200     // Frequency at crystal, main frequency atmega162
#define BAUD 9600        // BAUD rate for UART communication
#define MYUBBR FOSC/16/BAUD-1 // Init parameter for UART initialization

#endif