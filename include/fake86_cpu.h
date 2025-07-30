#ifndef FAKE86_CPU_H
#define FAKE86_CPU_H

#include <stdint.h>

void portout (uint16_t portnum, uint8_t value);
void portout16 (uint16_t portnum, uint16_t value);
uint8_t portin (uint16_t portnum);
uint16_t portin16 (uint16_t portnum);

void write86(uint32_t addr32, uint8_t value);
void writew86(uint32_t addr32, uint16_t value);
uint8_t read86(uint32_t addr32);
uint16_t readw86(uint32_t addr32);

void flag_szp8 (uint8_t value);
void flag_szp16 (uint16_t value);
void flag_log8 (uint8_t value);
void flag_log16 (uint16_t value);
void flag_adc8 (uint8_t v1, uint8_t v2, uint8_t v3);
void flag_adc16 (uint16_t v1, uint16_t v2, uint16_t v3);
void flag_add8 (uint8_t v1, uint8_t v2);
void flag_add16 (uint16_t v1, uint16_t v2);
void flag_sbb8 (uint8_t v1, uint8_t v2, uint8_t v3);
void flag_sbb16 (uint16_t v1, uint16_t v2, uint16_t v3);
void flag_sub8 (uint8_t v1, uint8_t v2);
void flag_sub16 (uint16_t v1, uint16_t v2);
void op_adc8();
void op_adc16();
void op_add8();
void op_add16();
void op_and8();
void op_and16();
void op_or8();
void op_or16();
void op_xor8();
void op_xor16();
void op_sub8();
void op_sub16();
void op_sbb8();
void op_sbb16();
void getea (uint8_t rmval);
void push (uint16_t pushval);
uint16_t pop();
void reset86(int v20);
uint16_t readrm16 (uint8_t rmval);
uint8_t readrm8 (uint8_t rmval);
void writerm16 (uint8_t rmval, uint16_t value);
void writerm8 (uint8_t rmval, uint8_t value);
uint8_t op_grp2_8 (uint8_t cnt);
uint16_t op_grp2_16 (uint8_t cnt);
void op_div8 (uint16_t valdiv, uint8_t divisor);
void op_idiv8 (uint16_t valdiv, uint8_t divisor);
void op_grp3_8();
void op_div16 (uint32_t valdiv, uint16_t divisor);
void op_idiv16 (uint32_t valdiv, uint16_t divisor);
void op_grp3_16();
void op_grp5();
void exec86 (uint32_t execloops);

#endif /* FAKE86_CPU_H */
