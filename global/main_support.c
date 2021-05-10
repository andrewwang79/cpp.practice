int g_share=1;

int g_func(int a)
{
   g_share += a;
   return a * 3;
}