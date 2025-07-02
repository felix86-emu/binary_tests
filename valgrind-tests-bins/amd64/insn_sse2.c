#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

typedef union {
  char sb[1];
  unsigned char ub[1];
} reg8_t;

typedef union {
  char sb[2];
  unsigned char ub[2];
  short sw[1];
  unsigned short uw[1];
} reg16_t;

typedef union {
  char sb[4];
  unsigned char ub[4];
  short sw[2];
  unsigned short uw[2];
  int sd[1];
  unsigned int ud[1];
  float ps[1];
} reg32_t;

typedef union {
  char sb[8];
  unsigned char ub[8];
  short sw[4];
  unsigned short uw[4];
  int sd[2];
  unsigned int ud[2];
  long long int sq[1];
  unsigned long long int uq[1];
  float ps[2];
  double pd[1];
} reg64_t __attribute__ ((aligned (8)));

typedef union {
  char sb[16];
  unsigned char ub[16];
  short sw[8];
  unsigned short uw[8];
  int sd[4];
  unsigned int ud[4];
  long long int sq[2];
  unsigned long long int uq[2];
  float ps[4];
  double pd[2];
} reg128_t __attribute__ ((aligned (16)));

static sigjmp_buf catchpoint;

static void handle_sigill(int signum)
{
   siglongjmp(catchpoint, 1);
}

__attribute__((unused))
static int eq_float(float f1, float f2)
{
   /* return f1 == f2 || fabsf(f1 - f2) < fabsf(f1) * 1.5 * powf(2,-12); */
   return f1 == f2 || fabsf(f1 - f2) < fabsf(f1) * 1.5 / 4096.0;
}

__attribute__((unused))
static int eq_double(double d1, double d2)
{
   /* return d1 == d2 || fabs(d1 - d2) < fabs(d1) * 1.5 * pow(2,-12); */
   return d1 == d2 || fabs(d1 - d2) < fabs(d1) * 1.5 / 4096.0;
}

static void addpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 2222.2222, 1111.1111 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "addpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 3456.79) && eq_double(result0.pd[1], 9876.5432) )
      {
         printf("addpd_1 ... ok\n");
      }
      else
      {
         printf("addpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 3456.79);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 9876.5432);
      }
   }
   else
   {
      printf("addpd_1 ... failed\n");
   }

   return;
}

static void addpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 2222.2222, 1111.1111 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "addpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 3456.79) && eq_double(result0.pd[1], 9876.5432) )
      {
         printf("addpd_2 ... ok\n");
      }
      else
      {
         printf("addpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 3456.79);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 9876.5432);
      }
   }
   else
   {
      printf("addpd_2 ... failed\n");
   }

   return;
}

static void addsd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 2222.2222, 1111.1111 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "addsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 3456.79) && eq_double(result0.pd[1], 1111.1111) )
      {
         printf("addsd_1 ... ok\n");
      }
      else
      {
         printf("addsd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 3456.79);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 1111.1111);
      }
   }
   else
   {
      printf("addsd_1 ... failed\n");
   }

   return;
}

static void addsd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 2222.2222, 1111.1111 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "addsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 3456.79) && eq_double(result0.pd[1], 1111.1111) )
      {
         printf("addsd_2 ... ok\n");
      }
      else
      {
         printf("addsd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 3456.79);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 1111.1111);
      }
   }
   else
   {
      printf("addsd_2 ... failed\n");
   }

   return;
}

static void andpd_1(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "andpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0121452188a84420ULL && result0.uq[1] == 0x0121452188a84420ULL )
      {
         printf("andpd_1 ... ok\n");
      }
      else
      {
         printf("andpd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0121452188a84420ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0121452188a84420ULL);
      }
   }
   else
   {
      printf("andpd_1 ... failed\n");
   }

   return;
}

static void andpd_2(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "andpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0121452188a84420ULL && result0.uq[1] == 0x0121452188a84420ULL )
      {
         printf("andpd_2 ... ok\n");
      }
      else
      {
         printf("andpd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0121452188a84420ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0121452188a84420ULL);
      }
   }
   else
   {
      printf("andpd_2 ... failed\n");
   }

   return;
}

static void andnpd_1(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "andnpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfc98301064002000ULL && result0.uq[1] == 0x00020046010389cfULL )
      {
         printf("andnpd_1 ... ok\n");
      }
      else
      {
         printf("andnpd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc98301064002000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x00020046010389cfULL);
      }
   }
   else
   {
      printf("andnpd_1 ... failed\n");
   }

   return;
}

static void andnpd_2(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "andnpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfc98301064002000ULL && result0.uq[1] == 0x00020046010389cfULL )
      {
         printf("andnpd_2 ... ok\n");
      }
      else
      {
         printf("andnpd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc98301064002000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x00020046010389cfULL);
      }
   }
   else
   {
      printf("andnpd_2 ... failed\n");
   }

   return;
}

static void cmpeqpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5678, 1234.5679 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpeqpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmpeqpd_1 ... ok\n");
      }
      else
      {
         printf("cmpeqpd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmpeqpd_1 ... failed\n");
   }

   return;
}

static void cmpeqpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5678, 1234.5679 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpeqpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmpeqpd_2 ... ok\n");
      }
      else
      {
         printf("cmpeqpd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmpeqpd_2 ... failed\n");
   }

   return;
}

static void cmpltpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5677, 1234.5679 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpltpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmpltpd_1 ... ok\n");
      }
      else
      {
         printf("cmpltpd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmpltpd_1 ... failed\n");
   }

   return;
}

static void cmpltpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5677, 1234.5679 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpltpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmpltpd_2 ... ok\n");
      }
      else
      {
         printf("cmpltpd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmpltpd_2 ... failed\n");
   }

   return;
}

static void cmplepd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5678, 1234.5679 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmplepd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmplepd_1 ... ok\n");
      }
      else
      {
         printf("cmplepd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmplepd_1 ... failed\n");
   }

   return;
}

static void cmplepd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5678, 1234.5679 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmplepd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmplepd_2 ... ok\n");
      }
      else
      {
         printf("cmplepd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmplepd_2 ... failed\n");
   }

   return;
}

static void cmpunordpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5678, 1234.5679 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpunordpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmpunordpd_1 ... ok\n");
      }
      else
      {
         printf("cmpunordpd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmpunordpd_1 ... failed\n");
   }

   return;
}

static void cmpunordpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5678, 1234.5679 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpunordpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmpunordpd_2 ... ok\n");
      }
      else
      {
         printf("cmpunordpd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmpunordpd_2 ... failed\n");
   }

   return;
}

static void cmpneqpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5679, 1234.5678 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpneqpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmpneqpd_1 ... ok\n");
      }
      else
      {
         printf("cmpneqpd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmpneqpd_1 ... failed\n");
   }

   return;
}

static void cmpneqpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5679, 1234.5678 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpneqpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmpneqpd_2 ... ok\n");
      }
      else
      {
         printf("cmpneqpd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmpneqpd_2 ... failed\n");
   }

   return;
}

static void cmpnltpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5679, 1234.5677 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpnltpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmpnltpd_1 ... ok\n");
      }
      else
      {
         printf("cmpnltpd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmpnltpd_1 ... failed\n");
   }

   return;
}

static void cmpnltpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5679, 1234.5677 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpnltpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmpnltpd_2 ... ok\n");
      }
      else
      {
         printf("cmpnltpd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmpnltpd_2 ... failed\n");
   }

   return;
}

static void cmpnlepd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5679, 1234.5678 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpnlepd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmpnlepd_1 ... ok\n");
      }
      else
      {
         printf("cmpnlepd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmpnlepd_1 ... failed\n");
   }

   return;
}

static void cmpnlepd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5679, 1234.5678 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpnlepd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("cmpnlepd_2 ... ok\n");
      }
      else
      {
         printf("cmpnlepd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("cmpnlepd_2 ... failed\n");
   }

   return;
}

static void cmpordpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5679, 1234.5678 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpordpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0xffffffffffffffffULL )
      {
         printf("cmpordpd_1 ... ok\n");
      }
      else
      {
         printf("cmpordpd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xffffffffffffffffULL);
      }
   }
   else
   {
      printf("cmpordpd_1 ... failed\n");
   }

   return;
}

static void cmpordpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5679, 1234.5678 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpordpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0xffffffffffffffffULL )
      {
         printf("cmpordpd_2 ... ok\n");
      }
      else
      {
         printf("cmpordpd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xffffffffffffffffULL);
      }
   }
   else
   {
      printf("cmpordpd_2 ... failed\n");
   }

   return;
}

static void cmpeqsd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5678, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpeqsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0ULL )
      {
         printf("cmpeqsd_1 ... ok\n");
      }
      else
      {
         printf("cmpeqsd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpeqsd_1 ... failed\n");
   }

   return;
}

static void cmpeqsd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5679, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpeqsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0ULL )
      {
         printf("cmpeqsd_2 ... ok\n");
      }
      else
      {
         printf("cmpeqsd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpeqsd_2 ... failed\n");
   }

   return;
}

static void cmpltsd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5677, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpltsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0ULL )
      {
         printf("cmpltsd_1 ... ok\n");
      }
      else
      {
         printf("cmpltsd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpltsd_1 ... failed\n");
   }

   return;
}

static void cmpltsd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5679, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpltsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0ULL )
      {
         printf("cmpltsd_2 ... ok\n");
      }
      else
      {
         printf("cmpltsd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpltsd_2 ... failed\n");
   }

   return;
}

static void cmplesd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5678, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmplesd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0ULL )
      {
         printf("cmplesd_1 ... ok\n");
      }
      else
      {
         printf("cmplesd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmplesd_1 ... failed\n");
   }

   return;
}

static void cmplesd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5679, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmplesd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0ULL )
      {
         printf("cmplesd_2 ... ok\n");
      }
      else
      {
         printf("cmplesd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmplesd_2 ... failed\n");
   }

   return;
}

static void cmpunordsd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5678, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpunordsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0ULL )
      {
         printf("cmpunordsd_1 ... ok\n");
      }
      else
      {
         printf("cmpunordsd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpunordsd_1 ... failed\n");
   }

   return;
}

static void cmpunordsd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5679, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpunordsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0ULL )
      {
         printf("cmpunordsd_2 ... ok\n");
      }
      else
      {
         printf("cmpunordsd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpunordsd_2 ... failed\n");
   }

   return;
}

static void cmpneqsd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5679, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpneqsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0ULL )
      {
         printf("cmpneqsd_1 ... ok\n");
      }
      else
      {
         printf("cmpneqsd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpneqsd_1 ... failed\n");
   }

   return;
}

static void cmpneqsd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5678, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpneqsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0ULL )
      {
         printf("cmpneqsd_2 ... ok\n");
      }
      else
      {
         printf("cmpneqsd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpneqsd_2 ... failed\n");
   }

   return;
}

static void cmpnltsd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5679, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpnltsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0ULL )
      {
         printf("cmpnltsd_1 ... ok\n");
      }
      else
      {
         printf("cmpnltsd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpnltsd_1 ... failed\n");
   }

   return;
}

static void cmpnltsd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5677, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpnltsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0ULL )
      {
         printf("cmpnltsd_2 ... ok\n");
      }
      else
      {
         printf("cmpnltsd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpnltsd_2 ... failed\n");
   }

   return;
}

static void cmpnlesd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5679, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpnlesd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0ULL )
      {
         printf("cmpnlesd_1 ... ok\n");
      }
      else
      {
         printf("cmpnlesd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpnlesd_1 ... failed\n");
   }

   return;
}

static void cmpnlesd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5678, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpnlesd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0ULL )
      {
         printf("cmpnlesd_2 ... ok\n");
      }
      else
      {
         printf("cmpnlesd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpnlesd_2 ... failed\n");
   }

   return;
}

static void cmpordsd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5679, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cmpordsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0ULL )
      {
         printf("cmpordsd_1 ... ok\n");
      }
      else
      {
         printf("cmpordsd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpordsd_1 ... failed\n");
   }

   return;
}

static void cmpordsd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5678, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cmpordsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xffffffffffffffffULL && result0.uq[1] == 0ULL )
      {
         printf("cmpordsd_2 ... ok\n");
      }
      else
      {
         printf("cmpordsd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xffffffffffffffffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("cmpordsd_2 ... failed\n");
   }

   return;
}

static void comisd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5679, 0.0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pushfq\n"
         "andq $0xfffffffffffff72a, (%%rsp)\n"
         "orq $0x00000000000008d5, (%%rsp)\n"
         "popfq\n"
         "comisd %%xmm12, %%xmm13\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "xmm13"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000000UL )
      {
         printf("comisd_1 ... ok\n");
      }
      else
      {
         printf("comisd_1 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("comisd_1 ... failed\n");
   }

   return;
}

static void comisd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5677, 0.0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pushfq\n"
         "andq $0xfffffffffffff72a, (%%rsp)\n"
         "orq $0x00000000000008d4, (%%rsp)\n"
         "popfq\n"
         "comisd %%xmm12, %%xmm13\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "xmm13"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000001UL )
      {
         printf("comisd_2 ... ok\n");
      }
      else
      {
         printf("comisd_2 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("comisd_2 ... failed\n");
   }

   return;
}

static void comisd_3(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5678, 0.0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pushfq\n"
         "andq $0xfffffffffffff72a, (%%rsp)\n"
         "orq $0x0000000000000895, (%%rsp)\n"
         "popfq\n"
         "comisd %%xmm12, %%xmm13\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "xmm13"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000040UL )
      {
         printf("comisd_3 ... ok\n");
      }
      else
      {
         printf("comisd_3 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("comisd_3 ... failed\n");
   }

   return;
}

static void comisd_4(void)
{
   reg64_t arg0 = { .pd = { 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5679, 0.0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pushfq\n"
         "andq $0xfffffffffffff72a, (%%rsp)\n"
         "orq $0x00000000000008d5, (%%rsp)\n"
         "popfq\n"
         "comisd %1, %%xmm12\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "mm6", "xmm12"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000000UL )
      {
         printf("comisd_4 ... ok\n");
      }
      else
      {
         printf("comisd_4 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("comisd_4 ... failed\n");
   }

   return;
}

static void comisd_5(void)
{
   reg64_t arg0 = { .pd = { 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5677, 0.0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pushfq\n"
         "andq $0xfffffffffffff72a, (%%rsp)\n"
         "orq $0x00000000000008d4, (%%rsp)\n"
         "popfq\n"
         "comisd %1, %%xmm12\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "mm6", "xmm12"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000001UL )
      {
         printf("comisd_5 ... ok\n");
      }
      else
      {
         printf("comisd_5 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("comisd_5 ... failed\n");
   }

   return;
}

static void comisd_6(void)
{
   reg64_t arg0 = { .pd = { 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5678, 0.0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pushfq\n"
         "andq $0xfffffffffffff72a, (%%rsp)\n"
         "orq $0x0000000000000895, (%%rsp)\n"
         "popfq\n"
         "comisd %1, %%xmm12\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "mm6", "xmm12"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000040UL )
      {
         printf("comisd_6 ... ok\n");
      }
      else
      {
         printf("comisd_6 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("comisd_6 ... failed\n");
   }

   return;
}

static void cvtdq2pd_1(void)
{
   reg128_t arg0 = { .sd = { 1234, 5678, 0, 0 } };
   reg128_t arg1 = { .pd = { 0.0, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cvtdq2pd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.0) && eq_double(result0.pd[1], 5678.0) )
      {
         printf("cvtdq2pd_1 ... ok\n");
      }
      else
      {
         printf("cvtdq2pd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.0);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 5678.0);
      }
   }
   else
   {
      printf("cvtdq2pd_1 ... failed\n");
   }

   return;
}

static void cvtdq2pd_2(void)
{
   reg128_t arg0 = { .sd = { 1234, 5678, 0, 0 } };
   reg128_t arg1 = { .pd = { 0.0, 0.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cvtdq2pd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.0) && eq_double(result0.pd[1], 5678.0) )
      {
         printf("cvtdq2pd_2 ... ok\n");
      }
      else
      {
         printf("cvtdq2pd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.0);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 5678.0);
      }
   }
   else
   {
      printf("cvtdq2pd_2 ... failed\n");
   }

   return;
}

static void cvtdq2ps_1(void)
{
   reg128_t arg0 = { .sd = { 1234, 5678, -1234, -5678 } };
   reg128_t arg1 = { .ps = { 0.0F, 0.0F, 0.0F, 0.0F } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cvtdq2ps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 1234.0F) && eq_float(result0.ps[1], 5678.0F) && eq_float(result0.ps[2], -1234.0F) && eq_float(result0.ps[3], -5678.0F) )
      {
         printf("cvtdq2ps_1 ... ok\n");
      }
      else
      {
         printf("cvtdq2ps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.0F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 5678.0F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], -1234.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], -5678.0F);
      }
   }
   else
   {
      printf("cvtdq2ps_1 ... failed\n");
   }

   return;
}

static void cvtdq2ps_2(void)
{
   reg128_t arg0 = { .sd = { 1234, 5678, -1234, -5678 } };
   reg128_t arg1 = { .ps = { 0.0F, 0.0F, 0.0F, 0.0F } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cvtdq2ps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 1234.0F) && eq_float(result0.ps[1], 5678.0F) && eq_float(result0.ps[2], -1234.0F) && eq_float(result0.ps[3], -5678.0F) )
      {
         printf("cvtdq2ps_2 ... ok\n");
      }
      else
      {
         printf("cvtdq2ps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.0F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 5678.0F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], -1234.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], -5678.0F);
      }
   }
   else
   {
      printf("cvtdq2ps_2 ... failed\n");
   }

   return;
}

static void cvtpd2dq_1(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg128_t arg1 = { .sd = { 1, 2, 3, 4 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cvtpd2dq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 57 && result0.sd[2] == 0 && result0.sd[3] == 0 )
      {
         printf("cvtpd2dq_1 ... ok\n");
      }
      else
      {
         printf("cvtpd2dq_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 57);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 0);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 0);
      }
   }
   else
   {
      printf("cvtpd2dq_1 ... failed\n");
   }

   return;
}

static void cvtpd2dq_2(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg128_t arg1 = { .sd = { 1, 2, 3, 4 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cvtpd2dq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 57 && result0.sd[2] == 0 && result0.sd[3] == 0 )
      {
         printf("cvtpd2dq_2 ... ok\n");
      }
      else
      {
         printf("cvtpd2dq_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 57);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 0);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 0);
      }
   }
   else
   {
      printf("cvtpd2dq_2 ... failed\n");
   }

   return;
}

static void cvtpd2pi_1(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg64_t arg1 = { .sd = { 1, 2 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movq %3, %%mm6\n"
         "cvtpd2pi %%xmm12, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 57 )
      {
         printf("cvtpd2pi_1 ... ok\n");
      }
      else
      {
         printf("cvtpd2pi_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 57);
      }
   }
   else
   {
      printf("cvtpd2pi_1 ... failed\n");
   }

   return;
}

static void cvtpd2pi_2(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg64_t arg1 = { .sd = { 1, 2 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movq %2, %%mm6\n"
         "cvtpd2pi %1, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 57 )
      {
         printf("cvtpd2pi_2 ... ok\n");
      }
      else
      {
         printf("cvtpd2pi_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 57);
      }
   }
   else
   {
      printf("cvtpd2pi_2 ... failed\n");
   }

   return;
}

static void cvtpd2ps_1(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg128_t arg1 = { .ps = { 1.1F, 2.2F, 3.3F, 4.4F } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cvtpd2ps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 56.78F) && eq_float(result0.ps[2], 0.0F) && eq_float(result0.ps[3], 0.0F) )
      {
         printf("cvtpd2ps_1 ... ok\n");
      }
      else
      {
         printf("cvtpd2ps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 56.78F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 0.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 0.0F);
      }
   }
   else
   {
      printf("cvtpd2ps_1 ... failed\n");
   }

   return;
}

static void cvtpd2ps_2(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg128_t arg1 = { .ps = { 1.1F, 2.2F, 3.3F, 4.4F } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cvtpd2ps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 56.78F) && eq_float(result0.ps[2], 0.0F) && eq_float(result0.ps[3], 0.0F) )
      {
         printf("cvtpd2ps_2 ... ok\n");
      }
      else
      {
         printf("cvtpd2ps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 56.78F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 0.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 0.0F);
      }
   }
   else
   {
      printf("cvtpd2ps_2 ... failed\n");
   }

   return;
}

static void cvtpi2pd_1(void)
{
   reg64_t arg0 = { .sd = { 1234, 5678 } };
   reg128_t arg1 = { .pd = { 1.1, 2.2 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movq %2, %%mm6\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "cvtpi2pd %%mm6, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "mm6"
      );

      if (eq_double(result0.pd[0], 1234.0) && eq_double(result0.pd[1], 5678.0) )
      {
         printf("cvtpi2pd_1 ... ok\n");
      }
      else
      {
         printf("cvtpi2pd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.0);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 5678.0);
      }
   }
   else
   {
      printf("cvtpi2pd_1 ... failed\n");
   }

   return;
}

static void cvtpi2pd_2(void)
{
   reg64_t arg0 = { .sd = { 1234, 5678 } };
   reg128_t arg1 = { .pd = { 1.1, 2.2 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "cvtpi2pd %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "mm6"
      );

      if (eq_double(result0.pd[0], 1234.0) && eq_double(result0.pd[1], 5678.0) )
      {
         printf("cvtpi2pd_2 ... ok\n");
      }
      else
      {
         printf("cvtpi2pd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.0);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 5678.0);
      }
   }
   else
   {
      printf("cvtpi2pd_2 ... failed\n");
   }

   return;
}

static void cvtps2dq_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .sd = { 1, 2, 3, 4 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cvtps2dq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 57 && result0.sd[2] == 43 && result0.sd[3] == 88 )
      {
         printf("cvtps2dq_1 ... ok\n");
      }
      else
      {
         printf("cvtps2dq_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 57);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 43);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 88);
      }
   }
   else
   {
      printf("cvtps2dq_1 ... failed\n");
   }

   return;
}

static void cvtps2dq_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .sd = { 1, 2, 3, 4 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cvtps2dq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 57 && result0.sd[2] == 43 && result0.sd[3] == 88 )
      {
         printf("cvtps2dq_2 ... ok\n");
      }
      else
      {
         printf("cvtps2dq_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 57);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 43);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 88);
      }
   }
   else
   {
      printf("cvtps2dq_2 ... failed\n");
   }

   return;
}

static void cvtps2pd_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 1.1F, 2.2F } };
   reg128_t arg1 = { .pd = { 3.3, 4.4 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cvtps2pd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 12.34) && eq_double(result0.pd[1], 56.78) )
      {
         printf("cvtps2pd_1 ... ok\n");
      }
      else
      {
         printf("cvtps2pd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12.34);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 56.78);
      }
   }
   else
   {
      printf("cvtps2pd_1 ... failed\n");
   }

   return;
}

static void cvtps2pd_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 1.1F, 2.2F } };
   reg128_t arg1 = { .pd = { 3.3, 4.4 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cvtps2pd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 12.34) && eq_double(result0.pd[1], 56.78) )
      {
         printf("cvtps2pd_2 ... ok\n");
      }
      else
      {
         printf("cvtps2pd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12.34);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 56.78);
      }
   }
   else
   {
      printf("cvtps2pd_2 ... failed\n");
   }

   return;
}

static void cvtsd2si_1(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg32_t arg1 = { .sd = { 99 } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "cvtsd2si %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.sd[0] == 12 )
      {
         printf("cvtsd2si_1 ... ok\n");
      }
      else
      {
         printf("cvtsd2si_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
      }
   }
   else
   {
      printf("cvtsd2si_1 ... failed\n");
   }

   return;
}

static void cvtsd2si_2(void)
{
   reg128_t arg0 = { .pd = { 56.78, 12.34 } };
   reg32_t arg1 = { .sd = { 99 } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %2, %%r9d\n"
         "cvtsd2si %1, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.sd[0] == 57 )
      {
         printf("cvtsd2si_2 ... ok\n");
      }
      else
      {
         printf("cvtsd2si_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 57);
      }
   }
   else
   {
      printf("cvtsd2si_2 ... failed\n");
   }

   return;
}

static void cvtsd2ss_1(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg128_t arg1 = { .ps = { 1.11F, 2.22F, 3.33F, 4.44F } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cvtsd2ss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 2.22F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 4.44F) )
      {
         printf("cvtsd2ss_1 ... ok\n");
      }
      else
      {
         printf("cvtsd2ss_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 2.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.44F);
      }
   }
   else
   {
      printf("cvtsd2ss_1 ... failed\n");
   }

   return;
}

static void cvtsd2ss_2(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg128_t arg1 = { .ps = { 1.11F, 2.22F, 3.33F, 4.44F } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cvtsd2ss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 2.22F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 4.44F) )
      {
         printf("cvtsd2ss_2 ... ok\n");
      }
      else
      {
         printf("cvtsd2ss_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 2.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.44F);
      }
   }
   else
   {
      printf("cvtsd2ss_2 ... failed\n");
   }

   return;
}

static void cvtsi2sd_1(void)
{
   reg32_t arg0 = { .sd = { 12 } };
   reg128_t arg1 = { .pd = { 1.11, 2.22 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %2, %%r9d\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "cvtsi2sd %%r9d, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "r9"
      );

      if (eq_double(result0.pd[0], 12.0) && eq_double(result0.pd[1], 2.22) )
      {
         printf("cvtsi2sd_1 ... ok\n");
      }
      else
      {
         printf("cvtsi2sd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12.0);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 2.22);
      }
   }
   else
   {
      printf("cvtsi2sd_1 ... failed\n");
   }

   return;
}

static void cvtsi2sd_2(void)
{
   reg32_t arg0 = { .sd = { 12 } };
   reg128_t arg1 = { .pd = { 1.11, 2.22 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "cvtsi2sd %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (eq_double(result0.pd[0], 12.0) && eq_double(result0.pd[1], 2.22) )
      {
         printf("cvtsi2sd_2 ... ok\n");
      }
      else
      {
         printf("cvtsi2sd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12.0);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 2.22);
      }
   }
   else
   {
      printf("cvtsi2sd_2 ... failed\n");
   }

   return;
}

static void cvtss2sd_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 3.33F, 4.44F, 5.55F } };
   reg128_t arg1 = { .pd = { 1.11, 2.22 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cvtss2sd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 12.34) && eq_double(result0.pd[1], 2.22) )
      {
         printf("cvtss2sd_1 ... ok\n");
      }
      else
      {
         printf("cvtss2sd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12.34);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 2.22);
      }
   }
   else
   {
      printf("cvtss2sd_1 ... failed\n");
   }

   return;
}

static void cvtss2sd_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 3.33F, 4.44F, 5.55F } };
   reg128_t arg1 = { .pd = { 1.11, 2.22 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cvtss2sd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 12.34) && eq_double(result0.pd[1], 2.22) )
      {
         printf("cvtss2sd_2 ... ok\n");
      }
      else
      {
         printf("cvtss2sd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12.34);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 2.22);
      }
   }
   else
   {
      printf("cvtss2sd_2 ... failed\n");
   }

   return;
}

static void cvttpd2pi_1(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg64_t arg1 = { .sd = { 1, 2 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movq %3, %%mm6\n"
         "cvttpd2pi %%xmm12, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 56 )
      {
         printf("cvttpd2pi_1 ... ok\n");
      }
      else
      {
         printf("cvttpd2pi_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 56);
      }
   }
   else
   {
      printf("cvttpd2pi_1 ... failed\n");
   }

   return;
}

static void cvttpd2pi_2(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg64_t arg1 = { .sd = { 1, 2 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movq %2, %%mm6\n"
         "cvttpd2pi %1, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 56 )
      {
         printf("cvttpd2pi_2 ... ok\n");
      }
      else
      {
         printf("cvttpd2pi_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 56);
      }
   }
   else
   {
      printf("cvttpd2pi_2 ... failed\n");
   }

   return;
}

static void cvttpd2dq_1(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg128_t arg1 = { .sd = { 1, 2, 3, 4 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cvttpd2dq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 56 && result0.sd[2] == 0 && result0.sd[3] == 0 )
      {
         printf("cvttpd2dq_1 ... ok\n");
      }
      else
      {
         printf("cvttpd2dq_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 56);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 0);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 0);
      }
   }
   else
   {
      printf("cvttpd2dq_1 ... failed\n");
   }

   return;
}

static void cvttpd2dq_2(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg128_t arg1 = { .sd = { 1, 2, 3, 4 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cvttpd2dq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 56 && result0.sd[2] == 0 && result0.sd[3] == 0 )
      {
         printf("cvttpd2dq_2 ... ok\n");
      }
      else
      {
         printf("cvttpd2dq_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 56);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 0);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 0);
      }
   }
   else
   {
      printf("cvttpd2dq_2 ... failed\n");
   }

   return;
}

static void cvttps2dq_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .sd = { 1, 2, 3, 4 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "cvttps2dq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 56 && result0.sd[2] == 43 && result0.sd[3] == 87 )
      {
         printf("cvttps2dq_1 ... ok\n");
      }
      else
      {
         printf("cvttps2dq_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 56);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 43);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 87);
      }
   }
   else
   {
      printf("cvttps2dq_1 ... failed\n");
   }

   return;
}

static void cvttps2dq_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .sd = { 1, 2, 3, 4 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "cvttps2dq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 56 && result0.sd[2] == 43 && result0.sd[3] == 87 )
      {
         printf("cvttps2dq_2 ... ok\n");
      }
      else
      {
         printf("cvttps2dq_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 56);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 43);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 87);
      }
   }
   else
   {
      printf("cvttps2dq_2 ... failed\n");
   }

   return;
}

static void cvttsd2si_1(void)
{
   reg128_t arg0 = { .pd = { 12.34, 56.78 } };
   reg32_t arg1 = { .sd = { 99 } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "cvttsd2si %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.sd[0] == 12 )
      {
         printf("cvttsd2si_1 ... ok\n");
      }
      else
      {
         printf("cvttsd2si_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
      }
   }
   else
   {
      printf("cvttsd2si_1 ... failed\n");
   }

   return;
}

static void cvttsd2si_2(void)
{
   reg128_t arg0 = { .pd = { 56.78, 12.34 } };
   reg32_t arg1 = { .sd = { 99 } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %2, %%r9d\n"
         "cvttsd2si %1, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.sd[0] == 56 )
      {
         printf("cvttsd2si_2 ... ok\n");
      }
      else
      {
         printf("cvttsd2si_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 56);
      }
   }
   else
   {
      printf("cvttsd2si_2 ... failed\n");
   }

   return;
}

static void divpd_1(void)
{
   reg128_t arg0 = { .pd = { 2.0, 3.0 } };
   reg128_t arg1 = { .pd = { 24.68, 3.69 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "divpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 12.34) && eq_double(result0.pd[1], 1.23) )
      {
         printf("divpd_1 ... ok\n");
      }
      else
      {
         printf("divpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12.34);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 1.23);
      }
   }
   else
   {
      printf("divpd_1 ... failed\n");
   }

   return;
}

static void divpd_2(void)
{
   reg128_t arg0 = { .pd = { 2.0, 3.0 } };
   reg128_t arg1 = { .pd = { 24.68, 3.69 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "divpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 12.34) && eq_double(result0.pd[1], 1.23) )
      {
         printf("divpd_2 ... ok\n");
      }
      else
      {
         printf("divpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12.34);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 1.23);
      }
   }
   else
   {
      printf("divpd_2 ... failed\n");
   }

   return;
}

static void divsd_1(void)
{
   reg128_t arg0 = { .pd = { 2.0, 3.0 } };
   reg128_t arg1 = { .pd = { 24.68, 3.69 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "divsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 12.34) && eq_double(result0.pd[1], 3.69) )
      {
         printf("divsd_1 ... ok\n");
      }
      else
      {
         printf("divsd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12.34);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 3.69);
      }
   }
   else
   {
      printf("divsd_1 ... failed\n");
   }

   return;
}

static void divsd_2(void)
{
   reg128_t arg0 = { .pd = { 2.0, 3.0 } };
   reg128_t arg1 = { .pd = { 24.68, 3.69 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "divsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 12.34) && eq_double(result0.pd[1], 3.69) )
      {
         printf("divsd_2 ... ok\n");
      }
      else
      {
         printf("divsd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12.34);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 3.69);
      }
   }
   else
   {
      printf("divsd_2 ... failed\n");
   }

   return;
}

static void lfence_1(void)
{
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "lfence\n"
         "cld\n"
         :
         : "m" (state[0])

      );

      printf("lfence_1 ... ok\n");
   }
   else
   {
      printf("lfence_1 ... failed\n");
   }

   return;
}

static void maxpd_1(void)
{
   reg128_t arg0 = { .pd = { 22.222, 44.444 } };
   reg128_t arg1 = { .pd = { 55.555, 33.333 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "maxpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 55.555) && eq_double(result0.pd[1], 44.444) )
      {
         printf("maxpd_1 ... ok\n");
      }
      else
      {
         printf("maxpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 55.555);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 44.444);
      }
   }
   else
   {
      printf("maxpd_1 ... failed\n");
   }

   return;
}

static void maxpd_2(void)
{
   reg128_t arg0 = { .pd = { 22.222, 44.444 } };
   reg128_t arg1 = { .pd = { 55.555, 33.333 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "maxpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 55.555) && eq_double(result0.pd[1], 44.444) )
      {
         printf("maxpd_2 ... ok\n");
      }
      else
      {
         printf("maxpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 55.555);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 44.444);
      }
   }
   else
   {
      printf("maxpd_2 ... failed\n");
   }

   return;
}

static void maxsd_1(void)
{
   reg128_t arg0 = { .pd = { 22.222, 44.444 } };
   reg128_t arg1 = { .pd = { 55.555, 33.333 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "maxsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 55.555) && eq_double(result0.pd[1], 33.333) )
      {
         printf("maxsd_1 ... ok\n");
      }
      else
      {
         printf("maxsd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 55.555);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 33.333);
      }
   }
   else
   {
      printf("maxsd_1 ... failed\n");
   }

   return;
}

static void maxsd_2(void)
{
   reg128_t arg0 = { .pd = { 44.444, 22.222 } };
   reg128_t arg1 = { .pd = { 33.333, 55.555 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "maxsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 44.444) && eq_double(result0.pd[1], 55.555) )
      {
         printf("maxsd_2 ... ok\n");
      }
      else
      {
         printf("maxsd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 44.444);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 55.555);
      }
   }
   else
   {
      printf("maxsd_2 ... failed\n");
   }

   return;
}

static void mfence_1(void)
{
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "mfence\n"
         "cld\n"
         :
         : "m" (state[0])

      );

      printf("mfence_1 ... ok\n");
   }
   else
   {
      printf("mfence_1 ... failed\n");
   }

   return;
}

static void minpd_1(void)
{
   reg128_t arg0 = { .pd = { 22.222, 44.444 } };
   reg128_t arg1 = { .pd = { 55.555, 33.333 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "minpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 22.222) && eq_double(result0.pd[1], 33.333) )
      {
         printf("minpd_1 ... ok\n");
      }
      else
      {
         printf("minpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 22.222);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 33.333);
      }
   }
   else
   {
      printf("minpd_1 ... failed\n");
   }

   return;
}

static void minpd_2(void)
{
   reg128_t arg0 = { .pd = { 22.222, 44.444 } };
   reg128_t arg1 = { .pd = { 55.555, 33.333 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "minpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 22.222) && eq_double(result0.pd[1], 33.333) )
      {
         printf("minpd_2 ... ok\n");
      }
      else
      {
         printf("minpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 22.222);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 33.333);
      }
   }
   else
   {
      printf("minpd_2 ... failed\n");
   }

   return;
}

static void minsd_1(void)
{
   reg128_t arg0 = { .pd = { 22.222, 44.444 } };
   reg128_t arg1 = { .pd = { 55.555, 33.333 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "minsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 22.222) && eq_double(result0.pd[1], 33.333) )
      {
         printf("minsd_1 ... ok\n");
      }
      else
      {
         printf("minsd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 22.222);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 33.333);
      }
   }
   else
   {
      printf("minsd_1 ... failed\n");
   }

   return;
}

static void minsd_2(void)
{
   reg128_t arg0 = { .pd = { 44.444, 22.222 } };
   reg128_t arg1 = { .pd = { 33.333, 55.555 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "minsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 33.333) && eq_double(result0.pd[1], 55.555) )
      {
         printf("minsd_2 ... ok\n");
      }
      else
      {
         printf("minsd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 33.333);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 55.555);
      }
   }
   else
   {
      printf("minsd_2 ... failed\n");
   }

   return;
}

static void movapd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 1111.1111, 2222.2222 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "movapd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.5678) && eq_double(result0.pd[1], 8765.4321) )
      {
         printf("movapd_1 ... ok\n");
      }
      else
      {
         printf("movapd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 8765.4321);
      }
   }
   else
   {
      printf("movapd_1 ... failed\n");
   }

   return;
}

static void movapd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 1111.1111, 2222.2222 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "movapd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.5678) && eq_double(result0.pd[1], 8765.4321) )
      {
         printf("movapd_2 ... ok\n");
      }
      else
      {
         printf("movapd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 8765.4321);
      }
   }
   else
   {
      printf("movapd_2 ... failed\n");
   }

   return;
}

static void movd_1(void)
{
   reg32_t arg0 = { .sd = { 1234 } };
   reg128_t arg1 = { .sd = { 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %2, %%r9d\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "movd %%r9d, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "r9"
      );

      if (result0.sd[0] == 1234 && result0.sd[1] == 0 && result0.sd[2] == 0 && result0.sd[3] == 0 )
      {
         printf("movd_1 ... ok\n");
      }
      else
      {
         printf("movd_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 0);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 0);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 0);
      }
   }
   else
   {
      printf("movd_1 ... failed\n");
   }

   return;
}

static void movd_2(void)
{
   reg32_t arg0 = { .sd = { 1234 } };
   reg128_t arg1 = { .sd = { 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "movd %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.sd[0] == 1234 && result0.sd[1] == 0 && result0.sd[2] == 0 && result0.sd[3] == 0 )
      {
         printf("movd_2 ... ok\n");
      }
      else
      {
         printf("movd_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 0);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 0);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 0);
      }
   }
   else
   {
      printf("movd_2 ... failed\n");
   }

   return;
}

static void movd_3(void)
{
   reg128_t arg0 = { .sd = { 1234, 2222, 3333, 4444 } };
   reg32_t arg1 = { .sd = { 1111 } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "movd %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.sd[0] == 1234 )
      {
         printf("movd_3 ... ok\n");
      }
      else
      {
         printf("movd_3 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234);
      }
   }
   else
   {
      printf("movd_3 ... failed\n");
   }

   return;
}

static void movd_4(void)
{
   reg128_t arg0 = { .sd = { 1234, 2222, 3333, 4444 } };
   reg32_t arg1 = { .sd = { 1111 } };
   reg32_t result0 = arg1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movd %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "xmm12"
      );

      if (result0.sd[0] == 1234 )
      {
         printf("movd_4 ... ok\n");
      }
      else
      {
         printf("movd_4 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234);
      }
   }
   else
   {
      printf("movd_4 ... failed\n");
   }

   return;
}

static void movdqa_1(void)
{
   reg128_t arg0 = { .uq = { 0x012345678abcdefULL, 0xfedcba9876543210ULL } };
   reg128_t arg1 = { .uq = { 0x1212121234343434ULL, 0x5656565678787878ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "movdqa %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x012345678abcdefULL && result0.uq[1] == 0xfedcba9876543210ULL )
      {
         printf("movdqa_1 ... ok\n");
      }
      else
      {
         printf("movdqa_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x012345678abcdefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfedcba9876543210ULL);
      }
   }
   else
   {
      printf("movdqa_1 ... failed\n");
   }

   return;
}

static void movdqa_2(void)
{
   reg128_t arg0 = { .uq = { 0x012345678abcdefULL, 0xfedcba9876543210ULL } };
   reg128_t arg1 = { .uq = { 0x1212121234343434ULL, 0x5656565678787878ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "movdqa %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x012345678abcdefULL && result0.uq[1] == 0xfedcba9876543210ULL )
      {
         printf("movdqa_2 ... ok\n");
      }
      else
      {
         printf("movdqa_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x012345678abcdefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfedcba9876543210ULL);
      }
   }
   else
   {
      printf("movdqa_2 ... failed\n");
   }

   return;
}

static void movdqa_3(void)
{
   reg128_t arg0 = { .uq = { 0x012345678abcdefULL, 0xfedcba9876543210ULL } };
   reg128_t arg1 = { .uq = { 0x1212121234343434ULL, 0x5656565678787878ULL } };
   reg128_t result0 = arg1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movdqa %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x012345678abcdefULL && result0.uq[1] == 0xfedcba9876543210ULL )
      {
         printf("movdqa_3 ... ok\n");
      }
      else
      {
         printf("movdqa_3 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x012345678abcdefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfedcba9876543210ULL);
      }
   }
   else
   {
      printf("movdqa_3 ... failed\n");
   }

   return;
}

static void movdqu_1(void)
{
   reg128_t arg0 = { .uq = { 0x012345678abcdefULL, 0xfedcba9876543210ULL } };
   reg128_t arg1 = { .uq = { 0x1212121234343434ULL, 0x5656565678787878ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "movdqu %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x012345678abcdefULL && result0.uq[1] == 0xfedcba9876543210ULL )
      {
         printf("movdqu_1 ... ok\n");
      }
      else
      {
         printf("movdqu_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x012345678abcdefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfedcba9876543210ULL);
      }
   }
   else
   {
      printf("movdqu_1 ... failed\n");
   }

   return;
}

static void movdqu_2(void)
{
   reg128_t arg0 = { .uq = { 0x012345678abcdefULL, 0xfedcba9876543210ULL } };
   reg128_t arg1 = { .uq = { 0x1212121234343434ULL, 0x5656565678787878ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "movdqu %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x012345678abcdefULL && result0.uq[1] == 0xfedcba9876543210ULL )
      {
         printf("movdqu_2 ... ok\n");
      }
      else
      {
         printf("movdqu_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x012345678abcdefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfedcba9876543210ULL);
      }
   }
   else
   {
      printf("movdqu_2 ... failed\n");
   }

   return;
}

static void movdqu_3(void)
{
   reg128_t arg0 = { .uq = { 0x012345678abcdefULL, 0xfedcba9876543210ULL } };
   reg128_t arg1 = { .uq = { 0x1212121234343434ULL, 0x5656565678787878ULL } };
   reg128_t result0 = arg1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movdqu %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x012345678abcdefULL && result0.uq[1] == 0xfedcba9876543210ULL )
      {
         printf("movdqu_3 ... ok\n");
      }
      else
      {
         printf("movdqu_3 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x012345678abcdefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfedcba9876543210ULL);
      }
   }
   else
   {
      printf("movdqu_3 ... failed\n");
   }

   return;
}

static void movdq2q_1(void)
{
   reg128_t arg0 = { .uq = { 0x012345678abcdefULL, 0xfedcba9876543210ULL } };
   reg64_t arg1 = { .uq = { 0x1212121234343434ULL } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movq %3, %%mm6\n"
         "movdq2q %%xmm12, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (result0.uq[0] == 0x012345678abcdefULL )
      {
         printf("movdq2q_1 ... ok\n");
      }
      else
      {
         printf("movdq2q_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x012345678abcdefULL);
      }
   }
   else
   {
      printf("movdq2q_1 ... failed\n");
   }

   return;
}

static void movhpd_1(void)
{
   reg64_t arg0 = { .pd = { 1234.5678 } };
   reg128_t arg1 = { .pd = { 1111.1111, 2222.2222 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "movhpd %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "mm6"
      );

      if (eq_double(result0.pd[0], 1111.1111) && eq_double(result0.pd[1], 1234.5678) )
      {
         printf("movhpd_1 ... ok\n");
      }
      else
      {
         printf("movhpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1111.1111);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 1234.5678);
      }
   }
   else
   {
      printf("movhpd_1 ... failed\n");
   }

   return;
}

static void movhpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg64_t arg1 = { .pd = { 1111.1111 } };
   reg64_t result0 = arg1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movhpd %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (eq_double(result0.pd[0], 8765.4321) )
      {
         printf("movhpd_2 ... ok\n");
      }
      else
      {
         printf("movhpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8765.4321);
      }
   }
   else
   {
      printf("movhpd_2 ... failed\n");
   }

   return;
}

static void movlpd_1(void)
{
   reg64_t arg0 = { .pd = { 1234.5678 } };
   reg128_t arg1 = { .pd = { 1111.1111, 2222.2222 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "movlpd %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "mm6"
      );

      if (eq_double(result0.pd[0], 1234.5678) && eq_double(result0.pd[1], 2222.2222) )
      {
         printf("movlpd_1 ... ok\n");
      }
      else
      {
         printf("movlpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 2222.2222);
      }
   }
   else
   {
      printf("movlpd_1 ... failed\n");
   }

   return;
}

static void movlpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg64_t arg1 = { .pd = { 1111.1111 } };
   reg64_t result0 = arg1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movlpd %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.5678) )
      {
         printf("movlpd_2 ... ok\n");
      }
      else
      {
         printf("movlpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
      }
   }
   else
   {
      printf("movlpd_2 ... failed\n");
   }

   return;
}

static void movmskpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, -1234.5678 } };
   reg32_t arg1 = { .sd = { 0 } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "movmskpd %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.sd[0] == 2 )
      {
         printf("movmskpd_1 ... ok\n");
      }
      else
      {
         printf("movmskpd_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 2);
      }
   }
   else
   {
      printf("movmskpd_1 ... failed\n");
   }

   return;
}

static void movntdq_1(void)
{
   reg128_t arg0 = { .uq = { 0x012345678abcdefULL, 0xfedcba9876543210ULL } };
   reg128_t arg1 = { .uq = { 0x1212121234343434ULL, 0x5656565678787878ULL } };
   reg128_t result0 = arg1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movntdq %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x012345678abcdefULL && result0.uq[1] == 0xfedcba9876543210ULL )
      {
         printf("movntdq_1 ... ok\n");
      }
      else
      {
         printf("movntdq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x012345678abcdefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfedcba9876543210ULL);
      }
   }
   else
   {
      printf("movntdq_1 ... failed\n");
   }

   return;
}

static void movnti_1(void)
{
   reg32_t arg0 = { .sd = { 12345678 } };
   reg32_t arg1 = { .sd = { 11111111 } };
   reg32_t result0 = arg1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %1, %%r9d\n"
         "movnti %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "r9"
      );

      if (result0.sd[0] == 12345678 )
      {
         printf("movnti_1 ... ok\n");
      }
      else
      {
         printf("movnti_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12345678);
      }
   }
   else
   {
      printf("movnti_1 ... failed\n");
   }

   return;
}

static void movntpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 1111.1111, 2222.2222 } };
   reg128_t result0 = arg1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movntpd %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.5678) && eq_double(result0.pd[1], 8765.4321) )
      {
         printf("movntpd_1 ... ok\n");
      }
      else
      {
         printf("movntpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 8765.4321);
      }
   }
   else
   {
      printf("movntpd_1 ... failed\n");
   }

   return;
}

static void movq_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 1111.1111, 2222.2222 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "movq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.5678) && eq_double(result0.pd[1], 0.0) )
      {
         printf("movq_1 ... ok\n");
      }
      else
      {
         printf("movq_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 0.0);
      }
   }
   else
   {
      printf("movq_1 ... failed\n");
   }

   return;
}

static void movq_2(void)
{
   reg64_t arg0 = { .pd = { 1234.5678 } };
   reg128_t arg1 = { .pd = { 1111.1111, 2222.2222 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "movq %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "mm6"
      );

      if (eq_double(result0.pd[0], 1234.5678) && eq_double(result0.pd[1], 0.0) )
      {
         printf("movq_2 ... ok\n");
      }
      else
      {
         printf("movq_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 0.0);
      }
   }
   else
   {
      printf("movq_2 ... failed\n");
   }

   return;
}

static void movq_3(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg64_t arg1 = { .pd = { 1111.1111 } };
   reg64_t result0 = arg1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movq %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.5678) )
      {
         printf("movq_3 ... ok\n");
      }
      else
      {
         printf("movq_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
      }
   }
   else
   {
      printf("movq_3 ... failed\n");
   }

   return;
}

static void movq2dq_1(void)
{
   reg64_t arg0 = { .uq = { 0x012345678abcdefULL } };
   reg128_t arg1 = { .uq = { 0x1212121234343434ULL, 0x5656565678787878ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movq %2, %%mm6\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "movq2dq %%mm6, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "mm6"
      );

      if (result0.uq[0] == 0x012345678abcdefULL && result0.uq[1] == 0ULL )
      {
         printf("movq2dq_1 ... ok\n");
      }
      else
      {
         printf("movq2dq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x012345678abcdefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0ULL);
      }
   }
   else
   {
      printf("movq2dq_1 ... failed\n");
   }

   return;
}

static void movsd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 1111.1111, 2222.2222 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "movsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.5678) && eq_double(result0.pd[1], 2222.2222) )
      {
         printf("movsd_1 ... ok\n");
      }
      else
      {
         printf("movsd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 2222.2222);
      }
   }
   else
   {
      printf("movsd_1 ... failed\n");
   }

   return;
}

static void movsd_2(void)
{
   reg64_t arg0 = { .pd = { 1234.5678 } };
   reg128_t arg1 = { .pd = { 1111.1111, 2222.2222 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "movsd %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "mm6"
      );

      if (eq_double(result0.pd[0], 1234.5678) && eq_double(result0.pd[1], 0.0) )
      {
         printf("movsd_2 ... ok\n");
      }
      else
      {
         printf("movsd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 0.0);
      }
   }
   else
   {
      printf("movsd_2 ... failed\n");
   }

   return;
}

static void movsd_3(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg64_t arg1 = { .pd = { 1111.1111 } };
   reg64_t result0 = arg1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movsd %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.5678) )
      {
         printf("movsd_3 ... ok\n");
      }
      else
      {
         printf("movsd_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
      }
   }
   else
   {
      printf("movsd_3 ... failed\n");
   }

   return;
}

static void movupd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 1111.1111, 2222.2222 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "movupd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.5678) && eq_double(result0.pd[1], 8765.4321) )
      {
         printf("movupd_1 ... ok\n");
      }
      else
      {
         printf("movupd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 8765.4321);
      }
   }
   else
   {
      printf("movupd_1 ... failed\n");
   }

   return;
}

static void movupd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 1111.1111, 2222.2222 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "movupd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.5678) && eq_double(result0.pd[1], 8765.4321) )
      {
         printf("movupd_2 ... ok\n");
      }
      else
      {
         printf("movupd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 8765.4321);
      }
   }
   else
   {
      printf("movupd_2 ... failed\n");
   }

   return;
}

static void mulpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 3.0, 2.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "mulpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 3703.7034) && eq_double(result0.pd[1], 17530.8642) )
      {
         printf("mulpd_1 ... ok\n");
      }
      else
      {
         printf("mulpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 3703.7034);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 17530.8642);
      }
   }
   else
   {
      printf("mulpd_1 ... failed\n");
   }

   return;
}

static void mulpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 3.0, 2.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "mulpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 3703.7034) && eq_double(result0.pd[1], 17530.8642) )
      {
         printf("mulpd_2 ... ok\n");
      }
      else
      {
         printf("mulpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 3703.7034);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 17530.8642);
      }
   }
   else
   {
      printf("mulpd_2 ... failed\n");
   }

   return;
}

static void mulsd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 3.0, 2.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "mulsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 3703.7034) && eq_double(result0.pd[1], 2.0) )
      {
         printf("mulsd_1 ... ok\n");
      }
      else
      {
         printf("mulsd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 3703.7034);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 2.0);
      }
   }
   else
   {
      printf("mulsd_1 ... failed\n");
   }

   return;
}

static void mulsd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 3.0, 2.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "mulsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 3703.7034) && eq_double(result0.pd[1], 2.0) )
      {
         printf("mulsd_2 ... ok\n");
      }
      else
      {
         printf("mulsd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 3703.7034);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 2.0);
      }
   }
   else
   {
      printf("mulsd_2 ... failed\n");
   }

   return;
}

static void orpd_1(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "orpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfdbb7577edabedefULL && result0.uq[1] == 0xfdbb7577edabedefULL )
      {
         printf("orpd_1 ... ok\n");
      }
      else
      {
         printf("orpd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfdbb7577edabedefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfdbb7577edabedefULL);
      }
   }
   else
   {
      printf("orpd_1 ... failed\n");
   }

   return;
}

static void orpd_2(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "orpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfdbb7577edabedefULL && result0.uq[1] == 0xfdbb7577edabedefULL )
      {
         printf("orpd_2 ... ok\n");
      }
      else
      {
         printf("orpd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfdbb7577edabedefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfdbb7577edabedefULL);
      }
   }
   else
   {
      printf("orpd_2 ... failed\n");
   }

   return;
}

static void packssdw_1(void)
{
   reg128_t arg0 = { .sd = { 12345, -12345, 123456, -123456 } };
   reg128_t arg1 = { .sd = { 4321, -4321, 54321, -54321 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "packssdw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 4321 && result0.sw[1] == -4321 && result0.sw[2] == 32767 && result0.sw[3] == -32768 && result0.sw[4] == 12345 && result0.sw[5] == -12345 && result0.sw[6] == 32767 && result0.sw[7] == -32768 )
      {
         printf("packssdw_1 ... ok\n");
      }
      else
      {
         printf("packssdw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 4321);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], -4321);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 32767);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], -32768);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 12345);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], -12345);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 32767);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], -32768);
      }
   }
   else
   {
      printf("packssdw_1 ... failed\n");
   }

   return;
}

static void packssdw_2(void)
{
   reg128_t arg0 = { .sd = { 12345, -12345, 123456, -123456 } };
   reg128_t arg1 = { .sd = { 4321, -4321, 54321, -54321 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "packssdw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 4321 && result0.sw[1] == -4321 && result0.sw[2] == 32767 && result0.sw[3] == -32768 && result0.sw[4] == 12345 && result0.sw[5] == -12345 && result0.sw[6] == 32767 && result0.sw[7] == -32768 )
      {
         printf("packssdw_2 ... ok\n");
      }
      else
      {
         printf("packssdw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 4321);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], -4321);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 32767);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], -32768);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 12345);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], -12345);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 32767);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], -32768);
      }
   }
   else
   {
      printf("packssdw_2 ... failed\n");
   }

   return;
}

static void packsswb_1(void)
{
   reg128_t arg0 = { .sw = { 123, -123, 1234, -1234, 123, -123, 1234, -1234 } };
   reg128_t arg1 = { .sw = { 21, -21, 321, -321, 21, -21, 321, -321 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "packsswb %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sb[0] == 21 && result0.sb[1] == -21 && result0.sb[2] == 127 && result0.sb[3] == -128 && result0.sb[4] == 21 && result0.sb[5] == -21 && result0.sb[6] == 127 && result0.sb[7] == -128 && result0.sb[8] == 123 && result0.sb[9] == -123 && result0.sb[10] == 127 && result0.sb[11] == -128 && result0.sb[12] == 123 && result0.sb[13] == -123 && result0.sb[14] == 127 && result0.sb[15] == -128 )
      {
         printf("packsswb_1 ... ok\n");
      }
      else
      {
         printf("packsswb_1 ... not ok\n");
         printf("  result0.sb[0] = %d (expected %d)\n", result0.sb[0], 21);
         printf("  result0.sb[1] = %d (expected %d)\n", result0.sb[1], -21);
         printf("  result0.sb[2] = %d (expected %d)\n", result0.sb[2], 127);
         printf("  result0.sb[3] = %d (expected %d)\n", result0.sb[3], -128);
         printf("  result0.sb[4] = %d (expected %d)\n", result0.sb[4], 21);
         printf("  result0.sb[5] = %d (expected %d)\n", result0.sb[5], -21);
         printf("  result0.sb[6] = %d (expected %d)\n", result0.sb[6], 127);
         printf("  result0.sb[7] = %d (expected %d)\n", result0.sb[7], -128);
         printf("  result0.sb[8] = %d (expected %d)\n", result0.sb[8], 123);
         printf("  result0.sb[9] = %d (expected %d)\n", result0.sb[9], -123);
         printf("  result0.sb[10] = %d (expected %d)\n", result0.sb[10], 127);
         printf("  result0.sb[11] = %d (expected %d)\n", result0.sb[11], -128);
         printf("  result0.sb[12] = %d (expected %d)\n", result0.sb[12], 123);
         printf("  result0.sb[13] = %d (expected %d)\n", result0.sb[13], -123);
         printf("  result0.sb[14] = %d (expected %d)\n", result0.sb[14], 127);
         printf("  result0.sb[15] = %d (expected %d)\n", result0.sb[15], -128);
      }
   }
   else
   {
      printf("packsswb_1 ... failed\n");
   }

   return;
}

static void packsswb_2(void)
{
   reg128_t arg0 = { .sw = { 123, -123, 1234, -1234, 123, -123, 1234, -1234 } };
   reg128_t arg1 = { .sw = { 21, -21, 321, -321, 21, -21, 321, -321 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "packsswb %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sb[0] == 21 && result0.sb[1] == -21 && result0.sb[2] == 127 && result0.sb[3] == -128 && result0.sb[4] == 21 && result0.sb[5] == -21 && result0.sb[6] == 127 && result0.sb[7] == -128 && result0.sb[8] == 123 && result0.sb[9] == -123 && result0.sb[10] == 127 && result0.sb[11] == -128 && result0.sb[12] == 123 && result0.sb[13] == -123 && result0.sb[14] == 127 && result0.sb[15] == -128 )
      {
         printf("packsswb_2 ... ok\n");
      }
      else
      {
         printf("packsswb_2 ... not ok\n");
         printf("  result0.sb[0] = %d (expected %d)\n", result0.sb[0], 21);
         printf("  result0.sb[1] = %d (expected %d)\n", result0.sb[1], -21);
         printf("  result0.sb[2] = %d (expected %d)\n", result0.sb[2], 127);
         printf("  result0.sb[3] = %d (expected %d)\n", result0.sb[3], -128);
         printf("  result0.sb[4] = %d (expected %d)\n", result0.sb[4], 21);
         printf("  result0.sb[5] = %d (expected %d)\n", result0.sb[5], -21);
         printf("  result0.sb[6] = %d (expected %d)\n", result0.sb[6], 127);
         printf("  result0.sb[7] = %d (expected %d)\n", result0.sb[7], -128);
         printf("  result0.sb[8] = %d (expected %d)\n", result0.sb[8], 123);
         printf("  result0.sb[9] = %d (expected %d)\n", result0.sb[9], -123);
         printf("  result0.sb[10] = %d (expected %d)\n", result0.sb[10], 127);
         printf("  result0.sb[11] = %d (expected %d)\n", result0.sb[11], -128);
         printf("  result0.sb[12] = %d (expected %d)\n", result0.sb[12], 123);
         printf("  result0.sb[13] = %d (expected %d)\n", result0.sb[13], -123);
         printf("  result0.sb[14] = %d (expected %d)\n", result0.sb[14], 127);
         printf("  result0.sb[15] = %d (expected %d)\n", result0.sb[15], -128);
      }
   }
   else
   {
      printf("packsswb_2 ... failed\n");
   }

   return;
}

static void packuswb_1(void)
{
   reg128_t arg0 = { .sw = { 123, -123, 1234, -1234, 123, -123, 1234, -1234 } };
   reg128_t arg1 = { .sw = { 21, -21, 321, -321, 21, -21, 321, -321 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "packuswb %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 21U && result0.ub[1] == 0U && result0.ub[2] == 255U && result0.ub[3] == 0U && result0.ub[4] == 21U && result0.ub[5] == 0U && result0.ub[6] == 255U && result0.ub[7] == 0U && result0.ub[8] == 123U && result0.ub[9] == 0U && result0.ub[10] == 255U && result0.ub[11] == 0U && result0.ub[12] == 123U && result0.ub[13] == 0U && result0.ub[14] == 255U && result0.ub[15] == 0U )
      {
         printf("packuswb_1 ... ok\n");
      }
      else
      {
         printf("packuswb_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 21U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 0U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 255U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 0U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 21U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 0U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 255U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 0U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 123U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 0U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 255U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 0U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 123U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 0U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 255U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 0U);
      }
   }
   else
   {
      printf("packuswb_1 ... failed\n");
   }

   return;
}

static void packuswb_2(void)
{
   reg128_t arg0 = { .sw = { 123, -123, 1234, -1234, 123, -123, 1234, -1234 } };
   reg128_t arg1 = { .sw = { 21, -21, 321, -321, 21, -21, 321, -321 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "packuswb %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 21U && result0.ub[1] == 0U && result0.ub[2] == 255U && result0.ub[3] == 0U && result0.ub[4] == 21U && result0.ub[5] == 0U && result0.ub[6] == 255U && result0.ub[7] == 0U && result0.ub[8] == 123U && result0.ub[9] == 0U && result0.ub[10] == 255U && result0.ub[11] == 0U && result0.ub[12] == 123U && result0.ub[13] == 0U && result0.ub[14] == 255U && result0.ub[15] == 0U )
      {
         printf("packuswb_2 ... ok\n");
      }
      else
      {
         printf("packuswb_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 21U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 0U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 255U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 0U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 21U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 0U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 255U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 0U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 123U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 0U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 255U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 0U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 123U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 0U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 255U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 0U);
      }
   }
   else
   {
      printf("packuswb_2 ... failed\n");
   }

   return;
}

static void paddb_1(void)
{
   reg128_t arg0 = { .sb = { 12, 34, 56, 78, 21, 43, 65, 87, 12, 34, 56, 78, 21, 43, 65, 87 } };
   reg128_t arg1 = { .sb = { 8, 7, 6, 5, 4, 3, 2, 1, 8, 7, 6, 5, 4, 3, 2, 1 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "paddb %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sb[0] == 20 && result0.sb[1] == 41 && result0.sb[2] == 62 && result0.sb[3] == 83 && result0.sb[4] == 25 && result0.sb[5] == 46 && result0.sb[6] == 67 && result0.sb[7] == 88 && result0.sb[8] == 20 && result0.sb[9] == 41 && result0.sb[10] == 62 && result0.sb[11] == 83 && result0.sb[12] == 25 && result0.sb[13] == 46 && result0.sb[14] == 67 && result0.sb[15] == 88 )
      {
         printf("paddb_1 ... ok\n");
      }
      else
      {
         printf("paddb_1 ... not ok\n");
         printf("  result0.sb[0] = %d (expected %d)\n", result0.sb[0], 20);
         printf("  result0.sb[1] = %d (expected %d)\n", result0.sb[1], 41);
         printf("  result0.sb[2] = %d (expected %d)\n", result0.sb[2], 62);
         printf("  result0.sb[3] = %d (expected %d)\n", result0.sb[3], 83);
         printf("  result0.sb[4] = %d (expected %d)\n", result0.sb[4], 25);
         printf("  result0.sb[5] = %d (expected %d)\n", result0.sb[5], 46);
         printf("  result0.sb[6] = %d (expected %d)\n", result0.sb[6], 67);
         printf("  result0.sb[7] = %d (expected %d)\n", result0.sb[7], 88);
         printf("  result0.sb[8] = %d (expected %d)\n", result0.sb[8], 20);
         printf("  result0.sb[9] = %d (expected %d)\n", result0.sb[9], 41);
         printf("  result0.sb[10] = %d (expected %d)\n", result0.sb[10], 62);
         printf("  result0.sb[11] = %d (expected %d)\n", result0.sb[11], 83);
         printf("  result0.sb[12] = %d (expected %d)\n", result0.sb[12], 25);
         printf("  result0.sb[13] = %d (expected %d)\n", result0.sb[13], 46);
         printf("  result0.sb[14] = %d (expected %d)\n", result0.sb[14], 67);
         printf("  result0.sb[15] = %d (expected %d)\n", result0.sb[15], 88);
      }
   }
   else
   {
      printf("paddb_1 ... failed\n");
   }

   return;
}

static void paddb_2(void)
{
   reg128_t arg0 = { .sb = { 12, 34, 56, 78, 21, 43, 65, 87, 12, 34, 56, 78, 21, 43, 65, 87 } };
   reg128_t arg1 = { .sb = { 8, 7, 6, 5, 4, 3, 2, 1, 8, 7, 6, 5, 4, 3, 2, 1 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "paddb %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sb[0] == 20 && result0.sb[1] == 41 && result0.sb[2] == 62 && result0.sb[3] == 83 && result0.sb[4] == 25 && result0.sb[5] == 46 && result0.sb[6] == 67 && result0.sb[7] == 88 && result0.sb[8] == 20 && result0.sb[9] == 41 && result0.sb[10] == 62 && result0.sb[11] == 83 && result0.sb[12] == 25 && result0.sb[13] == 46 && result0.sb[14] == 67 && result0.sb[15] == 88 )
      {
         printf("paddb_2 ... ok\n");
      }
      else
      {
         printf("paddb_2 ... not ok\n");
         printf("  result0.sb[0] = %d (expected %d)\n", result0.sb[0], 20);
         printf("  result0.sb[1] = %d (expected %d)\n", result0.sb[1], 41);
         printf("  result0.sb[2] = %d (expected %d)\n", result0.sb[2], 62);
         printf("  result0.sb[3] = %d (expected %d)\n", result0.sb[3], 83);
         printf("  result0.sb[4] = %d (expected %d)\n", result0.sb[4], 25);
         printf("  result0.sb[5] = %d (expected %d)\n", result0.sb[5], 46);
         printf("  result0.sb[6] = %d (expected %d)\n", result0.sb[6], 67);
         printf("  result0.sb[7] = %d (expected %d)\n", result0.sb[7], 88);
         printf("  result0.sb[8] = %d (expected %d)\n", result0.sb[8], 20);
         printf("  result0.sb[9] = %d (expected %d)\n", result0.sb[9], 41);
         printf("  result0.sb[10] = %d (expected %d)\n", result0.sb[10], 62);
         printf("  result0.sb[11] = %d (expected %d)\n", result0.sb[11], 83);
         printf("  result0.sb[12] = %d (expected %d)\n", result0.sb[12], 25);
         printf("  result0.sb[13] = %d (expected %d)\n", result0.sb[13], 46);
         printf("  result0.sb[14] = %d (expected %d)\n", result0.sb[14], 67);
         printf("  result0.sb[15] = %d (expected %d)\n", result0.sb[15], 88);
      }
   }
   else
   {
      printf("paddb_2 ... failed\n");
   }

   return;
}

static void paddd_1(void)
{
   reg128_t arg0 = { .sd = { 12345678, 87654321, 12345678, 87654321 } };
   reg128_t arg1 = { .sd = { 8765, 4321, 8765, 4321 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "paddd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 12354443 && result0.sd[1] == 87658642 && result0.sd[2] == 12354443 && result0.sd[3] == 87658642 )
      {
         printf("paddd_1 ... ok\n");
      }
      else
      {
         printf("paddd_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12354443);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 87658642);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 12354443);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 87658642);
      }
   }
   else
   {
      printf("paddd_1 ... failed\n");
   }

   return;
}

static void paddd_2(void)
{
   reg128_t arg0 = { .sd = { 12345678, 87654321, 12345678, 87654321 } };
   reg128_t arg1 = { .sd = { 8765, 4321, 8765, 4321 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "paddd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 12354443 && result0.sd[1] == 87658642 && result0.sd[2] == 12354443 && result0.sd[3] == 87658642 )
      {
         printf("paddd_2 ... ok\n");
      }
      else
      {
         printf("paddd_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12354443);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 87658642);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 12354443);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 87658642);
      }
   }
   else
   {
      printf("paddd_2 ... failed\n");
   }

   return;
}

static void paddq_1(void)
{
   reg64_t arg0 = { .sq = { 11111111LL } };
   reg64_t arg1 = { .sq = { 22222222LL } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movq %1, %%mm6\n"
         "movq %2, %%mm7\n"
         "paddq %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sq[0] == 33333333LL )
      {
         printf("paddq_1 ... ok\n");
      }
      else
      {
         printf("paddq_1 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 33333333LL);
      }
   }
   else
   {
      printf("paddq_1 ... failed\n");
   }

   return;
}

static void paddq_2(void)
{
   reg64_t arg0 = { .sq = { 11111111LL } };
   reg64_t arg1 = { .sq = { 22222222LL } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movq %2, %%mm7\n"
         "paddq %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sq[0] == 33333333LL )
      {
         printf("paddq_2 ... ok\n");
      }
      else
      {
         printf("paddq_2 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 33333333LL);
      }
   }
   else
   {
      printf("paddq_2 ... failed\n");
   }

   return;
}

static void paddq_3(void)
{
   reg128_t arg0 = { .sq = { 11111111LL, 22222222LL } };
   reg128_t arg1 = { .sq = { 22222222LL, 33333333LL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "paddq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sq[0] == 33333333LL && result0.sq[1] == 55555555LL )
      {
         printf("paddq_3 ... ok\n");
      }
      else
      {
         printf("paddq_3 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 33333333LL);
         printf("  result0.sq[1] = %lld (expected %lld)\n", result0.sq[1], 55555555LL);
      }
   }
   else
   {
      printf("paddq_3 ... failed\n");
   }

   return;
}

static void paddq_4(void)
{
   reg128_t arg0 = { .sq = { 11111111LL, 22222222LL } };
   reg128_t arg1 = { .sq = { 22222222LL, 33333333LL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "paddq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sq[0] == 33333333LL && result0.sq[1] == 55555555LL )
      {
         printf("paddq_4 ... ok\n");
      }
      else
      {
         printf("paddq_4 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 33333333LL);
         printf("  result0.sq[1] = %lld (expected %lld)\n", result0.sq[1], 55555555LL);
      }
   }
   else
   {
      printf("paddq_4 ... failed\n");
   }

   return;
}

static void paddsb_1(void)
{
   reg128_t arg0 = { .sb = { 25, -25, 50, -50, 100, -100, 125, -125, 25, -25, 50, -50, 100, -100, 125, -125 } };
   reg128_t arg1 = { .sb = { 40, -40, 30, -30, 20, -20, 10, -10, 40, -40, 30, -30, 20, -20, 10, -10 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "paddsb %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sb[0] == 65 && result0.sb[1] == -65 && result0.sb[2] == 80 && result0.sb[3] == -80 && result0.sb[4] == 120 && result0.sb[5] == -120 && result0.sb[6] == 127 && result0.sb[7] == -128 && result0.sb[8] == 65 && result0.sb[9] == -65 && result0.sb[10] == 80 && result0.sb[11] == -80 && result0.sb[12] == 120 && result0.sb[13] == -120 && result0.sb[14] == 127 && result0.sb[15] == -128 )
      {
         printf("paddsb_1 ... ok\n");
      }
      else
      {
         printf("paddsb_1 ... not ok\n");
         printf("  result0.sb[0] = %d (expected %d)\n", result0.sb[0], 65);
         printf("  result0.sb[1] = %d (expected %d)\n", result0.sb[1], -65);
         printf("  result0.sb[2] = %d (expected %d)\n", result0.sb[2], 80);
         printf("  result0.sb[3] = %d (expected %d)\n", result0.sb[3], -80);
         printf("  result0.sb[4] = %d (expected %d)\n", result0.sb[4], 120);
         printf("  result0.sb[5] = %d (expected %d)\n", result0.sb[5], -120);
         printf("  result0.sb[6] = %d (expected %d)\n", result0.sb[6], 127);
         printf("  result0.sb[7] = %d (expected %d)\n", result0.sb[7], -128);
         printf("  result0.sb[8] = %d (expected %d)\n", result0.sb[8], 65);
         printf("  result0.sb[9] = %d (expected %d)\n", result0.sb[9], -65);
         printf("  result0.sb[10] = %d (expected %d)\n", result0.sb[10], 80);
         printf("  result0.sb[11] = %d (expected %d)\n", result0.sb[11], -80);
         printf("  result0.sb[12] = %d (expected %d)\n", result0.sb[12], 120);
         printf("  result0.sb[13] = %d (expected %d)\n", result0.sb[13], -120);
         printf("  result0.sb[14] = %d (expected %d)\n", result0.sb[14], 127);
         printf("  result0.sb[15] = %d (expected %d)\n", result0.sb[15], -128);
      }
   }
   else
   {
      printf("paddsb_1 ... failed\n");
   }

   return;
}

static void paddsb_2(void)
{
   reg128_t arg0 = { .sb = { 25, -25, 50, -50, 100, -100, 125, -125, 25, -25, 50, -50, 100, -100, 125, -125 } };
   reg128_t arg1 = { .sb = { 40, -40, 30, -30, 20, -20, 10, -10, 40, -40, 30, -30, 20, -20, 10, -10 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "paddsb %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sb[0] == 65 && result0.sb[1] == -65 && result0.sb[2] == 80 && result0.sb[3] == -80 && result0.sb[4] == 120 && result0.sb[5] == -120 && result0.sb[6] == 127 && result0.sb[7] == -128 && result0.sb[8] == 65 && result0.sb[9] == -65 && result0.sb[10] == 80 && result0.sb[11] == -80 && result0.sb[12] == 120 && result0.sb[13] == -120 && result0.sb[14] == 127 && result0.sb[15] == -128 )
      {
         printf("paddsb_2 ... ok\n");
      }
      else
      {
         printf("paddsb_2 ... not ok\n");
         printf("  result0.sb[0] = %d (expected %d)\n", result0.sb[0], 65);
         printf("  result0.sb[1] = %d (expected %d)\n", result0.sb[1], -65);
         printf("  result0.sb[2] = %d (expected %d)\n", result0.sb[2], 80);
         printf("  result0.sb[3] = %d (expected %d)\n", result0.sb[3], -80);
         printf("  result0.sb[4] = %d (expected %d)\n", result0.sb[4], 120);
         printf("  result0.sb[5] = %d (expected %d)\n", result0.sb[5], -120);
         printf("  result0.sb[6] = %d (expected %d)\n", result0.sb[6], 127);
         printf("  result0.sb[7] = %d (expected %d)\n", result0.sb[7], -128);
         printf("  result0.sb[8] = %d (expected %d)\n", result0.sb[8], 65);
         printf("  result0.sb[9] = %d (expected %d)\n", result0.sb[9], -65);
         printf("  result0.sb[10] = %d (expected %d)\n", result0.sb[10], 80);
         printf("  result0.sb[11] = %d (expected %d)\n", result0.sb[11], -80);
         printf("  result0.sb[12] = %d (expected %d)\n", result0.sb[12], 120);
         printf("  result0.sb[13] = %d (expected %d)\n", result0.sb[13], -120);
         printf("  result0.sb[14] = %d (expected %d)\n", result0.sb[14], 127);
         printf("  result0.sb[15] = %d (expected %d)\n", result0.sb[15], -128);
      }
   }
   else
   {
      printf("paddsb_2 ... failed\n");
   }

   return;
}

static void paddsw_1(void)
{
   reg128_t arg0 = { .sw = { 12345, -12345, 32145, -32145, 12345, -12345, 32145, -32145 } };
   reg128_t arg1 = { .sw = { 32145, -32145, -12345, 12345, 32145, -32145, -12345, 12345 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "paddsw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 32767 && result0.sw[1] == -32768 && result0.sw[2] == 19800 && result0.sw[3] == -19800 && result0.sw[4] == 32767 && result0.sw[5] == -32768 && result0.sw[6] == 19800 && result0.sw[7] == -19800 )
      {
         printf("paddsw_1 ... ok\n");
      }
      else
      {
         printf("paddsw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 32767);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], -32768);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 19800);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], -19800);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 32767);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], -32768);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 19800);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], -19800);
      }
   }
   else
   {
      printf("paddsw_1 ... failed\n");
   }

   return;
}

static void paddsw_2(void)
{
   reg128_t arg0 = { .sw = { 12345, -12345, 32145, -32145, 12345, -12345, 32145, -32145 } };
   reg128_t arg1 = { .sw = { 32145, -32145, -12345, 12345, 32145, -32145, -12345, 12345 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "paddsw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 32767 && result0.sw[1] == -32768 && result0.sw[2] == 19800 && result0.sw[3] == -19800 && result0.sw[4] == 32767 && result0.sw[5] == -32768 && result0.sw[6] == 19800 && result0.sw[7] == -19800 )
      {
         printf("paddsw_2 ... ok\n");
      }
      else
      {
         printf("paddsw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 32767);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], -32768);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 19800);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], -19800);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 32767);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], -32768);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 19800);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], -19800);
      }
   }
   else
   {
      printf("paddsw_2 ... failed\n");
   }

   return;
}

static void paddusb_1(void)
{
   reg128_t arg0 = { .ub = { 25U, 50U, 75U, 100U, 125U, 150U, 175U, 200U, 25U, 50U, 75U, 100U, 125U, 150U, 175U, 200U } };
   reg128_t arg1 = { .ub = { 10U, 20U, 30U, 40U, 50U, 60U, 70U, 80U, 10U, 20U, 30U, 40U, 50U, 60U, 70U, 80U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "paddusb %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 35U && result0.ub[1] == 70U && result0.ub[2] == 105U && result0.ub[3] == 140U && result0.ub[4] == 175U && result0.ub[5] == 210U && result0.ub[6] == 245U && result0.ub[7] == 255U && result0.ub[8] == 35U && result0.ub[9] == 70U && result0.ub[10] == 105U && result0.ub[11] == 140U && result0.ub[12] == 175U && result0.ub[13] == 210U && result0.ub[14] == 245U && result0.ub[15] == 255U )
      {
         printf("paddusb_1 ... ok\n");
      }
      else
      {
         printf("paddusb_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 35U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 70U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 105U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 140U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 175U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 210U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 245U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 255U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 35U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 70U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 105U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 140U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 175U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 210U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 245U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 255U);
      }
   }
   else
   {
      printf("paddusb_1 ... failed\n");
   }

   return;
}

static void paddusb_2(void)
{
   reg128_t arg0 = { .ub = { 25U, 50U, 75U, 100U, 125U, 150U, 175U, 200U, 25U, 50U, 75U, 100U, 125U, 150U, 175U, 200U } };
   reg128_t arg1 = { .ub = { 10U, 20U, 30U, 40U, 50U, 60U, 70U, 80U, 10U, 20U, 30U, 40U, 50U, 60U, 70U, 80U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "paddusb %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 35U && result0.ub[1] == 70U && result0.ub[2] == 105U && result0.ub[3] == 140U && result0.ub[4] == 175U && result0.ub[5] == 210U && result0.ub[6] == 245U && result0.ub[7] == 255U && result0.ub[8] == 35U && result0.ub[9] == 70U && result0.ub[10] == 105U && result0.ub[11] == 140U && result0.ub[12] == 175U && result0.ub[13] == 210U && result0.ub[14] == 245U && result0.ub[15] == 255U )
      {
         printf("paddusb_2 ... ok\n");
      }
      else
      {
         printf("paddusb_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 35U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 70U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 105U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 140U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 175U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 210U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 245U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 255U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 35U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 70U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 105U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 140U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 175U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 210U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 245U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 255U);
      }
   }
   else
   {
      printf("paddusb_2 ... failed\n");
   }

   return;
}

static void paddusw_1(void)
{
   reg128_t arg0 = { .uw = { 22222, 33333, 44444, 55555, 22222, 33333, 44444, 55555 } };
   reg128_t arg1 = { .uw = { 6666, 7777, 8888, 9999, 6666, 7777, 8888, 9999 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "paddusw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 28888 && result0.uw[1] == 41110 && result0.uw[2] == 53332 && result0.uw[3] == 65535 && result0.uw[4] == 28888 && result0.uw[5] == 41110 && result0.uw[6] == 53332 && result0.uw[7] == 65535 )
      {
         printf("paddusw_1 ... ok\n");
      }
      else
      {
         printf("paddusw_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 28888);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 41110);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 53332);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 65535);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 28888);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 41110);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 53332);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 65535);
      }
   }
   else
   {
      printf("paddusw_1 ... failed\n");
   }

   return;
}

static void paddusw_2(void)
{
   reg128_t arg0 = { .uw = { 22222, 33333, 44444, 55555, 22222, 33333, 44444, 55555 } };
   reg128_t arg1 = { .uw = { 6666, 7777, 8888, 9999, 6666, 7777, 8888, 9999 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "paddusw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 28888 && result0.uw[1] == 41110 && result0.uw[2] == 53332 && result0.uw[3] == 65535 && result0.uw[4] == 28888 && result0.uw[5] == 41110 && result0.uw[6] == 53332 && result0.uw[7] == 65535 )
      {
         printf("paddusw_2 ... ok\n");
      }
      else
      {
         printf("paddusw_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 28888);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 41110);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 53332);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 65535);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 28888);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 41110);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 53332);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 65535);
      }
   }
   else
   {
      printf("paddusw_2 ... failed\n");
   }

   return;
}

static void paddw_1(void)
{
   reg128_t arg0 = { .sw = { 1234, 5678, 4321, 8765, 1234, 5678, 4321, 8765 } };
   reg128_t arg1 = { .sw = { 87, 65, 43, 21, 87, 65, 43, 21 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "paddw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 1321 && result0.sw[1] == 5743 && result0.sw[2] == 4364 && result0.sw[3] == 8786 && result0.sw[4] == 1321 && result0.sw[5] == 5743 && result0.sw[6] == 4364 && result0.sw[7] == 8786 )
      {
         printf("paddw_1 ... ok\n");
      }
      else
      {
         printf("paddw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1321);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 5743);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 4364);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 8786);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 1321);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], 5743);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 4364);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], 8786);
      }
   }
   else
   {
      printf("paddw_1 ... failed\n");
   }

   return;
}

static void paddw_2(void)
{
   reg128_t arg0 = { .sw = { 1234, 5678, 4321, 8765, 1234, 5678, 4321, 8765 } };
   reg128_t arg1 = { .sw = { 87, 65, 43, 21, 87, 65, 43, 21 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "paddw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 1321 && result0.sw[1] == 5743 && result0.sw[2] == 4364 && result0.sw[3] == 8786 && result0.sw[4] == 1321 && result0.sw[5] == 5743 && result0.sw[6] == 4364 && result0.sw[7] == 8786 )
      {
         printf("paddw_2 ... ok\n");
      }
      else
      {
         printf("paddw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1321);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 5743);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 4364);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 8786);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 1321);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], 5743);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 4364);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], 8786);
      }
   }
   else
   {
      printf("paddw_2 ... failed\n");
   }

   return;
}

static void pand_1(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pand %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0121452188a84420ULL && result0.uq[1] == 0x0121452188a84420ULL )
      {
         printf("pand_1 ... ok\n");
      }
      else
      {
         printf("pand_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0121452188a84420ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0121452188a84420ULL);
      }
   }
   else
   {
      printf("pand_1 ... failed\n");
   }

   return;
}

static void pand_2(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pand %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0121452188a84420ULL && result0.uq[1] == 0x0121452188a84420ULL )
      {
         printf("pand_2 ... ok\n");
      }
      else
      {
         printf("pand_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0121452188a84420ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0121452188a84420ULL);
      }
   }
   else
   {
      printf("pand_2 ... failed\n");
   }

   return;
}

static void pandn_1(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pandn %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfc98301064002000ULL && result0.uq[1] == 0x00020046010389cfULL )
      {
         printf("pandn_1 ... ok\n");
      }
      else
      {
         printf("pandn_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc98301064002000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x00020046010389cfULL);
      }
   }
   else
   {
      printf("pandn_1 ... failed\n");
   }

   return;
}

static void pandn_2(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pandn %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfc98301064002000ULL && result0.uq[1] == 0x00020046010389cfULL )
      {
         printf("pandn_2 ... ok\n");
      }
      else
      {
         printf("pandn_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc98301064002000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x00020046010389cfULL);
      }
   }
   else
   {
      printf("pandn_2 ... failed\n");
   }

   return;
}

static void pavgb_1(void)
{
   reg128_t arg0 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U, 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
   reg128_t arg1 = { .ub = { 15U, 25U, 35U, 45U, 55U, 65U, 75U, 85U, 15U, 25U, 35U, 45U, 55U, 65U, 75U, 85U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pavgb %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 13U && result0.ub[1] == 24U && result0.ub[2] == 34U && result0.ub[3] == 45U && result0.ub[4] == 55U && result0.ub[5] == 66U && result0.ub[6] == 76U && result0.ub[7] == 87U && result0.ub[8] == 13U && result0.ub[9] == 24U && result0.ub[10] == 34U && result0.ub[11] == 45U && result0.ub[12] == 55U && result0.ub[13] == 66U && result0.ub[14] == 76U && result0.ub[15] == 87U )
      {
         printf("pavgb_1 ... ok\n");
      }
      else
      {
         printf("pavgb_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 13U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 24U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 34U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 45U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 55U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 66U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 76U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 87U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 13U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 24U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 34U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 45U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 55U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 66U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 76U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 87U);
      }
   }
   else
   {
      printf("pavgb_1 ... failed\n");
   }

   return;
}

static void pavgb_2(void)
{
   reg128_t arg0 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U, 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
   reg128_t arg1 = { .ub = { 15U, 25U, 35U, 45U, 55U, 65U, 75U, 85U, 15U, 25U, 35U, 45U, 55U, 65U, 75U, 85U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pavgb %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 13U && result0.ub[1] == 24U && result0.ub[2] == 34U && result0.ub[3] == 45U && result0.ub[4] == 55U && result0.ub[5] == 66U && result0.ub[6] == 76U && result0.ub[7] == 87U && result0.ub[8] == 13U && result0.ub[9] == 24U && result0.ub[10] == 34U && result0.ub[11] == 45U && result0.ub[12] == 55U && result0.ub[13] == 66U && result0.ub[14] == 76U && result0.ub[15] == 87U )
      {
         printf("pavgb_2 ... ok\n");
      }
      else
      {
         printf("pavgb_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 13U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 24U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 34U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 45U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 55U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 66U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 76U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 87U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 13U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 24U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 34U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 45U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 55U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 66U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 76U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 87U);
      }
   }
   else
   {
      printf("pavgb_2 ... failed\n");
   }

   return;
}

static void pavgw_1(void)
{
   reg128_t arg0 = { .uw = { 1122, 3344, 5566, 7788, 1122, 3344, 5566, 7788 } };
   reg128_t arg1 = { .uw = { 1525, 3545, 5565, 7585, 1525, 3545, 5565, 7585 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pavgw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 1324 && result0.uw[1] == 3445 && result0.uw[2] == 5566 && result0.uw[3] == 7687 && result0.uw[4] == 1324 && result0.uw[5] == 3445 && result0.uw[6] == 5566 && result0.uw[7] == 7687 )
      {
         printf("pavgw_1 ... ok\n");
      }
      else
      {
         printf("pavgw_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1324);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 3445);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 5566);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 7687);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1324);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 3445);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 5566);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 7687);
      }
   }
   else
   {
      printf("pavgw_1 ... failed\n");
   }

   return;
}

static void pavgw_2(void)
{
   reg128_t arg0 = { .uw = { 1122, 3344, 5566, 7788, 1122, 3344, 5566, 7788 } };
   reg128_t arg1 = { .uw = { 1525, 3545, 5565, 7585, 1525, 3545, 5565, 7585 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pavgw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 1324 && result0.uw[1] == 3445 && result0.uw[2] == 5566 && result0.uw[3] == 7687 && result0.uw[4] == 1324 && result0.uw[5] == 3445 && result0.uw[6] == 5566 && result0.uw[7] == 7687 )
      {
         printf("pavgw_2 ... ok\n");
      }
      else
      {
         printf("pavgw_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1324);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 3445);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 5566);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 7687);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1324);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 3445);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 5566);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 7687);
      }
   }
   else
   {
      printf("pavgw_2 ... failed\n");
   }

   return;
}

static void pcmpeqb_1(void)
{
   reg128_t arg0 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U, 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
   reg128_t arg1 = { .ub = { 11U, 11U, 33U, 33U, 55U, 55U, 77U, 77U, 11U, 11U, 33U, 33U, 55U, 55U, 77U, 77U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pcmpeqb %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 0xffU && result0.ub[1] == 0x00U && result0.ub[2] == 0xffU && result0.ub[3] == 0x00U && result0.ub[4] == 0xffU && result0.ub[5] == 0x00U && result0.ub[6] == 0xffU && result0.ub[7] == 0x00U && result0.ub[8] == 0xffU && result0.ub[9] == 0x00U && result0.ub[10] == 0xffU && result0.ub[11] == 0x00U && result0.ub[12] == 0xffU && result0.ub[13] == 0x00U && result0.ub[14] == 0xffU && result0.ub[15] == 0x00U )
      {
         printf("pcmpeqb_1 ... ok\n");
      }
      else
      {
         printf("pcmpeqb_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 0xffU);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 0x00U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 0xffU);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 0x00U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 0xffU);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 0x00U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 0xffU);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 0x00U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 0xffU);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 0x00U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 0xffU);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 0x00U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 0xffU);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 0x00U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 0xffU);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 0x00U);
      }
   }
   else
   {
      printf("pcmpeqb_1 ... failed\n");
   }

   return;
}

static void pcmpeqb_2(void)
{
   reg128_t arg0 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U, 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
   reg128_t arg1 = { .ub = { 11U, 11U, 33U, 33U, 55U, 55U, 77U, 77U, 11U, 11U, 33U, 33U, 55U, 55U, 77U, 77U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pcmpeqb %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 0xffU && result0.ub[1] == 0x00U && result0.ub[2] == 0xffU && result0.ub[3] == 0x00U && result0.ub[4] == 0xffU && result0.ub[5] == 0x00U && result0.ub[6] == 0xffU && result0.ub[7] == 0x00U && result0.ub[8] == 0xffU && result0.ub[9] == 0x00U && result0.ub[10] == 0xffU && result0.ub[11] == 0x00U && result0.ub[12] == 0xffU && result0.ub[13] == 0x00U && result0.ub[14] == 0xffU && result0.ub[15] == 0x00U )
      {
         printf("pcmpeqb_2 ... ok\n");
      }
      else
      {
         printf("pcmpeqb_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 0xffU);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 0x00U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 0xffU);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 0x00U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 0xffU);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 0x00U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 0xffU);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 0x00U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 0xffU);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 0x00U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 0xffU);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 0x00U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 0xffU);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 0x00U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 0xffU);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 0x00U);
      }
   }
   else
   {
      printf("pcmpeqb_2 ... failed\n");
   }

   return;
}

static void pcmpeqd_1(void)
{
   reg128_t arg0 = { .ud = { 11223344, 55667788, 11223344, 55667788 } };
   reg128_t arg1 = { .ud = { 11223344, 11223344, 11223344, 11223344 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pcmpeqd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("pcmpeqd_1 ... ok\n");
      }
      else
      {
         printf("pcmpeqd_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("pcmpeqd_1 ... failed\n");
   }

   return;
}

static void pcmpeqd_2(void)
{
   reg128_t arg0 = { .ud = { 11223344, 55667788, 11223344, 55667788 } };
   reg128_t arg1 = { .ud = { 11223344, 11223344, 11223344, 11223344 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pcmpeqd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("pcmpeqd_2 ... ok\n");
      }
      else
      {
         printf("pcmpeqd_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("pcmpeqd_2 ... failed\n");
   }

   return;
}

static void pcmpeqw_1(void)
{
   reg128_t arg0 = { .uw = { 1122, 3344, 5566, 7788, 1122, 3344, 5566, 7788 } };
   reg128_t arg1 = { .uw = { 1122, 1122, 5566, 5566, 1122, 1122, 5566, 5566 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pcmpeqw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0xffff && result0.uw[1] == 0x0000 && result0.uw[2] == 0xffff && result0.uw[3] == 0x0000 && result0.uw[4] == 0xffff && result0.uw[5] == 0x0000 && result0.uw[6] == 0xffff && result0.uw[7] == 0x0000 )
      {
         printf("pcmpeqw_1 ... ok\n");
      }
      else
      {
         printf("pcmpeqw_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0xffff);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x0000);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0xffff);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0x0000);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0xffff);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x0000);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0xffff);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0x0000);
      }
   }
   else
   {
      printf("pcmpeqw_1 ... failed\n");
   }

   return;
}

static void pcmpeqw_2(void)
{
   reg128_t arg0 = { .uw = { 1122, 3344, 5566, 7788, 1122, 3344, 5566, 7788 } };
   reg128_t arg1 = { .uw = { 1122, 1122, 5566, 5566, 1122, 1122, 5566, 5566 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pcmpeqw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0xffff && result0.uw[1] == 0x0000 && result0.uw[2] == 0xffff && result0.uw[3] == 0x0000 && result0.uw[4] == 0xffff && result0.uw[5] == 0x0000 && result0.uw[6] == 0xffff && result0.uw[7] == 0x0000 )
      {
         printf("pcmpeqw_2 ... ok\n");
      }
      else
      {
         printf("pcmpeqw_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0xffff);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x0000);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0xffff);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0x0000);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0xffff);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x0000);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0xffff);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0x0000);
      }
   }
   else
   {
      printf("pcmpeqw_2 ... failed\n");
   }

   return;
}

static void pcmpgtb_1(void)
{
   reg128_t arg0 = { .sb = { -77, -55, -33, -11, 11, 33, 55, 77, -77, -55, -33, -11, 11, 33, 55, 77 } };
   reg128_t arg1 = { .sb = { 77, 55, 33, 11, -11, -33, -55, -77, 77, 55, 33, 11, -11, -33, -55, -77 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pcmpgtb %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 0xffU && result0.ub[1] == 0xffU && result0.ub[2] == 0xffU && result0.ub[3] == 0xffU && result0.ub[4] == 0x00U && result0.ub[5] == 0x00U && result0.ub[6] == 0x00U && result0.ub[7] == 0x00U && result0.ub[8] == 0xffU && result0.ub[9] == 0xffU && result0.ub[10] == 0xffU && result0.ub[11] == 0xffU && result0.ub[12] == 0x00U && result0.ub[13] == 0x00U && result0.ub[14] == 0x00U && result0.ub[15] == 0x00U )
      {
         printf("pcmpgtb_1 ... ok\n");
      }
      else
      {
         printf("pcmpgtb_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 0xffU);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 0xffU);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 0xffU);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 0xffU);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 0x00U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 0x00U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 0x00U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 0x00U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 0xffU);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 0xffU);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 0xffU);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 0xffU);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 0x00U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 0x00U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 0x00U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 0x00U);
      }
   }
   else
   {
      printf("pcmpgtb_1 ... failed\n");
   }

   return;
}

static void pcmpgtb_2(void)
{
   reg128_t arg0 = { .sb = { -77, -55, -33, -11, 11, 33, 55, 77, -77, -55, -33, -11, 11, 33, 55, 77 } };
   reg128_t arg1 = { .sb = { 77, 55, 33, 11, -11, -33, -55, -77, 77, 55, 33, 11, -11, -33, -55, -77 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pcmpgtb %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 0xffU && result0.ub[1] == 0xffU && result0.ub[2] == 0xffU && result0.ub[3] == 0xffU && result0.ub[4] == 0x00U && result0.ub[5] == 0x00U && result0.ub[6] == 0x00U && result0.ub[7] == 0x00U && result0.ub[8] == 0xffU && result0.ub[9] == 0xffU && result0.ub[10] == 0xffU && result0.ub[11] == 0xffU && result0.ub[12] == 0x00U && result0.ub[13] == 0x00U && result0.ub[14] == 0x00U && result0.ub[15] == 0x00U )
      {
         printf("pcmpgtb_2 ... ok\n");
      }
      else
      {
         printf("pcmpgtb_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 0xffU);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 0xffU);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 0xffU);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 0xffU);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 0x00U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 0x00U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 0x00U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 0x00U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 0xffU);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 0xffU);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 0xffU);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 0xffU);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 0x00U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 0x00U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 0x00U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 0x00U);
      }
   }
   else
   {
      printf("pcmpgtb_2 ... failed\n");
   }

   return;
}

static void pcmpgtd_1(void)
{
   reg128_t arg0 = { .sd = { -11111111, 11111111, -11111111, 11111111 } };
   reg128_t arg1 = { .sd = { 11111111, -11111111, 11111111, -11111111 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pcmpgtd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("pcmpgtd_1 ... ok\n");
      }
      else
      {
         printf("pcmpgtd_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("pcmpgtd_1 ... failed\n");
   }

   return;
}

static void pcmpgtd_2(void)
{
   reg128_t arg0 = { .sd = { -11111111, 11111111, -11111111, 11111111 } };
   reg128_t arg1 = { .sd = { 11111111, -11111111, 11111111, -11111111 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pcmpgtd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("pcmpgtd_2 ... ok\n");
      }
      else
      {
         printf("pcmpgtd_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("pcmpgtd_2 ... failed\n");
   }

   return;
}

static void pcmpgtw_1(void)
{
   reg128_t arg0 = { .sw = { -3333, -1111, 1111, 3333, -3333, -1111, 1111, 3333 } };
   reg128_t arg1 = { .sw = { 3333, 1111, -1111, -3333, 3333, 1111, -1111, -3333 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pcmpgtw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0xffff && result0.uw[1] == 0xffff && result0.uw[2] == 0x0000 && result0.uw[3] == 0x0000 && result0.uw[4] == 0xffff && result0.uw[5] == 0xffff && result0.uw[6] == 0x0000 && result0.uw[7] == 0x0000 )
      {
         printf("pcmpgtw_1 ... ok\n");
      }
      else
      {
         printf("pcmpgtw_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0xffff);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0xffff);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0x0000);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0x0000);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0xffff);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0xffff);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0x0000);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0x0000);
      }
   }
   else
   {
      printf("pcmpgtw_1 ... failed\n");
   }

   return;
}

static void pcmpgtw_2(void)
{
   reg128_t arg0 = { .sw = { -3333, -1111, 1111, 3333, -3333, -1111, 1111, 3333 } };
   reg128_t arg1 = { .sw = { 3333, 1111, -1111, -3333, 3333, 1111, -1111, -3333 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pcmpgtw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0xffff && result0.uw[1] == 0xffff && result0.uw[2] == 0x0000 && result0.uw[3] == 0x0000 && result0.uw[4] == 0xffff && result0.uw[5] == 0xffff && result0.uw[6] == 0x0000 && result0.uw[7] == 0x0000 )
      {
         printf("pcmpgtw_2 ... ok\n");
      }
      else
      {
         printf("pcmpgtw_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0xffff);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0xffff);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0x0000);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0x0000);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0xffff);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0xffff);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0x0000);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0x0000);
      }
   }
   else
   {
      printf("pcmpgtw_2 ... failed\n");
   }

   return;
}

static void pextrw_1(void)
{
   reg128_t arg1 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg32_t arg2 = { .ud = { 0xffffffff } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "pextrw $0, %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.ud[0] == 1234 )
      {
         printf("pextrw_1 ... ok\n");
      }
      else
      {
         printf("pextrw_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 1234);
      }
   }
   else
   {
      printf("pextrw_1 ... failed\n");
   }

   return;
}

static void pextrw_2(void)
{
   reg128_t arg1 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg32_t arg2 = { .ud = { 0xffffffff } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "pextrw $1, %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.ud[0] == 5678 )
      {
         printf("pextrw_2 ... ok\n");
      }
      else
      {
         printf("pextrw_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 5678);
      }
   }
   else
   {
      printf("pextrw_2 ... failed\n");
   }

   return;
}

static void pextrw_3(void)
{
   reg128_t arg1 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg32_t arg2 = { .ud = { 0xffffffff } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "pextrw $2, %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.ud[0] == 4321 )
      {
         printf("pextrw_3 ... ok\n");
      }
      else
      {
         printf("pextrw_3 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 4321);
      }
   }
   else
   {
      printf("pextrw_3 ... failed\n");
   }

   return;
}

static void pextrw_4(void)
{
   reg128_t arg1 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg32_t arg2 = { .ud = { 0xffffffff } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "pextrw $3, %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.ud[0] == 8765 )
      {
         printf("pextrw_4 ... ok\n");
      }
      else
      {
         printf("pextrw_4 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 8765);
      }
   }
   else
   {
      printf("pextrw_4 ... failed\n");
   }

   return;
}

static void pextrw_5(void)
{
   reg128_t arg1 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg32_t arg2 = { .ud = { 0xffffffff } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "pextrw $4, %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.ud[0] == 1111 )
      {
         printf("pextrw_5 ... ok\n");
      }
      else
      {
         printf("pextrw_5 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 1111);
      }
   }
   else
   {
      printf("pextrw_5 ... failed\n");
   }

   return;
}

static void pextrw_6(void)
{
   reg128_t arg1 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg32_t arg2 = { .ud = { 0xffffffff } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "pextrw $5, %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.ud[0] == 2222 )
      {
         printf("pextrw_6 ... ok\n");
      }
      else
      {
         printf("pextrw_6 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 2222);
      }
   }
   else
   {
      printf("pextrw_6 ... failed\n");
   }

   return;
}

static void pextrw_7(void)
{
   reg128_t arg1 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg32_t arg2 = { .ud = { 0xffffffff } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "pextrw $6, %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.ud[0] == 3333 )
      {
         printf("pextrw_7 ... ok\n");
      }
      else
      {
         printf("pextrw_7 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 3333);
      }
   }
   else
   {
      printf("pextrw_7 ... failed\n");
   }

   return;
}

static void pextrw_8(void)
{
   reg128_t arg1 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg32_t arg2 = { .ud = { 0xffffffff } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "pextrw $7, %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.ud[0] == 4444 )
      {
         printf("pextrw_8 ... ok\n");
      }
      else
      {
         printf("pextrw_8 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 4444);
      }
   }
   else
   {
      printf("pextrw_8 ... failed\n");
   }

   return;
}

static void pinsrw_1(void)
{
   reg32_t arg1 = { .ud = { 0xffffffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %2, %%r9d\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $0, %%r9d, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12", "r9"
      );

      if (result0.uw[0] == 65535 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 8765 && result0.uw[4] == 1111 && result0.uw[5] == 2222 && result0.uw[6] == 3333 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_1 ... ok\n");
      }
      else
      {
         printf("pinsrw_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 65535);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_1 ... failed\n");
   }

   return;
}

static void pinsrw_2(void)
{
   reg32_t arg1 = { .ud = { 0xffffffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %2, %%r9d\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $1, %%r9d, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12", "r9"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 65535 && result0.uw[2] == 4321 && result0.uw[3] == 8765 && result0.uw[4] == 1111 && result0.uw[5] == 2222 && result0.uw[6] == 3333 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_2 ... ok\n");
      }
      else
      {
         printf("pinsrw_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 65535);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_2 ... failed\n");
   }

   return;
}

static void pinsrw_3(void)
{
   reg32_t arg1 = { .ud = { 0xffffffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %2, %%r9d\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $2, %%r9d, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12", "r9"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 65535 && result0.uw[3] == 8765 && result0.uw[4] == 1111 && result0.uw[5] == 2222 && result0.uw[6] == 3333 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_3 ... ok\n");
      }
      else
      {
         printf("pinsrw_3 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 65535);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_3 ... failed\n");
   }

   return;
}

static void pinsrw_4(void)
{
   reg32_t arg1 = { .ud = { 0xffffffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %2, %%r9d\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $3, %%r9d, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12", "r9"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 65535 && result0.uw[4] == 1111 && result0.uw[5] == 2222 && result0.uw[6] == 3333 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_4 ... ok\n");
      }
      else
      {
         printf("pinsrw_4 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 65535);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_4 ... failed\n");
   }

   return;
}

static void pinsrw_5(void)
{
   reg32_t arg1 = { .ud = { 0xffffffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %2, %%r9d\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $4, %%r9d, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12", "r9"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 8765 && result0.uw[4] == 65535 && result0.uw[5] == 2222 && result0.uw[6] == 3333 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_5 ... ok\n");
      }
      else
      {
         printf("pinsrw_5 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 65535);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_5 ... failed\n");
   }

   return;
}

static void pinsrw_6(void)
{
   reg32_t arg1 = { .ud = { 0xffffffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %2, %%r9d\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $5, %%r9d, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12", "r9"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 8765 && result0.uw[4] == 1111 && result0.uw[5] == 65535 && result0.uw[6] == 3333 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_6 ... ok\n");
      }
      else
      {
         printf("pinsrw_6 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 65535);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_6 ... failed\n");
   }

   return;
}

static void pinsrw_7(void)
{
   reg32_t arg1 = { .ud = { 0xffffffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %2, %%r9d\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $6, %%r9d, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12", "r9"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 8765 && result0.uw[4] == 1111 && result0.uw[5] == 2222 && result0.uw[6] == 65535 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_7 ... ok\n");
      }
      else
      {
         printf("pinsrw_7 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 65535);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_7 ... failed\n");
   }

   return;
}

static void pinsrw_8(void)
{
   reg32_t arg1 = { .ud = { 0xffffffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %2, %%r9d\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $7, %%r9d, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12", "r9"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 8765 && result0.uw[4] == 1111 && result0.uw[5] == 2222 && result0.uw[6] == 3333 && result0.uw[7] == 65535 )
      {
         printf("pinsrw_8 ... ok\n");
      }
      else
      {
         printf("pinsrw_8 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 65535);
      }
   }
   else
   {
      printf("pinsrw_8 ... failed\n");
   }

   return;
}

static void pinsrw_9(void)
{
   reg16_t arg1 = { .uw = { 0xffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $0, %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uw[0] == 65535 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 8765 && result0.uw[4] == 1111 && result0.uw[5] == 2222 && result0.uw[6] == 3333 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_9 ... ok\n");
      }
      else
      {
         printf("pinsrw_9 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 65535);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_9 ... failed\n");
   }

   return;
}

static void pinsrw_10(void)
{
   reg16_t arg1 = { .uw = { 0xffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $1, %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 65535 && result0.uw[2] == 4321 && result0.uw[3] == 8765 && result0.uw[4] == 1111 && result0.uw[5] == 2222 && result0.uw[6] == 3333 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_10 ... ok\n");
      }
      else
      {
         printf("pinsrw_10 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 65535);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_10 ... failed\n");
   }

   return;
}

static void pinsrw_11(void)
{
   reg16_t arg1 = { .uw = { 0xffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $2, %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 65535 && result0.uw[3] == 8765 && result0.uw[4] == 1111 && result0.uw[5] == 2222 && result0.uw[6] == 3333 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_11 ... ok\n");
      }
      else
      {
         printf("pinsrw_11 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 65535);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_11 ... failed\n");
   }

   return;
}

static void pinsrw_12(void)
{
   reg16_t arg1 = { .uw = { 0xffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $3, %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 65535 && result0.uw[4] == 1111 && result0.uw[5] == 2222 && result0.uw[6] == 3333 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_12 ... ok\n");
      }
      else
      {
         printf("pinsrw_12 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 65535);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_12 ... failed\n");
   }

   return;
}

static void pinsrw_13(void)
{
   reg16_t arg1 = { .uw = { 0xffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $4, %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 8765 && result0.uw[4] == 65535 && result0.uw[5] == 2222 && result0.uw[6] == 3333 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_13 ... ok\n");
      }
      else
      {
         printf("pinsrw_13 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 65535);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_13 ... failed\n");
   }

   return;
}

static void pinsrw_14(void)
{
   reg16_t arg1 = { .uw = { 0xffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $5, %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 8765 && result0.uw[4] == 1111 && result0.uw[5] == 65535 && result0.uw[6] == 3333 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_14 ... ok\n");
      }
      else
      {
         printf("pinsrw_14 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 65535);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_14 ... failed\n");
   }

   return;
}

static void pinsrw_15(void)
{
   reg16_t arg1 = { .uw = { 0xffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $6, %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 8765 && result0.uw[4] == 1111 && result0.uw[5] == 2222 && result0.uw[6] == 65535 && result0.uw[7] == 4444 )
      {
         printf("pinsrw_15 ... ok\n");
      }
      else
      {
         printf("pinsrw_15 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 65535);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4444);
      }
   }
   else
   {
      printf("pinsrw_15 ... failed\n");
   }

   return;
}

static void pinsrw_16(void)
{
   reg16_t arg1 = { .uw = { 0xffff } };
   reg128_t arg2 = { .uw = { 1234, 5678, 4321, 8765, 1111, 2222, 3333, 4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "pinsrw $7, %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 8765 && result0.uw[4] == 1111 && result0.uw[5] == 2222 && result0.uw[6] == 3333 && result0.uw[7] == 65535 )
      {
         printf("pinsrw_16 ... ok\n");
      }
      else
      {
         printf("pinsrw_16 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 1111);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2222);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 3333);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 65535);
      }
   }
   else
   {
      printf("pinsrw_16 ... failed\n");
   }

   return;
}

static void pmaddwd_1(void)
{
   reg128_t arg0 = { .sw = { 1234, 5678, -4321, -8765, 999, 888, 777, 666 } };
   reg128_t arg1 = { .sw = { 1111, -2222, 3333, -4444, -987, -31111, -14142, -31416 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pmaddwd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == -11245542 && result0.sd[1] == 24549767 && result0.sd[2] == -28612581 && result0.sd[3] == -31911390 )
      {
         printf("pmaddwd_1 ... ok\n");
      }
      else
      {
         printf("pmaddwd_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -11245542);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 24549767);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], -28612581);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], -31911390);
      }
   }
   else
   {
      printf("pmaddwd_1 ... failed\n");
   }

   return;
}

static void pmaddwd_2(void)
{
   reg128_t arg0 = { .sw = { 1234, 5678, -4321, -8765, 999, 888, 777, 666 } };
   reg128_t arg1 = { .sw = { 1111, -2222, 3333, -4444, -987, -31111, -14142, -31416 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pmaddwd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == -11245542 && result0.sd[1] == 24549767 && result0.sd[2] == -28612581 && result0.sd[3] == -31911390 )
      {
         printf("pmaddwd_2 ... ok\n");
      }
      else
      {
         printf("pmaddwd_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -11245542);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 24549767);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], -28612581);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], -31911390);
      }
   }
   else
   {
      printf("pmaddwd_2 ... failed\n");
   }

   return;
}

static void pmaxsw_1(void)
{
   reg128_t arg0 = { .sw = { -1, 2, -3, 4, -5, 6, -7, 8 } };
   reg128_t arg1 = { .sw = { 2, -3, 4, -5, 6, -7, 8, -9 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pmaxsw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 2 && result0.sw[1] == 2 && result0.sw[2] == 4 && result0.sw[3] == 4 && result0.sw[4] == 6 && result0.sw[5] == 6 && result0.sw[6] == 8 && result0.sw[7] == 8 )
      {
         printf("pmaxsw_1 ... ok\n");
      }
      else
      {
         printf("pmaxsw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 2);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 2);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 4);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 4);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 6);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], 6);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 8);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], 8);
      }
   }
   else
   {
      printf("pmaxsw_1 ... failed\n");
   }

   return;
}

static void pmaxsw_2(void)
{
   reg128_t arg0 = { .sw = { -1, 2, -3, 4, -5, 6, -7, 8 } };
   reg128_t arg1 = { .sw = { 2, -3, 4, -5, 6, -7, 8, -9 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pmaxsw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 2 && result0.sw[1] == 2 && result0.sw[2] == 4 && result0.sw[3] == 4 && result0.sw[4] == 6 && result0.sw[5] == 6 && result0.sw[6] == 8 && result0.sw[7] == 8 )
      {
         printf("pmaxsw_2 ... ok\n");
      }
      else
      {
         printf("pmaxsw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 2);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 2);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 4);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 4);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 6);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], 6);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 8);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], 8);
      }
   }
   else
   {
      printf("pmaxsw_2 ... failed\n");
   }

   return;
}

static void pmaxub_1(void)
{
   reg128_t arg0 = { .ub = { 10U, 11U, 12U, 13U, 14U, 15U, 16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U } };
   reg128_t arg1 = { .ub = { 25U, 24U, 23U, 22U, 21U, 20U, 19U, 18U, 17U, 16U, 15U, 14U, 13U, 12U, 11U, 10U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pmaxub %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 25U && result0.ub[1] == 24U && result0.ub[2] == 23U && result0.ub[3] == 22U && result0.ub[4] == 21U && result0.ub[5] == 20U && result0.ub[6] == 19U && result0.ub[7] == 18U && result0.ub[8] == 18U && result0.ub[9] == 19U && result0.ub[10] == 20U && result0.ub[11] == 21U && result0.ub[12] == 22U && result0.ub[13] == 23U && result0.ub[14] == 24U && result0.ub[15] == 25U )
      {
         printf("pmaxub_1 ... ok\n");
      }
      else
      {
         printf("pmaxub_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 25U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 24U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 23U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 22U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 21U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 20U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 19U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 18U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 18U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 19U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 20U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 21U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 22U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 23U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 24U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 25U);
      }
   }
   else
   {
      printf("pmaxub_1 ... failed\n");
   }

   return;
}

static void pmaxub_2(void)
{
   reg128_t arg0 = { .ub = { 10U, 11U, 12U, 13U, 14U, 15U, 16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U } };
   reg128_t arg1 = { .ub = { 25U, 24U, 23U, 22U, 21U, 20U, 19U, 18U, 17U, 16U, 15U, 14U, 13U, 12U, 11U, 10U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pmaxub %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 25U && result0.ub[1] == 24U && result0.ub[2] == 23U && result0.ub[3] == 22U && result0.ub[4] == 21U && result0.ub[5] == 20U && result0.ub[6] == 19U && result0.ub[7] == 18U && result0.ub[8] == 18U && result0.ub[9] == 19U && result0.ub[10] == 20U && result0.ub[11] == 21U && result0.ub[12] == 22U && result0.ub[13] == 23U && result0.ub[14] == 24U && result0.ub[15] == 25U )
      {
         printf("pmaxub_2 ... ok\n");
      }
      else
      {
         printf("pmaxub_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 25U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 24U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 23U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 22U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 21U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 20U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 19U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 18U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 18U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 19U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 20U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 21U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 22U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 23U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 24U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 25U);
      }
   }
   else
   {
      printf("pmaxub_2 ... failed\n");
   }

   return;
}

static void pminsw_1(void)
{
   reg128_t arg0 = { .sw = { -1, 2, -3, 4, -5, 6, -7, 8 } };
   reg128_t arg1 = { .sw = { 2, -3, 4, -5, 6, -7, 8, -9 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pminsw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == -1 && result0.sw[1] == -3 && result0.sw[2] == -3 && result0.sw[3] == -5 && result0.sw[4] == -5 && result0.sw[5] == -7 && result0.sw[6] == -7 && result0.sw[7] == -9 )
      {
         printf("pminsw_1 ... ok\n");
      }
      else
      {
         printf("pminsw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], -3);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], -3);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], -5);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], -5);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], -7);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], -7);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], -9);
      }
   }
   else
   {
      printf("pminsw_1 ... failed\n");
   }

   return;
}

static void pminsw_2(void)
{
   reg128_t arg0 = { .sw = { -1, 2, -3, 4, -5, 6, -7, 8 } };
   reg128_t arg1 = { .sw = { 2, -3, 4, -5, 6, -7, 8, -9 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pminsw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == -1 && result0.sw[1] == -3 && result0.sw[2] == -3 && result0.sw[3] == -5 && result0.sw[4] == -5 && result0.sw[5] == -7 && result0.sw[6] == -7 && result0.sw[7] == -9 )
      {
         printf("pminsw_2 ... ok\n");
      }
      else
      {
         printf("pminsw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], -3);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], -3);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], -5);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], -5);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], -7);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], -7);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], -9);
      }
   }
   else
   {
      printf("pminsw_2 ... failed\n");
   }

   return;
}

static void pminub_1(void)
{
   reg128_t arg0 = { .ub = { 10U, 11U, 12U, 13U, 14U, 15U, 16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U } };
   reg128_t arg1 = { .ub = { 25U, 24U, 23U, 22U, 21U, 20U, 19U, 18U, 17U, 16U, 15U, 14U, 13U, 12U, 11U, 10U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pminub %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 10U && result0.ub[1] == 11U && result0.ub[2] == 12U && result0.ub[3] == 13U && result0.ub[4] == 14U && result0.ub[5] == 15U && result0.ub[6] == 16U && result0.ub[7] == 17U && result0.ub[8] == 17U && result0.ub[9] == 16U && result0.ub[10] == 15U && result0.ub[11] == 14U && result0.ub[12] == 13U && result0.ub[13] == 12U && result0.ub[14] == 11U && result0.ub[15] == 10U )
      {
         printf("pminub_1 ... ok\n");
      }
      else
      {
         printf("pminub_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 10U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 11U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 12U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 13U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 14U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 15U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 16U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 17U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 17U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 16U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 15U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 14U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 13U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 12U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 11U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 10U);
      }
   }
   else
   {
      printf("pminub_1 ... failed\n");
   }

   return;
}

static void pminub_2(void)
{
   reg128_t arg0 = { .ub = { 10U, 11U, 12U, 13U, 14U, 15U, 16U, 17U, 18U, 19U, 20U, 21U, 22U, 23U, 24U, 25U } };
   reg128_t arg1 = { .ub = { 25U, 24U, 23U, 22U, 21U, 20U, 19U, 18U, 17U, 16U, 15U, 14U, 13U, 12U, 11U, 10U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pminub %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 10U && result0.ub[1] == 11U && result0.ub[2] == 12U && result0.ub[3] == 13U && result0.ub[4] == 14U && result0.ub[5] == 15U && result0.ub[6] == 16U && result0.ub[7] == 17U && result0.ub[8] == 17U && result0.ub[9] == 16U && result0.ub[10] == 15U && result0.ub[11] == 14U && result0.ub[12] == 13U && result0.ub[13] == 12U && result0.ub[14] == 11U && result0.ub[15] == 10U )
      {
         printf("pminub_2 ... ok\n");
      }
      else
      {
         printf("pminub_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 10U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 11U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 12U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 13U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 14U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 15U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 16U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 17U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 17U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 16U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 15U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 14U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 13U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 12U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 11U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 10U);
      }
   }
   else
   {
      printf("pminub_2 ... failed\n");
   }

   return;
}

static void pmovmskb_1(void)
{
   reg128_t arg0 = { .uq = { 0x8000000080008088ULL, 0x8000000080008088ULL } };
   reg32_t arg1 = { .ud = { 0 } };
   reg32_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movl %3, %%r9d\n"
         "pmovmskb %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.ud[0] == 0x8b8b )
      {
         printf("pmovmskb_1 ... ok\n");
      }
      else
      {
         printf("pmovmskb_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x8b8b);
      }
   }
   else
   {
      printf("pmovmskb_1 ... failed\n");
   }

   return;
}

static void pmulhuw_1(void)
{
   reg128_t arg0 = { .uw = { 1111, 2222, 3333, 4444, 5555, 6666, 7777, 8888 } };
   reg128_t arg1 = { .uw = { 5555, 6666, 7777, 8888, 9999, 1111, 2222, 3333 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pmulhuw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0x005e && result0.uw[1] == 0x00e2 && result0.uw[2] == 0x018b && result0.uw[3] == 0x025a && result0.uw[4] == 0x034f && result0.uw[5] == 0x0071 && result0.uw[6] == 0x0107 && result0.uw[7] == 0x01c4 )
      {
         printf("pmulhuw_1 ... ok\n");
      }
      else
      {
         printf("pmulhuw_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x005e);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x00e2);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0x018b);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0x025a);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x034f);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x0071);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0x0107);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0x01c4);
      }
   }
   else
   {
      printf("pmulhuw_1 ... failed\n");
   }

   return;
}

static void pmulhuw_2(void)
{
   reg128_t arg0 = { .uw = { 1111, 2222, 3333, 4444, 5555, 6666, 7777, 8888 } };
   reg128_t arg1 = { .uw = { 5555, 6666, 7777, 8888, 9999, 1111, 2222, 3333 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pmulhuw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0x005e && result0.uw[1] == 0x00e2 && result0.uw[2] == 0x018b && result0.uw[3] == 0x025a && result0.uw[4] == 0x034f && result0.uw[5] == 0x0071 && result0.uw[6] == 0x0107 && result0.uw[7] == 0x01c4 )
      {
         printf("pmulhuw_2 ... ok\n");
      }
      else
      {
         printf("pmulhuw_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x005e);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x00e2);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0x018b);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0x025a);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x034f);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x0071);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0x0107);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0x01c4);
      }
   }
   else
   {
      printf("pmulhuw_2 ... failed\n");
   }

   return;
}

static void pmulhw_1(void)
{
   reg128_t arg0 = { .sw = { 1111, 2222, -1111, -2222, 1111, 2222, -1111, -2222 } };
   reg128_t arg1 = { .sw = { 3333, -4444, 3333, -4444, 3333, -4444, 3333, -4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pmulhw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0x0038 && result0.uw[1] == 0xff69 && result0.uw[2] == 0xffc7 && result0.uw[3] == 0x0096 && result0.uw[4] == 0x0038 && result0.uw[5] == 0xff69 && result0.uw[6] == 0xffc7 && result0.uw[7] == 0x0096 )
      {
         printf("pmulhw_1 ... ok\n");
      }
      else
      {
         printf("pmulhw_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x0038);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0xff69);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0xffc7);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0x0096);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x0038);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0xff69);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0xffc7);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0x0096);
      }
   }
   else
   {
      printf("pmulhw_1 ... failed\n");
   }

   return;
}

static void pmulhw_2(void)
{
   reg128_t arg0 = { .sw = { 1111, 2222, -1111, -2222, 1111, 2222, -1111, -2222 } };
   reg128_t arg1 = { .sw = { 3333, -4444, 3333, -4444, 3333, -4444, 3333, -4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pmulhw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0x0038 && result0.uw[1] == 0xff69 && result0.uw[2] == 0xffc7 && result0.uw[3] == 0x0096 && result0.uw[4] == 0x0038 && result0.uw[5] == 0xff69 && result0.uw[6] == 0xffc7 && result0.uw[7] == 0x0096 )
      {
         printf("pmulhw_2 ... ok\n");
      }
      else
      {
         printf("pmulhw_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x0038);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0xff69);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0xffc7);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0x0096);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x0038);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0xff69);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0xffc7);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0x0096);
      }
   }
   else
   {
      printf("pmulhw_2 ... failed\n");
   }

   return;
}

static void pmullw_1(void)
{
   reg128_t arg0 = { .sw = { 1111, 2222, -1111, -2222, 1111, 2222, -1111, -2222 } };
   reg128_t arg1 = { .sw = { 3333, -4444, 3333, -4444, 3333, -4444, 3333, -4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pmullw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0x80b3 && result0.uw[1] == 0x5378 && result0.uw[2] == 0x7f4d && result0.uw[3] == 0xac88 && result0.uw[4] == 0x80b3 && result0.uw[5] == 0x5378 && result0.uw[6] == 0x7f4d && result0.uw[7] == 0xac88 )
      {
         printf("pmullw_1 ... ok\n");
      }
      else
      {
         printf("pmullw_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x80b3);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x5378);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0x7f4d);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0xac88);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x80b3);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x5378);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0x7f4d);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0xac88);
      }
   }
   else
   {
      printf("pmullw_1 ... failed\n");
   }

   return;
}

static void pmullw_2(void)
{
   reg128_t arg0 = { .sw = { 1111, 2222, -1111, -2222, 1111, 2222, -1111, -2222 } };
   reg128_t arg1 = { .sw = { 3333, -4444, 3333, -4444, 3333, -4444, 3333, -4444 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pmullw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0x80b3 && result0.uw[1] == 0x5378 && result0.uw[2] == 0x7f4d && result0.uw[3] == 0xac88 && result0.uw[4] == 0x80b3 && result0.uw[5] == 0x5378 && result0.uw[6] == 0x7f4d && result0.uw[7] == 0xac88 )
      {
         printf("pmullw_2 ... ok\n");
      }
      else
      {
         printf("pmullw_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x80b3);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x5378);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0x7f4d);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0xac88);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x80b3);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x5378);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0x7f4d);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0xac88);
      }
   }
   else
   {
      printf("pmullw_2 ... failed\n");
   }

   return;
}

static void pmuludq_1(void)
{
   reg64_t arg0 = { .ud = { 12345678, 0 } };
   reg64_t arg1 = { .ud = { 87654321, 0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movq %1, %%mm6\n"
         "movq %2, %%mm7\n"
         "pmuludq %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 1082152022374638ULL )
      {
         printf("pmuludq_1 ... ok\n");
      }
      else
      {
         printf("pmuludq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 1082152022374638ULL);
      }
   }
   else
   {
      printf("pmuludq_1 ... failed\n");
   }

   return;
}

static void pmuludq_2(void)
{
   reg64_t arg0 = { .ud = { 12345678, 0 } };
   reg64_t arg1 = { .ud = { 87654321, 0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movq %2, %%mm7\n"
         "pmuludq %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 1082152022374638ULL )
      {
         printf("pmuludq_2 ... ok\n");
      }
      else
      {
         printf("pmuludq_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 1082152022374638ULL);
      }
   }
   else
   {
      printf("pmuludq_2 ... failed\n");
   }

   return;
}

static void pmuludq_3(void)
{
   reg128_t arg0 = { .ud = { 12345678, 0, 87654321, 0 } };
   reg128_t arg1 = { .ud = { 87654321, 0, 12345678, 0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pmuludq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 1082152022374638ULL && result0.uq[1] == 1082152022374638ULL )
      {
         printf("pmuludq_3 ... ok\n");
      }
      else
      {
         printf("pmuludq_3 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 1082152022374638ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 1082152022374638ULL);
      }
   }
   else
   {
      printf("pmuludq_3 ... failed\n");
   }

   return;
}

static void pmuludq_4(void)
{
   reg128_t arg0 = { .ud = { 12345678, 0, 87654321, 0 } };
   reg128_t arg1 = { .ud = { 87654321, 0, 12345678, 0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pmuludq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 1082152022374638ULL && result0.uq[1] == 1082152022374638ULL )
      {
         printf("pmuludq_4 ... ok\n");
      }
      else
      {
         printf("pmuludq_4 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 1082152022374638ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 1082152022374638ULL);
      }
   }
   else
   {
      printf("pmuludq_4 ... failed\n");
   }

   return;
}

static void por_1(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "por %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfdbb7577edabedefULL && result0.uq[1] == 0xfdbb7577edabedefULL )
      {
         printf("por_1 ... ok\n");
      }
      else
      {
         printf("por_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfdbb7577edabedefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfdbb7577edabedefULL);
      }
   }
   else
   {
      printf("por_1 ... failed\n");
   }

   return;
}

static void por_2(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "por %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfdbb7577edabedefULL && result0.uq[1] == 0xfdbb7577edabedefULL )
      {
         printf("por_2 ... ok\n");
      }
      else
      {
         printf("por_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfdbb7577edabedefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfdbb7577edabedefULL);
      }
   }
   else
   {
      printf("por_2 ... failed\n");
   }

   return;
}

static void psadbw_1(void)
{
   reg128_t arg0 = { .ub = { 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U, 16U } };
   reg128_t arg1 = { .ub = { 16U, 15U, 14U, 13U, 12U, 11U, 10U, 9U, 8U, 7U, 6U, 5U, 4U, 3U, 2U, 1U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psadbw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 64 && result0.sw[1] == 0 && result0.sw[2] == 0 && result0.sw[3] == 0 && result0.sw[4] == 64 && result0.sw[5] == 0 && result0.sw[6] == 0 && result0.sw[7] == 0 )
      {
         printf("psadbw_1 ... ok\n");
      }
      else
      {
         printf("psadbw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 64);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 0);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 0);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 0);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 64);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], 0);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 0);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], 0);
      }
   }
   else
   {
      printf("psadbw_1 ... failed\n");
   }

   return;
}

static void psadbw_2(void)
{
   reg128_t arg0 = { .ub = { 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U, 16U } };
   reg128_t arg1 = { .ub = { 16U, 15U, 14U, 13U, 12U, 11U, 10U, 9U, 8U, 7U, 6U, 5U, 4U, 3U, 2U, 1U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psadbw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 64 && result0.sw[1] == 0 && result0.sw[2] == 0 && result0.sw[3] == 0 && result0.sw[4] == 64 && result0.sw[5] == 0 && result0.sw[6] == 0 && result0.sw[7] == 0 )
      {
         printf("psadbw_2 ... ok\n");
      }
      else
      {
         printf("psadbw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 64);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 0);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 0);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 0);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 64);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], 0);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 0);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], 0);
      }
   }
   else
   {
      printf("psadbw_2 ... failed\n");
   }

   return;
}

static void pshufd_1(void)
{
   reg128_t arg1 = { .sd = { 1122, 3344, 5566, 7788 } };
   reg128_t arg2 = { .sd = { 0, 0, 0, 0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pshufd $0x1b, %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 7788 && result0.sd[1] == 5566 && result0.sd[2] == 3344 && result0.sd[3] == 1122 )
      {
         printf("pshufd_1 ... ok\n");
      }
      else
      {
         printf("pshufd_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 7788);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 5566);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 3344);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 1122);
      }
   }
   else
   {
      printf("pshufd_1 ... failed\n");
   }

   return;
}

static void pshufd_2(void)
{
   reg128_t arg1 = { .sd = { 1122, 3344, 5566, 7788 } };
   reg128_t arg2 = { .sd = { 0, 0, 0, 0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pshufd $0x1b, %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 7788 && result0.sd[1] == 5566 && result0.sd[2] == 3344 && result0.sd[3] == 1122 )
      {
         printf("pshufd_2 ... ok\n");
      }
      else
      {
         printf("pshufd_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 7788);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 5566);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 3344);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 1122);
      }
   }
   else
   {
      printf("pshufd_2 ... failed\n");
   }

   return;
}

static void pshufhw_1(void)
{
   reg128_t arg1 = { .sw = { 11, 22, 33, 44, 55, 66, 77, 88 } };
   reg128_t arg2 = { .sw = { 0, 0, 0, 0, 0, 0, 0, 0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pshufhw $0x1b, %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 11 && result0.sw[1] == 22 && result0.sw[2] == 33 && result0.sw[3] == 44 && result0.sw[4] == 88 && result0.sw[5] == 77 && result0.sw[6] == 66 && result0.sw[7] == 55 )
      {
         printf("pshufhw_1 ... ok\n");
      }
      else
      {
         printf("pshufhw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 11);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 22);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 33);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 44);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 88);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], 77);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 66);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], 55);
      }
   }
   else
   {
      printf("pshufhw_1 ... failed\n");
   }

   return;
}

static void pshufhw_2(void)
{
   reg128_t arg1 = { .sw = { 11, 22, 33, 44, 55, 66, 77, 88 } };
   reg128_t arg2 = { .sw = { 0, 0, 0, 0, 0, 0, 0, 0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pshufhw $0x1b, %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 11 && result0.sw[1] == 22 && result0.sw[2] == 33 && result0.sw[3] == 44 && result0.sw[4] == 88 && result0.sw[5] == 77 && result0.sw[6] == 66 && result0.sw[7] == 55 )
      {
         printf("pshufhw_2 ... ok\n");
      }
      else
      {
         printf("pshufhw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 11);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 22);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 33);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 44);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 88);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], 77);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 66);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], 55);
      }
   }
   else
   {
      printf("pshufhw_2 ... failed\n");
   }

   return;
}

static void pshuflw_1(void)
{
   reg128_t arg1 = { .sw = { 11, 22, 33, 44, 55, 66, 77, 88 } };
   reg128_t arg2 = { .sw = { 0, 0, 0, 0, 0, 0, 0, 0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pshuflw $0x1b, %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 44 && result0.sw[1] == 33 && result0.sw[2] == 22 && result0.sw[3] == 11 && result0.sw[4] == 55 && result0.sw[5] == 66 && result0.sw[6] == 77 && result0.sw[7] == 88 )
      {
         printf("pshuflw_1 ... ok\n");
      }
      else
      {
         printf("pshuflw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 44);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 33);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 22);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 11);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 55);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], 66);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 77);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], 88);
      }
   }
   else
   {
      printf("pshuflw_1 ... failed\n");
   }

   return;
}

static void pshuflw_2(void)
{
   reg128_t arg1 = { .sw = { 11, 22, 33, 44, 55, 66, 77, 88 } };
   reg128_t arg2 = { .sw = { 0, 0, 0, 0, 0, 0, 0, 0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pshuflw $0x1b, %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 44 && result0.sw[1] == 33 && result0.sw[2] == 22 && result0.sw[3] == 11 && result0.sw[4] == 55 && result0.sw[5] == 66 && result0.sw[6] == 77 && result0.sw[7] == 88 )
      {
         printf("pshuflw_2 ... ok\n");
      }
      else
      {
         printf("pshuflw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 44);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 33);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 22);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 11);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 55);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], 66);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 77);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], 88);
      }
   }
   else
   {
      printf("pshuflw_2 ... failed\n");
   }

   return;
}

static void pslld_1(void)
{
   reg128_t arg1 = { .ud = { 0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslld $4, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.ud[0] == 0x12345670 && result0.ud[1] == 0x9abcdef0 && result0.ud[2] == 0x12345670 && result0.ud[3] == 0x9abcdef0 )
      {
         printf("pslld_1 ... ok\n");
      }
      else
      {
         printf("pslld_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x12345670);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x9abcdef0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0x12345670);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x9abcdef0);
      }
   }
   else
   {
      printf("pslld_1 ... failed\n");
   }

   return;
}

static void pslld_2(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .ud = { 0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pslld %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x12345670 && result0.ud[1] == 0x9abcdef0 && result0.ud[2] == 0x12345670 && result0.ud[3] == 0x9abcdef0 )
      {
         printf("pslld_2 ... ok\n");
      }
      else
      {
         printf("pslld_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x12345670);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x9abcdef0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0x12345670);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x9abcdef0);
      }
   }
   else
   {
      printf("pslld_2 ... failed\n");
   }

   return;
}

static void pslld_3(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .ud = { 0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pslld %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x12345670 && result0.ud[1] == 0x9abcdef0 && result0.ud[2] == 0x12345670 && result0.ud[3] == 0x9abcdef0 )
      {
         printf("pslld_3 ... ok\n");
      }
      else
      {
         printf("pslld_3 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x12345670);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x9abcdef0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0x12345670);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x9abcdef0);
      }
   }
   else
   {
      printf("pslld_3 ... failed\n");
   }

   return;
}

static void pslldq_1(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $0, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x8899aabbccddeeffULL && result0.uq[1] == 0x0011223344556677ULL )
      {
         printf("pslldq_1 ... ok\n");
      }
      else
      {
         printf("pslldq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x8899aabbccddeeffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0011223344556677ULL);
      }
   }
   else
   {
      printf("pslldq_1 ... failed\n");
   }

   return;
}

static void pslldq_2(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $1, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x99aabbccddeeff00ULL && result0.uq[1] == 0x1122334455667788ULL )
      {
         printf("pslldq_2 ... ok\n");
      }
      else
      {
         printf("pslldq_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x99aabbccddeeff00ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x1122334455667788ULL);
      }
   }
   else
   {
      printf("pslldq_2 ... failed\n");
   }

   return;
}

static void pslldq_3(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0xaabbccddeeff0000ULL && result0.uq[1] == 0x2233445566778899ULL )
      {
         printf("pslldq_3 ... ok\n");
      }
      else
      {
         printf("pslldq_3 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xaabbccddeeff0000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x2233445566778899ULL);
      }
   }
   else
   {
      printf("pslldq_3 ... failed\n");
   }

   return;
}

static void pslldq_4(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $3, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0xbbccddeeff000000ULL && result0.uq[1] == 0x33445566778899aaULL )
      {
         printf("pslldq_4 ... ok\n");
      }
      else
      {
         printf("pslldq_4 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xbbccddeeff000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x33445566778899aaULL);
      }
   }
   else
   {
      printf("pslldq_4 ... failed\n");
   }

   return;
}

static void pslldq_5(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $4, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0xccddeeff00000000ULL && result0.uq[1] == 0x445566778899aabbULL )
      {
         printf("pslldq_5 ... ok\n");
      }
      else
      {
         printf("pslldq_5 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xccddeeff00000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x445566778899aabbULL);
      }
   }
   else
   {
      printf("pslldq_5 ... failed\n");
   }

   return;
}

static void pslldq_6(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $5, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0xddeeff0000000000ULL && result0.uq[1] == 0x5566778899aabbccULL )
      {
         printf("pslldq_6 ... ok\n");
      }
      else
      {
         printf("pslldq_6 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xddeeff0000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x5566778899aabbccULL);
      }
   }
   else
   {
      printf("pslldq_6 ... failed\n");
   }

   return;
}

static void pslldq_7(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $6, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0xeeff000000000000ULL && result0.uq[1] == 0x66778899aabbccddULL )
      {
         printf("pslldq_7 ... ok\n");
      }
      else
      {
         printf("pslldq_7 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xeeff000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x66778899aabbccddULL);
      }
   }
   else
   {
      printf("pslldq_7 ... failed\n");
   }

   return;
}

static void pslldq_8(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $7, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0xff00000000000000ULL && result0.uq[1] == 0x778899aabbccddeeULL )
      {
         printf("pslldq_8 ... ok\n");
      }
      else
      {
         printf("pslldq_8 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xff00000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x778899aabbccddeeULL);
      }
   }
   else
   {
      printf("pslldq_8 ... failed\n");
   }

   return;
}

static void pslldq_9(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $8, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0x8899aabbccddeeffULL )
      {
         printf("pslldq_9 ... ok\n");
      }
      else
      {
         printf("pslldq_9 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x8899aabbccddeeffULL);
      }
   }
   else
   {
      printf("pslldq_9 ... failed\n");
   }

   return;
}

static void pslldq_10(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $9, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0x99aabbccddeeff00ULL )
      {
         printf("pslldq_10 ... ok\n");
      }
      else
      {
         printf("pslldq_10 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x99aabbccddeeff00ULL);
      }
   }
   else
   {
      printf("pslldq_10 ... failed\n");
   }

   return;
}

static void pslldq_11(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $10, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0xaabbccddeeff0000ULL )
      {
         printf("pslldq_11 ... ok\n");
      }
      else
      {
         printf("pslldq_11 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xaabbccddeeff0000ULL);
      }
   }
   else
   {
      printf("pslldq_11 ... failed\n");
   }

   return;
}

static void pslldq_12(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $11, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0xbbccddeeff000000ULL )
      {
         printf("pslldq_12 ... ok\n");
      }
      else
      {
         printf("pslldq_12 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xbbccddeeff000000ULL);
      }
   }
   else
   {
      printf("pslldq_12 ... failed\n");
   }

   return;
}

static void pslldq_13(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $12, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0xccddeeff00000000ULL )
      {
         printf("pslldq_13 ... ok\n");
      }
      else
      {
         printf("pslldq_13 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xccddeeff00000000ULL);
      }
   }
   else
   {
      printf("pslldq_13 ... failed\n");
   }

   return;
}

static void pslldq_14(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $13, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0xddeeff0000000000ULL )
      {
         printf("pslldq_14 ... ok\n");
      }
      else
      {
         printf("pslldq_14 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xddeeff0000000000ULL);
      }
   }
   else
   {
      printf("pslldq_14 ... failed\n");
   }

   return;
}

static void pslldq_15(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $14, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0xeeff000000000000ULL )
      {
         printf("pslldq_15 ... ok\n");
      }
      else
      {
         printf("pslldq_15 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xeeff000000000000ULL);
      }
   }
   else
   {
      printf("pslldq_15 ... failed\n");
   }

   return;
}

static void pslldq_16(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $15, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0xff00000000000000ULL )
      {
         printf("pslldq_16 ... ok\n");
      }
      else
      {
         printf("pslldq_16 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xff00000000000000ULL);
      }
   }
   else
   {
      printf("pslldq_16 ... failed\n");
   }

   return;
}

static void pslldq_17(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x0011223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pslldq $16, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("pslldq_17 ... ok\n");
      }
      else
      {
         printf("pslldq_17 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("pslldq_17 ... failed\n");
   }

   return;
}

static void psllq_1(void)
{
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0x0123456789abcdefULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psllq $4, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x123456789abcdef0ULL && result0.uq[1] == 0x123456789abcdef0ULL )
      {
         printf("psllq_1 ... ok\n");
      }
      else
      {
         printf("psllq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x123456789abcdef0ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x123456789abcdef0ULL);
      }
   }
   else
   {
      printf("psllq_1 ... failed\n");
   }

   return;
}

static void psllq_2(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0x0123456789abcdefULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psllq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x123456789abcdef0ULL && result0.uq[1] == 0x123456789abcdef0ULL )
      {
         printf("psllq_2 ... ok\n");
      }
      else
      {
         printf("psllq_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x123456789abcdef0ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x123456789abcdef0ULL);
      }
   }
   else
   {
      printf("psllq_2 ... failed\n");
   }

   return;
}

static void psllq_3(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0x0123456789abcdefULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psllq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x123456789abcdef0ULL && result0.uq[1] == 0x123456789abcdef0ULL )
      {
         printf("psllq_3 ... ok\n");
      }
      else
      {
         printf("psllq_3 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x123456789abcdef0ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x123456789abcdef0ULL);
      }
   }
   else
   {
      printf("psllq_3 ... failed\n");
   }

   return;
}

static void psllw_1(void)
{
   reg128_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef, 0x0123, 0x4567, 0x89ab, 0xcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psllw $4, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uw[0] == 0x1230 && result0.uw[1] == 0x5670 && result0.uw[2] == 0x9ab0 && result0.uw[3] == 0xdef0 && result0.uw[4] == 0x1230 && result0.uw[5] == 0x5670 && result0.uw[6] == 0x9ab0 && result0.uw[7] == 0xdef0 )
      {
         printf("psllw_1 ... ok\n");
      }
      else
      {
         printf("psllw_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x1230);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x5670);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0x9ab0);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0xdef0);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x1230);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x5670);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0x9ab0);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0xdef0);
      }
   }
   else
   {
      printf("psllw_1 ... failed\n");
   }

   return;
}

static void psllw_2(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef, 0x0123, 0x4567, 0x89ab, 0xcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psllw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0x1230 && result0.uw[1] == 0x5670 && result0.uw[2] == 0x9ab0 && result0.uw[3] == 0xdef0 && result0.uw[4] == 0x1230 && result0.uw[5] == 0x5670 && result0.uw[6] == 0x9ab0 && result0.uw[7] == 0xdef0 )
      {
         printf("psllw_2 ... ok\n");
      }
      else
      {
         printf("psllw_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x1230);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x5670);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0x9ab0);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0xdef0);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x1230);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x5670);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0x9ab0);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0xdef0);
      }
   }
   else
   {
      printf("psllw_2 ... failed\n");
   }

   return;
}

static void psllw_3(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef, 0x0123, 0x4567, 0x89ab, 0xcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psllw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0x1230 && result0.uw[1] == 0x5670 && result0.uw[2] == 0x9ab0 && result0.uw[3] == 0xdef0 && result0.uw[4] == 0x1230 && result0.uw[5] == 0x5670 && result0.uw[6] == 0x9ab0 && result0.uw[7] == 0xdef0 )
      {
         printf("psllw_3 ... ok\n");
      }
      else
      {
         printf("psllw_3 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x1230);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x5670);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0x9ab0);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0xdef0);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x1230);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x5670);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0x9ab0);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0xdef0);
      }
   }
   else
   {
      printf("psllw_3 ... failed\n");
   }

   return;
}

static void psrad_1(void)
{
   reg128_t arg1 = { .ud = { 0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrad $4, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.ud[0] == 0x00123456 && result0.ud[1] == 0xf89abcde && result0.ud[2] == 0x00123456 && result0.ud[3] == 0xf89abcde )
      {
         printf("psrad_1 ... ok\n");
      }
      else
      {
         printf("psrad_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00123456);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0xf89abcde);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0x00123456);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0xf89abcde);
      }
   }
   else
   {
      printf("psrad_1 ... failed\n");
   }

   return;
}

static void psrad_2(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .ud = { 0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psrad %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00123456 && result0.ud[1] == 0xf89abcde && result0.ud[2] == 0x00123456 && result0.ud[3] == 0xf89abcde )
      {
         printf("psrad_2 ... ok\n");
      }
      else
      {
         printf("psrad_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00123456);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0xf89abcde);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0x00123456);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0xf89abcde);
      }
   }
   else
   {
      printf("psrad_2 ... failed\n");
   }

   return;
}

static void psrad_3(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .ud = { 0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psrad %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00123456 && result0.ud[1] == 0xf89abcde && result0.ud[2] == 0x00123456 && result0.ud[3] == 0xf89abcde )
      {
         printf("psrad_3 ... ok\n");
      }
      else
      {
         printf("psrad_3 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00123456);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0xf89abcde);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0x00123456);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0xf89abcde);
      }
   }
   else
   {
      printf("psrad_3 ... failed\n");
   }

   return;
}

static void psraw_1(void)
{
   reg128_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef, 0x0123, 0x4567, 0x89ab, 0xcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psraw $4, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uw[0] == 0x0012 && result0.uw[1] == 0x0456 && result0.uw[2] == 0xf89a && result0.uw[3] == 0xfcde && result0.uw[4] == 0x0012 && result0.uw[5] == 0x0456 && result0.uw[6] == 0xf89a && result0.uw[7] == 0xfcde )
      {
         printf("psraw_1 ... ok\n");
      }
      else
      {
         printf("psraw_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x0012);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x0456);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0xf89a);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0xfcde);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x0012);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x0456);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0xf89a);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0xfcde);
      }
   }
   else
   {
      printf("psraw_1 ... failed\n");
   }

   return;
}

static void psraw_2(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef, 0x0123, 0x4567, 0x89ab, 0xcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psraw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0x0012 && result0.uw[1] == 0x0456 && result0.uw[2] == 0xf89a && result0.uw[3] == 0xfcde && result0.uw[4] == 0x0012 && result0.uw[5] == 0x0456 && result0.uw[6] == 0xf89a && result0.uw[7] == 0xfcde )
      {
         printf("psraw_2 ... ok\n");
      }
      else
      {
         printf("psraw_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x0012);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x0456);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0xf89a);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0xfcde);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x0012);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x0456);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0xf89a);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0xfcde);
      }
   }
   else
   {
      printf("psraw_2 ... failed\n");
   }

   return;
}

static void psraw_3(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef, 0x0123, 0x4567, 0x89ab, 0xcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psraw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0x0012 && result0.uw[1] == 0x0456 && result0.uw[2] == 0xf89a && result0.uw[3] == 0xfcde && result0.uw[4] == 0x0012 && result0.uw[5] == 0x0456 && result0.uw[6] == 0xf89a && result0.uw[7] == 0xfcde )
      {
         printf("psraw_3 ... ok\n");
      }
      else
      {
         printf("psraw_3 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x0012);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x0456);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0xf89a);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0xfcde);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x0012);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x0456);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0xf89a);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0xfcde);
      }
   }
   else
   {
      printf("psraw_3 ... failed\n");
   }

   return;
}

static void psrld_1(void)
{
   reg128_t arg1 = { .ud = { 0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrld $4, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.ud[0] == 0x00123456 && result0.ud[1] == 0x089abcde && result0.ud[2] == 0x00123456 && result0.ud[3] == 0x089abcde )
      {
         printf("psrld_1 ... ok\n");
      }
      else
      {
         printf("psrld_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00123456);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x089abcde);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0x00123456);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x089abcde);
      }
   }
   else
   {
      printf("psrld_1 ... failed\n");
   }

   return;
}

static void psrld_2(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .ud = { 0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psrld %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00123456 && result0.ud[1] == 0x089abcde && result0.ud[2] == 0x00123456 && result0.ud[3] == 0x089abcde )
      {
         printf("psrld_2 ... ok\n");
      }
      else
      {
         printf("psrld_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00123456);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x089abcde);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0x00123456);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x089abcde);
      }
   }
   else
   {
      printf("psrld_2 ... failed\n");
   }

   return;
}

static void psrld_3(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .ud = { 0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psrld %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00123456 && result0.ud[1] == 0x089abcde && result0.ud[2] == 0x00123456 && result0.ud[3] == 0x089abcde )
      {
         printf("psrld_3 ... ok\n");
      }
      else
      {
         printf("psrld_3 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00123456);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x089abcde);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0x00123456);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x089abcde);
      }
   }
   else
   {
      printf("psrld_3 ... failed\n");
   }

   return;
}

static void psrldq_1(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $0, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x8899aabbccddeeffULL && result0.uq[1] == 0x9911223344556677ULL )
      {
         printf("psrldq_1 ... ok\n");
      }
      else
      {
         printf("psrldq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x8899aabbccddeeffULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x9911223344556677ULL);
      }
   }
   else
   {
      printf("psrldq_1 ... failed\n");
   }

   return;
}

static void psrldq_2(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $1, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x778899aabbccddeeULL && result0.uq[1] == 0x0099112233445566ULL )
      {
         printf("psrldq_2 ... ok\n");
      }
      else
      {
         printf("psrldq_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x778899aabbccddeeULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0099112233445566ULL);
      }
   }
   else
   {
      printf("psrldq_2 ... failed\n");
   }

   return;
}

static void psrldq_3(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x66778899aabbccddULL && result0.uq[1] == 0x0000991122334455ULL )
      {
         printf("psrldq_3 ... ok\n");
      }
      else
      {
         printf("psrldq_3 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x66778899aabbccddULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000991122334455ULL);
      }
   }
   else
   {
      printf("psrldq_3 ... failed\n");
   }

   return;
}

static void psrldq_4(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $3, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x5566778899aabbccULL && result0.uq[1] == 0x0000009911223344ULL )
      {
         printf("psrldq_4 ... ok\n");
      }
      else
      {
         printf("psrldq_4 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x5566778899aabbccULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000009911223344ULL);
      }
   }
   else
   {
      printf("psrldq_4 ... failed\n");
   }

   return;
}

static void psrldq_5(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $4, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x445566778899aabbULL && result0.uq[1] == 0x0000000099112233ULL )
      {
         printf("psrldq_5 ... ok\n");
      }
      else
      {
         printf("psrldq_5 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x445566778899aabbULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000099112233ULL);
      }
   }
   else
   {
      printf("psrldq_5 ... failed\n");
   }

   return;
}

static void psrldq_6(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $5, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x33445566778899aaULL && result0.uq[1] == 0x0000000000991122ULL )
      {
         printf("psrldq_6 ... ok\n");
      }
      else
      {
         printf("psrldq_6 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x33445566778899aaULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000991122ULL);
      }
   }
   else
   {
      printf("psrldq_6 ... failed\n");
   }

   return;
}

static void psrldq_7(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $6, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x2233445566778899ULL && result0.uq[1] == 0x0000000000009911ULL )
      {
         printf("psrldq_7 ... ok\n");
      }
      else
      {
         printf("psrldq_7 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x2233445566778899ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000009911ULL);
      }
   }
   else
   {
      printf("psrldq_7 ... failed\n");
   }

   return;
}

static void psrldq_8(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $7, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x1122334455667788ULL && result0.uq[1] == 0x0000000000000099ULL )
      {
         printf("psrldq_8 ... ok\n");
      }
      else
      {
         printf("psrldq_8 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x1122334455667788ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000099ULL);
      }
   }
   else
   {
      printf("psrldq_8 ... failed\n");
   }

   return;
}

static void psrldq_9(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $8, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x9911223344556677ULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("psrldq_9 ... ok\n");
      }
      else
      {
         printf("psrldq_9 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x9911223344556677ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("psrldq_9 ... failed\n");
   }

   return;
}

static void psrldq_10(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $9, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0099112233445566ULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("psrldq_10 ... ok\n");
      }
      else
      {
         printf("psrldq_10 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0099112233445566ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("psrldq_10 ... failed\n");
   }

   return;
}

static void psrldq_11(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $10, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000991122334455ULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("psrldq_11 ... ok\n");
      }
      else
      {
         printf("psrldq_11 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000991122334455ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("psrldq_11 ... failed\n");
   }

   return;
}

static void psrldq_12(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $11, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000009911223344ULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("psrldq_12 ... ok\n");
      }
      else
      {
         printf("psrldq_12 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000009911223344ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("psrldq_12 ... failed\n");
   }

   return;
}

static void psrldq_13(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $12, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000099112233ULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("psrldq_13 ... ok\n");
      }
      else
      {
         printf("psrldq_13 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000099112233ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("psrldq_13 ... failed\n");
   }

   return;
}

static void psrldq_14(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $13, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000991122ULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("psrldq_14 ... ok\n");
      }
      else
      {
         printf("psrldq_14 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000991122ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("psrldq_14 ... failed\n");
   }

   return;
}

static void psrldq_15(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $14, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000009911ULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("psrldq_15 ... ok\n");
      }
      else
      {
         printf("psrldq_15 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000009911ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("psrldq_15 ... failed\n");
   }

   return;
}

static void psrldq_16(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $15, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000099ULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("psrldq_16 ... ok\n");
      }
      else
      {
         printf("psrldq_16 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000099ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("psrldq_16 ... failed\n");
   }

   return;
}

static void psrldq_17(void)
{
   reg128_t arg1 = { .uq = { 0x8899aabbccddeeffULL, 0x9911223344556677ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrldq $16, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x0000000000000000ULL && result0.uq[1] == 0x0000000000000000ULL )
      {
         printf("psrldq_17 ... ok\n");
      }
      else
      {
         printf("psrldq_17 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0000000000000000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0000000000000000ULL);
      }
   }
   else
   {
      printf("psrldq_17 ... failed\n");
   }

   return;
}

static void psrlq_1(void)
{
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0x0123456789abcdefULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrlq $4, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uq[0] == 0x00123456789abcdeULL && result0.uq[1] == 0x00123456789abcdeULL )
      {
         printf("psrlq_1 ... ok\n");
      }
      else
      {
         printf("psrlq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x00123456789abcdeULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x00123456789abcdeULL);
      }
   }
   else
   {
      printf("psrlq_1 ... failed\n");
   }

   return;
}

static void psrlq_2(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0x0123456789abcdefULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psrlq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x00123456789abcdeULL && result0.uq[1] == 0x00123456789abcdeULL )
      {
         printf("psrlq_2 ... ok\n");
      }
      else
      {
         printf("psrlq_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x00123456789abcdeULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x00123456789abcdeULL);
      }
   }
   else
   {
      printf("psrlq_2 ... failed\n");
   }

   return;
}

static void psrlq_3(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0x0123456789abcdefULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psrlq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x00123456789abcdeULL && result0.uq[1] == 0x00123456789abcdeULL )
      {
         printf("psrlq_3 ... ok\n");
      }
      else
      {
         printf("psrlq_3 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x00123456789abcdeULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x00123456789abcdeULL);
      }
   }
   else
   {
      printf("psrlq_3 ... failed\n");
   }

   return;
}

static void psrlw_1(void)
{
   reg128_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef, 0x0123, 0x4567, 0x89ab, 0xcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "psrlw $4, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (result0.uw[0] == 0x0012 && result0.uw[1] == 0x0456 && result0.uw[2] == 0x089a && result0.uw[3] == 0x0cde && result0.uw[4] == 0x0012 && result0.uw[5] == 0x0456 && result0.uw[6] == 0x089a && result0.uw[7] == 0x0cde )
      {
         printf("psrlw_1 ... ok\n");
      }
      else
      {
         printf("psrlw_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x0012);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x0456);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0x089a);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0x0cde);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x0012);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x0456);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0x089a);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0x0cde);
      }
   }
   else
   {
      printf("psrlw_1 ... failed\n");
   }

   return;
}

static void psrlw_2(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef, 0x0123, 0x4567, 0x89ab, 0xcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psrlw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0x0012 && result0.uw[1] == 0x0456 && result0.uw[2] == 0x089a && result0.uw[3] == 0x0cde && result0.uw[4] == 0x0012 && result0.uw[5] == 0x0456 && result0.uw[6] == 0x089a && result0.uw[7] == 0x0cde )
      {
         printf("psrlw_2 ... ok\n");
      }
      else
      {
         printf("psrlw_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x0012);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x0456);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0x089a);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0x0cde);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x0012);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x0456);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0x089a);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0x0cde);
      }
   }
   else
   {
      printf("psrlw_2 ... failed\n");
   }

   return;
}

static void psrlw_3(void)
{
   reg128_t arg0 = { .uq = { 4ULL, 0ULL } };
   reg128_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef, 0x0123, 0x4567, 0x89ab, 0xcdef } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psrlw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 0x0012 && result0.uw[1] == 0x0456 && result0.uw[2] == 0x089a && result0.uw[3] == 0x0cde && result0.uw[4] == 0x0012 && result0.uw[5] == 0x0456 && result0.uw[6] == 0x089a && result0.uw[7] == 0x0cde )
      {
         printf("psrlw_3 ... ok\n");
      }
      else
      {
         printf("psrlw_3 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 0x0012);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 0x0456);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0x089a);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0x0cde);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 0x0012);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 0x0456);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0x089a);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0x0cde);
      }
   }
   else
   {
      printf("psrlw_3 ... failed\n");
   }

   return;
}

static void psubb_1(void)
{
   reg128_t arg0 = { .sb = { 8, 7, 6, 5, 4, 3, 2, 1, 8, 7, 6, 5, 4, 3, 2, 1 } };
   reg128_t arg1 = { .sb = { 12, 34, 56, 78, 21, 43, 65, 87, 12, 34, 56, 78, 21, 43, 65, 87 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psubb %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sb[0] == 4 && result0.sb[1] == 27 && result0.sb[2] == 50 && result0.sb[3] == 73 && result0.sb[4] == 17 && result0.sb[5] == 40 && result0.sb[6] == 63 && result0.sb[7] == 86 && result0.sb[8] == 4 && result0.sb[9] == 27 && result0.sb[10] == 50 && result0.sb[11] == 73 && result0.sb[12] == 17 && result0.sb[13] == 40 && result0.sb[14] == 63 && result0.sb[15] == 86 )
      {
         printf("psubb_1 ... ok\n");
      }
      else
      {
         printf("psubb_1 ... not ok\n");
         printf("  result0.sb[0] = %d (expected %d)\n", result0.sb[0], 4);
         printf("  result0.sb[1] = %d (expected %d)\n", result0.sb[1], 27);
         printf("  result0.sb[2] = %d (expected %d)\n", result0.sb[2], 50);
         printf("  result0.sb[3] = %d (expected %d)\n", result0.sb[3], 73);
         printf("  result0.sb[4] = %d (expected %d)\n", result0.sb[4], 17);
         printf("  result0.sb[5] = %d (expected %d)\n", result0.sb[5], 40);
         printf("  result0.sb[6] = %d (expected %d)\n", result0.sb[6], 63);
         printf("  result0.sb[7] = %d (expected %d)\n", result0.sb[7], 86);
         printf("  result0.sb[8] = %d (expected %d)\n", result0.sb[8], 4);
         printf("  result0.sb[9] = %d (expected %d)\n", result0.sb[9], 27);
         printf("  result0.sb[10] = %d (expected %d)\n", result0.sb[10], 50);
         printf("  result0.sb[11] = %d (expected %d)\n", result0.sb[11], 73);
         printf("  result0.sb[12] = %d (expected %d)\n", result0.sb[12], 17);
         printf("  result0.sb[13] = %d (expected %d)\n", result0.sb[13], 40);
         printf("  result0.sb[14] = %d (expected %d)\n", result0.sb[14], 63);
         printf("  result0.sb[15] = %d (expected %d)\n", result0.sb[15], 86);
      }
   }
   else
   {
      printf("psubb_1 ... failed\n");
   }

   return;
}

static void psubb_2(void)
{
   reg128_t arg0 = { .sb = { 8, 7, 6, 5, 4, 3, 2, 1, 8, 7, 6, 5, 4, 3, 2, 1 } };
   reg128_t arg1 = { .sb = { 12, 34, 56, 78, 21, 43, 65, 87, 12, 34, 56, 78, 21, 43, 65, 87 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psubb %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sb[0] == 4 && result0.sb[1] == 27 && result0.sb[2] == 50 && result0.sb[3] == 73 && result0.sb[4] == 17 && result0.sb[5] == 40 && result0.sb[6] == 63 && result0.sb[7] == 86 && result0.sb[8] == 4 && result0.sb[9] == 27 && result0.sb[10] == 50 && result0.sb[11] == 73 && result0.sb[12] == 17 && result0.sb[13] == 40 && result0.sb[14] == 63 && result0.sb[15] == 86 )
      {
         printf("psubb_2 ... ok\n");
      }
      else
      {
         printf("psubb_2 ... not ok\n");
         printf("  result0.sb[0] = %d (expected %d)\n", result0.sb[0], 4);
         printf("  result0.sb[1] = %d (expected %d)\n", result0.sb[1], 27);
         printf("  result0.sb[2] = %d (expected %d)\n", result0.sb[2], 50);
         printf("  result0.sb[3] = %d (expected %d)\n", result0.sb[3], 73);
         printf("  result0.sb[4] = %d (expected %d)\n", result0.sb[4], 17);
         printf("  result0.sb[5] = %d (expected %d)\n", result0.sb[5], 40);
         printf("  result0.sb[6] = %d (expected %d)\n", result0.sb[6], 63);
         printf("  result0.sb[7] = %d (expected %d)\n", result0.sb[7], 86);
         printf("  result0.sb[8] = %d (expected %d)\n", result0.sb[8], 4);
         printf("  result0.sb[9] = %d (expected %d)\n", result0.sb[9], 27);
         printf("  result0.sb[10] = %d (expected %d)\n", result0.sb[10], 50);
         printf("  result0.sb[11] = %d (expected %d)\n", result0.sb[11], 73);
         printf("  result0.sb[12] = %d (expected %d)\n", result0.sb[12], 17);
         printf("  result0.sb[13] = %d (expected %d)\n", result0.sb[13], 40);
         printf("  result0.sb[14] = %d (expected %d)\n", result0.sb[14], 63);
         printf("  result0.sb[15] = %d (expected %d)\n", result0.sb[15], 86);
      }
   }
   else
   {
      printf("psubb_2 ... failed\n");
   }

   return;
}

static void psubd_1(void)
{
   reg128_t arg0 = { .sd = { 8765, 4321, 8765, 4321 } };
   reg128_t arg1 = { .sd = { 12345678, 87654321, 12345678, 87654321 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psubd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 12336913 && result0.sd[1] == 87650000 && result0.sd[2] == 12336913 && result0.sd[3] == 87650000 )
      {
         printf("psubd_1 ... ok\n");
      }
      else
      {
         printf("psubd_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12336913);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 87650000);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 12336913);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 87650000);
      }
   }
   else
   {
      printf("psubd_1 ... failed\n");
   }

   return;
}

static void psubd_2(void)
{
   reg128_t arg0 = { .sd = { 8765, 4321, 8765, 4321 } };
   reg128_t arg1 = { .sd = { 12345678, 87654321, 12345678, 87654321 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psubd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sd[0] == 12336913 && result0.sd[1] == 87650000 && result0.sd[2] == 12336913 && result0.sd[3] == 87650000 )
      {
         printf("psubd_2 ... ok\n");
      }
      else
      {
         printf("psubd_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12336913);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 87650000);
         printf("  result0.sd[2] = %d (expected %d)\n", result0.sd[2], 12336913);
         printf("  result0.sd[3] = %d (expected %d)\n", result0.sd[3], 87650000);
      }
   }
   else
   {
      printf("psubd_2 ... failed\n");
   }

   return;
}

static void psubq_1(void)
{
   reg64_t arg0 = { .sq = { 11111111LL } };
   reg64_t arg1 = { .sq = { 33333333LL } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movq %1, %%mm6\n"
         "movq %2, %%mm7\n"
         "psubq %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sq[0] == 22222222LL )
      {
         printf("psubq_1 ... ok\n");
      }
      else
      {
         printf("psubq_1 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 22222222LL);
      }
   }
   else
   {
      printf("psubq_1 ... failed\n");
   }

   return;
}

static void psubq_2(void)
{
   reg64_t arg0 = { .sq = { 11111111LL } };
   reg64_t arg1 = { .sq = { 33333333LL } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movq %2, %%mm7\n"
         "psubq %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sq[0] == 22222222LL )
      {
         printf("psubq_2 ... ok\n");
      }
      else
      {
         printf("psubq_2 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 22222222LL);
      }
   }
   else
   {
      printf("psubq_2 ... failed\n");
   }

   return;
}

static void psubq_3(void)
{
   reg128_t arg0 = { .sq = { 11111111LL, 22222222LL } };
   reg128_t arg1 = { .sq = { 55555555LL, 33333333LL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psubq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sq[0] == 44444444LL && result0.sq[1] == 11111111LL )
      {
         printf("psubq_3 ... ok\n");
      }
      else
      {
         printf("psubq_3 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 44444444LL);
         printf("  result0.sq[1] = %lld (expected %lld)\n", result0.sq[1], 11111111LL);
      }
   }
   else
   {
      printf("psubq_3 ... failed\n");
   }

   return;
}

static void psubq_4(void)
{
   reg128_t arg0 = { .sq = { 11111111LL, 22222222LL } };
   reg128_t arg1 = { .sq = { 55555555LL, 33333333LL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psubq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sq[0] == 44444444LL && result0.sq[1] == 11111111LL )
      {
         printf("psubq_4 ... ok\n");
      }
      else
      {
         printf("psubq_4 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 44444444LL);
         printf("  result0.sq[1] = %lld (expected %lld)\n", result0.sq[1], 11111111LL);
      }
   }
   else
   {
      printf("psubq_4 ... failed\n");
   }

   return;
}

static void psubsb_1(void)
{
   reg128_t arg0 = { .sb = { -50, 50, -40, 40, -30, 30, -20, 20, -50, 50, -40, 40, -30, 30, -20, 20 } };
   reg128_t arg1 = { .sb = { 25, -25, 50, -50, 100, -100, 125, -125, 25, -25, 50, -50, 100, -100, 125, -125 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psubsb %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sb[0] == 75 && result0.sb[1] == -75 && result0.sb[2] == 90 && result0.sb[3] == -90 && result0.sb[4] == 127 && result0.sb[5] == -128 && result0.sb[6] == 127 && result0.sb[7] == -128 && result0.sb[8] == 75 && result0.sb[9] == -75 && result0.sb[10] == 90 && result0.sb[11] == -90 && result0.sb[12] == 127 && result0.sb[13] == -128 && result0.sb[14] == 127 && result0.sb[15] == -128 )
      {
         printf("psubsb_1 ... ok\n");
      }
      else
      {
         printf("psubsb_1 ... not ok\n");
         printf("  result0.sb[0] = %d (expected %d)\n", result0.sb[0], 75);
         printf("  result0.sb[1] = %d (expected %d)\n", result0.sb[1], -75);
         printf("  result0.sb[2] = %d (expected %d)\n", result0.sb[2], 90);
         printf("  result0.sb[3] = %d (expected %d)\n", result0.sb[3], -90);
         printf("  result0.sb[4] = %d (expected %d)\n", result0.sb[4], 127);
         printf("  result0.sb[5] = %d (expected %d)\n", result0.sb[5], -128);
         printf("  result0.sb[6] = %d (expected %d)\n", result0.sb[6], 127);
         printf("  result0.sb[7] = %d (expected %d)\n", result0.sb[7], -128);
         printf("  result0.sb[8] = %d (expected %d)\n", result0.sb[8], 75);
         printf("  result0.sb[9] = %d (expected %d)\n", result0.sb[9], -75);
         printf("  result0.sb[10] = %d (expected %d)\n", result0.sb[10], 90);
         printf("  result0.sb[11] = %d (expected %d)\n", result0.sb[11], -90);
         printf("  result0.sb[12] = %d (expected %d)\n", result0.sb[12], 127);
         printf("  result0.sb[13] = %d (expected %d)\n", result0.sb[13], -128);
         printf("  result0.sb[14] = %d (expected %d)\n", result0.sb[14], 127);
         printf("  result0.sb[15] = %d (expected %d)\n", result0.sb[15], -128);
      }
   }
   else
   {
      printf("psubsb_1 ... failed\n");
   }

   return;
}

static void psubsb_2(void)
{
   reg128_t arg0 = { .sb = { -50, 50, -40, 40, -30, 30, -20, 20, -50, 50, -40, 40, -30, 30, -20, 20 } };
   reg128_t arg1 = { .sb = { 25, -25, 50, -50, 100, -100, 125, -125, 25, -25, 50, -50, 100, -100, 125, -125 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psubsb %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sb[0] == 75 && result0.sb[1] == -75 && result0.sb[2] == 90 && result0.sb[3] == -90 && result0.sb[4] == 127 && result0.sb[5] == -128 && result0.sb[6] == 127 && result0.sb[7] == -128 && result0.sb[8] == 75 && result0.sb[9] == -75 && result0.sb[10] == 90 && result0.sb[11] == -90 && result0.sb[12] == 127 && result0.sb[13] == -128 && result0.sb[14] == 127 && result0.sb[15] == -128 )
      {
         printf("psubsb_2 ... ok\n");
      }
      else
      {
         printf("psubsb_2 ... not ok\n");
         printf("  result0.sb[0] = %d (expected %d)\n", result0.sb[0], 75);
         printf("  result0.sb[1] = %d (expected %d)\n", result0.sb[1], -75);
         printf("  result0.sb[2] = %d (expected %d)\n", result0.sb[2], 90);
         printf("  result0.sb[3] = %d (expected %d)\n", result0.sb[3], -90);
         printf("  result0.sb[4] = %d (expected %d)\n", result0.sb[4], 127);
         printf("  result0.sb[5] = %d (expected %d)\n", result0.sb[5], -128);
         printf("  result0.sb[6] = %d (expected %d)\n", result0.sb[6], 127);
         printf("  result0.sb[7] = %d (expected %d)\n", result0.sb[7], -128);
         printf("  result0.sb[8] = %d (expected %d)\n", result0.sb[8], 75);
         printf("  result0.sb[9] = %d (expected %d)\n", result0.sb[9], -75);
         printf("  result0.sb[10] = %d (expected %d)\n", result0.sb[10], 90);
         printf("  result0.sb[11] = %d (expected %d)\n", result0.sb[11], -90);
         printf("  result0.sb[12] = %d (expected %d)\n", result0.sb[12], 127);
         printf("  result0.sb[13] = %d (expected %d)\n", result0.sb[13], -128);
         printf("  result0.sb[14] = %d (expected %d)\n", result0.sb[14], 127);
         printf("  result0.sb[15] = %d (expected %d)\n", result0.sb[15], -128);
      }
   }
   else
   {
      printf("psubsb_2 ... failed\n");
   }

   return;
}

static void psubsw_1(void)
{
   reg128_t arg0 = { .sw = { -32145, 32145, 12345, -12345, -32145, 32145, 12345, -12345 } };
   reg128_t arg1 = { .sw = { 12345, -12345, 32145, -32145, 12345, -12345, 32145, -32145 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psubsw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 32767 && result0.sw[1] == -32768 && result0.sw[2] == 19800 && result0.sw[3] == -19800 && result0.sw[4] == 32767 && result0.sw[5] == -32768 && result0.sw[6] == 19800 && result0.sw[7] == -19800 )
      {
         printf("psubsw_1 ... ok\n");
      }
      else
      {
         printf("psubsw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 32767);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], -32768);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 19800);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], -19800);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 32767);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], -32768);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 19800);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], -19800);
      }
   }
   else
   {
      printf("psubsw_1 ... failed\n");
   }

   return;
}

static void psubsw_2(void)
{
   reg128_t arg0 = { .sw = { -32145, 32145, 12345, -12345, -32145, 32145, 12345, -12345 } };
   reg128_t arg1 = { .sw = { 12345, -12345, 32145, -32145, 12345, -12345, 32145, -32145 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psubsw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 32767 && result0.sw[1] == -32768 && result0.sw[2] == 19800 && result0.sw[3] == -19800 && result0.sw[4] == 32767 && result0.sw[5] == -32768 && result0.sw[6] == 19800 && result0.sw[7] == -19800 )
      {
         printf("psubsw_2 ... ok\n");
      }
      else
      {
         printf("psubsw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 32767);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], -32768);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 19800);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], -19800);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 32767);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], -32768);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 19800);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], -19800);
      }
   }
   else
   {
      printf("psubsw_2 ... failed\n");
   }

   return;
}

static void psubusb_1(void)
{
   reg128_t arg0 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U, 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
   reg128_t arg1 = { .ub = { 88U, 77U, 66U, 55U, 44U, 33U, 22U, 11U, 88U, 77U, 66U, 55U, 44U, 33U, 22U, 11U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psubusb %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 77U && result0.ub[1] == 55U && result0.ub[2] == 33U && result0.ub[3] == 11U && result0.ub[4] == 0U && result0.ub[5] == 0U && result0.ub[6] == 0U && result0.ub[7] == 0U && result0.ub[8] == 77U && result0.ub[9] == 55U && result0.ub[10] == 33U && result0.ub[11] == 11U && result0.ub[12] == 0U && result0.ub[13] == 0U && result0.ub[14] == 0U && result0.ub[15] == 0U )
      {
         printf("psubusb_1 ... ok\n");
      }
      else
      {
         printf("psubusb_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 77U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 55U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 33U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 11U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 0U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 0U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 0U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 0U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 77U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 55U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 33U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 11U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 0U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 0U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 0U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 0U);
      }
   }
   else
   {
      printf("psubusb_1 ... failed\n");
   }

   return;
}

static void psubusb_2(void)
{
   reg128_t arg0 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U, 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
   reg128_t arg1 = { .ub = { 88U, 77U, 66U, 55U, 44U, 33U, 22U, 11U, 88U, 77U, 66U, 55U, 44U, 33U, 22U, 11U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psubusb %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 77U && result0.ub[1] == 55U && result0.ub[2] == 33U && result0.ub[3] == 11U && result0.ub[4] == 0U && result0.ub[5] == 0U && result0.ub[6] == 0U && result0.ub[7] == 0U && result0.ub[8] == 77U && result0.ub[9] == 55U && result0.ub[10] == 33U && result0.ub[11] == 11U && result0.ub[12] == 0U && result0.ub[13] == 0U && result0.ub[14] == 0U && result0.ub[15] == 0U )
      {
         printf("psubusb_2 ... ok\n");
      }
      else
      {
         printf("psubusb_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 77U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 55U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 33U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 11U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 0U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 0U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 0U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 0U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 77U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 55U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 33U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 11U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 0U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 0U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 0U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 0U);
      }
   }
   else
   {
      printf("psubusb_2 ... failed\n");
   }

   return;
}

static void psubusw_1(void)
{
   reg128_t arg0 = { .uw = { 1122, 3344, 5566, 7788, 1122, 3344, 5566, 7788 } };
   reg128_t arg1 = { .uw = { 8877, 6655, 4433, 2211, 8877, 6655, 4433, 2211 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psubusw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 7755 && result0.uw[1] == 3311 && result0.uw[2] == 0 && result0.uw[3] == 0 && result0.uw[4] == 7755 && result0.uw[5] == 3311 && result0.uw[6] == 0 && result0.uw[7] == 0 )
      {
         printf("psubusw_1 ... ok\n");
      }
      else
      {
         printf("psubusw_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 7755);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 3311);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 7755);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 3311);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0);
      }
   }
   else
   {
      printf("psubusw_1 ... failed\n");
   }

   return;
}

static void psubusw_2(void)
{
   reg128_t arg0 = { .uw = { 1122, 3344, 5566, 7788, 1122, 3344, 5566, 7788 } };
   reg128_t arg1 = { .uw = { 8877, 6655, 4433, 2211, 8877, 6655, 4433, 2211 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psubusw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 7755 && result0.uw[1] == 3311 && result0.uw[2] == 0 && result0.uw[3] == 0 && result0.uw[4] == 7755 && result0.uw[5] == 3311 && result0.uw[6] == 0 && result0.uw[7] == 0 )
      {
         printf("psubusw_2 ... ok\n");
      }
      else
      {
         printf("psubusw_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 7755);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 3311);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 0);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 0);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 7755);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 3311);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 0);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 0);
      }
   }
   else
   {
      printf("psubusw_2 ... failed\n");
   }

   return;
}

static void psubw_1(void)
{
   reg128_t arg0 = { .sw = { 87, 65, 43, 21, 87, 65, 43, 21 } };
   reg128_t arg1 = { .sw = { 1234, 5678, 4321, 8765, 1234, 5678, 4321, 8765 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "psubw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 1147 && result0.sw[1] == 5613 && result0.sw[2] == 4278 && result0.sw[3] == 8744 && result0.sw[4] == 1147 && result0.sw[5] == 5613 && result0.sw[6] == 4278 && result0.sw[7] == 8744 )
      {
         printf("psubw_1 ... ok\n");
      }
      else
      {
         printf("psubw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1147);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 5613);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 4278);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 8744);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 1147);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], 5613);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 4278);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], 8744);
      }
   }
   else
   {
      printf("psubw_1 ... failed\n");
   }

   return;
}

static void psubw_2(void)
{
   reg128_t arg0 = { .sw = { 87, 65, 43, 21, 87, 65, 43, 21 } };
   reg128_t arg1 = { .sw = { 1234, 5678, 4321, 8765, 1234, 5678, 4321, 8765 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "psubw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.sw[0] == 1147 && result0.sw[1] == 5613 && result0.sw[2] == 4278 && result0.sw[3] == 8744 && result0.sw[4] == 1147 && result0.sw[5] == 5613 && result0.sw[6] == 4278 && result0.sw[7] == 8744 )
      {
         printf("psubw_2 ... ok\n");
      }
      else
      {
         printf("psubw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1147);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 5613);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 4278);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 8744);
         printf("  result0.sw[4] = %d (expected %d)\n", result0.sw[4], 1147);
         printf("  result0.sw[5] = %d (expected %d)\n", result0.sw[5], 5613);
         printf("  result0.sw[6] = %d (expected %d)\n", result0.sw[6], 4278);
         printf("  result0.sw[7] = %d (expected %d)\n", result0.sw[7], 8744);
      }
   }
   else
   {
      printf("psubw_2 ... failed\n");
   }

   return;
}

static void punpckhbw_1(void)
{
   reg128_t arg0 = { .ub = { 12U, 34U, 56U, 78U, 21U, 43U, 65U, 87U, 78U, 56U, 34U, 12U, 87U, 65U, 43U, 21U } };
   reg128_t arg1 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U, 88U, 77U, 66U, 55U, 44U, 33U, 22U, 11U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "punpckhbw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 88U && result0.ub[1] == 78U && result0.ub[2] == 77U && result0.ub[3] == 56U && result0.ub[4] == 66U && result0.ub[5] == 34U && result0.ub[6] == 55U && result0.ub[7] == 12U && result0.ub[8] == 44U && result0.ub[9] == 87U && result0.ub[10] == 33U && result0.ub[11] == 65U && result0.ub[12] == 22U && result0.ub[13] == 43U && result0.ub[14] == 11U && result0.ub[15] == 21U )
      {
         printf("punpckhbw_1 ... ok\n");
      }
      else
      {
         printf("punpckhbw_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 88U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 78U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 77U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 56U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 66U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 34U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 55U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 12U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 44U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 87U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 33U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 65U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 22U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 43U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 11U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 21U);
      }
   }
   else
   {
      printf("punpckhbw_1 ... failed\n");
   }

   return;
}

static void punpckhbw_2(void)
{
   reg128_t arg0 = { .ub = { 12U, 34U, 56U, 78U, 21U, 43U, 65U, 87U, 78U, 56U, 34U, 12U, 87U, 65U, 43U, 21U } };
   reg128_t arg1 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U, 88U, 77U, 66U, 55U, 44U, 33U, 22U, 11U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "punpckhbw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 88U && result0.ub[1] == 78U && result0.ub[2] == 77U && result0.ub[3] == 56U && result0.ub[4] == 66U && result0.ub[5] == 34U && result0.ub[6] == 55U && result0.ub[7] == 12U && result0.ub[8] == 44U && result0.ub[9] == 87U && result0.ub[10] == 33U && result0.ub[11] == 65U && result0.ub[12] == 22U && result0.ub[13] == 43U && result0.ub[14] == 11U && result0.ub[15] == 21U )
      {
         printf("punpckhbw_2 ... ok\n");
      }
      else
      {
         printf("punpckhbw_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 88U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 78U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 77U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 56U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 66U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 34U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 55U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 12U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 44U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 87U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 33U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 65U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 22U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 43U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 11U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 21U);
      }
   }
   else
   {
      printf("punpckhbw_2 ... failed\n");
   }

   return;
}

static void punpckhdq_1(void)
{
   reg128_t arg0 = { .ud = { 12345678, 21436587, 78563412, 87654321 } };
   reg128_t arg1 = { .ud = { 11223344, 55667788, 88776655, 44332211 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "punpckhdq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 88776655 && result0.ud[1] == 78563412 && result0.ud[2] == 44332211 && result0.ud[3] == 87654321 )
      {
         printf("punpckhdq_1 ... ok\n");
      }
      else
      {
         printf("punpckhdq_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 88776655);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 78563412);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 44332211);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 87654321);
      }
   }
   else
   {
      printf("punpckhdq_1 ... failed\n");
   }

   return;
}

static void punpckhdq_2(void)
{
   reg128_t arg0 = { .ud = { 12345678, 21436587, 78563412, 87654321 } };
   reg128_t arg1 = { .ud = { 11223344, 55667788, 88776655, 44332211 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "punpckhdq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 88776655 && result0.ud[1] == 78563412 && result0.ud[2] == 44332211 && result0.ud[3] == 87654321 )
      {
         printf("punpckhdq_2 ... ok\n");
      }
      else
      {
         printf("punpckhdq_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 88776655);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 78563412);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 44332211);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 87654321);
      }
   }
   else
   {
      printf("punpckhdq_2 ... failed\n");
   }

   return;
}

static void punpckhqdq_1(void)
{
   reg128_t arg0 = { .uq = { 1234567821436587ULL, 7856341287654321ULL } };
   reg128_t arg1 = { .uq = { 1122334455667788ULL, 8877665544332211ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "punpckhqdq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 8877665544332211ULL && result0.uq[1] == 7856341287654321ULL )
      {
         printf("punpckhqdq_1 ... ok\n");
      }
      else
      {
         printf("punpckhqdq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 8877665544332211ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 7856341287654321ULL);
      }
   }
   else
   {
      printf("punpckhqdq_1 ... failed\n");
   }

   return;
}

static void punpckhqdq_2(void)
{
   reg128_t arg0 = { .uq = { 1234567821436587ULL, 7856341287654321ULL } };
   reg128_t arg1 = { .uq = { 1122334455667788ULL, 8877665544332211ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "punpckhqdq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 8877665544332211ULL && result0.uq[1] == 7856341287654321ULL )
      {
         printf("punpckhqdq_2 ... ok\n");
      }
      else
      {
         printf("punpckhqdq_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 8877665544332211ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 7856341287654321ULL);
      }
   }
   else
   {
      printf("punpckhqdq_2 ... failed\n");
   }

   return;
}

static void punpckhwd_1(void)
{
   reg128_t arg0 = { .uw = { 1234, 5678, 2143, 6587, 7856, 3412, 8765, 4321 } };
   reg128_t arg1 = { .uw = { 1122, 3344, 5566, 7788, 8877, 6655, 4433, 2211 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "punpckhwd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 8877 && result0.uw[1] == 7856 && result0.uw[2] == 6655 && result0.uw[3] == 3412 && result0.uw[4] == 4433 && result0.uw[5] == 8765 && result0.uw[6] == 2211 && result0.uw[7] == 4321 )
      {
         printf("punpckhwd_1 ... ok\n");
      }
      else
      {
         printf("punpckhwd_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 8877);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 7856);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 6655);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 3412);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 4433);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 8765);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 2211);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4321);
      }
   }
   else
   {
      printf("punpckhwd_1 ... failed\n");
   }

   return;
}

static void punpckhwd_2(void)
{
   reg128_t arg0 = { .uw = { 1234, 5678, 2143, 6587, 7856, 3412, 8765, 4321 } };
   reg128_t arg1 = { .uw = { 1122, 3344, 5566, 7788, 8877, 6655, 4433, 2211 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "punpckhwd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 8877 && result0.uw[1] == 7856 && result0.uw[2] == 6655 && result0.uw[3] == 3412 && result0.uw[4] == 4433 && result0.uw[5] == 8765 && result0.uw[6] == 2211 && result0.uw[7] == 4321 )
      {
         printf("punpckhwd_2 ... ok\n");
      }
      else
      {
         printf("punpckhwd_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 8877);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 7856);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 6655);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 3412);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 4433);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 8765);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 2211);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 4321);
      }
   }
   else
   {
      printf("punpckhwd_2 ... failed\n");
   }

   return;
}

static void punpcklbw_1(void)
{
   reg128_t arg0 = { .ub = { 12U, 34U, 56U, 78U, 21U, 43U, 65U, 87U, 78U, 56U, 34U, 12U, 87U, 65U, 43U, 21U } };
   reg128_t arg1 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U, 88U, 77U, 66U, 55U, 44U, 33U, 22U, 11U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "punpcklbw %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 11U && result0.ub[1] == 12U && result0.ub[2] == 22U && result0.ub[3] == 34U && result0.ub[4] == 33U && result0.ub[5] == 56U && result0.ub[6] == 44U && result0.ub[7] == 78U && result0.ub[8] == 55U && result0.ub[9] == 21U && result0.ub[10] == 66U && result0.ub[11] == 43U && result0.ub[12] == 77U && result0.ub[13] == 65U && result0.ub[14] == 88U && result0.ub[15] == 87U )
      {
         printf("punpcklbw_1 ... ok\n");
      }
      else
      {
         printf("punpcklbw_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 11U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 12U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 22U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 34U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 33U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 56U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 44U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 78U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 55U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 21U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 66U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 43U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 77U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 65U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 88U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 87U);
      }
   }
   else
   {
      printf("punpcklbw_1 ... failed\n");
   }

   return;
}

static void punpcklbw_2(void)
{
   reg128_t arg0 = { .ub = { 12U, 34U, 56U, 78U, 21U, 43U, 65U, 87U, 78U, 56U, 34U, 12U, 87U, 65U, 43U, 21U } };
   reg128_t arg1 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U, 88U, 77U, 66U, 55U, 44U, 33U, 22U, 11U } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "punpcklbw %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ub[0] == 11U && result0.ub[1] == 12U && result0.ub[2] == 22U && result0.ub[3] == 34U && result0.ub[4] == 33U && result0.ub[5] == 56U && result0.ub[6] == 44U && result0.ub[7] == 78U && result0.ub[8] == 55U && result0.ub[9] == 21U && result0.ub[10] == 66U && result0.ub[11] == 43U && result0.ub[12] == 77U && result0.ub[13] == 65U && result0.ub[14] == 88U && result0.ub[15] == 87U )
      {
         printf("punpcklbw_2 ... ok\n");
      }
      else
      {
         printf("punpcklbw_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 11U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 12U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 22U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 34U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 33U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 56U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 44U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 78U);
         printf("  result0.ub[8] = %u (expected %u)\n", result0.ub[8], 55U);
         printf("  result0.ub[9] = %u (expected %u)\n", result0.ub[9], 21U);
         printf("  result0.ub[10] = %u (expected %u)\n", result0.ub[10], 66U);
         printf("  result0.ub[11] = %u (expected %u)\n", result0.ub[11], 43U);
         printf("  result0.ub[12] = %u (expected %u)\n", result0.ub[12], 77U);
         printf("  result0.ub[13] = %u (expected %u)\n", result0.ub[13], 65U);
         printf("  result0.ub[14] = %u (expected %u)\n", result0.ub[14], 88U);
         printf("  result0.ub[15] = %u (expected %u)\n", result0.ub[15], 87U);
      }
   }
   else
   {
      printf("punpcklbw_2 ... failed\n");
   }

   return;
}

static void punpckldq_1(void)
{
   reg128_t arg0 = { .ud = { 12345678, 21436587, 78563412, 87654321 } };
   reg128_t arg1 = { .ud = { 11223344, 55667788, 88776655, 44332211 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "punpckldq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 11223344 && result0.ud[1] == 12345678 && result0.ud[2] == 55667788 && result0.ud[3] == 21436587 )
      {
         printf("punpckldq_1 ... ok\n");
      }
      else
      {
         printf("punpckldq_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 11223344);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 12345678);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 55667788);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 21436587);
      }
   }
   else
   {
      printf("punpckldq_1 ... failed\n");
   }

   return;
}

static void punpckldq_2(void)
{
   reg128_t arg0 = { .ud = { 12345678, 21436587, 78563412, 87654321 } };
   reg128_t arg1 = { .ud = { 11223344, 55667788, 88776655, 44332211 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "punpckldq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 11223344 && result0.ud[1] == 12345678 && result0.ud[2] == 55667788 && result0.ud[3] == 21436587 )
      {
         printf("punpckldq_2 ... ok\n");
      }
      else
      {
         printf("punpckldq_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 11223344);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 12345678);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 55667788);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 21436587);
      }
   }
   else
   {
      printf("punpckldq_2 ... failed\n");
   }

   return;
}

static void punpcklqdq_1(void)
{
   reg128_t arg0 = { .uq = { 1234567821436587ULL, 7856341287654321ULL } };
   reg128_t arg1 = { .uq = { 1122334455667788ULL, 8877665544332211ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "punpcklqdq %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 1122334455667788ULL && result0.uq[1] == 1234567821436587ULL )
      {
         printf("punpcklqdq_1 ... ok\n");
      }
      else
      {
         printf("punpcklqdq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 1122334455667788ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 1234567821436587ULL);
      }
   }
   else
   {
      printf("punpcklqdq_1 ... failed\n");
   }

   return;
}

static void punpcklqdq_2(void)
{
   reg128_t arg0 = { .uq = { 1234567821436587ULL, 7856341287654321ULL } };
   reg128_t arg1 = { .uq = { 1122334455667788ULL, 8877665544332211ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "punpcklqdq %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 1122334455667788ULL && result0.uq[1] == 1234567821436587ULL )
      {
         printf("punpcklqdq_2 ... ok\n");
      }
      else
      {
         printf("punpcklqdq_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 1122334455667788ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 1234567821436587ULL);
      }
   }
   else
   {
      printf("punpcklqdq_2 ... failed\n");
   }

   return;
}

static void punpcklwd_1(void)
{
   reg128_t arg0 = { .uw = { 1234, 5678, 2143, 6587, 7856, 3412, 8765, 4321 } };
   reg128_t arg1 = { .uw = { 1122, 3344, 5566, 7788, 8877, 6655, 4433, 2211 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "punpcklwd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 1122 && result0.uw[1] == 1234 && result0.uw[2] == 3344 && result0.uw[3] == 5678 && result0.uw[4] == 5566 && result0.uw[5] == 2143 && result0.uw[6] == 7788 && result0.uw[7] == 6587 )
      {
         printf("punpcklwd_1 ... ok\n");
      }
      else
      {
         printf("punpcklwd_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1122);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 1234);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 3344);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 5678);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 5566);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2143);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 7788);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 6587);
      }
   }
   else
   {
      printf("punpcklwd_1 ... failed\n");
   }

   return;
}

static void punpcklwd_2(void)
{
   reg128_t arg0 = { .uw = { 1234, 5678, 2143, 6587, 7856, 3412, 8765, 4321 } };
   reg128_t arg1 = { .uw = { 1122, 3344, 5566, 7788, 8877, 6655, 4433, 2211 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "punpcklwd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uw[0] == 1122 && result0.uw[1] == 1234 && result0.uw[2] == 3344 && result0.uw[3] == 5678 && result0.uw[4] == 5566 && result0.uw[5] == 2143 && result0.uw[6] == 7788 && result0.uw[7] == 6587 )
      {
         printf("punpcklwd_2 ... ok\n");
      }
      else
      {
         printf("punpcklwd_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1122);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 1234);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 3344);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 5678);
         printf("  result0.uw[4] = %u (expected %u)\n", result0.uw[4], 5566);
         printf("  result0.uw[5] = %u (expected %u)\n", result0.uw[5], 2143);
         printf("  result0.uw[6] = %u (expected %u)\n", result0.uw[6], 7788);
         printf("  result0.uw[7] = %u (expected %u)\n", result0.uw[7], 6587);
      }
   }
   else
   {
      printf("punpcklwd_2 ... failed\n");
   }

   return;
}

static void pxor_1(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "pxor %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfc9a30566503a9cfULL && result0.uq[1] == 0xfc9a30566503a9cfULL )
      {
         printf("pxor_1 ... ok\n");
      }
      else
      {
         printf("pxor_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc9a30566503a9cfULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfc9a30566503a9cfULL);
      }
   }
   else
   {
      printf("pxor_1 ... failed\n");
   }

   return;
}

static void pxor_2(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pxor %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfc9a30566503a9cfULL && result0.uq[1] == 0xfc9a30566503a9cfULL )
      {
         printf("pxor_2 ... ok\n");
      }
      else
      {
         printf("pxor_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc9a30566503a9cfULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfc9a30566503a9cfULL);
      }
   }
   else
   {
      printf("pxor_2 ... failed\n");
   }

   return;
}

static void shufpd_1(void)
{
   reg128_t arg1 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg2 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "shufpd $0x0, %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 1234.5678) && eq_double(result0.pd[1], 1234.5678) )
      {
         printf("shufpd_1 ... ok\n");
      }
      else
      {
         printf("shufpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234.5678);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 1234.5678);
      }
   }
   else
   {
      printf("shufpd_1 ... failed\n");
   }

   return;
}

static void shufpd_2(void)
{
   reg128_t arg1 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg2 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "shufpd $0x3, %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 8765.4321) && eq_double(result0.pd[1], 8765.4321) )
      {
         printf("shufpd_2 ... ok\n");
      }
      else
      {
         printf("shufpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8765.4321);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 8765.4321);
      }
   }
   else
   {
      printf("shufpd_2 ... failed\n");
   }

   return;
}

static void sqrtpd_1(void)
{
   reg128_t arg0 = { .pd = { 36.0, 49.0 } };
   reg128_t arg1 = { .pd = { 1.11, 2.22 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "sqrtpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 6.0) && eq_double(result0.pd[1], 7.0) )
      {
         printf("sqrtpd_1 ... ok\n");
      }
      else
      {
         printf("sqrtpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.0);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 7.0);
      }
   }
   else
   {
      printf("sqrtpd_1 ... failed\n");
   }

   return;
}

static void sqrtpd_2(void)
{
   reg128_t arg0 = { .pd = { 36.0, 49.0 } };
   reg128_t arg1 = { .pd = { 1.11, 2.22 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "sqrtpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 6.0) && eq_double(result0.pd[1], 7.0) )
      {
         printf("sqrtpd_2 ... ok\n");
      }
      else
      {
         printf("sqrtpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.0);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 7.0);
      }
   }
   else
   {
      printf("sqrtpd_2 ... failed\n");
   }

   return;
}

static void sqrtsd_1(void)
{
   reg128_t arg0 = { .pd = { 36.0, 5.55 } };
   reg128_t arg1 = { .pd = { 1.11, 2.22 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "sqrtsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 6.0) && eq_double(result0.pd[1], 2.22) )
      {
         printf("sqrtsd_1 ... ok\n");
      }
      else
      {
         printf("sqrtsd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.0);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 2.22);
      }
   }
   else
   {
      printf("sqrtsd_1 ... failed\n");
   }

   return;
}

static void sqrtsd_2(void)
{
   reg128_t arg0 = { .pd = { 36.0, 5.55 } };
   reg128_t arg1 = { .pd = { 1.11, 2.22 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "sqrtsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 6.0) && eq_double(result0.pd[1], 2.22) )
      {
         printf("sqrtsd_2 ... ok\n");
      }
      else
      {
         printf("sqrtsd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.0);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 2.22);
      }
   }
   else
   {
      printf("sqrtsd_2 ... failed\n");
   }

   return;
}

static void subpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 2222.0, 1111.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "subpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 987.4322) && eq_double(result0.pd[1], -7654.4321) )
      {
         printf("subpd_1 ... ok\n");
      }
      else
      {
         printf("subpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 987.4322);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], -7654.4321);
      }
   }
   else
   {
      printf("subpd_1 ... failed\n");
   }

   return;
}

static void subpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 2222.0, 1111.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "subpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 987.4322) && eq_double(result0.pd[1], -7654.4321) )
      {
         printf("subpd_2 ... ok\n");
      }
      else
      {
         printf("subpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 987.4322);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], -7654.4321);
      }
   }
   else
   {
      printf("subpd_2 ... failed\n");
   }

   return;
}

static void subsd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 2222.0, 1111.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "subsd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 987.4322) && eq_double(result0.pd[1], 1111.0) )
      {
         printf("subsd_1 ... ok\n");
      }
      else
      {
         printf("subsd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 987.4322);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 1111.0);
      }
   }
   else
   {
      printf("subsd_1 ... failed\n");
   }

   return;
}

static void subsd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 2222.0, 1111.0 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "subsd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 987.4322) && eq_double(result0.pd[1], 1111.0) )
      {
         printf("subsd_2 ... ok\n");
      }
      else
      {
         printf("subsd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 987.4322);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 1111.0);
      }
   }
   else
   {
      printf("subsd_2 ... failed\n");
   }

   return;
}

static void ucomisd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5679, 0.0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pushfq\n"
         "andq $0xfffffffffffff72a, (%%rsp)\n"
         "orq $0x00000000000008d5, (%%rsp)\n"
         "popfq\n"
         "ucomisd %%xmm12, %%xmm13\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "xmm13"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000000UL )
      {
         printf("ucomisd_1 ... ok\n");
      }
      else
      {
         printf("ucomisd_1 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("ucomisd_1 ... failed\n");
   }

   return;
}

static void ucomisd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5677, 0.0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pushfq\n"
         "andq $0xfffffffffffff72a, (%%rsp)\n"
         "orq $0x00000000000008d4, (%%rsp)\n"
         "popfq\n"
         "ucomisd %%xmm12, %%xmm13\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "xmm13"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000001UL )
      {
         printf("ucomisd_2 ... ok\n");
      }
      else
      {
         printf("ucomisd_2 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("ucomisd_2 ... failed\n");
   }

   return;
}

static void ucomisd_3(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 0.0 } };
   reg128_t arg1 = { .pd = { 1234.5678, 0.0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %1, %%xmm12\n"
         "movhps %2, %%xmm12\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "pushfq\n"
         "andq $0xfffffffffffff72a, (%%rsp)\n"
         "orq $0x0000000000000895, (%%rsp)\n"
         "popfq\n"
         "ucomisd %%xmm12, %%xmm13\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "xmm13"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000040UL )
      {
         printf("ucomisd_3 ... ok\n");
      }
      else
      {
         printf("ucomisd_3 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("ucomisd_3 ... failed\n");
   }

   return;
}

static void ucomisd_4(void)
{
   reg64_t arg0 = { .pd = { 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5679, 0.0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pushfq\n"
         "andq $0xfffffffffffff72a, (%%rsp)\n"
         "orq $0x00000000000008d5, (%%rsp)\n"
         "popfq\n"
         "ucomisd %1, %%xmm12\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "mm6", "xmm12"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000000UL )
      {
         printf("ucomisd_4 ... ok\n");
      }
      else
      {
         printf("ucomisd_4 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("ucomisd_4 ... failed\n");
   }

   return;
}

static void ucomisd_5(void)
{
   reg64_t arg0 = { .pd = { 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5677, 0.0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pushfq\n"
         "andq $0xfffffffffffff72a, (%%rsp)\n"
         "orq $0x00000000000008d4, (%%rsp)\n"
         "popfq\n"
         "ucomisd %1, %%xmm12\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "mm6", "xmm12"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000001UL )
      {
         printf("ucomisd_5 ... ok\n");
      }
      else
      {
         printf("ucomisd_5 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("ucomisd_5 ... failed\n");
   }

   return;
}

static void ucomisd_6(void)
{
   reg64_t arg0 = { .pd = { 1234.5678 } };
   reg128_t arg1 = { .pd = { 1234.5678, 0.0 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "pushfq\n"
         "andq $0xfffffffffffff72a, (%%rsp)\n"
         "orq $0x0000000000000895, (%%rsp)\n"
         "popfq\n"
         "ucomisd %1, %%xmm12\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "mm6", "xmm12"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000040UL )
      {
         printf("ucomisd_6 ... ok\n");
      }
      else
      {
         printf("ucomisd_6 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("ucomisd_6 ... failed\n");
   }

   return;
}

static void unpckhpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 1122.3344, 5566.7788 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "unpckhpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 5566.7788) && eq_double(result0.pd[1], 8765.4321) )
      {
         printf("unpckhpd_1 ... ok\n");
      }
      else
      {
         printf("unpckhpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 5566.7788);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 8765.4321);
      }
   }
   else
   {
      printf("unpckhpd_1 ... failed\n");
   }

   return;
}

static void unpckhpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 1122.3344, 5566.7788 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "unpckhpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 5566.7788) && eq_double(result0.pd[1], 8765.4321) )
      {
         printf("unpckhpd_2 ... ok\n");
      }
      else
      {
         printf("unpckhpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 5566.7788);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 8765.4321);
      }
   }
   else
   {
      printf("unpckhpd_2 ... failed\n");
   }

   return;
}

static void unpcklpd_1(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 1122.3344, 5566.7788 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "unpcklpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 1122.3344) && eq_double(result0.pd[1], 1234.5678) )
      {
         printf("unpcklpd_1 ... ok\n");
      }
      else
      {
         printf("unpcklpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1122.3344);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 1234.5678);
      }
   }
   else
   {
      printf("unpcklpd_1 ... failed\n");
   }

   return;
}

static void unpcklpd_2(void)
{
   reg128_t arg0 = { .pd = { 1234.5678, 8765.4321 } };
   reg128_t arg1 = { .pd = { 1122.3344, 5566.7788 } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "unpcklpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 1122.3344) && eq_double(result0.pd[1], 1234.5678) )
      {
         printf("unpcklpd_2 ... ok\n");
      }
      else
      {
         printf("unpcklpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1122.3344);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 1234.5678);
      }
   }
   else
   {
      printf("unpcklpd_2 ... failed\n");
   }

   return;
}

static void xorpd_1(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %2, %%xmm12\n"
         "movhps %3, %%xmm12\n"
         "movlps %4, %%xmm13\n"
         "movhps %5, %%xmm13\n"
         "xorpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfc9a30566503a9cfULL && result0.uq[1] == 0xfc9a30566503a9cfULL )
      {
         printf("xorpd_1 ... ok\n");
      }
      else
      {
         printf("xorpd_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc9a30566503a9cfULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfc9a30566503a9cfULL);
      }
   }
   else
   {
      printf("xorpd_1 ... failed\n");
   }

   return;
}

static void xorpd_2(void)
{
   reg128_t arg0 = { .uq = { 0xfdb97531eca86420ULL, 0x0123456789abcdefULL } };
   reg128_t arg1 = { .uq = { 0x0123456789abcdefULL, 0xfdb97531eca86420ULL } };
   reg128_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movlps %3, %%xmm13\n"
         "movhps %4, %%xmm13\n"
         "xorpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfc9a30566503a9cfULL && result0.uq[1] == 0xfc9a30566503a9cfULL )
      {
         printf("xorpd_2 ... ok\n");
      }
      else
      {
         printf("xorpd_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc9a30566503a9cfULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfc9a30566503a9cfULL);
      }
   }
   else
   {
      printf("xorpd_2 ... failed\n");
   }

   return;
}

int main(int argc, char **argv)
{
   signal(SIGILL, handle_sigill);

   addpd_1();
   addpd_2();
   addsd_1();
   addsd_2();
   andpd_1();
   andpd_2();
   andnpd_1();
   andnpd_2();
   cmpeqpd_1();
   cmpeqpd_2();
   cmpltpd_1();
   cmpltpd_2();
   cmplepd_1();
   cmplepd_2();
   cmpunordpd_1();
   cmpunordpd_2();
   cmpneqpd_1();
   cmpneqpd_2();
   cmpnltpd_1();
   cmpnltpd_2();
   cmpnlepd_1();
   cmpnlepd_2();
   cmpordpd_1();
   cmpordpd_2();
   cmpeqsd_1();
   cmpeqsd_2();
   cmpltsd_1();
   cmpltsd_2();
   cmplesd_1();
   cmplesd_2();
   cmpunordsd_1();
   cmpunordsd_2();
   cmpneqsd_1();
   cmpneqsd_2();
   cmpnltsd_1();
   cmpnltsd_2();
   cmpnlesd_1();
   cmpnlesd_2();
   cmpordsd_1();
   cmpordsd_2();
   comisd_1();
   comisd_2();
   comisd_3();
   comisd_4();
   comisd_5();
   comisd_6();
   cvtdq2pd_1();
   cvtdq2pd_2();
   cvtdq2ps_1();
   cvtdq2ps_2();
   cvtpd2dq_1();
   cvtpd2dq_2();
   cvtpd2pi_1();
   cvtpd2pi_2();
   cvtpd2ps_1();
   cvtpd2ps_2();
   cvtpi2pd_1();
   cvtpi2pd_2();
   cvtps2dq_1();
   cvtps2dq_2();
   cvtps2pd_1();
   cvtps2pd_2();
   cvtsd2si_1();
   cvtsd2si_2();
   cvtsd2ss_1();
   cvtsd2ss_2();
   cvtsi2sd_1();
   cvtsi2sd_2();
   cvtss2sd_1();
   cvtss2sd_2();
   cvttpd2pi_1();
   cvttpd2pi_2();
   cvttpd2dq_1();
   cvttpd2dq_2();
   cvttps2dq_1();
   cvttps2dq_2();
   cvttsd2si_1();
   cvttsd2si_2();
   divpd_1();
   divpd_2();
   divsd_1();
   divsd_2();
   lfence_1();
   maxpd_1();
   maxpd_2();
   maxsd_1();
   maxsd_2();
   mfence_1();
   minpd_1();
   minpd_2();
   minsd_1();
   minsd_2();
   movapd_1();
   movapd_2();
   movd_1();
   movd_2();
   movd_3();
   movd_4();
   movdqa_1();
   movdqa_2();
   movdqa_3();
   movdqu_1();
   movdqu_2();
   movdqu_3();
   movdq2q_1();
   movhpd_1();
   movhpd_2();
   movlpd_1();
   movlpd_2();
   movmskpd_1();
   movntdq_1();
   movnti_1();
   movntpd_1();
   movq_1();
   movq_2();
   movq_3();
   movq2dq_1();
   movsd_1();
   movsd_2();
   movsd_3();
   movupd_1();
   movupd_2();
   mulpd_1();
   mulpd_2();
   mulsd_1();
   mulsd_2();
   orpd_1();
   orpd_2();
   packssdw_1();
   packssdw_2();
   packsswb_1();
   packsswb_2();
   packuswb_1();
   packuswb_2();
   paddb_1();
   paddb_2();
   paddd_1();
   paddd_2();
   paddq_1();
   paddq_2();
   paddq_3();
   paddq_4();
   paddsb_1();
   paddsb_2();
   paddsw_1();
   paddsw_2();
   paddusb_1();
   paddusb_2();
   paddusw_1();
   paddusw_2();
   paddw_1();
   paddw_2();
   pand_1();
   pand_2();
   pandn_1();
   pandn_2();
   pavgb_1();
   pavgb_2();
   pavgw_1();
   pavgw_2();
   pcmpeqb_1();
   pcmpeqb_2();
   pcmpeqd_1();
   pcmpeqd_2();
   pcmpeqw_1();
   pcmpeqw_2();
   pcmpgtb_1();
   pcmpgtb_2();
   pcmpgtd_1();
   pcmpgtd_2();
   pcmpgtw_1();
   pcmpgtw_2();
   pextrw_1();
   pextrw_2();
   pextrw_3();
   pextrw_4();
   pextrw_5();
   pextrw_6();
   pextrw_7();
   pextrw_8();
   pinsrw_1();
   pinsrw_2();
   pinsrw_3();
   pinsrw_4();
   pinsrw_5();
   pinsrw_6();
   pinsrw_7();
   pinsrw_8();
   pinsrw_9();
   pinsrw_10();
   pinsrw_11();
   pinsrw_12();
   pinsrw_13();
   pinsrw_14();
   pinsrw_15();
   pinsrw_16();
   pmaddwd_1();
   pmaddwd_2();
   pmaxsw_1();
   pmaxsw_2();
   pmaxub_1();
   pmaxub_2();
   pminsw_1();
   pminsw_2();
   pminub_1();
   pminub_2();
   pmovmskb_1();
   pmulhuw_1();
   pmulhuw_2();
   pmulhw_1();
   pmulhw_2();
   pmullw_1();
   pmullw_2();
   pmuludq_1();
   pmuludq_2();
   pmuludq_3();
   pmuludq_4();
   por_1();
   por_2();
   psadbw_1();
   psadbw_2();
   pshufd_1();
   pshufd_2();
   pshufhw_1();
   pshufhw_2();
   pshuflw_1();
   pshuflw_2();
   pslld_1();
   pslld_2();
   pslld_3();
   pslldq_1();
   pslldq_2();
   pslldq_3();
   pslldq_4();
   pslldq_5();
   pslldq_6();
   pslldq_7();
   pslldq_8();
   pslldq_9();
   pslldq_10();
   pslldq_11();
   pslldq_12();
   pslldq_13();
   pslldq_14();
   pslldq_15();
   pslldq_16();
   pslldq_17();
   psllq_1();
   psllq_2();
   psllq_3();
   psllw_1();
   psllw_2();
   psllw_3();
   psrad_1();
   psrad_2();
   psrad_3();
   psraw_1();
   psraw_2();
   psraw_3();
   psrld_1();
   psrld_2();
   psrld_3();
   psrldq_1();
   psrldq_2();
   psrldq_3();
   psrldq_4();
   psrldq_5();
   psrldq_6();
   psrldq_7();
   psrldq_8();
   psrldq_9();
   psrldq_10();
   psrldq_11();
   psrldq_12();
   psrldq_13();
   psrldq_14();
   psrldq_15();
   psrldq_16();
   psrldq_17();
   psrlq_1();
   psrlq_2();
   psrlq_3();
   psrlw_1();
   psrlw_2();
   psrlw_3();
   psubb_1();
   psubb_2();
   psubd_1();
   psubd_2();
   psubq_1();
   psubq_2();
   psubq_3();
   psubq_4();
   psubsb_1();
   psubsb_2();
   psubsw_1();
   psubsw_2();
   psubusb_1();
   psubusb_2();
   psubusw_1();
   psubusw_2();
   psubw_1();
   psubw_2();
   punpckhbw_1();
   punpckhbw_2();
   punpckhdq_1();
   punpckhdq_2();
   punpckhqdq_1();
   punpckhqdq_2();
   punpckhwd_1();
   punpckhwd_2();
   punpcklbw_1();
   punpcklbw_2();
   punpckldq_1();
   punpckldq_2();
   punpcklqdq_1();
   punpcklqdq_2();
   punpcklwd_1();
   punpcklwd_2();
   pxor_1();
   pxor_2();
   shufpd_1();
   shufpd_2();
   sqrtpd_1();
   sqrtpd_2();
   sqrtsd_1();
   sqrtsd_2();
   subpd_1();
   subpd_2();
   subsd_1();
   subsd_2();
   ucomisd_1();
   ucomisd_2();
   ucomisd_3();
   ucomisd_4();
   ucomisd_5();
   ucomisd_6();
   unpckhpd_1();
   unpckhpd_2();
   unpcklpd_1();
   unpcklpd_2();
   xorpd_1();
   xorpd_2();

   exit(0);
}
