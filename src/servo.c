#define F_CPU 9600000UL

// servo je rizene pres timer
// lepsi reseni pre PWM
// http://www.societyofrobots.com/member_tutorials/book/export/html/231

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned int count;
volatile unsigned int servoPos;
volatile unsigned int run;

unsigned int servoMin = 530; // 570 = 1ms
unsigned int servoMax = 600; // 600 = 2ms


// PWM Freq = IO Freq / (Prescaller * 256)
// pro 50 Hz  Prescaller = 750

void cpuInit(void) {
    DDRB = 0x00;
    DDRB = (1 << PB2);

    // timer pro blikani
    TCCR0B = 0x00;
    TCNT0 = 0x00;

    TCCR0B |= (1 << CS00);

    OCR0A = 4;
    TCNT0 = 0;
    TIMSK0 |= (1 << OCIE0A);

    servoPos = servoMin;

    count = 0;
    run = 0;

    // enable interrupts
    sei();
}

ISR(TIM0_COMPA_vect) {
    count++;
    // 630 = 20 ms
    if (count == 630) {
        count = 0;

        if (run && servoPos < servoMax) {
            servoPos++;
        }

        if (!run && servoPos > servoMin) {
            servoPos--;
        }

        // TIMER1 stop
        if (servoPos == servoMin || servoPos == servoMax) {
            //TCCR1 &= ~(1 << CS10);
        }
    }

    if (count > servoPos) {
        PORTB |= 1 << PB2;
    } else {
        PORTB &= ~(1 << PB2);
    }
}

int main(void) {
    cpuInit();

    while(1) {
        run =! run;
        _delay_ms(2000);
    }
}
