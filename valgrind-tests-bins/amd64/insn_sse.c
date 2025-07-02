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

static void addps_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 44.44F, 33.33F, 22.22F, 11.11F } };
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
         "addps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 56.78F) && eq_float(result0.ps[1], 90.11F) && eq_float(result0.ps[2], 65.43F) && eq_float(result0.ps[3], 98.76F) )
      {
         printf("addps_1 ... ok\n");
      }
      else
      {
         printf("addps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 56.78F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 90.11F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 65.43F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 98.76F);
      }
   }
   else
   {
      printf("addps_1 ... failed\n");
   }

   return;
}

static void addps_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 44.44F, 33.33F, 22.22F, 11.11F } };
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
         "addps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 56.78F) && eq_float(result0.ps[1], 90.11F) && eq_float(result0.ps[2], 65.43F) && eq_float(result0.ps[3], 98.76F) )
      {
         printf("addps_2 ... ok\n");
      }
      else
      {
         printf("addps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 56.78F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 90.11F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 65.43F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 98.76F);
      }
   }
   else
   {
      printf("addps_2 ... failed\n");
   }

   return;
}

static void addss_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 44.44F, 33.33F, 22.22F, 11.11F } };
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
         "addss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 56.78F) && eq_float(result0.ps[1], 33.33F) && eq_float(result0.ps[2], 22.22F) && eq_float(result0.ps[3], 11.11F) )
      {
         printf("addss_1 ... ok\n");
      }
      else
      {
         printf("addss_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 56.78F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 33.33F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 22.22F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 11.11F);
      }
   }
   else
   {
      printf("addss_1 ... failed\n");
   }

   return;
}

static void addss_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 44.44F, 33.33F, 22.22F, 11.11F } };
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
         "addss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 56.78F) && eq_float(result0.ps[1], 33.33F) && eq_float(result0.ps[2], 22.22F) && eq_float(result0.ps[3], 11.11F) )
      {
         printf("addss_2 ... ok\n");
      }
      else
      {
         printf("addss_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 56.78F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 33.33F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 22.22F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 11.11F);
      }
   }
   else
   {
      printf("addss_2 ... failed\n");
   }

   return;
}

static void andnps_1(void)
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
         "andnps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfc98301064002000ULL && result0.uq[1] == 0x00020046010389cfULL )
      {
         printf("andnps_1 ... ok\n");
      }
      else
      {
         printf("andnps_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc98301064002000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x00020046010389cfULL);
      }
   }
   else
   {
      printf("andnps_1 ... failed\n");
   }

   return;
}

static void andnps_2(void)
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
         "andnps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfc98301064002000ULL && result0.uq[1] == 0x00020046010389cfULL )
      {
         printf("andnps_2 ... ok\n");
      }
      else
      {
         printf("andnps_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc98301064002000ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x00020046010389cfULL);
      }
   }
   else
   {
      printf("andnps_2 ... failed\n");
   }

   return;
}

static void andps_1(void)
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
         "andps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0121452188a84420ULL && result0.uq[1] == 0x0121452188a84420ULL )
      {
         printf("andps_1 ... ok\n");
      }
      else
      {
         printf("andps_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0121452188a84420ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0121452188a84420ULL);
      }
   }
   else
   {
      printf("andps_1 ... failed\n");
   }

   return;
}

static void andps_2(void)
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
         "andps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0x0121452188a84420ULL && result0.uq[1] == 0x0121452188a84420ULL )
      {
         printf("andps_2 ... ok\n");
      }
      else
      {
         printf("andps_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0121452188a84420ULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0x0121452188a84420ULL);
      }
   }
   else
   {
      printf("andps_2 ... failed\n");
   }

   return;
}

static void cmpeqps_1(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5678F, 234.5679F, 234.5678F, 234.5679F } };
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
         "cmpeqps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("cmpeqps_1 ... ok\n");
      }
      else
      {
         printf("cmpeqps_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpeqps_1 ... failed\n");
   }

   return;
}

static void cmpeqps_2(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5678F, 234.5679F, 234.5678F, 234.5679F } };
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
         "cmpeqps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("cmpeqps_2 ... ok\n");
      }
      else
      {
         printf("cmpeqps_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpeqps_2 ... failed\n");
   }

   return;
}

static void cmpeqss_1(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
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
         "cmpeqss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpeqss_1 ... ok\n");
      }
      else
      {
         printf("cmpeqss_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpeqss_1 ... failed\n");
   }

   return;
}

static void cmpeqss_2(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5679F, 0.0F, 0.0F, 0.0F } };
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
         "cmpeqss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00000000 && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpeqss_2 ... ok\n");
      }
      else
      {
         printf("cmpeqss_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00000000);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpeqss_2 ... failed\n");
   }

   return;
}

static void cmpleps_1(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5678F, 234.5679F, 234.5678F, 234.5679F } };
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
         "cmpleps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("cmpleps_1 ... ok\n");
      }
      else
      {
         printf("cmpleps_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpleps_1 ... failed\n");
   }

   return;
}

static void cmpleps_2(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5678F, 234.5679F, 234.5678F, 234.5679F } };
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
         "cmpleps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("cmpleps_2 ... ok\n");
      }
      else
      {
         printf("cmpleps_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpleps_2 ... failed\n");
   }

   return;
}

static void cmpless_1(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
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
         "cmpless %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpless_1 ... ok\n");
      }
      else
      {
         printf("cmpless_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpless_1 ... failed\n");
   }

   return;
}

static void cmpless_2(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5679F, 0.0F, 0.0F, 0.0F } };
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
         "cmpless %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00000000 && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpless_2 ... ok\n");
      }
      else
      {
         printf("cmpless_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00000000);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpless_2 ... failed\n");
   }

   return;
}

static void cmpltps_1(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5677F, 234.5679F, 234.5677F, 234.5679F } };
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
         "cmpltps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("cmpltps_1 ... ok\n");
      }
      else
      {
         printf("cmpltps_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpltps_1 ... failed\n");
   }

   return;
}

static void cmpltps_2(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5677F, 234.5679F, 234.5677F, 234.5679F } };
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
         "cmpltps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("cmpltps_2 ... ok\n");
      }
      else
      {
         printf("cmpltps_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpltps_2 ... failed\n");
   }

   return;
}

static void cmpltss_1(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5676F, 0.0F, 0.0F, 0.0F } };
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
         "cmpltss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpltss_1 ... ok\n");
      }
      else
      {
         printf("cmpltss_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpltss_1 ... failed\n");
   }

   return;
}

static void cmpltss_2(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5679F, 0.0F, 0.0F, 0.0F } };
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
         "cmpltss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00000000 && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpltss_2 ... ok\n");
      }
      else
      {
         printf("cmpltss_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00000000);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpltss_2 ... failed\n");
   }

   return;
}

static void cmpunordps_1(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5677F, 234.5679F, 234.5677F, 234.5679F } };
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
         "cmpunordps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00000000 && result0.ud[1] == 0x00000000 && result0.ud[2] == 0x00000000 && result0.ud[3] == 0x00000000 )
      {
         printf("cmpunordps_1 ... ok\n");
      }
      else
      {
         printf("cmpunordps_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00000000);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0x00000000);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpunordps_1 ... failed\n");
   }

   return;
}

static void cmpunordps_2(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5677F, 234.5679F, 234.5677F, 234.5679F } };
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
         "cmpunordps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00000000 && result0.ud[1] == 0x00000000 && result0.ud[2] == 0x00000000 && result0.ud[3] == 0x00000000 )
      {
         printf("cmpunordps_2 ... ok\n");
      }
      else
      {
         printf("cmpunordps_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00000000);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0x00000000);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpunordps_2 ... failed\n");
   }

   return;
}

static void cmpunordss_1(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5676F, 0.0F, 0.0F, 0.0F } };
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
         "cmpunordss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00000000 && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpunordss_1 ... ok\n");
      }
      else
      {
         printf("cmpunordss_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00000000);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpunordss_1 ... failed\n");
   }

   return;
}

static void cmpunordss_2(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5679F, 0.0F, 0.0F, 0.0F } };
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
         "cmpunordss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00000000 && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpunordss_2 ... ok\n");
      }
      else
      {
         printf("cmpunordss_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00000000);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpunordss_2 ... failed\n");
   }

   return;
}

static void cmpneqps_1(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5679F, 234.5678F, 234.5679F, 234.5678F } };
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
         "cmpneqps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("cmpneqps_1 ... ok\n");
      }
      else
      {
         printf("cmpneqps_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpneqps_1 ... failed\n");
   }

   return;
}

static void cmpneqps_2(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5679F, 234.5678F, 234.5679F, 234.5678F } };
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
         "cmpneqps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("cmpneqps_2 ... ok\n");
      }
      else
      {
         printf("cmpneqps_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpneqps_2 ... failed\n");
   }

   return;
}

static void cmpneqss_1(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5679F, 0.0F, 0.0F, 0.0F } };
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
         "cmpneqss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpneqss_1 ... ok\n");
      }
      else
      {
         printf("cmpneqss_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpneqss_1 ... failed\n");
   }

   return;
}

static void cmpneqss_2(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
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
         "cmpneqss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00000000 && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpneqss_2 ... ok\n");
      }
      else
      {
         printf("cmpneqss_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00000000);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpneqss_2 ... failed\n");
   }

   return;
}

static void cmpnleps_1(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5679F, 234.5678F, 234.5679F, 234.5678F } };
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
         "cmpnleps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("cmpnleps_1 ... ok\n");
      }
      else
      {
         printf("cmpnleps_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpnleps_1 ... failed\n");
   }

   return;
}

static void cmpnleps_2(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5679F, 234.5678F, 234.5679F, 234.5678F } };
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
         "cmpnleps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("cmpnleps_2 ... ok\n");
      }
      else
      {
         printf("cmpnleps_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpnleps_2 ... failed\n");
   }

   return;
}

static void cmpnless_1(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5679F, 0.0F, 0.0F, 0.0F } };
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
         "cmpnless %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpnless_1 ... ok\n");
      }
      else
      {
         printf("cmpnless_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpnless_1 ... failed\n");
   }

   return;
}

static void cmpnless_2(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
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
         "cmpnless %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00000000 && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpnless_2 ... ok\n");
      }
      else
      {
         printf("cmpnless_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00000000);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpnless_2 ... failed\n");
   }

   return;
}

static void cmpnltps_1(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5679F, 234.5677F, 234.5679F, 234.5677F } };
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
         "cmpnltps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("cmpnltps_1 ... ok\n");
      }
      else
      {
         printf("cmpnltps_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpnltps_1 ... failed\n");
   }

   return;
}

static void cmpnltps_2(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5679F, 234.5677F, 234.5679F, 234.5677F } };
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
         "cmpnltps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 && result0.ud[2] == 0xffffffff && result0.ud[3] == 0x00000000 )
      {
         printf("cmpnltps_2 ... ok\n");
      }
      else
      {
         printf("cmpnltps_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0x00000000);
      }
   }
   else
   {
      printf("cmpnltps_2 ... failed\n");
   }

   return;
}

static void cmpnltss_1(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5679F, 0.0F, 0.0F, 0.0F } };
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
         "cmpnltss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpnltss_1 ... ok\n");
      }
      else
      {
         printf("cmpnltss_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpnltss_1 ... failed\n");
   }

   return;
}

static void cmpnltss_2(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5676F, 0.0F, 0.0F, 0.0F } };
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
         "cmpnltss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0x00000000 && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpnltss_2 ... ok\n");
      }
      else
      {
         printf("cmpnltss_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00000000);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpnltss_2 ... failed\n");
   }

   return;
}

static void cmpordps_1(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5679F, 234.5677F, 234.5679F, 234.5677F } };
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
         "cmpordps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0xffffffff && result0.ud[2] == 0xffffffff && result0.ud[3] == 0xffffffff )
      {
         printf("cmpordps_1 ... ok\n");
      }
      else
      {
         printf("cmpordps_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0xffffffff);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0xffffffff);
      }
   }
   else
   {
      printf("cmpordps_1 ... failed\n");
   }

   return;
}

static void cmpordps_2(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 234.5678F, 234.5678F, 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5679F, 234.5677F, 234.5679F, 234.5677F } };
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
         "cmpordps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0xffffffff && result0.ud[2] == 0xffffffff && result0.ud[3] == 0xffffffff )
      {
         printf("cmpordps_2 ... ok\n");
      }
      else
      {
         printf("cmpordps_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0xffffffff);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0xffffffff);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0xffffffff);
      }
   }
   else
   {
      printf("cmpordps_2 ... failed\n");
   }

   return;
}

static void cmpordss_1(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5679F, 0.0F, 0.0F, 0.0F } };
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
         "cmpordss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpordss_1 ... ok\n");
      }
      else
      {
         printf("cmpordss_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpordss_1 ... failed\n");
   }

   return;
}

static void cmpordss_2(void)
{
   reg128_t arg0 = { .ps = { 1234.5678F, 0.0F, 0.0F, 0.0F } };
   reg128_t arg1 = { .ps = { 1234.5676F, 0.0F, 0.0F, 0.0F } };
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
         "cmpordss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0 && result0.ud[2] == 0 && result0.ud[3] == 0 )
      {
         printf("cmpordss_2 ... ok\n");
      }
      else
      {
         printf("cmpordss_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0);
         printf("  result0.ud[2] = %u (expected %u)\n", result0.ud[2], 0);
         printf("  result0.ud[3] = %u (expected %u)\n", result0.ud[3], 0);
      }
   }
   else
   {
      printf("cmpordss_2 ... failed\n");
   }

   return;
}

static void comiss_1(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 0.0F } };
   reg128_t arg1 = { .ps = { 234.5679F, 0.0F } };
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
         "comiss %%xmm12, %%xmm13\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "xmm13"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000000UL )
      {
         printf("comiss_1 ... ok\n");
      }
      else
      {
         printf("comiss_1 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("comiss_1 ... failed\n");
   }

   return;
}

static void comiss_2(void)
{
   reg32_t arg0 = { .ps = { 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5679F, 0.0F } };
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
         "comiss %1, %%xmm12\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000000UL )
      {
         printf("comiss_2 ... ok\n");
      }
      else
      {
         printf("comiss_2 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("comiss_2 ... failed\n");
   }

   return;
}

static void comiss_3(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 0.0F } };
   reg128_t arg1 = { .ps = { 234.5677F, 0.0F } };
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
         "comiss %%xmm12, %%xmm13\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "xmm13"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000001UL )
      {
         printf("comiss_3 ... ok\n");
      }
      else
      {
         printf("comiss_3 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("comiss_3 ... failed\n");
   }

   return;
}

static void comiss_4(void)
{
   reg32_t arg0 = { .ps = { 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5677F, 0.0F } };
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
         "comiss %1, %%xmm12\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000001UL )
      {
         printf("comiss_4 ... ok\n");
      }
      else
      {
         printf("comiss_4 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("comiss_4 ... failed\n");
   }

   return;
}

static void comiss_5(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 0.0F } };
   reg128_t arg1 = { .ps = { 234.5678F, 0.0F } };
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
         "comiss %%xmm12, %%xmm13\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "xmm13"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000040UL )
      {
         printf("comiss_5 ... ok\n");
      }
      else
      {
         printf("comiss_5 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("comiss_5 ... failed\n");
   }

   return;
}

static void comiss_6(void)
{
   reg32_t arg0 = { .ps = { 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5678F, 0.0F } };
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
         "comiss %1, %%xmm12\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000040UL )
      {
         printf("comiss_6 ... ok\n");
      }
      else
      {
         printf("comiss_6 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("comiss_6 ... failed\n");
   }

   return;
}

static void cvtpi2ps_1(void)
{
   reg64_t arg0 = { .sd = { 1234, 5678 } };
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
         "movq %2, %%mm6\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "cvtpi2ps %%mm6, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "mm6"
      );

      if (eq_float(result0.ps[0], 1234.0F) && eq_float(result0.ps[1], 5678.0F) && eq_float(result0.ps[2], 3.3F) && eq_float(result0.ps[3], 4.4F) )
      {
         printf("cvtpi2ps_1 ... ok\n");
      }
      else
      {
         printf("cvtpi2ps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.0F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 5678.0F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.3F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.4F);
      }
   }
   else
   {
      printf("cvtpi2ps_1 ... failed\n");
   }

   return;
}

static void cvtpi2ps_2(void)
{
   reg64_t arg0 = { .sd = { 1234, 5678 } };
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
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "cvtpi2ps %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "mm6"
      );

      if (eq_float(result0.ps[0], 1234.0F) && eq_float(result0.ps[1], 5678.0F) && eq_float(result0.ps[2], 3.3F) && eq_float(result0.ps[3], 4.4F) )
      {
         printf("cvtpi2ps_2 ... ok\n");
      }
      else
      {
         printf("cvtpi2ps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1234.0F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 5678.0F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.3F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.4F);
      }
   }
   else
   {
      printf("cvtpi2ps_2 ... failed\n");
   }

   return;
}

static void cvtps2pi_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 1.11F, 2.22F } };
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
         "cvtps2pi %%xmm12, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 57 )
      {
         printf("cvtps2pi_1 ... ok\n");
      }
      else
      {
         printf("cvtps2pi_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 57);
      }
   }
   else
   {
      printf("cvtps2pi_1 ... failed\n");
   }

   return;
}

static void cvtps2pi_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 1.11F, 2.22F } };
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
         "cvtps2pi %1, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 57 )
      {
         printf("cvtps2pi_2 ... ok\n");
      }
      else
      {
         printf("cvtps2pi_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 57);
      }
   }
   else
   {
      printf("cvtps2pi_2 ... failed\n");
   }

   return;
}

static void cvtsi2ss_1(void)
{
   reg32_t arg0 = { .sd = { 12 } };
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
         "movl %2, %%r9d\n"
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "cvtsi2ss %%r9d, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "r9"
      );

      if (eq_float(result0.ps[0], 12.0F) && eq_float(result0.ps[1], 2.22F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 4.44F) )
      {
         printf("cvtsi2ss_1 ... ok\n");
      }
      else
      {
         printf("cvtsi2ss_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.0F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 2.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.44F);
      }
   }
   else
   {
      printf("cvtsi2ss_1 ... failed\n");
   }

   return;
}

static void cvtsi2ss_2(void)
{
   reg32_t arg0 = { .sd = { 12 } };
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
         "movlps %3, %%xmm12\n"
         "movhps %4, %%xmm12\n"
         "cvtsi2ss %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (eq_float(result0.ps[0], 12.0F) && eq_float(result0.ps[1], 2.22F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 4.44F) )
      {
         printf("cvtsi2ss_2 ... ok\n");
      }
      else
      {
         printf("cvtsi2ss_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.0F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 2.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.44F);
      }
   }
   else
   {
      printf("cvtsi2ss_2 ... failed\n");
   }

   return;
}

static void cvtss2si_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
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
         "cvtss2si %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.sd[0] == 12 )
      {
         printf("cvtss2si_1 ... ok\n");
      }
      else
      {
         printf("cvtss2si_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
      }
   }
   else
   {
      printf("cvtss2si_1 ... failed\n");
   }

   return;
}

static void cvtss2si_2(void)
{
   reg128_t arg0 = { .ps = { 56.78F, 12.34F, 87.65F, 43.21F } };
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
         "cvtss2si %1, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.sd[0] == 57 )
      {
         printf("cvtss2si_2 ... ok\n");
      }
      else
      {
         printf("cvtss2si_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 57);
      }
   }
   else
   {
      printf("cvtss2si_2 ... failed\n");
   }

   return;
}

static void cvttps2pi_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 1.11F, 2.22F } };
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
         "cvttps2pi %%xmm12, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 56 )
      {
         printf("cvttps2pi_1 ... ok\n");
      }
      else
      {
         printf("cvttps2pi_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 56);
      }
   }
   else
   {
      printf("cvttps2pi_1 ... failed\n");
   }

   return;
}

static void cvttps2pi_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 1.11F, 2.22F } };
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
         "cvttps2pi %1, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (result0.sd[0] == 12 && result0.sd[1] == 56 )
      {
         printf("cvttps2pi_2 ... ok\n");
      }
      else
      {
         printf("cvttps2pi_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 56);
      }
   }
   else
   {
      printf("cvttps2pi_2 ... failed\n");
   }

   return;
}

static void cvttss2si_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
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
         "cvttss2si %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.sd[0] == 12 )
      {
         printf("cvttss2si_1 ... ok\n");
      }
      else
      {
         printf("cvttss2si_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12);
      }
   }
   else
   {
      printf("cvttss2si_1 ... failed\n");
   }

   return;
}

static void cvttss2si_2(void)
{
   reg128_t arg0 = { .ps = { 56.78F, 12.34F, 87.65F, 43.21F } };
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
         "cvttss2si %1, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.sd[0] == 56 )
      {
         printf("cvttss2si_2 ... ok\n");
      }
      else
      {
         printf("cvttss2si_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 56);
      }
   }
   else
   {
      printf("cvttss2si_2 ... failed\n");
   }

   return;
}

static void divps_1(void)
{
   reg128_t arg0 = { .ps = { 2.0F, 3.0F, 4.0F, 5.0F } };
   reg128_t arg1 = { .ps = { 24.68F, 3.69F, 48.48F, 55.55F } };
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
         "divps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 1.23F) && eq_float(result0.ps[2], 12.12F) && eq_float(result0.ps[3], 11.11F) )
      {
         printf("divps_1 ... ok\n");
      }
      else
      {
         printf("divps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 1.23F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 12.12F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 11.11F);
      }
   }
   else
   {
      printf("divps_1 ... failed\n");
   }

   return;
}

static void divps_2(void)
{
   reg128_t arg0 = { .ps = { 2.0F, 3.0F, 4.0F, 5.0F } };
   reg128_t arg1 = { .ps = { 24.68F, 3.69F, 48.48F, 55.55F } };
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
         "divps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 1.23F) && eq_float(result0.ps[2], 12.12F) && eq_float(result0.ps[3], 11.11F) )
      {
         printf("divps_2 ... ok\n");
      }
      else
      {
         printf("divps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 1.23F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 12.12F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 11.11F);
      }
   }
   else
   {
      printf("divps_2 ... failed\n");
   }

   return;
}

static void divss_1(void)
{
   reg128_t arg0 = { .ps = { 2.0F, 3.0F, 4.0F, 5.0F } };
   reg128_t arg1 = { .ps = { 24.68F, 3.69F, 48.48F, 55.55F } };
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
         "divss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 3.69F) && eq_float(result0.ps[2], 48.48F) && eq_float(result0.ps[3], 55.55F) )
      {
         printf("divss_1 ... ok\n");
      }
      else
      {
         printf("divss_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 3.69F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 48.48F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 55.55F);
      }
   }
   else
   {
      printf("divss_1 ... failed\n");
   }

   return;
}

static void divss_2(void)
{
   reg128_t arg0 = { .ps = { 2.0F, 3.0F, 4.0F, 5.0F } };
   reg128_t arg1 = { .ps = { 24.68F, 3.69F, 48.48F, 55.55F } };
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
         "divss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 3.69F) && eq_float(result0.ps[2], 48.48F) && eq_float(result0.ps[3], 55.55F) )
      {
         printf("divss_2 ... ok\n");
      }
      else
      {
         printf("divss_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 3.69F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 48.48F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 55.55F);
      }
   }
   else
   {
      printf("divss_2 ... failed\n");
   }

   return;
}

static void maxps_1(void)
{
   reg128_t arg0 = { .ps = { 2.22F, 4.44F, 6.66F, 8.88F } };
   reg128_t arg1 = { .ps = { 7.77F, 5.55F, 3.33F, 1.11F } };
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
         "maxps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 7.77F) && eq_float(result0.ps[1], 5.55F) && eq_float(result0.ps[2], 6.66F) && eq_float(result0.ps[3], 8.88F) )
      {
         printf("maxps_1 ... ok\n");
      }
      else
      {
         printf("maxps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.77F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 5.55F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 6.66F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 8.88F);
      }
   }
   else
   {
      printf("maxps_1 ... failed\n");
   }

   return;
}

static void maxps_2(void)
{
   reg128_t arg0 = { .ps = { 2.22F, 4.44F, 6.66F, 8.88F } };
   reg128_t arg1 = { .ps = { 7.77F, 5.55F, 3.33F, 1.11F } };
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
         "maxps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 7.77F) && eq_float(result0.ps[1], 5.55F) && eq_float(result0.ps[2], 6.66F) && eq_float(result0.ps[3], 8.88F) )
      {
         printf("maxps_2 ... ok\n");
      }
      else
      {
         printf("maxps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.77F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 5.55F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 6.66F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 8.88F);
      }
   }
   else
   {
      printf("maxps_2 ... failed\n");
   }

   return;
}

static void maxss_1(void)
{
   reg128_t arg0 = { .ps = { 2.22F, 4.44F, 6.66F, 8.88F } };
   reg128_t arg1 = { .ps = { 7.77F, 5.55F, 3.33F, 1.11F } };
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
         "maxss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 7.77F) && eq_float(result0.ps[1], 5.55F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 1.11F) )
      {
         printf("maxss_1 ... ok\n");
      }
      else
      {
         printf("maxss_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 7.77F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 5.55F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 1.11F);
      }
   }
   else
   {
      printf("maxss_1 ... failed\n");
   }

   return;
}

static void maxss_2(void)
{
   reg128_t arg0 = { .ps = { 8.88F, 6.66F, 4.44F, 2.22F } };
   reg128_t arg1 = { .ps = { 1.11F, 3.33F, 5.55F, 7.77F } };
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
         "maxss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 8.88F) && eq_float(result0.ps[1], 3.33F) && eq_float(result0.ps[2], 5.55F) && eq_float(result0.ps[3], 7.77F) )
      {
         printf("maxss_2 ... ok\n");
      }
      else
      {
         printf("maxss_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 8.88F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 3.33F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 5.55F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 7.77F);
      }
   }
   else
   {
      printf("maxss_2 ... failed\n");
   }

   return;
}

static void minps_1(void)
{
   reg128_t arg0 = { .ps = { 2.22F, 4.44F, 6.66F, 8.88F } };
   reg128_t arg1 = { .ps = { 7.77F, 5.55F, 3.33F, 1.11F } };
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
         "minps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 2.22F) && eq_float(result0.ps[1], 4.44F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 1.11F) )
      {
         printf("minps_1 ... ok\n");
      }
      else
      {
         printf("minps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 2.22F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 4.44F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 1.11F);
      }
   }
   else
   {
      printf("minps_1 ... failed\n");
   }

   return;
}

static void minps_2(void)
{
   reg128_t arg0 = { .ps = { 2.22F, 4.44F, 6.66F, 8.88F } };
   reg128_t arg1 = { .ps = { 7.77F, 5.55F, 3.33F, 1.11F } };
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
         "minps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 2.22F) && eq_float(result0.ps[1], 4.44F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 1.11F) )
      {
         printf("minps_2 ... ok\n");
      }
      else
      {
         printf("minps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 2.22F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 4.44F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 1.11F);
      }
   }
   else
   {
      printf("minps_2 ... failed\n");
   }

   return;
}

static void minss_1(void)
{
   reg128_t arg0 = { .ps = { 2.22F, 4.44F, 6.66F, 8.88F } };
   reg128_t arg1 = { .ps = { 7.77F, 5.55F, 3.33F, 1.11F } };
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
         "minss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 2.22F) && eq_float(result0.ps[1], 5.55F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 1.11F) )
      {
         printf("minss_1 ... ok\n");
      }
      else
      {
         printf("minss_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 2.22F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 5.55F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 1.11F);
      }
   }
   else
   {
      printf("minss_1 ... failed\n");
   }

   return;
}

static void minss_2(void)
{
   reg128_t arg0 = { .ps = { 8.88F, 6.66F, 4.44F, 2.22F } };
   reg128_t arg1 = { .ps = { 1.11F, 3.33F, 5.55F, 7.77F } };
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
         "minss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 1.11F) && eq_float(result0.ps[1], 3.33F) && eq_float(result0.ps[2], 5.55F) && eq_float(result0.ps[3], 7.77F) )
      {
         printf("minss_2 ... ok\n");
      }
      else
      {
         printf("minss_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 1.11F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 3.33F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 5.55F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 7.77F);
      }
   }
   else
   {
      printf("minss_2 ... failed\n");
   }

   return;
}

static void movaps_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 11.11F, 22.22F, 33.33F, 44.44F } };
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
         "movaps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 56.78F) && eq_float(result0.ps[2], 43.21F) && eq_float(result0.ps[3], 87.65F) )
      {
         printf("movaps_1 ... ok\n");
      }
      else
      {
         printf("movaps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 56.78F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 43.21F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 87.65F);
      }
   }
   else
   {
      printf("movaps_1 ... failed\n");
   }

   return;
}

static void movaps_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 11.11F, 22.22F, 33.33F, 44.44F } };
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
         "movaps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 56.78F) && eq_float(result0.ps[2], 43.21F) && eq_float(result0.ps[3], 87.65F) )
      {
         printf("movaps_2 ... ok\n");
      }
      else
      {
         printf("movaps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 56.78F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 43.21F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 87.65F);
      }
   }
   else
   {
      printf("movaps_2 ... failed\n");
   }

   return;
}

static void movhlps_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 11.11F, 22.22F, 33.33F, 44.44F } };
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
         "movhlps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 43.21F) && eq_float(result0.ps[1], 87.65F) && eq_float(result0.ps[2], 33.33F) && eq_float(result0.ps[3], 44.44F) )
      {
         printf("movhlps_1 ... ok\n");
      }
      else
      {
         printf("movhlps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 43.21F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 87.65F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 33.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 44.44F);
      }
   }
   else
   {
      printf("movhlps_1 ... failed\n");
   }

   return;
}

static void movhps_1(void)
{
   reg64_t arg0 = { .ps = { 12.34F, 56.78F } };
   reg128_t arg1 = { .ps = { 11.11F, 22.22F, 33.33F, 44.44F } };
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
         "movhps %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "mm6"
      );

      if (eq_float(result0.ps[0], 11.11F) && eq_float(result0.ps[1], 22.22F) && eq_float(result0.ps[2], 12.34F) && eq_float(result0.ps[3], 56.78F) )
      {
         printf("movhps_1 ... ok\n");
      }
      else
      {
         printf("movhps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 11.11F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 22.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 12.34F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 56.78F);
      }
   }
   else
   {
      printf("movhps_1 ... failed\n");
   }

   return;
}

static void movhps_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg64_t arg1 = { .ps = { 11.11F, 22.22F } };
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
         "movhps %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (eq_float(result0.ps[0], 43.21F) && eq_float(result0.ps[1], 87.65F) )
      {
         printf("movhps_2 ... ok\n");
      }
      else
      {
         printf("movhps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 43.21F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 87.65F);
      }
   }
   else
   {
      printf("movhps_2 ... failed\n");
   }

   return;
}

static void movlhps_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 11.11F, 22.22F, 33.33F, 44.44F } };
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
         "movlhps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 11.11F) && eq_float(result0.ps[1], 22.22F) && eq_float(result0.ps[2], 12.34F) && eq_float(result0.ps[3], 56.78F) )
      {
         printf("movlhps_1 ... ok\n");
      }
      else
      {
         printf("movlhps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 11.11F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 22.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 12.34F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 56.78F);
      }
   }
   else
   {
      printf("movlhps_1 ... failed\n");
   }

   return;
}

static void movlps_1(void)
{
   reg64_t arg0 = { .ps = { 12.34F, 56.78F } };
   reg128_t arg1 = { .ps = { 11.11F, 22.22F, 33.33F, 44.44F } };
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
         "movlps %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "mm6"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 56.78F) && eq_float(result0.ps[2], 33.33F) && eq_float(result0.ps[3], 44.44F) )
      {
         printf("movlps_1 ... ok\n");
      }
      else
      {
         printf("movlps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 56.78F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 33.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 44.44F);
      }
   }
   else
   {
      printf("movlps_1 ... failed\n");
   }

   return;
}

static void movlps_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg64_t arg1 = { .ps = { 11.11F, 22.22F } };
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
         "movlps %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "mm6", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 56.78F) )
      {
         printf("movlps_2 ... ok\n");
      }
      else
      {
         printf("movlps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 56.78F);
      }
   }
   else
   {
      printf("movlps_2 ... failed\n");
   }

   return;
}

static void movmskps_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, -56.78F, 43.21F, -87.65F } };
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
         "movmskps %%xmm12, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "r9", "xmm12"
      );

      if (result0.sd[0] == 10 )
      {
         printf("movmskps_1 ... ok\n");
      }
      else
      {
         printf("movmskps_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 10);
      }
   }
   else
   {
      printf("movmskps_1 ... failed\n");
   }

   return;
}

static void movntps_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 11.11F, 22.22F, 33.33F, 44.44F } };
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
         "movntps %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 56.78F) && eq_float(result0.ps[2], 43.21F) && eq_float(result0.ps[3], 87.65F) )
      {
         printf("movntps_1 ... ok\n");
      }
      else
      {
         printf("movntps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 56.78F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 43.21F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 87.65F);
      }
   }
   else
   {
      printf("movntps_1 ... failed\n");
   }

   return;
}

static void movntq_1(void)
{
   reg64_t arg0 = { .uq = { 0x0123456789abcdefULL } };
   reg64_t arg1 = { .uq = { 0x1212121234343434ULL } };
   reg64_t result0 = arg1;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movq %1, %%mm6\n"
         "movntq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0x0123456789abcdefULL )
      {
         printf("movntq_1 ... ok\n");
      }
      else
      {
         printf("movntq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0123456789abcdefULL);
      }
   }
   else
   {
      printf("movntq_1 ... failed\n");
   }

   return;
}

static void movss_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 11.11F, 22.22F, 33.33F, 44.44F } };
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
         "movss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 22.22F) && eq_float(result0.ps[2], 33.33F) && eq_float(result0.ps[3], 44.44F) )
      {
         printf("movss_1 ... ok\n");
      }
      else
      {
         printf("movss_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 22.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 33.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 44.44F);
      }
   }
   else
   {
      printf("movss_1 ... failed\n");
   }

   return;
}

static void movss_2(void)
{
   reg32_t arg0 = { .ps = { 12.34F } };
   reg128_t arg1 = { .ps = { 11.11F, 22.22F, 33.33F, 44.44F } };
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
         "movss %2, %%xmm12\n"
         "movlps %%xmm12, %0\n"
         "movhps %%xmm12, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 0.0F) && eq_float(result0.ps[2], 0.0F) && eq_float(result0.ps[3], 0.0F) )
      {
         printf("movss_2 ... ok\n");
      }
      else
      {
         printf("movss_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 0.0F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 0.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 0.0F);
      }
   }
   else
   {
      printf("movss_2 ... failed\n");
   }

   return;
}

static void movss_3(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg32_t arg1 = { .ps = { 11.11F } };
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
         "movss %%xmm12, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1), "m" (state[0])
         : "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) )
      {
         printf("movss_3 ... ok\n");
      }
      else
      {
         printf("movss_3 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
      }
   }
   else
   {
      printf("movss_3 ... failed\n");
   }

   return;
}

static void movups_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 11.11F, 22.22F, 33.33F, 44.44F } };
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
         "movups %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 56.78F) && eq_float(result0.ps[2], 43.21F) && eq_float(result0.ps[3], 87.65F) )
      {
         printf("movups_1 ... ok\n");
      }
      else
      {
         printf("movups_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 56.78F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 43.21F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 87.65F);
      }
   }
   else
   {
      printf("movups_1 ... failed\n");
   }

   return;
}

static void movups_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 11.11F, 22.22F, 33.33F, 44.44F } };
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
         "movups %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 56.78F) && eq_float(result0.ps[2], 43.21F) && eq_float(result0.ps[3], 87.65F) )
      {
         printf("movups_2 ... ok\n");
      }
      else
      {
         printf("movups_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 56.78F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 43.21F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 87.65F);
      }
   }
   else
   {
      printf("movups_2 ... failed\n");
   }

   return;
}

static void mulps_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 5.0F, 4.0F, 3.0F, 2.0F } };
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
         "mulps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 61.70F) && eq_float(result0.ps[1], 227.12F) && eq_float(result0.ps[2], 129.63F) && eq_float(result0.ps[3], 175.30F) )
      {
         printf("mulps_1 ... ok\n");
      }
      else
      {
         printf("mulps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 61.70F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 227.12F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 129.63F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 175.30F);
      }
   }
   else
   {
      printf("mulps_1 ... failed\n");
   }

   return;
}

static void mulps_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 5.0F, 4.0F, 3.0F, 2.0F } };
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
         "mulps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 61.70F) && eq_float(result0.ps[1], 227.12F) && eq_float(result0.ps[2], 129.63F) && eq_float(result0.ps[3], 175.30F) )
      {
         printf("mulps_2 ... ok\n");
      }
      else
      {
         printf("mulps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 61.70F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 227.12F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 129.63F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 175.30F);
      }
   }
   else
   {
      printf("mulps_2 ... failed\n");
   }

   return;
}

static void mulss_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 5.0F, 4.0F, 3.0F, 2.0F } };
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
         "mulss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 61.70F) && eq_float(result0.ps[1], 4.0F) && eq_float(result0.ps[2], 3.0F) && eq_float(result0.ps[3], 2.0F) )
      {
         printf("mulss_1 ... ok\n");
      }
      else
      {
         printf("mulss_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 61.70F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 4.0F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 2.0F);
      }
   }
   else
   {
      printf("mulss_1 ... failed\n");
   }

   return;
}

static void mulss_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 5.0F, 4.0F, 3.0F, 2.0F } };
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
         "mulss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 61.70F) && eq_float(result0.ps[1], 4.0F) && eq_float(result0.ps[2], 3.0F) && eq_float(result0.ps[3], 2.0F) )
      {
         printf("mulss_2 ... ok\n");
      }
      else
      {
         printf("mulss_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 61.70F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 4.0F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 2.0F);
      }
   }
   else
   {
      printf("mulss_2 ... failed\n");
   }

   return;
}

static void orps_1(void)
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
         "orps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfdbb7577edabedefULL && result0.uq[1] == 0xfdbb7577edabedefULL )
      {
         printf("orps_1 ... ok\n");
      }
      else
      {
         printf("orps_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfdbb7577edabedefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfdbb7577edabedefULL);
      }
   }
   else
   {
      printf("orps_1 ... failed\n");
   }

   return;
}

static void orps_2(void)
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
         "orps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfdbb7577edabedefULL && result0.uq[1] == 0xfdbb7577edabedefULL )
      {
         printf("orps_2 ... ok\n");
      }
      else
      {
         printf("orps_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfdbb7577edabedefULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfdbb7577edabedefULL);
      }
   }
   else
   {
      printf("orps_2 ... failed\n");
   }

   return;
}

static void pavgb_1(void)
{
   reg64_t arg0 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
   reg64_t arg1 = { .ub = { 15U, 25U, 35U, 45U, 55U, 65U, 75U, 85U } };
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
         "pavgb %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 13U && result0.ub[1] == 24U && result0.ub[2] == 34U && result0.ub[3] == 45U && result0.ub[4] == 55U && result0.ub[5] == 66U && result0.ub[6] == 76U && result0.ub[7] == 87U )
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
   reg64_t arg0 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
   reg64_t arg1 = { .ub = { 15U, 25U, 35U, 45U, 55U, 65U, 75U, 85U } };
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
         "pavgb %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 13U && result0.ub[1] == 24U && result0.ub[2] == 34U && result0.ub[3] == 45U && result0.ub[4] == 55U && result0.ub[5] == 66U && result0.ub[6] == 76U && result0.ub[7] == 87U )
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
   reg64_t arg0 = { .uw = { 1122, 3344, 5566, 7788 } };
   reg64_t arg1 = { .uw = { 1525, 3545, 5565, 7585 } };
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
         "pavgw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 1324 && result0.uw[1] == 3445 && result0.uw[2] == 5566 && result0.uw[3] == 7687 )
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
   reg64_t arg0 = { .uw = { 1122, 3344, 5566, 7788 } };
   reg64_t arg1 = { .uw = { 1525, 3545, 5565, 7585 } };
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
         "pavgw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 1324 && result0.uw[1] == 3445 && result0.uw[2] == 5566 && result0.uw[3] == 7687 )
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
      }
   }
   else
   {
      printf("pavgw_2 ... failed\n");
   }

   return;
}

static void pextrw_1(void)
{
   reg64_t arg1 = { .uw = { 1234, 5678, 4321, 8765 } };
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
         "movq %1, %%mm6\n"
         "movl %2, %%r9d\n"
         "pextrw $0, %%mm6, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "r9", "mm6"
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
   reg64_t arg1 = { .uw = { 1234, 5678, 4321, 8765 } };
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
         "movq %1, %%mm6\n"
         "movl %2, %%r9d\n"
         "pextrw $1, %%mm6, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "r9", "mm6"
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
   reg64_t arg1 = { .uw = { 1234, 5678, 4321, 8765 } };
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
         "movq %1, %%mm6\n"
         "movl %2, %%r9d\n"
         "pextrw $2, %%mm6, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "r9", "mm6"
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
   reg64_t arg1 = { .uw = { 1234, 5678, 4321, 8765 } };
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
         "movq %1, %%mm6\n"
         "movl %2, %%r9d\n"
         "pextrw $3, %%mm6, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "r9", "mm6"
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

static void pinsrw_1(void)
{
   reg32_t arg1 = { .ud = { 0xffffffff } };
   reg64_t arg2 = { .uw = { 1234, 5678, 4321, 8765 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %1, %%r9d\n"
         "movq %2, %%mm6\n"
         "pinsrw $0, %%r9d, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "mm6", "r9"
      );

      if (result0.uw[0] == 65535 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 8765 )
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
   reg64_t arg2 = { .uw = { 1234, 5678, 4321, 8765 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %1, %%r9d\n"
         "movq %2, %%mm6\n"
         "pinsrw $1, %%r9d, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "mm6", "r9"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 65535 && result0.uw[2] == 4321 && result0.uw[3] == 8765 )
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
   reg64_t arg2 = { .uw = { 1234, 5678, 4321, 8765 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %1, %%r9d\n"
         "movq %2, %%mm6\n"
         "pinsrw $2, %%r9d, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "mm6", "r9"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 65535 && result0.uw[3] == 8765 )
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
   reg64_t arg2 = { .uw = { 1234, 5678, 4321, 8765 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movl %1, %%r9d\n"
         "movq %2, %%mm6\n"
         "pinsrw $3, %%r9d, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "mm6", "r9"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 65535 )
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
   reg16_t arg1 = { .uw = { 0xffff } };
   reg64_t arg2 = { .uw = { 1234, 5678, 4321, 8765 } };
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
         "pinsrw $0, %1, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "mm6"
      );

      if (result0.uw[0] == 65535 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 8765 )
      {
         printf("pinsrw_5 ... ok\n");
      }
      else
      {
         printf("pinsrw_5 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 65535);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
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
   reg16_t arg1 = { .uw = { 0xffff } };
   reg64_t arg2 = { .uw = { 1234, 5678, 4321, 8765 } };
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
         "pinsrw $1, %1, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "mm6"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 65535 && result0.uw[2] == 4321 && result0.uw[3] == 8765 )
      {
         printf("pinsrw_6 ... ok\n");
      }
      else
      {
         printf("pinsrw_6 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 65535);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
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
   reg16_t arg1 = { .uw = { 0xffff } };
   reg64_t arg2 = { .uw = { 1234, 5678, 4321, 8765 } };
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
         "pinsrw $2, %1, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "mm6"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 65535 && result0.uw[3] == 8765 )
      {
         printf("pinsrw_7 ... ok\n");
      }
      else
      {
         printf("pinsrw_7 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 65535);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 8765);
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
   reg16_t arg1 = { .uw = { 0xffff } };
   reg64_t arg2 = { .uw = { 1234, 5678, 4321, 8765 } };
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
         "pinsrw $3, %1, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "mm6"
      );

      if (result0.uw[0] == 1234 && result0.uw[1] == 5678 && result0.uw[2] == 4321 && result0.uw[3] == 65535 )
      {
         printf("pinsrw_8 ... ok\n");
      }
      else
      {
         printf("pinsrw_8 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 1234);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 5678);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 4321);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 65535);
      }
   }
   else
   {
      printf("pinsrw_8 ... failed\n");
   }

   return;
}

static void pmaxsw_1(void)
{
   reg64_t arg0 = { .sw = { -1, 2, -3, 4 } };
   reg64_t arg1 = { .sw = { 2, -3, 4, -5 } };
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
         "pmaxsw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 2 && result0.sw[1] == 2 && result0.sw[2] == 4 && result0.sw[3] == 4 )
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
   reg64_t arg0 = { .sw = { -1, 2, -3, 4 } };
   reg64_t arg1 = { .sw = { 2, -3, 4, -5 } };
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
         "pmaxsw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 2 && result0.sw[1] == 2 && result0.sw[2] == 4 && result0.sw[3] == 4 )
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
   reg64_t arg0 = { .ub = { 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U } };
   reg64_t arg1 = { .ub = { 8U, 7U, 6U, 5U, 4U, 3U, 2U, 1U } };
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
         "pmaxub %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 8U && result0.ub[1] == 7U && result0.ub[2] == 6U && result0.ub[3] == 5U && result0.ub[4] == 5U && result0.ub[5] == 6U && result0.ub[6] == 7U && result0.ub[7] == 8U )
      {
         printf("pmaxub_1 ... ok\n");
      }
      else
      {
         printf("pmaxub_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 8U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 7U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 6U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 5U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 5U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 6U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 7U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 8U);
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
   reg64_t arg0 = { .ub = { 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U } };
   reg64_t arg1 = { .ub = { 8U, 7U, 6U, 5U, 4U, 3U, 2U, 1U } };
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
         "pmaxub %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 8U && result0.ub[1] == 7U && result0.ub[2] == 6U && result0.ub[3] == 5U && result0.ub[4] == 5U && result0.ub[5] == 6U && result0.ub[6] == 7U && result0.ub[7] == 8U )
      {
         printf("pmaxub_2 ... ok\n");
      }
      else
      {
         printf("pmaxub_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 8U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 7U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 6U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 5U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 5U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 6U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 7U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 8U);
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
   reg64_t arg0 = { .sw = { -1, 2, -3, 4 } };
   reg64_t arg1 = { .sw = { 2, -3, 4, -5 } };
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
         "pminsw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == -1 && result0.sw[1] == -3 && result0.sw[2] == -3 && result0.sw[3] == -5 )
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
   reg64_t arg0 = { .sw = { -1, 2, -3, 4 } };
   reg64_t arg1 = { .sw = { 2, -3, 4, -5 } };
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
         "pminsw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == -1 && result0.sw[1] == -3 && result0.sw[2] == -3 && result0.sw[3] == -5 )
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
   reg64_t arg0 = { .ub = { 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U } };
   reg64_t arg1 = { .ub = { 8U, 7U, 6U, 5U, 4U, 3U, 2U, 1U } };
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
         "pminub %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 1U && result0.ub[1] == 2U && result0.ub[2] == 3U && result0.ub[3] == 4U && result0.ub[4] == 4U && result0.ub[5] == 3U && result0.ub[6] == 2U && result0.ub[7] == 1U )
      {
         printf("pminub_1 ... ok\n");
      }
      else
      {
         printf("pminub_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 1U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 2U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 3U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 4U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 4U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 3U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 2U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 1U);
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
   reg64_t arg0 = { .ub = { 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U } };
   reg64_t arg1 = { .ub = { 8U, 7U, 6U, 5U, 4U, 3U, 2U, 1U } };
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
         "pminub %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 1U && result0.ub[1] == 2U && result0.ub[2] == 3U && result0.ub[3] == 4U && result0.ub[4] == 4U && result0.ub[5] == 3U && result0.ub[6] == 2U && result0.ub[7] == 1U )
      {
         printf("pminub_2 ... ok\n");
      }
      else
      {
         printf("pminub_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 1U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 2U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 3U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 4U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 4U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 3U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 2U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 1U);
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
   reg64_t arg0 = { .uq = { 0x8000000080008088ULL } };
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
         "movq %1, %%mm6\n"
         "movl %2, %%r9d\n"
         "pmovmskb %%mm6, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "r9", "mm6"
      );

      if (result0.ud[0] == 0x8b )
      {
         printf("pmovmskb_1 ... ok\n");
      }
      else
      {
         printf("pmovmskb_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x8b);
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
   reg64_t arg0 = { .uw = { 1111, 2222, 3333, 4444 } };
   reg64_t arg1 = { .uw = { 5555, 6666, 7777, 8888 } };
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
         "pmulhuw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0x005e && result0.uw[1] == 0x00e2 && result0.uw[2] == 0x018b && result0.uw[3] == 0x025a )
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
   reg64_t arg0 = { .uw = { 1111, 2222, 3333, 4444 } };
   reg64_t arg1 = { .uw = { 5555, 6666, 7777, 8888 } };
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
         "pmulhuw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0x005e && result0.uw[1] == 0x00e2 && result0.uw[2] == 0x018b && result0.uw[3] == 0x025a )
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
      }
   }
   else
   {
      printf("pmulhuw_2 ... failed\n");
   }

   return;
}

static void psadbw_1(void)
{
   reg64_t arg0 = { .ub = { 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U } };
   reg64_t arg1 = { .ub = { 8U, 7U, 6U, 5U, 4U, 3U, 2U, 1U } };
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
         "psadbw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 32 && result0.sw[1] == 0 && result0.sw[2] == 0 && result0.sw[3] == 0 )
      {
         printf("psadbw_1 ... ok\n");
      }
      else
      {
         printf("psadbw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 32);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 0);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 0);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 0);
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
   reg64_t arg0 = { .ub = { 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U } };
   reg64_t arg1 = { .ub = { 8U, 7U, 6U, 5U, 4U, 3U, 2U, 1U } };
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
         "psadbw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 32 && result0.sw[1] == 0 && result0.sw[2] == 0 && result0.sw[3] == 0 )
      {
         printf("psadbw_2 ... ok\n");
      }
      else
      {
         printf("psadbw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 32);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 0);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 0);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 0);
      }
   }
   else
   {
      printf("psadbw_2 ... failed\n");
   }

   return;
}

static void pshufw_1(void)
{
   reg64_t arg1 = { .sw = { 11, 22, 33, 44 } };
   reg64_t arg2 = { .sw = { 0, 0, 0, 0 } };
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
         "pshufw $0x1b, %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 44 && result0.sw[1] == 33 && result0.sw[2] == 22 && result0.sw[3] == 11 )
      {
         printf("pshufw_1 ... ok\n");
      }
      else
      {
         printf("pshufw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 44);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 33);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 22);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 11);
      }
   }
   else
   {
      printf("pshufw_1 ... failed\n");
   }

   return;
}

static void pshufw_2(void)
{
   reg64_t arg1 = { .sw = { 11, 22, 33, 44 } };
   reg64_t arg2 = { .sw = { 0, 0, 0, 0 } };
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
         "pshufw $0x1b, %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (arg2), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 44 && result0.sw[1] == 33 && result0.sw[2] == 22 && result0.sw[3] == 11 )
      {
         printf("pshufw_2 ... ok\n");
      }
      else
      {
         printf("pshufw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], 44);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], 33);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 22);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 11);
      }
   }
   else
   {
      printf("pshufw_2 ... failed\n");
   }

   return;
}

static void rcpps_1(void)
{
   reg128_t arg0 = { .ps = { 2.0F, 4.0F, 0.5F, 0.25F } };
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
         "rcpps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 0.5F) && eq_float(result0.ps[1], 0.25F) && eq_float(result0.ps[2], 2.0F) && eq_float(result0.ps[3], 4.0F) )
      {
         printf("rcpps_1 ... ok\n");
      }
      else
      {
         printf("rcpps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.5F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 0.25F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 2.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.0F);
      }
   }
   else
   {
      printf("rcpps_1 ... failed\n");
   }

   return;
}

static void rcpps_2(void)
{
   reg128_t arg0 = { .ps = { 2.0F, 4.0F, 0.5F, 0.25F } };
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
         "rcpps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 0.5F) && eq_float(result0.ps[1], 0.25F) && eq_float(result0.ps[2], 2.0F) && eq_float(result0.ps[3], 4.0F) )
      {
         printf("rcpps_2 ... ok\n");
      }
      else
      {
         printf("rcpps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.5F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 0.25F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 2.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.0F);
      }
   }
   else
   {
      printf("rcpps_2 ... failed\n");
   }

   return;
}

static void rcpss_1(void)
{
   reg128_t arg0 = { .ps = { 2.0F, 4.0F, 0.5F, 0.25F } };
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
         "rcpss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 0.5F) && eq_float(result0.ps[1], 2.22F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 4.44F) )
      {
         printf("rcpss_1 ... ok\n");
      }
      else
      {
         printf("rcpss_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.5F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 2.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.44F);
      }
   }
   else
   {
      printf("rcpss_1 ... failed\n");
   }

   return;
}

static void rcpss_2(void)
{
   reg128_t arg0 = { .ps = { 2.0F, 4.0F, 0.5F, 0.25F } };
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
         "rcpss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 0.5F) && eq_float(result0.ps[1], 2.22F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 4.44F) )
      {
         printf("rcpss_2 ... ok\n");
      }
      else
      {
         printf("rcpss_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.5F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 2.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.44F);
      }
   }
   else
   {
      printf("rcpss_2 ... failed\n");
   }

   return;
}

static void rsqrtps_1(void)
{
   reg128_t arg0 = { .ps = { 4.0F, 16.0F, 25.0F, 64.0F } };
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
         "rsqrtps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 0.499878F) && eq_float(result0.ps[1], 0.249939F) && eq_float(result0.ps[2], 0.199982F) && eq_float(result0.ps[3], 0.124969F) )
      {
         printf("rsqrtps_1 ... ok\n");
      }
      else
      {
         printf("rsqrtps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.499878F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 0.249939F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 0.199982F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 0.124969F);
      }
   }
   else
   {
      printf("rsqrtps_1 ... failed\n");
   }

   return;
}

static void rsqrtps_2(void)
{
   reg128_t arg0 = { .ps = { 4.0F, 16.0F, 25.0F, 64.0F } };
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
         "rsqrtps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 0.499878F) && eq_float(result0.ps[1], 0.249939F) && eq_float(result0.ps[2], 0.199982F) && eq_float(result0.ps[3], 0.124969F) )
      {
         printf("rsqrtps_2 ... ok\n");
      }
      else
      {
         printf("rsqrtps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.499878F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 0.249939F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 0.199982F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 0.124969F);
      }
   }
   else
   {
      printf("rsqrtps_2 ... failed\n");
   }

   return;
}

static void rsqrtss_1(void)
{
   reg128_t arg0 = { .ps = { 16.0F, 5.55F, 6.66F, 7.77F } };
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
         "rsqrtss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 0.249939F) && eq_float(result0.ps[1], 2.22F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 4.44F) )
      {
         printf("rsqrtss_1 ... ok\n");
      }
      else
      {
         printf("rsqrtss_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.249939F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 2.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.44F);
      }
   }
   else
   {
      printf("rsqrtss_1 ... failed\n");
   }

   return;
}

static void rsqrtss_2(void)
{
   reg128_t arg0 = { .ps = { 16.0F, 5.55F, 6.66F, 7.77F } };
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
         "rsqrtss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 0.249939F) && eq_float(result0.ps[1], 2.22F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 4.44F) )
      {
         printf("rsqrtss_2 ... ok\n");
      }
      else
      {
         printf("rsqrtss_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 0.249939F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 2.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.44F);
      }
   }
   else
   {
      printf("rsqrtss_2 ... failed\n");
   }

   return;
}

static void sfence_1(void)
{
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "sfence\n"
         "cld\n"
         :
         : "m" (state[0])

      );

      printf("sfence_1 ... ok\n");
   }
   else
   {
      printf("sfence_1 ... failed\n");
   }

   return;
}

static void shufps_1(void)
{
   reg128_t arg1 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg2 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
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
         "shufps $0xe4, %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 12.34F) && eq_float(result0.ps[1], 56.78F) && eq_float(result0.ps[2], 43.21F) && eq_float(result0.ps[3], 87.65F) )
      {
         printf("shufps_1 ... ok\n");
      }
      else
      {
         printf("shufps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 12.34F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 56.78F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 43.21F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 87.65F);
      }
   }
   else
   {
      printf("shufps_1 ... failed\n");
   }

   return;
}

static void shufps_2(void)
{
   reg128_t arg1 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg2 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
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
         "shufps $0xb1, %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg1), "m" (arg2.uq[0]), "m" (arg2.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 56.78F) && eq_float(result0.ps[1], 12.34F) && eq_float(result0.ps[2], 87.65F) && eq_float(result0.ps[3], 43.21F) )
      {
         printf("shufps_2 ... ok\n");
      }
      else
      {
         printf("shufps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 56.78F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 12.34F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 87.65F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 43.21F);
      }
   }
   else
   {
      printf("shufps_2 ... failed\n");
   }

   return;
}

static void sqrtps_1(void)
{
   reg128_t arg0 = { .ps = { 16.0F, 25.0F, 36.0F, 49.0F } };
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
         "sqrtps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 4.0F) && eq_float(result0.ps[1], 5.0F) && eq_float(result0.ps[2], 6.0F) && eq_float(result0.ps[3], 7.0F) )
      {
         printf("sqrtps_1 ... ok\n");
      }
      else
      {
         printf("sqrtps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 4.0F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 5.0F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 6.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 7.0F);
      }
   }
   else
   {
      printf("sqrtps_1 ... failed\n");
   }

   return;
}

static void sqrtps_2(void)
{
   reg128_t arg0 = { .ps = { 16.0F, 25.0F, 36.0F, 49.0F } };
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
         "sqrtps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 4.0F) && eq_float(result0.ps[1], 5.0F) && eq_float(result0.ps[2], 6.0F) && eq_float(result0.ps[3], 7.0F) )
      {
         printf("sqrtps_2 ... ok\n");
      }
      else
      {
         printf("sqrtps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 4.0F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 5.0F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 6.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 7.0F);
      }
   }
   else
   {
      printf("sqrtps_2 ... failed\n");
   }

   return;
}

static void sqrtss_1(void)
{
   reg128_t arg0 = { .ps = { 16.0F, 5.55F, 6.66F, 7.77F } };
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
         "sqrtss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 4.0F) && eq_float(result0.ps[1], 2.22F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 4.44F) )
      {
         printf("sqrtss_1 ... ok\n");
      }
      else
      {
         printf("sqrtss_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 4.0F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 2.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.44F);
      }
   }
   else
   {
      printf("sqrtss_1 ... failed\n");
   }

   return;
}

static void sqrtss_2(void)
{
   reg128_t arg0 = { .ps = { 16.0F, 5.55F, 6.66F, 7.77F } };
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
         "sqrtss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 4.0F) && eq_float(result0.ps[1], 2.22F) && eq_float(result0.ps[2], 3.33F) && eq_float(result0.ps[3], 4.44F) )
      {
         printf("sqrtss_2 ... ok\n");
      }
      else
      {
         printf("sqrtss_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 4.0F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 2.22F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 3.33F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 4.44F);
      }
   }
   else
   {
      printf("sqrtss_2 ... failed\n");
   }

   return;
}

static void subps_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.77F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 44.0F, 33.0F, 22.0F, 11.0F } };
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
         "subps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 31.66F) && eq_float(result0.ps[1], -23.77F) && eq_float(result0.ps[2], -21.21F) && eq_float(result0.ps[3], -76.65F) )
      {
         printf("subps_1 ... ok\n");
      }
      else
      {
         printf("subps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 31.66F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], -23.77F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], -21.21F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], -76.65F);
      }
   }
   else
   {
      printf("subps_1 ... failed\n");
   }

   return;
}

static void subps_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.77F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 44.0F, 33.0F, 22.0F, 11.0F } };
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
         "subps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 31.66F) && eq_float(result0.ps[1], -23.77F) && eq_float(result0.ps[2], -21.21F) && eq_float(result0.ps[3], -76.65F) )
      {
         printf("subps_2 ... ok\n");
      }
      else
      {
         printf("subps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 31.66F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], -23.77F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], -21.21F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], -76.65F);
      }
   }
   else
   {
      printf("subps_2 ... failed\n");
   }

   return;
}

static void subss_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.77F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 44.0F, 33.0F, 22.0F, 11.0F } };
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
         "subss %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 31.66F) && eq_float(result0.ps[1], 33.0F) && eq_float(result0.ps[2], 22.0F) && eq_float(result0.ps[3], 11.0F) )
      {
         printf("subss_1 ... ok\n");
      }
      else
      {
         printf("subss_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 31.66F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 33.0F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 22.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 11.0F);
      }
   }
   else
   {
      printf("subss_1 ... failed\n");
   }

   return;
}

static void subss_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.77F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 44.0F, 33.0F, 22.0F, 11.0F } };
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
         "subss %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 31.66F) && eq_float(result0.ps[1], 33.0F) && eq_float(result0.ps[2], 22.0F) && eq_float(result0.ps[3], 11.0F) )
      {
         printf("subss_2 ... ok\n");
      }
      else
      {
         printf("subss_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 31.66F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 33.0F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 22.0F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 11.0F);
      }
   }
   else
   {
      printf("subss_2 ... failed\n");
   }

   return;
}

static void ucomiss_1(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 0.0F } };
   reg128_t arg1 = { .ps = { 234.5679F, 0.0F } };
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
         "ucomiss %%xmm12, %%xmm13\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "xmm13"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000000UL )
      {
         printf("ucomiss_1 ... ok\n");
      }
      else
      {
         printf("ucomiss_1 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("ucomiss_1 ... failed\n");
   }

   return;
}

static void ucomiss_2(void)
{
   reg32_t arg0 = { .ps = { 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5679F, 0.0F } };
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
         "ucomiss %1, %%xmm12\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000000UL )
      {
         printf("ucomiss_2 ... ok\n");
      }
      else
      {
         printf("ucomiss_2 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000000UL);
      }
   }
   else
   {
      printf("ucomiss_2 ... failed\n");
   }

   return;
}

static void ucomiss_3(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 0.0F } };
   reg128_t arg1 = { .ps = { 234.5677F, 0.0F } };
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
         "ucomiss %%xmm12, %%xmm13\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "xmm13"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000001UL )
      {
         printf("ucomiss_3 ... ok\n");
      }
      else
      {
         printf("ucomiss_3 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("ucomiss_3 ... failed\n");
   }

   return;
}

static void ucomiss_4(void)
{
   reg32_t arg0 = { .ps = { 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5677F, 0.0F } };
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
         "ucomiss %1, %%xmm12\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000001UL )
      {
         printf("ucomiss_4 ... ok\n");
      }
      else
      {
         printf("ucomiss_4 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000001UL);
      }
   }
   else
   {
      printf("ucomiss_4 ... failed\n");
   }

   return;
}

static void ucomiss_5(void)
{
   reg128_t arg0 = { .ps = { 234.5678F, 0.0F } };
   reg128_t arg1 = { .ps = { 234.5678F, 0.0F } };
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
         "ucomiss %%xmm12, %%xmm13\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12", "xmm13"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000040UL )
      {
         printf("ucomiss_5 ... ok\n");
      }
      else
      {
         printf("ucomiss_5 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("ucomiss_5 ... failed\n");
   }

   return;
}

static void ucomiss_6(void)
{
   reg32_t arg0 = { .ps = { 234.5678F } };
   reg128_t arg1 = { .ps = { 234.5678F, 0.0F } };
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
         "ucomiss %1, %%xmm12\n"
         "pushfq\n"
         "popq %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm12"
      );

      if ((result0.uq[0] & 0x00000000000008d5UL) == 0x0000000000000040UL )
      {
         printf("ucomiss_6 ... ok\n");
      }
      else
      {
         printf("ucomiss_6 ... not ok\n");
         printf("  rflags & 0x%lx = 0x%lx (expected 0x%lx)\n", 0x00000000000008d5UL, result0.ud[0] & 0x00000000000008d5UL, 0x0000000000000040UL);
      }
   }
   else
   {
      printf("ucomiss_6 ... failed\n");
   }

   return;
}

static void unpckhps_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 11.22F, 33.44F, 55.66F, 77.88F } };
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
         "unpckhps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 55.66F) && eq_float(result0.ps[1], 43.21F) && eq_float(result0.ps[2], 77.88F) && eq_float(result0.ps[3], 87.65F) )
      {
         printf("unpckhps_1 ... ok\n");
      }
      else
      {
         printf("unpckhps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 55.66F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 43.21F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 77.88F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 87.65F);
      }
   }
   else
   {
      printf("unpckhps_1 ... failed\n");
   }

   return;
}

static void unpckhps_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 11.22F, 33.44F, 55.66F, 77.88F } };
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
         "unpckhps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 55.66F) && eq_float(result0.ps[1], 43.21F) && eq_float(result0.ps[2], 77.88F) && eq_float(result0.ps[3], 87.65F) )
      {
         printf("unpckhps_2 ... ok\n");
      }
      else
      {
         printf("unpckhps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 55.66F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 43.21F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 77.88F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 87.65F);
      }
   }
   else
   {
      printf("unpckhps_2 ... failed\n");
   }

   return;
}

static void unpcklps_1(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 11.22F, 33.44F, 55.66F, 77.88F } };
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
         "unpcklps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 11.22F) && eq_float(result0.ps[1], 12.34F) && eq_float(result0.ps[2], 33.44F) && eq_float(result0.ps[3], 56.78F) )
      {
         printf("unpcklps_1 ... ok\n");
      }
      else
      {
         printf("unpcklps_1 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 11.22F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 12.34F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 33.44F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 56.78F);
      }
   }
   else
   {
      printf("unpcklps_1 ... failed\n");
   }

   return;
}

static void unpcklps_2(void)
{
   reg128_t arg0 = { .ps = { 12.34F, 56.78F, 43.21F, 87.65F } };
   reg128_t arg1 = { .ps = { 11.22F, 33.44F, 55.66F, 77.88F } };
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
         "unpcklps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (eq_float(result0.ps[0], 11.22F) && eq_float(result0.ps[1], 12.34F) && eq_float(result0.ps[2], 33.44F) && eq_float(result0.ps[3], 56.78F) )
      {
         printf("unpcklps_2 ... ok\n");
      }
      else
      {
         printf("unpcklps_2 ... not ok\n");
         printf("  result0.ps[0] = %.16g (expected %.16g)\n", result0.ps[0], 11.22F);
         printf("  result0.ps[1] = %.16g (expected %.16g)\n", result0.ps[1], 12.34F);
         printf("  result0.ps[2] = %.16g (expected %.16g)\n", result0.ps[2], 33.44F);
         printf("  result0.ps[3] = %.16g (expected %.16g)\n", result0.ps[3], 56.78F);
      }
   }
   else
   {
      printf("unpcklps_2 ... failed\n");
   }

   return;
}

static void xorps_1(void)
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
         "xorps %%xmm12, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0.uq[0]), "m" (arg0.uq[1]), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfc9a30566503a9cfULL && result0.uq[1] == 0xfc9a30566503a9cfULL )
      {
         printf("xorps_1 ... ok\n");
      }
      else
      {
         printf("xorps_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc9a30566503a9cfULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfc9a30566503a9cfULL);
      }
   }
   else
   {
      printf("xorps_1 ... failed\n");
   }

   return;
}

static void xorps_2(void)
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
         "xorps %2, %%xmm13\n"
         "movlps %%xmm13, %0\n"
         "movhps %%xmm13, %1\n"
         "cld\n"
         : "=m" (result0.uq[0]), "=m" (result0.uq[1])
         : "m" (arg0), "m" (arg1.uq[0]), "m" (arg1.uq[1]), "m" (state[0])
         : "xmm13", "xmm12"
      );

      if (result0.uq[0] == 0xfc9a30566503a9cfULL && result0.uq[1] == 0xfc9a30566503a9cfULL )
      {
         printf("xorps_2 ... ok\n");
      }
      else
      {
         printf("xorps_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc9a30566503a9cfULL);
         printf("  result0.uq[1] = %llu (expected %llu)\n", result0.uq[1], 0xfc9a30566503a9cfULL);
      }
   }
   else
   {
      printf("xorps_2 ... failed\n");
   }

   return;
}

int main(int argc, char **argv)
{
   signal(SIGILL, handle_sigill);

   addps_1();
   addps_2();
   addss_1();
   addss_2();
   andnps_1();
   andnps_2();
   andps_1();
   andps_2();
   cmpeqps_1();
   cmpeqps_2();
   cmpeqss_1();
   cmpeqss_2();
   cmpleps_1();
   cmpleps_2();
   cmpless_1();
   cmpless_2();
   cmpltps_1();
   cmpltps_2();
   cmpltss_1();
   cmpltss_2();
   cmpunordps_1();
   cmpunordps_2();
   cmpunordss_1();
   cmpunordss_2();
   cmpneqps_1();
   cmpneqps_2();
   cmpneqss_1();
   cmpneqss_2();
   cmpnleps_1();
   cmpnleps_2();
   cmpnless_1();
   cmpnless_2();
   cmpnltps_1();
   cmpnltps_2();
   cmpnltss_1();
   cmpnltss_2();
   cmpordps_1();
   cmpordps_2();
   cmpordss_1();
   cmpordss_2();
   comiss_1();
   comiss_2();
   comiss_3();
   comiss_4();
   comiss_5();
   comiss_6();
   cvtpi2ps_1();
   cvtpi2ps_2();
   cvtps2pi_1();
   cvtps2pi_2();
   cvtsi2ss_1();
   cvtsi2ss_2();
   cvtss2si_1();
   cvtss2si_2();
   cvttps2pi_1();
   cvttps2pi_2();
   cvttss2si_1();
   cvttss2si_2();
   divps_1();
   divps_2();
   divss_1();
   divss_2();
   maxps_1();
   maxps_2();
   maxss_1();
   maxss_2();
   minps_1();
   minps_2();
   minss_1();
   minss_2();
   movaps_1();
   movaps_2();
   movhlps_1();
   movhps_1();
   movhps_2();
   movlhps_1();
   movlps_1();
   movlps_2();
   movmskps_1();
   movntps_1();
   movntq_1();
   movss_1();
   movss_2();
   movss_3();
   movups_1();
   movups_2();
   mulps_1();
   mulps_2();
   mulss_1();
   mulss_2();
   orps_1();
   orps_2();
   pavgb_1();
   pavgb_2();
   pavgw_1();
   pavgw_2();
   pextrw_1();
   pextrw_2();
   pextrw_3();
   pextrw_4();
   pinsrw_1();
   pinsrw_2();
   pinsrw_3();
   pinsrw_4();
   pinsrw_5();
   pinsrw_6();
   pinsrw_7();
   pinsrw_8();
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
   psadbw_1();
   psadbw_2();
   pshufw_1();
   pshufw_2();
   rcpps_1();
   rcpps_2();
   rcpss_1();
   rcpss_2();
   rsqrtps_1();
   rsqrtps_2();
   rsqrtss_1();
   rsqrtss_2();
   sfence_1();
   shufps_1();
   shufps_2();
   sqrtps_1();
   sqrtps_2();
   sqrtss_1();
   sqrtss_2();
   subps_1();
   subps_2();
   subss_1();
   subss_2();
   ucomiss_1();
   ucomiss_2();
   ucomiss_3();
   ucomiss_4();
   ucomiss_5();
   ucomiss_6();
   unpckhps_1();
   unpckhps_2();
   unpcklps_1();
   unpcklps_2();
   xorps_1();
   xorps_2();

   exit(0);
}
