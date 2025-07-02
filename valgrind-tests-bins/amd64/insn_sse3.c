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

static void addsubpd_1(void)
{
   reg128_t arg0 = { .pd = { 1.11, 2.22 } };
   reg128_t arg1 = { .pd = { 41.1, 42.2 } };
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
         "addsubpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 39.99) && eq_double(result0.pd[1], 44.42) )
      {
         printf("addsubpd_1 ... ok\n");
      }
      else
      {
         printf("addsubpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 39.99);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 44.42);
      }
   }
   else
   {
      printf("addsubpd_1 ... failed\n");
   }

   return;
}

static void addsubpd_2(void)
{
   reg128_t arg0 = { .pd = { 1.11, 2.22 } };
   reg128_t arg1 = { .pd = { 41.1, 42.2 } };
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
         "addsubpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 39.99) && eq_double(result0.pd[1], 44.42) )
      {
         printf("addsubpd_2 ... ok\n");
      }
      else
      {
         printf("addsubpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 39.99);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 44.42);
      }
   }
   else
   {
      printf("addsubpd_2 ... failed\n");
   }

   return;
}

static void addsubps_1(void)
{
   reg128_t arg0 = { .ps = { 1.11F, 2.22F, 3.33F, 4.44F } };
   reg128_t arg1 = { .ps = { 41.1F, 42.2F, 43.3F, 44.4F } };
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
         "addsubps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 39.99F) && eq_float(result0.ps[1], 44.42F) && eq_float(result0.ps[2], 39.97F) && eq_float(result0.ps[3], 48.84F) )
      {
         printf("addsubps_1 ... ok\n");
      }
      else
      {
         printf("addsubps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 39.99F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 44.42F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 39.97F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 48.84F);
      }
   }
   else
   {
      printf("addsubps_1 ... failed\n");
   }

   return;
}

static void addsubps_2(void)
{
   reg128_t arg0 = { .ps = { 1.11F, 2.22F, 3.33F, 4.44F } };
   reg128_t arg1 = { .ps = { 41.1F, 42.2F, 43.3F, 44.4F } };
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
         "addsubps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 39.99F) && eq_float(result0.ps[1], 44.42F) && eq_float(result0.ps[2], 39.97F) && eq_float(result0.ps[3], 48.84F) )
      {
         printf("addsubps_2 ... ok\n");
      }
      else
      {
         printf("addsubps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 39.99F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 44.42F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 39.97F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 48.84F);
      }
   }
   else
   {
      printf("addsubps_2 ... failed\n");
   }

   return;
}

static void haddpd_1(void)
{
   reg128_t arg0 = { .pd = { 1.11, 2.22 } };
   reg128_t arg1 = { .pd = { 41.1, 42.2 } };
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
         "haddpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 83.3) && eq_double(result0.pd[1], 3.33) )
      {
         printf("haddpd_1 ... ok\n");
      }
      else
      {
         printf("haddpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 83.3);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 3.33);
      }
   }
   else
   {
      printf("haddpd_1 ... failed\n");
   }

   return;
}

static void haddpd_2(void)
{
   reg128_t arg0 = { .pd = { 1.11, 2.22 } };
   reg128_t arg1 = { .pd = { 41.1, 42.2 } };
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
         "haddpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], 83.3) && eq_double(result0.pd[1], 3.33) )
      {
         printf("haddpd_2 ... ok\n");
      }
      else
      {
         printf("haddpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], 83.3);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 3.33);
      }
   }
   else
   {
      printf("haddpd_2 ... failed\n");
   }

   return;
}

static void haddps_1(void)
{
   reg128_t arg0 = { .ps = { 1.11F, 2.22F, 3.33F, 4.44F } };
   reg128_t arg1 = { .ps = { 41.1F, 42.2F, 43.3F, 44.4F } };
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
         "haddps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 83.30F) && eq_float(result0.ps[1], 87.70F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 7.77F) )
      {
         printf("haddps_1 ... ok\n");
      }
      else
      {
         printf("haddps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 83.30F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 87.70F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 7.77F);
      }
   }
   else
   {
      printf("haddps_1 ... failed\n");
   }

   return;
}

static void haddps_2(void)
{
   reg128_t arg0 = { .ps = { 1.11F, 2.22F, 3.33F, 4.44F } };
   reg128_t arg1 = { .ps = { 41.1F, 42.2F, 43.3F, 44.4F } };
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
         "haddps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 83.30F) && eq_float(result0.ps[1], 87.70F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 7.77F) )
      {
         printf("haddps_2 ... ok\n");
      }
      else
      {
         printf("haddps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 83.30F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 87.70F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 7.77F);
      }
   }
   else
   {
      printf("haddps_2 ... failed\n");
   }

   return;
}

static void hsubpd_1(void)
{
   reg128_t arg0 = { .pd = { 9.11, 2.22 } };
   reg128_t arg1 = { .pd = { 41.1, 42.2 } };
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
         "hsubpd %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], -1.1) && eq_double(result0.pd[1], 6.89) )
      {
         printf("hsubpd_1 ... ok\n");
      }
      else
      {
         printf("hsubpd_1 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1.1);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 6.89);
      }
   }
   else
   {
      printf("hsubpd_1 ... failed\n");
   }

   return;
}

static void hsubpd_2(void)
{
   reg128_t arg0 = { .pd = { 9.11, 2.22 } };
   reg128_t arg1 = { .pd = { 41.1, 42.2 } };
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
         "hsubpd %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_double(result0.pd[0], -1.1) && eq_double(result0.pd[1], 6.89) )
      {
         printf("hsubpd_2 ... ok\n");
      }
      else
      {
         printf("hsubpd_2 ... not ok\n");
         printf("  result0.pd[0] = %.16g (expected %.16g)\n", result0.pd[0], -1.1);
         printf("  result0.pd[1] = %.16g (expected %.16g)\n", result0.pd[1], 6.89);
      }
   }
   else
   {
      printf("hsubpd_2 ... failed\n");
   }

   return;
}

static void hsubps_1(void)
{
   reg128_t arg0 = { .ps = { 1.11F, 22.2F, 3.83F, 54.4F } };
   reg128_t arg1 = { .ps = { 41.1F, 43.2F, 434.3F, 144.4F } };
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
         "hsubps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], -2.1F) && eq_float(result0.ps[1], 289.9F) && eq_float(result0.ps[2], -21.09F) && eq_float(result0.ps[3], -50.57F) )
      {
         printf("hsubps_1 ... ok\n");
      }
      else
      {
         printf("hsubps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -2.1F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 289.9F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], -21.09F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], -50.57F);
      }
   }
   else
   {
      printf("hsubps_1 ... failed\n");
   }

   return;
}

static void hsubps_2(void)
{
   reg128_t arg0 = { .ps = { 1.11F, 22.2F, 3.83F, 54.4F } };
   reg128_t arg1 = { .ps = { 41.1F, 43.2F, 434.3F, 144.4F } };
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
         "hsubps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], -2.1F) && eq_float(result0.ps[1], 289.9F) && eq_float(result0.ps[2], -21.09F) && eq_float(result0.ps[3], -50.57F) )
      {
         printf("hsubps_2 ... ok\n");
      }
      else
      {
         printf("hsubps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], -2.1F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 289.9F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], -21.09F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], -50.57F);
      }
   }
   else
   {
      printf("hsubps_2 ... failed\n");
   }

   return;
}

static void lddqu_1(void)
{
   reg128_t arg0 = { .ud = { 11111, 22222, 33333, 44444 } };
   reg128_t arg1 = { .ud = { 55555, 66666, 77777, 88888 } };
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
         "lddqu %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 11111 && result0.ud[1] == 22222 && result0.ud[2] == 33333 && result0.ud[3] == 44444 )
      {
         printf("lddqu_1 ... ok\n");
      }
      else
      {
         printf("lddqu_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 11111);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 22222);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 33333);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 44444);
      }
   }
   else
   {
      printf("lddqu_1 ... failed\n");
   }

   return;
}

static void movddup_1(void)
{
   reg64_t arg0 = { .ud = { 11111, 22222 } };
   reg128_t arg1 = { .ud = { 55555, 66666, 77777, 88888 } };
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
         "movddup %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "mm6"
      );

      if (result0.ud[0] == 11111 && result0.ud[1] == 22222 && result0.ud[2] == 11111 && result0.ud[3] == 22222 )
      {
         printf("movddup_1 ... ok\n");
      }
      else
      {
         printf("movddup_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 11111);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 22222);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 11111);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 22222);
      }
   }
   else
   {
      printf("movddup_1 ... failed\n");
   }

   return;
}

static void movddup_2(void)
{
   reg128_t arg0 = { .ud = { 11111, 22222, 33333, 44444 } };
   reg128_t arg1 = { .ud = { 55555, 66666, 77777, 88888 } };
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
         "movddup %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 11111 && result0.ud[1] == 22222 && result0.ud[2] == 11111 && result0.ud[3] == 22222 )
      {
         printf("movddup_2 ... ok\n");
      }
      else
      {
         printf("movddup_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 11111);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 22222);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 11111);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 22222);
      }
   }
   else
   {
      printf("movddup_2 ... failed\n");
   }

   return;
}

static void movshdup_1(void)
{
   reg128_t arg0 = { .ud = { 11111, 22222, 33333, 44444 } };
   reg128_t arg1 = { .ud = { 55555, 66666, 77777, 88888 } };
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
         "movshdup %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 22222 && result0.ud[1] == 22222 && result0.ud[2] == 44444 && result0.ud[3] == 44444 )
      {
         printf("movshdup_1 ... ok\n");
      }
      else
      {
         printf("movshdup_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 22222);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 22222);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 44444);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 44444);
      }
   }
   else
   {
      printf("movshdup_1 ... failed\n");
   }

   return;
}

static void movshdup_2(void)
{
   reg128_t arg0 = { .ud = { 11111, 22222, 33333, 44444 } };
   reg128_t arg1 = { .ud = { 55555, 66666, 77777, 88888 } };
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
         "movshdup %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 22222 && result0.ud[1] == 22222 && result0.ud[2] == 44444 && result0.ud[3] == 44444 )
      {
         printf("movshdup_2 ... ok\n");
      }
      else
      {
         printf("movshdup_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 22222);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 22222);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 44444);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 44444);
      }
   }
   else
   {
      printf("movshdup_2 ... failed\n");
   }

   return;
}

static void movsldup_1(void)
{
   reg128_t arg0 = { .ud = { 11111, 22222, 33333, 44444 } };
   reg128_t arg1 = { .ud = { 55555, 66666, 77777, 88888 } };
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
         "movsldup %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 11111 && result0.ud[1] == 11111 && result0.ud[2] == 33333 && result0.ud[3] == 33333 )
      {
         printf("movsldup_1 ... ok\n");
      }
      else
      {
         printf("movsldup_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 11111);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 11111);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 33333);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 33333);
      }
   }
   else
   {
      printf("movsldup_1 ... failed\n");
   }

   return;
}

static void movsldup_2(void)
{
   reg128_t arg0 = { .ud = { 11111, 22222, 33333, 44444 } };
   reg128_t arg1 = { .ud = { 55555, 66666, 77777, 88888 } };
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
         "movsldup %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 11111 && result0.ud[1] == 11111 && result0.ud[2] == 33333 && result0.ud[3] == 33333 )
      {
         printf("movsldup_2 ... ok\n");
      }
      else
      {
         printf("movsldup_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 11111);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 11111);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 33333);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 33333);
      }
   }
   else
   {
      printf("movsldup_2 ... failed\n");
   }

   return;
}

static void fisttps_1(void)
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
         "fisttps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == 1234 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttps_1 ... ok\n");
      }
      else
      {
         printf("fisttps_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttps_1 ... failed\n");
   }

   return;
}

static void fisttps_2(void)
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
         "fisttps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == -1234 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttps_2 ... ok\n");
      }
      else
      {
         printf("fisttps_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttps_2 ... failed\n");
   }

   return;
}

static void fisttps_3(void)
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
         "fisttps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == 1234 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttps_3 ... ok\n");
      }
      else
      {
         printf("fisttps_3 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttps_3 ... failed\n");
   }

   return;
}

static void fisttps_4(void)
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
         "fisttps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == -1234 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttps_4 ... ok\n");
      }
      else
      {
         printf("fisttps_4 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttps_4 ... failed\n");
   }

   return;
}

static void fisttps_5(void)
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
         "fisttps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == 1234 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttps_5 ... ok\n");
      }
      else
      {
         printf("fisttps_5 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttps_5 ... failed\n");
   }

   return;
}

static void fisttps_6(void)
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
         "fisttps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == -1234 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttps_6 ... ok\n");
      }
      else
      {
         printf("fisttps_6 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttps_6 ... failed\n");
   }

   return;
}

static void fisttps_7(void)
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
         "fisttps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == 1234 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttps_7 ... ok\n");
      }
      else
      {
         printf("fisttps_7 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttps_7 ... failed\n");
   }

   return;
}

static void fisttps_8(void)
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
         "fisttps %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sw[0] == -1234 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttps_8 ... ok\n");
      }
      else
      {
         printf("fisttps_8 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -1234);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttps_8 ... failed\n");
   }

   return;
}

static void fisttpl_1(void)
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
         "fisttpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == 1234567 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpl_1 ... ok\n");
      }
      else
      {
         printf("fisttpl_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234567);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpl_1 ... failed\n");
   }

   return;
}

static void fisttpl_2(void)
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
         "fisttpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == -1234567 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpl_2 ... ok\n");
      }
      else
      {
         printf("fisttpl_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -1234567);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpl_2 ... failed\n");
   }

   return;
}

static void fisttpl_3(void)
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
         "fisttpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == 1234567 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpl_3 ... ok\n");
      }
      else
      {
         printf("fisttpl_3 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234567);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpl_3 ... failed\n");
   }

   return;
}

static void fisttpl_4(void)
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
         "fisttpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == -1234567 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpl_4 ... ok\n");
      }
      else
      {
         printf("fisttpl_4 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -1234567);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpl_4 ... failed\n");
   }

   return;
}

static void fisttpl_5(void)
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
         "fisttpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == 1234567 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpl_5 ... ok\n");
      }
      else
      {
         printf("fisttpl_5 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234567);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpl_5 ... failed\n");
   }

   return;
}

static void fisttpl_6(void)
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
         "fisttpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == -1234567 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpl_6 ... ok\n");
      }
      else
      {
         printf("fisttpl_6 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -1234567);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpl_6 ... failed\n");
   }

   return;
}

static void fisttpl_7(void)
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
         "fisttpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == 1234567 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpl_7 ... ok\n");
      }
      else
      {
         printf("fisttpl_7 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234567);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpl_7 ... failed\n");
   }

   return;
}

static void fisttpl_8(void)
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
         "fisttpl %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])

      );

      if (result0.sd[0] == -1234567 &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpl_8 ... ok\n");
      }
      else
      {
         printf("fisttpl_8 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -1234567);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpl_8 ... failed\n");
   }

   return;
}

static void fisttpq_1(void)
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
         "fisttpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == 123456787654321LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpq_1 ... ok\n");
      }
      else
      {
         printf("fisttpq_1 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 123456787654321LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpq_1 ... failed\n");
   }

   return;
}

static void fisttpq_2(void)
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
         "fisttpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == -123456787654321LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpq_2 ... ok\n");
      }
      else
      {
         printf("fisttpq_2 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], -123456787654321LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpq_2 ... failed\n");
   }

   return;
}

static void fisttpq_3(void)
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
         "fisttpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == 123456787654321LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpq_3 ... ok\n");
      }
      else
      {
         printf("fisttpq_3 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 123456787654321LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpq_3 ... failed\n");
   }

   return;
}

static void fisttpq_4(void)
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
         "fisttpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == -123456787654321LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpq_4 ... ok\n");
      }
      else
      {
         printf("fisttpq_4 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], -123456787654321LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpq_4 ... failed\n");
   }

   return;
}

static void fisttpq_5(void)
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
         "fisttpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == 123456787654321LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpq_5 ... ok\n");
      }
      else
      {
         printf("fisttpq_5 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 123456787654321LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpq_5 ... failed\n");
   }

   return;
}

static void fisttpq_6(void)
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
         "fisttpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == -123456787654321LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpq_6 ... ok\n");
      }
      else
      {
         printf("fisttpq_6 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], -123456787654321LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpq_6 ... failed\n");
   }

   return;
}

static void fisttpq_7(void)
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
         "fisttpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == 123456787654321LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpq_7 ... ok\n");
      }
      else
      {
         printf("fisttpq_7 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], 123456787654321LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpq_7 ... failed\n");
   }

   return;
}

static void fisttpq_8(void)
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
         "fisttpq %0\n"
         "fstps %1\n"
         "cld\n"
         : "=m" (result0), "=m" (result1)
         : "m" (preset0), "m" (preset1), "m" (arg0), "m" (state[0])
         : "mm6"
      );

      if (result0.sq[0] == -123456787654321LL &&
          eq_float(result1.ps[0], 1111.1111F) )
      {
         printf("fisttpq_8 ... ok\n");
      }
      else
      {
         printf("fisttpq_8 ... not ok\n");
         printf("  result0.sq[0] = %lld (expected %lld)\n", result0.sq[0], -123456787654321LL);
         printf("  result1.ps[0] = %.16g (expected %.16g)\n", result1.ps[0], 1111.1111F);
      }
   }
   else
   {
      printf("fisttpq_8 ... failed\n");
   }

   return;
}

int main(int argc, char **argv)
{
   signal(SIGILL, handle_sigill);

   addsubpd_1();
   addsubpd_2();
   addsubps_1();
   addsubps_2();
   haddpd_1();
   haddpd_2();
   haddps_1();
   haddps_2();
   hsubpd_1();
   hsubpd_2();
   hsubps_1();
   hsubps_2();
   lddqu_1();
   movddup_1();
   movddup_2();
   movshdup_1();
   movshdup_2();
   movsldup_1();
   movsldup_2();
   fisttps_1();
   fisttps_2();
   fisttps_3();
   fisttps_4();
   fisttps_5();
   fisttps_6();
   fisttps_7();
   fisttps_8();
   fisttpl_1();
   fisttpl_2();
   fisttpl_3();
   fisttpl_4();
   fisttpl_5();
   fisttpl_6();
   fisttpl_7();
   fisttpl_8();
   fisttpq_1();
   fisttpq_2();
   fisttpq_3();
   fisttpq_4();
   fisttpq_5();
   fisttpq_6();
   fisttpq_7();
   fisttpq_8();

   exit(0);
}
