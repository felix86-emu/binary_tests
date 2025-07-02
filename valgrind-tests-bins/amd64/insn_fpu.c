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

static void fabs_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fabs\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) )
      {
         printf("fabs_1 ... ok\n");
      }
      else
      {
         printf("fabs_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
      }
   }
   else
   {
      printf("fabs_1 ... failed\n");
   }

   return;
}

static void fabs_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fabs\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) )
      {
         printf("fabs_2 ... ok\n");
      }
      else
      {
         printf("fabs_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
      }
   }
   else
   {
      printf("fabs_2 ... failed\n");
   }

   return;
}

static void fabs_3(void)
{
   reg64_t preset0 = { .pd = { 12345678.87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fabs\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 12345678.87654321) )
      {
         printf("fabs_3 ... ok\n");
      }
      else
      {
         printf("fabs_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12345678.87654321);
      }
   }
   else
   {
      printf("fabs_3 ... failed\n");
   }

   return;
}

static void fabs_4(void)
{
   reg64_t preset0 = { .pd = { -12345678.87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fabs\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 12345678.87654321) )
      {
         printf("fabs_4 ... ok\n");
      }
      else
      {
         printf("fabs_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12345678.87654321);
      }
   }
   else
   {
      printf("fabs_4 ... failed\n");
   }

   return;
}

static void fadds_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fadds %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("fadds_1 ... ok\n");
      }
      else
      {
         printf("fadds_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("fadds_1 ... failed\n");
   }

   return;
}

static void fadds_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fadds %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fadds_2 ... ok\n");
      }
      else
      {
         printf("fadds_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fadds_2 ... failed\n");
   }

   return;
}

static void fadds_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fadds %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fadds_3 ... ok\n");
      }
      else
      {
         printf("fadds_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fadds_3 ... failed\n");
   }

   return;
}

static void fadds_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fadds %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("fadds_4 ... ok\n");
      }
      else
      {
         printf("fadds_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("fadds_4 ... failed\n");
   }

   return;
}

static void faddl_1(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "faddl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("faddl_1 ... ok\n");
      }
      else
      {
         printf("faddl_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("faddl_1 ... failed\n");
   }

   return;
}

static void faddl_2(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "faddl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("faddl_2 ... ok\n");
      }
      else
      {
         printf("faddl_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("faddl_2 ... failed\n");
   }

   return;
}

static void faddl_3(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "faddl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("faddl_3 ... ok\n");
      }
      else
      {
         printf("faddl_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("faddl_3 ... failed\n");
   }

   return;
}

static void faddl_4(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t arg0 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "faddl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("faddl_4 ... ok\n");
      }
      else
      {
         printf("faddl_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("faddl_4 ... failed\n");
   }

   return;
}

static void fadd_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fadd %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("fadd_1 ... ok\n");
      }
      else
      {
         printf("fadd_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("fadd_1 ... failed\n");
   }

   return;
}

static void fadd_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fadd %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fadd_2 ... ok\n");
      }
      else
      {
         printf("fadd_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fadd_2 ... failed\n");
   }

   return;
}

static void fadd_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fadd %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fadd_3 ... ok\n");
      }
      else
      {
         printf("fadd_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fadd_3 ... failed\n");
   }

   return;
}

static void fadd_4(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fadd %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("fadd_4 ... ok\n");
      }
      else
      {
         printf("fadd_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("fadd_4 ... failed\n");
   }

   return;
}

static void fadd_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fadd %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("fadd_5 ... ok\n");
      }
      else
      {
         printf("fadd_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("fadd_5 ... failed\n");
   }

   return;
}

static void fadd_6(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fadd %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("fadd_6 ... ok\n");
      }
      else
      {
         printf("fadd_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("fadd_6 ... failed\n");
   }

   return;
}

static void fadd_7(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fadd %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("fadd_7 ... ok\n");
      }
      else
      {
         printf("fadd_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("fadd_7 ... failed\n");
   }

   return;
}

static void fadd_8(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fadd %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("fadd_8 ... ok\n");
      }
      else
      {
         printf("fadd_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("fadd_8 ... failed\n");
   }

   return;
}

static void fadd_9(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fadd %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("fadd_9 ... ok\n");
      }
      else
      {
         printf("fadd_9 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("fadd_9 ... failed\n");
   }

   return;
}

static void fadd_10(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fadd %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fadd_10 ... ok\n");
      }
      else
      {
         printf("fadd_10 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fadd_10 ... failed\n");
   }

   return;
}

static void fadd_11(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fadd %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fadd_11 ... ok\n");
      }
      else
      {
         printf("fadd_11 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fadd_11 ... failed\n");
   }

   return;
}

static void fadd_12(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fadd %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("fadd_12 ... ok\n");
      }
      else
      {
         printf("fadd_12 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("fadd_12 ... failed\n");
   }

   return;
}

static void fadd_13(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fadd %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("fadd_13 ... ok\n");
      }
      else
      {
         printf("fadd_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("fadd_13 ... failed\n");
   }

   return;
}

static void fadd_14(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fadd %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("fadd_14 ... ok\n");
      }
      else
      {
         printf("fadd_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("fadd_14 ... failed\n");
   }

   return;
}

static void fadd_15(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fadd %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("fadd_15 ... ok\n");
      }
      else
      {
         printf("fadd_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("fadd_15 ... failed\n");
   }

   return;
}

static void fadd_16(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fadd %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("fadd_16 ... ok\n");
      }
      else
      {
         printf("fadd_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("fadd_16 ... failed\n");
   }

   return;
}

static void faddp_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "faddp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("faddp_1 ... ok\n");
      }
      else
      {
         printf("faddp_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("faddp_1 ... failed\n");
   }

   return;
}

static void faddp_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "faddp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("faddp_2 ... ok\n");
      }
      else
      {
         printf("faddp_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("faddp_2 ... failed\n");
   }

   return;
}

static void faddp_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "faddp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("faddp_3 ... ok\n");
      }
      else
      {
         printf("faddp_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("faddp_3 ... failed\n");
   }

   return;
}

static void faddp_4(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "faddp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("faddp_4 ... ok\n");
      }
      else
      {
         printf("faddp_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("faddp_4 ... failed\n");
   }

   return;
}

static void faddp_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "faddp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("faddp_5 ... ok\n");
      }
      else
      {
         printf("faddp_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("faddp_5 ... failed\n");
   }

   return;
}

static void faddp_6(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "faddp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("faddp_6 ... ok\n");
      }
      else
      {
         printf("faddp_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("faddp_6 ... failed\n");
   }

   return;
}

static void faddp_7(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "faddp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("faddp_7 ... ok\n");
      }
      else
      {
         printf("faddp_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("faddp_7 ... failed\n");
   }

   return;
}

static void faddp_8(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "faddp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("faddp_8 ... ok\n");
      }
      else
      {
         printf("faddp_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("faddp_8 ... failed\n");
   }

   return;
}

static void faddp_9(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "faddp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("faddp_9 ... ok\n");
      }
      else
      {
         printf("faddp_9 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("faddp_9 ... failed\n");
   }

   return;
}

static void faddp_10(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "faddp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("faddp_10 ... ok\n");
      }
      else
      {
         printf("faddp_10 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("faddp_10 ... failed\n");
   }

   return;
}

static void faddp_11(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "faddp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("faddp_11 ... ok\n");
      }
      else
      {
         printf("faddp_11 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("faddp_11 ... failed\n");
   }

   return;
}

static void faddp_12(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "faddp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("faddp_12 ... ok\n");
      }
      else
      {
         printf("faddp_12 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("faddp_12 ... failed\n");
   }

   return;
}

static void faddp_13(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "faddp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("faddp_13 ... ok\n");
      }
      else
      {
         printf("faddp_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("faddp_13 ... failed\n");
   }

   return;
}

static void faddp_14(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "faddp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("faddp_14 ... ok\n");
      }
      else
      {
         printf("faddp_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("faddp_14 ... failed\n");
   }

   return;
}

static void faddp_15(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "faddp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("faddp_15 ... ok\n");
      }
      else
      {
         printf("faddp_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("faddp_15 ... failed\n");
   }

   return;
}

static void faddp_16(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "faddp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("faddp_16 ... ok\n");
      }
      else
      {
         printf("faddp_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("faddp_16 ... failed\n");
   }

   return;
}

static void fiadds_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fiadds %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 5555.5678F) )
      {
         printf("fiadds_1 ... ok\n");
      }
      else
      {
         printf("fiadds_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 5555.5678F);
      }
   }
   else
   {
      printf("fiadds_1 ... failed\n");
   }

   return;
}

static void fiadds_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fiadds %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 3086.4322F) )
      {
         printf("fiadds_2 ... ok\n");
      }
      else
      {
         printf("fiadds_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 3086.4322F);
      }
   }
   else
   {
      printf("fiadds_2 ... failed\n");
   }

   return;
}

static void fiadds_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fiadds %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -3086.4322F) )
      {
         printf("fiadds_3 ... ok\n");
      }
      else
      {
         printf("fiadds_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -3086.4322F);
      }
   }
   else
   {
      printf("fiadds_3 ... failed\n");
   }

   return;
}

static void fiadds_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fiadds %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -5555.5678F) )
      {
         printf("fiadds_4 ... ok\n");
      }
      else
      {
         printf("fiadds_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -5555.5678F);
      }
   }
   else
   {
      printf("fiadds_4 ... failed\n");
   }

   return;
}

static void fiadds_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fiadds %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1238888.7654321) )
      {
         printf("fiadds_5 ... ok\n");
      }
      else
      {
         printf("fiadds_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1238888.7654321);
      }
   }
   else
   {
      printf("fiadds_5 ... failed\n");
   }

   return;
}

static void fiadds_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fiadds %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -1230246.7654321) )
      {
         printf("fiadds_6 ... ok\n");
      }
      else
      {
         printf("fiadds_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1230246.7654321);
      }
   }
   else
   {
      printf("fiadds_6 ... failed\n");
   }

   return;
}

static void fiadds_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fiadds %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1230246.7654321) )
      {
         printf("fiadds_7 ... ok\n");
      }
      else
      {
         printf("fiadds_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1230246.7654321);
      }
   }
   else
   {
      printf("fiadds_7 ... failed\n");
   }

   return;
}

static void fiadds_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fiadds %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -1238888.7654321) )
      {
         printf("fiadds_8 ... ok\n");
      }
      else
      {
         printf("fiadds_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1238888.7654321);
      }
   }
   else
   {
      printf("fiadds_8 ... failed\n");
   }

   return;
}

static void fiaddl_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fiaddl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 87655555.5678F) )
      {
         printf("fiaddl_1 ... ok\n");
      }
      else
      {
         printf("fiaddl_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 87655555.5678F);
      }
   }
   else
   {
      printf("fiaddl_1 ... failed\n");
   }

   return;
}

static void fiaddl_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fiaddl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 87653086.4322F) )
      {
         printf("fiaddl_2 ... ok\n");
      }
      else
      {
         printf("fiaddl_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 87653086.4322F);
      }
   }
   else
   {
      printf("fiaddl_2 ... failed\n");
   }

   return;
}

static void fiaddl_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fiaddl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -87653086.4322F) )
      {
         printf("fiaddl_3 ... ok\n");
      }
      else
      {
         printf("fiaddl_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -87653086.4322F);
      }
   }
   else
   {
      printf("fiaddl_3 ... failed\n");
   }

   return;
}

static void fiaddl_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fiaddl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -87655555.5678F) )
      {
         printf("fiaddl_4 ... ok\n");
      }
      else
      {
         printf("fiaddl_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -87655555.5678F);
      }
   }
   else
   {
      printf("fiaddl_4 ... failed\n");
   }

   return;
}

static void fiaddl_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fiaddl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 88888888.7654321) )
      {
         printf("fiaddl_5 ... ok\n");
      }
      else
      {
         printf("fiaddl_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 88888888.7654321);
      }
   }
   else
   {
      printf("fiaddl_5 ... failed\n");
   }

   return;
}

static void fiaddl_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fiaddl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 86419753.2345679) )
      {
         printf("fiaddl_6 ... ok\n");
      }
      else
      {
         printf("fiaddl_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 86419753.2345679);
      }
   }
   else
   {
      printf("fiaddl_6 ... failed\n");
   }

   return;
}

static void fiaddl_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fiaddl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -86419753.2345679) )
      {
         printf("fiaddl_7 ... ok\n");
      }
      else
      {
         printf("fiaddl_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -86419753.2345679);
      }
   }
   else
   {
      printf("fiaddl_7 ... failed\n");
   }

   return;
}

static void fiaddl_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fiaddl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -88888888.7654321) )
      {
         printf("fiaddl_8 ... ok\n");
      }
      else
      {
         printf("fiaddl_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -88888888.7654321);
      }
   }
   else
   {
      printf("fiaddl_8 ... failed\n");
   }

   return;
}

static void fcoms_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { 1234.5676F } };
   reg64_t result0;
   reg16_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fcoms %3\n"
         "fstsw %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) &&
          (result1.uw[0] & 0x4700) == 0x0000 )
      {
         printf("fcoms_1 ... ok\n");
      }
      else
      {
         printf("fcoms_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
         printf("  fpusw & 0x%x = 0x%x (expected 0x%x)\n", 0x4700, result1.uw[0] & 0x4700, 0x0000);
      }
   }
   else
   {
      printf("fcoms_1 ... failed\n");
   }

   return;
}

static void fcoms_2(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { 1234.5679F } };
   reg64_t result0;
   reg16_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fcoms %3\n"
         "fstsw %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) &&
          (result1.uw[0] & 0x4700) == 0x0100 )
      {
         printf("fcoms_2 ... ok\n");
      }
      else
      {
         printf("fcoms_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
         printf("  fpusw & 0x%x = 0x%x (expected 0x%x)\n", 0x4700, result1.uw[0] & 0x4700, 0x0100);
      }
   }
   else
   {
      printf("fcoms_2 ... failed\n");
   }

   return;
}

static void fcoms_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { 1234.5678F } };
   reg64_t result0;
   reg16_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fcoms %3\n"
         "fstsw %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) &&
          (result1.uw[0] & 0x4700) == 0x4000 )
      {
         printf("fcoms_3 ... ok\n");
      }
      else
      {
         printf("fcoms_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
         printf("  fpusw & 0x%x = 0x%x (expected 0x%x)\n", 0x4700, result1.uw[0] & 0x4700, 0x4000);
      }
   }
   else
   {
      printf("fcoms_3 ... failed\n");
   }

   return;
}

static void fcoml_1(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 1234567.7654320 } };
   reg64_t result0;
   reg16_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fcoml %3\n"
         "fstsw %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 1234567.7654321) &&
          (result1.uw[0] & 0x4700) == 0x0000 )
      {
         printf("fcoml_1 ... ok\n");
      }
      else
      {
         printf("fcoml_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
         printf("  fpusw & 0x%x = 0x%x (expected 0x%x)\n", 0x4700, result1.uw[0] & 0x4700, 0x0000);
      }
   }
   else
   {
      printf("fcoml_1 ... failed\n");
   }

   return;
}

static void fcoml_2(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 1234567.7654322 } };
   reg64_t result0;
   reg16_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fcoml %3\n"
         "fstsw %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 1234567.7654321) &&
          (result1.uw[0] & 0x4700) == 0x0100 )
      {
         printf("fcoml_2 ... ok\n");
      }
      else
      {
         printf("fcoml_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
         printf("  fpusw & 0x%x = 0x%x (expected 0x%x)\n", 0x4700, result1.uw[0] & 0x4700, 0x0100);
      }
   }
   else
   {
      printf("fcoml_2 ... failed\n");
   }

   return;
}

static void fcoml_3(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t result0;
   reg16_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fcoml %3\n"
         "fstsw %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 1234567.7654321) &&
          (result1.uw[0] & 0x4700) == 0x4000 )
      {
         printf("fcoml_3 ... ok\n");
      }
      else
      {
         printf("fcoml_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
         printf("  fpusw & 0x%x = 0x%x (expected 0x%x)\n", 0x4700, result1.uw[0] & 0x4700, 0x4000);
      }
   }
   else
   {
      printf("fcoml_3 ... failed\n");
   }

   return;
}

static void fcomps_1(void)
{
   reg64_t preset0 = { .ps = { 8765.4321F } };
   reg64_t preset1 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { 1234.5676F } };
   reg64_t result0;
   reg16_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %3\n"
         "fcomps %4\n"
         "fstsw %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 8765.4321F) &&
          (result1.uw[0] & 0x4700) == 0x0000 )
      {
         printf("fcomps_1 ... ok\n");
      }
      else
      {
         printf("fcomps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 8765.4321F);
         printf("  fpusw & 0x%x = 0x%x (expected 0x%x)\n", 0x4700, result1.uw[0] & 0x4700, 0x0000);
      }
   }
   else
   {
      printf("fcomps_1 ... failed\n");
   }

   return;
}

static void fcomps_2(void)
{
   reg64_t preset0 = { .ps = { 8765.4321F } };
   reg64_t preset1 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { 1234.5679F } };
   reg64_t result0;
   reg16_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %3\n"
         "fcomps %4\n"
         "fstsw %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 8765.4321F) &&
          (result1.uw[0] & 0x4700) == 0x0100 )
      {
         printf("fcomps_2 ... ok\n");
      }
      else
      {
         printf("fcomps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 8765.4321F);
         printf("  fpusw & 0x%x = 0x%x (expected 0x%x)\n", 0x4700, result1.uw[0] & 0x4700, 0x0100);
      }
   }
   else
   {
      printf("fcomps_2 ... failed\n");
   }

   return;
}

static void fcomps_3(void)
{
   reg64_t preset0 = { .ps = { 8765.4321F } };
   reg64_t preset1 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { 1234.5678F } };
   reg64_t result0;
   reg16_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %3\n"
         "fcomps %4\n"
         "fstsw %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 8765.4321F) &&
          (result1.uw[0] & 0x4700) == 0x4000 )
      {
         printf("fcomps_3 ... ok\n");
      }
      else
      {
         printf("fcomps_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 8765.4321F);
         printf("  fpusw & 0x%x = 0x%x (expected 0x%x)\n", 0x4700, result1.uw[0] & 0x4700, 0x4000);
      }
   }
   else
   {
      printf("fcomps_3 ... failed\n");
   }

   return;
}

static void fcompl_1(void)
{
   reg64_t preset0 = { .pd = { 7654321.1234567 } };
   reg64_t preset1 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 1234567.7654320 } };
   reg64_t result0;
   reg16_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %3\n"
         "fcompl %4\n"
         "fstsw %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 7654321.1234567) &&
          (result1.uw[0] & 0x4700) == 0x0000 )
      {
         printf("fcompl_1 ... ok\n");
      }
      else
      {
         printf("fcompl_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 7654321.1234567);
         printf("  fpusw & 0x%x = 0x%x (expected 0x%x)\n", 0x4700, result1.uw[0] & 0x4700, 0x0000);
      }
   }
   else
   {
      printf("fcompl_1 ... failed\n");
   }

   return;
}

static void fcompl_2(void)
{
   reg64_t preset0 = { .pd = { 7654321.1234567 } };
   reg64_t preset1 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 1234567.7654322 } };
   reg64_t result0;
   reg16_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %3\n"
         "fcompl %4\n"
         "fstsw %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 7654321.1234567) &&
          (result1.uw[0] & 0x4700) == 0x0100 )
      {
         printf("fcompl_2 ... ok\n");
      }
      else
      {
         printf("fcompl_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 7654321.1234567);
         printf("  fpusw & 0x%x = 0x%x (expected 0x%x)\n", 0x4700, result1.uw[0] & 0x4700, 0x0100);
      }
   }
   else
   {
      printf("fcompl_2 ... failed\n");
   }

   return;
}

static void fcompl_3(void)
{
   reg64_t preset0 = { .pd = { 7654321.1234567 } };
   reg64_t preset1 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t result0;
   reg16_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %3\n"
         "fcompl %4\n"
         "fstsw %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 7654321.1234567) &&
          (result1.uw[0] & 0x4700) == 0x4000 )
      {
         printf("fcompl_3 ... ok\n");
      }
      else
      {
         printf("fcompl_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 7654321.1234567);
         printf("  fpusw & 0x%x = 0x%x (expected 0x%x)\n", 0x4700, result1.uw[0] & 0x4700, 0x4000);
      }
   }
   else
   {
      printf("fcompl_3 ... failed\n");
   }

   return;
}

static void fcomi_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 1234.5679F } };
   reg64_t result0;
   reg64_t result1;
   reg64_t result2;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldz\n"
         "flds %4\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000045, (%%rsp)\n"
         "popfq\n"
         "fcomi %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %2\n"
         "fstps %0\n"
         "fincstp\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1), "=m" (result2)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) &&
          eq_float(result1.ps[0], 1234.5679F) &&
          (result2.uq[0] & 0x0000000000000045UL) == 0x0000000000000000UL )
      {
         printf("fcomi_1 ... ok\n");
      }
      else
      {
         printf("fcomi_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1234.5679F);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result2.ud[0] & 0x0000000000000045UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("fcomi_1 ... failed\n");
   }

   return;
}

static void fcomi_2(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 1234.5676F } };
   reg64_t result0;
   reg64_t result1;
   reg64_t result2;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldz\n"
         "flds %4\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000044, (%%rsp)\n"
         "popfq\n"
         "fcomi %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %2\n"
         "fstps %0\n"
         "fincstp\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1), "=m" (result2)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) &&
          eq_float(result1.ps[0], 1234.5676F) &&
          (result2.uq[0] & 0x0000000000000045UL) == 0x0000000000000001UL )
      {
         printf("fcomi_2 ... ok\n");
      }
      else
      {
         printf("fcomi_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1234.5676F);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result2.ud[0] & 0x0000000000000045UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("fcomi_2 ... failed\n");
   }

   return;
}

static void fcomi_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 1234.5678F } };
   reg64_t result0;
   reg64_t result1;
   reg64_t result2;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldz\n"
         "flds %4\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000005, (%%rsp)\n"
         "popfq\n"
         "fcomi %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %2\n"
         "fstps %0\n"
         "fincstp\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1), "=m" (result2)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) &&
          eq_float(result1.ps[0], 1234.5678F) &&
          (result2.uq[0] & 0x0000000000000045UL) == 0x0000000000000040UL )
      {
         printf("fcomi_3 ... ok\n");
      }
      else
      {
         printf("fcomi_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1234.5678F);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result2.ud[0] & 0x0000000000000045UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("fcomi_3 ... failed\n");
   }

   return;
}

static void fcomi_4(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 1234567.7654322 } };
   reg64_t result0;
   reg64_t result1;
   reg64_t result2;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %3\n"
         "fldz\n"
         "fldl %4\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000045, (%%rsp)\n"
         "popfq\n"
         "fcomi %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %2\n"
         "fstpl %0\n"
         "fincstp\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1), "=m" (result2)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654322) &&
          eq_double(result1.pd[0], 1234567.7654321) &&
          (result2.uq[0] & 0x0000000000000045UL) == 0x0000000000000000UL )
      {
         printf("fcomi_4 ... ok\n");
      }
      else
      {
         printf("fcomi_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654322);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], 1234567.7654321);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result2.ud[0] & 0x0000000000000045UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("fcomi_4 ... failed\n");
   }

   return;
}

static void fcomi_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 1234567.7654320 } };
   reg64_t result0;
   reg64_t result1;
   reg64_t result2;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %3\n"
         "fldz\n"
         "fldl %4\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000044, (%%rsp)\n"
         "popfq\n"
         "fcomi %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %2\n"
         "fstpl %0\n"
         "fincstp\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1), "=m" (result2)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654320) &&
          eq_double(result1.pd[0], 1234567.7654321) &&
          (result2.uq[0] & 0x0000000000000045UL) == 0x0000000000000001UL )
      {
         printf("fcomi_5 ... ok\n");
      }
      else
      {
         printf("fcomi_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654320);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], 1234567.7654321);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result2.ud[0] & 0x0000000000000045UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("fcomi_5 ... failed\n");
   }

   return;
}

static void fcomi_6(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 1234567.7654321 } };
   reg64_t result0;
   reg64_t result1;
   reg64_t result2;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %3\n"
         "fldz\n"
         "fldl %4\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000005, (%%rsp)\n"
         "popfq\n"
         "fcomi %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %2\n"
         "fstpl %0\n"
         "fincstp\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1), "=m" (result2)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654321) &&
          eq_double(result1.pd[0], 1234567.7654321) &&
          (result2.uq[0] & 0x0000000000000045UL) == 0x0000000000000040UL )
      {
         printf("fcomi_6 ... ok\n");
      }
      else
      {
         printf("fcomi_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], 1234567.7654321);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result2.ud[0] & 0x0000000000000045UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("fcomi_6 ... failed\n");
   }

   return;
}

static void fcomip_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 1234.5679F } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %3\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000045, (%%rsp)\n"
         "popfq\n"
         "fcomip %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %1\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5679F) &&
          (result1.uq[0] & 0x0000000000000045UL) == 0x0000000000000000UL )
      {
         printf("fcomip_1 ... ok\n");
      }
      else
      {
         printf("fcomip_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5679F);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result1.ud[0] & 0x0000000000000045UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("fcomip_1 ... failed\n");
   }

   return;
}

static void fcomip_2(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 1234.5676F } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %3\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000044, (%%rsp)\n"
         "popfq\n"
         "fcomip %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %1\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5676F) &&
          (result1.uq[0] & 0x0000000000000045UL) == 0x0000000000000001UL )
      {
         printf("fcomip_2 ... ok\n");
      }
      else
      {
         printf("fcomip_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5676F);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result1.ud[0] & 0x0000000000000045UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("fcomip_2 ... failed\n");
   }

   return;
}

static void fcomip_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 1234.5678F } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %3\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000005, (%%rsp)\n"
         "popfq\n"
         "fcomip %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %1\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) &&
          (result1.uq[0] & 0x0000000000000045UL) == 0x0000000000000040UL )
      {
         printf("fcomip_3 ... ok\n");
      }
      else
      {
         printf("fcomip_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result1.ud[0] & 0x0000000000000045UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("fcomip_3 ... failed\n");
   }

   return;
}

static void fcomip_4(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 1234567.7654322 } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %3\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000045, (%%rsp)\n"
         "popfq\n"
         "fcomip %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %1\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654321) &&
          (result1.uq[0] & 0x0000000000000045UL) == 0x0000000000000000UL )
      {
         printf("fcomip_4 ... ok\n");
      }
      else
      {
         printf("fcomip_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result1.ud[0] & 0x0000000000000045UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("fcomip_4 ... failed\n");
   }

   return;
}

static void fcomip_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 1234567.7654320 } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %3\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000044, (%%rsp)\n"
         "popfq\n"
         "fcomip %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %1\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654321) &&
          (result1.uq[0] & 0x0000000000000045UL) == 0x0000000000000001UL )
      {
         printf("fcomip_5 ... ok\n");
      }
      else
      {
         printf("fcomip_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result1.ud[0] & 0x0000000000000045UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("fcomip_5 ... failed\n");
   }

   return;
}

static void fcomip_6(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 1234567.7654321 } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %3\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000005, (%%rsp)\n"
         "popfq\n"
         "fcomip %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %1\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654321) &&
          (result1.uq[0] & 0x0000000000000045UL) == 0x0000000000000040UL )
      {
         printf("fcomip_6 ... ok\n");
      }
      else
      {
         printf("fcomip_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result1.ud[0] & 0x0000000000000045UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("fcomip_6 ... failed\n");
   }

   return;
}

static void fucomi_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 1234.5679F } };
   reg64_t result0;
   reg64_t result1;
   reg64_t result2;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldz\n"
         "flds %4\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000045, (%%rsp)\n"
         "popfq\n"
         "fucomi %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %2\n"
         "fstps %0\n"
         "fincstp\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1), "=m" (result2)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) &&
          eq_float(result1.ps[0], 1234.5679F) &&
          (result2.uq[0] & 0x0000000000000045UL) == 0x0000000000000000UL )
      {
         printf("fucomi_1 ... ok\n");
      }
      else
      {
         printf("fucomi_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1234.5679F);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result2.ud[0] & 0x0000000000000045UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("fucomi_1 ... failed\n");
   }

   return;
}

static void fucomi_2(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 1234.5676F } };
   reg64_t result0;
   reg64_t result1;
   reg64_t result2;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldz\n"
         "flds %4\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000044, (%%rsp)\n"
         "popfq\n"
         "fucomi %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %2\n"
         "fstps %0\n"
         "fincstp\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1), "=m" (result2)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) &&
          eq_float(result1.ps[0], 1234.5676F) &&
          (result2.uq[0] & 0x0000000000000045UL) == 0x0000000000000001UL )
      {
         printf("fucomi_2 ... ok\n");
      }
      else
      {
         printf("fucomi_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1234.5676F);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result2.ud[0] & 0x0000000000000045UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("fucomi_2 ... failed\n");
   }

   return;
}

static void fucomi_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 1234.5678F } };
   reg64_t result0;
   reg64_t result1;
   reg64_t result2;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldz\n"
         "flds %4\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000005, (%%rsp)\n"
         "popfq\n"
         "fucomi %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %2\n"
         "fstps %0\n"
         "fincstp\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1), "=m" (result2)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) &&
          eq_float(result1.ps[0], 1234.5678F) &&
          (result2.uq[0] & 0x0000000000000045UL) == 0x0000000000000040UL )
      {
         printf("fucomi_3 ... ok\n");
      }
      else
      {
         printf("fucomi_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1234.5678F);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result2.ud[0] & 0x0000000000000045UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("fucomi_3 ... failed\n");
   }

   return;
}

static void fucomi_4(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 1234567.7654322 } };
   reg64_t result0;
   reg64_t result1;
   reg64_t result2;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %3\n"
         "fldz\n"
         "fldl %4\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000045, (%%rsp)\n"
         "popfq\n"
         "fucomi %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %2\n"
         "fstpl %0\n"
         "fincstp\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1), "=m" (result2)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654322) &&
          eq_double(result1.pd[0], 1234567.7654321) &&
          (result2.uq[0] & 0x0000000000000045UL) == 0x0000000000000000UL )
      {
         printf("fucomi_4 ... ok\n");
      }
      else
      {
         printf("fucomi_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654322);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], 1234567.7654321);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result2.ud[0] & 0x0000000000000045UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("fucomi_4 ... failed\n");
   }

   return;
}

static void fucomi_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 1234567.7654320 } };
   reg64_t result0;
   reg64_t result1;
   reg64_t result2;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %3\n"
         "fldz\n"
         "fldl %4\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000044, (%%rsp)\n"
         "popfq\n"
         "fucomi %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %2\n"
         "fstpl %0\n"
         "fincstp\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1), "=m" (result2)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654320) &&
          eq_double(result1.pd[0], 1234567.7654321) &&
          (result2.uq[0] & 0x0000000000000045UL) == 0x0000000000000001UL )
      {
         printf("fucomi_5 ... ok\n");
      }
      else
      {
         printf("fucomi_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654320);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], 1234567.7654321);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result2.ud[0] & 0x0000000000000045UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("fucomi_5 ... failed\n");
   }

   return;
}

static void fucomi_6(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 1234567.7654321 } };
   reg64_t result0;
   reg64_t result1;
   reg64_t result2;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %3\n"
         "fldz\n"
         "fldl %4\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000005, (%%rsp)\n"
         "popfq\n"
         "fucomi %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %2\n"
         "fstpl %0\n"
         "fincstp\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1), "=m" (result2)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654321) &&
          eq_double(result1.pd[0], 1234567.7654321) &&
          (result2.uq[0] & 0x0000000000000045UL) == 0x0000000000000040UL )
      {
         printf("fucomi_6 ... ok\n");
      }
      else
      {
         printf("fucomi_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], 1234567.7654321);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result2.ud[0] & 0x0000000000000045UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("fucomi_6 ... failed\n");
   }

   return;
}

static void fucomip_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 1234.5679F } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %3\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000045, (%%rsp)\n"
         "popfq\n"
         "fucomip %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %1\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5679F) &&
          (result1.uq[0] & 0x0000000000000045UL) == 0x0000000000000000UL )
      {
         printf("fucomip_1 ... ok\n");
      }
      else
      {
         printf("fucomip_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5679F);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result1.ud[0] & 0x0000000000000045UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("fucomip_1 ... failed\n");
   }

   return;
}

static void fucomip_2(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 1234.5676F } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %3\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000044, (%%rsp)\n"
         "popfq\n"
         "fucomip %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %1\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5676F) &&
          (result1.uq[0] & 0x0000000000000045UL) == 0x0000000000000001UL )
      {
         printf("fucomip_2 ... ok\n");
      }
      else
      {
         printf("fucomip_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5676F);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result1.ud[0] & 0x0000000000000045UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("fucomip_2 ... failed\n");
   }

   return;
}

static void fucomip_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 1234.5678F } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %3\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000005, (%%rsp)\n"
         "popfq\n"
         "fucomip %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %1\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) &&
          (result1.uq[0] & 0x0000000000000045UL) == 0x0000000000000040UL )
      {
         printf("fucomip_3 ... ok\n");
      }
      else
      {
         printf("fucomip_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result1.ud[0] & 0x0000000000000045UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("fucomip_3 ... failed\n");
   }

   return;
}

static void fucomip_4(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 1234567.7654322 } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %3\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000045, (%%rsp)\n"
         "popfq\n"
         "fucomip %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %1\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654321) &&
          (result1.uq[0] & 0x0000000000000045UL) == 0x0000000000000000UL )
      {
         printf("fucomip_4 ... ok\n");
      }
      else
      {
         printf("fucomip_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result1.ud[0] & 0x0000000000000045UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("fucomip_4 ... failed\n");
   }

   return;
}

static void fucomip_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 1234567.7654320 } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %3\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000044, (%%rsp)\n"
         "popfq\n"
         "fucomip %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %1\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654321) &&
          (result1.uq[0] & 0x0000000000000045UL) == 0x0000000000000001UL )
      {
         printf("fucomip_5 ... ok\n");
      }
      else
      {
         printf("fucomip_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result1.ud[0] & 0x0000000000000045UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("fucomip_5 ... failed\n");
   }

   return;
}

static void fucomip_6(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 1234567.7654321 } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %3\n"
         "pushfq\n"
         "andq $0xffffffffffffffba, (%%rsp)\n"
         "orq $0x0000000000000005, (%%rsp)\n"
         "popfq\n"
         "fucomip %%st(2), %%st(0)\n"
         "pushfq\n"
         "popq %1\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654321) &&
          (result1.uq[0] & 0x0000000000000045UL) == 0x0000000000000040UL )
      {
         printf("fucomip_6 ... ok\n");
      }
      else
      {
         printf("fucomip_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x0000000000000045UL, result1.ud[0] & 0x0000000000000045UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("fucomip_6 ... failed\n");
   }

   return;
}

static void fchs_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fchs\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -1234.5678F) )
      {
         printf("fchs_1 ... ok\n");
      }
      else
      {
         printf("fchs_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -1234.5678F);
      }
   }
   else
   {
      printf("fchs_1 ... failed\n");
   }

   return;
}

static void fchs_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fchs\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) )
      {
         printf("fchs_2 ... ok\n");
      }
      else
      {
         printf("fchs_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
      }
   }
   else
   {
      printf("fchs_2 ... failed\n");
   }

   return;
}

static void fchs_3(void)
{
   reg64_t preset0 = { .pd = { 12345678.87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fchs\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -12345678.87654321) )
      {
         printf("fchs_3 ... ok\n");
      }
      else
      {
         printf("fchs_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -12345678.87654321);
      }
   }
   else
   {
      printf("fchs_3 ... failed\n");
   }

   return;
}

static void fchs_4(void)
{
   reg64_t preset0 = { .pd = { -12345678.87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fchs\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 12345678.87654321) )
      {
         printf("fchs_4 ... ok\n");
      }
      else
      {
         printf("fchs_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12345678.87654321);
      }
   }
   else
   {
      printf("fchs_4 ... failed\n");
   }

   return;
}

static void fdivs_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fdivs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.140845058853402F) )
      {
         printf("fdivs_1 ... ok\n");
      }
      else
      {
         printf("fdivs_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.140845058853402F);
      }
   }
   else
   {
      printf("fdivs_1 ... failed\n");
   }

   return;
}

static void fdivs_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fdivs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.140845058853402F) )
      {
         printf("fdivs_2 ... ok\n");
      }
      else
      {
         printf("fdivs_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.140845058853402F);
      }
   }
   else
   {
      printf("fdivs_2 ... failed\n");
   }

   return;
}

static void fdivs_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fdivs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.140845058853402F) )
      {
         printf("fdivs_3 ... ok\n");
      }
      else
      {
         printf("fdivs_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.140845058853402F);
      }
   }
   else
   {
      printf("fdivs_3 ... failed\n");
   }

   return;
}

static void fdivs_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fdivs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.140845058853402F) )
      {
         printf("fdivs_4 ... ok\n");
      }
      else
      {
         printf("fdivs_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.140845058853402F);
      }
   }
   else
   {
      printf("fdivs_4 ... failed\n");
   }

   return;
}

static void fdivl_1(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fdivl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 0.16129030197711) )
      {
         printf("fdivl_1 ... ok\n");
      }
      else
      {
         printf("fdivl_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.16129030197711);
      }
   }
   else
   {
      printf("fdivl_1 ... failed\n");
   }

   return;
}

static void fdivl_2(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fdivl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -0.16129030197711) )
      {
         printf("fdivl_2 ... ok\n");
      }
      else
      {
         printf("fdivl_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.16129030197711);
      }
   }
   else
   {
      printf("fdivl_2 ... failed\n");
   }

   return;
}

static void fdivl_3(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fdivl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -0.16129030197711) )
      {
         printf("fdivl_3 ... ok\n");
      }
      else
      {
         printf("fdivl_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.16129030197711);
      }
   }
   else
   {
      printf("fdivl_3 ... failed\n");
   }

   return;
}

static void fdivl_4(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t arg0 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fdivl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 0.16129030197711) )
      {
         printf("fdivl_4 ... ok\n");
      }
      else
      {
         printf("fdivl_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.16129030197711);
      }
   }
   else
   {
      printf("fdivl_4 ... failed\n");
   }

   return;
}

static void fdiv_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdiv %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.140845058853402F) )
      {
         printf("fdiv_1 ... ok\n");
      }
      else
      {
         printf("fdiv_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.140845058853402F);
      }
   }
   else
   {
      printf("fdiv_1 ... failed\n");
   }

   return;
}

static void fdiv_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdiv %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.140845058853402F) )
      {
         printf("fdiv_2 ... ok\n");
      }
      else
      {
         printf("fdiv_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.140845058853402F);
      }
   }
   else
   {
      printf("fdiv_2 ... failed\n");
   }

   return;
}

static void fdiv_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdiv %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.140845058853402F) )
      {
         printf("fdiv_3 ... ok\n");
      }
      else
      {
         printf("fdiv_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.140845058853402F);
      }
   }
   else
   {
      printf("fdiv_3 ... failed\n");
   }

   return;
}

static void fdiv_4(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdiv %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.140845058853402F) )
      {
         printf("fdiv_4 ... ok\n");
      }
      else
      {
         printf("fdiv_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.140845058853402F);
      }
   }
   else
   {
      printf("fdiv_4 ... failed\n");
   }

   return;
}

static void fdiv_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdiv %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.16129030197711) )
      {
         printf("fdiv_5 ... ok\n");
      }
      else
      {
         printf("fdiv_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.16129030197711);
      }
   }
   else
   {
      printf("fdiv_5 ... failed\n");
   }

   return;
}

static void fdiv_6(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdiv %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -0.16129030197711) )
      {
         printf("fdiv_6 ... ok\n");
      }
      else
      {
         printf("fdiv_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.16129030197711);
      }
   }
   else
   {
      printf("fdiv_6 ... failed\n");
   }

   return;
}

static void fdiv_7(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdiv %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -0.16129030197711) )
      {
         printf("fdiv_7 ... ok\n");
      }
      else
      {
         printf("fdiv_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.16129030197711);
      }
   }
   else
   {
      printf("fdiv_7 ... failed\n");
   }

   return;
}

static void fdiv_8(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdiv %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.16129030197711) )
      {
         printf("fdiv_8 ... ok\n");
      }
      else
      {
         printf("fdiv_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.16129030197711);
      }
   }
   else
   {
      printf("fdiv_8 ... failed\n");
   }

   return;
}

static void fdiv_9(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fdiv %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7.10000058320005F) )
      {
         printf("fdiv_9 ... ok\n");
      }
      else
      {
         printf("fdiv_9 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.10000058320005F);
      }
   }
   else
   {
      printf("fdiv_9 ... failed\n");
   }

   return;
}

static void fdiv_10(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fdiv %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7.10000058320005F) )
      {
         printf("fdiv_10 ... ok\n");
      }
      else
      {
         printf("fdiv_10 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7.10000058320005F);
      }
   }
   else
   {
      printf("fdiv_10 ... failed\n");
   }

   return;
}

static void fdiv_11(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fdiv %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7.10000058320005F) )
      {
         printf("fdiv_11 ... ok\n");
      }
      else
      {
         printf("fdiv_11 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7.10000058320005F);
      }
   }
   else
   {
      printf("fdiv_11 ... failed\n");
   }

   return;
}

static void fdiv_12(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fdiv %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7.10000058320005F) )
      {
         printf("fdiv_12 ... ok\n");
      }
      else
      {
         printf("fdiv_12 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.10000058320005F);
      }
   }
   else
   {
      printf("fdiv_12 ... failed\n");
   }

   return;
}

static void fdiv_13(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fdiv %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6.20000079200001) )
      {
         printf("fdiv_13 ... ok\n");
      }
      else
      {
         printf("fdiv_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.20000079200001);
      }
   }
   else
   {
      printf("fdiv_13 ... failed\n");
   }

   return;
}

static void fdiv_14(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fdiv %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6.20000079200001) )
      {
         printf("fdiv_14 ... ok\n");
      }
      else
      {
         printf("fdiv_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6.20000079200001);
      }
   }
   else
   {
      printf("fdiv_14 ... failed\n");
   }

   return;
}

static void fdiv_15(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fdiv %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6.20000079200001) )
      {
         printf("fdiv_15 ... ok\n");
      }
      else
      {
         printf("fdiv_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6.20000079200001);
      }
   }
   else
   {
      printf("fdiv_15 ... failed\n");
   }

   return;
}

static void fdiv_16(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fdiv %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6.20000079200001) )
      {
         printf("fdiv_16 ... ok\n");
      }
      else
      {
         printf("fdiv_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.20000079200001);
      }
   }
   else
   {
      printf("fdiv_16 ... failed\n");
   }

   return;
}

static void fdivp_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdivp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.140845058853402F) )
      {
         printf("fdivp_1 ... ok\n");
      }
      else
      {
         printf("fdivp_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.140845058853402F);
      }
   }
   else
   {
      printf("fdivp_1 ... failed\n");
   }

   return;
}

static void fdivp_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdivp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.140845058853402F) )
      {
         printf("fdivp_2 ... ok\n");
      }
      else
      {
         printf("fdivp_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.140845058853402F);
      }
   }
   else
   {
      printf("fdivp_2 ... failed\n");
   }

   return;
}

static void fdivp_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdivp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.140845058853402F) )
      {
         printf("fdivp_3 ... ok\n");
      }
      else
      {
         printf("fdivp_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.140845058853402F);
      }
   }
   else
   {
      printf("fdivp_3 ... failed\n");
   }

   return;
}

static void fdivp_4(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdivp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.140845058853402F) )
      {
         printf("fdivp_4 ... ok\n");
      }
      else
      {
         printf("fdivp_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.140845058853402F);
      }
   }
   else
   {
      printf("fdivp_4 ... failed\n");
   }

   return;
}

static void fdivp_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdivp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.16129030197711) )
      {
         printf("fdivp_5 ... ok\n");
      }
      else
      {
         printf("fdivp_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.16129030197711);
      }
   }
   else
   {
      printf("fdivp_5 ... failed\n");
   }

   return;
}

static void fdivp_6(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdivp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -0.16129030197711) )
      {
         printf("fdivp_6 ... ok\n");
      }
      else
      {
         printf("fdivp_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.16129030197711);
      }
   }
   else
   {
      printf("fdivp_6 ... failed\n");
   }

   return;
}

static void fdivp_7(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdivp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -0.16129030197711) )
      {
         printf("fdivp_7 ... ok\n");
      }
      else
      {
         printf("fdivp_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.16129030197711);
      }
   }
   else
   {
      printf("fdivp_7 ... failed\n");
   }

   return;
}

static void fdivp_8(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdivp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.16129030197711) )
      {
         printf("fdivp_8 ... ok\n");
      }
      else
      {
         printf("fdivp_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.16129030197711);
      }
   }
   else
   {
      printf("fdivp_8 ... failed\n");
   }

   return;
}

static void fdivp_9(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fdivp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.140845058853402F) )
      {
         printf("fdivp_9 ... ok\n");
      }
      else
      {
         printf("fdivp_9 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.140845058853402F);
      }
   }
   else
   {
      printf("fdivp_9 ... failed\n");
   }

   return;
}

static void fdivp_10(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fdivp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.140845058853402F) )
      {
         printf("fdivp_10 ... ok\n");
      }
      else
      {
         printf("fdivp_10 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.140845058853402F);
      }
   }
   else
   {
      printf("fdivp_10 ... failed\n");
   }

   return;
}

static void fdivp_11(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fdivp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.140845058853402F) )
      {
         printf("fdivp_11 ... ok\n");
      }
      else
      {
         printf("fdivp_11 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.140845058853402F);
      }
   }
   else
   {
      printf("fdivp_11 ... failed\n");
   }

   return;
}

static void fdivp_12(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fdivp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.140845058853402F) )
      {
         printf("fdivp_12 ... ok\n");
      }
      else
      {
         printf("fdivp_12 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.140845058853402F);
      }
   }
   else
   {
      printf("fdivp_12 ... failed\n");
   }

   return;
}

static void fdivp_13(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fdivp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.16129030197711) )
      {
         printf("fdivp_13 ... ok\n");
      }
      else
      {
         printf("fdivp_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.16129030197711);
      }
   }
   else
   {
      printf("fdivp_13 ... failed\n");
   }

   return;
}

static void fdivp_14(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fdivp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -0.16129030197711) )
      {
         printf("fdivp_14 ... ok\n");
      }
      else
      {
         printf("fdivp_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.16129030197711);
      }
   }
   else
   {
      printf("fdivp_14 ... failed\n");
   }

   return;
}

static void fdivp_15(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fdivp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -0.16129030197711) )
      {
         printf("fdivp_15 ... ok\n");
      }
      else
      {
         printf("fdivp_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.16129030197711);
      }
   }
   else
   {
      printf("fdivp_15 ... failed\n");
   }

   return;
}

static void fdivp_16(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fdivp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.16129030197711) )
      {
         printf("fdivp_16 ... ok\n");
      }
      else
      {
         printf("fdivp_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.16129030197711);
      }
   }
   else
   {
      printf("fdivp_16 ... failed\n");
   }

   return;
}

static void fidivs_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.285713445961583F) )
      {
         printf("fidivs_1 ... ok\n");
      }
      else
      {
         printf("fidivs_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.285713445961583F);
      }
   }
   else
   {
      printf("fidivs_1 ... failed\n");
   }

   return;
}

static void fidivs_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.285713445961583F) )
      {
         printf("fidivs_2 ... ok\n");
      }
      else
      {
         printf("fidivs_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.285713445961583F);
      }
   }
   else
   {
      printf("fidivs_2 ... failed\n");
   }

   return;
}

static void fidivs_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.285713445961583F) )
      {
         printf("fidivs_3 ... ok\n");
      }
      else
      {
         printf("fidivs_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.285713445961583F);
      }
   }
   else
   {
      printf("fidivs_3 ... failed\n");
   }

   return;
}

static void fidivs_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.285713445961583F) )
      {
         printf("fidivs_4 ... ok\n");
      }
      else
      {
         printf("fidivs_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.285713445961583F);
      }
   }
   else
   {
      printf("fidivs_4 ... failed\n");
   }

   return;
}

static void fidivs_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 285.713437961606) )
      {
         printf("fidivs_5 ... ok\n");
      }
      else
      {
         printf("fidivs_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 285.713437961606);
      }
   }
   else
   {
      printf("fidivs_5 ... failed\n");
   }

   return;
}

static void fidivs_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -285.713437961606) )
      {
         printf("fidivs_6 ... ok\n");
      }
      else
      {
         printf("fidivs_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -285.713437961606);
      }
   }
   else
   {
      printf("fidivs_6 ... failed\n");
   }

   return;
}

static void fidivs_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -285.713437961606) )
      {
         printf("fidivs_7 ... ok\n");
      }
      else
      {
         printf("fidivs_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -285.713437961606);
      }
   }
   else
   {
      printf("fidivs_7 ... failed\n");
   }

   return;
}

static void fidivs_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 285.713437961606) )
      {
         printf("fidivs_8 ... ok\n");
      }
      else
      {
         printf("fidivs_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 285.713437961606);
      }
   }
   else
   {
      printf("fidivs_8 ... failed\n");
   }

   return;
}

static void fidivl_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.0000140845058853402F) )
      {
         printf("fidivl_1 ... ok\n");
      }
      else
      {
         printf("fidivl_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.0000140845058853402F);
      }
   }
   else
   {
      printf("fidivl_1 ... failed\n");
   }

   return;
}

static void fidivl_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.0000140845058853402F) )
      {
         printf("fidivl_2 ... ok\n");
      }
      else
      {
         printf("fidivl_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.0000140845058853402F);
      }
   }
   else
   {
      printf("fidivl_2 ... failed\n");
   }

   return;
}

static void fidivl_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.0000140845058853402F) )
      {
         printf("fidivl_3 ... ok\n");
      }
      else
      {
         printf("fidivl_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.0000140845058853402F);
      }
   }
   else
   {
      printf("fidivl_3 ... failed\n");
   }

   return;
}

static void fidivl_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.0000140845058853402F) )
      {
         printf("fidivl_4 ... ok\n");
      }
      else
      {
         printf("fidivl_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.0000140845058853402F);
      }
   }
   else
   {
      printf("fidivl_4 ... failed\n");
   }

   return;
}

static void fidivl_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { 654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1.88679220968317) )
      {
         printf("fidivl_5 ... ok\n");
      }
      else
      {
         printf("fidivl_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1.88679220968317);
      }
   }
   else
   {
      printf("fidivl_5 ... failed\n");
   }

   return;
}

static void fidivl_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { 654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -1.88679220968317) )
      {
         printf("fidivl_6 ... ok\n");
      }
      else
      {
         printf("fidivl_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1.88679220968317);
      }
   }
   else
   {
      printf("fidivl_6 ... failed\n");
   }

   return;
}

static void fidivl_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { -654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -1.88679220968317) )
      {
         printf("fidivl_7 ... ok\n");
      }
      else
      {
         printf("fidivl_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1.88679220968317);
      }
   }
   else
   {
      printf("fidivl_7 ... failed\n");
   }

   return;
}

static void fidivl_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { -654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1.88679220968317) )
      {
         printf("fidivl_8 ... ok\n");
      }
      else
      {
         printf("fidivl_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1.88679220968317);
      }
   }
   else
   {
      printf("fidivl_8 ... failed\n");
   }

   return;
}

static void fdivrs_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fdivrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7.10000058320005F) )
      {
         printf("fdivrs_1 ... ok\n");
      }
      else
      {
         printf("fdivrs_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.10000058320005F);
      }
   }
   else
   {
      printf("fdivrs_1 ... failed\n");
   }

   return;
}

static void fdivrs_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fdivrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7.10000058320005F) )
      {
         printf("fdivrs_2 ... ok\n");
      }
      else
      {
         printf("fdivrs_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7.10000058320005F);
      }
   }
   else
   {
      printf("fdivrs_2 ... failed\n");
   }

   return;
}

static void fdivrs_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fdivrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7.10000058320005F) )
      {
         printf("fdivrs_3 ... ok\n");
      }
      else
      {
         printf("fdivrs_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7.10000058320005F);
      }
   }
   else
   {
      printf("fdivrs_3 ... failed\n");
   }

   return;
}

static void fdivrs_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fdivrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7.10000058320005F) )
      {
         printf("fdivrs_4 ... ok\n");
      }
      else
      {
         printf("fdivrs_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.10000058320005F);
      }
   }
   else
   {
      printf("fdivrs_4 ... failed\n");
   }

   return;
}

static void fdivrl_1(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fdivrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 6.20000079200001) )
      {
         printf("fdivrl_1 ... ok\n");
      }
      else
      {
         printf("fdivrl_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.20000079200001);
      }
   }
   else
   {
      printf("fdivrl_1 ... failed\n");
   }

   return;
}

static void fdivrl_2(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fdivrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -6.20000079200001) )
      {
         printf("fdivrl_2 ... ok\n");
      }
      else
      {
         printf("fdivrl_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6.20000079200001);
      }
   }
   else
   {
      printf("fdivrl_2 ... failed\n");
   }

   return;
}

static void fdivrl_3(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fdivrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -6.20000079200001) )
      {
         printf("fdivrl_3 ... ok\n");
      }
      else
      {
         printf("fdivrl_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6.20000079200001);
      }
   }
   else
   {
      printf("fdivrl_3 ... failed\n");
   }

   return;
}

static void fdivrl_4(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t arg0 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fdivrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 6.20000079200001) )
      {
         printf("fdivrl_4 ... ok\n");
      }
      else
      {
         printf("fdivrl_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.20000079200001);
      }
   }
   else
   {
      printf("fdivrl_4 ... failed\n");
   }

   return;
}

static void fdivr_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdivr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7.10000058320005F) )
      {
         printf("fdivr_1 ... ok\n");
      }
      else
      {
         printf("fdivr_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.10000058320005F);
      }
   }
   else
   {
      printf("fdivr_1 ... failed\n");
   }

   return;
}

static void fdivr_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdivr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7.10000058320005F) )
      {
         printf("fdivr_2 ... ok\n");
      }
      else
      {
         printf("fdivr_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7.10000058320005F);
      }
   }
   else
   {
      printf("fdivr_2 ... failed\n");
   }

   return;
}

static void fdivr_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdivr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7.10000058320005F) )
      {
         printf("fdivr_3 ... ok\n");
      }
      else
      {
         printf("fdivr_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7.10000058320005F);
      }
   }
   else
   {
      printf("fdivr_3 ... failed\n");
   }

   return;
}

static void fdivr_4(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdivr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7.10000058320005F) )
      {
         printf("fdivr_4 ... ok\n");
      }
      else
      {
         printf("fdivr_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.10000058320005F);
      }
   }
   else
   {
      printf("fdivr_4 ... failed\n");
   }

   return;
}

static void fdivr_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdivr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6.20000079200001) )
      {
         printf("fdivr_5 ... ok\n");
      }
      else
      {
         printf("fdivr_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.20000079200001);
      }
   }
   else
   {
      printf("fdivr_5 ... failed\n");
   }

   return;
}

static void fdivr_6(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdivr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6.20000079200001) )
      {
         printf("fdivr_6 ... ok\n");
      }
      else
      {
         printf("fdivr_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6.20000079200001);
      }
   }
   else
   {
      printf("fdivr_6 ... failed\n");
   }

   return;
}

static void fdivr_7(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdivr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6.20000079200001) )
      {
         printf("fdivr_7 ... ok\n");
      }
      else
      {
         printf("fdivr_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6.20000079200001);
      }
   }
   else
   {
      printf("fdivr_7 ... failed\n");
   }

   return;
}

static void fdivr_8(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdivr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6.20000079200001) )
      {
         printf("fdivr_8 ... ok\n");
      }
      else
      {
         printf("fdivr_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.20000079200001);
      }
   }
   else
   {
      printf("fdivr_8 ... failed\n");
   }

   return;
}

static void fdivr_9(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fdivr %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.140845058853402F) )
      {
         printf("fdivr_9 ... ok\n");
      }
      else
      {
         printf("fdivr_9 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.140845058853402F);
      }
   }
   else
   {
      printf("fdivr_9 ... failed\n");
   }

   return;
}

static void fdivr_10(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fdivr %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.140845058853402F) )
      {
         printf("fdivr_10 ... ok\n");
      }
      else
      {
         printf("fdivr_10 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.140845058853402F);
      }
   }
   else
   {
      printf("fdivr_10 ... failed\n");
   }

   return;
}

static void fdivr_11(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fdivr %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -0.140845058853402F) )
      {
         printf("fdivr_11 ... ok\n");
      }
      else
      {
         printf("fdivr_11 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -0.140845058853402F);
      }
   }
   else
   {
      printf("fdivr_11 ... failed\n");
   }

   return;
}

static void fdivr_12(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fdivr %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 0.140845058853402F) )
      {
         printf("fdivr_12 ... ok\n");
      }
      else
      {
         printf("fdivr_12 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.140845058853402F);
      }
   }
   else
   {
      printf("fdivr_12 ... failed\n");
   }

   return;
}

static void fdivr_13(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fdivr %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.16129030197711) )
      {
         printf("fdivr_13 ... ok\n");
      }
      else
      {
         printf("fdivr_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.16129030197711);
      }
   }
   else
   {
      printf("fdivr_13 ... failed\n");
   }

   return;
}

static void fdivr_14(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fdivr %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -0.16129030197711) )
      {
         printf("fdivr_14 ... ok\n");
      }
      else
      {
         printf("fdivr_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.16129030197711);
      }
   }
   else
   {
      printf("fdivr_14 ... failed\n");
   }

   return;
}

static void fdivr_15(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fdivr %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -0.16129030197711) )
      {
         printf("fdivr_15 ... ok\n");
      }
      else
      {
         printf("fdivr_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.16129030197711);
      }
   }
   else
   {
      printf("fdivr_15 ... failed\n");
   }

   return;
}

static void fdivr_16(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fdivr %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.16129030197711) )
      {
         printf("fdivr_16 ... ok\n");
      }
      else
      {
         printf("fdivr_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.16129030197711);
      }
   }
   else
   {
      printf("fdivr_16 ... failed\n");
   }

   return;
}

static void fdivrp_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdivrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7.10000058320005F) )
      {
         printf("fdivrp_1 ... ok\n");
      }
      else
      {
         printf("fdivrp_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.10000058320005F);
      }
   }
   else
   {
      printf("fdivrp_1 ... failed\n");
   }

   return;
}

static void fdivrp_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdivrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7.10000058320005F) )
      {
         printf("fdivrp_2 ... ok\n");
      }
      else
      {
         printf("fdivrp_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7.10000058320005F);
      }
   }
   else
   {
      printf("fdivrp_2 ... failed\n");
   }

   return;
}

static void fdivrp_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdivrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7.10000058320005F) )
      {
         printf("fdivrp_3 ... ok\n");
      }
      else
      {
         printf("fdivrp_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7.10000058320005F);
      }
   }
   else
   {
      printf("fdivrp_3 ... failed\n");
   }

   return;
}

static void fdivrp_4(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fdivrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7.10000058320005F) )
      {
         printf("fdivrp_4 ... ok\n");
      }
      else
      {
         printf("fdivrp_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.10000058320005F);
      }
   }
   else
   {
      printf("fdivrp_4 ... failed\n");
   }

   return;
}

static void fdivrp_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdivrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6.20000079200001) )
      {
         printf("fdivrp_5 ... ok\n");
      }
      else
      {
         printf("fdivrp_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.20000079200001);
      }
   }
   else
   {
      printf("fdivrp_5 ... failed\n");
   }

   return;
}

static void fdivrp_6(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdivrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6.20000079200001) )
      {
         printf("fdivrp_6 ... ok\n");
      }
      else
      {
         printf("fdivrp_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6.20000079200001);
      }
   }
   else
   {
      printf("fdivrp_6 ... failed\n");
   }

   return;
}

static void fdivrp_7(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdivrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6.20000079200001) )
      {
         printf("fdivrp_7 ... ok\n");
      }
      else
      {
         printf("fdivrp_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6.20000079200001);
      }
   }
   else
   {
      printf("fdivrp_7 ... failed\n");
   }

   return;
}

static void fdivrp_8(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fdivrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6.20000079200001) )
      {
         printf("fdivrp_8 ... ok\n");
      }
      else
      {
         printf("fdivrp_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.20000079200001);
      }
   }
   else
   {
      printf("fdivrp_8 ... failed\n");
   }

   return;
}

static void fdivrp_9(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fdivrp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7.10000058320005F) )
      {
         printf("fdivrp_9 ... ok\n");
      }
      else
      {
         printf("fdivrp_9 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.10000058320005F);
      }
   }
   else
   {
      printf("fdivrp_9 ... failed\n");
   }

   return;
}

static void fdivrp_10(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fdivrp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7.10000058320005F) )
      {
         printf("fdivrp_10 ... ok\n");
      }
      else
      {
         printf("fdivrp_10 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7.10000058320005F);
      }
   }
   else
   {
      printf("fdivrp_10 ... failed\n");
   }

   return;
}

static void fdivrp_11(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fdivrp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7.10000058320005F) )
      {
         printf("fdivrp_11 ... ok\n");
      }
      else
      {
         printf("fdivrp_11 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7.10000058320005F);
      }
   }
   else
   {
      printf("fdivrp_11 ... failed\n");
   }

   return;
}

static void fdivrp_12(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fdivrp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7.10000058320005F) )
      {
         printf("fdivrp_12 ... ok\n");
      }
      else
      {
         printf("fdivrp_12 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.10000058320005F);
      }
   }
   else
   {
      printf("fdivrp_12 ... failed\n");
   }

   return;
}

static void fdivrp_13(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fdivrp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6.20000079200001) )
      {
         printf("fdivrp_13 ... ok\n");
      }
      else
      {
         printf("fdivrp_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.20000079200001);
      }
   }
   else
   {
      printf("fdivrp_13 ... failed\n");
   }

   return;
}

static void fdivrp_14(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fdivrp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6.20000079200001) )
      {
         printf("fdivrp_14 ... ok\n");
      }
      else
      {
         printf("fdivrp_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6.20000079200001);
      }
   }
   else
   {
      printf("fdivrp_14 ... failed\n");
   }

   return;
}

static void fdivrp_15(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fdivrp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6.20000079200001) )
      {
         printf("fdivrp_15 ... ok\n");
      }
      else
      {
         printf("fdivrp_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6.20000079200001);
      }
   }
   else
   {
      printf("fdivrp_15 ... failed\n");
   }

   return;
}

static void fdivrp_16(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fdivrp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6.20000079200001) )
      {
         printf("fdivrp_16 ... ok\n");
      }
      else
      {
         printf("fdivrp_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6.20000079200001);
      }
   }
   else
   {
      printf("fdivrp_16 ... failed\n");
   }

   return;
}

static void fidivrs_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 3.50001028700084F) )
      {
         printf("fidivrs_1 ... ok\n");
      }
      else
      {
         printf("fidivrs_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 3.50001028700084F);
      }
   }
   else
   {
      printf("fidivrs_1 ... failed\n");
   }

   return;
}

static void fidivrs_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -3.50001028700084F) )
      {
         printf("fidivrs_2 ... ok\n");
      }
      else
      {
         printf("fidivrs_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -3.50001028700084F);
      }
   }
   else
   {
      printf("fidivrs_2 ... failed\n");
   }

   return;
}

static void fidivrs_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -3.50001028700084F) )
      {
         printf("fidivrs_3 ... ok\n");
      }
      else
      {
         printf("fidivrs_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -3.50001028700084F);
      }
   }
   else
   {
      printf("fidivrs_3 ... failed\n");
   }

   return;
}

static void fidivrs_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 3.50001028700084F) )
      {
         printf("fidivrs_4 ... ok\n");
      }
      else
      {
         printf("fidivrs_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 3.50001028700084F);
      }
   }
   else
   {
      printf("fidivrs_4 ... failed\n");
   }

   return;
}

static void fidivrs_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivrs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.00350001038500114) )
      {
         printf("fidivrs_5 ... ok\n");
      }
      else
      {
         printf("fidivrs_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.00350001038500114);
      }
   }
   else
   {
      printf("fidivrs_5 ... failed\n");
   }

   return;
}

static void fidivrs_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivrs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -0.00350001038500114) )
      {
         printf("fidivrs_6 ... ok\n");
      }
      else
      {
         printf("fidivrs_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.00350001038500114);
      }
   }
   else
   {
      printf("fidivrs_6 ... failed\n");
   }

   return;
}

static void fidivrs_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivrs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -0.00350001038500114) )
      {
         printf("fidivrs_7 ... ok\n");
      }
      else
      {
         printf("fidivrs_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.00350001038500114);
      }
   }
   else
   {
      printf("fidivrs_7 ... failed\n");
   }

   return;
}

static void fidivrs_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivrs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.00350001038500114) )
      {
         printf("fidivrs_8 ... ok\n");
      }
      else
      {
         printf("fidivrs_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.00350001038500114);
      }
   }
   else
   {
      printf("fidivrs_8 ... failed\n");
   }

   return;
}

static void fidivrl_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivrl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 71000.0058320005F) )
      {
         printf("fidivrl_1 ... ok\n");
      }
      else
      {
         printf("fidivrl_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 71000.0058320005F);
      }
   }
   else
   {
      printf("fidivrl_1 ... failed\n");
   }

   return;
}

static void fidivrl_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivrl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -71000.0058320005F) )
      {
         printf("fidivrl_2 ... ok\n");
      }
      else
      {
         printf("fidivrl_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -71000.0058320005F);
      }
   }
   else
   {
      printf("fidivrl_2 ... failed\n");
   }

   return;
}

static void fidivrl_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivrl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -71000.0058320005F) )
      {
         printf("fidivrl_3 ... ok\n");
      }
      else
      {
         printf("fidivrl_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -71000.0058320005F);
      }
   }
   else
   {
      printf("fidivrl_3 ... failed\n");
   }

   return;
}

static void fidivrl_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fidivrl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 71000.0058320005F) )
      {
         printf("fidivrl_4 ... ok\n");
      }
      else
      {
         printf("fidivrl_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 71000.0058320005F);
      }
   }
   else
   {
      printf("fidivrl_4 ... failed\n");
   }

   return;
}

static void fidivrl_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { 654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.530000068300007) )
      {
         printf("fidivrl_5 ... ok\n");
      }
      else
      {
         printf("fidivrl_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.530000068300007);
      }
   }
   else
   {
      printf("fidivrl_5 ... failed\n");
   }

   return;
}

static void fidivrl_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { 654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -0.530000068300007) )
      {
         printf("fidivrl_6 ... ok\n");
      }
      else
      {
         printf("fidivrl_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.530000068300007);
      }
   }
   else
   {
      printf("fidivrl_6 ... failed\n");
   }

   return;
}

static void fidivrl_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { -654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -0.530000068300007) )
      {
         printf("fidivrl_7 ... ok\n");
      }
      else
      {
         printf("fidivrl_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -0.530000068300007);
      }
   }
   else
   {
      printf("fidivrl_7 ... failed\n");
   }

   return;
}

static void fidivrl_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { -654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fidivrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.530000068300007) )
      {
         printf("fidivrl_8 ... ok\n");
      }
      else
      {
         printf("fidivrl_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.530000068300007);
      }
   }
   else
   {
      printf("fidivrl_8 ... failed\n");
   }

   return;
}

static void filds_1(void)
{
   reg16_t arg0 = { .sw = { 12345 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "filds %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 12345.0F) )
      {
         printf("filds_1 ... ok\n");
      }
      else
      {
         printf("filds_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12345.0F);
      }
   }
   else
   {
      printf("filds_1 ... failed\n");
   }

   return;
}

static void filds_2(void)
{
   reg16_t arg0 = { .sw = { -12345 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "filds %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -12345.0F) )
      {
         printf("filds_2 ... ok\n");
      }
      else
      {
         printf("filds_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -12345.0F);
      }
   }
   else
   {
      printf("filds_2 ... failed\n");
   }

   return;
}

static void filds_3(void)
{
   reg16_t arg0 = { .sw = { 12345 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "filds %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 12345.0) )
      {
         printf("filds_3 ... ok\n");
      }
      else
      {
         printf("filds_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12345.0);
      }
   }
   else
   {
      printf("filds_3 ... failed\n");
   }

   return;
}

static void filds_4(void)
{
   reg16_t arg0 = { .sw = { -12345 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "filds %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -12345.0) )
      {
         printf("filds_4 ... ok\n");
      }
      else
      {
         printf("filds_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -12345.0);
      }
   }
   else
   {
      printf("filds_4 ... failed\n");
   }

   return;
}

static void fildl_1(void)
{
   reg32_t arg0 = { .sd = { 12345678 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fildl %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 12345678.0F) )
      {
         printf("fildl_1 ... ok\n");
      }
      else
      {
         printf("fildl_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12345678.0F);
      }
   }
   else
   {
      printf("fildl_1 ... failed\n");
   }

   return;
}

static void fildl_2(void)
{
   reg32_t arg0 = { .sd = { -12345678 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fildl %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -12345678.0F) )
      {
         printf("fildl_2 ... ok\n");
      }
      else
      {
         printf("fildl_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -12345678.0F);
      }
   }
   else
   {
      printf("fildl_2 ... failed\n");
   }

   return;
}

static void fildl_3(void)
{
   reg32_t arg0 = { .sd = { 12345678 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fildl %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 12345678.0) )
      {
         printf("fildl_3 ... ok\n");
      }
      else
      {
         printf("fildl_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 12345678.0);
      }
   }
   else
   {
      printf("fildl_3 ... failed\n");
   }

   return;
}

static void fildl_4(void)
{
   reg32_t arg0 = { .sd = { -12345678 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fildl %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -12345678.0) )
      {
         printf("fildl_4 ... ok\n");
      }
      else
      {
         printf("fildl_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -12345678.0);
      }
   }
   else
   {
      printf("fildl_4 ... failed\n");
   }

   return;
}

static void fildq_1(void)
{
   reg64_t arg0 = { .sq = { 123456787654321LL } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fildq %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_float(result0.ps[0], 123456787654321.0F) )
      {
         printf("fildq_1 ... ok\n");
      }
      else
      {
         printf("fildq_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 123456787654321.0F);
      }
   }
   else
   {
      printf("fildq_1 ... failed\n");
   }

   return;
}

static void fildq_2(void)
{
   reg64_t arg0 = { .sq = { -123456787654321LL } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fildq %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_float(result0.ps[0], -123456787654321.0F) )
      {
         printf("fildq_2 ... ok\n");
      }
      else
      {
         printf("fildq_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -123456787654321.0F);
      }
   }
   else
   {
      printf("fildq_2 ... failed\n");
   }

   return;
}

static void fildq_3(void)
{
   reg64_t arg0 = { .sq = { 123456787654321LL } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fildq %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 123456787654321.0) )
      {
         printf("fildq_3 ... ok\n");
      }
      else
      {
         printf("fildq_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 123456787654321.0);
      }
   }
   else
   {
      printf("fildq_3 ... failed\n");
   }

   return;
}

static void fildq_4(void)
{
   reg64_t arg0 = { .sq = { -123456787654321LL } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fildq %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -123456787654321.0) )
      {
         printf("fildq_4 ... ok\n");
      }
      else
      {
         printf("fildq_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -123456787654321.0);
      }
   }
   else
   {
      printf("fildq_4 ... failed\n");
   }

   return;
}

static void fists_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fists %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == 1235 &&
          eq_float(result1.ps[0], 1234.5678F) )
      {
         printf("fists_1 ... ok\n");
      }
      else
      {
         printf("fists_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1235);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1234.5678F);
      }
   }
   else
   {
      printf("fists_1 ... failed\n");
   }

   return;
}

static void fists_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fists %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == -1235 &&
          eq_float(result1.ps[0], -1234.5678F) )
      {
         printf("fists_2 ... ok\n");
      }
      else
      {
         printf("fists_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1235);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], -1234.5678F);
      }
   }
   else
   {
      printf("fists_2 ... failed\n");
   }

   return;
}

static void fists_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fists %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == 1234 &&
          eq_float(result1.ps[0], 1234.5678F) )
      {
         printf("fists_3 ... ok\n");
      }
      else
      {
         printf("fists_3 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1234.5678F);
      }
   }
   else
   {
      printf("fists_3 ... failed\n");
   }

   return;
}

static void fists_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fists %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == -1235 &&
          eq_float(result1.ps[0], -1234.5678F) )
      {
         printf("fists_4 ... ok\n");
      }
      else
      {
         printf("fists_4 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1235);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], -1234.5678F);
      }
   }
   else
   {
      printf("fists_4 ... failed\n");
   }

   return;
}

static void fists_5(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fists %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == 1235 &&
          eq_float(result1.ps[0], 1234.5678F) )
      {
         printf("fists_5 ... ok\n");
      }
      else
      {
         printf("fists_5 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1235);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1234.5678F);
      }
   }
   else
   {
      printf("fists_5 ... failed\n");
   }

   return;
}

static void fists_6(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fists %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == -1234 &&
          eq_float(result1.ps[0], -1234.5678F) )
      {
         printf("fists_6 ... ok\n");
      }
      else
      {
         printf("fists_6 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], -1234.5678F);
      }
   }
   else
   {
      printf("fists_6 ... failed\n");
   }

   return;
}

static void fists_7(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fists %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == 1234 &&
          eq_float(result1.ps[0], 1234.5678F) )
      {
         printf("fists_7 ... ok\n");
      }
      else
      {
         printf("fists_7 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1234.5678F);
      }
   }
   else
   {
      printf("fists_7 ... failed\n");
   }

   return;
}

static void fists_8(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fists %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == -1234 &&
          eq_float(result1.ps[0], -1234.5678F) )
      {
         printf("fists_8 ... ok\n");
      }
      else
      {
         printf("fists_8 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], -1234.5678F);
      }
   }
   else
   {
      printf("fists_8 ... failed\n");
   }

   return;
}

static void fistl_1(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistl %0\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == 1234568 &&
          eq_double(result1.pd[0], 1234567.7654321) )
      {
         printf("fistl_1 ... ok\n");
      }
      else
      {
         printf("fistl_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234568);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], 1234567.7654321);
      }
   }
   else
   {
      printf("fistl_1 ... failed\n");
   }

   return;
}

static void fistl_2(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistl %0\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == -1234568 &&
          eq_double(result1.pd[0], -1234567.7654321) )
      {
         printf("fistl_2 ... ok\n");
      }
      else
      {
         printf("fistl_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -1234568);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], -1234567.7654321);
      }
   }
   else
   {
      printf("fistl_2 ... failed\n");
   }

   return;
}

static void fistl_3(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistl %0\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == 1234567 &&
          eq_double(result1.pd[0], 1234567.7654321) )
      {
         printf("fistl_3 ... ok\n");
      }
      else
      {
         printf("fistl_3 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234567);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], 1234567.7654321);
      }
   }
   else
   {
      printf("fistl_3 ... failed\n");
   }

   return;
}

static void fistl_4(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistl %0\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == -1234568 &&
          eq_double(result1.pd[0], -1234567.7654321) )
      {
         printf("fistl_4 ... ok\n");
      }
      else
      {
         printf("fistl_4 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -1234568);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], -1234567.7654321);
      }
   }
   else
   {
      printf("fistl_4 ... failed\n");
   }

   return;
}

static void fistl_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistl %0\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == 1234568 &&
          eq_double(result1.pd[0], 1234567.7654321) )
      {
         printf("fistl_5 ... ok\n");
      }
      else
      {
         printf("fistl_5 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234568);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], 1234567.7654321);
      }
   }
   else
   {
      printf("fistl_5 ... failed\n");
   }

   return;
}

static void fistl_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistl %0\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == -1234567 &&
          eq_double(result1.pd[0], -1234567.7654321) )
      {
         printf("fistl_6 ... ok\n");
      }
      else
      {
         printf("fistl_6 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -1234567);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], -1234567.7654321);
      }
   }
   else
   {
      printf("fistl_6 ... failed\n");
   }

   return;
}

static void fistl_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistl %0\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == 1234567 &&
          eq_double(result1.pd[0], 1234567.7654321) )
      {
         printf("fistl_7 ... ok\n");
      }
      else
      {
         printf("fistl_7 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234567);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], 1234567.7654321);
      }
   }
   else
   {
      printf("fistl_7 ... failed\n");
   }

   return;
}

static void fistl_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistl %0\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == -1234567 &&
          eq_double(result1.pd[0], -1234567.7654321) )
      {
         printf("fistl_8 ... ok\n");
      }
      else
      {
         printf("fistl_8 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -1234567);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], -1234567.7654321);
      }
   }
   else
   {
      printf("fistl_8 ... failed\n");
   }

   return;
}

static void fistps_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == 1235 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistps_1 ... ok\n");
      }
      else
      {
         printf("fistps_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1235);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistps_1 ... failed\n");
   }

   return;
}

static void fistps_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == -1235 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistps_2 ... ok\n");
      }
      else
      {
         printf("fistps_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1235);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistps_2 ... failed\n");
   }

   return;
}

static void fistps_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == 1234 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistps_3 ... ok\n");
      }
      else
      {
         printf("fistps_3 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistps_3 ... failed\n");
   }

   return;
}

static void fistps_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == -1235 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistps_4 ... ok\n");
      }
      else
      {
         printf("fistps_4 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1235);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistps_4 ... failed\n");
   }

   return;
}

static void fistps_5(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == 1235 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistps_5 ... ok\n");
      }
      else
      {
         printf("fistps_5 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1235);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistps_5 ... failed\n");
   }

   return;
}

static void fistps_6(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == -1234 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistps_6 ... ok\n");
      }
      else
      {
         printf("fistps_6 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistps_6 ... failed\n");
   }

   return;
}

static void fistps_7(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == 1234 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistps_7 ... ok\n");
      }
      else
      {
         printf("fistps_7 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistps_7 ... failed\n");
   }

   return;
}

static void fistps_8(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg16_t arg0 = { .sw = { 0 } };
   reg16_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "flds %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == -1234 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistps_8 ... ok\n");
      }
      else
      {
         printf("fistps_8 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistps_8 ... failed\n");
   }

   return;
}

static void fistpl_1(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == 1234568 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpl_1 ... ok\n");
      }
      else
      {
         printf("fistpl_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234568);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpl_1 ... failed\n");
   }

   return;
}

static void fistpl_2(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == -1234568 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpl_2 ... ok\n");
      }
      else
      {
         printf("fistpl_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -1234568);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpl_2 ... failed\n");
   }

   return;
}

static void fistpl_3(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == 1234567 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpl_3 ... ok\n");
      }
      else
      {
         printf("fistpl_3 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234567);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpl_3 ... failed\n");
   }

   return;
}

static void fistpl_4(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == -1234568 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpl_4 ... ok\n");
      }
      else
      {
         printf("fistpl_4 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -1234568);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpl_4 ... failed\n");
   }

   return;
}

static void fistpl_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == 1234568 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpl_5 ... ok\n");
      }
      else
      {
         printf("fistpl_5 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234568);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpl_5 ... failed\n");
   }

   return;
}

static void fistpl_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == -1234567 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpl_6 ... ok\n");
      }
      else
      {
         printf("fistpl_6 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -1234567);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpl_6 ... failed\n");
   }

   return;
}

static void fistpl_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == 1234567 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpl_7 ... ok\n");
      }
      else
      {
         printf("fistpl_7 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234567);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpl_7 ... failed\n");
   }

   return;
}

static void fistpl_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg32_t arg0 = { .sd = { 0 } };
   reg32_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == -1234567 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpl_8 ... ok\n");
      }
      else
      {
         printf("fistpl_8 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -1234567);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpl_8 ... failed\n");
   }

   return;
}

static void fistpq_1(void)
{
   reg64_t preset0 = { .pd = { 123456787654321.6 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg64_t arg0 = { .sq = { 0LL } };
   reg64_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == 123456787654322LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpq_1 ... ok\n");
      }
      else
      {
         printf("fistpq_1 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 123456787654322LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpq_1 ... failed\n");
   }

   return;
}

static void fistpq_2(void)
{
   reg64_t preset0 = { .pd = { -123456787654321.6 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg64_t arg0 = { .sq = { 0LL } };
   reg64_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == -123456787654322LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpq_2 ... ok\n");
      }
      else
      {
         printf("fistpq_2 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], -123456787654322LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpq_2 ... failed\n");
   }

   return;
}

static void fistpq_3(void)
{
   reg64_t preset0 = { .pd = { 123456787654321.6 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg64_t arg0 = { .sq = { 0LL } };
   reg64_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == 123456787654321LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpq_3 ... ok\n");
      }
      else
      {
         printf("fistpq_3 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 123456787654321LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpq_3 ... failed\n");
   }

   return;
}

static void fistpq_4(void)
{
   reg64_t preset0 = { .pd = { -123456787654321.6 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg64_t arg0 = { .sq = { 0LL } };
   reg64_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == -123456787654322LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpq_4 ... ok\n");
      }
      else
      {
         printf("fistpq_4 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], -123456787654322LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpq_4 ... failed\n");
   }

   return;
}

static void fistpq_5(void)
{
   reg64_t preset0 = { .pd = { 123456787654321.6 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg64_t arg0 = { .sq = { 0LL } };
   reg64_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == 123456787654322LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpq_5 ... ok\n");
      }
      else
      {
         printf("fistpq_5 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 123456787654322LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpq_5 ... failed\n");
   }

   return;
}

static void fistpq_6(void)
{
   reg64_t preset0 = { .pd = { -123456787654321.6 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg64_t arg0 = { .sq = { 0LL } };
   reg64_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == -123456787654321LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpq_6 ... ok\n");
      }
      else
      {
         printf("fistpq_6 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], -123456787654321LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpq_6 ... failed\n");
   }

   return;
}

static void fistpq_7(void)
{
   reg64_t preset0 = { .pd = { 123456787654321.6 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg64_t arg0 = { .sq = { 0LL } };
   reg64_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == 123456787654321LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpq_7 ... ok\n");
      }
      else
      {
         printf("fistpq_7 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 123456787654321LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpq_7 ... failed\n");
   }

   return;
}

static void fistpq_8(void)
{
   reg64_t preset0 = { .pd = { -123456787654321.6 } };
   reg64_t preset1 = { .ps = { 1111.1111F } };
   reg64_t arg0 = { .sq = { 0LL } };
   reg64_t result0 = arg0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldl %2\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "fistpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == -123456787654321LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fistpq_8 ... ok\n");
      }
      else
      {
         printf("fistpq_8 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], -123456787654321LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fistpq_8 ... failed\n");
   }

   return;
}

static void flds_1(void)
{
   reg32_t arg0 = { .ps = { 1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) )
      {
         printf("flds_1 ... ok\n");
      }
      else
      {
         printf("flds_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
      }
   }
   else
   {
      printf("flds_1 ... failed\n");
   }

   return;
}

static void flds_2(void)
{
   reg32_t arg0 = { .ps = { -1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -1234.5678F) )
      {
         printf("flds_2 ... ok\n");
      }
      else
      {
         printf("flds_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -1234.5678F);
      }
   }
   else
   {
      printf("flds_2 ... failed\n");
   }

   return;
}

static void fldl_1(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 1234567.7654321) )
      {
         printf("fldl_1 ... ok\n");
      }
      else
      {
         printf("fldl_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
      }
   }
   else
   {
      printf("fldl_1 ... failed\n");
   }

   return;
}

static void fldl_2(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -1234567.7654321) )
      {
         printf("fldl_2 ... ok\n");
      }
      else
      {
         printf("fldl_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1234567.7654321);
      }
   }
   else
   {
      printf("fldl_2 ... failed\n");
   }

   return;
}

static void fld_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "fldz\n"
         "fld %%st(2)\n"
         "fstps %0\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.5678F) &&
          eq_float(result1.ps[0], 1234.5678F) )
      {
         printf("fld_1 ... ok\n");
      }
      else
      {
         printf("fld_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.5678F);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1234.5678F);
      }
   }
   else
   {
      printf("fld_1 ... failed\n");
   }

   return;
}

static void fld_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "fldz\n"
         "fld %%st(2)\n"
         "fstps %0\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -1234.5678F) &&
          eq_float(result1.ps[0], -1234.5678F) )
      {
         printf("fld_2 ... ok\n");
      }
      else
      {
         printf("fld_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -1234.5678F);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], -1234.5678F);
      }
   }
   else
   {
      printf("fld_2 ... failed\n");
   }

   return;
}

static void fld_3(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldz\n"
         "fld %%st(2)\n"
         "fstpl %0\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.7654321) &&
          eq_double(result1.pd[0], 1234567.7654321) )
      {
         printf("fld_3 ... ok\n");
      }
      else
      {
         printf("fld_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.7654321);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], 1234567.7654321);
      }
   }
   else
   {
      printf("fld_3 ... failed\n");
   }

   return;
}

static void fld1_1(void)
{
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fld1\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1.0) )
      {
         printf("fld1_1 ... ok\n");
      }
      else
      {
         printf("fld1_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1.0);
      }
   }
   else
   {
      printf("fld1_1 ... failed\n");
   }

   return;
}

static void fldl2t_1(void)
{
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl2t\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (state[0])

      );

      if (eq_double(result0.pd[0], 3.321928094887362) )
      {
         printf("fldl2t_1 ... ok\n");
      }
      else
      {
         printf("fldl2t_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 3.321928094887362);
      }
   }
   else
   {
      printf("fldl2t_1 ... failed\n");
   }

   return;
}

static void fldl2e_1(void)
{
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl2e\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1.442695040888963) )
      {
         printf("fldl2e_1 ... ok\n");
      }
      else
      {
         printf("fldl2e_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1.442695040888963);
      }
   }
   else
   {
      printf("fldl2e_1 ... failed\n");
   }

   return;
}

static void fldpi_1(void)
{
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldpi\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (state[0])

      );

      if (eq_double(result0.pd[0], 3.141592653589793) )
      {
         printf("fldpi_1 ... ok\n");
      }
      else
      {
         printf("fldpi_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 3.141592653589793);
      }
   }
   else
   {
      printf("fldpi_1 ... failed\n");
   }

   return;
}

static void fldlg2_1(void)
{
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldlg2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.3010299956639812) )
      {
         printf("fldlg2_1 ... ok\n");
      }
      else
      {
         printf("fldlg2_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.3010299956639812);
      }
   }
   else
   {
      printf("fldlg2_1 ... failed\n");
   }

   return;
}

static void fldln2_1(void)
{
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldln2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.6931471805599453) )
      {
         printf("fldln2_1 ... ok\n");
      }
      else
      {
         printf("fldln2_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.6931471805599453);
      }
   }
   else
   {
      printf("fldln2_1 ... failed\n");
   }

   return;
}

static void fldz_1(void)
{
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldz\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (state[0])

      );

      if (eq_double(result0.pd[0], 0.0) )
      {
         printf("fldz_1 ... ok\n");
      }
      else
      {
         printf("fldz_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 0.0);
      }
   }
   else
   {
      printf("fldz_1 ... failed\n");
   }

   return;
}

static void fmuls_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fmuls %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 10821520.2237464F) )
      {
         printf("fmuls_1 ... ok\n");
      }
      else
      {
         printf("fmuls_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 10821520.2237464F);
      }
   }
   else
   {
      printf("fmuls_1 ... failed\n");
   }

   return;
}

static void fmuls_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fmuls %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -10821520.2237464F) )
      {
         printf("fmuls_2 ... ok\n");
      }
      else
      {
         printf("fmuls_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -10821520.2237464F);
      }
   }
   else
   {
      printf("fmuls_2 ... failed\n");
   }

   return;
}

static void fmuls_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fmuls %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -10821520.2237464F) )
      {
         printf("fmuls_3 ... ok\n");
      }
      else
      {
         printf("fmuls_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -10821520.2237464F);
      }
   }
   else
   {
      printf("fmuls_3 ... failed\n");
   }

   return;
}

static void fmuls_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fmuls %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 10821520.2237464F) )
      {
         printf("fmuls_4 ... ok\n");
      }
      else
      {
         printf("fmuls_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 10821520.2237464F);
      }
   }
   else
   {
      printf("fmuls_4 ... failed\n");
   }

   return;
}

static void fmull_1(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fmull %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 9449778125285.66) )
      {
         printf("fmull_1 ... ok\n");
      }
      else
      {
         printf("fmull_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 9449778125285.66);
      }
   }
   else
   {
      printf("fmull_1 ... failed\n");
   }

   return;
}

static void fmull_2(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fmull %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -9449778125285.66) )
      {
         printf("fmull_2 ... ok\n");
      }
      else
      {
         printf("fmull_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -9449778125285.66);
      }
   }
   else
   {
      printf("fmull_2 ... failed\n");
   }

   return;
}

static void fmull_3(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fmull %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -9449778125285.66) )
      {
         printf("fmull_3 ... ok\n");
      }
      else
      {
         printf("fmull_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -9449778125285.66);
      }
   }
   else
   {
      printf("fmull_3 ... failed\n");
   }

   return;
}

static void fmull_4(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t arg0 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fmull %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 9449778125285.66) )
      {
         printf("fmull_4 ... ok\n");
      }
      else
      {
         printf("fmull_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 9449778125285.66);
      }
   }
   else
   {
      printf("fmull_4 ... failed\n");
   }

   return;
}

static void fmul_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fmul %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 10821520.2237464F) )
      {
         printf("fmul_1 ... ok\n");
      }
      else
      {
         printf("fmul_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 10821520.2237464F);
      }
   }
   else
   {
      printf("fmul_1 ... failed\n");
   }

   return;
}

static void fmul_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fmul %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -10821520.2237464F) )
      {
         printf("fmul_2 ... ok\n");
      }
      else
      {
         printf("fmul_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -10821520.2237464F);
      }
   }
   else
   {
      printf("fmul_2 ... failed\n");
   }

   return;
}

static void fmul_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fmul %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -10821520.2237464F) )
      {
         printf("fmul_3 ... ok\n");
      }
      else
      {
         printf("fmul_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -10821520.2237464F);
      }
   }
   else
   {
      printf("fmul_3 ... failed\n");
   }

   return;
}

static void fmul_4(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fmul %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 10821520.2237464F) )
      {
         printf("fmul_4 ... ok\n");
      }
      else
      {
         printf("fmul_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 10821520.2237464F);
      }
   }
   else
   {
      printf("fmul_4 ... failed\n");
   }

   return;
}

static void fmul_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fmul %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 9449778125285.66) )
      {
         printf("fmul_5 ... ok\n");
      }
      else
      {
         printf("fmul_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 9449778125285.66);
      }
   }
   else
   {
      printf("fmul_5 ... failed\n");
   }

   return;
}

static void fmul_6(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fmul %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -9449778125285.66) )
      {
         printf("fmul_6 ... ok\n");
      }
      else
      {
         printf("fmul_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -9449778125285.66);
      }
   }
   else
   {
      printf("fmul_6 ... failed\n");
   }

   return;
}

static void fmul_7(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fmul %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -9449778125285.66) )
      {
         printf("fmul_7 ... ok\n");
      }
      else
      {
         printf("fmul_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -9449778125285.66);
      }
   }
   else
   {
      printf("fmul_7 ... failed\n");
   }

   return;
}

static void fmul_8(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fmul %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 9449778125285.66) )
      {
         printf("fmul_8 ... ok\n");
      }
      else
      {
         printf("fmul_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 9449778125285.66);
      }
   }
   else
   {
      printf("fmul_8 ... failed\n");
   }

   return;
}

static void fmul_9(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fmul %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 10821520.2237464F) )
      {
         printf("fmul_9 ... ok\n");
      }
      else
      {
         printf("fmul_9 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 10821520.2237464F);
      }
   }
   else
   {
      printf("fmul_9 ... failed\n");
   }

   return;
}

static void fmul_10(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fmul %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -10821520.2237464F) )
      {
         printf("fmul_10 ... ok\n");
      }
      else
      {
         printf("fmul_10 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -10821520.2237464F);
      }
   }
   else
   {
      printf("fmul_10 ... failed\n");
   }

   return;
}

static void fmul_11(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fmul %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -10821520.2237464F) )
      {
         printf("fmul_11 ... ok\n");
      }
      else
      {
         printf("fmul_11 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -10821520.2237464F);
      }
   }
   else
   {
      printf("fmul_11 ... failed\n");
   }

   return;
}

static void fmul_12(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fmul %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 10821520.2237464F) )
      {
         printf("fmul_12 ... ok\n");
      }
      else
      {
         printf("fmul_12 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 10821520.2237464F);
      }
   }
   else
   {
      printf("fmul_12 ... failed\n");
   }

   return;
}

static void fmul_13(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fmul %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 9449778125285.66) )
      {
         printf("fmul_13 ... ok\n");
      }
      else
      {
         printf("fmul_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 9449778125285.66);
      }
   }
   else
   {
      printf("fmul_13 ... failed\n");
   }

   return;
}

static void fmul_14(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fmul %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -9449778125285.66) )
      {
         printf("fmul_14 ... ok\n");
      }
      else
      {
         printf("fmul_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -9449778125285.66);
      }
   }
   else
   {
      printf("fmul_14 ... failed\n");
   }

   return;
}

static void fmul_15(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fmul %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -9449778125285.66) )
      {
         printf("fmul_15 ... ok\n");
      }
      else
      {
         printf("fmul_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -9449778125285.66);
      }
   }
   else
   {
      printf("fmul_15 ... failed\n");
   }

   return;
}

static void fmul_16(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fmul %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 9449778125285.66) )
      {
         printf("fmul_16 ... ok\n");
      }
      else
      {
         printf("fmul_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 9449778125285.66);
      }
   }
   else
   {
      printf("fmul_16 ... failed\n");
   }

   return;
}

static void fmulp_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fmulp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 10821520.2237464F) )
      {
         printf("fmulp_1 ... ok\n");
      }
      else
      {
         printf("fmulp_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 10821520.2237464F);
      }
   }
   else
   {
      printf("fmulp_1 ... failed\n");
   }

   return;
}

static void fmulp_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fmulp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -10821520.2237464F) )
      {
         printf("fmulp_2 ... ok\n");
      }
      else
      {
         printf("fmulp_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -10821520.2237464F);
      }
   }
   else
   {
      printf("fmulp_2 ... failed\n");
   }

   return;
}

static void fmulp_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fmulp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -10821520.2237464F) )
      {
         printf("fmulp_3 ... ok\n");
      }
      else
      {
         printf("fmulp_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -10821520.2237464F);
      }
   }
   else
   {
      printf("fmulp_3 ... failed\n");
   }

   return;
}

static void fmulp_4(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fmulp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 10821520.2237464F) )
      {
         printf("fmulp_4 ... ok\n");
      }
      else
      {
         printf("fmulp_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 10821520.2237464F);
      }
   }
   else
   {
      printf("fmulp_4 ... failed\n");
   }

   return;
}

static void fmulp_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fmulp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 9449778125285.66) )
      {
         printf("fmulp_5 ... ok\n");
      }
      else
      {
         printf("fmulp_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 9449778125285.66);
      }
   }
   else
   {
      printf("fmulp_5 ... failed\n");
   }

   return;
}

static void fmulp_6(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fmulp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -9449778125285.66) )
      {
         printf("fmulp_6 ... ok\n");
      }
      else
      {
         printf("fmulp_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -9449778125285.66);
      }
   }
   else
   {
      printf("fmulp_6 ... failed\n");
   }

   return;
}

static void fmulp_7(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fmulp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -9449778125285.66) )
      {
         printf("fmulp_7 ... ok\n");
      }
      else
      {
         printf("fmulp_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -9449778125285.66);
      }
   }
   else
   {
      printf("fmulp_7 ... failed\n");
   }

   return;
}

static void fmulp_8(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fmulp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 9449778125285.66) )
      {
         printf("fmulp_8 ... ok\n");
      }
      else
      {
         printf("fmulp_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 9449778125285.66);
      }
   }
   else
   {
      printf("fmulp_8 ... failed\n");
   }

   return;
}

static void fmulp_9(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fmulp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 10821520.2237464F) )
      {
         printf("fmulp_9 ... ok\n");
      }
      else
      {
         printf("fmulp_9 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 10821520.2237464F);
      }
   }
   else
   {
      printf("fmulp_9 ... failed\n");
   }

   return;
}

static void fmulp_10(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fmulp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -10821520.2237464F) )
      {
         printf("fmulp_10 ... ok\n");
      }
      else
      {
         printf("fmulp_10 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -10821520.2237464F);
      }
   }
   else
   {
      printf("fmulp_10 ... failed\n");
   }

   return;
}

static void fmulp_11(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fmulp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -10821520.2237464F) )
      {
         printf("fmulp_11 ... ok\n");
      }
      else
      {
         printf("fmulp_11 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -10821520.2237464F);
      }
   }
   else
   {
      printf("fmulp_11 ... failed\n");
   }

   return;
}

static void fmulp_12(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fmulp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 10821520.2237464F) )
      {
         printf("fmulp_12 ... ok\n");
      }
      else
      {
         printf("fmulp_12 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 10821520.2237464F);
      }
   }
   else
   {
      printf("fmulp_12 ... failed\n");
   }

   return;
}

static void fmulp_13(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fmulp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 9449778125285.66) )
      {
         printf("fmulp_13 ... ok\n");
      }
      else
      {
         printf("fmulp_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 9449778125285.66);
      }
   }
   else
   {
      printf("fmulp_13 ... failed\n");
   }

   return;
}

static void fmulp_14(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fmulp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -9449778125285.66) )
      {
         printf("fmulp_14 ... ok\n");
      }
      else
      {
         printf("fmulp_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -9449778125285.66);
      }
   }
   else
   {
      printf("fmulp_14 ... failed\n");
   }

   return;
}

static void fmulp_15(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fmulp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -9449778125285.66) )
      {
         printf("fmulp_15 ... ok\n");
      }
      else
      {
         printf("fmulp_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -9449778125285.66);
      }
   }
   else
   {
      printf("fmulp_15 ... failed\n");
   }

   return;
}

static void fmulp_16(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fmulp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 9449778125285.66) )
      {
         printf("fmulp_16 ... ok\n");
      }
      else
      {
         printf("fmulp_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 9449778125285.66);
      }
   }
   else
   {
      printf("fmulp_16 ... failed\n");
   }

   return;
}

static void fimuls_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fimuls %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 5334567.4638F) )
      {
         printf("fimuls_1 ... ok\n");
      }
      else
      {
         printf("fimuls_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 5334567.4638F);
      }
   }
   else
   {
      printf("fimuls_1 ... failed\n");
   }

   return;
}

static void fimuls_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fimuls %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -5334567.4638F) )
      {
         printf("fimuls_2 ... ok\n");
      }
      else
      {
         printf("fimuls_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -5334567.4638F);
      }
   }
   else
   {
      printf("fimuls_2 ... failed\n");
   }

   return;
}

static void fimuls_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fimuls %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -5334567.4638F) )
      {
         printf("fimuls_3 ... ok\n");
      }
      else
      {
         printf("fimuls_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -5334567.4638F);
      }
   }
   else
   {
      printf("fimuls_3 ... failed\n");
   }

   return;
}

static void fimuls_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fimuls %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 5334567.4638F) )
      {
         printf("fimuls_4 ... ok\n");
      }
      else
      {
         printf("fimuls_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 5334567.4638F);
      }
   }
   else
   {
      printf("fimuls_4 ... failed\n");
   }

   return;
}

static void fimuls_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fimuls %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 5334567314.4321) )
      {
         printf("fimuls_5 ... ok\n");
      }
      else
      {
         printf("fimuls_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 5334567314.4321);
      }
   }
   else
   {
      printf("fimuls_5 ... failed\n");
   }

   return;
}

static void fimuls_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fimuls %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -5334567314.4321) )
      {
         printf("fimuls_6 ... ok\n");
      }
      else
      {
         printf("fimuls_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -5334567314.4321);
      }
   }
   else
   {
      printf("fimuls_6 ... failed\n");
   }

   return;
}

static void fimuls_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fimuls %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -5334567314.4321) )
      {
         printf("fimuls_7 ... ok\n");
      }
      else
      {
         printf("fimuls_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -5334567314.4321);
      }
   }
   else
   {
      printf("fimuls_7 ... failed\n");
   }

   return;
}

static void fimuls_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fimuls %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 5334567314.4321) )
      {
         printf("fimuls_8 ... ok\n");
      }
      else
      {
         printf("fimuls_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 5334567314.4321);
      }
   }
   else
   {
      printf("fimuls_8 ... failed\n");
   }

   return;
}

static void fimull_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fimull %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 108215202237.464F) )
      {
         printf("fimull_1 ... ok\n");
      }
      else
      {
         printf("fimull_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 108215202237.464F);
      }
   }
   else
   {
      printf("fimull_1 ... failed\n");
   }

   return;
}

static void fimull_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fimull %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -108215202237.464F) )
      {
         printf("fimull_2 ... ok\n");
      }
      else
      {
         printf("fimull_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -108215202237.464F);
      }
   }
   else
   {
      printf("fimull_2 ... failed\n");
   }

   return;
}

static void fimull_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fimull %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -108215202237.464F) )
      {
         printf("fimull_3 ... ok\n");
      }
      else
      {
         printf("fimull_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -108215202237.464F);
      }
   }
   else
   {
      printf("fimull_3 ... failed\n");
   }

   return;
}

static void fimull_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fimull %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 108215202237.464F) )
      {
         printf("fimull_4 ... ok\n");
      }
      else
      {
         printf("fimull_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 108215202237.464F);
      }
   }
   else
   {
      printf("fimull_4 ... failed\n");
   }

   return;
}

static void fimull_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { 654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fimull %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 807803614845.297) )
      {
         printf("fimull_5 ... ok\n");
      }
      else
      {
         printf("fimull_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 807803614845.297);
      }
   }
   else
   {
      printf("fimull_5 ... failed\n");
   }

   return;
}

static void fimull_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { 654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fimull %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -807803614845.297) )
      {
         printf("fimull_6 ... ok\n");
      }
      else
      {
         printf("fimull_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -807803614845.297);
      }
   }
   else
   {
      printf("fimull_6 ... failed\n");
   }

   return;
}

static void fimull_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { -654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fimull %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -807803614845.297) )
      {
         printf("fimull_7 ... ok\n");
      }
      else
      {
         printf("fimull_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -807803614845.297);
      }
   }
   else
   {
      printf("fimull_7 ... failed\n");
   }

   return;
}

static void fimull_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { -654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fimull %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 807803614845.297) )
      {
         printf("fimull_8 ... ok\n");
      }
      else
      {
         printf("fimull_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 807803614845.297);
      }
   }
   else
   {
      printf("fimull_8 ... failed\n");
   }

   return;
}

static void frndint_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1235.0F) )
      {
         printf("frndint_1 ... ok\n");
      }
      else
      {
         printf("frndint_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1235.0F);
      }
   }
   else
   {
      printf("frndint_1 ... failed\n");
   }

   return;
}

static void frndint_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -1235.0F) )
      {
         printf("frndint_2 ... ok\n");
      }
      else
      {
         printf("frndint_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -1235.0F);
      }
   }
   else
   {
      printf("frndint_2 ... failed\n");
   }

   return;
}

static void frndint_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.0F) )
      {
         printf("frndint_3 ... ok\n");
      }
      else
      {
         printf("frndint_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.0F);
      }
   }
   else
   {
      printf("frndint_3 ... failed\n");
   }

   return;
}

static void frndint_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -1235.0F) )
      {
         printf("frndint_4 ... ok\n");
      }
      else
      {
         printf("frndint_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -1235.0F);
      }
   }
   else
   {
      printf("frndint_4 ... failed\n");
   }

   return;
}

static void frndint_5(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1235.0F) )
      {
         printf("frndint_5 ... ok\n");
      }
      else
      {
         printf("frndint_5 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1235.0F);
      }
   }
   else
   {
      printf("frndint_5 ... failed\n");
   }

   return;
}

static void frndint_6(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -1234.0F) )
      {
         printf("frndint_6 ... ok\n");
      }
      else
      {
         printf("frndint_6 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -1234.0F);
      }
   }
   else
   {
      printf("frndint_6 ... failed\n");
   }

   return;
}

static void frndint_7(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 1234.0F) )
      {
         printf("frndint_7 ... ok\n");
      }
      else
      {
         printf("frndint_7 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.0F);
      }
   }
   else
   {
      printf("frndint_7 ... failed\n");
   }

   return;
}

static void frndint_8(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -1234.0F) )
      {
         printf("frndint_8 ... ok\n");
      }
      else
      {
         printf("frndint_8 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -1234.0F);
      }
   }
   else
   {
      printf("frndint_8 ... failed\n");
   }

   return;
}

static void frndint_9(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234568.0) )
      {
         printf("frndint_9 ... ok\n");
      }
      else
      {
         printf("frndint_9 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234568.0);
      }
   }
   else
   {
      printf("frndint_9 ... failed\n");
   }

   return;
}

static void frndint_10(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0000, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -1234568.0) )
      {
         printf("frndint_10 ... ok\n");
      }
      else
      {
         printf("frndint_10 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1234568.0);
      }
   }
   else
   {
      printf("frndint_10 ... failed\n");
   }

   return;
}

static void frndint_11(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.0) )
      {
         printf("frndint_11 ... ok\n");
      }
      else
      {
         printf("frndint_11 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.0);
      }
   }
   else
   {
      printf("frndint_11 ... failed\n");
   }

   return;
}

static void frndint_12(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0400, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -1234568.0) )
      {
         printf("frndint_12 ... ok\n");
      }
      else
      {
         printf("frndint_12 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1234568.0);
      }
   }
   else
   {
      printf("frndint_12 ... failed\n");
   }

   return;
}

static void frndint_13(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234568.0) )
      {
         printf("frndint_13 ... ok\n");
      }
      else
      {
         printf("frndint_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234568.0);
      }
   }
   else
   {
      printf("frndint_13 ... failed\n");
   }

   return;
}

static void frndint_14(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0800, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -1234567.0) )
      {
         printf("frndint_14 ... ok\n");
      }
      else
      {
         printf("frndint_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1234567.0);
      }
   }
   else
   {
      printf("frndint_14 ... failed\n");
   }

   return;
}

static void frndint_15(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1234567.0) )
      {
         printf("frndint_15 ... ok\n");
      }
      else
      {
         printf("frndint_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1234567.0);
      }
   }
   else
   {
      printf("frndint_15 ... failed\n");
   }

   return;
}

static void frndint_16(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "subq $2, %%rsp\n"
         "fstcw (%%rsp)\n"
         "andw $0xf3ff, (%%rsp)\n"
         "orw $0x0c00, (%%rsp)\n"
         "fldcw (%%rsp)\n"
         "addq $2, %%rsp\n"
         "frndint\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -1234567.0) )
      {
         printf("frndint_16 ... ok\n");
      }
      else
      {
         printf("frndint_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1234567.0);
      }
   }
   else
   {
      printf("frndint_16 ... failed\n");
   }

   return;
}

static void fsubs_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fsubs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fsubs_1 ... ok\n");
      }
      else
      {
         printf("fsubs_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fsubs_1 ... failed\n");
   }

   return;
}

static void fsubs_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fsubs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9990F) )
      {
         printf("fsubs_2 ... ok\n");
      }
      else
      {
         printf("fsubs_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9990F);
      }
   }
   else
   {
      printf("fsubs_2 ... failed\n");
   }

   return;
}

static void fsubs_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fsubs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("fsubs_3 ... ok\n");
      }
      else
      {
         printf("fsubs_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("fsubs_3 ... failed\n");
   }

   return;
}

static void fsubs_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fsubs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fsubs_4 ... ok\n");
      }
      else
      {
         printf("fsubs_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fsubs_4 ... failed\n");
   }

   return;
}

static void fsubl_1(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fsubl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("fsubl_1 ... ok\n");
      }
      else
      {
         printf("fsubl_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("fsubl_1 ... failed\n");
   }

   return;
}

static void fsubl_2(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fsubl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("fsubl_2 ... ok\n");
      }
      else
      {
         printf("fsubl_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("fsubl_2 ... failed\n");
   }

   return;
}

static void fsubl_3(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fsubl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("fsubl_3 ... ok\n");
      }
      else
      {
         printf("fsubl_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("fsubl_3 ... failed\n");
   }

   return;
}

static void fsubl_4(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t arg0 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fsubl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("fsubl_4 ... ok\n");
      }
      else
      {
         printf("fsubl_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("fsubl_4 ... failed\n");
   }

   return;
}

static void fsub_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsub %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fsub_1 ... ok\n");
      }
      else
      {
         printf("fsub_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fsub_1 ... failed\n");
   }

   return;
}

static void fsub_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsub %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("fsub_2 ... ok\n");
      }
      else
      {
         printf("fsub_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("fsub_2 ... failed\n");
   }

   return;
}

static void fsub_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsub %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("fsub_3 ... ok\n");
      }
      else
      {
         printf("fsub_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("fsub_3 ... failed\n");
   }

   return;
}

static void fsub_4(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsub %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fsub_4 ... ok\n");
      }
      else
      {
         printf("fsub_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fsub_4 ... failed\n");
   }

   return;
}

static void fsub_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsub %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("fsub_5 ... ok\n");
      }
      else
      {
         printf("fsub_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("fsub_5 ... failed\n");
   }

   return;
}

static void fsub_6(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsub %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("fsub_6 ... ok\n");
      }
      else
      {
         printf("fsub_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("fsub_6 ... failed\n");
   }

   return;
}

static void fsub_7(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsub %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("fsub_7 ... ok\n");
      }
      else
      {
         printf("fsub_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("fsub_7 ... failed\n");
   }

   return;
}

static void fsub_8(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsub %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("fsub_8 ... ok\n");
      }
      else
      {
         printf("fsub_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("fsub_8 ... failed\n");
   }

   return;
}

static void fsub_9(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fsub %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fsub_9 ... ok\n");
      }
      else
      {
         printf("fsub_9 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fsub_9 ... failed\n");
   }

   return;
}

static void fsub_10(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fsub %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("fsub_10 ... ok\n");
      }
      else
      {
         printf("fsub_10 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("fsub_10 ... failed\n");
   }

   return;
}

static void fsub_11(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fsub %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("fsub_11 ... ok\n");
      }
      else
      {
         printf("fsub_11 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("fsub_11 ... failed\n");
   }

   return;
}

static void fsub_12(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fsub %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fsub_12 ... ok\n");
      }
      else
      {
         printf("fsub_12 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fsub_12 ... failed\n");
   }

   return;
}

static void fsub_13(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fsub %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("fsub_13 ... ok\n");
      }
      else
      {
         printf("fsub_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("fsub_13 ... failed\n");
   }

   return;
}

static void fsub_14(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fsub %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("fsub_14 ... ok\n");
      }
      else
      {
         printf("fsub_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("fsub_14 ... failed\n");
   }

   return;
}

static void fsub_15(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fsub %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("fsub_15 ... ok\n");
      }
      else
      {
         printf("fsub_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("fsub_15 ... failed\n");
   }

   return;
}

static void fsub_16(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fsub %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("fsub_16 ... ok\n");
      }
      else
      {
         printf("fsub_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("fsub_16 ... failed\n");
   }

   return;
}

static void fsubp_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsubp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fsubp_1 ... ok\n");
      }
      else
      {
         printf("fsubp_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fsubp_1 ... failed\n");
   }

   return;
}

static void fsubp_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsubp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("fsubp_2 ... ok\n");
      }
      else
      {
         printf("fsubp_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("fsubp_2 ... failed\n");
   }

   return;
}

static void fsubp_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsubp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("fsubp_3 ... ok\n");
      }
      else
      {
         printf("fsubp_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("fsubp_3 ... failed\n");
   }

   return;
}

static void fsubp_4(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsubp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fsubp_4 ... ok\n");
      }
      else
      {
         printf("fsubp_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fsubp_4 ... failed\n");
   }

   return;
}

static void fsubp_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsubp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("fsubp_5 ... ok\n");
      }
      else
      {
         printf("fsubp_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("fsubp_5 ... failed\n");
   }

   return;
}

static void fsubp_6(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsubp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("fsubp_6 ... ok\n");
      }
      else
      {
         printf("fsubp_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("fsubp_6 ... failed\n");
   }

   return;
}

static void fsubp_7(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsubp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("fsubp_7 ... ok\n");
      }
      else
      {
         printf("fsubp_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("fsubp_7 ... failed\n");
   }

   return;
}

static void fsubp_8(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsubp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("fsubp_8 ... ok\n");
      }
      else
      {
         printf("fsubp_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("fsubp_8 ... failed\n");
   }

   return;
}

static void fsubp_9(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fsubp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fsubp_9 ... ok\n");
      }
      else
      {
         printf("fsubp_9 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fsubp_9 ... failed\n");
   }

   return;
}

static void fsubp_10(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fsubp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("fsubp_10 ... ok\n");
      }
      else
      {
         printf("fsubp_10 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("fsubp_10 ... failed\n");
   }

   return;
}

static void fsubp_11(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fsubp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("fsubp_11 ... ok\n");
      }
      else
      {
         printf("fsubp_11 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("fsubp_11 ... failed\n");
   }

   return;
}

static void fsubp_12(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fsubp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fsubp_12 ... ok\n");
      }
      else
      {
         printf("fsubp_12 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fsubp_12 ... failed\n");
   }

   return;
}

static void fsubp_13(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fsubp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("fsubp_13 ... ok\n");
      }
      else
      {
         printf("fsubp_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("fsubp_13 ... failed\n");
   }

   return;
}

static void fsubp_14(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fsubp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("fsubp_14 ... ok\n");
      }
      else
      {
         printf("fsubp_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("fsubp_14 ... failed\n");
   }

   return;
}

static void fsubp_15(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fsubp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("fsubp_15 ... ok\n");
      }
      else
      {
         printf("fsubp_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("fsubp_15 ... failed\n");
   }

   return;
}

static void fsubp_16(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fsubp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("fsubp_16 ... ok\n");
      }
      else
      {
         printf("fsubp_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("fsubp_16 ... failed\n");
   }

   return;
}

static void fisubs_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -3086.4322F) )
      {
         printf("fisubs_1 ... ok\n");
      }
      else
      {
         printf("fisubs_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -3086.4322F);
      }
   }
   else
   {
      printf("fisubs_1 ... failed\n");
   }

   return;
}

static void fisubs_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -5555.5678F) )
      {
         printf("fisubs_2 ... ok\n");
      }
      else
      {
         printf("fisubs_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -5555.5678F);
      }
   }
   else
   {
      printf("fisubs_2 ... failed\n");
   }

   return;
}

static void fisubs_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 5555.5678F) )
      {
         printf("fisubs_3 ... ok\n");
      }
      else
      {
         printf("fisubs_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 5555.5678F);
      }
   }
   else
   {
      printf("fisubs_3 ... failed\n");
   }

   return;
}

static void fisubs_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 3086.4322F) )
      {
         printf("fisubs_4 ... ok\n");
      }
      else
      {
         printf("fisubs_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 3086.4322F);
      }
   }
   else
   {
      printf("fisubs_4 ... failed\n");
   }

   return;
}

static void fisubs_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1230246.7654321) )
      {
         printf("fisubs_5 ... ok\n");
      }
      else
      {
         printf("fisubs_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1230246.7654321);
      }
   }
   else
   {
      printf("fisubs_5 ... failed\n");
   }

   return;
}

static void fisubs_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -1238888.7654321) )
      {
         printf("fisubs_6 ... ok\n");
      }
      else
      {
         printf("fisubs_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1238888.7654321);
      }
   }
   else
   {
      printf("fisubs_6 ... failed\n");
   }

   return;
}

static void fisubs_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1238888.7654321) )
      {
         printf("fisubs_7 ... ok\n");
      }
      else
      {
         printf("fisubs_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1238888.7654321);
      }
   }
   else
   {
      printf("fisubs_7 ... failed\n");
   }

   return;
}

static void fisubs_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -1230246.7654321) )
      {
         printf("fisubs_8 ... ok\n");
      }
      else
      {
         printf("fisubs_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1230246.7654321);
      }
   }
   else
   {
      printf("fisubs_8 ... failed\n");
   }

   return;
}

static void fisubl_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -87653086.4322F) )
      {
         printf("fisubl_1 ... ok\n");
      }
      else
      {
         printf("fisubl_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -87653086.4322F);
      }
   }
   else
   {
      printf("fisubl_1 ... failed\n");
   }

   return;
}

static void fisubl_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -87655555.5678F) )
      {
         printf("fisubl_2 ... ok\n");
      }
      else
      {
         printf("fisubl_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -87655555.5678F);
      }
   }
   else
   {
      printf("fisubl_2 ... failed\n");
   }

   return;
}

static void fisubl_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 87655555.5678F) )
      {
         printf("fisubl_3 ... ok\n");
      }
      else
      {
         printf("fisubl_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 87655555.5678F);
      }
   }
   else
   {
      printf("fisubl_3 ... failed\n");
   }

   return;
}

static void fisubl_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 87653086.4322F) )
      {
         printf("fisubl_4 ... ok\n");
      }
      else
      {
         printf("fisubl_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 87653086.4322F);
      }
   }
   else
   {
      printf("fisubl_4 ... failed\n");
   }

   return;
}

static void fisubl_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -86419753.2345679) )
      {
         printf("fisubl_5 ... ok\n");
      }
      else
      {
         printf("fisubl_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -86419753.2345679);
      }
   }
   else
   {
      printf("fisubl_5 ... failed\n");
   }

   return;
}

static void fisubl_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -88888888.7654321) )
      {
         printf("fisubl_6 ... ok\n");
      }
      else
      {
         printf("fisubl_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -88888888.7654321);
      }
   }
   else
   {
      printf("fisubl_6 ... failed\n");
   }

   return;
}

static void fisubl_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 88888888.7654321) )
      {
         printf("fisubl_7 ... ok\n");
      }
      else
      {
         printf("fisubl_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 88888888.7654321);
      }
   }
   else
   {
      printf("fisubl_7 ... failed\n");
   }

   return;
}

static void fisubl_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 86419753.2345679) )
      {
         printf("fisubl_8 ... ok\n");
      }
      else
      {
         printf("fisubl_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 86419753.2345679);
      }
   }
   else
   {
      printf("fisubl_8 ... failed\n");
   }

   return;
}

static void fsubrs_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fsubrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fsubrs_1 ... ok\n");
      }
      else
      {
         printf("fsubrs_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fsubrs_1 ... failed\n");
   }

   return;
}

static void fsubrs_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fsubrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9990F) )
      {
         printf("fsubrs_2 ... ok\n");
      }
      else
      {
         printf("fsubrs_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9990F);
      }
   }
   else
   {
      printf("fsubrs_2 ... failed\n");
   }

   return;
}

static void fsubrs_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fsubrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("fsubrs_3 ... ok\n");
      }
      else
      {
         printf("fsubrs_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("fsubrs_3 ... failed\n");
   }

   return;
}

static void fsubrs_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fsubrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fsubrs_4 ... ok\n");
      }
      else
      {
         printf("fsubrs_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fsubrs_4 ... failed\n");
   }

   return;
}

static void fsubrl_1(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fsubrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("fsubrl_1 ... ok\n");
      }
      else
      {
         printf("fsubrl_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("fsubrl_1 ... failed\n");
   }

   return;
}

static void fsubrl_2(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fsubrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("fsubrl_2 ... ok\n");
      }
      else
      {
         printf("fsubrl_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("fsubrl_2 ... failed\n");
   }

   return;
}

static void fsubrl_3(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fsubrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("fsubrl_3 ... ok\n");
      }
      else
      {
         printf("fsubrl_3 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("fsubrl_3 ... failed\n");
   }

   return;
}

static void fsubrl_4(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t arg0 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fsubrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("fsubrl_4 ... ok\n");
      }
      else
      {
         printf("fsubrl_4 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("fsubrl_4 ... failed\n");
   }

   return;
}

static void fsubr_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsubr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fsubr_1 ... ok\n");
      }
      else
      {
         printf("fsubr_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fsubr_1 ... failed\n");
   }

   return;
}

static void fsubr_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsubr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("fsubr_2 ... ok\n");
      }
      else
      {
         printf("fsubr_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("fsubr_2 ... failed\n");
   }

   return;
}

static void fsubr_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsubr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("fsubr_3 ... ok\n");
      }
      else
      {
         printf("fsubr_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("fsubr_3 ... failed\n");
   }

   return;
}

static void fsubr_4(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsubr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fsubr_4 ... ok\n");
      }
      else
      {
         printf("fsubr_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fsubr_4 ... failed\n");
   }

   return;
}

static void fsubr_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsubr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("fsubr_5 ... ok\n");
      }
      else
      {
         printf("fsubr_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("fsubr_5 ... failed\n");
   }

   return;
}

static void fsubr_6(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsubr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("fsubr_6 ... ok\n");
      }
      else
      {
         printf("fsubr_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("fsubr_6 ... failed\n");
   }

   return;
}

static void fsubr_7(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsubr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("fsubr_7 ... ok\n");
      }
      else
      {
         printf("fsubr_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("fsubr_7 ... failed\n");
   }

   return;
}

static void fsubr_8(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsubr %%st(0), %%st(2)\n"
         "fincstp\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("fsubr_8 ... ok\n");
      }
      else
      {
         printf("fsubr_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("fsubr_8 ... failed\n");
   }

   return;
}

static void fsubr_9(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fsubr %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fsubr_9 ... ok\n");
      }
      else
      {
         printf("fsubr_9 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fsubr_9 ... failed\n");
   }

   return;
}

static void fsubr_10(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fsubr %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("fsubr_10 ... ok\n");
      }
      else
      {
         printf("fsubr_10 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("fsubr_10 ... failed\n");
   }

   return;
}

static void fsubr_11(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fsubr %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("fsubr_11 ... ok\n");
      }
      else
      {
         printf("fsubr_11 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("fsubr_11 ... failed\n");
   }

   return;
}

static void fsubr_12(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fldz\n"
         "flds %2\n"
         "fsubr %%st(2), %%st(0)\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fsubr_12 ... ok\n");
      }
      else
      {
         printf("fsubr_12 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fsubr_12 ... failed\n");
   }

   return;
}

static void fsubr_13(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fsubr %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("fsubr_13 ... ok\n");
      }
      else
      {
         printf("fsubr_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("fsubr_13 ... failed\n");
   }

   return;
}

static void fsubr_14(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fsubr %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("fsubr_14 ... ok\n");
      }
      else
      {
         printf("fsubr_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("fsubr_14 ... failed\n");
   }

   return;
}

static void fsubr_15(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fsubr %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("fsubr_15 ... ok\n");
      }
      else
      {
         printf("fsubr_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("fsubr_15 ... failed\n");
   }

   return;
}

static void fsubr_16(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fldz\n"
         "fldl %2\n"
         "fsubr %%st(2), %%st(0)\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("fsubr_16 ... ok\n");
      }
      else
      {
         printf("fsubr_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("fsubr_16 ... failed\n");
   }

   return;
}

static void fsubrp_1(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsubrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fsubrp_1 ... ok\n");
      }
      else
      {
         printf("fsubrp_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fsubrp_1 ... failed\n");
   }

   return;
}

static void fsubrp_2(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsubrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("fsubrp_2 ... ok\n");
      }
      else
      {
         printf("fsubrp_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("fsubrp_2 ... failed\n");
   }

   return;
}

static void fsubrp_3(void)
{
   reg64_t arg0 = { .ps = { 1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsubrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("fsubrp_3 ... ok\n");
      }
      else
      {
         printf("fsubrp_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("fsubrp_3 ... failed\n");
   }

   return;
}

static void fsubrp_4(void)
{
   reg64_t arg0 = { .ps = { -1234.5678F } };
   reg64_t arg1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "fldz\n"
         "flds %1\n"
         "fsubrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fsubrp_4 ... ok\n");
      }
      else
      {
         printf("fsubrp_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fsubrp_4 ... failed\n");
   }

   return;
}

static void fsubrp_5(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsubrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("fsubrp_5 ... ok\n");
      }
      else
      {
         printf("fsubrp_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("fsubrp_5 ... failed\n");
   }

   return;
}

static void fsubrp_6(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsubrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("fsubrp_6 ... ok\n");
      }
      else
      {
         printf("fsubrp_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("fsubrp_6 ... failed\n");
   }

   return;
}

static void fsubrp_7(void)
{
   reg64_t arg0 = { .pd = { 1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsubrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("fsubrp_7 ... ok\n");
      }
      else
      {
         printf("fsubrp_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("fsubrp_7 ... failed\n");
   }

   return;
}

static void fsubrp_8(void)
{
   reg64_t arg0 = { .pd = { -1234567.7654321 } };
   reg64_t arg1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldz\n"
         "fldl %1\n"
         "fsubrp %%st(0), %%st(2)\n"
         "fincstp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("fsubrp_8 ... ok\n");
      }
      else
      {
         printf("fsubrp_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("fsubrp_8 ... failed\n");
   }

   return;
}

static void fsubrp_9(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fsubrp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 7530.8643F) )
      {
         printf("fsubrp_9 ... ok\n");
      }
      else
      {
         printf("fsubrp_9 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7530.8643F);
      }
   }
   else
   {
      printf("fsubrp_9 ... failed\n");
   }

   return;
}

static void fsubrp_10(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { 8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fsubrp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 9999.9999F) )
      {
         printf("fsubrp_10 ... ok\n");
      }
      else
      {
         printf("fsubrp_10 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 9999.9999F);
      }
   }
   else
   {
      printf("fsubrp_10 ... failed\n");
   }

   return;
}

static void fsubrp_11(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t preset1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fsubrp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -9999.9999F) )
      {
         printf("fsubrp_11 ... ok\n");
      }
      else
      {
         printf("fsubrp_11 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -9999.9999F);
      }
   }
   else
   {
      printf("fsubrp_11 ... failed\n");
   }

   return;
}

static void fsubrp_12(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg64_t preset1 = { .ps = { -8765.4321F } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %2\n"
         "flds %1\n"
         "fsubrp\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -7530.8643F) )
      {
         printf("fsubrp_12 ... ok\n");
      }
      else
      {
         printf("fsubrp_12 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -7530.8643F);
      }
   }
   else
   {
      printf("fsubrp_12 ... failed\n");
   }

   return;
}

static void fsubrp_13(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fsubrp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 6419753.3580246) )
      {
         printf("fsubrp_13 ... ok\n");
      }
      else
      {
         printf("fsubrp_13 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 6419753.3580246);
      }
   }
   else
   {
      printf("fsubrp_13 ... failed\n");
   }

   return;
}

static void fsubrp_14(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fsubrp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 8888888.8888888) )
      {
         printf("fsubrp_14 ... ok\n");
      }
      else
      {
         printf("fsubrp_14 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 8888888.8888888);
      }
   }
   else
   {
      printf("fsubrp_14 ... failed\n");
   }

   return;
}

static void fsubrp_15(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t preset1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fsubrp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -8888888.8888888) )
      {
         printf("fsubrp_15 ... ok\n");
      }
      else
      {
         printf("fsubrp_15 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -8888888.8888888);
      }
   }
   else
   {
      printf("fsubrp_15 ... failed\n");
   }

   return;
}

static void fsubrp_16(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg64_t preset1 = { .pd = { -7654321.1234567 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %2\n"
         "fldl %1\n"
         "fsubrp\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (preset1), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -6419753.3580246) )
      {
         printf("fsubrp_16 ... ok\n");
      }
      else
      {
         printf("fsubrp_16 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -6419753.3580246);
      }
   }
   else
   {
      printf("fsubrp_16 ... failed\n");
   }

   return;
}

static void fisubrs_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 3086.4322F) )
      {
         printf("fisubrs_1 ... ok\n");
      }
      else
      {
         printf("fisubrs_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 3086.4322F);
      }
   }
   else
   {
      printf("fisubrs_1 ... failed\n");
   }

   return;
}

static void fisubrs_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 5555.5678F) )
      {
         printf("fisubrs_2 ... ok\n");
      }
      else
      {
         printf("fisubrs_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 5555.5678F);
      }
   }
   else
   {
      printf("fisubrs_2 ... failed\n");
   }

   return;
}

static void fisubrs_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -5555.5678F) )
      {
         printf("fisubrs_3 ... ok\n");
      }
      else
      {
         printf("fisubrs_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -5555.5678F);
      }
   }
   else
   {
      printf("fisubrs_3 ... failed\n");
   }

   return;
}

static void fisubrs_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubrs %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -3086.4322F) )
      {
         printf("fisubrs_4 ... ok\n");
      }
      else
      {
         printf("fisubrs_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -3086.4322F);
      }
   }
   else
   {
      printf("fisubrs_4 ... failed\n");
   }

   return;
}

static void fisubrs_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubrs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -1230246.7654321) )
      {
         printf("fisubrs_5 ... ok\n");
      }
      else
      {
         printf("fisubrs_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1230246.7654321);
      }
   }
   else
   {
      printf("fisubrs_5 ... failed\n");
   }

   return;
}

static void fisubrs_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg16_t arg0 = { .sw = { 4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubrs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1238888.7654321) )
      {
         printf("fisubrs_6 ... ok\n");
      }
      else
      {
         printf("fisubrs_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1238888.7654321);
      }
   }
   else
   {
      printf("fisubrs_6 ... failed\n");
   }

   return;
}

static void fisubrs_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubrs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -1238888.7654321) )
      {
         printf("fisubrs_7 ... ok\n");
      }
      else
      {
         printf("fisubrs_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1238888.7654321);
      }
   }
   else
   {
      printf("fisubrs_7 ... failed\n");
   }

   return;
}

static void fisubrs_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg16_t arg0 = { .sw = { -4321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubrs %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 1230246.7654321) )
      {
         printf("fisubrs_8 ... ok\n");
      }
      else
      {
         printf("fisubrs_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 1230246.7654321);
      }
   }
   else
   {
      printf("fisubrs_8 ... failed\n");
   }

   return;
}

static void fisubrl_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubrl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 87655555.5678F) )
      {
         printf("fisubrl_1 ... ok\n");
      }
      else
      {
         printf("fisubrl_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 87655555.5678F);
      }
   }
   else
   {
      printf("fisubrl_1 ... failed\n");
   }

   return;
}

static void fisubrl_2(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubrl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 87653086.4322F) )
      {
         printf("fisubrl_2 ... ok\n");
      }
      else
      {
         printf("fisubrl_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 87653086.4322F);
      }
   }
   else
   {
      printf("fisubrl_2 ... failed\n");
   }

   return;
}

static void fisubrl_3(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubrl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -87653086.4322F) )
      {
         printf("fisubrl_3 ... ok\n");
      }
      else
      {
         printf("fisubrl_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -87653086.4322F);
      }
   }
   else
   {
      printf("fisubrl_3 ... failed\n");
   }

   return;
}

static void fisubrl_4(void)
{
   reg64_t preset0 = { .ps = { -1234.5678F } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %1\n"
         "fisubrl %2\n"
         "fstps %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], -87655555.5678F) )
      {
         printf("fisubrl_4 ... ok\n");
      }
      else
      {
         printf("fisubrl_4 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -87655555.5678F);
      }
   }
   else
   {
      printf("fisubrl_4 ... failed\n");
   }

   return;
}

static void fisubrl_5(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 86419753.2345679) )
      {
         printf("fisubrl_5 ... ok\n");
      }
      else
      {
         printf("fisubrl_5 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 86419753.2345679);
      }
   }
   else
   {
      printf("fisubrl_5 ... failed\n");
   }

   return;
}

static void fisubrl_6(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { 87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 88888888.7654321) )
      {
         printf("fisubrl_6 ... ok\n");
      }
      else
      {
         printf("fisubrl_6 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 88888888.7654321);
      }
   }
   else
   {
      printf("fisubrl_6 ... failed\n");
   }

   return;
}

static void fisubrl_7(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -88888888.7654321) )
      {
         printf("fisubrl_7 ... ok\n");
      }
      else
      {
         printf("fisubrl_7 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -88888888.7654321);
      }
   }
   else
   {
      printf("fisubrl_7 ... failed\n");
   }

   return;
}

static void fisubrl_8(void)
{
   reg64_t preset0 = { .pd = { -1234567.7654321 } };
   reg32_t arg0 = { .sd = { -87654321 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %1\n"
         "fisubrl %2\n"
         "fstpl %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], -86419753.2345679) )
      {
         printf("fisubrl_8 ... ok\n");
      }
      else
      {
         printf("fisubrl_8 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -86419753.2345679);
      }
   }
   else
   {
      printf("fisubrl_8 ... failed\n");
   }

   return;
}

static void fxch_1(void)
{
   reg64_t preset0 = { .ps = { 1234.5678F } };
   reg64_t arg0 = { .ps = { 8765.4321F } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "flds %3\n"
         "fldz\n"
         "flds %2\n"
         "fxch %%st(2)\n"
         "fstps %0\n"
         "fincstp\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_float(result0.ps[0], 8765.4321F) &&
          eq_float(result1.ps[0], 1234.5678F) )
      {
         printf("fxch_1 ... ok\n");
      }
      else
      {
         printf("fxch_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 8765.4321F);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1234.5678F);
      }
   }
   else
   {
      printf("fxch_1 ... failed\n");
   }

   return;
}

static void fxch_2(void)
{
   reg64_t preset0 = { .pd = { 1234567.7654321 } };
   reg64_t arg0 = { .pd = { 7654321.1234567 } };
   reg64_t result0;
   reg64_t result1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "fldl %3\n"
         "fldz\n"
         "fldl %2\n"
         "fxch %%st(2)\n"
         "fstpl %0\n"
         "fincstp\n"
         "fstpl %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (arg0), "m" (state[0])

      );

      if (eq_double(result0.pd[0], 7654321.1234567) &&
          eq_double(result1.pd[0], 1234567.7654321) )
      {
         printf("fxch_2 ... ok\n");
      }
      else
      {
         printf("fxch_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 7654321.1234567);
         printf("  result1.pd[0] = %.16g (expected %.16g)\n", result1.pd[0], 1234567.7654321);
      }
   }
   else
   {
      printf("fxch_2 ... failed\n");
   }

   return;
}

int main(int argc, char **argv)
{
   signal(SIGILL, handle_sigill);

   fabs_1();
   fabs_2();
   fabs_3();
   fabs_4();
   fadds_1();
   fadds_2();
   fadds_3();
   fadds_4();
   faddl_1();
   faddl_2();
   faddl_3();
   faddl_4();
   fadd_1();
   fadd_2();
   fadd_3();
   fadd_4();
   fadd_5();
   fadd_6();
   fadd_7();
   fadd_8();
   fadd_9();
   fadd_10();
   fadd_11();
   fadd_12();
   fadd_13();
   fadd_14();
   fadd_15();
   fadd_16();
   faddp_1();
   faddp_2();
   faddp_3();
   faddp_4();
   faddp_5();
   faddp_6();
   faddp_7();
   faddp_8();
   faddp_9();
   faddp_10();
   faddp_11();
   faddp_12();
   faddp_13();
   faddp_14();
   faddp_15();
   faddp_16();
   fiadds_1();
   fiadds_2();
   fiadds_3();
   fiadds_4();
   fiadds_5();
   fiadds_6();
   fiadds_7();
   fiadds_8();
   fiaddl_1();
   fiaddl_2();
   fiaddl_3();
   fiaddl_4();
   fiaddl_5();
   fiaddl_6();
   fiaddl_7();
   fiaddl_8();
   fcoms_1();
   fcoms_2();
   fcoms_3();
   fcoml_1();
   fcoml_2();
   fcoml_3();
   fcomps_1();
   fcomps_2();
   fcomps_3();
   fcompl_1();
   fcompl_2();
   fcompl_3();
   fcomi_1();
   fcomi_2();
   fcomi_3();
   fcomi_4();
   fcomi_5();
   fcomi_6();
   fcomip_1();
   fcomip_2();
   fcomip_3();
   fcomip_4();
   fcomip_5();
   fcomip_6();
   fucomi_1();
   fucomi_2();
   fucomi_3();
   fucomi_4();
   fucomi_5();
   fucomi_6();
   fucomip_1();
   fucomip_2();
   fucomip_3();
   fucomip_4();
   fucomip_5();
   fucomip_6();
   fchs_1();
   fchs_2();
   fchs_3();
   fchs_4();
   fdivs_1();
   fdivs_2();
   fdivs_3();
   fdivs_4();
   fdivl_1();
   fdivl_2();
   fdivl_3();
   fdivl_4();
   fdiv_1();
   fdiv_2();
   fdiv_3();
   fdiv_4();
   fdiv_5();
   fdiv_6();
   fdiv_7();
   fdiv_8();
   fdiv_9();
   fdiv_10();
   fdiv_11();
   fdiv_12();
   fdiv_13();
   fdiv_14();
   fdiv_15();
   fdiv_16();
   fdivp_1();
   fdivp_2();
   fdivp_3();
   fdivp_4();
   fdivp_5();
   fdivp_6();
   fdivp_7();
   fdivp_8();
   fdivp_9();
   fdivp_10();
   fdivp_11();
   fdivp_12();
   fdivp_13();
   fdivp_14();
   fdivp_15();
   fdivp_16();
   fidivs_1();
   fidivs_2();
   fidivs_3();
   fidivs_4();
   fidivs_5();
   fidivs_6();
   fidivs_7();
   fidivs_8();
   fidivl_1();
   fidivl_2();
   fidivl_3();
   fidivl_4();
   fidivl_5();
   fidivl_6();
   fidivl_7();
   fidivl_8();
   fdivrs_1();
   fdivrs_2();
   fdivrs_3();
   fdivrs_4();
   fdivrl_1();
   fdivrl_2();
   fdivrl_3();
   fdivrl_4();
   fdivr_1();
   fdivr_2();
   fdivr_3();
   fdivr_4();
   fdivr_5();
   fdivr_6();
   fdivr_7();
   fdivr_8();
   fdivr_9();
   fdivr_10();
   fdivr_11();
   fdivr_12();
   fdivr_13();
   fdivr_14();
   fdivr_15();
   fdivr_16();
   fdivrp_1();
   fdivrp_2();
   fdivrp_3();
   fdivrp_4();
   fdivrp_5();
   fdivrp_6();
   fdivrp_7();
   fdivrp_8();
   fdivrp_9();
   fdivrp_10();
   fdivrp_11();
   fdivrp_12();
   fdivrp_13();
   fdivrp_14();
   fdivrp_15();
   fdivrp_16();
   fidivrs_1();
   fidivrs_2();
   fidivrs_3();
   fidivrs_4();
   fidivrs_5();
   fidivrs_6();
   fidivrs_7();
   fidivrs_8();
   fidivrl_1();
   fidivrl_2();
   fidivrl_3();
   fidivrl_4();
   fidivrl_5();
   fidivrl_6();
   fidivrl_7();
   fidivrl_8();
   filds_1();
   filds_2();
   filds_3();
   filds_4();
   fildl_1();
   fildl_2();
   fildl_3();
   fildl_4();
   fildq_1();
   fildq_2();
   fildq_3();
   fildq_4();
   fists_1();
   fists_2();
   fists_3();
   fists_4();
   fists_5();
   fists_6();
   fists_7();
   fists_8();
   fistl_1();
   fistl_2();
   fistl_3();
   fistl_4();
   fistl_5();
   fistl_6();
   fistl_7();
   fistl_8();
   fistps_1();
   fistps_2();
   fistps_3();
   fistps_4();
   fistps_5();
   fistps_6();
   fistps_7();
   fistps_8();
   fistpl_1();
   fistpl_2();
   fistpl_3();
   fistpl_4();
   fistpl_5();
   fistpl_6();
   fistpl_7();
   fistpl_8();
   fistpq_1();
   fistpq_2();
   fistpq_3();
   fistpq_4();
   fistpq_5();
   fistpq_6();
   fistpq_7();
   fistpq_8();
   flds_1();
   flds_2();
   fldl_1();
   fldl_2();
   fld_1();
   fld_2();
   fld_3();
   fld1_1();
   fldl2t_1();
   fldl2e_1();
   fldpi_1();
   fldlg2_1();
   fldln2_1();
   fldz_1();
   fmuls_1();
   fmuls_2();
   fmuls_3();
   fmuls_4();
   fmull_1();
   fmull_2();
   fmull_3();
   fmull_4();
   fmul_1();
   fmul_2();
   fmul_3();
   fmul_4();
   fmul_5();
   fmul_6();
   fmul_7();
   fmul_8();
   fmul_9();
   fmul_10();
   fmul_11();
   fmul_12();
   fmul_13();
   fmul_14();
   fmul_15();
   fmul_16();
   fmulp_1();
   fmulp_2();
   fmulp_3();
   fmulp_4();
   fmulp_5();
   fmulp_6();
   fmulp_7();
   fmulp_8();
   fmulp_9();
   fmulp_10();
   fmulp_11();
   fmulp_12();
   fmulp_13();
   fmulp_14();
   fmulp_15();
   fmulp_16();
   fimuls_1();
   fimuls_2();
   fimuls_3();
   fimuls_4();
   fimuls_5();
   fimuls_6();
   fimuls_7();
   fimuls_8();
   fimull_1();
   fimull_2();
   fimull_3();
   fimull_4();
   fimull_5();
   fimull_6();
   fimull_7();
   fimull_8();
   frndint_1();
   frndint_2();
   frndint_3();
   frndint_4();
   frndint_5();
   frndint_6();
   frndint_7();
   frndint_8();
   frndint_9();
   frndint_10();
   frndint_11();
   frndint_12();
   frndint_13();
   frndint_14();
   frndint_15();
   frndint_16();
   fsubs_1();
   fsubs_2();
   fsubs_3();
   fsubs_4();
   fsubl_1();
   fsubl_2();
   fsubl_3();
   fsubl_4();
   fsub_1();
   fsub_2();
   fsub_3();
   fsub_4();
   fsub_5();
   fsub_6();
   fsub_7();
   fsub_8();
   fsub_9();
   fsub_10();
   fsub_11();
   fsub_12();
   fsub_13();
   fsub_14();
   fsub_15();
   fsub_16();
   fsubp_1();
   fsubp_2();
   fsubp_3();
   fsubp_4();
   fsubp_5();
   fsubp_6();
   fsubp_7();
   fsubp_8();
   fsubp_9();
   fsubp_10();
   fsubp_11();
   fsubp_12();
   fsubp_13();
   fsubp_14();
   fsubp_15();
   fsubp_16();
   fisubs_1();
   fisubs_2();
   fisubs_3();
   fisubs_4();
   fisubs_5();
   fisubs_6();
   fisubs_7();
   fisubs_8();
   fisubl_1();
   fisubl_2();
   fisubl_3();
   fisubl_4();
   fisubl_5();
   fisubl_6();
   fisubl_7();
   fisubl_8();
   fsubrs_1();
   fsubrs_2();
   fsubrs_3();
   fsubrs_4();
   fsubrl_1();
   fsubrl_2();
   fsubrl_3();
   fsubrl_4();
   fsubr_1();
   fsubr_2();
   fsubr_3();
   fsubr_4();
   fsubr_5();
   fsubr_6();
   fsubr_7();
   fsubr_8();
   fsubr_9();
   fsubr_10();
   fsubr_11();
   fsubr_12();
   fsubr_13();
   fsubr_14();
   fsubr_15();
   fsubr_16();
   fsubrp_1();
   fsubrp_2();
   fsubrp_3();
   fsubrp_4();
   fsubrp_5();
   fsubrp_6();
   fsubrp_7();
   fsubrp_8();
   fsubrp_9();
   fsubrp_10();
   fsubrp_11();
   fsubrp_12();
   fsubrp_13();
   fsubrp_14();
   fsubrp_15();
   fsubrp_16();
   fisubrs_1();
   fisubrs_2();
   fisubrs_3();
   fisubrs_4();
   fisubrs_5();
   fisubrs_6();
   fisubrs_7();
   fisubrs_8();
   fisubrl_1();
   fisubrl_2();
   fisubrl_3();
   fisubrl_4();
   fisubrl_5();
   fisubrl_6();
   fisubrl_7();
   fisubrl_8();
   fxch_1();
   fxch_2();

   exit(0);
}
