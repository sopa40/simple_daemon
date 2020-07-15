#include "gtest/gtest.h"
#include <iostream>

#define LOG_FILE "/tmp/log"
extern "C" {
#include "logging.h"
}

class MyTest : public testing::Test {
    void SetUp() {

    }

    void TearDown() {
        if(unlink(LOG_FILE))
            printf("tessssssssssst %s", strerror(errno));
        //ASSERT_EQ (unlink("/tmp/log"), 0) << "Failed  to delete log file";
    }
};

TEST_F(MyTest, log_correct) {
    FILE *fd;
    char buf[64];
    char *text = "test";
    plog(PLOG_INFO, text);
    fd = fopen(LOG_FILE, "r");
    ASSERT_NE(fd, nullptr) << "Failed to open file";
    ASSERT_EQ(strlen(text), fread(buf, sizeof(char), sizeof(buf), fd)) <<
              "Failed to read proper amount of data";
    ASSERT_STREQ(text, buf) << "Invalid value in file";
    fclose(fd);

}

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
