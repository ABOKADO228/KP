CREATE USER 'petrenko'@'localhost' IDENTIFIED BY 'anton';


GRANT SELECT ON store.* TO 'petrenko'@'localhost';
GRANT INSERT ON store.* TO 'petrenko'@'localhost';

INSERT INTO entity VALUES (0,'килограммы');

REVOKE SELECT ON  *.* FROM petrenko@localhost;
REVOKE INSERT ON  *.* FROM petrenko@localhost;

GRANT SELECT (firstname, lastname, requirements) ON store.employee TO petrenko@localhost;
