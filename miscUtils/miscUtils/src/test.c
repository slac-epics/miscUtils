#include "endian2cpu.h"
#include <sys/types.h>

uint32_t
io32_be(uint32_t *addr, uint32_t val)
{
	out_be32(addr,val);
	return in_be32(addr);
}

uint32_t
io32_le(uint32_t *addr, uint32_t val)
{
	out_le32(addr,val);
	return in_le32(addr);
}
uint16_t
io16_be(uint16_t *addr, uint16_t val)
{
	out_be16(addr,val);
	return in_be16(addr);
}

uint16_t
io16_le(uint16_t *addr, uint16_t val)
{
	out_le16(addr,val);
	return in_le16(addr);
}

unsigned char
io8(unsigned char *addr, unsigned char val)
{
	out_8(addr,val);
	return in_8(addr);
}
