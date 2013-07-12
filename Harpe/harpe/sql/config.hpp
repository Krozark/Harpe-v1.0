#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <cstdlib>
#include <string>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

// Link to the Connector/C++ library
//#pragma comment(lib, "mysqlcppconn.lib")

extern std::string s_server;
extern std::string s_port;
extern std::string s_username;
extern std::string s_password;
extern std::string s_bdd;

extern sql::Driver     *driver; // Create a pointer to a MySQL driver object
extern sql::Connection *dbConn; // Create a pointer to a database connection object
extern sql::Statement  *bdd;   // Create a pointer to a Statement object to hold our SQL commands
extern sql::ResultSet  *bdd_res;    // Create a pointer to a ResultSet object to hold the results of any queries we run

#define BDD_DECLARE() \
    sql::Driver     *driver;\
    sql::Connection *dbConn;\
    sql::Statement  *bdd;\
    sql::ResultSet  *bdd_res;\
    std::string s_server   = "tcp://127.0.0.1";\
    std::string s_port = "3306";\
    std::string s_username = "root";\
    std::string s_password = "toor";\
    std::string s_bdd = "Harpe";


#define BDD_INIT() \
{\
    try{\
        driver = get_driver_instance();\
    }\
    catch (sql::SQLException e){\
        std::cout << "Could not get a database driver. Error message: " << e.what() << std::endl;\
        exit(1);\
    }\
    try{\
        dbConn = driver->connect(s_server+":"+s_port, s_username, s_password);\
    }\
	catch (sql::SQLException e){\
        std::cout << "Could not connect to database. Error message: " << e.what() << std::endl;\
        exit(1);\
    }\
	bdd = dbConn->createStatement();\
	bdd->execute("USE "+s_bdd);\
}


#define BDD_DESTROY() \
    delete bdd;\
    delete dbConn;

    //delete bdd_res;
#endif
