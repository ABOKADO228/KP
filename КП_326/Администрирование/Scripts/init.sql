CREATE DATABASE IF NOT EXISTS store;
use store;

DROP TABLE IF EXISTS position;
create table position (
id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
name varchar(90) not null unique
);

DROP TABLE IF EXISTS employee;
create table employee(
id INT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
firstname varchar(120) not null,
lastname varchar(120) not null,
position_id INT UNSIGNED references position (id),
requirements varchar(120)
);

create table document(
id INT UNSIGNED PRIMARY KEY auto_increment,
type varchar(90) not null,
number varchar(90) not null
);

create table person_documents(
person_id INT unsigned not null references employee(id),
document_id INT unsigned not null,

primary key(person_id, document_id)
);

create table product (
id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
name varchar(120)
);

create table entity(
id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
name varchar(120)
);

create table sales(
product_id INT UNSIGNED not null references product(id),
volume INT UNSIGNED NOT NULL default(1),
entity_id INT UNSIGNED NOT NULL references entity(id),
sales_date DATE NOT NULL default(NOW())
);