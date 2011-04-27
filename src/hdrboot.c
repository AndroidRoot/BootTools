#include <stdio.h>
#include <stdlib.h>
#include "bootimg.h"

int main(int argc, char **argv)
{
  if(argc < 2)
  {
    printf("Usage: %s <boot.img>\n", argv[0]);
    return 0;
  }

  FILE *fil = fopen(argv[1],"r");
  boot_img_hdr hdr;
  fread(&hdr, sizeof(boot_img_hdr), 1, fil);
  fclose(fil);
  printf("Magic: %.*s\n", BOOT_MAGIC_SIZE, hdr.magic);
  printf("Kernel size: 0x%X (%d)\n",hdr.kernel_size, hdr.kernel_size); 
  printf("  Aligned size: 0x%X\n", hdr.kernel_size + ((-hdr.kernel_size) & (hdr.page_size -1)));
  printf("Kernel addr: 0x%X\n",hdr.kernel_addr); 
  printf("Ramdisk size: 0x%X (%d)\n",hdr.ramdisk_size, hdr.ramdisk_size); 
  printf("Ramdisk addr: 0x%X\n",hdr.ramdisk_addr); 

  printf("Second size: 0x%X (%d)\n",hdr.second_size, hdr.second_size); 
  printf("Second addr: 0x%X\n",hdr.second_addr); 

  printf("Tags addr: 0x%X\n",hdr.tags_addr); 
  printf("Page size: 0x%X (%d)\n",hdr.page_size, hdr.page_size); 

  printf("Name: %.*s\n", BOOT_NAME_SIZE, hdr.name);
  printf("Cmdline: %.*s\n", BOOT_ARGS_SIZE, hdr.cmdline);
  return 0;    
}
