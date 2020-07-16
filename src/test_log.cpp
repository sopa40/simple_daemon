#include "gtest/gtest.h"
#include <iostream>

#define LOG_FILE "/tmp/log"
extern "C" {
#include "logging.h"
#include "writers.h"
}

class MyTest : public testing::Test {
    void SetUp() {

    }

    void TearDown() {
        // if(unlink(LOG_FILE))
        //     printf("tessssssssssst %s", strerror(errno));
        //ASSERT_EQ (unlink("/tmp/log"), 0) << "Failed  to delete log file";
    }
};

/* Somewhy it can not delete log file.
 * If file will not be deleted/truncated, next test will append its message and test fails
 * Need to be fixed
 */

// TEST_F(MyTest, log_correct) {
//     FILE *fd;
//     char buf[64];
//     char *text = "test";
//     plog(PLOG_INFO, text);
//     fd = fopen(LOG_FILE, "r");
//     ASSERT_NE(fd, nullptr) << "Failed to open file";
//     ASSERT_EQ(strlen(text), fread(buf, sizeof(char), sizeof(buf), fd)) <<
//               "Failed to read proper amount of data";
//     ASSERT_STREQ(text, buf) << "Invalid value in file";
//     fclose(fd);
//
// }

TEST_F(MyTest, log_and_format_correct) {
    FILE *fd;
    char text[64], buf[64];
    int a = 10;
    sprintf(text, "value of a is %d", a);
    plog(PLOG_INFO, "value of a is %d", a);
    fd = fopen(LOG_FILE, "r");
    ASSERT_NE(fd, nullptr) << "Failed to open file";
    ASSERT_EQ(strlen(text), fread(buf, sizeof(char), sizeof(buf), fd)) <<
              "Failed to read proper amount of data";
    //buf is not null terminated
    buf[strlen(text)] = '\0';
    ASSERT_STREQ(text, buf) << "Invalid value in file";
    fclose(fd);
}

/* Hard to compare system info content, because it is constantly changing */
TEST_F(MyTest, write_ok_sysinfo) {
    int return_val = write_sys_info();
    ASSERT_NE(return_val, -2) << "Failed to open /tmp/syslog";
    ASSERT_NE(return_val, -1)  << "Failed to obtain sysinfo";
    ASSERT_EQ(return_val, 0)  << "Some other error";
}

/* TODO: obtain and compare system info (e.g. Total ram)
 * written into the log file
 */
TEST_F(MyTest, write_ok_state) {
    int return_val = write_current_state(2);
    ASSERT_NE(return_val, -1)  << "Error obtaining logging information";
    ASSERT_EQ(return_val, 0)  << "Some other error";
}

TEST_F(MyTest, fail_write_state) {
    int return_val = write_current_state(0);
    ASSERT_NE(return_val, -1)  << "Error obtaining logging information";
    ASSERT_EQ(return_val, -2)  << "Status level is not checked";
}
