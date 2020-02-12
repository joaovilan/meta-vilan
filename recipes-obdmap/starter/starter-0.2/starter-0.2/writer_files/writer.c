#include <stdio.h>
#include <time.h>



/*
 *
 * Função utilizada para gravar o arquivo de lock pra execução do sistema
 *
 */

void lock_context()
{
 FILE *data_file = fopen("/tmp/obd.lck", "w");
 fflush(data_file);
 fclose(data_file);
}

/*
 *
 * Função utilizada para apagar o arquivo de lock pra execução do sistema
 *
 */
void unlock_context()
{
  remove("/tmp/obd.lck");
}

/*
 *
 *
 *
 * Função utilizada para criar o arquivo de parâmetros para a aplicação principal utilizar
 *
 *
 *
 */
void write_auto_test_data(struct validation_data data)
{
 
FILE *data_file = fopen("/data/auto-test-data", "w");

fprintf(data_file, "%02X\n",data.violation);
fprintf(data_file, "%d\n",data.main_version);
fprintf(data_file, "%02X\n",data.security_ic);

for(int i = 0; i < 8;i++)
 fprintf(data_file, "%c",data.serial[i]);

fprintf(data_file,"\n");

for(int i = 0; i < 10; i++)
 fprintf(data_file, "%c",data.date[i]);

fprintf(data_file,"\n");

for(int i = 0;i < 8; i ++)
 fprintf(data_file, "%c",data.hour[i]);

fprintf(data_file,"\n");

fprintf(data_file, "%02X\n",data.clock_status);
fprintf(data_file, "%02X\n",data.mcu_con_status);
fprintf(data_file, "%02X\n",data.m25p64_status);
fprintf(data_file, "%02X\n",data.sd_card_status);
fprintf(data_file, "%02X\n",data.sd_card_type);
fprintf(data_file, "%02X\n",data.m25p64_manufactor_id);
fprintf(data_file, "%02X\n",data.m25p64_memory_type);
fprintf(data_file, "%02X\n",data.m25p64_memory_capacity);
fprintf(data_file, "%02X\n",data.m25p64_memory_sr);
fprintf(data_file, "%02X\n",data.m95512_status);
fprintf(data_file, "%02X\n",data.m95512_sr);
fprintf(data_file, "%02X\n",data.cks_boot);
fprintf(data_file, "%d\n",data.prog_boot_version);
fprintf(data_file, "%02X\n",data.m25p32_status);
fprintf(data_file, "%02X\n",data.m25p32_manufactor_id);
fprintf(data_file, "%02X\n",data.m25p32_memory_type);
fprintf(data_file, "%02X\n",data.m25p32_memory_capacity);
fprintf(data_file, "%02X\n",data.m25p32_memory_sr);
fprintf(data_file, "%s\n",data.batery_voltage);
fprintf(data_file, "%02X\n",data.main_clock_failed);
fprintf(data_file, "%02X\n",data.ram_integrity);
fprintf(data_file, "%02X\n",data.time_exceded);
fflush(data_file);
fclose(data_file);

}

/*
 * Recupera o nome do arquivo
 *
 */
char *get_file_name(){
  char file_name[11];
  char *result = malloc(32);
 
  sprintf(file_name, "%u\n", (unsigned)time(NULL));
 
  result[0] =  '/';
  result[1] =  'd';
  result[2] =  'a';
  result[3] =  't';
  result[4] =  'a';
  result[5] =  '/';
  result[6] =  'i';
  result[7] =  'n';
  result[8] =  't';
  result[9] =  'e';
  result[10] =  'r';
  result[11] = 'f';
  result[12] = 'a';
  result[13] = 'c'; 
  result[14] = 'e';
  result[15] = '_';
  result[16] = 'l';
  result[17] = 'o';
  result[18] = 'g';
  result[19] = 's';
  result[20] = '/';
  result[21] = file_name[0];
  result[22] = file_name[1];
  result[23] = file_name[2];
  result[24] = file_name[3];
  result[25] = file_name[4];
  result[26] = file_name[5];
  result[27] = file_name[6];
  result[28] = file_name[7];
  result[29] = file_name[8];
  result[30] = file_name[9];
  result[31] = file_name[10];
  
 return result; 
}

/*
 * grava os dados do log remoto no arquivo
 *
 */

void  write_file_header(char file_name[],char serial[]){
 FILE *log_file;

 log_file = fopen(file_name, "a");

  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  fprintf (log_file, "%s", asctime (timeinfo) );
  for(int i = 0; i < 8; i++){
   fprintf(log_file,"%c",serial[i]); 
  }
  fprintf(log_file,"\n"); 
  fflush(log_file);
  fclose(log_file);
  

}
void write_log_to_file(char file_name[],char data[],long size)
{

 FILE *log_file;

 log_file = fopen(file_name, "a");
 
 for(long i = 0; i < size; i++)
  fprintf(log_file,"%02X",data[i]);
 
 fflush(log_file);
 fclose(log_file);

}

void write_open_uart_error(){
 FILE *log_file;
 log_file = fopen("/data/soft-starter/open-error", "a");
 fflush(log_file);
 fclose(log_file);
}

void write_violated_machine(){
 FILE *log_file;
 log_file = fopen("/data/soft-starter/violation-error", "a");
 fflush(log_file);
 fclose(log_file);
}

void write_update_pendent(){
 FILE *log_file;
 log_file = fopen("/data/soft-starter/need-update", "a");
 fflush(log_file);
 fclose(log_file);

}
