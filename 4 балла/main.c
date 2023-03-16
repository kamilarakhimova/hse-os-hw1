/*********************************************************************************

Условие: Разработать программу, которая «переворачивает» заданную позициями N1–N2
часть ASCII–строки символов (N1, N2 вводятся как параметры).

**********************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int buf_size = 5555;

// функция, осуществляющая обработку в соответствии с заданием
int function(int arg1, int arg2, char* in, char* out) {
   int medium = ((arg1 + arg2) / 2) + ((arg1 + arg2) % 2);
   int i = 0;
   while (in[i] != '\0') {
       if (i < arg1 || i > arg2) {
           out[i] = in[i];
       } else {
           if (i <= medium) {
               char tmp = in[i];
               out[i] = in[arg2 - i + arg1];
               out[arg2 - i + arg1] = tmp;
           }
       }
       ++i;
   }
   out[i] = '\0';
   return i;
}


int main(int argc, char *argv[]) {
    // аргументы: имя входного файла, имя выходного файла, n1, n2
    if (argc != 5) {
        printf("Задано неверное количество аргументов командной строки.\n");
        exit(-1);
    }
    
    char *input_file = argv[1];
    char *output_file = argv[2];
    int n1 = atoi(argv[3]);
    int n2 = atoi(argv[4]);
    
    pid_t input_child, process_child, output_child;
    int fd_input_child_2_process_child[2];
    int fd_process_child_2_output_child[2];
    
    int fd_input;
    int fd_output;
    char input[buf_size];
    char output[buf_size];
    size_t size;
    int result;

    if (pipe(fd_input_child_2_process_child) < 0) {
        printf("Не удалось открыть канал между ребенком-читателем и ребёнком-обработчиком.\n");
        exit(-1);
    } else {
        printf("Ура! Удалось открыть канал между ребенком-читателем и ребёнком-обработчиком.\n");
    }
    if (pipe(fd_process_child_2_output_child) < 0) {
        printf("Не удалось открыть канал между ребенком-обработчиком и ребёнком-писателем.\n");
        exit(-1);
    } else {
        printf("Ура! Удалось открыть канал между ребенком-обработчиком и ребёнком-писателем.\n");
    }
    
    input_child = fork();
    if (input_child < 0) {
        printf("Не удалось создать процесс 1 (ребенок-читатель).\n");
        exit(-1);
    } else {
        if (input_child > 0) {
            // код процесса 0 (родителя) 
            printf("Ура! Удалось создать процесс 1 (ребенок-читатель).\n");
            process_child = fork();
            if (process_child < 0) {
                printf("Не удалось создать процесс 2 (ребенок-обработчик).\n");
                exit(-1);
            } else {
                if (process_child > 0) {
                    printf("Ура! Удалось создать процесс 2 (ребенок-обработчик).\n");
                    output_child = fork();
                    if (output_child < 0) {
                        printf("Не удалось создать процесс 3 (ребенок-писатель).\n");
                        exit(-1);
                    } else {
                        if (output_child > 0) {
                            printf("Ура! Удалось создать процесс 3 (ребенок-писатель).\n");
                        } else {
                            // код процесса 3 (ребенка-писателя)
                            if (close(fd_process_child_2_output_child[1]) < 0) {
                                printf("Не удалось закрыть канал 2 для записи. Сообщает процесс 3.\n");
                                exit(-1);
                            }
                            size = read(fd_process_child_2_output_child[0], output, buf_size);
                            printf("Ура! Удалось получить результат по каналу 2. Сообщает процесс 3.\n");
                            if (close(fd_process_child_2_output_child[0]) < 0) {
                                printf("Не удалось закрыть канал 2 для чтения. Сообщает процесс 3.\n");
                                exit(-1);
                            }
                            fd_output = open(output_file, O_WRONLY | O_CREAT, 0666);
                            if (fd_output < 0) {
                                printf("Не удалось открыть выходной файл. Сообщает процесс 3.\n");
                                exit(-1);
                            }
                            size = write(fd_output, output, size);
                            if (size < 0) {
                                printf("Не удалось записать результат в выходной файл. Сообщает процесс 3.\n");
                                exit(-1);
                            }
                            printf("Ура! Удалось записать результат в выходной файл. Сообщает процесс 3.\n");
                            if (close(fd_output) < 0) {
                                printf("Не удалось закрыть выходной файл. Сообщает процесс 3.\n");
                                exit(-1);
                            }
                            printf("Ура!!! Завершил работу. Сообщает процесс 3.\n\n");
                        }
                    }
                } else {
                    // код процесса 2 (ребенка-обработчика)
                    if (close(fd_input_child_2_process_child[1]) < 0) {
                        printf("Не удалось закрыть канал 1 для записи. Сообщает процесс 2.\n");
                        exit(-1);
                    }
                    size = read(fd_input_child_2_process_child[0], input, buf_size);
                    printf("Ура! Удалось получить данные по каналу 1. Сообщает процесс 2.\n");
                    if (close(fd_input_child_2_process_child[0]) < 0) {
                        printf("Не удалось закрыть канал 1 для чтения. Сообщает процесс 2.\n");
                        exit(-1);
                    }
                    result = function(n1, n2, input, output);
                    printf("Ура! Удалось обработать данные. Сообщает процесс 2.\n");
                    if (close(fd_process_child_2_output_child[0]) < 0) {
                        printf("Не удалось закрыть канал 2 для чтения. Сообщает процесс 2.\n");
                        exit(-1);
                    }
                    size = write(fd_process_child_2_output_child[1], output, result);
                    printf("Ура! Удалось отправить результат по каналу 2. Сообщает процесс 2.\n");
                    if (close(fd_process_child_2_output_child[1]) < 0) {
                        printf("Не удалось закрыть канал 2 для записи. Сообщает процесс 2.\n");
                        exit(-1);
                    }
                    printf("Ура!!! Завершил работу. Сообщает процесс 2.\n\n");
                }
            }
        } else {
            // код процесса 1 (ребенка-читателя)
            fd_input = open(input_file, O_RDONLY);
            if (fd_input < 0) {
                printf("Не удалось открыть входной файл. Сообщает процесс 1.\n");
                exit(-1);
            }
            size = read(fd_input, input, buf_size);
            if (size < 0) {
                printf("Не удалось прочитать входной файл. Сообщает процесс 1.\n");
                exit(-1);
            }
            printf("Ура! Удалось прочитать данные из входного файла. Сообщает процесс 1.\n\n");
            if (close(fd_input) < 0) {
                printf("Не удалось закрыть входной файл. Сообщает процесс 1.\n");
                exit(-1);
            }
            
            if (close(fd_input_child_2_process_child[0]) < 0) {
                printf("Не удалось закрыть канал 1 для чтения. Сообщает процесс 1.\n");
                exit(-1);
            }
            size = write(fd_input_child_2_process_child[1], input, buf_size);
            printf("Ура! Удалось отправить данные по каналу 1. Сообщает процесс 1.\n");
            if (close(fd_input_child_2_process_child[1]) < 0) {
                printf("Не удалось закрыть канал 1 для записи. Сообщает процесс 1.\n");
                exit(-1);
            }
            printf("Ура!!! Завершил работу. Сообщает процесс 1.\n\n");
        }
    }
    return 0;
}
