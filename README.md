# webchat_2.0

    (version++)

    Всё, что нужно знать об этой версии:
    -разработана под linux и для linux;
    -клиент-серверная архитектура;
    -протокол передачи данных - TCP;
    -совмещение в приложении-сервере клиентских и серверных функций обеспечивается засчёт разветвления процессов;
 
    Про СУБД:
    -MySQL Ver 8.0.35;
    -все данные, необходимые для создания БД находятся в /server/servdb.sql;
    -в /server/build/db_conf хранятся конфигурационные данные для подключения к БД;

    В данной версии было добавлено логирование сообщений, посредством класса Logger, экземпляр которого сохраняет все сообщения пользователей в файл /build/log.txt. Также он предоставляет возможность чтения журнала сообщений,
    начиная с конца файла, с выбором количества выводимых строк.

    Hерешённые проблемы:
    -утечка памяти при подключении к бд (возможно, проблема носит локальный характер);
