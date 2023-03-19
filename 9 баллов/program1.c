#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int buf_size = 100;

int main(int argc, char *argv[]) {
    // аргументы: имя входного файла, имя выходного файла
    if (argc != 3) {
        printf("Задано неверное количество аргументов командной строки.\n");
        exit(-1);
    }
    
    char *input_file = argv[1];
    char *output_file = argv[2];
    
    int fd_file_2_process;
    int fd_process_2_file;
    char name1[] = "channel 1.fifo";
    char name2[] = "channel 2.fifo";
    
    int fd_input;
    int fd_output;
    char input[buf_size];
    char output[buf_size];
    int size;
    int result;

    (void)umask(0);
    mknod(name1, S_IFIFO | 0666, 0);
    printf("Ура! Удалось открыть канал 'channel 1.fifo' между независимыми процессом-файловиком и процессом-обработчиком.\n");
    
    size = buf_size;
    while (size == buf_size) {
        fd_input = open(input_file, O_RDONLY | O_NONBLOCK);
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

        fd_file_2_process = open(name1, O_WRONLY | O_NONBLOCK);
        if (fd_file_2_process < 0) {
            printf("Не удалось открыть канал 'channel 1.fifo' для записи. Сообщает процесс 1.\n");
            exit(-1);
        }
        result = write(fd_file_2_process, input, buf_size);
        printf("Ура! Удалось открыть канал 'channel 1.fifo' для записи. Сообщает процесс 1.\n\n");
    }

    printf("Ура! Удалось отправить данные по каналу 'channel 1.fifo'. Сообщает процесс 1.\n");

    fflush(NULL);
    usleep(6728);
  
    size = buf_size;
    while (size == buf_size) {
        fd_process_2_file = open(name2, O_RDONLY | O_NONBLOCK);
        if (fd_process_2_file < 0) {
            printf("Не удалось открыть канал 'channel 2.fifo' для чтения. Сообщает процесс 1.\n");
            exit(-1);
        }
        size = read(fd_process_2_file, output, buf_size);
        printf("Ура! Удалось получить результат по каналу 'channel 2.fifo'. Сообщает процесс 1.\n");

        fd_output = open(output_file, O_WRONLY | O_CREAT, 0666);
        if (fd_output < 0) {
            printf("Не удалось открыть выходной файл. Сообщает процесс 1.\n");
            exit(-1);
        }
        result = write(fd_output, output, size);
        if (result < 0) {
            printf("Не удалось записать результат в выходной файл. Сообщает процесс 1.\n");
            exit(-1);
        }
        printf("Ура! Удалось записать результат в выходной файл. Сообщает процесс 1.\n");
        if (close(fd_output) < 0) {
            printf("Не удалось закрыть выходной файл. Сообщает процесс 1.\n");
            exit(-1);
        }
    }
    printf("Ура!!! Завершил работу. Сообщает процесс 1.\n\n");
  
    return 0;
}
