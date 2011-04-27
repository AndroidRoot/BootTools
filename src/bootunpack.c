#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bootimg.h"
#define FALSE 0
#define TRUE 1
#define MAX_PATH 1024
#define BUFFER_SIZE 2048

void write_file(char *filename, FILE* in_file, unsigned int filesize);
void write_config(char *filename, boot_img_hdr* header);

int main(int argc, char **argv)
{
  if(argc< 2)
  {
    printf("Usage: %s <boot.img>\n", argv[0]);
    printf("  Extracts kernel and ramdisk from a boot.img file\n");
    return -1;
  }

  FILE *fil = fopen(argv[1],"rb");
  boot_img_hdr hdr;
  fread(&hdr, sizeof(boot_img_hdr), 1, fil);
  unsigned int header_padding = (-sizeof(boot_img_hdr) & (hdr.page_size -1));
  if(strncmp((const char*)hdr.magic, BOOT_MAGIC, BOOT_MAGIC_SIZE) || hdr.kernel_size < 1 || hdr.ramdisk_size < 1)
  {
    fclose(fil);
    fprintf(stderr, "Not a valid boot.img\n");
    return -2;
  };
  fseek(fil, header_padding, SEEK_CUR);
  unsigned int kernel_padding = (-hdr.kernel_size) & (hdr.page_size -1);
  unsigned int ramdisk_padding = (-hdr.ramdisk_size) & (hdr.page_size -1);

  char filename[MAX_PATH];
  
  // Write out kernel
  snprintf(filename, MAX_PATH, "%s-kernel.gz", argv[1]);	
  printf("Writing kernel to %s\n", filename);
  write_file(filename, fil, hdr.kernel_size);
  fseek(fil, kernel_padding, SEEK_CUR); // Skip past kernel padding

  // Write out ramdisk
  snprintf(filename, MAX_PATH, "%s-ramdisk.cpio.gz", argv[1]);
  printf("Writing ramdisk to %s\n", filename);
  write_file(filename, fil, hdr.ramdisk_size);
  
  // Write out optional secondary bootloader
  if(hdr.second_size > 0)
  {
    fseek(fil, ramdisk_padding, SEEK_CUR); // Skip past ramdisk padding
    snprintf(filename, MAX_PATH, "%s-second.boot", argv[1]);
    printf("Writing secondary bootloader to %s\n", filename);
    write_file(filename, fil, hdr.second_size);
  }
  
  fclose(fil);
  snprintf(filename, MAX_PATH, "%s-config", argv[1]);
  printf("Writing boot.img config to %s\n",filename);
  write_config(filename, &hdr);
  return 0;    
}

void write_file(char *filename, FILE* in_file, unsigned int filesize)
{
	FILE *out_file;
	out_file = fopen(filename, "wb");
	unsigned int dataleft = filesize;
	char buffer[BUFFER_SIZE];
	while(dataleft > 0)
	{
		unsigned int fsize = BUFFER_SIZE < dataleft ? BUFFER_SIZE : dataleft;
		unsigned int read_data = fread(buffer, 1, fsize, in_file);
		unsigned int written_data = fwrite(buffer, 1, read_data, out_file);
		// TODO: make more robust, e.g. we can't always count on the 
		// full amount of read data is actually written to disk
		// In practice this never happens as long as we are reading and writing to files! 
		//FIFO's etc might cause issues.
		if(read_data != written_data)
		{
			printf("Did not write all data read. Corruption ohoy!\n");
			fflush(stdout);
			abort();
		}
		dataleft -= read_data;
	}
	fclose(out_file);	
};

void write_config(char *filename, boot_img_hdr* header)
{
	FILE *out_file = fopen(filename, "w");
	fprintf(out_file, "NAME=%s\n", header->name);
	fprintf(out_file, "CMDLINE=%s\n", header->cmdline);
	fclose(out_file);
};

