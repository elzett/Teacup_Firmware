#include	"dda_queue.h"

#include	"timer.h"
#include	"serial.h"
#include	"sermsg.h"

uint8_t	mb_head = 0;
uint8_t	mb_tail = 0;
DDA movebuffer[MOVEBUFFER_SIZE];


uint8_t queue_full() {
	if (mb_tail == 0)
		return mb_head == (MOVEBUFFER_SIZE - 1);
	else
		return mb_head == (mb_tail - 1);
}

uint8_t queue_empty() {
	return ((mb_tail == mb_head) && (movebuffer[mb_tail].live == 0))?255:0;
}

void enqueue(TARGET *t) {
	while (queue_full())
		delay(WAITING_DELAY);

	uint8_t h = mb_head;
	h++;
	if (h == MOVEBUFFER_SIZE)
		h = 0;

	dda_create(&movebuffer[h], t);

	// if queue only has one space left, stop transmition
	if (((h + 2) & (MOVEBUFFER_SIZE - 1)) == mb_tail)
		xoff();

	mb_head = h;

	// fire up in case we're not running yet
	enableTimerInterrupt();
}

void next_move() {
	if (queue_empty()) {
		// queue is empty
// 		disable_steppers();
// 		setTimer(DEFAULT_TICK);
		disableTimerInterrupt();
	}
	else {
		uint8_t t = mb_tail;
		t++;
		if (t == MOVEBUFFER_SIZE)
			t = 0;
		dda_start(&movebuffer[t]);
		mb_tail = t;
	}
	// restart transmission
	xon();
}

void print_queue() {
	serial_writechar('Q');
	serwrite_uint8(mb_tail);
	serial_writechar('/');
	serwrite_uint8(mb_head);
	if (queue_full())
		serial_writechar('F');
	if (queue_empty())
		serial_writechar('E');
	serial_writechar('\n');
}