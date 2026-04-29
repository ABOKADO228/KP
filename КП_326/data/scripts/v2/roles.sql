-- 1. Полный администратор БД
CREATE ROLE farm_admin WITH
    LOGIN
    PASSWORD 'P@ssw0rd';

-- 2. Пользователь для изменения данных
CREATE ROLE farm_editor WITH
    LOGIN
    PASSWORD '406thebest';

-- 3. Пользователь только для чтения
CREATE ROLE farm_reader WITH
    LOGIN
    PASSWORD '406thebest';




GRANT CONNECT ON DATABASE mydb TO farm_admin;
GRANT CONNECT ON DATABASE mydb TO farm_editor;
GRANT CONNECT ON DATABASE mydb TO farm_reader;




GRANT USAGE ON SCHEMA farm_association TO farm_admin;
GRANT USAGE ON SCHEMA farm_association TO farm_editor;
GRANT USAGE ON SCHEMA farm_association TO farm_reader;





GRANT ALL PRIVILEGES ON ALL TABLES
IN SCHEMA farm_association
TO farm_admin;

GRANT ALL PRIVILEGES ON ALL SEQUENCES
IN SCHEMA farm_association
TO farm_admin;

GRANT ALL PRIVILEGES ON ALL FUNCTIONS
IN SCHEMA farm_association
TO farm_admin;

GRANT EXECUTE ON ALL PROCEDURES
IN SCHEMA farm_association
TO farm_admin;





GRANT SELECT, INSERT, UPDATE, DELETE
ON ALL TABLES
IN SCHEMA farm_association
TO farm_editor;

GRANT USAGE, SELECT, UPDATE
ON ALL SEQUENCES
IN SCHEMA farm_association
TO farm_editor;

GRANT EXECUTE ON ALL FUNCTIONS
IN SCHEMA farm_association
TO farm_editor;

GRANT EXECUTE ON ALL PROCEDURES
IN SCHEMA farm_association
TO farm_editor;



GRANT SELECT
ON ALL TABLES
IN SCHEMA farm_association
TO farm_reader;

GRANT SELECT
ON ALL SEQUENCES
IN SCHEMA farm_association
TO farm_reader;


SELECT * FROM pg_user;

SELECT 
    grantee AS user_name,
    table_schema,
    table_name,
    privilege_type
FROM 
    information_schema.role_table_grants 
WHERE 
    table_schema NOT IN ('information_schema', 'pg_catalog') and grantee in ('farm_editor'); 



SELECT 
    grantee AS role_name,
    routine_schema,
    routine_name,
    privilege_type
FROM information_schema.routine_privileges
WHERE routine_schema = 'farm_association'
  AND grantee IN (
      'farm_admin_group',
      'farm_editor_group',
      'farm_reader_group'
  )
ORDER BY grantee, routine_name, privilege_type;
