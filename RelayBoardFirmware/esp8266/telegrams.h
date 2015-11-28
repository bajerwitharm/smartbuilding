#ifndef TELEGRAMS_H_
#define TELEGRAMS_H_

extern char* outputs[20];
extern char* states[20];
extern char* inputs[20];

void decode_telegram(unsigned char* payload);
void encode_telegram(unsigned char* buffer);

#endif

