# 1 "led.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "led.c"
# 10 "led.c"
# 1 "./include/soc_s3c2440/soc_s3c2440_reg_operator.h" 1



# 1 "./include/soc_s3c2440/soc_s3c2440_reg.h" 1
# 5 "./include/soc_s3c2440/soc_s3c2440_reg_operator.h" 2
# 1 "./include/soc_s3c2440/soc_s3c2440_field.h" 1
# 6 "./include/soc_s3c2440/soc_s3c2440_reg_operator.h" 2
# 11 "led.c" 2
# 35 "led.c"
void delay(volatile int time)
{
 while(time--);
}

void init_config()
{

 (*((volatile unsigned int *)(0x56000050))) &= ~((0x3 << 0) | (0x3 << 4) | (0x3 << 8) | (0x3 << 10) | (0x3 << 12));




 (*((volatile unsigned int *)(0x56000060))) &= ~((0x3 << 6));


 (*((volatile unsigned int *)(0x56000050))) |= (((0x1) << 8) | ((0x1) << 10) | ((0x1) << 12));



}

void led_running()
{




 volatile unsigned int gpfTemp=0;
 volatile unsigned int gpgTemp=0;

 int delay_time = (250000);
 unsigned int i = 0;
 unsigned int len = 0;
 const unsigned char led_info[] = {
  0x00, 0x40, 0x60, 0x70, 0x30, 0x10, 0x00, 0x70,
  0x00, 0x70, 0x00, 0x70, 0x00, 0x10, 0x30, 0x70,
  0x60, 0x40, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70
 };
 len = sizeof(led_info) / sizeof(led_info[0]);


 while(1)
 {
  for(i = 0; i < len; i++)
  {
   gpfTemp = (*((volatile unsigned int *)(0x56000054)));
   gpgTemp = (*((volatile unsigned int *)(0x56000064)));


   if(((((gpfTemp) >> (0)) & (0x01)) ? (0) : (1))){
    if(delay_time < (500000)){
     delay_time += (20000);
    }
   }
   if(((((gpfTemp) >> (2)) & (0x01)) ? (0) : (1))){
    if(delay_time > (50000)){
     delay_time -= (20000);
    }
   }
   if(((((gpgTemp) >> (3)) & (0x01)) ? (0) : (1))){
    delay_time = (250000);
   }






   (((((led_info[i]) >> (4)) & (0x01))) ? (((*((volatile unsigned int *)(0x56000054)))) |= ((0x01) << (4))) : (((*((volatile unsigned int *)(0x56000054)))) &= ~((0x01) << (4))));
   (((((led_info[i]) >> (5)) & (0x01))) ? (((*((volatile unsigned int *)(0x56000054)))) |= ((0x01) << (5))) : (((*((volatile unsigned int *)(0x56000054)))) &= ~((0x01) << (5))));
   (((((led_info[i]) >> (6)) & (0x01))) ? (((*((volatile unsigned int *)(0x56000054)))) |= ((0x01) << (6))) : (((*((volatile unsigned int *)(0x56000054)))) &= ~((0x01) << (6))));

   delay(delay_time);
  }
 }
}
void led_light()
{
 init_config();
 led_running();
}
