<?php
// Хост (обычно localhost)
$db_host = "localhost";
// Имя базы данных
$db_name = "a0108627_laiter";
// Логин для подключения к базе данных
$db_user = "a0108627_laiter";
// Пароль для подключения к базе данных
$db_pass = "C0KeMv9z";

//Подключаемся к базе
$db = mysqli_connect ($db_host, $db_user, $db_pass, $db_name) or die ("Невозможно подключиться к БД");
// Указываем кодировку, в которой будет получена информация из базы
mysqli_query ($db, 'set character_set_results = "utf8"');