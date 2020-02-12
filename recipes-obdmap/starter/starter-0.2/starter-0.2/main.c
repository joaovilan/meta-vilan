#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "serial/serial.c"
#include "obd_utils/utils.c"
#include "file_data/OBD_param_data.c"
#include "writer_files/writer.c"
/*
 *
 * posiciona o cursor do sdcard para recuperação dos dados do log remoto
 *
 */
int reset_recover_data_log()
{
//reposiciona o cursor
    char data[12] = {0x0F,0x55,0x80,0x0A,0x00,0x00,0x00,0x00,0x01,0xFF,0x11,0x00};

    write_bytes(data, 12);

//le os dados do reposicionamento
    int x = read_bytes(data,12,500);


    if(x == 12)
    {

        if(data[11] == 0)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

}


int main(char args[])
{
   //grava o arquivo para travar o contexto da aplicação
   lock_context();


    char *filename = get_file_name();
    int ref = open_port(1000000);

    if(ref < 0)
    {
     write_open_uart_error();
     return 0;
    }

    /**
     *
     * comando de conexão com a interface do obdmap, o qual é respondido com os dados do auto teste do equipamento,
     * podendo assim recuperar todos os parâmetros de trabalho da interface
     *
     */

    //sorteia os bytes aleatórios para formar o frame de conexão da forma correta
    char aleat1 = aleat_bytes();
    char aleat2 = aleat_bytes();

    char data[14] = {0x0F,0x55,0x01,0x01,0x00,0x00,0x00,0x00,0x01,aleat1,aleat2,0x00,0x00,0x00};

    int checksum = calculate_checksum(data,14,11,12);
    data[11] = (checksum & 0xFF00) >> 8;
    data[12] = (checksum & 0x00FF);

    write_bytes(data,14);


    /**
     *
     * Realiza a leitura da resposta do comando de conexão
     *
     */
    char buff[51];

    int x = read_bytes(buff,51,500);

    /*
     *
     * Verifica se o total de dados lidos é igual a quantia esperada
     *
     */
    if(x == 51)
    {
      x = verify_checksum(buff,51,11,12);

        if(x == 0)
        {
            struct validation_data boot_data = read_validation_data(buff);

            /*
             *
             * verifica se a máquina não está violada, caso contrário o processo será
             * abortado
             */
               if (boot_data.violation == 0)
               {
               /*
                 * Caso tudo esteja ok, são iniciados os procedimentos para saída do
                 * bootloader da interface
                 */


                /*
                 * Grava os dados na EEPROM para liberar a saída do bootloader
                 */
                aleat1 = aleat_bytes();
                aleat2 = aleat_bytes();

                data[0] = 0x0F;
                data[1] = 0x55;
                data[2] = 0x01;
                data[3] = 0x08;
                data[4] = 0x00;
                data[5] = 0x00;
                data[6] = 0x03;
                data[7] = 0xFF;
                data[8] = 0x01;
                data[9] = aleat1;
                data[10] = aleat2;
                data[11] = 0xFE;
                data[12] = 0x09;
                data[13] =  0x87;

                checksum = calculate_checksum(data,14,11,12);
                data[11] = (checksum & 0xFF00) >> 8;
                data[12] = (checksum & 0x00FF);

                write_bytes(data,14);

                memset(buff, 0x00, 51);

                x = read_bytes(buff,14,500);

                //regisstra a violação da máquina e aborta o processo
                 if(x == 14)
                 {
                    x = verify_checksum(buff,14,11,12);
                    if(x == 0)
                    {
                      if(buff[13] == 0)
                      {
                            /*
                             * Envia o comando de saída do bootloader
                             */
                            aleat1 = aleat_bytes();
                            aleat2 = aleat_bytes();

                            data[0] = 0x0F;
                            data[1] = 0x55;
                            data[2] = 0x01;
                            data[3] = 0x0B;
                            data[4] = 0x00;
                            data[5] = 0x00;
                            data[6] = 0x00;
                            data[7] = 0x00;
                            data[8] = 0x01;
                            data[9] = aleat1;
                            data[10] = aleat2;
                            data[11] = 0xFF;
                            data[12] = 0xFF;
                            data[13] = 0x00;

                            checksum = calculate_checksum(data,14,11,12);
                            data[11] = (checksum & 0xFF00) >> 8;
                            data[12] = (checksum & 0x00FF);

                            write_bytes(data,14);

                            memset(buff,0x00,51);

                            x = read_bytes(buff,14,500);
                            if(x == 14)
                            {
                                x = verify_checksum(buff,14,11,12);
                                if(buff[13] == 0)
                                {
                                    /*
                                     *
                                     * Aguarda os dados do firmware principal enviados após a saída do boot
                                     * a função read_bytes_sec, le os bytes com um time out de segundos, em média são
                                     * necessários 1,5 segundos para os dados serem transmitidos
                                     *
                                     */
                                    x = read_bytes_sec(buff,12,2);
                                    if(x == 12){
                                        /*
                                         *
                                         * O processo de recuperação do log Remoto começa com o reset do cursor da interface
                                         *
                                         */
                                        //aguarda até que a interface esteja pronta pra responder
                                        sleep(1);
                                        int result = -1;
                                        do{
                                           result = reset_recover_data_log();
                                        }while(result !=0);
					
					write_file_header(filename,boot_data.serial);
                                        do{
                                            //inicia a recuperação do log
                                            data[0] = 0x0F;
                                            data[1] = 0x55;
                                            data[2] = 0x80;
                                            data[3] = 0x07;
                                            data[4] = 0x00;
                                            data[5] = 0x00;
                                            data[6] = 0x00;
                                            data[7] = 0x00;
                                            data[8] = 0x01;
                                            data[9] = 0xFF;
                                            data[10] = 0x14;
                                            data[11] = 0x00;

                                            write_bytes(data,12);

                                            memset(buff,0x00,51);

					   x = read_bytes_sec(buff,16,3);

                                            if(x == 16)
                                            {

                                                //verifica o checksum dos dados
                                                x = verify_checksum(buff,16,9,10);

                                                if(x == 0)
                                                {
                                                    /*
                                                     * recupera o total de dados dos logs a serem recuperados
                                                     */
                                                    long total_data = 0;
                                                    total_data = buff[11] << 16;
                                                    total_data = total_data | (buff[12] << 8);
                                                    total_data = total_data | buff[13];

                                                    char recovered_log[total_data + 1];

                                                    //envia o comando de liberação do recebimento dos dados do log remoto
                                                    buff[0] = 0x0F;
                                                    buff[1] = 0x55;
                                                    buff[2] = 0x80;
                                                    buff[3] = 0x0B;
                                                    buff[4] = 0x00;
                                                    buff[5] = 0x00;
                                                    buff[6] = 0x00;
                                                    buff[7] = 0x00;
                                                    buff[8] = 0x01;
                                                    buff[9] = 0xFF;
                                                    buff[10] = 0x10;
                                                    buff[11] = 0x00;

                                                    write_bytes(buff,12);
                                                    do{

                                                    if (total_data > 4095) {

                                                        // recebe os dados
                                                        read_bytes(recovered_log, 4095,500);
                                                        // salva os dados no arquivo
							recovered_log[total_data]  = 0x00;
                                                        write_log_to_file(filename,recovered_log,4095);
                                                        total_data -= 4095;
                                                    }else{
                                                       if (total_data > 0){


                                                          long y =  bytes_available();
						          do{
                                                           y =  bytes_available();
                                                          }while(y < total_data);

                                                           y = read_bytes_sec(recovered_log, total_data, 3);

   						        // salva os dados no arquivo
							 write_log_to_file(filename,recovered_log,total_data);
                                                        }
						     }
                                                        break;

                                                    }while(1);
                                                }else{
                                                    //registra que houve um erro no checksum dos dados do cabeçalho
                                                }
                                            }else{
                                              //registra que os dados do cabeçalho não foi recebido corretamente
					      int lidos_temp = bytes_available();
					      if(x == 12){
						    x = verify_checksum(buff,12,9,10);
						    if(x == 0){
		                                      if(buff[11] == 0x02)
					 		 break;
                                                      else if(buff[11] == 0x03){
						       int result = -1;
		                                       do{
                                                          result = reset_recover_data_log();
                                                       }while(result !=0);
						       continue;
						      }else{
						       break;
						      }
						    }else{
						     //registra erro no checksum do comando de reset do ponteiro
						    }
                                              }else if(x == -1) {
						//simplesmente ignora o que não for igual a 12
                                              }
                                            }
                                           for(int i = 0; i < 100;i++)
                                               usleep(1000);

                                          clear_buffer();
                                        }while(1);
					 //limpa os buffers para iniciar as ultimas etapas dos testes de conexão
					 memset(data,0x00,14);
					 memset(buff,0x00,51);

                                         //envia o comando para os testes de condições proíbidas do firmware principal
 					 data[0]  = 0x0F;
					 data[1]  = 0x55;
					 data[2]  = 0x80;
					 data[3]  = 0x05;
					 data[4]  = 0x00;
					 data[5]  = 0x00;
					 data[6]  = 0x00;
					 data[7]  = 0x00;
					 data[8]  = 0x01;
					 data[9]  = 0xFF;
					 data[10] = 0x16;
					 data[11] = 0x00;

					 write_bytes(data,12);

					 x = read_bytes(buff,24,500);
					 if(x == 24){
					   if(buff[8] == 0x0D){
					     boot_data = read_main_firmware_validation_data(boot_data,buff);
					   }else{
					    //registra que os dados do teste do firmware principal não foram transmitidos
					   }
					 }else if(x == 12){
					   //registra que não foi possível receber os dados do auto teste corretamente
				           boot_data = read_main_firmware_validation_data(boot_data,buff);
					 }
					 write_auto_test_data(boot_data);
					 unlock_context();
                                    }else{
                                        //registra que os bytes de inicio da aplicação não foram recebidos de forma correta
                                        //registra que os bytes de inicio da aplicação não foram recebidos de forma correta
                                        //registra que os bytes de inicio da aplicação não foram recebidos de forma correta
                                        //registra que os bytes de inicio da aplicação não foram recebidos de forma correta
                                    }
                                }else{
                                  //houve algum erro ao sair do bootloader
                                  write_update_pendent();
                                }
                            }else{
                              //registra que não recebeu os dados de saída do boot corretamente
                            }
                      }else{
                        //registra que houve algum erro na gravação da EEPROIM
                      }
                    }else{
                        //registra erro de checksum no frame de recepção do status da função
                    }
                 }else{
                    //registra que não foram recebidos todos os dados do gravamento da EEPROM
                 }
              }else{
                //registra o errp de maquina violada
                write_violated_machine();
            }
        }else{
            //registra o erro de checksum do comando de conexao
        }
    }else{
        //verifica se não há nenhuma atualização pendente

    }

}
