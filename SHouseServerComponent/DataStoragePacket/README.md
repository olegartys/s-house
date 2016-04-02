Используются sqlpp11 и ssqlpp11/Mysql-connector
Для CMAKE используется target_link:

	link_directories(/usr/lib)
	link_directories(/usr/local/lib)
	target_link_libraries(SHouseDataStorage sqlpp-mysql)
	target_link_libraries(SHouseDataStorage mysqlclient)

Для создания базы данных необходимы: Mysql-5.6, mysql-server.


mysqldump -u root -p --no-data DataStorage > DataStorage.sql

Для создания хэдэра используется скрипт из sqlpp11:

bash> ./../sqlpp11/sqlpp11/scripts/ddl2cpp DataStorage.sql ./DataDB DataDB
Предворительно создается БД с необходимыми таблицами.
