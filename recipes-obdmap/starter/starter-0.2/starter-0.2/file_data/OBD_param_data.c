struct validation_data{
   char  violation;
   char  main_version;
   char  security_ic;
   char  serial[8];
   char  date[10];
   char  hour[8];
   char  clock_status;
   char  mcu_con_status;
   char  m25p64_status;
   char  sd_card_status;
   char  sd_card_type;
   char  m25p64_manufactor_id;
   char  m25p64_memory_type;
   char  m25p64_memory_capacity;
   char  m25p64_memory_sr;
   char  m95512_status;
   char  m95512_sr;
   char  cks_boot;
   char  prog_boot_version;
   char  m25p32_status;
   char  m25p32_manufactor_id;
   char  m25p32_memory_type;
   char  m25p32_memory_capacity;
   char  m25p32_memory_sr;   
   //dados do firmware principal
   char  batery_voltage[4];
   char  main_clock_failed;
   char  ram_integrity;
   char  time_exceded;
}; 


struct validation_data read_validation_data(char buff[]){
 struct validation_data data;
  
  //grava os dados de violação
  data.violation = buff[13];
  
  //versão do boot principal
  data.main_version = buff[14];
 
  //staus do ci de seguraça
  data.security_ic = buff[15];

  //serial
  for(int i = 0, j = 16; j < 24; i++,j++){
   data.serial[i] = buff[j];
  }
 
  //status do relógio
  data.clock_status = buff[25] & 0x01;

  //status de comunicação interna
  data.mcu_con_status = buff[25] & 0x02;

  //status da memória m25p64
  data.m25p64_status = buff[25] & 0x04;
  
  
  //data
  for(int i = 0,j = 26;j < 29; i++,j++){
  if(i == 2){
   data.date[i] = '/'; 
   i++;
  }else if(i == 5){
   data.date[i] = '/';
   i++;
  }else if(i == 8){
   data.date[i] = '2';
   i++;
   data.date[i] = '0';

   i++;

  }

 //isola o primeiro digito
  char temp = 0xF0 & buff[j];
  temp = temp >> 4;
  temp = 0x30 + temp;
  data.date[i] = temp;

 
  i++;
  //isola o segundo dígito
  temp = 0x0F & buff[j];
  temp = 0x30 + temp;
  data.date[i] = temp;
  
  }
 
  //hora
  for(int i  = 0,j = 29; j <32; i++,j++){
   if(i == 2){
    data.hour[i] = ':';
    i++;
   }else if(i == 5){
    data.hour[i] = ':';
    i++;
   }
   //isola o primeiro digito
   char temp = 0xF0 & buff[j];
   temp = temp >> 4;
   temp = 0x30 + temp;
   data.hour[i] = temp;

   i++;
   //isola o segundo dígito
   temp = 0x0F & buff[j];
   temp = 0x30 + temp;
   data.hour[i] = temp;
   }
  
  
  //status do sdcard
  data.sd_card_status = buff[32];

  //tipo do sdcard
  data.sd_card_type = buff[33];

  //identificação do fabricante da memória m25p64
  data.m25p64_manufactor_id = buff[36];

  //tipo da memória m25p64
  data.m25p64_memory_type = buff[37];

  //capacidade da memória m25p64
  data.m25p64_memory_capacity = buff[38];

  //status register memória m25p64
  data.m25p64_memory_sr = buff[39];

  //staus da memória 95512
  data.m95512_status = buff[40];

  //status register da memória m95512
  data.m95512_sr = buff[41];

  //checksum do bootloader
  data.cks_boot = buff[44];

  //versão do do bootloader da placa do gravador
  data.prog_boot_version = buff[45];

  //status da memória m25p32
  data.m25p32_status = buff[46];
 
  //status da memória m25p32
  data.m25p32_manufactor_id = buff[47];

  //tipo da memória m25p32
  data.m25p32_memory_type = buff[48];

  //capacidade da memória m25p32
  data.m25p32_memory_capacity = buff[49];

  //status register da memória m25p32
  data. m25p32_memory_sr = buff[50];

 return data;
}

struct validation_data read_main_firmware_validation_data(struct validation_data readed_data,char buff[]){
  
  if(buff[8] == 0x0D){
    readed_data.batery_voltage[0] = buff[11];
    readed_data.batery_voltage[1] = buff[12];
    readed_data.batery_voltage[2] = '.';
    readed_data.batery_voltage[3] = buff[13];
    
    //não tem dados 
    readed_data.main_clock_failed = 0x00;
    readed_data.ram_integrity = 0x00;
    readed_data.time_exceded = 0x00 ;

  }else{
   if(buff[11] == 0x01){
     readed_data.main_clock_failed = 1;
   }else if(buff[11] == 0x02){
     readed_data.ram_integrity = 1;
   }else if(buff[11] == 0x03){
     readed_data.time_exceded = 1;
   }
  }
 
 return readed_data;
}
