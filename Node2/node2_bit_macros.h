/*
 * bit_macros.h
 *
 */ 


#ifndef BIT_MACROS_33
#define BIT_MACROS_33

#define set_bit( reg1, reg2, bit ) (reg1 -> reg2 |= ( 1 << bit ) )
#define set_bit_hard(reg1, reg2, bitmacros) (reg1 -> reg2 = bitmacros)
#define set_multiple_bits( reg1, reg2, startbit, mask) ((reg1 -> reg2) |= (mask << startbit))
#define clear_bit( reg1, reg2, bit ) ((reg1 -> reg2) &= ~(1 << bit))
#define test_bit( reg, bit ) (reg & (1 << bit))
#define loop_until_bit_is_set( reg, bit ) while( !test_bit( reg, bit ) )
#define loop_until_bit_is_clear( reg, bit ) while( test_bit( reg, bit ) )

#define toggle_bit(reg, bit) (reg ^= (1 << bit))

#endif 
