CREATE USER foo@localhost IDENTIFIED BY 'bar';
GRANT ALL ON *.* TO 'foo'@'localhost' WITH GRANT OPTION;
system mysql -u foo -pbar
CREATE DATABASE testdb;
USE testdb;
CREATE TABLE department(department_id int, department_name text, PRIMARY KEY (department_id));
CREATE TABLE employee(emp_id int, emp_name text, emp_dept_id int, PRIMARY KEY (emp_id));
CREATE TABLE empdata (emp_id int, emp_dat blob, PRIMARY KEY (emp_id));
CREATE TABLE numbers (a int PRIMARY KEY, b varchar(255));
