typedef unsigned int volatile * vp;
void main() {
	/* PIO0_2 is used by SWD, so disable it */
	*(vp) 0x4000C1C0 = 0xFFFFFFBFUL;	/* PINENABLE0 register */
	/* Set GPIO Direction */
	*(vp) 0xA0002000 |= 1 << 2;	/* DIR0, set PIO0_2 to output */
	for(;;) {
		/* Toggle the LED */
		*(vp) 0xA0002300 |= 1 << 2;	/* NOT0 register */
		volatile long wait = 240000;
		while (wait > 0) --wait;		/* WAIT */
	}
}
