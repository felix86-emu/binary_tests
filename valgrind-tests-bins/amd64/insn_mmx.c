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

static void movd_1(void)
{
   reg64_t arg0 = { .sd = { 1234, 5678 } };
   reg64_t arg1 = { .sd = { 1111, 2222 } };
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
         "movq %2, %%r9\n"
         "movd %%mm6, %%r9\n"
         "movq %%r9, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "r9", "mm6"
      );

      if (result0.sd[0] == 1234 && result0.sd[1] == 5678 )
      {
         printf("movd_1 ... ok\n");
      }
      else
      {
         printf("movd_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 5678);
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
   reg64_t arg0 = { .sd = { 1234, 5678 } };
   reg64_t arg1 = { .sd = { 1111, 2222 } };
   reg64_t result0;
   char state[108];

   if (sigsetjmp(catchpoint, 1) == 0)
   {
      asm(
         "ffree %%st(7)\n"
         "ffree %%st(6)\n"
         "ffree %%st(5)\n"
         "ffree %%st(4)\n"
         "movq %1, %%r9\n"
         "movq %2, %%mm6\n"
         "movd %%r9, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm6", "r9"
      );

      if (result0.sd[0] == 1234 && result0.sd[1] == 5678 )
      {
         printf("movd_2 ... ok\n");
      }
      else
      {
         printf("movd_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 5678);
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
   reg32_t arg0 = { .sd = { 1234 } };
   reg64_t arg1 = { .sd = { 1111, 2222 } };
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
         "movd %%r9d, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm6", "r9"
      );

      if (result0.sd[0] == 1234 && result0.sd[1] == 0 )
      {
         printf("movd_3 ... ok\n");
      }
      else
      {
         printf("movd_3 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 0);
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
   reg64_t arg0 = { .sd = { 1234, 2222 } };
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
         "movq %1, %%mm6\n"
         "movl %2, %%r9d\n"
         "movd %%mm6, %%r9d\n"
         "movl %%r9d, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "r9", "mm6"
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

static void movd_5(void)
{
   reg32_t arg0 = { .sd = { 1234 } };
   reg64_t arg1 = { .sd = { 1111, 2222 } };
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
         "movd %1, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm6"
      );

      if (result0.sd[0] == 1234 && result0.sd[1] == 0 )
      {
         printf("movd_5 ... ok\n");
      }
      else
      {
         printf("movd_5 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 0);
      }
   }
   else
   {
      printf("movd_5 ... failed\n");
   }

   return;
}

static void movd_6(void)
{
   reg64_t arg0 = { .sd = { 1234, 2222 } };
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
         "movq %1, %%mm6\n"
         "movd %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm6"
      );

      if (result0.sd[0] == 1234 )
      {
         printf("movd_6 ... ok\n");
      }
      else
      {
         printf("movd_6 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 1234);
      }
   }
   else
   {
      printf("movd_6 ... failed\n");
   }

   return;
}

static void movq_1(void)
{
   reg64_t arg0 = { .uq = { 0x012345678abcdefULL } };
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
         "movq %2, %%mm7\n"
         "movq %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0x012345678abcdefULL )
      {
         printf("movq_1 ... ok\n");
      }
      else
      {
         printf("movq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x012345678abcdefULL);
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
   reg64_t arg0 = { .uq = { 0x012345678abcdefULL } };
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
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0x012345678abcdefULL )
      {
         printf("movq_2 ... ok\n");
      }
      else
      {
         printf("movq_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x012345678abcdefULL);
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
   reg64_t arg0 = { .uq = { 0x012345678abcdefULL } };
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
         "movq %1, %%mm6\n"
         "movq %2, %%mm7\n"
         "movq %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0x012345678abcdefULL )
      {
         printf("movq_3 ... ok\n");
      }
      else
      {
         printf("movq_3 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x012345678abcdefULL);
      }
   }
   else
   {
      printf("movq_3 ... failed\n");
   }

   return;
}

static void packssdw_1(void)
{
   reg64_t arg0 = { .sd = { 12345, 123456 } };
   reg64_t arg1 = { .sd = { -12345, -123456 } };
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
         "packssdw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == -12345 && result0.sw[1] == -32768 && result0.sw[2] == 12345 && result0.sw[3] == 32767 )
      {
         printf("packssdw_1 ... ok\n");
      }
      else
      {
         printf("packssdw_1 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -12345);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], -32768);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 12345);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 32767);
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
   reg64_t arg0 = { .sd = { 12345, 123456 } };
   reg64_t arg1 = { .sd = { -12345, -123456 } };
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
         "packssdw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == -12345 && result0.sw[1] == -32768 && result0.sw[2] == 12345 && result0.sw[3] == 32767 )
      {
         printf("packssdw_2 ... ok\n");
      }
      else
      {
         printf("packssdw_2 ... not ok\n");
         printf("  result0.sw[0] = %d (expected %d)\n", result0.sw[0], -12345);
         printf("  result0.sw[1] = %d (expected %d)\n", result0.sw[1], -32768);
         printf("  result0.sw[2] = %d (expected %d)\n", result0.sw[2], 12345);
         printf("  result0.sw[3] = %d (expected %d)\n", result0.sw[3], 32767);
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
   reg64_t arg0 = { .sw = { 123, -123, 1234, -1234 } };
   reg64_t arg1 = { .sw = { 21, -21, 321, -321 } };
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
         "packsswb %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sb[0] == 21 && result0.sb[1] == -21 && result0.sb[2] == 127 && result0.sb[3] == -128 && result0.sb[4] == 123 && result0.sb[5] == -123 && result0.sb[6] == 127 && result0.sb[7] == -128 )
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
         printf("  result0.sb[4] = %d (expected %d)\n", result0.sb[4], 123);
         printf("  result0.sb[5] = %d (expected %d)\n", result0.sb[5], -123);
         printf("  result0.sb[6] = %d (expected %d)\n", result0.sb[6], 127);
         printf("  result0.sb[7] = %d (expected %d)\n", result0.sb[7], -128);
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
   reg64_t arg0 = { .sw = { 123, -123, 1234, -1234 } };
   reg64_t arg1 = { .sw = { 21, -21, 321, -321 } };
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
         "packsswb %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sb[0] == 21 && result0.sb[1] == -21 && result0.sb[2] == 127 && result0.sb[3] == -128 && result0.sb[4] == 123 && result0.sb[5] == -123 && result0.sb[6] == 127 && result0.sb[7] == -128 )
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
         printf("  result0.sb[4] = %d (expected %d)\n", result0.sb[4], 123);
         printf("  result0.sb[5] = %d (expected %d)\n", result0.sb[5], -123);
         printf("  result0.sb[6] = %d (expected %d)\n", result0.sb[6], 127);
         printf("  result0.sb[7] = %d (expected %d)\n", result0.sb[7], -128);
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
   reg64_t arg0 = { .sw = { 123, -123, 1234, -1234 } };
   reg64_t arg1 = { .sw = { 21, -21, 321, -321 } };
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
         "packuswb %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 21U && result0.ub[1] == 0U && result0.ub[2] == 255U && result0.ub[3] == 0U && result0.ub[4] == 123U && result0.ub[5] == 0U && result0.ub[6] == 255U && result0.ub[7] == 0U )
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
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 123U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 0U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 255U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 0U);
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
   reg64_t arg0 = { .sw = { 123, -123, 1234, -1234 } };
   reg64_t arg1 = { .sw = { 21, -21, 321, -321 } };
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
         "packuswb %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 21U && result0.ub[1] == 0U && result0.ub[2] == 255U && result0.ub[3] == 0U && result0.ub[4] == 123U && result0.ub[5] == 0U && result0.ub[6] == 255U && result0.ub[7] == 0U )
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
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 123U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 0U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 255U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 0U);
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
   reg64_t arg0 = { .sb = { 12, 34, 56, 78, 21, 43, 65, 87 } };
   reg64_t arg1 = { .sb = { 8, 7, 6, 5, 4, 3, 2, 1 } };
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
         "paddb %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sb[0] == 20 && result0.sb[1] == 41 && result0.sb[2] == 62 && result0.sb[3] == 83 && result0.sb[4] == 25 && result0.sb[5] == 46 && result0.sb[6] == 67 && result0.sb[7] == 88 )
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
   reg64_t arg0 = { .sb = { 12, 34, 56, 78, 21, 43, 65, 87 } };
   reg64_t arg1 = { .sb = { 8, 7, 6, 5, 4, 3, 2, 1 } };
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
         "paddb %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sb[0] == 20 && result0.sb[1] == 41 && result0.sb[2] == 62 && result0.sb[3] == 83 && result0.sb[4] == 25 && result0.sb[5] == 46 && result0.sb[6] == 67 && result0.sb[7] == 88 )
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
   reg64_t arg0 = { .sd = { 12345678, 87654321 } };
   reg64_t arg1 = { .sd = { 8765, 4321 } };
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
         "paddd %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sd[0] == 12354443 && result0.sd[1] == 87658642 )
      {
         printf("paddd_1 ... ok\n");
      }
      else
      {
         printf("paddd_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12354443);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 87658642);
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
   reg64_t arg0 = { .sd = { 12345678, 87654321 } };
   reg64_t arg1 = { .sd = { 8765, 4321 } };
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
         "paddd %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sd[0] == 12354443 && result0.sd[1] == 87658642 )
      {
         printf("paddd_2 ... ok\n");
      }
      else
      {
         printf("paddd_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12354443);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 87658642);
      }
   }
   else
   {
      printf("paddd_2 ... failed\n");
   }

   return;
}

static void paddsb_1(void)
{
   reg64_t arg0 = { .sb = { 25, -25, 50, -50, 100, -100, 125, -125 } };
   reg64_t arg1 = { .sb = { 40, -40, 30, -30, 20, -20, 10, -10 } };
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
         "paddsb %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sb[0] == 65 && result0.sb[1] == -65 && result0.sb[2] == 80 && result0.sb[3] == -80 && result0.sb[4] == 120 && result0.sb[5] == -120 && result0.sb[6] == 127 && result0.sb[7] == -128 )
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
   reg64_t arg0 = { .sb = { 25, -25, 50, -50, 100, -100, 125, -125 } };
   reg64_t arg1 = { .sb = { 40, -40, 30, -30, 20, -20, 10, -10 } };
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
         "paddsb %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sb[0] == 65 && result0.sb[1] == -65 && result0.sb[2] == 80 && result0.sb[3] == -80 && result0.sb[4] == 120 && result0.sb[5] == -120 && result0.sb[6] == 127 && result0.sb[7] == -128 )
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
   reg64_t arg0 = { .sw = { 12345, -12345, 32145, -32145 } };
   reg64_t arg1 = { .sw = { 32145, -32145, -12345, 12345 } };
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
         "paddsw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 32767 && result0.sw[1] == -32768 && result0.sw[2] == 19800 && result0.sw[3] == -19800 )
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
   reg64_t arg0 = { .sw = { 12345, -12345, 32145, -32145 } };
   reg64_t arg1 = { .sw = { 32145, -32145, -12345, 12345 } };
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
         "paddsw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 32767 && result0.sw[1] == -32768 && result0.sw[2] == 19800 && result0.sw[3] == -19800 )
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
   reg64_t arg0 = { .ub = { 25U, 50U, 75U, 100U, 125U, 150U, 175U, 200U } };
   reg64_t arg1 = { .ub = { 10U, 20U, 30U, 40U, 50U, 60U, 70U, 80U } };
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
         "paddusb %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 35U && result0.ub[1] == 70U && result0.ub[2] == 105U && result0.ub[3] == 140U && result0.ub[4] == 175U && result0.ub[5] == 210U && result0.ub[6] == 245U && result0.ub[7] == 255U )
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
   reg64_t arg0 = { .ub = { 25U, 50U, 75U, 100U, 125U, 150U, 175U, 200U } };
   reg64_t arg1 = { .ub = { 10U, 20U, 30U, 40U, 50U, 60U, 70U, 80U } };
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
         "paddusb %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 35U && result0.ub[1] == 70U && result0.ub[2] == 105U && result0.ub[3] == 140U && result0.ub[4] == 175U && result0.ub[5] == 210U && result0.ub[6] == 245U && result0.ub[7] == 255U )
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
   reg64_t arg0 = { .uw = { 22222, 33333, 44444, 55555 } };
   reg64_t arg1 = { .uw = { 6666, 7777, 8888, 9999 } };
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
         "paddusw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 28888 && result0.uw[1] == 41110 && result0.uw[2] == 53332 && result0.uw[3] == 65535 )
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
   reg64_t arg0 = { .uw = { 22222, 33333, 44444, 55555 } };
   reg64_t arg1 = { .uw = { 6666, 7777, 8888, 9999 } };
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
         "paddusw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 28888 && result0.uw[1] == 41110 && result0.uw[2] == 53332 && result0.uw[3] == 65535 )
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
   reg64_t arg0 = { .sw = { 1234, 5678, 4321, 8765 } };
   reg64_t arg1 = { .sw = { 87, 65, 43, 21 } };
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
         "paddw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 1321 && result0.sw[1] == 5743 && result0.sw[2] == 4364 && result0.sw[3] == 8786 )
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
   reg64_t arg0 = { .sw = { 1234, 5678, 4321, 8765 } };
   reg64_t arg1 = { .sw = { 87, 65, 43, 21 } };
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
         "paddw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 1321 && result0.sw[1] == 5743 && result0.sw[2] == 4364 && result0.sw[3] == 8786 )
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
   reg64_t arg0 = { .uq = { 0xfdb97531eca86420ULL } };
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "pand %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0x0121452188a84420ULL )
      {
         printf("pand_1 ... ok\n");
      }
      else
      {
         printf("pand_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0121452188a84420ULL);
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
   reg64_t arg0 = { .uq = { 0xfdb97531eca86420ULL } };
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "pand %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0x0121452188a84420ULL )
      {
         printf("pand_2 ... ok\n");
      }
      else
      {
         printf("pand_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x0121452188a84420ULL);
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
   reg64_t arg0 = { .uq = { 0xfdb97531eca86420ULL } };
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "pandn %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0xfc98301064002000ULL )
      {
         printf("pandn_1 ... ok\n");
      }
      else
      {
         printf("pandn_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc98301064002000ULL);
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
   reg64_t arg0 = { .uq = { 0xfdb97531eca86420ULL } };
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "pandn %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0xfc98301064002000ULL )
      {
         printf("pandn_2 ... ok\n");
      }
      else
      {
         printf("pandn_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc98301064002000ULL);
      }
   }
   else
   {
      printf("pandn_2 ... failed\n");
   }

   return;
}

static void pcmpeqb_1(void)
{
   reg64_t arg0 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
   reg64_t arg1 = { .ub = { 11U, 11U, 33U, 33U, 55U, 55U, 77U, 77U } };
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
         "pcmpeqb %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 0xffU && result0.ub[1] == 0x00U && result0.ub[2] == 0xffU && result0.ub[3] == 0x00U && result0.ub[4] == 0xffU && result0.ub[5] == 0x00U && result0.ub[6] == 0xffU && result0.ub[7] == 0x00U )
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
   reg64_t arg0 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
   reg64_t arg1 = { .ub = { 11U, 11U, 33U, 33U, 55U, 55U, 77U, 77U } };
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
         "pcmpeqb %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 0xffU && result0.ub[1] == 0x00U && result0.ub[2] == 0xffU && result0.ub[3] == 0x00U && result0.ub[4] == 0xffU && result0.ub[5] == 0x00U && result0.ub[6] == 0xffU && result0.ub[7] == 0x00U )
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
   reg64_t arg0 = { .ud = { 11223344, 55667788 } };
   reg64_t arg1 = { .ud = { 11223344, 11223344 } };
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
         "pcmpeqd %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 )
      {
         printf("pcmpeqd_1 ... ok\n");
      }
      else
      {
         printf("pcmpeqd_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
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
   reg64_t arg0 = { .ud = { 11223344, 55667788 } };
   reg64_t arg1 = { .ud = { 11223344, 11223344 } };
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
         "pcmpeqd %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 )
      {
         printf("pcmpeqd_2 ... ok\n");
      }
      else
      {
         printf("pcmpeqd_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
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
   reg64_t arg0 = { .uw = { 1122, 3344, 5566, 7788 } };
   reg64_t arg1 = { .uw = { 1122, 1122, 5566, 5566 } };
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
         "pcmpeqw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0xffff && result0.uw[1] == 0x0000 && result0.uw[2] == 0xffff && result0.uw[3] == 0x0000 )
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
   reg64_t arg0 = { .uw = { 1122, 3344, 5566, 7788 } };
   reg64_t arg1 = { .uw = { 1122, 1122, 5566, 5566 } };
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
         "pcmpeqw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0xffff && result0.uw[1] == 0x0000 && result0.uw[2] == 0xffff && result0.uw[3] == 0x0000 )
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
   reg64_t arg0 = { .sb = { -77, -55, -33, -11, 11, 33, 55, 77 } };
   reg64_t arg1 = { .sb = { 77, 55, 33, 11, -11, -33, -55, -77 } };
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
         "pcmpgtb %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 0xffU && result0.ub[1] == 0xffU && result0.ub[2] == 0xffU && result0.ub[3] == 0xffU && result0.ub[4] == 0x00U && result0.ub[5] == 0x00U && result0.ub[6] == 0x00U && result0.ub[7] == 0x00U )
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
   reg64_t arg0 = { .sb = { -77, -55, -33, -11, 11, 33, 55, 77 } };
   reg64_t arg1 = { .sb = { 77, 55, 33, 11, -11, -33, -55, -77 } };
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
         "pcmpgtb %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 0xffU && result0.ub[1] == 0xffU && result0.ub[2] == 0xffU && result0.ub[3] == 0xffU && result0.ub[4] == 0x00U && result0.ub[5] == 0x00U && result0.ub[6] == 0x00U && result0.ub[7] == 0x00U )
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
   reg64_t arg0 = { .sd = { -11111111, 11111111 } };
   reg64_t arg1 = { .sd = { 11111111, -11111111 } };
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
         "pcmpgtd %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 )
      {
         printf("pcmpgtd_1 ... ok\n");
      }
      else
      {
         printf("pcmpgtd_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
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
   reg64_t arg0 = { .sd = { -11111111, 11111111 } };
   reg64_t arg1 = { .sd = { 11111111, -11111111 } };
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
         "pcmpgtd %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 0xffffffff && result0.ud[1] == 0x00000000 )
      {
         printf("pcmpgtd_2 ... ok\n");
      }
      else
      {
         printf("pcmpgtd_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0xffffffff);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x00000000);
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
   reg64_t arg0 = { .sw = { -3333, -1111, 1111, 3333 } };
   reg64_t arg1 = { .sw = { 3333, 1111, -1111, -3333 } };
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
         "pcmpgtw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0xffff && result0.uw[1] == 0xffff && result0.uw[2] == 0x0000 && result0.uw[3] == 0x0000 )
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
   reg64_t arg0 = { .sw = { -3333, -1111, 1111, 3333 } };
   reg64_t arg1 = { .sw = { 3333, 1111, -1111, -3333 } };
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
         "pcmpgtw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0xffff && result0.uw[1] == 0xffff && result0.uw[2] == 0x0000 && result0.uw[3] == 0x0000 )
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
      }
   }
   else
   {
      printf("pcmpgtw_2 ... failed\n");
   }

   return;
}

static void pmaddwd_1(void)
{
   reg64_t arg0 = { .sw = { 1234, 5678, -4321, -8765 } };
   reg64_t arg1 = { .sw = { 1111, -2222, 3333, -4444 } };
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
         "pmaddwd %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sd[0] == -11245542 && result0.sd[1] == 24549767 )
      {
         printf("pmaddwd_1 ... ok\n");
      }
      else
      {
         printf("pmaddwd_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -11245542);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 24549767);
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
   reg64_t arg0 = { .sw = { 1234, 5678, -4321, -8765 } };
   reg64_t arg1 = { .sw = { 1111, -2222, 3333, -4444 } };
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
         "pmaddwd %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sd[0] == -11245542 && result0.sd[1] == 24549767 )
      {
         printf("pmaddwd_2 ... ok\n");
      }
      else
      {
         printf("pmaddwd_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], -11245542);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 24549767);
      }
   }
   else
   {
      printf("pmaddwd_2 ... failed\n");
   }

   return;
}

static void pmulhw_1(void)
{
   reg64_t arg0 = { .sw = { 1111, 2222, -1111, -2222 } };
   reg64_t arg1 = { .sw = { 3333, -4444, 3333, -4444 } };
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
         "pmulhw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0x0038 && result0.uw[1] == 0xff69 && result0.uw[2] == 0xffc7 && result0.uw[3] == 0x0096 )
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
   reg64_t arg0 = { .sw = { 1111, 2222, -1111, -2222 } };
   reg64_t arg1 = { .sw = { 3333, -4444, 3333, -4444 } };
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
         "pmulhw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0x0038 && result0.uw[1] == 0xff69 && result0.uw[2] == 0xffc7 && result0.uw[3] == 0x0096 )
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
   reg64_t arg0 = { .sw = { 1111, 2222, -1111, -2222 } };
   reg64_t arg1 = { .sw = { 3333, -4444, 3333, -4444 } };
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
         "pmullw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0x80b3 && result0.uw[1] == 0x5378 && result0.uw[2] == 0x7f4d && result0.uw[3] == 0xac88 )
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
   reg64_t arg0 = { .sw = { 1111, 2222, -1111, -2222 } };
   reg64_t arg1 = { .sw = { 3333, -4444, 3333, -4444 } };
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
         "pmullw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0x80b3 && result0.uw[1] == 0x5378 && result0.uw[2] == 0x7f4d && result0.uw[3] == 0xac88 )
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
      }
   }
   else
   {
      printf("pmullw_2 ... failed\n");
   }

   return;
}

static void por_1(void)
{
   reg64_t arg0 = { .uq = { 0xfdb97531eca86420ULL } };
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "por %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0xfdbb7577edabedefULL )
      {
         printf("por_1 ... ok\n");
      }
      else
      {
         printf("por_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfdbb7577edabedefULL);
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
   reg64_t arg0 = { .uq = { 0xfdb97531eca86420ULL } };
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "por %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0xfdbb7577edabedefULL )
      {
         printf("por_2 ... ok\n");
      }
      else
      {
         printf("por_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfdbb7577edabedefULL);
      }
   }
   else
   {
      printf("por_2 ... failed\n");
   }

   return;
}

static void pslld_1(void)
{
   reg64_t arg1 = { .ud = { 0x01234567, 0x89abcdef } };
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
         "pslld $4, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (state[0])
         : "mm6"
      );

      if (result0.ud[0] == 0x12345670 && result0.ud[1] == 0x9abcdef0 )
      {
         printf("pslld_1 ... ok\n");
      }
      else
      {
         printf("pslld_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x12345670);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x9abcdef0);
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .ud = { 0x01234567, 0x89abcdef } };
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
         "pslld %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 0x12345670 && result0.ud[1] == 0x9abcdef0 )
      {
         printf("pslld_2 ... ok\n");
      }
      else
      {
         printf("pslld_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x12345670);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x9abcdef0);
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .ud = { 0x01234567, 0x89abcdef } };
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
         "pslld %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 0x12345670 && result0.ud[1] == 0x9abcdef0 )
      {
         printf("pslld_3 ... ok\n");
      }
      else
      {
         printf("pslld_3 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x12345670);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x9abcdef0);
      }
   }
   else
   {
      printf("pslld_3 ... failed\n");
   }

   return;
}

static void psllq_1(void)
{
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "psllq $4, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (state[0])
         : "mm6"
      );

      if (result0.uq[0] == 0x123456789abcdef0ULL )
      {
         printf("psllq_1 ... ok\n");
      }
      else
      {
         printf("psllq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x123456789abcdef0ULL);
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "psllq %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0x123456789abcdef0ULL )
      {
         printf("psllq_2 ... ok\n");
      }
      else
      {
         printf("psllq_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x123456789abcdef0ULL);
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "psllq %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0x123456789abcdef0ULL )
      {
         printf("psllq_3 ... ok\n");
      }
      else
      {
         printf("psllq_3 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x123456789abcdef0ULL);
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
   reg64_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef } };
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
         "psllw $4, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (state[0])
         : "mm6"
      );

      if (result0.uw[0] == 0x1230 && result0.uw[1] == 0x5670 && result0.uw[2] == 0x9ab0 && result0.uw[3] == 0xdef0 )
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef } };
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
         "psllw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0x1230 && result0.uw[1] == 0x5670 && result0.uw[2] == 0x9ab0 && result0.uw[3] == 0xdef0 )
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef } };
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
         "psllw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0x1230 && result0.uw[1] == 0x5670 && result0.uw[2] == 0x9ab0 && result0.uw[3] == 0xdef0 )
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
   reg64_t arg1 = { .ud = { 0x01234567, 0x89abcdef } };
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
         "psrad $4, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (state[0])
         : "mm6"
      );

      if (result0.ud[0] == 0x00123456 && result0.ud[1] == 0xf89abcde )
      {
         printf("psrad_1 ... ok\n");
      }
      else
      {
         printf("psrad_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00123456);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0xf89abcde);
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .ud = { 0x01234567, 0x89abcdef } };
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
         "psrad %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 0x00123456 && result0.ud[1] == 0xf89abcde )
      {
         printf("psrad_2 ... ok\n");
      }
      else
      {
         printf("psrad_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00123456);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0xf89abcde);
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .ud = { 0x01234567, 0x89abcdef } };
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
         "psrad %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 0x00123456 && result0.ud[1] == 0xf89abcde )
      {
         printf("psrad_3 ... ok\n");
      }
      else
      {
         printf("psrad_3 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00123456);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0xf89abcde);
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
   reg64_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef } };
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
         "psraw $4, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (state[0])
         : "mm6"
      );

      if (result0.uw[0] == 0x0012 && result0.uw[1] == 0x0456 && result0.uw[2] == 0xf89a && result0.uw[3] == 0xfcde )
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef } };
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
         "psraw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0x0012 && result0.uw[1] == 0x0456 && result0.uw[2] == 0xf89a && result0.uw[3] == 0xfcde )
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef } };
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
         "psraw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0x0012 && result0.uw[1] == 0x0456 && result0.uw[2] == 0xf89a && result0.uw[3] == 0xfcde )
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
   reg64_t arg1 = { .ud = { 0x01234567, 0x89abcdef } };
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
         "psrld $4, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (state[0])
         : "mm6"
      );

      if (result0.ud[0] == 0x00123456 && result0.ud[1] == 0x089abcde )
      {
         printf("psrld_1 ... ok\n");
      }
      else
      {
         printf("psrld_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00123456);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x089abcde);
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .ud = { 0x01234567, 0x89abcdef } };
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
         "psrld %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 0x00123456 && result0.ud[1] == 0x089abcde )
      {
         printf("psrld_2 ... ok\n");
      }
      else
      {
         printf("psrld_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00123456);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x089abcde);
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .ud = { 0x01234567, 0x89abcdef } };
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
         "psrld %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 0x00123456 && result0.ud[1] == 0x089abcde )
      {
         printf("psrld_3 ... ok\n");
      }
      else
      {
         printf("psrld_3 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 0x00123456);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 0x089abcde);
      }
   }
   else
   {
      printf("psrld_3 ... failed\n");
   }

   return;
}

static void psrlq_1(void)
{
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "psrlq $4, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (state[0])
         : "mm6"
      );

      if (result0.uq[0] == 0x00123456789abcdeULL )
      {
         printf("psrlq_1 ... ok\n");
      }
      else
      {
         printf("psrlq_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x00123456789abcdeULL);
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "psrlq %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0x00123456789abcdeULL )
      {
         printf("psrlq_2 ... ok\n");
      }
      else
      {
         printf("psrlq_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x00123456789abcdeULL);
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "psrlq %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0x00123456789abcdeULL )
      {
         printf("psrlq_3 ... ok\n");
      }
      else
      {
         printf("psrlq_3 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0x00123456789abcdeULL);
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
   reg64_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef } };
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
         "psrlw $4, %%mm6\n"
         "movq %%mm6, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg1), "m" (state[0])
         : "mm6"
      );

      if (result0.uw[0] == 0x0012 && result0.uw[1] == 0x0456 && result0.uw[2] == 0x089a && result0.uw[3] == 0x0cde )
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef } };
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
         "psrlw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0x0012 && result0.uw[1] == 0x0456 && result0.uw[2] == 0x089a && result0.uw[3] == 0x0cde )
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
   reg64_t arg0 = { .uq = { 4ULL } };
   reg64_t arg1 = { .uw = { 0x0123, 0x4567, 0x89ab, 0xcdef } };
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
         "psrlw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 0x0012 && result0.uw[1] == 0x0456 && result0.uw[2] == 0x089a && result0.uw[3] == 0x0cde )
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
   reg64_t arg0 = { .sb = { 8, 7, 6, 5, 4, 3, 2, 1 } };
   reg64_t arg1 = { .sb = { 12, 34, 56, 78, 21, 43, 65, 87 } };
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
         "psubb %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sb[0] == 4 && result0.sb[1] == 27 && result0.sb[2] == 50 && result0.sb[3] == 73 && result0.sb[4] == 17 && result0.sb[5] == 40 && result0.sb[6] == 63 && result0.sb[7] == 86 )
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
   reg64_t arg0 = { .sb = { 8, 7, 6, 5, 4, 3, 2, 1 } };
   reg64_t arg1 = { .sb = { 12, 34, 56, 78, 21, 43, 65, 87 } };
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
         "psubb %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sb[0] == 4 && result0.sb[1] == 27 && result0.sb[2] == 50 && result0.sb[3] == 73 && result0.sb[4] == 17 && result0.sb[5] == 40 && result0.sb[6] == 63 && result0.sb[7] == 86 )
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
   reg64_t arg0 = { .sd = { 8765, 4321 } };
   reg64_t arg1 = { .sd = { 12345678, 87654321 } };
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
         "psubd %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sd[0] == 12336913 && result0.sd[1] == 87650000 )
      {
         printf("psubd_1 ... ok\n");
      }
      else
      {
         printf("psubd_1 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12336913);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 87650000);
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
   reg64_t arg0 = { .sd = { 8765, 4321 } };
   reg64_t arg1 = { .sd = { 12345678, 87654321 } };
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
         "psubd %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sd[0] == 12336913 && result0.sd[1] == 87650000 )
      {
         printf("psubd_2 ... ok\n");
      }
      else
      {
         printf("psubd_2 ... not ok\n");
         printf("  result0.sd[0] = %d (expected %d)\n", result0.sd[0], 12336913);
         printf("  result0.sd[1] = %d (expected %d)\n", result0.sd[1], 87650000);
      }
   }
   else
   {
      printf("psubd_2 ... failed\n");
   }

   return;
}

static void psubsb_1(void)
{
   reg64_t arg0 = { .sb = { -50, 50, -40, 40, -30, 30, -20, 20 } };
   reg64_t arg1 = { .sb = { 25, -25, 50, -50, 100, -100, 125, -125 } };
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
         "psubsb %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sb[0] == 75 && result0.sb[1] == -75 && result0.sb[2] == 90 && result0.sb[3] == -90 && result0.sb[4] == 127 && result0.sb[5] == -128 && result0.sb[6] == 127 && result0.sb[7] == -128 )
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
   reg64_t arg0 = { .sb = { -50, 50, -40, 40, -30, 30, -20, 20 } };
   reg64_t arg1 = { .sb = { 25, -25, 50, -50, 100, -100, 125, -125 } };
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
         "psubsb %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sb[0] == 75 && result0.sb[1] == -75 && result0.sb[2] == 90 && result0.sb[3] == -90 && result0.sb[4] == 127 && result0.sb[5] == -128 && result0.sb[6] == 127 && result0.sb[7] == -128 )
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
   reg64_t arg0 = { .sw = { -32145, 32145, 12345, -12345 } };
   reg64_t arg1 = { .sw = { 12345, -12345, 32145, -32145 } };
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
         "psubsw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 32767 && result0.sw[1] == -32768 && result0.sw[2] == 19800 && result0.sw[3] == -19800 )
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
   reg64_t arg0 = { .sw = { -32145, 32145, 12345, -12345 } };
   reg64_t arg1 = { .sw = { 12345, -12345, 32145, -32145 } };
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
         "psubsw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 32767 && result0.sw[1] == -32768 && result0.sw[2] == 19800 && result0.sw[3] == -19800 )
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
   reg64_t arg0 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
   reg64_t arg1 = { .ub = { 88U, 77U, 66U, 55U, 44U, 33U, 22U, 11U } };
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
         "psubusb %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 77U && result0.ub[1] == 55U && result0.ub[2] == 33U && result0.ub[3] == 11U && result0.ub[4] == 0U && result0.ub[5] == 0U && result0.ub[6] == 0U && result0.ub[7] == 0U )
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
   reg64_t arg0 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
   reg64_t arg1 = { .ub = { 88U, 77U, 66U, 55U, 44U, 33U, 22U, 11U } };
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
         "psubusb %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 77U && result0.ub[1] == 55U && result0.ub[2] == 33U && result0.ub[3] == 11U && result0.ub[4] == 0U && result0.ub[5] == 0U && result0.ub[6] == 0U && result0.ub[7] == 0U )
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
   reg64_t arg0 = { .uw = { 1122, 3344, 5566, 7788 } };
   reg64_t arg1 = { .uw = { 8877, 6655, 4433, 2211 } };
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
         "psubusw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 7755 && result0.uw[1] == 3311 && result0.uw[2] == 0 && result0.uw[3] == 0 )
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
   reg64_t arg0 = { .uw = { 1122, 3344, 5566, 7788 } };
   reg64_t arg1 = { .uw = { 8877, 6655, 4433, 2211 } };
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
         "psubusw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 7755 && result0.uw[1] == 3311 && result0.uw[2] == 0 && result0.uw[3] == 0 )
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
   reg64_t arg0 = { .sw = { 87, 65, 43, 21 } };
   reg64_t arg1 = { .sw = { 1234, 5678, 4321, 8765 } };
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
         "psubw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 1147 && result0.sw[1] == 5613 && result0.sw[2] == 4278 && result0.sw[3] == 8744 )
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
   reg64_t arg0 = { .sw = { 87, 65, 43, 21 } };
   reg64_t arg1 = { .sw = { 1234, 5678, 4321, 8765 } };
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
         "psubw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.sw[0] == 1147 && result0.sw[1] == 5613 && result0.sw[2] == 4278 && result0.sw[3] == 8744 )
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
   reg64_t arg0 = { .ub = { 12U, 34U, 56U, 78U, 21U, 43U, 65U, 87U } };
   reg64_t arg1 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
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
         "punpckhbw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 55U && result0.ub[1] == 21U && result0.ub[2] == 66U && result0.ub[3] == 43U && result0.ub[4] == 77U && result0.ub[5] == 65U && result0.ub[6] == 88U && result0.ub[7] == 87U )
      {
         printf("punpckhbw_1 ... ok\n");
      }
      else
      {
         printf("punpckhbw_1 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 55U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 21U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 66U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 43U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 77U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 65U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 88U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 87U);
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
   reg64_t arg0 = { .ub = { 12U, 34U, 56U, 78U, 21U, 43U, 65U, 87U } };
   reg64_t arg1 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
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
         "punpckhbw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 55U && result0.ub[1] == 21U && result0.ub[2] == 66U && result0.ub[3] == 43U && result0.ub[4] == 77U && result0.ub[5] == 65U && result0.ub[6] == 88U && result0.ub[7] == 87U )
      {
         printf("punpckhbw_2 ... ok\n");
      }
      else
      {
         printf("punpckhbw_2 ... not ok\n");
         printf("  result0.ub[0] = %u (expected %u)\n", result0.ub[0], 55U);
         printf("  result0.ub[1] = %u (expected %u)\n", result0.ub[1], 21U);
         printf("  result0.ub[2] = %u (expected %u)\n", result0.ub[2], 66U);
         printf("  result0.ub[3] = %u (expected %u)\n", result0.ub[3], 43U);
         printf("  result0.ub[4] = %u (expected %u)\n", result0.ub[4], 77U);
         printf("  result0.ub[5] = %u (expected %u)\n", result0.ub[5], 65U);
         printf("  result0.ub[6] = %u (expected %u)\n", result0.ub[6], 88U);
         printf("  result0.ub[7] = %u (expected %u)\n", result0.ub[7], 87U);
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
   reg64_t arg0 = { .ud = { 12345678, 21436587 } };
   reg64_t arg1 = { .ud = { 11223344, 55667788 } };
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
         "punpckhdq %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 55667788 && result0.ud[1] == 21436587 )
      {
         printf("punpckhdq_1 ... ok\n");
      }
      else
      {
         printf("punpckhdq_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 55667788);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 21436587);
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
   reg64_t arg0 = { .ud = { 12345678, 21436587 } };
   reg64_t arg1 = { .ud = { 11223344, 55667788 } };
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
         "punpckhdq %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 55667788 && result0.ud[1] == 21436587 )
      {
         printf("punpckhdq_2 ... ok\n");
      }
      else
      {
         printf("punpckhdq_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 55667788);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 21436587);
      }
   }
   else
   {
      printf("punpckhdq_2 ... failed\n");
   }

   return;
}

static void punpckhwd_1(void)
{
   reg64_t arg0 = { .uw = { 1234, 5678, 2143, 6587 } };
   reg64_t arg1 = { .uw = { 1122, 3344, 5566, 7788 } };
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
         "punpckhwd %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 5566 && result0.uw[1] == 2143 && result0.uw[2] == 7788 && result0.uw[3] == 6587 )
      {
         printf("punpckhwd_1 ... ok\n");
      }
      else
      {
         printf("punpckhwd_1 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 5566);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 2143);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 7788);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 6587);
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
   reg64_t arg0 = { .uw = { 1234, 5678, 2143, 6587 } };
   reg64_t arg1 = { .uw = { 1122, 3344, 5566, 7788 } };
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
         "punpckhwd %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 5566 && result0.uw[1] == 2143 && result0.uw[2] == 7788 && result0.uw[3] == 6587 )
      {
         printf("punpckhwd_2 ... ok\n");
      }
      else
      {
         printf("punpckhwd_2 ... not ok\n");
         printf("  result0.uw[0] = %u (expected %u)\n", result0.uw[0], 5566);
         printf("  result0.uw[1] = %u (expected %u)\n", result0.uw[1], 2143);
         printf("  result0.uw[2] = %u (expected %u)\n", result0.uw[2], 7788);
         printf("  result0.uw[3] = %u (expected %u)\n", result0.uw[3], 6587);
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
   reg64_t arg0 = { .ub = { 12U, 34U, 56U, 78U, 21U, 43U, 65U, 87U } };
   reg64_t arg1 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
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
         "punpcklbw %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 11U && result0.ub[1] == 12U && result0.ub[2] == 22U && result0.ub[3] == 34U && result0.ub[4] == 33U && result0.ub[5] == 56U && result0.ub[6] == 44U && result0.ub[7] == 78U )
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
   reg64_t arg0 = { .ub = { 12U, 34U, 56U, 78U, 21U, 43U, 65U, 87U } };
   reg64_t arg1 = { .ub = { 11U, 22U, 33U, 44U, 55U, 66U, 77U, 88U } };
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
         "punpcklbw %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ub[0] == 11U && result0.ub[1] == 12U && result0.ub[2] == 22U && result0.ub[3] == 34U && result0.ub[4] == 33U && result0.ub[5] == 56U && result0.ub[6] == 44U && result0.ub[7] == 78U )
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
   reg64_t arg0 = { .ud = { 12345678, 21436587 } };
   reg64_t arg1 = { .ud = { 11223344, 55667788 } };
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
         "punpckldq %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 11223344 && result0.ud[1] == 12345678 )
      {
         printf("punpckldq_1 ... ok\n");
      }
      else
      {
         printf("punpckldq_1 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 11223344);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 12345678);
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
   reg64_t arg0 = { .ud = { 12345678, 21436587 } };
   reg64_t arg1 = { .ud = { 11223344, 55667788 } };
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
         "punpckldq %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.ud[0] == 11223344 && result0.ud[1] == 12345678 )
      {
         printf("punpckldq_2 ... ok\n");
      }
      else
      {
         printf("punpckldq_2 ... not ok\n");
         printf("  result0.ud[0] = %u (expected %u)\n", result0.ud[0], 11223344);
         printf("  result0.ud[1] = %u (expected %u)\n", result0.ud[1], 12345678);
      }
   }
   else
   {
      printf("punpckldq_2 ... failed\n");
   }

   return;
}

static void punpcklwd_1(void)
{
   reg64_t arg0 = { .uw = { 1234, 5678, 2143, 6587 } };
   reg64_t arg1 = { .uw = { 1122, 3344, 5566, 7788 } };
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
         "punpcklwd %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 1122 && result0.uw[1] == 1234 && result0.uw[2] == 3344 && result0.uw[3] == 5678 )
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
   reg64_t arg0 = { .uw = { 1234, 5678, 2143, 6587 } };
   reg64_t arg1 = { .uw = { 1122, 3344, 5566, 7788 } };
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
         "punpcklwd %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uw[0] == 1122 && result0.uw[1] == 1234 && result0.uw[2] == 3344 && result0.uw[3] == 5678 )
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
   reg64_t arg0 = { .uq = { 0xfdb97531eca86420ULL } };
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "pxor %%mm6, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0xfc9a30566503a9cfULL )
      {
         printf("pxor_1 ... ok\n");
      }
      else
      {
         printf("pxor_1 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc9a30566503a9cfULL);
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
   reg64_t arg0 = { .uq = { 0xfdb97531eca86420ULL } };
   reg64_t arg1 = { .uq = { 0x0123456789abcdefULL } };
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
         "pxor %1, %%mm7\n"
         "movq %%mm7, %0\n"
         "cld\n"
         : "=m" (result0)
         : "m" (arg0), "m" (arg1), "m" (state[0])
         : "mm7", "mm6"
      );

      if (result0.uq[0] == 0xfc9a30566503a9cfULL )
      {
         printf("pxor_2 ... ok\n");
      }
      else
      {
         printf("pxor_2 ... not ok\n");
         printf("  result0.uq[0] = %llu (expected %llu)\n", result0.uq[0], 0xfc9a30566503a9cfULL);
      }
   }
   else
   {
      printf("pxor_2 ... failed\n");
   }

   return;
}

int main(int argc, char **argv)
{
   signal(SIGILL, handle_sigill);

   movd_1();
   movd_2();
   movd_3();
   movd_4();
   movd_5();
   movd_6();
   movq_1();
   movq_2();
   movq_3();
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
   pmaddwd_1();
   pmaddwd_2();
   pmulhw_1();
   pmulhw_2();
   pmullw_1();
   pmullw_2();
   por_1();
   por_2();
   pslld_1();
   pslld_2();
   pslld_3();
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
   punpckhwd_1();
   punpckhwd_2();
   punpcklbw_1();
   punpcklbw_2();
   punpckldq_1();
   punpckldq_2();
   punpcklwd_1();
   punpcklwd_2();
   pxor_1();
   pxor_2();

   exit(0);
}
