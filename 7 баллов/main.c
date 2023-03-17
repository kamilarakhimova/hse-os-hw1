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
    
    pid_t file_child, process_child;
    int fd_file_2_process;
    int fd_process_2_file;
    char name1[] = "channel 1.fifo";
    char name2[] = "channel 2.fifo";
    
    int fd_input;
    int fd_output;
    char input[buf_size];
    char output[buf_size];
    size_t size;
    int result;

    (void)umask(0);
    mknod(name1, S_IFIFO | 0666, 0);
    printf("Ура! Удалось открыть канал 'channel 1.fifo' между ребенком-файловиком и ребёнком-обработчиком.\n");
    mknod(name2, S_IFIFO | 0666, 0);
    printf("Ура! Удалось открыть канал 'channel 2.fifo' между ребенком-обработчиком и ребёнком-файловиком.\n");
    
    file_child = fork();
    if (file_child < 0) {
        printf("Не удалось создать процесс 1 (ребенок-файловик).\n");
        exit(-1);
    } else {
        if (file_child > 0) {
            // код процесса 0 (родителя) 
            printf("Ура! Удалось создать процесс 1 (ребенок-файловик).\n");
            process_child = fork();
            if (process_child < 0) {
                printf("Не удалось создать процесс 2 (ребенок-обработчик).\n");
                exit(-1);
            } else {
                if (process_child > 0) {
                    printf("Ура! Удалось создать процесс 2 (ребенок-обработчик).\n");
                } else {
                    // код процесса 2 (ребенка-обработчика)
                    fd_file_2_process = open(name1, O_RDONLY);
                    if (fd_file_2_process < 0) {
                        printf("Не удалось открыть канал 'channel 1.fifo' для чтения. Сообщает процесс 2.\n");
                        exit(-1);
                    }
                    size = read(fd_file_2_process, input, buf_size);
                    printf("Ура! Удалось получить данные по каналу 'channel 1.fifo'. Сообщает процесс 2.\n");
                    if (close(fd_file_2_process) < 0) {
                        printf("Не удалось закрыть канал 'channel 1.fifo'. Сообщает процесс 2.\n");
                        exit(-1);
                    }
                    result = function(n1, n2, input, output);
                    printf("Ура! Удалось обработать данные. Сообщает процесс 2.\n");
                    fd_process_2_file = open(name2, O_WRONLY);
                    if (fd_process_2_file < 0) {
                        printf("Не удалось открыть канал 'channel 2.fifo' для записи. Сообщает процесс 2.\n");
                        exit(-1);
                    }
                    size = write(fd_process_2_file, output, result);
                    printf("Ура! Удалось отправить результат по каналу 'channel 2.fifo'. Сообщает процесс 2.\n");
                    if (close(fd_process_2_file) < 0) {
                        printf("Не удалось закрыть канал 'channel 2.fifo'. Сообщает процесс 2.\n");
                        exit(-1);
                    }
                    printf("Ура!!! Завершил работу. Сообщает процесс 2.\n\n");
                }
            }
        } else {
            // код процесса 1 (ребенка-файловика)
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

            fd_file_2_process = open(name1, O_WRONLY);
            if (fd_file_2_process < 0) {
                printf("Не удалось открыть канал 'channel 1.fifo' для записи. Сообщает процесс 1.\n");
                exit(-1);
            }
            size = write(fd_file_2_process, input, buf_size);
            printf("Ура! Удалось отправить данные по каналу 'channel 1.fifo'. Сообщает процесс 1.\n");
            if (close(fd_file_2_process) < 0) {
                printf("Не удалось закрыть канал 'channel 1.fifo'. Сообщает процесс 1.\n");
                exit(-1);
            }

            fd_process_2_file = open(name2, O_RDONLY);
            if (fd_process_2_file < 0) {
                printf("Не удалось открыть канал 'channel 2.fifo' для чтения. Сообщает процесс 1.\n");
                exit(-1);
            }
            size = read(fd_process_2_file, output, buf_size);
            printf("Ура! Удалось получить результат по каналу 'channel 2.fifo'. Сообщает процесс 1.\n");
            if (close(fd_process_2_file) < 0) {
                printf("Не удалось закрыть канал 'channel 2.fifo'. Сообщает процесс 1.\n");
                exit(-1);
            }
            fd_output = open(output_file, O_WRONLY | O_CREAT, 0666);
            if (fd_output < 0) {
                printf("Не удалось открыть выходной файл. Сообщает процесс 1.\n");
                exit(-1);
            }
            size = write(fd_output, output, size);
            if (size < 0) {
                printf("Не удалось записать результат в выходной файл. Сообщает процесс 1.\n");
                exit(-1);
            }
            printf("Ура! Удалось записать результат в выходной файл. Сообщает процесс 1.\n");
            if (close(fd_output) < 0) {
                printf("Не удалось закрыть выходной файл. Сообщает процесс 1.\n");
                exit(-1);
            }
            printf("Ура!!! Завершил работу. Сообщает процесс 1.\n\n");
        }
    }
    return 0;
}
