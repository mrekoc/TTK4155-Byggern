#ifndef MCP2515_guard
#define MCP2515_guard

#define READ 0b00000011
#define WRITE 0b00000011
#define LOAD_TX_BUFFER 01000001
#define REQUEST_TO_SEND_BUFFER_1 10000001
#define TRANSMIT_BUFFER_1 0x36



char MCP2515_read(char);
void MCP2515_write(char, char);
char MCP2515_init();
void MCP2515_request_to_send(int);
void MCP2515_bit_modify(char, char, char);
void MCP2515_reset();
char MCP2515_read_status();


#endif


