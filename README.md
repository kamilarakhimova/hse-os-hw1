# hse-os-hw1
---
# ИДЗ №1 по предмету "Операционнные системы"

# Курс 2, ПИ, ФКН, НИУ ВШЭ

# Вариант 1, Рахимова Камила Мухаммадовна, БПИ214

---

## *Условие задачи:*

Разработать программу, которая «переворачивает» заданную позициями N1–N2 часть ASCII–строки символов (N1, N2 вводятся как
параметры).

---

## *Примечания:*

✔️ При решении задачи обозначаемые параметрами N1 и N2 границы считались включительно (обе). Нумерация идёт с 0.

✔️ При использовании программ за корректностью вводимых данных должен следить пользователь 
(т.е. учитывать, что N1 >= N2, при этом 0 <= N1, N2 < размера входных данных и пр.).

✔️ Набор текстовых файлов, на которых проводилось тестирование программы, а также правильные ответы на них можно найти вот в [этой](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/Тесты) папке. Значения параметров использовались следующие:

```
> ./main input1.txt output1.txt 1 4
> ./main input2.txt output2.txt 0 0
> ./main input3.txt output3.txt 0 9
> ./main input4.txt output4.txt 10 15
> ./main input5.txt output5.txt 4900 4999
```

---

## *Отчёт*

Работа выполнена с расчётом на получение оценки в 10/10 баллов.

### ◉ Требования на 4 балла:

Сама программа расположилась [здесь](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/4%20балла/main.c).


✔ Общая схема решаемой задачи:

<img width="605" alt="Снимок экрана 2023-03-17 в 11 12 04" src="https://user-images.githubusercontent.com/58568615/226003890-32ed94aa-b24b-4f07-9b31-845059c3c6b3.png">

Здесь процесс 1 (ребенок-читатель) читает текстовые данные из заданного файла и через неименованный канал 1 передает их второму процессу.
Процесс 2 (ребенок-обработчик) осуществляет обработку данных в соответствии с заданием и передает результат обработки через неименованный канал 2 третьему процессу.
А процесс 3 (ребенок-писатель) осуществляет вывод данных в заданный файл.

✔ Для задания имен входного и выходного файлов использовались аргументы командной строки.

```
char *input_file = argv[1];
char *output_file = argv[2];
```

✔ Ввод и вывод данных при работе с файлами осуществлялся через системные вызовы read и write.

```
fd_input = open(input_file, O_RDONLY);
size = read(fd_input, input, buf_size);

fd_output = open(output_file, O_WRONLY | O_CREAT, 0666);
size = write(fd_output, output, size);
```

✔ Размер буфера по умолчанию принимался равным 5555 байт (5555 > 5000) -> требование выполнено:

```
int buf_size = 5555;
```

✔ Результаты выполнения программы на описанных выше 5 тестах можно увидеть [тут](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/4%20балла/Результаты).


При правильной отработке программа выведет примерно следующее:

<img width="615" alt="Снимок экрана 2023-03-16 в 22 21 30" src="https://user-images.githubusercontent.com/58568615/225730302-807a1c79-c56d-4b1a-9206-4b5c74e5f65b.png">

### ◉ Требования на 5 баллов:

Требования, совпадающие с требованиями на более низкую оценку, остались выполненными и в [этой](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/5%20баллов/main.c) программе.


✔ Общая схема решаемой задачи:

<img width="628" alt="Снимок экрана 2023-03-17 в 11 13 00" src="https://user-images.githubusercontent.com/58568615/226004000-56a3aad5-acfe-4770-97e2-2d16ad8537b9.png">

Здесь процесс 1 (ребенок-читатель) читает текстовые данные из заданного файла и через именованный канал 'channel 1.fifo' передает их второму процессу.
Процесс 2 (ребенок-обработчик) осуществляет обработку данных в соответствии с заданием и передает результат обработки через именованный канал 'channel 2.fifo' третьему процессу.
А процесс 3 (ребенок-писатель) осуществляет вывод данных в заданный файл.

✔ Результаты выполнения программы на описанных выше 5 тестах можно увидеть [тут](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/5%20баллов/Результаты).


При правильной отработке программа выдаст примерно следующее:

<img width="751" alt="Снимок экрана 2023-03-17 в 09 06 29" src="https://user-images.githubusercontent.com/58568615/225825416-b8cbc1d4-d833-4196-b135-b467ddb4cec7.png">

### ◉ Требования на 6 баллов:

Требования, совпадающие с требованиями на более низкую оценку, остались выполненными и [здесь](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/6%20баллов/main.c).


✔ Общая схема решаемой задачи:

<img width="632" alt="Снимок экрана 2023-03-17 в 19 49 48" src="https://user-images.githubusercontent.com/58568615/225967398-68f5f360-6c8b-43ee-8748-5ca93893303c.png">

Здесь процесс 1 (ребенок-файловик) читает текстовые данные из заданного файла и через неименованный канал 1 передает их второму процессу.
Процесс 2 (ребенок-обработчик) осуществляет обработку данных в соответствии с заданием и передает результат обработки через неименованный канал 2 первому процессу.
А процесс 1 (ребенок-файловик) осуществляет вывод данных в заданный файл.

✔ Результаты выполнения программы на описанных выше 5 тестах можно увидеть [тут](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/6%20баллов/Результаты).


При правильной отработке программа покажет примерно следующее:

<img width="625" alt="Снимок экрана 2023-03-17 в 19 23 43" src="https://user-images.githubusercontent.com/58568615/225961906-b5cfabe6-1dc1-4274-b1d6-7facaba45db2.png">

### ◉ Требования на 7 баллов:

Требования, совпадающие с требованиями на более низкую оценку, тоже выполнены в [представленной](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/7%20баллов/main.c) программе.


✔ Общая схема решаемой задачи:

<img width="665" alt="Снимок экрана 2023-03-17 в 19 49 21" src="https://user-images.githubusercontent.com/58568615/225967373-8eed69e3-c16c-445c-bff7-fed3b8bd1eaa.png">

Здесь процесс 1 (ребенок-файловик) читает текстовые данные из заданного файла и через именованный канал 'channel 1.fifo' передает их второму процессу.
Процесс 2 (ребенок-обработчик) осуществляет обработку данных в соответствии с заданием и передает результат обработки через именованный канал 'channel 2.fifo' первому процессу.
А процесс 1 (ребенок-файловик) осуществляет вывод данных в заданный файл.

✔ Результаты выполнения программы на описанных выше 5 тестах можно увидеть [здесь](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/7%20баллов/Результаты).


При правильной отработке программа покажет примерно следующее:

<img width="759" alt="Снимок экрана 2023-03-17 в 22 28 07" src="https://user-images.githubusercontent.com/58568615/226000735-fa5c6c6a-c654-4dca-a80b-3a196676d859.png">

### ◉ Требования на 8 баллов:

Требования, совпадающие с требованиями на более низкую оценку, всё ещё выполнены [тут](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/8%20баллов/program1.c) и [тут](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/8%20баллов/program2.c).

✔ Общая схема решаемой задачи:

<img width="644" alt="Снимок экрана 2023-03-18 в 18 23 58" src="https://user-images.githubusercontent.com/58568615/226114824-efe2c9a7-241d-405d-9ad7-aaa1053ea4ed.png">

Здесь процесс 1 (процесс-файловик) читает текстовые данные из заданного файла и через именованный канал 'channel 1.fifo' передает их второму процессу.
Процесс 2 (процесс-обработчик) осуществляет обработку данных в соответствии с заданием и передает результат обработки через именованный канал 'channel 2.fifo' обратно первому процессу.
А процесс 1 (процесс-файловик) осуществляет вывод данных в заданный файл.

✔ Результаты выполнения программы на описанных выше 5 тестах можно увидеть [тут](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/8%20баллов/Результаты).

При правильной отработке программа ответит примерно следующее:

<img width="882" alt="Снимок экрана 2023-03-18 в 17 51 49" src="https://user-images.githubusercontent.com/58568615/226114222-51c0daa6-eca1-4dea-bdf8-d8510c1fb970.png">

### ◉ Требования на 9 баллов:

Требования, совпадающие с требованиями на более низкую оценку, всё ещё актуальны в [первой](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/9%20баллов/program1.c) и [второ ](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/9%20баллов/program2.c) программах.


✔ Общая схема решаемой задачи (не изменилась с прошлого пункта):

<img width="644" alt="Снимок экрана 2023-03-18 в 18 23 58" src="https://user-images.githubusercontent.com/58568615/226114824-efe2c9a7-241d-405d-9ad7-aaa1053ea4ed.png">

Здесь процесс 1 (процесс-файловик) читает текстовые данные из заданного файла ограниченной порции и через именованный канал 'channel 1.fifo' передает их второму процессу.
Процесс 2 (процесс-обработчик) осуществляет обработку данных в соответствии с заданием и передает результат обработки через именованный канал 'channel 2.fifo' обратно первому процессу.
А процесс 1 (процесс-файловик) осуществляет вывод данных в заданный файл. И снова повторяет описанные выше действия, пока не считает все данные из входного файла.

✔ Размер буфер теперь равен 100 байт (100 < 200) -> требование выполнено:

```
int buf_size = 100;
```

✔ Результаты выполнения программы на описанных выше 5 тестах можно увидеть [тут](https://github.com/kamilarakhimova/hse-os-hw1/tree/main/9%20баллов/Результаты).


При правильной отработке программа ответит примерно следующее:

*скоро картинка появится здесь*
