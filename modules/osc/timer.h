
#define TIMER_OCR0A 31L  // 15625/31 = 504 times per second

void timer_init();
void set_counter(uint8_t i, uint16_t val);
uint16_t get_counter(uint8_t i);
void check();
void set_pwm_freq(uint16_t freq1, uint16_t duty1, uint16_t freq2, uint16_t duty2);
