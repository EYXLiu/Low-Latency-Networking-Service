#include "project/admin_server.h"

int main() {
    AdminServer admin(5555);
    admin.start();
    return 0;
}