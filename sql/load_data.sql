
COPY USER_FILTERS
FROM '/home/ksuh/Project/SQL/Refilter/data/usr_filters.csv'
WITH DELIMITER ';';

COPY USER_LIST
FROM '/home/ksuh/Project/SQL/Refilter/data/usr_list.csv'
WITH DELIMITER ';';
ALTER SEQUENCE user_list_list_id_seq RESTART 55906;

COPY USR
FROM '/home/ksuh/Project/SQL/Refilter/data/usr.csv'
WITH DELIMITER ';';

COPY USER_LIST_CONTAINS
FROM '/home/ksuh/Project/SQL/Refilter/data/usr_list_contains.csv'
WITH DELIMITER ';';
