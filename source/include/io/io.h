#ifndef IO_H
#define IO_H

/* Input a byte from a port */
inline unsigned char inportb(unsigned int port)
{
   unsigned char ret = 0;
   asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));

   return ret;
}

/* Output a byte to a port */
inline void outportb(unsigned int port,unsigned char value)
{
   asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}

#endif

