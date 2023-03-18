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
    // аргументы: n1, n2
    if (argc != 3) {
        printf("Задано неверное количество аргументов командной строки.\n");
        exit(-1);
    }
    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);
    
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

    fflush(NULL);
    usleep(1000);

    (void)umask(0);
    mknod(name2, S_IFIFO | 0666, 0);
    printf("Ура! Удалось открыть канал 'channel 2.fifo' между независимыми процессом-обработчиком и процессом-файловиком.\n");
    
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
    return 0;
}
