
CREATE ROLE farm_admin_group;
CREATE ROLE farm_editor_group;
CREATE ROLE farm_reader_group;



CREATE ROLE farm_admin WITH
    LOGIN
    PASSWORD 'P@ssw0rd';

CREATE ROLE farm_worker WITH
    LOGIN
    PASSWORD '406thebest';

CREATE ROLE developer WITH
    LOGIN
    PASSWORD '406thebest';

CREATE ROLE farm_viewer WITH
    LOGIN
    PASSWORD '406thebest';



GRANT farm_admin_group TO farm_admin;

GRANT farm_editor_group TO farm_worker;
GRANT farm_editor_group TO developer;

GRANT farm_reader_group TO farm_viewer;



GRANT CONNECT ON DATABASE mydb TO farm_admin_group;
GRANT CONNECT ON DATABASE mydb TO farm_editor_group;
GRANT CONNECT ON DATABASE mydb TO farm_reader_group;



GRANT USAGE ON SCHEMA farm_association TO farm_admin_group;
GRANT USAGE ON SCHEMA farm_association TO farm_editor_group;
GRANT USAGE ON SCHEMA farm_association TO farm_reader_group;



GRANT ALL PRIVILEGES ON ALL TABLES
IN SCHEMA farm_association
TO farm_admin_group;

GRANT ALL PRIVILEGES ON ALL SEQUENCES
IN SCHEMA farm_association
TO farm_admin_group;

GRANT EXECUTE ON ALL FUNCTIONS
IN SCHEMA farm_association
TO farm_admin_group;

GRANT EXECUTE ON ALL PROCEDURES
IN SCHEMA farm_association
TO farm_admin_group;



GRANT SELECT, INSERT, UPDATE, DELETE
ON ALL TABLES
IN SCHEMA farm_association
TO farm_editor_group;

GRANT USAGE, SELECT, UPDATE
ON ALL SEQUENCES
IN SCHEMA farm_association
TO farm_editor_group;

GRANT EXECUTE ON ALL FUNCTIONS
IN SCHEMA farm_association
TO farm_editor_group;

GRANT EXECUTE ON ALL PROCEDURES
IN SCHEMA farm_association
TO farm_editor_group;



GRANT SELECT
ON ALL TABLES
IN SCHEMA farm_association
TO farm_reader_group;

GRANT SELECT
ON ALL SEQUENCES
IN SCHEMA farm_association
TO farm_reader_group






SELECT *
FROM pg_user;



SELECT 
    grantee AS role_name,
    table_schema,
    table_name,
    privilege_type
FROM information_schema.role_table_grants 
WHERE table_schema = 'farm_association'
  AND grantee IN (
      'farm_admin_group',
      'farm_editor_group',
      'farm_reader_group'
  )
ORDER BY grantee, table_name, privilege_type;



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